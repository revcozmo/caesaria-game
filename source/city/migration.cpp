// This file is part of CaesarIA.
//
// CaesarIA is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// CaesarIA is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with CaesarIA.  If not, see <http://www.gnu.org/licenses/>.

#include "migration.hpp"
#include "helper.hpp"
#include "core/safetycast.hpp"
#include "gfx/tilemap.hpp"
#include "walker/emigrant.hpp"
#include "core/position.hpp"
#include "objects/road.hpp"
#include "objects/house.hpp"
#include "gfx/tile.hpp"
#include "funds.hpp"
#include "objects/constants.hpp"
#include "game/settings.hpp"
#include "world/empire.hpp"
#include "game/gamedate.hpp"
#include "statistic.hpp"
#include "cityservice_info.hpp"

using namespace constants;

class CityMigration::Impl
{
public:
  PlayerCityPtr city;
  int lastMonthPopulation;
  int lastMonthMigration;
  DateTime lastUpdate;

  float getMigrationKoeff()
  {
    return ( std::min<float>( city->getPopulation(), 150 ) / 150.f );
  }

  CityServiceInfo::Parameters getLastParams()
  {
    SmartPtr<CityServiceInfo> info = ptr_cast<CityServiceInfo>( city->findService( CityServiceInfo::getDefaultName() ) );

    CityServiceInfo::Parameters params;
    if( info.isValid() )
    {
      params = info->getLast();
    }

    return params;
  }

  void createMigrationToCity();
  void createMigrationFromCity();
  unsigned int calcVacantHouse();
};

CityServicePtr CityMigration::create(PlayerCityPtr city )
{
  CityServicePtr ret( new CityMigration( city ) );
  ret->drop();

  return ret;
}

CityMigration::CityMigration( PlayerCityPtr city )
: CityService( getDefaultName() ), _d( new Impl )
{
  _d->city = city;
  _d->lastMonthMigration = 0;
  _d->lastMonthPopulation = 0;
  _d->lastUpdate = GameDate::current();
}

void CityMigration::update( const unsigned int time )
{
  if( time % 50 != 1 )
    return;

  float migrationKoeff = _d->getMigrationKoeff();
  CityServiceInfo::Parameters params = _d->getLastParams();

  int worklessCitizenAway = GameSettings::get( GameSettings::worklessCitizenAway );
  int emigrantsIndesirability = 50; //base indesirability value
  float emDesKoeff = math::clamp<float>( (float)GameSettings::get( GameSettings::emigrantSalaryKoeff ), 1.f, 99.f );

  //if salary in city more then empire people more effectivelly go to our city
  int diffSalary = _d->city->getEmpire()->getWorkersSalary() - _d->city->getFunds().getWorkerSalary();
  emigrantsIndesirability += diffSalary * emDesKoeff;

  //emigrant like when lot of food stock int city
  int minMonthWithFood = GameSettings::get( GameSettings::minMonthWithFood );
  emigrantsIndesirability += ( params.monthWithFood < minMonthWithFood
                               ? ((minMonthWithFood - params.monthWithFood) * 3)
                               : -params.monthWithFood );
  //emigrant need workplaces
  emigrantsIndesirability += params.workless == 0
                              ? -10
                              : (params.workless * (params.workless < worklessCitizenAway ? 1 : 2));

  emigrantsIndesirability *= migrationKoeff;

  int maxIndesirability = 100;
  int goddesRandom = rand() % maxIndesirability;
  if( goddesRandom > emigrantsIndesirability )
  {
    _d->createMigrationToCity();
  }

  if( _d->lastUpdate.getMonthToDate( GameDate::current() ) > 0 )
  {
    _d->lastUpdate = GameDate::current();
    _d->lastMonthMigration = _d->city->getPopulation() - _d->lastMonthPopulation;
    _d->lastMonthPopulation = _d->city->getPopulation();

    if( params.workless * migrationKoeff > worklessCitizenAway
        || emigrantsIndesirability > maxIndesirability )
    {
      _d->createMigrationFromCity();
    }
  }
}

std::string CityMigration::getReason() const
{
  unsigned int vacantHouse = _d->calcVacantHouse();
  if( vacantHouse == 0 )
    return "##migration_lack_empty_house##";

  if( _d->getMigrationKoeff() > 0.99f )
  {
    CityServiceInfo::Parameters params = _d->getLastParams();
    if( params.monthWithFood < (int)GameSettings::get( GameSettings::minMonthWithFood ) )
      return "##migration_lessfood_granary##";
    if( params.monthWithFood == 0 )
      return "##migration_empty_granary##";
    if( params.workless > 5 )
      return "##migration_lack_jobs##";
    if( params.workless > (int)GameSettings::get( GameSettings::worklessCitizenAway ) )
      return "##migration_people_away##";
  }
  return "##migration_peoples_arrived_in_city##";
}

std::string CityMigration::getDefaultName()
{
  return "migration";
}

VariantMap CityMigration::save() const
{
  VariantMap ret;

  ret[ "lastUpdate" ] = _d->lastUpdate;
  ret[ "lastMonthMigration" ] = _d->lastMonthMigration;
  ret[ "lastMonthPopulation" ] = _d->lastMonthPopulation;

  return ret;
}

void CityMigration::load(const VariantMap& stream)
{
  _d->lastUpdate = stream.get( "lastUpdate", GameDate::current() ).toDateTime();
  _d->lastMonthMigration = stream.get( "lastMonthMigration", 0 );
  _d->lastMonthPopulation = stream.get( "lastMonthPopulation", 0 );
}

unsigned int CityMigration::Impl::calcVacantHouse()
{
  unsigned int vh = 0;
  CityHelper helper( city );
  HouseList houses = helper.find<House>(building::house);
  foreach( house, houses )
  {
    if( (*house)->getAccessRoads().size() > 0 )
    {
      vh += math::clamp<int>( (*house)->getMaxHabitants() - (*house)->getHabitants().count(), 0, 0xff );
    }
  }

  return vh;
}

void CityMigration::Impl::createMigrationToCity()
{
  unsigned int vh = calcVacantHouse();
  if( vh == 0 )
  {
    return;
  }

  WalkerList walkers = city->getWalkers( walker::emigrant );

  if( vh <= walkers.size() * 5 )
  {
    return;
  }

  Tile& roadTile = city->getTilemap().at( city->getBorderInfo().roadEntry );

  EmigrantPtr emigrant = Emigrant::create( city );

  if( emigrant.isValid() )
  {
    emigrant->send2city( roadTile );
  }
}

void CityMigration::Impl::createMigrationFromCity()
{
  CityHelper helper( city );
  HouseList houses = helper.find<House>(building::house);
  const int minWorkersNumber = 4;
  for( HouseList::iterator i=houses.begin(); i != houses.end(); )
  {
    int houseWorkless = (*i)->getServiceValue( Service::recruter );
    if( !(*i)->getEnterArea().empty() && houseWorkless > 0 ) { i++; }
    else { i = houses.erase( i ); }
  }

  if( !houses.empty() )
  {
    int stepNumber = std::max<int>( rand() % houses.size(), 1 );
    for( int i=0; i < stepNumber; i++ )
    {
      HouseList::iterator house = houses.begin();
      std::advance( house, rand() % houses.size() );

      EmigrantPtr emigrant = Emigrant::create( city );

      if( emigrant.isValid() )
      {
        (*house)->remHabitants( minWorkersNumber );
        emigrant->leaveCity( *(*house)->getEnterArea().front() );
      }
    }
  }
}
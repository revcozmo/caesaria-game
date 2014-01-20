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

#include "religion.hpp"
#include "game/divinity.hpp"
#include "city/helper.hpp"
#include "game/resourcegroup.hpp"
#include "core/position.hpp"
#include "constants.hpp"
#include "city/statistic.hpp"

using namespace constants;

class Temple::Impl
{
public:
  RomeDivinityPtr divinity;
};

Temple::Temple( RomeDivinityPtr divinity, TileOverlay::Type type, int imgId, const Size& size )
: ServiceBuilding( divinity->getServiceType(), type, size ), _td( new Impl )
{
  _td->divinity = divinity;
  setPicture( ResourceGroup::security, imgId );
}

RomeDivinityPtr Temple::getDivinity() const
{
  return _td->divinity;
}

void Temple::deliverService()
{
  if( getWalkers().empty() && getWorkersCount() > 0 )
  {
    ServiceBuilding::deliverService();
  }
}

unsigned int Temple::getWalkerDistance() const
{
  return 26;
}

Temple::~Temple()
{

}

TempleCeres::TempleCeres() : SmallTemple( DivinePantheon::ceres(), building::templeCeres, 45 )
{
}

BigTempleCeres::BigTempleCeres() : BigTemple( DivinePantheon::ceres(), building::cathedralCeres, 46 )
{
}

TempleNeptune::TempleNeptune() : SmallTemple( DivinePantheon::neptune(), building::templeNeptune, 47 )
{
}

BigTempleNeptune::BigTempleNeptune() : BigTemple( DivinePantheon::neptune(), building::cathedralNeptune, 48 )
{
}

TempleMars::TempleMars() : SmallTemple( DivinePantheon::mars(), building::templeMars, 51 )
{
}

BigTempleMars::BigTempleMars() : BigTemple( DivinePantheon::mars(), building::cathedralMars, 52 )
{
}

TempleVenus::TempleVenus() : SmallTemple( DivinePantheon::venus(), building::templeVenus, 53 )
{
}

BigTempleVenus::BigTempleVenus() : BigTemple( DivinePantheon::venus(), building::cathedralVenus, 54 )
{
}

TempleMercure::TempleMercure() : SmallTemple( DivinePantheon::mercury(), building::templeMercury, 49 )
{
}

BigTempleMercure::BigTempleMercure() : BigTemple( DivinePantheon::mercury(), building::cathedralMercury, 50 )
{
}

unsigned int BigTempleMercure::getParishionerNumber() const
{
  return 300;
}

TempleOracle::TempleOracle() : BigTemple( RomeDivinityPtr(), building::oracle, 55 )
{
  _animationRef().load( ResourceGroup::security, 56, 6);
  _animationRef().setOffset( Point( 9, 30 ) );
  _fgPicturesRef().resize(1);
}

unsigned int TempleOracle::getParishionerNumber() const
{
  return 500;
}

void TempleOracle::build(PlayerCityPtr city, const TilePos& pos)
{
  BigTemple::build( city, pos );

  if( isDeleted() )
  {
    _setError( "##oracle_need_2_cart_marble##" );
  }
}

SmallTemple::SmallTemple( RomeDivinityPtr divinity, TileOverlay::Type type, int imgId )
  : Temple( divinity, type, imgId, Size(2) )
{
  setMaxWorkers( 2 );
}

unsigned int SmallTemple::getParishionerNumber() const
{
  return 150;
}

BigTemple::BigTemple( RomeDivinityPtr divinity, TileOverlay::Type type, int imgId )
  : Temple( divinity, type, imgId, Size(3) )
{
  setMaxWorkers( 8 );
}

unsigned int BigTemple::getParishionerNumber() const
{
  return 300;
}

void BigTemple::build(PlayerCityPtr city, const TilePos& pos)
{
  CityStatistic::GoodsMap goods = CityStatistic::getGoodsMap( city );
  if( goods[ Good::marble ] >= 2 )
  {
    Temple::build( city, pos );
  }
  else
  {
    _setError( "##need_marble_for_large_temple##" );
    deleteLater();
  }
}

// This file is part of openCaesar3.
//
// openCaesar3 is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// openCaesar3 is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with openCaesar3.  If not, see <http://www.gnu.org/licenses/>.


#include "amphitheater.hpp"
#include "core/position.hpp"
#include "game/resourcegroup.hpp"
#include "core/foreach.hpp"
#include "game/city.hpp"
#include "training.hpp"
#include "core/gettext.hpp"
#include "core/stringhelper.hpp"
#include "core/logger.hpp"
#include "building/constants.hpp"

using namespace constants;


Amphitheater::Amphitheater() : EntertainmentBuilding(Service::amphitheater, building::amphitheater, Size(3))
{
  _fgPicturesRef().resize(2);
}

void Amphitheater::timeStep(const unsigned long time)
{
  EntertainmentBuilding::timeStep( time );  
}

void Amphitheater::build(PlayerCityPtr city, const TilePos& pos)
{
  EntertainmentBuilding::build( city, pos );

  CityHelper helper( city );
  ActorColonyList actors = helper.find<ActorColony>( building::actorColony );
  if( actors.empty() )
  {
    _setError( _("##need_actor_colony##") );
  }

  GladiatorSchoolList gladiators = helper.find<GladiatorSchool>( building::gladiatorSchool );
  if( gladiators.empty() )
  {
    _setError( _("##need_gladiator_school##") );
  }
}

void Amphitheater::deliverService()
{
  EntertainmentBuilding::deliverService();

  _fgPicturesRef().at(0) = _animationRef().isRunning()
                         ? Picture::load( ResourceGroup::entertaiment, 12 )
                         : Picture::getInvalid();

  if( _animationRef().isRunning())
  {
    _fgPicturesRef().front() = Picture::load( ResourceGroup::entertaiment, 12 );
  }
  else
  {
    _fgPicturesRef().front() = Picture::getInvalid();
    _fgPicturesRef().back() = Picture::getInvalid();
  }
}
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
//
// Copyright 2012-2014 Dalerank, dalerankn8@gmail.com

#include "prefecture.hpp"
#include "gfx/picture.hpp"
#include "game/resourcegroup.hpp"
#include "core/position.hpp"
#include "walker/prefect.hpp"
#include "pathway/pathway_helper.hpp"
#include "gfx/tile.hpp"
#include "pathway/path_finding.hpp"
#include "gfx/tilemap.hpp"
#include "city/helper.hpp"
#include "city/cityservice_fire.hpp"
#include "objects/constants.hpp"
#include "gfx/helper.hpp"
#include "objects_factory.hpp"

using namespace gfx;

REGISTER_CLASS_IN_OVERLAYFACTORY(object::prefecture, Prefecture)

class Prefecture::Impl
{
public:
  TilePos fireDetect;

public:
  TilePos checkFireDetect(PlayerCityPtr city , const TilePos& pos);
};

Prefecture::Prefecture()
  : ServiceBuilding(Service::prefect, object::prefecture, Size(1)),
    _d( new Impl )
{
  _d->fireDetect = gfx::tilemap::invalidLocation();

  setPicture( MetaDataHolder::randomPicture( type(), size() ) );
  _fgPictures().resize(1);
}

Prefecture::~Prefecture() {}

void Prefecture::timeStep(const unsigned long time)
{
  ServiceBuilding::timeStep( time );
}

void Prefecture::deliverService()
{
  if( !isActive() )
    return;

  if( numberWorkers() > 0 && walkers().size() == 0 )
  {
    TilePos fireDetectPos = _d->checkFireDetect( _city(), pos() );
    bool fireDetect = fireDetectPos.i() >= 0;
    PrefectPtr prefect = Prefect::create( _city() );
    prefect->setMaxDistance( walkerDistance() );

    if( fireDetect )
    {
      TilePos startPos = roadside().front()->pos();

      OverlayPtr ruin = _city()->getOverlay( _d->fireDetect );
      Pathway pathway = PathwayHelper::create( startPos, ruin, PathwayHelper::allTerrain );

      bool fireInOutWorkArea = pathway.length() <= walkerDistance();
      if( pathway.isValid() && fireInOutWorkArea )
      {
        pathway.setNextTile( ruin->tile() );
        prefect->setPos( pathway.startPos() );
        prefect->setBase( this );
        prefect->setPathway( pathway );
        prefect->go();
      }
      else
      {
        fireDetect = false;
      }

      _d->fireDetect = gfx::tilemap::invalidLocation();
    }
    
    prefect->send2City( this, Prefect::patrol, fireDetect ? 1000 : 0 );

    addWalker( prefect.object() );
  }
}

unsigned int Prefecture::walkerDistance() const { return 26; }
void Prefecture::fireDetect( const TilePos& pos ){ _d->fireDetect = pos; }

TilePos Prefecture::Impl::checkFireDetect( PlayerCityPtr city, const TilePos& pos )
{
  if( fireDetect.i() >= 0 )
    return fireDetect;

  city::FirePtr fire;
  fire << city->findService( city::Fire::defaultName() );

  fireDetect = gfx::tilemap::invalidLocation();
  if( city.isValid() )
  {
    int minDistance = 9999;
    foreach ( it, fire->locations() )
    {
      int currentDistance = pos.distanceFrom( *it );
      if( currentDistance < minDistance )
      {
        minDistance = currentDistance;
        fireDetect = *it;
      }
    }
  }

  return fireDetect;
}

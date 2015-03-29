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

#ifndef __CAESARIA_MARKET_LADY_H_INCLUDED__
#define __CAESARIA_MARKET_LADY_H_INCLUDED__

#include "serviceman.hpp"
#include "core/predefinitions.hpp"

class MarketLady : public ServiceWalker
{
public:
  static ServiceWalkerPtr create( PlayerCityPtr city  );
  MarketPtr market() const;

protected:
  virtual void _updateThoughts();
  virtual void _centerTile();

private:
  MarketLady( PlayerCityPtr city );
};

#endif //__CAESARIA_MARKET_LADY_H_INCLUDED__

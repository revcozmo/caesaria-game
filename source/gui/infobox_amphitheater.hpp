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

#ifndef _CAESARIA_INFOBOX_AMPHITHEATER_H_INCLUDE_
#define _CAESARIA_INFOBOX_AMPHITHEATER_H_INCLUDE_

#include "infobox_construction.hpp"

namespace gui
{

namespace infobox
{

class AboutAmphitheater : public AboutConstruction
{
public:
  AboutAmphitheater( Widget* parent, PlayerCityPtr city, const gfx::Tile& tile );
  virtual ~AboutAmphitheater();

private:
  void _updateWarnings();
  void _updateShowsInfo(AmphitheaterPtr amph);
  void _updateBouthsInfo(AmphitheaterPtr amph);
};

}

}//end namespace gui
#endif //_CAESARIA_INFOBOX_AMPHITHEATER_H_INCLUDE_

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

#ifndef _CAESARIA_EVENT_CHANGEBUILDINGOPTIONS_H_INCLUDE_
#define _CAESARIA_EVENT_CHANGEBUILDINGOPTIONS_H_INCLUDE_

#include "event.hpp"
#include "core/variant_map.hpp"

namespace events
{

class ChangeBuildingOptions : public GameEvent
{
public:
  static GameEventPtr create();

  virtual void load(const VariantMap& );
  virtual VariantMap save() const;

protected:
  virtual void _exec( Game& game, unsigned int );
  virtual bool _mayExec(Game &game, unsigned int time) const;

private:
  VariantMap _vars;
};

}

#endif //_CAESARIA_EVENT_CHANGEBUILDINGOPTIONS_H_INCLUDE_

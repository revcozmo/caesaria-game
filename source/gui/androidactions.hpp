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

#ifndef _CAESARIA_ANDROIDACTIONSBAR_H_INCLUDE_
#define _CAESARIA_ANDROIDACTIONSBAR_H_INCLUDE_

#include "widget.hpp"
#include "core/signals.hpp"

namespace gui
{

class AndroidActionsBar : public Widget
{
public:
  virtual void beforeDraw( gfx::Engine& painter);
  AndroidActionsBar( Widget* parent );

public oc3_signals:
  Signal0<>& onRequestTileHelp();
  Signal0<>& onEscapeClicked();
  Signal0<>& onRequestMenu();

private:
  class Impl;
  ScopedPtr<Impl> _d;
};

}//end namesapce gui

#endif //_CAESARIA_ANDROIDACTIONSBAR_H_INCLUDE_
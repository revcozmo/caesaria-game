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

#ifndef __CAESARIA_PUSHBUTTON_H_INCLUDE_
#define __CAESARIA_PUSHBUTTON_H_INCLUDE_

#include "widget.hpp"
#include "core/signals.hpp"
#include "gfx/picture.hpp"

class Font;
namespace gui
{

//Usual button class
class PushButton : public Widget
{
public:
  typedef enum { grayBorderLine, smallGrayBorderLine, whiteBorderUp,
                 blackBorderUp, noBackground } BackgroundStyle;

	PushButton( Widget* );

	//! constructor
	PushButton( Widget* parent,
			        const Rect& rectangle,
              const std::string& caption="",
              int id=-1, 
			        bool noclip=false,
              const BackgroundStyle bgtyle=grayBorderLine);

	//! destructor
	virtual ~PushButton();

	//! called if an event happened.
	virtual bool onEvent(const NEvent& event);

  //! prepare render state
  virtual void beforeDraw( GfxEngine& painter );

  //! override render function 
  virtual void draw( GfxEngine& painter );

  virtual void setText(const std::string& text );

  virtual bool isPushButton() const;

  virtual void setPressed( bool pressed );

  virtual bool isPressed() const;

  virtual bool isBodyVisible() const;

  virtual void drawIcon( GfxEngine& painter );

  virtual void setPicture( const std::string& rcname, int index);
  virtual void setPicture( Picture picture, ElementState state );
  virtual void setPicture( const std::string& rcname, int index, ElementState state );

  virtual void setIcon( const std::string& rcname, int index, ElementState state );
  virtual void setIcon( const std::string& rcname, int index );
  virtual void setIconOffset( Point offset );

  virtual void setBackgroundStyle( const BackgroundStyle style );

  virtual void setFont( const Font& font, ElementState state );

  virtual void setFont( const Font& font );

  virtual Font& getFont( ElementState state );

  virtual void setIsPushButton( bool value );

  virtual void setupUI(const VariantMap &ui);

  virtual void setTextOffset( const Point& offset );

oc3_signals public:
  virtual Signal0<>& onClicked(); 

protected:

  virtual void _resizeEvent();

  //! when left mouse button pressed down
	virtual bool _leftMouseBtnPressed( const NEvent& event );

  //! when left mouse button left up
	virtual bool _btnMouseUp( const NEvent& event );

  //! when some mouse button clicked
	virtual void _btnClicked();

	virtual ElementState _getActiveButtonState();
 
  virtual void _updateTexture( ElementState state );

  PictureRef& _getBackground( ElementState state );
  PictureRef& _getTextPicture( ElementState state );

private:
  class Impl;
  ScopedPtr< Impl > _d;
};

}//end namespace gui
#endif //__CAESARIA_PUSHBUTTON_H_INCLUDE_

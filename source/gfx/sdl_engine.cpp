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
//
// Copyright 2012-2013 Gregoire Athanase, gathanase@gmail.com


#include "sdl_engine.hpp"

#include <cstdlib>
#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <SDL.h>
#include <SDL_ttf.h>

#include "IMG_savepng.h"
#include "core/exception.hpp"
#include "core/requirements.hpp"
#include "core/position.hpp"
#include "pictureconverter.hpp"
#include "core/time.hpp"
#include "core/logger.hpp"
#include "core/stringhelper.hpp"
#include "core/font.hpp"
#include "core/eventconverter.hpp"

#ifdef CAESARIA_PLATFORM_MACOSX
#include <dlfcn.h>
#endif

class GfxSdlEngine::Impl
{
public:
  Picture screen;
  Picture maskedPic;
  
  int rmask, gmask, bmask, amask;
  unsigned int fps, lastFps;
  unsigned int lastUpdateFps;
  Font debugFont;
  bool showDebugInfo;
};


Picture& GfxSdlEngine::getScreen()
{
  return _d->screen;
}

GfxSdlEngine::GfxSdlEngine() : GfxEngine(), _d( new Impl )
{
  resetTileDrawMask();
}

GfxSdlEngine::~GfxSdlEngine()
{
}

void GfxSdlEngine::deletePicture( Picture* pic )
{
  if( pic )
    unloadPicture( *pic );
}

void GfxSdlEngine::init()
{
  _d->lastUpdateFps = DateTime::getElapsedTime();
  _d->fps = 0;
  _d->showDebugInfo = false;

  Logger::warning( "GrafixEngine: init");
  int rc = SDL_Init(SDL_INIT_VIDEO);
  if (rc != 0)
  {
    Logger::warning( StringHelper::format( 0xff, "Unable to initialize SDL: %d", SDL_GetError() ) );
    THROW("Unable to initialize SDL: " << SDL_GetError());
  }
  
  Logger::warning( "GrafixEngine: ttf init");
  rc = TTF_Init();
  if (rc != 0)
  {
    THROW("Unable to initialize SDL: " << SDL_GetError());
  }

  unsigned int flags = SDL_DOUBLEBUF | SDL_SWSURFACE;
  flags |= (getFlag( GfxEngine::fullscreen ) > 0 ? SDL_FULLSCREEN : 0);
    
#ifdef CAESARIA_PLATFORM_MACOSX
    void* cocoa_lib;
    cocoa_lib = dlopen( "/System/Library/Frameworks/Cocoa.framework/Cocoa", RTLD_LAZY );
    void (*nsappload)(void);
    nsappload = (void(*)()) dlsym(	cocoa_lib, "NSApplicationLoad");
    nsappload();
#endif
 
  Logger::warning( StringHelper::format( 0xff, "GrafixEngine: set mode %dx%d",  _srcSize.getWidth(), _srcSize.getHeight() ) );
  SDL_Surface* scr = SDL_SetVideoMode(_srcSize.getWidth(), _srcSize.getHeight(), 32, flags );  // 32bpp
    
  Logger::warning( "GrafixEngine: init successfull");
  _d->screen.init( scr, Point( 0, 0 ) );
  
  if( !_d->screen.isValid() ) 
  {
    THROW("Unable to set video mode: " << SDL_GetError());
  }

  Logger::warning( "GrafixEngine: set caption");
  SDL_WM_SetCaption( "CaesarIA: "CAESARIA_VERSION, 0 );

  SDL_EnableKeyRepeat(1, 100);
}


void GfxSdlEngine::exit()
{
  TTF_Quit();
  SDL_Quit();
}

/* Convert picture to SDL surface and then put surface into Picture class
 */

void GfxSdlEngine::loadPicture( Picture& ioPicture )
{
  // convert pixel format
  SDL_Surface* newImage = SDL_DisplayFormatAlpha( ioPicture.getSurface() );
  
  if( newImage == NULL ) 
  {
    THROW("Cannot convert surface, maybe out of memory");
  }
  SDL_FreeSurface(ioPicture.getSurface());

  ioPicture.init( newImage, ioPicture.getOffset() );
}

void GfxSdlEngine::unloadPicture( Picture& ioPicture )
{
  SDL_FreeSurface( ioPicture.getSurface() );
  ioPicture = Picture();
}

void GfxSdlEngine::startRenderFrame()
{
  SDL_FillRect( _d->screen.getSurface(), NULL, 0 );  // black background for a complete redraw
}

void GfxSdlEngine::endRenderFrame()
{
  if( _d->showDebugInfo )
  {
    std::string debugText = StringHelper::format( 0xff, "fps: %d", _d->lastFps );
    _d->debugFont.draw( _d->screen, debugText, 4, 22, false );
  }

  SDL_Flip( _d->screen.getSurface() ); //Refresh the screen
  _d->fps++;

  if( DateTime::getElapsedTime() - _d->lastUpdateFps > 1000 )
  {
    _d->lastUpdateFps = DateTime::getElapsedTime();
    _d->lastFps = _d->fps;
    _d->fps = 0;
  }
}

void GfxSdlEngine::drawPicture(const Picture &picture, const int dx, const int dy, Rect* clipRect )
{
  if( !picture.isValid() )
      return;

  if( clipRect != 0 )
  {
    SDL_Rect r = { (short)clipRect->left(), (short)clipRect->top(), (Uint16)clipRect->getWidth(), (Uint16)clipRect->getHeight() };
    SDL_SetClipRect( _d->screen.getSurface(), &r );
  }

  if( _d->rmask || _d->gmask || _d->bmask  )
  {
    PictureConverter::maskColor( _d->maskedPic, picture, _d->rmask, _d->gmask, _d->bmask, _d->amask );

    _d->screen.draw( _d->maskedPic, dx, dy );
  }
  else
  {
    _d->screen.draw( picture, dx, dy );
  }

  if( clipRect != 0 )
  {
    SDL_SetClipRect( _d->screen.getSurface(), 0 );
  }
}

void GfxSdlEngine::drawPicture( const Picture &picture, const Point& pos, Rect* clipRect )
{
  drawPicture( picture, pos.x(), pos.y(), clipRect );
}

void GfxSdlEngine::setTileDrawMask( int rmask, int gmask, int bmask, int amask )
{
  _d->rmask = rmask;
  _d->gmask = gmask;
  _d->bmask = bmask;
  _d->amask = amask;
}

void GfxSdlEngine::resetTileDrawMask()
{
  _d->rmask = _d->gmask = _d->bmask = _d->amask = 0;
}

Picture* GfxSdlEngine::createPicture(const Size& size )
{
  SDL_Surface* img = SDL_CreateRGBSurface( 0, size.getWidth(), size.getHeight(), 32,
                                           0, 0, 0, 0 );

  Logger::warningIf( NULL == img, StringHelper::format( 0xff, "Cannot make surface, size=%dx%d", size.getWidth(), size.getHeight() ) );

  Picture *pic = new Picture();
  pic->init(img, Point( 0, 0 ));  // no offset
  
  return pic;
}

void GfxSdlEngine::createScreenshot( const std::string& filename )
{
  IMG_SavePNG( filename.c_str(), _d->screen.getSurface(), -1 );
}

GfxEngine::Modes GfxSdlEngine::getAvailableModes() const
{
  Modes ret;

  /* Get available fullscreen/hardware modes */
  SDL_Rect** modes = SDL_ListModes(NULL, SDL_FULLSCREEN|SDL_HWSURFACE);

  for(int i=0; modes[i]; ++i)
  {
    ret.push_back( Size( modes[i]->w, modes[i]->h) );
  }

  return ret;
}

unsigned int GfxSdlEngine::getFps() const
{
  return _d->fps;
}

void GfxSdlEngine::setFlag( int flag, int value )
{
  GfxEngine::setFlag( flag, value );

  if( flag == debugInfo )
  {
    _d->showDebugInfo = value > 0;
    _d->debugFont = Font::create( FONT_2 );
  }
}

void GfxSdlEngine::delay( const unsigned int msec )
{
  SDL_Delay( msec );
}

bool GfxSdlEngine::haveEvent( NEvent& event )
{
  SDL_Event sdlEvent;

  if( SDL_PollEvent(&sdlEvent) )
  {
    event = EventConverter::instance().get( sdlEvent );
    return true;
  }

  return false;
}

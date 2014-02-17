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

#ifndef __CAESARIA_CITYSERVICE_FACTORY_H_INCLUDED__
#define __CAESARIA_CITYSERVICE_FACTORY_H_INCLUDED__

#include "cityservice.hpp"
#include "core/scopedptr.hpp"
#include "city/city.hpp"

class CityServiceCreator : public ReferenceCounted
{
public:
  virtual CityServicePtr create( PlayerCityPtr city ) = 0;
  virtual std::string getServiceName() const = 0;
};

typedef SmartPtr<CityServiceCreator> CityServiceCreatorPtr;

template< class T >
class BaseServiceCreator : public CityServiceCreator
{
public:
  virtual CityServicePtr create( PlayerCityPtr city )
  {
    CityServicePtr ret( T::create( city ) );
    return ret;
  }

  virtual std::string getServiceName() const { return T::getDefaultName(); }
};


class CityServiceFactory
{
public:
  static CityServicePtr create( const std::string& name, PlayerCityPtr city);
  static CityServiceFactory& instance();
  void addCreator( CityServiceCreatorPtr creator );

  template<class T>
  void addCreator()
  {
    CityServiceCreatorPtr ret( new BaseServiceCreator<T>() );
    addCreator( ret );
  }

private:
  CityServiceFactory();

  class Impl;
  ScopedPtr<Impl> _d;
};

#endif//__CAESARIA_CITYSERVICE_FACTORY_H_INCLUDED__

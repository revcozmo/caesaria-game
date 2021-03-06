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

#ifndef __CAESARIA_PANTHEON_H_INCLUDED__
#define __CAESARIA_PANTHEON_H_INCLUDED__

#include "core/referencecounted.hpp"
#include "core/scopedptr.hpp"
#include "game/predefinitions.hpp"
#include "vfs/path.hpp"
#include "romedivinity.hpp"
#include "core/variant.hpp"

namespace religion
{

namespace rome
{

class Pantheon
{
public:
  static Pantheon& instance();

  void load(const VariantMap& stream );
  void save(VariantMap& stream);

  static void doFestival( RomeDivinityType who, int type );
  static void doFestival( const std::string& who, int type );

  DivinityList all();
  static DivinityPtr ceres();
  static DivinityPtr mars();
  static DivinityPtr neptune();
  static DivinityPtr venus();
  static DivinityPtr mercury();

  static DivinityPtr get( RomeDivinityType name );
  static DivinityPtr get( const std::string& name );

private:
  Pantheon();

  class Impl;
  ScopedPtr< Impl > _d;
};

}//end namespace rome

}//end namespace religion

#endif //__CAESARIA_PANTHEON_H_INCLUDED__

#ifndef __variable_h__
#define __variable_h__

#include "type.h"
#include "ioclass.h"

#include <string>
#include <map>
#include <iostream>

class Variable
{
public:
  Variable( std::string name_, Type& type_, IOClass& ioclass_ )
      : mName( name_ ), mType( type_ ), mIOClass( ioclass_ )
  {
  }
  const std::string& name()
  {
    return mName;
  }
  bool isSigned()
  {
    return mType.isSigned();
  }
  int width()
  {
    return mType.width();
  }
  friend std::ostream& operator<<(std::ostream& out, Variable& a)
  {
#if 0
    switch (a.mIOClass.mID)
    {
      case IOClass::INPUT:
        break;
      case IOClass::OUTPUT:
        break;
      case IOClass::WIRE:
        break;
      case IOClass::REGISTER:
        break;
      default:
        throw;
    }
#endif
    out << a.mIOClass.name();
    if (a.width() > 1)
      out << " [" << a.width()-1 << ":0]";
    out << a.name() << ";";
    return out;
  }
private:
  std::string mName;
  Type& mType;
  IOClass& mIOClass;
};

class Variables
{
public:
  static Variables& instance()
  {
    static Variables variables;
    return variables;
  }
  bool isVariable( std::string variable )
  {
    return mVariables.find( variable ) != mVariables.end();
  }
  Variable& getVariable( std::string variable )
  {
    variables_t::iterator i = mVariables.find( variable );
    if( i == mVariables.end() )
      throw;
    return i->second;
  }
  Variable& addVariable( std::string variable, Type& type, IOClass& ioclass )
  {
    variables_t::iterator i = mVariables.find( variable );
    if( i != mVariables.end() )
      throw;
    Variable* pV = new Variable( variable,type,ioclass );
    mVariables.insert( pair_t( variable, *pV) );
    return *pV;
  }
private:
  Variables()
  {
  }

  typedef std::pair< std::string, Variable > pair_t;
  typedef std::map< std::string, Variable > variables_t;

  variables_t mVariables;
};

#endif//__variable_h__

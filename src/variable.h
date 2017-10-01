#ifndef __variable_h__
#define __variable_h__

#include "type.h"
#include "ioclass.h"
#include <string>
#include <map>

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
  void addVariable( std::string variable, Type& type, IOClass& ioclass )
  {
    variables_t::iterator i = mVariables.find( variable );
    if( i != mVariables.end() )
      throw;
    mVariables.insert( pair_t( variable,Variable( variable,type,ioclass ) ) );
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

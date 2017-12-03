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
  // constructor
  Variable( std::string name_, Type& type_, IOClass& ioclass_ )
      : mName( name_ ), mType( type_ ), mIOClass( ioclass_ )
  {
  }
  
  // get the variable name
   std::string& name()
  {
    return mName;
  }
  
  // return whether the variable is signed
  bool isSigned()
  {
    return mType.isSigned();
  }
  
  // return the bit width of the variable
  int width()
  {
    return mType.width();
  }
  
  // return the variable IO class ID (see enumerations in ioclass.h)
  IOClass::ID ioClass()
  {
    return mIOClass.id();
  }
  
  // stream overload to output the variable declaration in Verilog
  friend std::ostream& operator<<(std::ostream& out, Variable& a)
  {
    // kludge fix for register
    if( a.mIOClass.id() == IOClass::REGISTER )
      out << "reg";
    else
      out << a.mIOClass.name();
    if (a.width() > 1)
      out << " [" << a.width()-1 << ":0]";
    out << " " << a.name() << ";";
    return out;
  }
  
private:
  std::string mName;
  Type& mType;
  IOClass& mIOClass;
};

class Variables
{
private:
  typedef std::pair< std::string, Variable > pair_t;
  typedef std::map< std::string, Variable > variables_t;

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
  Variable& addVariable( Variable& variable )
  {
    variables_t::iterator i = mVariables.find( variable.name() );
    if( i != mVariables.end() )
      throw;
    mVariables.insert( pair_t( variable.name(), variable) );
    return variable;
  }
  typedef variables_t::iterator iterator_t;
  iterator_t begin(){return mVariables.begin();}
  iterator_t end(){return mVariables.end();}
  Variable&  variable(iterator_t i){return i->second;}
  
private:
  Variables()
  {
  }

  variables_t mVariables;
};

#endif//__variable_h__

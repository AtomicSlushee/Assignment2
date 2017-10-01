#ifndef __ioclass_h__
#define __ioclass_h__

#include <string>
#include <map>

class IOClass
{
public:
  enum ID
  {
    INPUT,
    OUTPUT,
    WIRE,
    REGISTER
  };
  IOClass( const std::string name_, ID id )
      : mName( name_ ), mID( id )
  {
  }
  const std::string& name()
  {
    return mName;
  }
private:
  std::string mName;
  ID mID;
};

class IOClasses
{
public:
  static IOClasses& instance()
  {
    static IOClasses ioclasses;
    return ioclasses;
  }
  bool isIOClass(const std::string ioclass)
  {
    return mIOClasses.find(ioclass) != mIOClasses.end();
  }
  IOClass& getIOClass(const std::string ioclass)
  {
    ioclasses_t::iterator i = mIOClasses.find(ioclass);
    if(i == mIOClasses.end())
      throw;
    return i->second;
  }
private:
  IOClasses()
  {
    static IOClass t_input( "input", IOClass::INPUT );
    static IOClass t_output( "output", IOClass::OUTPUT );
    static IOClass t_wire("wire", IOClass::WIRE);
    static IOClass t_register("register", IOClass::REGISTER);
    mIOClasses.insert(pair_t(t_input.name(),t_input));
    mIOClasses.insert(pair_t(t_output.name(),t_output));
    mIOClasses.insert(pair_t(t_wire.name(),t_wire));
    mIOClasses.insert(pair_t(t_register.name(),t_register));
  }
  
  typedef std::pair< std::string, IOClass > pair_t;
  typedef std::map< std::string, IOClass > ioclasses_t;
  
  ioclasses_t mIOClasses;
};

#endif//__ioclass_h__

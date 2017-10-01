#ifndef __type_h__
#define __type_h__

#include <string>
#include <map>

class Type
{
public:
  Type( const std::string name_, int width_, bool signed_ )
      : mName( name_ ), mWidth( width_ ), mSigned( signed_ )
  {
  }
  const std::string& name()
  {
    return mName;
  }
  int width()
  {
    return mWidth;
  }
  bool isSigned()
  {
    return mSigned;
  }
private:
  std::string mName;
  int mWidth;
  bool mSigned;
};

class Types
{
public:
  static Types& instance()
  {
    static Types types;
    return types;
  }
  bool isType( std::string type )
  {
    return mTypes.find( type ) != mTypes.end();
  }
  Type& getType( std::string type )
  {
    types_t::iterator i = mTypes.find(type);
    if( i == mTypes.end())
      throw -1;
    return i->second;
  }

private:
  Types()
  {
    static Type t_int1( "Int1",1,true );
    static Type t_int2( "Int2",2,true );
    static Type t_int8( "Int8",8,true );
    static Type t_int16( "Int16",16,true );
    static Type t_int32( "Int32",32,true );
    static Type t_int64( "Int64",64,true );
    static Type t_uint1( "UInt1",1,true );
    static Type t_uint2( "UInt2",2,true );
    static Type t_uint8( "UInt8",8,true );
    static Type t_uint16( "UInt16",16,true );
    static Type t_uint32( "UInt32",32,true );
    static Type t_uint64( "UInt64",64,true );
    mTypes.insert( pair_t( t_int1.name(),t_int1 ) );
    mTypes.insert( pair_t( t_int2.name(),t_int2 ) );
    mTypes.insert( pair_t( t_int8.name(),t_int8 ) );
    mTypes.insert( pair_t( t_int16.name(),t_int16 ) );
    mTypes.insert( pair_t( t_int32.name(),t_int32 ) );
    mTypes.insert( pair_t( t_int64.name(),t_int64 ) );
    mTypes.insert( pair_t( t_uint1.name(),t_uint1 ) );
    mTypes.insert( pair_t( t_uint2.name(),t_uint2 ) );
    mTypes.insert( pair_t( t_uint8.name(),t_uint8 ) );
    mTypes.insert( pair_t( t_uint16.name(),t_uint16 ) );
    mTypes.insert( pair_t( t_uint32.name(),t_uint32 ) );
    mTypes.insert( pair_t( t_uint64.name(),t_uint64 ) );
  }

  typedef std::pair< std::string, Type > pair_t;
  typedef std::map< std::string, Type > types_t;

  types_t mTypes;
};

#endif//__type_h__

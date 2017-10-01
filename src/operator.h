#ifndef __operator_h__
#define __operator_h__

#include <string>
#include <map>

class Operator
{
public:
  enum ID
  {
    REG, ADD, SUB, MUL, GT, LT, EQ, MUX2x1, SHR, SHL, DIV, MOD, INC, DEC, UNITY, SELECT
  };
  Operator( const std::string name, ID id, const std::string comp )
      : mName( name ), mID( id ), mComponent( comp )
  {
  }
  const std::string& name()
  {
    return mName;
  }
  ID id()
  {
    return mID;
  }
  const std::string& component()
  {
    return mComponent;
  }
private:
  std::string mName;
  ID mID;
  std::string mComponent;
};

class Operators
{
public:
  static Operators& instance()
  {
    static Operators operators;
    return operators;
  }
  bool isOperator( const std::string oper )
  {
    return mOperators.find( oper ) != mOperators.end();
  }
  Operator& getOperator( const std::string oper )
  {
    operators_t::iterator i = mOperators.find( oper );
    if( i == mOperators.end() )
      throw;
    return i->second;
  }
  bool isOperatorID( const std::string oper, Operator::ID id)
  {
    operators_t::iterator i = mOperators.find( oper );
    return ((i != mOperators.end()) && (getOperator( oper ).id() == id));
  }
  bool isAssignment( const std::string oper )
  {
    return isOperatorID(oper,Operator::REG);
  }
  bool isUnity( const std::string oper )
  {
    return isOperatorID(oper,Operator::UNITY);
  }
  bool isSelect( const std::string oper )
  {
    return isOperatorID(oper,Operator::SELECT);
  }
  bool isMux( const std::string oper )
  {
    return isOperatorID(oper,Operator::MUX2x1);
  }
private:
  Operators()
  {
    static Operator t_reg( "=",Operator::REG,"REG" );
    static Operator t_add( "+",Operator::ADD,"ADD" );
    static Operator t_sub( "-",Operator::SUB,"SUB" );
    static Operator t_mul( "*",Operator::MUL,"MUL" );
    static Operator t_gt( ">",Operator::GT,"COMP" );
    static Operator t_lt( "<",Operator::LT,"COMP" );
    static Operator t_eq( "==",Operator::EQ,"COMP" );
    static Operator t_mux2x1( "?",Operator::MUX2x1,"MUX2x1" );
    static Operator t_shr( ">>",Operator::SHR,"SHR" );
    static Operator t_shl( "<<",Operator::SHL,"SHL" );
    static Operator t_div( "/",Operator::DIV,"DIV" );
    static Operator t_mod( "%",Operator::MOD,"MOD" );
    static Operator t_inc( " +1",Operator::INC,"INC" );
    static Operator t_dec( " -1",Operator::DEC,"DEC" );
    static Operator t_unity( "1",Operator::UNITY,"n/a" );
    static Operator t_select( ":",Operator::SELECT,"n/a" );
    mOperators.insert( pair_t( t_reg.name(),t_reg ) );
    mOperators.insert( pair_t( t_add.name(),t_add ) );
    mOperators.insert( pair_t( t_sub.name(),t_sub ) );
    mOperators.insert( pair_t( t_mul.name(),t_mul ) );
    mOperators.insert( pair_t( t_gt.name(),t_gt ) );
    mOperators.insert( pair_t( t_lt.name(),t_lt ) );
    mOperators.insert( pair_t( t_eq.name(),t_eq ) );
    mOperators.insert( pair_t( t_mux2x1.name(),t_mux2x1 ) );
    mOperators.insert( pair_t( t_shr.name(),t_shr ) );
    mOperators.insert( pair_t( t_shl.name(),t_shl ) );
    mOperators.insert( pair_t( t_div.name(),t_div ) );
    mOperators.insert( pair_t( t_mod.name(),t_mod ) );
    mOperators.insert( pair_t( t_inc.name(),t_inc ) );
    mOperators.insert( pair_t( t_dec.name(),t_dec ) );
    mOperators.insert( pair_t( t_unity.name(),t_unity ) );
    mOperators.insert( pair_t( t_select.name(), t_select ) );
  }

  typedef std::pair< std::string, Operator > pair_t;
  typedef std::map< std::string, Operator > operators_t;

  operators_t mOperators;
};

#endif//__operator_h__

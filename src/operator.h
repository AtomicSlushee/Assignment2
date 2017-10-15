#ifndef __operator_h__
#define __operator_h__

#include <string>
#include <map>

class Operator
{
public:
  // enumerations of the component types
  enum ID
  {
    REG, ADD, SUB, MUL, GT, LT, EQ, MUX2x1, SHR, SHL, DIV, MOD, INC, DEC, UNITY, SELECT
  };

  // constructor
  Operator( std::string name, ID id, const std::string comp, int nargs )
      : mName( name ), mID( id ), mComponent( comp ), mNumArgs(nargs)
  {
  }

  // return the component netlist symbolic name
  std::string& name()
  {
    return mName;
  }

  // return the operator ID enumeration
  ID id()
  {
    return mID;
  }

  // return the component name for the operator;
  // uses the optional 'isSigned' flag to prepend 'S' if necessary
  const std::string component(bool isSigned=false)
  {
    std::string name = mComponent;
    if(isSigned && (mID!=REG) && (mID!=MUX2x1))
      name = "S" + name;
    return name;
  }

  // return the number of input arguments for the operator
  const int nargs()
  {
    return mNumArgs;
  }

private:
  std::string mName;
  ID mID;
  std::string mComponent;
  int mNumArgs;
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
  Operator& getOperatorByComponent( const std::string comp )
  {
    for (operators_t::iterator i = mOperators.begin(); i != mOperators.end(); i++)
    {
      if (comp == i->second.component())
        return i->second;
    }
    throw;
  }
  Operator& getOperatorByID( Operator::ID id)
  {
    for (operators_t::iterator i = mOperators.begin(); i != mOperators.end(); i++)
    {
      if (id == i->second.id())
        return i->second;
    }
    throw;
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
    static Operator t_reg( "=",Operator::REG,"REG",1 );
    static Operator t_add( "+",Operator::ADD,"ADD",2 );
    static Operator t_sub( "-",Operator::SUB,"SUB",2 );
    static Operator t_mul( "*",Operator::MUL,"MUL",2 );
    static Operator t_gt( ">",Operator::GT,"COMP",2 );
    static Operator t_lt( "<",Operator::LT,"COMP",2 );
    static Operator t_eq( "==",Operator::EQ,"COMP",2 );
    static Operator t_mux2x1( "?",Operator::MUX2x1,"MUX2x1",3 );
    static Operator t_shr( ">>",Operator::SHR,"SHR",2 );
    static Operator t_shl( "<<",Operator::SHL,"SHL",2 );
    static Operator t_div( "/",Operator::DIV,"DIV",2 );
    static Operator t_mod( "%",Operator::MOD,"MOD",2 );
    static Operator t_inc( " +1",Operator::INC,"INC",1 );
    static Operator t_dec( " -1",Operator::DEC,"DEC",1 );
    static Operator t_unity( "1",Operator::UNITY,"n/a",0 );
    static Operator t_select( ":",Operator::SELECT,"n/a",0 );
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

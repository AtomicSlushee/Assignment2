#ifndef __latency_h__
#define __latency_h__

#include "operator.h"

#include <map>

class LatencyKey
{
public:
  LatencyKey(Operator& op, int width):mOperator(op),mWidth(width){}
  bool operator<(const LatencyKey& k) const
  {
    if (this->mOperator.name() == k.mOperator.name())
      return this->mWidth < k.mWidth;
    return this->mOperator.name() < k.mOperator.name();
  }
private:
  Operator& mOperator;
  int mWidth;
};

class Latencies
{
public:
  static Latencies& instance()
  {
    static Latencies latencies;
    return latencies;
  }
  double getLatency(Operator& op, int width)
  {
    latencies_t::iterator i = mLatencies.find(key_t(op,width));
    if (i == mLatencies.end())
      throw;
    return i->second;
  }
private:
  Latencies()
  {
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::REG ), 1 ), 2.616 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::REG ), 2 ), 2.644 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::REG ), 8 ), 2.879 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::REG ), 16 ), 3.061 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::REG ), 32 ), 3.602 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::REG ), 64 ), 3.966 ) );

    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::ADD ), 1 ), 2.704 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::ADD ), 2 ), 3.713 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::ADD ), 8 ), 4.924 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::ADD ), 16 ), 5.638 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::ADD ), 32 ), 7.270 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::ADD ), 64 ), 9.566 ) );
    
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::SUB ), 1 ), 3.024 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::SUB ), 2 ), 3.412 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::SUB ), 8 ), 4.890 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::SUB ), 16 ), 5.569 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::SUB ), 32 ), 7.253 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::SUB ), 64 ), 9.566 ) );
    
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::MUL ), 1 ), 2.438 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::MUL ), 2 ), 3.412 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::MUL ), 8 ), 7.453 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::MUL ), 16 ), 7.811 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::MUL ), 32 ), 12.395 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::MUL ), 64 ), 15.354 ) );
    
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::GT ), 1 ), 3.031 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::GT ), 2 ), 3.934 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::GT ), 8 ), 5.949 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::GT ), 16 ), 6.256 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::GT ), 32 ), 7.264 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::GT ), 64 ), 8.416 ) );

    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::LT ), 1 ), 3.031 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::LT ), 2 ), 3.934 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::LT ), 8 ), 5.949 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::LT ), 16 ), 6.256 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::LT ), 32 ), 7.264 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::LT ), 64 ), 8.416 ) );

    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::EQ ), 1 ), 3.031 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::EQ ), 2 ), 3.934 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::EQ ), 8 ), 5.949 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::EQ ), 16 ), 6.256 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::EQ ), 32 ), 7.264 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::EQ ), 64 ), 8.416 ) );

    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::MUX2x1 ), 1 ), 4.083 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::MUX2x1 ), 2 ), 4.115 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::MUX2x1 ), 8 ), 4.815 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::MUX2x1 ), 16 ), 5.623 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::MUX2x1 ), 32 ), 8.079 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::MUX2x1 ), 64 ), 8.766 ) );
    
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::SHR ), 1 ), 3.644 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::SHR ), 2 ), 4.007 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::SHR ), 8 ), 5.178 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::SHR ), 16 ), 6.460 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::SHR ), 32 ), 8.819 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::SHR ), 64 ), 11.095 ) );

    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::SHL ), 1 ), 3.614 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::SHL ), 2 ), 3.980 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::SHL ), 8 ), 5.152 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::SHL ), 16 ), 6.549 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::SHL ), 32 ), 8.565 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::SHL ), 64 ), 11.220 ) );

    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::DIV ), 1 ), 0.619 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::DIV ), 2 ), 2.144 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::DIV ), 8 ), 15.439 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::DIV ), 16 ), 33.093 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::DIV ), 32 ), 86.312 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::DIV ), 64 ), 243.233 ) );
    
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::MOD ), 1 ), 0.758 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::MOD ), 2 ), 2.149 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::MOD ), 8 ), 16.078 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::MOD ), 16 ), 35.563 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::MOD ), 32 ), 88.142 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::MOD ), 64 ), 250.583 ) );

    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::INC ), 1 ), 1.792 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::INC ), 2 ), 2.218 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::INC ), 8 ), 3.111 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::INC ), 16 ), 3.471 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::INC ), 32 ), 4.347 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::INC ), 64 ), 6.200 ) );

    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::DEC ), 1 ), 1.792 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::DEC ), 2 ), 2.218 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::DEC ), 8 ), 3.108 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::DEC ), 16 ), 3.701 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::DEC ), 32 ), 4.685 ) );
    mLatencies.insert( pair_t( key_t( Operators::instance().getOperatorByID( Operator::DEC ), 64 ), 6.503 ) );
  }

  typedef LatencyKey key_t;
  typedef std::pair< key_t, double > pair_t;
  typedef std::map< key_t, double > latencies_t;

  latencies_t mLatencies;
};

#endif//__latency_h__

#ifndef __assignment_h__
#define __assignment_h__

#include "type.h"
#include "ioclass.h"
#include "variable.h"
#include "operator.h"
#include "latency.h"

#include <list>
#include <iostream>
#include <sstream>

class Assignment
{
  friend class Assignments;
public:
  Assignment( int count, Operator& op, Variable& result, Variable& input1, Variable& input2 = dummyvar(), Variable& input3 =
                  dummyvar() )
      : mCount(count), mWidth(0), mLatency(0.0), mOperator( op ), mResult( result ), mOperand1( input1 ), mOperand2( input2 ), mOperand3( input3 )
  {
    // determine operator width
    mWidth = result.width();
    if ((op.id() == Operator::GT) ||
        (op.id() == Operator::LT) ||
        (op.id() == Operator::EQ))
    {
      mWidth = (input1.width() > input2.width()) ? input1.width() : input2.width();
    }
    mLatency = Latencies::instance().getLatency(op,mWidth);
  }
  friend std::ostream& operator<<(std::ostream& out, Assignment& a)
  {
    out << a.mOperator.component() << " #(.DATAWIDTH(" << a.mWidth << "))";
    out << " inst" << a.mCount << "_" << a.mOperator.component(); 
    out << " (" << extend(a.mOperand1,a.mWidth);
    if (a.mOperator.nargs() > 1)
      out << "," << extend(a.mOperand2,a.mWidth);
    if (a.mOperator.nargs() > 2)
      out << "," << extend(a.mOperand3,a.mWidth);
    out << "," << a.mResult.name() << ");";
    /*DEBUG*/ out << " // latency = " << a.mLatency << " ns";
    return out;
  }
private:
  // return reference to a dummy variable to fill in unused operands
  static Variable& dummyvar()
  {
    static Type dummytype( "dummy",1,false );
    static IOClass dummyio( "dummy",IOClass::INPUT );
    static Variable dummyvar( "dummy",dummytype,dummyio );
    return dummyvar;
  }
  // create, if necessary, a width-extended input
  static std::string extend(Variable& var, int width)
  {
    // first, assume no modification
    std::string result(var.name());
    // now evaluate variable width versus component width
    if (var.width() < width)
    {
      std::ostringstream tmp;
      if (var.isSigned())
      {
        // sign extend
        tmp << "{" << (width-var.width()) << "{" << var.name() << "[" << var.width()-1 << "]" << "}," << result << "}";
      }
      else
      {
        // pad with zeros
        tmp << "{" << (width-var.width()) << "'b0," << result << "}";
      }
      result = tmp.str();
    }
    return result;
  }
private:
  int mCount;
  int mWidth;
  double mLatency;
  Operator& mOperator;
  Variable& mResult;
  Variable& mOperand1;
  Variable& mOperand2;
  Variable& mOperand3;
};

class Assignments
{
public:
  static Assignments& instance()
  {
    static Assignments assignments;
    return assignments;
  }
  Assignment& addAssignment( Operator& op, Variable& output, Variable& input1, Variable& input2 = Assignment::dummyvar(),
                      Variable& input3 = Assignment::dummyvar() )
  {
    Assignment* pA = new Assignment( mCount++,op,output,input1,input2,input3 );
    mAssignments.push_back( *pA );
    return *pA;
  }
private:
  Assignments():mCount(0)
  {
  }

  typedef std::list< Assignment > assignments_t;

  assignments_t mAssignments;
  int mCount;
};

#endif//__assignment_h__

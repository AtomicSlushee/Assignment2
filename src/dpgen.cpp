#include "dpgen.h"
#include "type.h"
#include "variable.h"
#include "ioclass.h"
#include "operator.h"
#include "tokenizer.h"
#include "latency.h"
#include "assignment.h"
#include "graphType.hpp"
#include <cstdio>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <vector>
#include <libgen.h>

// enable debug output
// #define DEBUGOUT if(1)
// disable debug output
#define DEBUGOUT if(0)

static const char* indent = "    ";

// define the clock and reset, just in case we need them
namespace builtIn
{
  Variable clock(std::string("__clk"),Types::instance().getType("UInt1"),IOClasses::instance().getIOClass("input"));
  Variable reset(std::string("__rst"),Types::instance().getType("UInt1"),IOClasses::instance().getIOClass("input"));
  Variable dummy1(std::string("__na1"),Types::instance().getType("UInt1"),IOClasses::instance().getIOClass("wire"));
  Variable dummy2(std::string("__na2"),Types::instance().getType("UInt1"),IOClasses::instance().getIOClass("wire"));
}


bool process( std::ifstream& in )
{
  bool addedClkRst = false;
  bool addedDummies = false;
  std::string line;
  int lineNum = 0;

  // for all lines in the circuit file
  while( std::getline( in,line ) )
  {
    lineNum++;
    Tokenizer tokens(line);
    std::string token;
    while( tokens.next(token))
    {
      // skip comments explicitly
      if ((token.length()>1) && (token[0]=='/') && (token[1]=='/'))
        break; // go to next line

      // is it an IOClass declaration?
      if( IOClasses::instance().isIOClass(token))
      {
        std::string ttoken;
        if( tokens.next(ttoken))
        {
          if( Types::instance().isType(ttoken) )
          {
            Type& type = Types::instance().getType(ttoken);
            IOClass& ioclass = IOClasses::instance().getIOClass(token);
            std::string vtoken;
            while( tokens.next(vtoken, delimiters::csv))
            {
              Variables::instance().addVariable(vtoken,type,ioclass);
              DEBUGOUT std::cout << "added variable " << vtoken << " of type " << ttoken << " with IO class " << token << std::endl;
            }
          }
          else
          {
            fprintf(stderr,"error: invalid data type %s on line %d\n", ttoken.c_str(), lineNum);
            return false;
          }
        }
        else
        {
          fprintf(stderr,"error: missing data type for IO Class %s on line %d\n", token.c_str(), lineNum );
          return false;
        }
        tokens.kill();
      }
      // must be a variable assignment
      else
      {
        if( Variables::instance().isVariable(token))
        {
          Variable& result = Variables::instance().getVariable(token);
          std::string etoken;
          if( tokens.next(etoken) && Operators::instance().isAssignment(etoken))
          {
            DEBUGOUT std::cout << "assigning " << token << " equals ";
            std::vector<std::string> args;
            while(tokens.next(etoken))
            {
              args.push_back(etoken);
            }
            if (args.size())
            {
              if (Variables::instance().isVariable(args[0]))
              {
                Variable& input1 = Variables::instance().getVariable(args[0]);
                if (args.size() == 1)
                {
                  DEBUGOUT std::cout << "variable " << args[0] << " with REG";
                  if (!addedClkRst)
                  {
                    Variables::instance().addVariable( builtIn::clock );
                    Variables::instance().addVariable( builtIn::reset );
                    addedClkRst = true;
                  }
                  Assignments::instance().addAssignment( Operators::instance().getOperatorByID( Operator::REG ),
                                                         result, input1, Assignment::dummyvar(), Assignment::dummyvar(),
                                                         builtIn::clock, builtIn::reset );
                }
                else if( (args.size() == 3) && (Operators::instance().isOperator(args[1])))
                {
                  Operator& op = Operators::instance().getOperator(args[1]);
                  bool unity = Operators::instance().isUnity(args[2]);
                  if( (op.id() == Operator::ADD) && unity)
                  {
                    DEBUGOUT std::cout << "INC 1";
                    Assignments::instance().addAssignment( Operators::instance().getOperatorByID(Operator::INC), result, input1);
                  }
                  else if ((op.id() == Operator::SUB) && unity)
                  {
                    DEBUGOUT std::cout << "DEC 1";
                    Assignments::instance().addAssignment( Operators::instance().getOperatorByID(Operator::DEC), result, input1);
                  }
                  else if (Variables::instance().isVariable(args[2]))
                  {
                    Variable& input2 = Variables::instance().getVariable(args[2]);
                    DEBUGOUT std::cout << args[0] << " " << op.component() << " " << args[2];
                    if( (op.id() == Operator::GT ) ||
                        (op.id() == Operator::LT ) ||
                        (op.id() == Operator::EQ ))
                    {
                      if( !addedDummies )
                      {
                        Variables::instance().addVariable( builtIn::dummy1 );
                        Variables::instance().addVariable( builtIn::dummy2 );
                        addedDummies = true;
                      }
                      Assignments::instance().addAssignment( op, result, input1, input2, Assignment::dummyvar(),
                                                             builtIn::dummy1, builtIn::dummy2);
                    }
                    else
                    {
                      Assignments::instance().addAssignment( op, result, input1, input2);
                    }
                  }
                  else
                  {
                    fprintf(stderr,"error: unknown variable %s on line %d\n",args[2].c_str(),lineNum);
                    return false;
                  }
                }
                else if ((args.size() == 5) && (Operators::instance().isMux(args[1])))
                {
                  if(Variables::instance().isVariable(args[2]) &&
                     Variables::instance().isVariable(args[4]) &&
                     Operators::instance().isSelect(args[3]))
                  {
                    DEBUGOUT std::cout << "MUX " << args[0] << " selects " << args[2] << " or " << args[4];
                    Assignments::instance().addAssignment( Operators::instance().getOperatorByID( Operator::MUX2x1 ),
                                                           result,
                                                           Variables::instance().getVariable( args[0] ),
                                                           Variables::instance().getVariable( args[2] ),
                                                           Variables::instance().getVariable( args[4] ) );
                  }
                  else
                  {
                    fprintf(stderr,"error: malformed mux on line %d\n",lineNum);
                    return false;
                  }
                }
                else
                {
                  fprintf(stderr,"error: malformed assignment on line %d\n",lineNum);
                  return false;
                }
              }
              else
              {
                fprintf(stderr,"error: argument for assignment to %s is not a variable on line %d\n",token.c_str(),lineNum);
                return false;
              }
            }
            else
            {
              fprintf(stderr,"error: assignment to %s has no arguments on line %d\n",token.c_str(),lineNum);
              return false;
            }
            DEBUGOUT std::cout << std::endl;
            tokens.kill();
          }
          else
          {
            fprintf(stderr,"error: no assignment operator for variable %s on line %d\n",token.c_str(),lineNum);
            return false;
          }
        }
        else
        {
          fprintf(stderr,"error: undefined variable %s on line %d\n",token.c_str(),lineNum);
          return false;
        }
      }
    }
  }

  return true;
}

bool critical()
{
  std::list<Assignment> nodeList;

  for (Assignments::iterator_t i = Assignments::instance().begin(); i != Assignments::instance().end(); i++)
  {
    nodeList.push_back(*i);
  }

  // We have created the file and filled up a vector of nodes.
  // Now create a graph
  graphType graph;
  graph.createWeightedGraph();
  graph.topologicalSort();    
  
  //DEBUGOUT graph.printGraph();

  graph.longestPath();

  //graph.printAssignmentNodes();
  graph.printLongestPath();
  
  return true;
}

bool verilog( std::ofstream& out, std::string name )
{
  bool success = true;
  bool comma = false;

  // timescale
  out << "`timescale 1ns / 1ps" << std::endl;

  // declare module
  out << "module m_" << name << "(";
  for (Variables::iterator_t i = Variables::instance().begin(); i != Variables::instance().end(); i++)
  {
    if( Variables::instance().variable(i).ioClass() == IOClass::INPUT )
    {
      if( comma )
        out << ",";
      out << Variables::instance().variable(i).name();
      comma = true;
    }
  }
  for (Variables::iterator_t i = Variables::instance().begin(); i != Variables::instance().end(); i++)
  {
    if( Variables::instance().variable(i).ioClass() == IOClass::OUTPUT )
    {
      if( comma )
        out << ",";
      out << Variables::instance().variable(i).name();
      comma = true;
    }
  }
  out << ");" << std::endl;

  // output the variable declarations
  for (Variables::iterator_t i = Variables::instance().begin(); i != Variables::instance().end(); i++)
  {
    out << indent << Variables::instance().variable(i) << std::endl;
  }

  out << std::endl;

  // output the assignments
  for (Assignments::iterator_t i = Assignments::instance().begin(); i != Assignments::instance().end(); i++)
  {
    out << indent << Assignments::instance().assignment(i) << std::endl;
  }

  // end the module
  out << "endmodule" << std::endl;

  return success;
}

std::string moduleName(char* argv)
{
  std::string name(basename(argv));
  size_t pos = name.find_last_of('.');
  if (std::string::npos != pos)
    name.erase(pos,std::string::npos);
  return name;
}

int main( int argc, char* argv[] )
{
  if( argc > 2 )
  {
    std::ifstream inFile( argv[1],std::ifstream::in );
    if( inFile.good() )
    {
      std::ofstream outFile( argv[2],std::ofstream::out );
      if( outFile.good() )
      {
        if( process( inFile ) )
        {
          if( critical() )
          {
            if( verilog( outFile, moduleName(argv[1])))
            {
              //fprintf(stdout, "converted %s to %s\n",argv[1],argv[2] );
            }
            else
            {
              fprintf(stderr, "error while converting %s to %s\n",argv[1],argv[2] );
            }
          }
          else
          {
            fprintf(stderr, "error calculating critical path\n");
          }
        }
        else
        {
          fprintf(stderr, "error while processing %s\n",argv[1] );
        }
        outFile.close();
      }
      else
      {
        fprintf(stderr, "error: invalid verilog filename\n" );
      }
      inFile.close();
    }
    else
    {
      fprintf(stderr, "error: invalid net list filename\n" );
    }
  }
  else
  {
    fprintf(stdout, "usage: dpgen <netListFile> <verilogFile>\n" );
  }

  return 0;
}


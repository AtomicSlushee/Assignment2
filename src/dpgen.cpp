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

// enable debug output
#define DEBUGOUT if(1)
// disable debug output
//#define DEBUGOUT if(0)

bool process( std::ifstream& in )
{
  std::string line;
  int lineNum = 0;
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
                  Assignments::instance().addAssignment( Operators::instance().getOperatorByID(Operator::REG), result, input1);
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
                    Assignments::instance().addAssignment( op, result, input1, input2);
                  }
                  else
                  {
                    fprintf(stderr,"error: malformed assignment to %s on line %d\n",token.c_str(),lineNum);
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
  graphType<Assignment, 10> graph;
  graph.createWeightedGraph(nodeList);
  return true;
}

bool verilog( std::ofstream& out )
{
  /*TODO*/ out << "// still need to handle clk/rst for REG and COMP stuff" << std::endl;
  /*TODO*/ out << "// also need to handle the module wrapper and such" << std::endl;

  bool success = true;

  // output the variable declarations
  for (Variables::iterator_t i = Variables::instance().begin(); i != Variables::instance().end(); i++)
  {
    out << Variables::instance().variable(i) << std::endl;
  }

  out << std::endl;

  // output the assignments
  for (Assignments::iterator_t i = Assignments::instance().begin(); i != Assignments::instance().end(); i++)
  {
    out << Assignments::instance().assignment(i) << std::endl;
  }

  return success;
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
            if( verilog( outFile ))
            {
              fprintf(stdout, "converted %s to %s\n",argv[1],argv[2] );
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


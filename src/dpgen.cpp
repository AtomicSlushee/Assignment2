#include "dpgen.h"
#include "type.h"
#include "variable.h"
#include "ioclass.h"
#include "operator.h"
#include "tokenizer.h"

#include <cstdio>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <vector>

bool process( std::ifstream& in, std::ofstream& out )
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
              out << "added variable " << vtoken << " of type " << ttoken << " with IO class " << token << std::endl;
            }
          }
          else
          {
            printf("error: invalid data type %s on line %d\n", ttoken.c_str(), lineNum);
            return false;
          }
        }
        else
        {
          printf("error: missing data type for IO Class %s on line %d\n", token.c_str(), lineNum );
          return false;
        }
        tokens.kill();
      }
      // must be a variable assignment
      else
      {
        //do { out << token; } while (tokens.next(token)); out << std::endl;
        if( Variables::instance().isVariable(token))
        {
          std::string etoken;
          if( tokens.next(etoken) && Operators::instance().isAssignment(etoken))
          {
            out << "assigning " << token << " equals ";
            std::vector<std::string> args;
            while(tokens.next(etoken))
            {
              args.push_back(etoken);
            }
            if (args.size())
            {
              if (Variables::instance().isVariable(args[0]))
              {
                if (args.size() == 1)
                {
                  out << "variable " << args[0] << " with REG";
                }
                else if( (args.size() == 3) && (Operators::instance().isOperator(args[1])))
                {
                  Operator& op = Operators::instance().getOperator(args[1]);
                  bool unity = Operators::instance().isUnity(args[2]);
                  if( (op.id() == Operator::ADD) && unity)
                  {
                    out << "INC 1";
                  }
                  else if ((op.id() == Operator::SUB) && unity)
                  {
                    out << "DEC 1";
                  }
                  else if (Variables::instance().isVariable(args[2]))
                  {
                    out << args[0] << " " << op.component() << " " << args[2];
                  }
                  else
                  {
                    printf("error: malformed assignment to %s on line %d\n",token.c_str(),lineNum);
                    return false;
                  }
                }
                else if ((args.size() == 5) && (Operators::instance().isMux(args[1])))
                {
                  if(Variables::instance().isVariable(args[2]) &&
                     Variables::instance().isVariable(args[4]) &&
                     Operators::instance().isSelect(args[3]))
                  {
                    out << "MUX " << args[0] << " selects " << args[2] << " or " << args[4];
                  }
                  else
                  {
                    printf("error: malformed mux on line %d\n",lineNum);
                    return false;
                  }
                }
                else
                {
                  printf("error: malformed assignment on line %d\n",lineNum);
                  return false;
                }
              }
              else
              {
                printf("error: argument for assignment to %s is not a variable on line %d\n",token.c_str(),lineNum);
                return false;
              }
            }
            else
            {
              printf("error: assignment to %s has no arguments on line %d\n",token.c_str(),lineNum);
              return false;
            }
            out << std::endl;
            tokens.kill();
          }
          else
          {
            printf("error: no assignment operator for variable %s on line %d\n",token.c_str(),lineNum);
            return false;
          }
        }
      }
    }
  }
  return true;
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
        if( process( inFile,outFile ) )
        {
          printf( "converted %s to %s\n",argv[1],argv[2] );
        }
        else
        {
          printf( "error while converting %s to %s\n",argv[1],argv[2] );
        }
        outFile.close();
      }
      else
      {
        printf( "error: invalid verilog filename\n" );
      }
      inFile.close();
    }
    else
    {
      printf( "error: invalid net list filename\n" );
    }
  }
  else
  {
    printf( "usage: dpgen <netListFile> <verilogFile>\n" );
  }

  return 0;
}


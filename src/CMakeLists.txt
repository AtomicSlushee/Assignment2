# Authors: Ken Steele, Andrew Byron and Eric Lichtenstein
# create a new executable for Assignment 2

FILE( GLOB SRCS *.cpp *.h )
add_executable( dpgen ${SRCS} )


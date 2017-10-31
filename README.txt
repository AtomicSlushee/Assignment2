Assignment #2

Group members (all ECE574A):

* Ken Steele (kensteele)
* Andrew Byron (avgbyron)
* Eric Lichtenstein (elichten)

Program description:

Implements 'dpgen' per homework assignment #2.  Given a C-like expression
of a netlist, it produces a Verilog module and calculates the critical
path per the provided timing data. The program first parses the netlist,
building a list of assignments comprising an operation, result and operands.
These are traversed and converted into an equivalent Verilog module. The
assignment tree is also converted into a vertex graph, topologically sorted,
and longest path computed from the weighted edges -- the result is presented
to the user via stdout. Program arguments per the assignment.

General distribution of work for this assignment:

* Ken -- netlist parser
* Andrew -- graph classes
* Eric -- finding critical path
* all -- general cleanup, debug, testing


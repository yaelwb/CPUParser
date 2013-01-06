CPUParser
=========

C++ exercise in creating parser and compiler for imaginary CPU with a defined set of commands. 
I created the user interface to convert assembly command file into binary file.

The CPU:
RISC architecture, 61 User instructions, 32 bit single precision floating point operations.
See Assembly language instructions set.pdf for detailed commands.
Usage of that CPU - The user should write an assembly program according to the instruction set 
and assembly language syntax. The CPUParser application will be used to parse and compile the code,
so that the user could run the compiled code in the CPU core.

Innput: An assembly program code file.
Output: A inary code file.
Please refer to attached assembly.txt and binary.txt for an example.
The software will load an assembly file, parse it, and according to parsing results (successful or not) 
will generate a MIF binary file or a detailed error list. 
It was written in an object-oriented approach and consists of the following elements:

Maps:
m_cmdMap – maps command strings to type strings
m_labelsMap – maps label strings to integers
m_opcodeMap – maps command strings to binary strings
m_regMap - maps register strings to binary strings

Classes: Please view attached classDiagram.jpg and sequenceDiagram.jpg for clarifications.
Parser - Holds processing results, a vector of CmdInfo’s, and parse functions
CmdInfo – Holds assembly and binary strings, set/get functions
Compiler – Holds processing results and compiling functions

The main work is done in Parser::ProcessLine, dealing with various commands, different argument number and types,
and a specific mechanism to cope with labels and goto/jump commands using m_labelsMap, proccesed in PostProcessLabels().
The Parser maintains a vector of CmdInfo objects, creating one per parsed command,then the labels info is inserted during the post proccesing stage,
and this information vector is used by the compiler to create the binary commands.

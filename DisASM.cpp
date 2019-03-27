#include "MachLangParser.h"
#include <iostream>
#include <cstdlib>

using namespace std;

/* This program is passed the name of a MIPS instruction file.  It
 * reads in that file.  If the file is not a correctly formatted file,
 * the program prints out an error message and exits.  If the file is
 * correctly formatted, the program prints out the corresponding assembly
 * for each binary machine code line from the file followed by its MIPS
 * assembly representation.
 */
int main(int argc, char *argv[])
{
  MachLangParser *parser;

  if(argc != 2){
    cerr << "You must enter an input file name " << endl;
    exit(1);
  }

  // Parse the input binary file
  parser = new MachLangParser(argv[1]);

  // If the file is not correctly formatted, print an error message and exit.
  if(parser->isFormatCorrect() == false){
    cerr << "Format of input file is incorrect " << endl;
    exit(1);
  }

  Instruction i;

  // Go through every instruction from the input file, printing out
  // its 32 bit binary representation followed by its MIPS assembly
  // representation.
  i = parser->getNextInstruction();

  while( i.getOpcode() != UNDEFINED )
  {
    cout << i.getEncoding() << "\t";
    cout << i.getAssembly() << endl ;
    i = parser->getNextInstruction();
  }

  delete parser;

}

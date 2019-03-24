#ifndef __MACHLANGPARSER_H__
#define __MACHLANGPARSER_H__

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <bitset>
#include "Instruction.h"
#include "RegisterTable.h"
#include "OpcodeTable.h"

using namespace std;


enum SignFlag{
    UNSIGNED, // 0
    SIGNED    // 1
};

/* This class reads in a machine language representation of a MIPS
 * program.  If the file is correctly formatted (i.e. every line
 * contains a properly formatted 32 bit machine language/binary
 * representation of a supported MIPS instruction, this
 * class will retain a list of Instructions (one for each machine
 * language instruction from the file).  This list of Instructions
 * can be iterated through.
 */
class MachLangParser{
 public:

  // Specify a text file containing machine language/binary representations
  // of MIPS instructions.  This constructor checks the correctness of
  // each line of the file to see if it is a 32 bit binary sequence that
  // represents a supported MIPS instruction.  If it does, a list of
  // Instructions is created and stored internally.  If it does not, then
  // the class will indicate that the file is incorrectly formatted.
  //
  MachLangParser(string input_file);

  // Destructor
  ~MachLangParser();

  // Returns true if the file specified was a valid/correct MIPS instruction
  // file.  Otherwise, returns false.
  bool isFormatCorrect() { return myFormatCorrect; }

  // Iterator that returns the next Instruction in the list of Instructions.
  Instruction getNextInstruction();

 private:
    vector<Instruction> myInstructions;      // list of Instructions
    bool myFormatCorrect;                    // is format correct?
    int myIndex;                             // iterator index

    OpcodeTable opcodes;

    static const int REG_WIDTH = 5;
    static const int J_ADDR_WIDTH = 26;
    static const int IMM_WIDTH = 16;
    static const int ARCH_NUM_BITS = 32;

    bool isLineCorrect(string line);

    void decode(Instruction& i);

    void decodeRType(Instruction& i);

    void decodeIType(Instruction& i);

    void decodeJType(Instruction& i);

    int convertToInt(string s, SignFlag f);

    void assemble(Instruction& i);

    string assembleRType(Instruction i);
    string assembleIType(Instruction i);
    string assembleJType(Instruction i);


};


#endif

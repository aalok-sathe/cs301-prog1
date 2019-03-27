#include "MachLangParser.h"

/*
 * Public class methods go here
 */

MachLangParser::MachLangParser(string inputfile)
// TODO docstring goes here
// Default constructor for the MachLangParser class.
// Initializes using a string filename of an input file,
// and processes each line in that input file to decode
// the binary and assemble an assembly instruction.
//
// Parameters:
//     string inputfile := path of inputfile relative to
//                         current directory
{
    // initialize private variables 'myFormatCorrect' and 'myIndex'
    myFormatCorrect = true;
    myIndex = 0;

    // Initialize 'Instruction' to store decoded instruction
    Instruction i;

    // open input filestream for reading
    ifstream in;
    in.open(inputfile.c_str());

    // if failed to open the file for reading, stop
    if (in.bad())
    {
        myFormatCorrect = false;
        return;
    }

    // process for each line in the file
    string line;
    while (getline(in, line))
    {
        // make sure the line meets basic format requirements
        // such as length and valid characters
        if (not isLineCorrect(line))
        {
            myFormatCorrect = false;
            break;
        }
        i.setEncoding(line);

        // call the 'decode' method that splits the string
        // into relevant pieces
        decode(i);

        // check if the instruction is supported
        if (i.getOpcode() == UNDEFINED)
        {
            myFormatCorrect = false;
            break;
        }
        // call the 'encode' method to put together
        // the assembly syntax
        assemble(i);

        // store instructions in private container
        // after processing them
        myInstructions.push_back(i);

    }

}


MachLangParser::~MachLangParser()
// Default deconstructor for the MachLangParser class
{}


Instruction MachLangParser::getNextInstruction()
// Method that returns the next instruction using a
// private 'myIndex' index variable
// If index is greater than the size of our container,
// return an empty/default 'Instruction' object whose
// opcode is equivalent to UNDEFINED
{
    if (myIndex < (int)(myInstructions.size()))
        return myInstructions[myIndex++];

    Instruction i;
    return i;
}


bool MachLangParser::isLineCorrect(string line)
// Checks to see if each of these conditions is matched:
// 1.  length == ARCH_NUM_BITS
// 2.  contains only 1s and 0s
{
    // check if line has appropriate length
    if (line.length() != ARCH_NUM_BITS)
        return false;

    // check if line contains only 1s and 0s only
    for (int i = 0; i < (int)line.length(); i++)
        if (line[i] != '0' and line[i] != '1')
            return false;

    return true;
}


void MachLangParser::decode(Instruction& i)
// A method that takes in an Instruction object i by
// reference, with an initialized encoding field that
// is a binary string, and decodes it to figure out
// its opcode, and registers, imm, shamt, and funct field,
// as applicable
{
    string opc = i.getEncoding().substr(0, OPCODE_LEN);
    string funct = i.getEncoding().substr(
            ARCH_NUM_BITS-OPCODE_LEN,
            OPCODE_LEN
        );

    Opcode instOpcode = opcodes.getInstr(opc, funct);
    InstType type = opcodes.getInstType(instOpcode);

    if (type == RTYPE)
        decodeRType(i);
    else if (type == ITYPE)
        decodeIType(i);
    else // type == JTYPE
        decodeJType(i);
}


void MachLangParser::decodeRType(Instruction& i)
// a decode helper method specific to R-type instructions
{
    // start with the binary string
    string encoding = i.getEncoding();

    // figure out the various field and operands according
    // to their known order, and pre-specified lengths
    int index = 0;
    string opstr = encoding.substr(index, OPCODE_LEN);
    index += OPCODE_LEN;
    string rs = encoding.substr(index, REG_WIDTH);
    index += REG_WIDTH;
    string rt = encoding.substr(index, REG_WIDTH);
    index += REG_WIDTH;
    string rd = encoding.substr(index, REG_WIDTH);
    index += REG_WIDTH;
    string shamt = encoding.substr(index, SHAMT_LEN);
    index += SHAMT_LEN;
    string funct = encoding.substr(index, OPCODE_LEN);

    Opcode opc = opcodes.getInstr(opstr, funct);

    // set the appropriate values of the instruction
    // and pass 'NumRegisters' to the places that are
    // not applicable
    i.setValues(
        opc,
        convertToInt(rs, UNSIGNED),
        convertToInt(rt, UNSIGNED),
        convertToInt(rd, UNSIGNED),
        convertToInt(shamt, UNSIGNED)
    );

}


void MachLangParser::decodeIType(Instruction& i)
// a decode helper method specific to I-type instructions
{
    string encoding = i.getEncoding();

    // figure out the various field and operands according
    // to their known order, and pre-specified lengths
    int index = 0;
    string opstr = encoding.substr(index, OPCODE_LEN);

    index += OPCODE_LEN;
    string rs = encoding.substr(index, REG_WIDTH);

    index += REG_WIDTH;
    string rt = encoding.substr(index, REG_WIDTH);

    index += REG_WIDTH;
    string imm = encoding.substr(index, IMM_WIDTH);

    Opcode opc = opcodes.getInstr(opstr, "");

    // set the appropriate values of the instruction
    // and pass 'NumRegisters' to the places that are
    // not applicable
    i.setValues(
        opc,
        convertToInt(rs, UNSIGNED),
        convertToInt(rt, UNSIGNED),
        NumRegisters,
        convertToInt(imm, SIGNED)
    );
}


void MachLangParser::decodeJType(Instruction& i)
// a decode helper method specific to J-type instructions
{
    string encoding = i.getEncoding();

    // figure out the various field and operands according
    // to their known order, and pre-specified lengths
    int index = 0;
    string opstr = encoding.substr(index, OPCODE_LEN);
    index += OPCODE_LEN;
    string addr = encoding.substr(index, J_ADDR_WIDTH);

    Opcode opc = opcodes.getInstr(opstr, "");

    // set the appropriate fields of the instruction
    // and pass 'NumRegisters' to the places that are
    // not applicable
    i.setValues(
        opc,
        NumRegisters,
        NumRegisters,
        NumRegisters,
        convertToInt(addr, SIGNED)
    );
}


int MachLangParser::convertToInt(string s, SignFlag f)
// Helper method that takes in a string of up to
// ARCH_NUM_BITS bits, and treats it as either signed
// unsigned based on the passed flag SignFlag f
{
    // if we need signed, sign extend it to match the
    // bitset width
    if ((int)f == 1)
    {
        int extn = ARCH_NUM_BITS - s.size();
        string signextend(extn, s[0]);
        signextend += s;
        std::bitset<ARCH_NUM_BITS> bits(signextend);
        return (int)(bits.to_ulong());
    }

    // unsigned is straightforward: just initialize
    // bitset and convert to unsigned int assuming
    // no sign bit
    std::bitset<ARCH_NUM_BITS> bits(s);
    return (int)(bits.to_ulong());
}


void MachLangParser::assemble(Instruction& i)
// method that takes an instruction with all the
// appropriate fields filled in, and puts together
// an assembly instruction
{
    string assembled;

    InstType type = opcodes.getInstType(i.getOpcode());
    if (type == RTYPE)
        assembled = assembleRType(i);
    else if (type == ITYPE)
        assembled = assembleIType(i);
    else // type == JTYPE
        assembled = assembleJType(i);

    i.setAssembly(assembled);
}


string MachLangParser::assembleRType(Instruction i)
// assemble specific to R-type: put together
{
    ostringstream assembled;
    // start with the instruction name corresponding to opcode
    assembled << opcodes.getName(i.getOpcode()) + "\t";

    Opcode opc = i.getOpcode();

    for (int it = 0; it < opcodes.numOperands(opc); it++)
    {
        if (opcodes.RSposition(opc) == it)
            assembled << "$" << i.getRS();
        else if (opcodes.RTposition(opc) == it)
            assembled << "$" << i.getRT();
        else if (opcodes.RDposition(opc) == it)
            assembled << "$" << i.getRD();
        else if (opcodes.IMMposition(opc) == it)
            assembled << i.getImmediate();

        if (it < opcodes.numOperands(opc) - 1)
            assembled << ", ";
    }

    return assembled.str();
}


string MachLangParser::assembleIType(Instruction i)
// assemble specific to I-type: put together
{
    ostringstream assembled;
    // start with the instruction name corresponding to opcode
    assembled << opcodes.getName(i.getOpcode()) + "\t";

    Opcode opc = i.getOpcode();
    if (opc != BEQ and opc != LW)
    {
        for (int it = 0; it < opcodes.numOperands(opc); it++)
        {
            if (opcodes.RSposition(opc) == it)
                assembled << "$" << i.getRS();
            else if (opcodes.RTposition(opc) == it)
                assembled << "$" << i.getRT();
            else if (opcodes.RDposition(opc) == it)
                assembled << "$" << i.getRD();
            else if (opcodes.IMMposition(opc) == it)
                assembled << i.getImmediate();

            if (it < opcodes.numOperands(opc) - 1)
                assembled << ", ";
        }
        return assembled.str();
    }

    // only LW or BEQ
    // go through each operand position and add only if the
    // operand's position matches the current position being
    // assembled.
    // LW:
    //  add support to display an offset to a register
    if (i.getOpcode() == LW)
        for (int it = 0; it < opcodes.numOperands(opc); it++)
        {
            if (opcodes.RSposition(opc) == it)
                assembled << "$" << i.getRS() << ")";
            else if (opcodes.RTposition(opc) == it)
                assembled << "$" << i.getRT() << ", ";
            else if (opcodes.IMMposition(opc) == it)
                assembled << i.getImmediate() << "(";
        }
    // BEQ:
    //  add support to display a hex-encoded address
    else if (i.getOpcode() == BEQ)
        for (int it = 0; it < opcodes.numOperands(opc); it++)
        {
            if (opcodes.RSposition(opc) == it)
                assembled << "$" << i.getRS();
            else if (opcodes.RTposition(opc) == it)
                assembled << "$" << i.getRT();
            else if (opcodes.IMMposition(opc) == it)
                // multiply by 4 to account for truncation of
                // last two 0 bits; pass 'hex' flag for hex output
                assembled << "0x" << hex << i.getImmediate()*4;

            if (it < opcodes.numOperands(opc) - 1)
                assembled << ", ";
        }

    return assembled.str();
}


string MachLangParser::assembleJType(Instruction i)
// assemble method specific to J-type: put together J-types
// that involve an address
{
    ostringstream assembled;
    // start with the instruction name corresponding to opcode
    assembled << opcodes.getName(i.getOpcode()) + "\t";

    Opcode opc = i.getOpcode();

    // go through each operand position and add only if the
    // operand's position matches the current position being
    // assembled
    for (int it = 0; it < opcodes.numOperands(opc); it++)
    {
        if (opcodes.RSposition(opc) == it)
            assembled << "$" << i.getRS();
        else if (opcodes.RTposition(opc) == it)
            assembled << "$" << i.getRT();
        else if (opcodes.IMMposition(opc) == it)
            // multiply by 4 to account for truncation of
            // last two 0 bits; pass 'hex' flag for hex output
            assembled << "0x" << hex << i.getImmediate()*4;

        if (it < opcodes.numOperands(opc) - 1)
            assembled << ", ";
    }

    return assembled.str();
}

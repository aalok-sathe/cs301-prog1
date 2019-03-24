#include "MachLangParser.h"

/*
 * Public class methods go here
 */

MachLangParser::MachLangParser(string inputfile)
                               : myFormatCorrect(true),
                                 myIndex(0)
// TODO docstring goes here
// DEBUG
{
    cout << "HERE ... ?" << endl;
    Instruction i;

    ifstream in;
    in.open(inputfile.c_str());

    if (in.bad())
    {
        myFormatCorrect = false;
        return;
    }

    string line;
    while (getline(in, line))
    {
        cout << "Line:\t" << line << endl;
        if (not isLineCorrect(line))
        {
            myFormatCorrect = false;
            break;
        }
        i.setEncoding(line);
        i.setAssembly("// TODO");

        decode(i);
        if (i.getOpcode() == UNDEFINED)
        {
            myFormatCorrect = false;
            break;
        }
        assemble(i);
        cout << "Instr:\t" << i.getString() << endl;

        myInstructions.push_back(i);

    }

}


MachLangParser::~MachLangParser()
// TODO docstring
{}


Instruction MachLangParser::getNextInstruction()
// TODO docstring
{
    if(myIndex < (int)(myInstructions.size())){
      myIndex++;
      return myInstructions[myIndex-1];
    }

    Instruction i;
    return i;
}


bool MachLangParser::isLineCorrect(string line)
// TODO docstring
// Checks to see if each of these conditions is matched:
//  -- length = ARCH_NUM_BITS
//  -- contains only 1s and 0s
{
    bool correct = true;

    // check if line has appropriate length
    if (line.length() != ARCH_NUM_BITS)
        correct = false;

    // check if line contains only 1s and 0s
    for (unsigned int i=0; i<line.length(); i++)
        if (line[i] != '0' and line[i] != '1')
            correct = false;

    return correct;
}


void MachLangParser::decode(Instruction& i)
{
    string opc = i.getEncoding().substr(0, 6);
    string funct = i.getEncoding().substr(26, 6);

    Opcode whichInst = opcodes.getInstr(opc, funct);
    InstType type = opcodes.getInstType(whichInst);

    if (type == 0)
        decodeRType(i);
    else if (type == 1)
        decodeIType(i);
    else // if (type == 2)
        decodeJType(i);
}


void MachLangParser::decodeRType(Instruction& i)
{
    string encoding = i.getEncoding();
    string opstr = encoding.substr(0, 6);
    string rs = encoding.substr(6, 5);
    string rt = encoding.substr(11, 5);
    string rd = encoding.substr(16, 5);
    string imm = encoding.substr(21, 5);
    string funct = encoding.substr(26, 6);

    Opcode opc = opcodes.getInstr(opstr, funct);

    i.setValues(
        opc,
        convertToInt(rs),
        convertToInt(rt),
        convertToInt(rd),
        convertToInt(imm)
    );

}


void MachLangParser::decodeIType(Instruction& i)
{
    string encoding = i.getEncoding();
    string opstr = encoding.substr(0, 6);
    string rs = encoding.substr(6, 5);
    string rt = encoding.substr(11, 5);
    string imm = encoding.substr(16, 16);

    Opcode opc = opcodes.getInstr(opstr, "");

    i.setValues(
        opc,
        convertToInt(rs),
        convertToInt(rt),
        32,
        convertToInt(imm)
    );
}


void MachLangParser::decodeJType(Instruction& i)
{
    string encoding = i.getEncoding();
    string opstr = encoding.substr(0, 6);
    string addr = encoding.substr(6, 26);

    Opcode opc = opcodes.getInstr(opstr, "");

    i.setValues(
        opc,
        32,
        32,
        32,
        convertToInt(addr)
    );
}


int MachLangParser::convertToInt(string s)
{
    std::bitset<ARCH_NUM_BITS> bits(s);
    return (int)bits.to_ulong();
}


void MachLangParser::assemble(Instruction& i)
// DEBUG TODO
{
    string assembled = "";
    assembled += opcodes.getName(i.getOpcode()) + "\t";
    InstType type = opcodes.getInstType(i.getOpcode());

    if (type == 0)
        assembled += assembleRType(i);
    else if (type == 1)
        assembled += assembleIType(i);
    else // if (type == 2)
        assembled += assembleJType(i);

    cout << "assembled:\t" << assembled << endl;
    i.setAssembly(assembled);
}


string MachLangParser::assembleRType(Instruction i)
{
    string assembled = "";
    Opcode opc = i.getOpcode();

    for (int i = 0; i < opcodes.numOperands(opc); i++)
    {
        if (opcodes.RSposition(opc) == i)
        {

        }
        if (opcodes.RTposition(opc) == i)
        {

        }
        if (opcodes.RDposition(opc) == i)
        {

        }
        if (opcodes.IMMposition(opc) == i)
        {

        }
    }

    return "";
}


string MachLangParser::assembleIType(Instruction i)
{
    return "";
}


string MachLangParser::assembleJType(Instruction i)
{
    return "";
}

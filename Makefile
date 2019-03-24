# makefile
# use to compile and test the assembler ASM for the STACK computer and
# its various components

DEBUG_FLAG= -DDEBUG -g -Wall
CFLAGS=-DDEBUG -g -Wall

.SUFFIXES: .cpp .o

.cpp.o:
	g++ $(CFLAGS) -c $<

DisASM: DisASM.o Instruction.o OpcodeTable.o MachLangParser.o
	g++ -o DisASM DisASM.o Instruction.o OpcodeTable.o MachLangParser.o 

MachLangParser.o: MachLangParser.h OpcodeTable.h Instruction.h 

Instruction.o: OpcodeTable.h Instruction.h 

OpcodeTable.o: OpcodeTable.h 

test: DisASM
	./DisASM inst.mach > inst.out~
	diff -y inst.out inst.out~

clean:
	/bin/rm -f DisASM *.o core



//
//		Assembler class.  This is a container for all the components
//		that make up the assembler.
//
#pragma once 

#include "SymTab.h"
#include "Instruction.h"
#include "FileAccess.h"
#include "Emulator.h"


class Assembler {

public:
    Assembler( int argc, char *argv[] );
    ~Assembler( );

    // Pass I - establish the locations of the symbols
    void PassI( );

    // Pass II - generate a translation
    void PassII( );

    // Display the symbols in the symbol table.
    void DisplaySymbolTable();
    
    // Run emulator on the translation.
    void RunProgramInEmulator() { m_emul.runProgram(); }

    // Convert to opcode
    int ConvertToOpcode(const string& opcode);

private:

    FileAccess m_facc;	    // File Access object
    SymbolTable m_symtab;	// Symbol table object
    Instruction m_inst;	    // Instruction object
    emulator m_emul;        // Emulator object
    int m_startingLocation;
};


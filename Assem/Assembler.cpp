//
//      Implementation of the Assembler class.
//

#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"

// Constructor for the assembler.  Note: we are passing argc and argv to the file access constructor.
// See main program.  
Assembler::Assembler( int argc, char *argv[] )
	: m_facc(argc, argv)
{
    // Nothing else to do here at this point.
}
// Destructor currently does nothing.  You might need to add something as you develope this project.
Assembler::~Assembler( )
{
}

// Prints the symbol table after the first pass through
void Assembler::DisplaySymbolTable() {

        m_symtab.DisplaySymbolTable(); 

}

// Pass I establishes the location of the labels.  You will write better function comments according to the coding standards.
void Assembler::PassI()
{
    int loc = 0;        // Tracks the location of the instructions to be generated.

    // Successively process each line of source code.
    while (true) {

        // Read the next line from the source file.
        string line;
        if (!m_facc.GetNextLine(line)) {

            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }

        // line has the next line from the file to be read
        // check if the line is an org directive
        if (line.substr(0,3) == "org") {

            // create an istringstream intialized to the line returned from getline
            istringstream parseLine(line);

            // create two strings to place the directive and operand into
            string directive,
                operand;

            // Place the istringstream contents into the string variables
            parseLine >> directive >> operand;

            // set loc based on org
            loc = stoi(operand);
            continue;
        }


            // Parse the line and get the instruction type.
            Instruction::InstructionType st = m_inst.ParseInstruction(line);

            // If this is an end statement, there is nothing left to do in pass I.
            // Pass II will determine if the end is the last statement.
            if (st == Instruction::ST_End) return;

            // Labels can only be on machine language and assembler language
            // instructions.  So, skip other instruction types. We can do better/
            if (st == Instruction::ST_Comment)
            {
                continue;
            }
            // If the instruction has a label, record it and its location in the
            // symbol table.
            if (m_inst.isLabel()) {
                cout << "Adding label: " << m_inst.GetLabel() << " at location: " << loc << endl;
                m_symtab.AddSymbol(m_inst.GetLabel(), loc);
            }
            // Compute the location of the next instruction.
            loc = m_inst.LocationNextInstruction(loc);
        }
    }




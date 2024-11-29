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

// Returns the opcode number for the string label that its assosicated with
int Assembler::ConvertToOpcode(const string& opcode) {

    // Create a map to represent the two values, string and integer, that are assosciated with eachother
    static map<string, int> opcodeTable = {

        {"ADD", 1}, {"SUB", 2}, {"MULT", 3}, {"DIV", 4}, {"LOAD", 5}, {"STORE", 6},
        {"READ", 7}, {"WRITE", 8}, {"B", 9}, {"BM", 10}, {"BZ", 11}, {"BP", 12},
        {"HALT", 13}
    };

    if (opcodeTable.find(opcode) != opcodeTable.end()) {
        return opcodeTable[opcode];
    }

    // If there is an invalid opcode, return -1 (fix this later)
    return -1;

}

// Pass I establishes the location of the labels.  You will write better function comments according to the coding standards.
void Assembler::PassI()
{
    int loc = 0; // Tracks the location of the instructions to be generated.

    // Successively process each line of source code.
    while (true)
    {
        // Read the next line from the source file.
        string line;
        if (!m_facc.GetNextLine(line))
        {
            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }


        string trimmedLine = line;
        trimmedLine.erase(0, trimmedLine.find_first_not_of(" \t")); // Remove leading whitespace.
        transform(trimmedLine.begin(), trimmedLine.end(), trimmedLine.begin(), ::tolower);
        // Check if the line is an org directive
        cout << "Processing line in Pass I: " << line << endl;

        if (trimmedLine.substr(0, 3) == "org")
        {
            istringstream parseLine(trimmedLine);
            string directive, operand;
            parseLine >> directive >> operand;

            loc = stoi(operand);
            m_startingLocation = loc; // Record starting location for Pass II.

            cout << "Set starting location to: " << loc << " (org directive)" << endl;
            continue;
        }

        // Parse the line and get the instruction type.
        Instruction::InstructionType st = m_inst.ParseInstruction(line);

        // If this is an end statement, there is nothing left to do in Pass I.
        if (st == Instruction::ST_End)
            return;

        // Skip comments and blank lines.
        if (st == Instruction::ST_Comment)
        {
            continue;
        }

        // If the instruction has a label, record it and its location in the symbol table.
        if (m_inst.isLabel())
        {
            cout << "Adding label: " << m_inst.GetLabel() << " at location: " << loc << endl;
            m_symtab.AddSymbol(m_inst.GetLabel(), loc);
        }

        // Handle assembler directives.
        if (st == Instruction::ST_AssemblerInstr)
        {
            if (m_inst.GetOpCode() == "ds")
            {
                int reserve = stoi(m_inst.GetOperand());
                loc += reserve;

                cout << "Reserved " << reserve << " locations (ds directive), new loc: " << loc << endl;
                continue;
            }
            if (m_inst.GetOpCode() == "dc")
            {
                cout << "Assigned value to location: " << loc << " (dc directive)" << endl;
                loc++;
                continue;
            }
        }

        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction(loc);
    }
}


// Pass II will read over the source file for a second time, generating machine code and checks for
// unresolved symbols

void Assembler::PassII() {
    m_facc.rewind();
    int loc = m_startingLocation;

    cout << "Translation of Program:\n" << endl;
    cout << "Location    Contents    Original Statement\n" << endl;

    string line;
    while (true) {
        if (!m_facc.GetNextLine(line)) {
            break; // End of file.
        }

        Instruction::InstructionType type = m_inst.ParseInstruction(line);

        // Skip comments and blank lines.
        if (type == Instruction::ST_Comment) {
            cout << setw(40) << line << endl;
            continue;
        }

        // Process assembler directives.
        if (type == Instruction::ST_AssemblerInstr) {
            string opcode = m_inst.GetOpCode();

            if (opcode == "dc") {
                int value = stoi(m_inst.GetOperand());
                cout << setw(10) << loc << setw(10) << value << setw(30) << line << endl;
                loc++; // Increment by 1 for dc
            }
            else if (opcode == "ds") {
                int reserve = stoi(m_inst.GetOperand());
                cout << setw(10) << loc << setw(10) << "" << setw(30) << line << endl;
                loc += reserve; // Increment by the storage size
            }
            continue;
        }

        // Process machine instructions.
        if (type == Instruction::ST_MachineLanguage) {
            string opcode = m_inst.GetOpCode();
            transform(opcode.begin(), opcode.end(), opcode.begin(), ::toupper);

            int opcodeValue = ConvertToOpcode(opcode);
            if (opcodeValue == -1) {
                Errors::RecordError("Invalid opcode: " + opcode);
                cout << "Error: Invalid opcode -> " << opcode << endl;
                continue;
            }

            int operand = m_inst.isNumericOperand() ? m_inst.GetOperandValue()
                : m_symtab.GetAddress(m_inst.GetOperand());

            if (operand == -1) {
                Errors::RecordError("Undefined symbol: " + m_inst.GetOperand());
                cout << "Error: Undefined symbol -> " << m_inst.GetOperand() << endl;
                continue;
            }

            int machineInstruction = opcodeValue * 1000 + operand;

            cout << setw(10) << loc << setw(10) << machineInstruction << setw(30) << line << endl;
            loc++; // Increment by 1 for each machine instruction
        }

        if (type == Instruction::ST_End) {
            cout << setw(40) << "end" << endl;
            break;
        }
    }
}










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

        // Check if the line is an org directive
        if (line.substr(0, 3) == "org")
        {
            istringstream parseLine(line);
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

    // Rewind the file for Pass II
    m_facc.rewind();
    //cout << "Testing if file starts reading from the beginning..." << endl;

    string line;
    /*
    if (m_facc.GetNextLine(line)) {
        cout << "First line: " << line << endl;
    }
    else {
        cout << "Failed to read the first line after rewinding." << endl;
    }
    */
    /*
    
    // Temporary tests for ConvertToOpcode
    cout << "Testing ConvertToOpcode..." << std::endl;
    cout << "Opcode for ADD: " << ConvertToOpcode("ADD") << std::endl;
    cout << "Opcode for HALT: " << ConvertToOpcode("HALT") << std::endl;
    cout << "Opcode for INVALID: " << ConvertToOpcode("INVALID") << std::endl;

    // Example of parsing a line and testing GetOpCode
    cout << "Testing GetOpCode..." << std::endl;
    m_inst.ParseInstruction("LOAD X");
    cout << "Parsed opcode: " << m_inst.GetOpCode() << std::endl;
    m_inst.ParseInstruction("STORE Y");
    cout << "Parsed opcode: " << m_inst.GetOpCode() << std::endl;
    */

    // Test for the symbol table returning correct address values
    
    /*
    cout << "Testing GetAddress..." << endl;
    string testOperand = "x"; // Replace with actual operand expected to exist in the symbol table
    int address = m_symtab.GetAddress(testOperand);
    cout << "Address retrieved for operand '" << testOperand << "': " << address << endl;
    testOperand = "y";
    address = m_symtab.GetAddress(testOperand);
    cout << "Address retrieved for operand '" << testOperand << "': " << address << endl;
    */
    
    int loc = m_startingLocation;  // Initialize to the start location from Pass I

    while (true) {

        // Read the next line from the source file.
        string line;

        // Rewind the file

        if (!m_facc.GetNextLine(line)) {

            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }


        // Parse the line to get opcode and operand
        Instruction::InstructionType type = m_inst.ParseInstruction(line);

        if (type == Instruction::ST_MachineLanguage || type == Instruction::ST_AssemblerInstr) {
            // Convert to machine code
            int opcode = ConvertToOpcode(m_inst.GetOpCode());
            
            if(int operand = m_inst.isNumericOperand()){
                
                // If we are here, then the operand has a numeric value
                m_inst.GetOperandValue();
            }
            else {

                // If we are here, than the operand does not have a numeric value, so we need to 
                // find the address by using the symbol table
                m_symtab.GetAddress(m_inst.GetOperand());
            }  


            
            /*
            // Generate the machine instruction in the form: <opcode><operand>
            int machineInstruction = GenerateMachineCode(opcode, operand);

            // Output the formatted line
            std::cout << std::setw(5) << loc << std::setw(10) << machineInstruction
                << std::setw(20) << line << std::endl;

            loc = m_inst.LocationNextInstruction(loc);

            
            */
        }

        
    }
    
}




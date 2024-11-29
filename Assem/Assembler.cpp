//
//      Implementation of the Assembler class.
//

#include "stdafx.h"
#include "Assembler.h"
#include "Instruction.h"
#include "Errors.h"

string TrimString(const string& str) {
    size_t first = str.find_first_not_of(" \t");
    if (first == string::npos) return ""; // All whitespace
    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, (last - first + 1));
}

string RemoveComment(string line) {
    size_t pos = line.find(';');
    if (pos == string::npos) {
        return line;
    }
    return line.erase(pos);  // Remove everything after the comment delimiter.
}

bool ParseLine(const string& line, string& label, string& opcode, string& operand) {
    istringstream ins(line);
    label = opcode = operand = "";

    if (line.empty()) return true;

    string token;
    vector<string> tokens;

    // Split the line into tokens.
    while (ins >> token) {
        tokens.push_back(token);
    }

    // Case 1: Line has a label, opcode, and operand.
    if (tokens.size() == 3) {
        label = tokens[0];
        opcode = tokens[1];
        operand = tokens[2];
    }
    // Case 2: Line has only opcode and operand.
    else if (tokens.size() == 2) {
        opcode = tokens[0];
        operand = tokens[1];
    }
    // Case 3: Line has only an opcode (e.g., "halt").
    else if (tokens.size() == 1) {
        opcode = tokens[0];
    }
    // Case 4: Line has extra tokens.
    else if (tokens.size() > 3) {
        return false;  // Extra tokens indicate an error.
    }

    return true;
}



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
    // Standardize opcode to uppercase
    string upperOpcode = opcode;
    transform(upperOpcode.begin(), upperOpcode.end(), upperOpcode.begin(), ::toupper);

    static map<string, int> opcodeTable = {
        {"ADD", 1}, {"SUB", 2}, {"MULT", 3}, {"DIV", 4}, {"LOAD", 5},
        {"STORE", 6}, {"READ", 7}, {"WRITE", 8}, {"B", 9}, {"BM", 10},
        {"BZ", 11}, {"BP", 12}, {"HALT", 13}
    };

    if (opcodeTable.find(upperOpcode) != opcodeTable.end()) {
        return opcodeTable[upperOpcode];
    }

    return -1; // Invalid opcode
}



// Pass I establishes the location of the labels.  You will write better function comments according to the coding standards.
// Pass I establishes the location of the labels.
// Pass I establishes the location of the labels.
// Pass I - Location calculation for labels and instructions
void Assembler::PassI() {
    int loc = 0; // Tracks the location of the instructions to be generated.
    bool orgProcessed = false; // Flag to track if org directive has been processed.

    // Successively process each line of source code.
    while (true) {
        string line;
        if (!m_facc.GetNextLine(line)) {
            // If there are no more lines, we are missing an end statement.
            return;
        }

        string trimmedLine = line;
        trimmedLine = TrimString(trimmedLine);
        //trimmedLine.erase(0, trimmedLine.find_first_not_of(" \t")); // Remove leading whitespace.
        transform(trimmedLine.begin(), trimmedLine.end(), trimmedLine.begin(), ::tolower);

        //cout << "Processing line in Pass I: " << line << endl;
        //cout << "Current Loc: " << loc << endl;

        // Check for org directive
        if (trimmedLine.substr(0, 3) == "org" && !orgProcessed) {
            istringstream parseLine(trimmedLine);
            string directive, operand;
            parseLine >> directive >> operand;

            loc = stoi(operand);  // Set location for org directive.
            m_startingLocation = loc;
            orgProcessed = true;  // Mark org as processed
            cout << "Set starting location to: " << loc << " (org directive)" << endl;
            continue;
        }

        // Parse instruction type
        Instruction::InstructionType st = m_inst.ParseInstruction(line);

        if (st == Instruction::ST_End)
            return;

        // Skip comments and blank lines
        if (st == Instruction::ST_Comment) {
            continue;
        }

        // If the instruction has a label, record it and its location
        if (m_inst.isLabel()) {
            cout << "Adding label: " << m_inst.GetLabel() << " at location: " << loc << endl;
            m_symtab.AddSymbol(m_inst.GetLabel(), loc);
        }

        // Handle assembler directives (ds, dc)
        if (st == Instruction::ST_AssemblerInstr) {
            if (m_inst.GetOpCode() == "ds") {
                int reserve = stoi(m_inst.GetOperand());
                cout << "Reserved " << reserve << " locations (ds directive), new loc: " << loc << endl;
                loc += reserve;  // Manually update location for ds
                continue;
            }
            if (m_inst.GetOpCode() == "dc") {
                // Handle dc directive (assign value and increment location)
                cout << "Assigned value to location: " << loc << " (dc directive)" << endl;
                loc++;  // Manually increment for dc
                continue;
            }
        }

        // Compute the location of the next instruction
        
            loc = m_inst.LocationNextInstruction(loc);
        
       
          // Now this will be used only for regular instructions
    }
}





// Pass II will read over the source file for a second time, generating machine code and checks for
// unresolved symbols

void Assembler::PassII() {
    m_facc.rewind();
    int loc = m_startingLocation;

    cout << "Translation of Program:\n" << endl;
    cout << setw(10) << "Location" << setw(15) << "Contents" << setw(40) << "Original Statement" << endl;

    string line;
    bool isOrgProcessed = false;  // Flag to track when the org directive is processed.

    while (true) {
        if (!m_facc.GetNextLine(line)) {
            break; // End of file.
        }

        // Store the original line for display purposes.
        string originalLine = line;

        // Remove comments and trim the line.
        line = RemoveComment(line);
        line = TrimString(line);

        // Skip blank lines.
        if (line.empty()) {
            continue;
        }

        // Skip printing locations before org is processed
        if (!isOrgProcessed) {
            if (line.substr(0, 3) == "org") {
                // Handle org directive
                string directive, operand;
                istringstream parseLine(line);
                parseLine >> directive >> operand;
                loc = stoi(operand);  // Set location for org directive.
                m_startingLocation = loc;
                isOrgProcessed = true;  // Mark org as processed
                cout << "Set starting location to: " << loc << " (org directive)" << endl;
                continue;
            }
            else {
                // Skip lines before org is processed (do not print location yet)
                continue;
            }
        }

        // Skip printing the location for `dc` and `ds` directives again
        bool isDirectivePrinted = false;

        // Parse the instruction
        string label, opcode, operand;
        bool isValid = ParseLine(line, label, opcode, operand);

        if (!isValid) {
            Errors::RecordError("Extra tokens in line: " + originalLine);
            cout << "Error: Extra tokens in line -> " << originalLine << endl;
            continue;
        }

        if (opcode.empty()) {
            Errors::RecordError("Missing opcode in line: " + originalLine);
            cout << "Error: Missing opcode -> " << originalLine << endl;
            continue;
        }

        // Handle assembler directives first
        if (opcode == "org") {
            loc = stoi(operand);
            continue;
        }

        // Handle `dc` directive (print only once)
        if (opcode == "dc" && !isDirectivePrinted) {
            int value = stoi(operand);
            cout << setw(10) << loc << setw(15) << value << setw(40) << originalLine << endl;
            loc++;  // DC increases the location by 1
            isDirectivePrinted = true;
            continue;
        }

        // Handle `ds` directive (print only once)
        if (opcode == "ds" && !isDirectivePrinted) {
            int reserve = stoi(operand);
            cout << setw(10) << loc << setw(15) << "reserved space" << setw(40) << originalLine << endl;
            loc += reserve; // DS reserves multiple locations
            isDirectivePrinted = true;
            continue;
        }

        if (opcode == "end") {
            // No machine instruction is generated for 'end', so break the loop.
            cout << setw(10) << loc << setw(15) << "" << setw(40) << originalLine << endl;
            break; // End processing
        }

        // Handle `halt` (opcode conversion)
        if (opcode == "halt") {
            int machineInstruction = ConvertToOpcode(opcode) * 1000;
            cout << setw(10) << loc << setw(15) << machineInstruction << setw(40) << originalLine << endl;
            loc++;  // Increment location for `halt`
            continue;
        }

        // Handle machine instructions (opcode conversion)
        int opcodeValue = ConvertToOpcode(opcode);
        if (opcodeValue == -1) {
            Errors::RecordError("Invalid opcode: " + opcode);
            cout << "Error: Invalid opcode -> " << opcode << endl;
            continue;
        }

        int operandValue = m_inst.isNumericOperand() ? m_inst.GetOperandValue()
            : m_symtab.GetAddress(operand);

        if (operandValue == -1) {
            Errors::RecordError("Undefined symbol: " + operand);
            cout << "Error: Undefined symbol -> " << operand << endl;
            continue;
        }

        int machineInstruction = opcodeValue * 1000 + operandValue;
        cout << setw(10) << loc << setw(15) << machineInstruction << setw(40) << originalLine << endl;
        loc++;
    }
}







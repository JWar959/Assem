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
    istringstream ins(TrimString(line)); // Normalize line input.
    label = opcode = operand = "";

    string token;
    vector<string> tokens;

    while (ins >> token) {
        tokens.push_back(token);
    }

    if (tokens.size() == 3) {
        label = tokens[0];
        opcode = tokens[1];
        operand = tokens[2];
    }
    else if (tokens.size() == 2) {
        opcode = tokens[0];
        operand = tokens[1];
    }
    else if (tokens.size() == 1) {
        opcode = tokens[0];
    }
    else if (tokens.size() > 3) {
        return false; // Too many tokens.
    }

    label = TrimString(label);
    opcode = TrimString(opcode);
    operand = TrimString(operand);

    return true;
}

// Constructor for the assembler.
Assembler::Assembler(int argc, char* argv[])
    : m_facc(argc, argv) {
}

// Destructor
Assembler::~Assembler() {}

void Assembler::DisplaySymbolTable() {
    m_symtab.DisplaySymbolTable();
}

int Assembler::ConvertToOpcode(const string& opcode) {
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

void Assembler::PassI() {
    int loc = 0; // Tracks the location of the instructions to be generated.
    bool orgProcessed = false; // Flag to track if org directive has been processed.

    while (true) {
        string line;
        if (!m_facc.GetNextLine(line)) {
            return; // End of file.
        }

        string trimmedLine = RemoveComment(line);
        trimmedLine = TrimString(trimmedLine);

        if (trimmedLine.empty()) continue; // Skip empty lines.

        if (trimmedLine.substr(0, 3) == "org" && !orgProcessed) {
            istringstream parseLine(trimmedLine);
            string directive, operand;
            parseLine >> directive >> operand;

            loc = stoi(operand);
            m_startingLocation = loc;
            orgProcessed = true;
            cout << "Set starting location to: " << loc << " (org directive)" << endl;
            continue;
        }

        Instruction::InstructionType st = m_inst.ParseInstruction(line);

        if (st == Instruction::ST_End) return; // End processing.

        if (st == Instruction::ST_Comment) continue; // Skip comments.

        if (m_inst.isLabel()) {
            cout << "Adding label: " << m_inst.GetLabel() << " at location: " << loc << endl;
            m_symtab.AddSymbol(m_inst.GetLabel(), loc);
        }

        if (st == Instruction::ST_AssemblerInstr) {
            if (m_inst.GetOpCode() == "ds") {
                int reserve = stoi(m_inst.GetOperand());
                cout << "Reserved " << reserve << " locations (ds directive), new loc: " << loc << endl;
                loc += reserve;
                continue;
            }
            if (m_inst.GetOpCode() == "dc") {
                if (m_inst.GetLabel().empty()) {
                    Errors::RecordError("DC directive missing a label at location: " + to_string(loc));
                    cout << "Error: DC directive missing a label at location: " << loc << endl;
                    continue;
                }
                int value = stoi(m_inst.GetOperand());
                cout << "Processing DC directive for label: " << m_inst.GetLabel()
                    << " Value: " << value << " Location: " << loc << endl;
                loc++;
                continue;
            }
        }

        loc = m_inst.LocationNextInstruction(loc); // Update location for next instruction.
    }
}

#include <format> // Include this at the top of your file.

#include <format> // For std::format

void Assembler::PassII() {
    m_facc.rewind();  // Rewind the file to the beginning.
    int loc = m_startingLocation;

    // Headers for the translation table
    cout << "Translation of Program:\n" << endl;
    cout << std::format("{:<10} {:<10} {:<40}", "Location", "Contents", "Original Statement") << endl;

    while (true) {
        string line;
        if (!m_facc.GetNextLine(line)) break;

        string originalLine = line;  // Save the original line for output.
        line = RemoveComment(line);
        line = TrimString(line);

        // Handle standalone comments
        if (!originalLine.empty() && originalLine[0] == ';') {
            cout << std::format("{:<10} {:<10} {:<40}", "", "", originalLine) << endl;
            continue;
        }

        // Skip blank lines after trimming
        if (line.empty()) continue;

        string label, opcode, operand;
        bool isValid = ParseLine(line, label, opcode, operand);

        // Report and skip invalid lines
        if (!isValid) {
            Errors::RecordError("Extra tokens in line: " + originalLine);
            cout << "Error: Extra tokens in line -> " << originalLine << endl;
            continue;
        }

        // Handle `org` directive
        if (opcode == "org") {
            cout << std::format("{:<10} {:<10} {:<40}", 0, "", originalLine) << endl;
            loc = stoi(operand);  // Update location after printing.
            continue;
        }

        // Handle `dc` directive
        if (opcode == "dc") {
            if (label.empty()) {
                Errors::RecordError("DC directive missing a label at location: " + to_string(loc));
                cout << "Error: DC directive missing a label at location: " << loc << endl;
                continue;
            }
            try {
                int value = stoi(operand);
                cout << std::format("{:<10} {:<10} {:<40}", loc, std::format("{:06}", value), originalLine) << endl;
                loc++;
            }
            catch (const invalid_argument&) {
                Errors::RecordError("Invalid operand for dc directive: " + operand);
                cout << "Error: Invalid operand for dc directive -> " << operand << endl;
            }
            continue;
        }

        // Handle `ds` directive
        if (opcode == "ds") {
            try {
                int reserve = stoi(operand);
                cout << std::format("{:<10} {:<10} {:<40}", loc, "reserved", originalLine) << endl;
                loc += reserve;
            }
            catch (const invalid_argument&) {
                Errors::RecordError("Invalid operand for ds directive: " + operand);
                cout << "Error: Invalid operand for ds directive -> " << operand << endl;
            }
            continue;
        }

        // Handle `end` directive
        if (opcode == "end") {
            cout << std::format("{:<10} {:<10} {:<40}", loc, "", originalLine) << endl;
            break;
        }

        // Handle machine instructions
        int opcodeValue = ConvertToOpcode(opcode);
        if (opcodeValue == -1) {
            Errors::RecordError("Invalid opcode: " + opcode);
            cout << "Error: Invalid opcode -> " + opcode << endl;
            continue;
        }

        // Handle operands for instructions
        int operandValue = 0;  // Default for instructions like `halt`
        if (!operand.empty()) {
            operandValue = m_inst.isNumericOperand() ? m_inst.GetOperandValue()
                : m_symtab.GetAddress(operand);

            if (operandValue == -1) {
                Errors::RecordError("Undefined symbol: " + operand);
                cout << "Error: Undefined symbol -> " << operand << endl;
                continue;
            }
        }

        int machineInstruction = opcodeValue * 10000 + operandValue;
        cout << std::format("{:<10} {:<10} {:<40}", loc, std::format("{:06}", machineInstruction), originalLine) << endl;

        loc++;
    }
}










#include "stdafx.h"

#include "Instruction.h"

Instruction::InstructionType Instruction::ParseInstruction(string a_buff) {

    // Clear the Label at the start of each new line
    m_Label.clear();

    m_instruction = a_buff;

    // Remove comments from the instruction line
    a_buff = RemoveComment(a_buff);

    // Extract label, opcode, and operand using GetLabelOpcodeEtc
    GetLabelOpcodeEtc(a_buff);

    // Determine the instruction type based on the opcode
    if (m_OpCode == "end") return ST_End;
    if (m_OpCode == "org" || m_OpCode == "dc" || m_OpCode == "ds") return ST_AssemblerInstr;
    if (m_OpCode == "add" || m_OpCode == "sub" || m_OpCode == "mult" || m_OpCode == "div" ||
        m_OpCode == "load" || m_OpCode == "store" || m_OpCode == "read" || m_OpCode == "write" ||
        m_OpCode == "b" || m_OpCode == "bm" || m_OpCode == "bz" || m_OpCode == "bp" ||
        m_OpCode == "halt") {
        return ST_MachineLanguage;
    }

    // If the line is empty or just a comment, return ST_Comment
    return m_OpCode.empty() ? ST_Comment : ST_AssemblerInstr;

}
string Instruction::RemoveComment(string line) {
    size_t pos = line.find(';');
    if (pos == string::npos)
    {
        return line;
    }
    return line.erase(pos);  // Note: erase changed line also.  That is why it was passed by value.
}

void Instruction::GetLabelOpcodeEtc(const string& a_buff) {
    istringstream parseLine(a_buff);
    string label, opcode, operand;

    // Try to read three parts: label, opcode, and operand
    if (parseLine >> label >> opcode >> operand) {
        m_Label = label;
        m_OpCode = opcode;
        m_Operand = operand;
    }
    else if (parseLine >> opcode >> operand) {
        m_Label.clear();  // No label
        m_OpCode = opcode;
        m_Operand = operand;
    }
    else if (parseLine >> opcode) {
        m_Label.clear();  // No label
        m_OpCode = opcode;
        m_Operand.clear();  // No operand
    }
}



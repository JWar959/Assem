//
// Class to parse and provide information about instructions.  Note: you will be adding more functionality.
//
#pragma once

// The elements of an instruction.
class Instruction {

public:

    Instruction()
        : m_Label(""),
        m_OpCode(""),
        m_Operand(""),
        m_instruction(""),
        m_NumOpCode(0),
        m_type(ST_Comment),
        m_IsNumericOperand(false),
        m_OperandNumValue(0)
    { }
    ~Instruction() { };

    // Codes to indicate the type of instruction we are processing.  Why is this inside the
    // class?
    enum InstructionType {
        ST_MachineLanguage, 		// A machine language instruction.
        ST_AssemblerInstr,  		// Assembler Language instruction.
        ST_Comment,          		// Comment or blank line
        ST_End                   	// end instruction.
    };
    // Parse the Instruction.  VICVIC
    InstructionType ParseInstruction(string a_buff);


    // return ST_Comment; 


 // Compute the location of the next instruction.
    int LocationNextInstruction(int a_loc) { return a_loc + 1; }

    // To access the label
    string& GetLabel() {

        return m_Label;
    };
    // To determine if a label is blank.
    bool isLabel() {

        return !m_Label.empty();
    };


private:

    void GetLabelOpcodeEtc(const string& a_buff);
    string RemoveComment(string line);


    // The elemements of a instruction
    string m_Label;         // The label.
    string m_OpCode;        // The symbolic op code.
    string m_Operand;       // The operand.


    string m_instruction;   // The original instruction.

    // Derived values.
    int m_NumOpCode;// The numerical value of the op code.  Only applicable for machine language instructions.
    InstructionType m_type; // The type of instruction.
    bool m_IsNumericOperand;// == true if the operand is numeric.
    int m_OperandNumValue;// The value of the operand if it is numeric.
 
};



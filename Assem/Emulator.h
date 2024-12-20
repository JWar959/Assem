//
//		Emulator class - supports the emulation of VC370 programs
//
#ifndef _EMULATOR_H      // A previous way of preventing multiple inclusions.
#define _EMULATOR_H

#include "stdafx.h"

class emulator {

public:

    const static int MEMSZ = 10'000;	// The size of the memory of the VC370.
    emulator() {

		//set whole array to 0
        memset( m_memory, 0, MEMSZ * sizeof(int) );
        m_accum = 0;
    }
    // Records instructions and data into VC370 memory.
	bool insertMemory(int a_location, int a_contents)
	{
		if (a_location >= 0 && a_location < MEMSZ)
		{
			m_memory[a_location] = a_contents;
			return true;
		}
		else
		{

			// Leave a better error message here
			cerr << "Grumble gumble - should not happen" << endl;
			return false;
		}
	}
    
    // Runs the VC370 program recorded in memory.
	bool runProgram()
	{
        int loc = 100; // Starting location from org directive

        while (true) {
            // Fetch the instruction
            int contents = m_memory[loc];
            int opcode = contents / 10000;
            int address = contents % 10000;

            // Debugging output
            cout << "Location: " << loc << ", Contents: " << contents
                << ", Opcode: " << opcode << ", Address: " << address << endl;

            // Decode and execute
            switch (opcode) {
            case 1: // ADD
                m_accum += m_memory[address];
                break;
            case 2: // SUB
                m_accum -= m_memory[address];
                break;
            case 5: // LOAD
                m_accum = m_memory[address];
                break;
            case 6: // STORE
                m_memory[address] = m_accum;
                break;
            case 7: // READ
                cout << "? ";
                cin >> m_memory[address];
                break;
            case 8: // WRITE
                cout << m_memory[address] << endl;
                break;
            case 12:
                if (m_accum > 0) {
                    loc = address;  // Jump to the specified address
                    continue;       // Skip incrementing loc; jump is handled
                }
                break;  // Otherwise, fall through to the next instruction                
            case 13: // HALT
                cout << "\nEnd of emulation" << endl;
                return true;
            default:
                cerr << "Illegal opcode at location " << loc << ": " << opcode << endl;
                return false;
            }

            loc++;
            if (loc >= MEMSZ) {
                cerr << "Error: Program counter exceeded memory bounds." << endl;
                return false;
            }
        }
	}



private:

    int m_memory[MEMSZ];    // The memory of the VC370.  Would have to make it
    						// a vector if it was much larger.
    int m_accum;		    	// The accumulator for the VC370
};

#endif


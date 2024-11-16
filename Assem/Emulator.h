//
//		Emulator class - supports the emulation of VC370 programs
//
#ifndef _EMULATOR_H      // A previous way of preventing multiple inclusions.
#define _EMULATOR_H

class emulator {

public:

    const static int MEMSZ = 10'000;	// The size of the memory of the VC370.
    emulator() {

        memset( m_memory, 0, MEMSZ * sizeof(int) );
        m_accum = 0;
    }
    // Records instructions and data into VC370 memory.
	bool insertMemory(int a_location, int a_contents)
	{
		if (a_location >= 0 && a_location < MEMSZ)
		{
			m_memory[a_location] = a_contents;
		}
		else
		{
			cerr << "Grumble gumble - should not happen" << endl;
		}
	}
    
    // Runs the VC370 program recorded in memory.
	bool runProgram()
	{
		int loc = 100;
		while (true)
		{
		    int contents = m_memory[loc];
			int opcode = contents / 10'000;
			int address = contents % 10'000;

			switch (opcode)
			{
			case 1:
				m_accum += m_memory[address];
				loc += 1;
				continue;
				// Other opcode 

			default:
				cerr << "Illegal opcode" << endl;
				exit(1);
			}
		}
	}

private:

    int m_memory[MEMSZ];    // The memory of the VC370.  Would have to make it
    						// a vector if it was much larger.
    int m_accum;		    	// The accumulator for the VC370
};

#endif


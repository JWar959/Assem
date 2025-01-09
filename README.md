Assembler Project

-- Overview -- 

This project is a custom assembler built as part of a Software Design course at Ramapo College of New Jersey. 
The assembler is capable of translating assembly language instructions into machine code for a hypothetical computer 
system. It also includes error handling, symbol table management, and an emulator for executing the generated machine code.

The project showcases core programming concepts, such as file I/O, parsing, data structures, and object-oriented design 
principles, while emphasizing modularity and robust error handling.

-- Features --

Pass I and Pass II Assembler Implementation:

Pass I: Parses assembly instructions, builds the symbol table, and calculates addresses.
Pass II: Generates machine code, resolves symbols, and handles various assembly scenarios.
Symbol Table Management:

Efficiently stores and retrieves symbols using a map-like data structure.
Detects duplicate symbols and undefined references.
Error Reporting:

Comprehensive error detection, including invalid opcodes, undefined labels, and address overflow.
Deferred detailed error reporting for flexibility and debugging.
File Handling:

Reads assembly input files and writes output to machine code files.
Supports comment removal and line parsing for clean processing.
Emulator:

Simulates the execution of machine code to verify correctness.
Implements robust input validation for runtime instructions.


-- Project Structure --

Assembler/
├── Assem/                  # Source files for assembler implementation
├── Emulator/               # Emulator to execute machine code
├── LICENSE                 # License file (MIT or chosen license)
├── README.md               # Documentation file
└── Tests/                  # Sample input files and expected outputs

-- How to Build and Run -- 

1. Clone the repository:
git clone https://github.com/your-username/assembler-project.git
cd assembler-project

2. Compile the project:
g++ -std=c++17 -o assembler Assem/*.cpp Emulator/*.cpp

3. Run the assembler:
./assembler input.asm output.obj

4. Run the emulator (optional):
./emulator output.obj

-- Usage --

Input: Assembly language file (.asm) containing valid assembly instructions and labels.
Output: Machine code file (.obj) generated based on the input assembly.

-- Key Learning Points -- 

This project provided hands-on experience with:

- Designing modular software systems with clear separation of concerns.
- Implementing and utilizing data structures, including maps and vectors.
- Handling complex workflows, such as multi-pass assembly translation.
- Debugging and validating outputs through rigorous testing.

-- Acknowledgments --
This project was completed as part of a course on software design taught by Professor 
Victor Miller.

-- License -- 
This project is licensed under the MIT License. You are free to use, modify, and distribute 
this project under the terms of the license.

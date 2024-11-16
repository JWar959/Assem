//
// Class to manage error reporting. Note: all members are static so we can access them anywhere.
// What other choices do we have to accomplish the same thing?
//
#ifndef _ERRORS_H
#define _ERRORS_H

#include <string>
#include <vector>
using namespace std;

class Errors {

public:
    
    // Initializes error reports.
    static void InitErrorReporting()
    {
        m_ErrorMsgs.clear();
    }

    // Records an error message.
	static void RecordError(const string a_emsg) {
		m_ErrorMsgs.push_back(a_emsg);
        m_WasErrorMessages = true;
	}
    static bool WasThereErrors() { return m_WasErrorMessages; }

    // Displays the collected error message.
    static void DisplayErrors()
    {
        // Display errors  Please add the code.

        // Errase error messages
        m_ErrorMsgs.clear();
    }

private:
	// Note that you will have to declare these variables in a .cpp file.
    static vector<string> m_ErrorMsgs;
    static bool m_WasErrorMessages;
};
#endif
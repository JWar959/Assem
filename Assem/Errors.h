//
// Class to manage error reporting. Note: all members are static so we can access them anywhere.
// What other choices do we have to accomplish the same thing?
//
#ifndef _ERRORS_H
#define _ERRORS_H

#include "stdafx.h"

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

    static bool WasThereErrors(){ 
        
        return m_WasErrorMessages; 
    
    }

    // Displays the collected error message.
    static void DisplayErrors()
    {
        if (m_ErrorMsgs.empty()) {
            cout << "No errors reported." << endl;
            return;
        }

        cout << "Error Messages:" << endl;
        for (const auto& msg : m_ErrorMsgs) {
            cout << " - " << msg << endl;
        }

        // Clear error messages after displaying.
        m_ErrorMsgs.clear();
        m_WasErrorMessages = false;
    }

private:
	// Note that you will have to declare these variables in a .cpp file.
    static vector<string> m_ErrorMsgs;
    static bool m_WasErrorMessages;
};
#endif
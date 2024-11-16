//
//		Implementation of the symbol table class.  This is the format I want for commenting functions.
//
#include "stdafx.h"
#include "SymTab.h"

void SymbolTable::DisplaySymbolTable() {
    std::cout << "Symbol Table:\n";
    std::cout << "Symbol #    Symbol      Location\n";

    int index = 0;
    for (const auto& entry : m_symbolTable) {
        std::cout << std::setw(5) << index++       // Symbol #
            << std::setw(15) << entry.first  // Symbol (left-aligned)
            << std::setw(10) << entry.second // Location
            << std::endl;
    }
}

int SymbolTable::GetAddress(const string& operand) {
    
    // Create an iterator and have it search for the string value
    auto it = m_symbolTable.find(operand);

    if (it == m_symbolTable.end()) {
        // If we are here, then a matching value was not found
        return -1;
    }

     // return the iterator
    return it->second;

}

/*
NAME

    AddSymbol - adds a new symbol to the symbol table.

SYNOPSIS

    void AddSymbol( char *a_symbol, int a_loc );

DESCRIPTION

    This function will place the symbol "a_symbol" and its location "a_loc"
    in the symbol table.
*/
void 
SymbolTable::AddSymbol( string &a_symbol, int a_loc )
{
    // If the symbol is already in the symbol table, record it as multiply defined.
	map<string, int>::iterator st = m_symbolTable.find( a_symbol );
    if( st != m_symbolTable.end() ) {

        st->second = multiplyDefinedSymbol;
        return;
    }
    // Record a the  location in the symbol table.
    m_symbolTable[a_symbol] = a_loc;
}

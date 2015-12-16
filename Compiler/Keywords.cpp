#include "Compiler.h"

#define __KEYWORD_CPP__
#include "Keywords.h"



bool FuncKeyword(TRANSLATOR * Translator)
{
	return true;
}

bool VarKeyword(TRANSLATOR * Translator)
{
    return true;
}



KEYWORD_TABLE::TSTATIC_VAL Keywords[] = 
{
	{{KEYWORD_TABLE::EmptyElement,KEYWORD_TABLE::EmptyElement}, {"function", FuncKeyword}},
	{{KEYWORD_TABLE::EmptyElement,KEYWORD_TABLE::EmptyElement}, {"var", VarKeyword}}

};


unsigned CountKeywords = sizeof(Keywords) / sizeof(Keywords[0]);



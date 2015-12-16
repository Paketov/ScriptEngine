#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <memory.h>


#include "Compiler.h"
#include "Keywords.h"

void TRANSLATOR::OutError(ERRORS Error, LPINTERNAL_CHAR FormatMsg, ...)
{
	va_list ListArg;
	va_start(ListArg, FormatMsg);
	fprintf(ErrOut,"\n Error %ui in line %ui", Error, Line);
	vfprintf(ErrOut,FormatMsg, ListArg);
	fprintf(ErrOut,"\n");
	LastErr = Error;
}

bool TRANSLATOR::InitCommandTable()
{

	if(!CommandsTable.New(&CommandsTable,CountOpcodes))
		return false;
	CommandsTable.Init(CountOpcodes);

	for(unsigned char i = 0; i < CountOpcodes;i++)
	{
		COMMANDS_TABLE::LPCELL Cell = CommandsTable.InitInsert(NameOpcode[i].NameOpcode);
		Cell->El = NameOpcode + i;
	}
	return true;
}

bool TRANSLATOR::InitKeywordTable()
{
	KeywordsTable.StaticInit(CountKeywords, Keywords);
	return true;
}

TRANSLATOR::TRANSLATOR(LPINTERNAL_CHAR NameInFile)
{
	InitCommandTable();
	InitKeywordTable();
	LastErr = ERRORS::SUCCESS;
	LenWordBuf = 0;
	Line = 0;
	InFile = fopen(NameInFile, "rt");
	if(InFile == NULL)
	{
		OutError(ERRORS::NOT_OPEN_INPUT_FILE, "Not OpenFile: %s", NameInFile);
		return;
	}
	ErrOut = stderr;
	MaxLenNumber = 20;
	MaxLenExp = 3;
	OUT_STRIG_TABLE::New(&OutStrings, 20);
	OutStrings.Init(20);
	//ReadGlobalScope();

}

INTERNAL_CHAR TRANSLATOR::ReadWord(INTERNAL_CHAR FirstChar)
{
	WordBuf[0] = FirstChar;
	LenWordBuf = 1;
	isHaveDigits = false;
	INTERNAL_CHAR c;
	while(true)
	{
		c = getc(InFile);
		if(ISALPHA(c))
		{
			WordBuf[LenWordBuf++] = c;
		}else if(ISDIGIT(c))
		{
			isHaveDigits = true;
			WordBuf[LenWordBuf++] = c;
		}else
			break;
	}
	WordBuf[LenWordBuf] = '\0';
	return c;
}



INTERNAL_CHAR TRANSLATOR::ReadNumber(INTERNAL_CHAR FirstChar)
{
	INTERNAL_CHAR c;
	unsigned char Radix = 10;
	char Neg = 1;
	switch(FirstChar)
	{
	case T('-'):
		Neg = -1;
		c = getc(InFile);
		break;
	case T('+'):
		c = getc(InFile);
		break;
	case T('0'):
		switch(c = getc(InFile))
		{
		case T('X'):
		case T('x'):
			Radix = 16;
			goto lblReadNextC;
		case T('o'):
		case T('O'):
			Radix = 8;
			goto lblReadNextC;
		case T('b'):
		case T('B'):
			Radix = 2;
lblReadNextC:
			c = getc(InFile);
			goto lblStartIntrpret;
		default:
			fseek(InFile,sizeof(INTERNAL_CHAR)* -1,SEEK_CUR);
		}
	default:
		c = FirstChar;
	}
lblStartIntrpret:

	unsigned char CountReadedDigits = 0;	
	double Ret = 0.0;	
	unsigned char MinDigit = min(9, Radix);
	INTERNAL_CHAR s = (Radix > 10)?(T('a') - 10) :T('0');
	//Если число не десятичное, то переход к дробной части невозможен
	INTERNAL_CHAR Separator = (Radix != 10)? T('0'): T('.');
	while(true)
	{
		unsigned Digit = (c = getc(InFile)) - T('0');
		if(Digit >= MinDigit)
		{		
			Digit = c - s;
			if(Digit >= Radix)
			{
				if(c == Separator)
					goto lblFractional;
				else if(CountReadedDigits > 0)
					break;
				OutError(ERRORS::BAD_NUMBER, "Expected digit.");
				return c;
			}
		}
		Ret = Ret * Radix + Digit;
		if(++CountReadedDigits > MaxLenNumber)
		{
			OutError(ERRORS::OUT_MAX_LEN_NUMBER, "Number very big.");
			return c;
		}
	}
	isInt = true;
	iNum = Ret * Neg;
	return c;

lblFractional:
	static const INTERNAL_CHAR TestIfExpPlus[2] = {T('e'), T('+')};
	static const INTERNAL_CHAR TestIfExpMinus[2] = {T('e'), T('-')};
	c = getc(InFile);
	for(double a = 1.0 / Radix;;c = getc(InFile))
	{
		unsigned char Digit = c - T('0');
		if(Digit >= Radix)
		{
			if((CountReadedDigits > 0) && ((c == T('e')) || (c == T('E'))))
			{
				//Если намечается экспанента
				c = getc(InFile);
				char NegExp;
				if(c == T('+'))
					NegExp = 1;
				else if(c == T('-'))
					NegExp = -1;
				else
				{
					//Неправильное дробное
					OutError(ERRORS::BAD_NUMBER, "Expected + or -.");
					return c;
				}
				//Читаем экспаненту
				int Exp = 0;
				unsigned char LenExp = 0;
				for(c = getc(InFile);;c = getc(InFile))
				{
					unsigned Digit = c - T('0');
					if(Digit >= Radix)
					{
						if(LenExp)
							break;
						else
						{
							OutError(ERRORS::BAD_NUMBER, "Bad exp.");
							return c;
						}
					}
					Exp = Exp * Radix + Digit;
					if(++LenExp > MaxLenExp)
					{
						OutError(ERRORS::BAD_NUMBER, "Bad exp.");
						return c;
					}
				}
				Ret *= pow((double)Radix, Exp);
				goto lblOutDouble;
			}else
			{
				//Если пришла не цифра
				goto lblOutDouble;
			}
		}

		Ret = Ret + Digit * a;
		a /= Radix;
		if(++CountReadedDigits >= MaxLenNumber)
		{
			OutError(ERRORS::OUT_MAX_LEN_NUMBER, "Number very big.");
			return c;
		}
	}
lblOutDouble:
	isInt = false;
	dNum = Ret * Neg;
	return c;
}

bool TRANSLATOR::InterpretAsKeyword(LPINTERNAL_CHAR Lecsem)
{
	KEYWORD_TABLE::LPCELL KeywordHandler = KeywordsTable.Search(Lecsem);
	if(KeywordHandler == NULL)
		return false;
	return KeywordHandler->KeywordProc(this);
}


bool TRANSLATOR::InterpretAsIdentificator(LPINTERNAL_CHAR Lecsem)
{

	return false;
}


bool TRANSLATOR::ReadGlobalScope()
{
	INTERNAL_CHAR c;
	CurScopeDeep = 1;
	while((c = getc(InFile)) != EOF)
	{
IntrpretChar:
		if(ISALPHA(c))
		{
			//Если первый символ - буква
			c = ReadWord(c);
			if(!isHaveDigits)
			{
				if(InterpretAsKeyword(WordBuf))
				{

				}
			}

			if(InterpretAsIdentificator(WordBuf))
			{

			}
		}else if(ISDIGIT(c))
		{
			//Если первый символ - цифра

		}else
		{
			switch(c)
			{
			case T('\n'):
				Line++;
				break;


			};


		}



	}
	return true;
}

void TRANSLATOR::Compile()
{
	ReadGlobalScope();
}


int main(int argc, char * argv[])
{

	printf("Compiler for zelli code.\n");



	char c;
	unsigned CurLine = 0;

	bool isGlobalScope = true;

	LPINTERNAL_CHAR Word[1000];
	unsigned SizeWord = 0;
	TRANSLATOR tr("C:\\Users\\andrew\\Desktop\\ZelliBytecode\\test2.zasm");


	return 0;
}



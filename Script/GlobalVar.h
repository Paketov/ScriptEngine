#ifndef __GLOBALVAR_H__
#define __GLOBALVAR_H__
#include <malloc.h>
#include "TypeData.h"
#include "VarFrame.h"


class GLOBAL_VAR: public VAR_FRAME
{
public:
	unsigned int MaxSize;

	GLOBAL_VAR()
	{
		StartVar = NULL;
		MaxSize  = 0;
	}

	GLOBAL_VAR(unsigned int StartSize,unsigned int nMaxSize)
	{
		StartVar = NULL;
		MaxSize = nMaxSize;
		Resize(StartSize);
	}

	~GLOBAL_VAR()
	{
		if(StartVar)
			MEM_FREE(StartVar);
	}

	//Увеличение размера стека
	bool Resize(unsigned int NewSize)
	{
		register LPINSIDE_DATA NewStartVar = (LPINSIDE_DATA)MEM_REALLOC(StartVar,NewSize);
		if(NewStartVar == NULL)
			return false;
		if(!StartVar)
		{
			//Если впервые инициализируем
			EndVar = StartVar = NewStartVar;
		}else
		{
			EndVar = (LPINSIDE_DATA)((unsigned int)EndVar + (int)((unsigned int)NewStartVar - (unsigned int)StartVar));
			StartVar = NewStartVar;
		}
		EndBlockAdr = (LPINSIDE_DATA)((unsigned int)StartVar + NewSize);
		return true;
	}

	LPINSIDE_DATA Rise(LPINSIDE_DATA BorderEndVar)
	{
		register unsigned int Differ = (unsigned int)(BorderEndVar + 1) - (unsigned int)StartVar;
		if(Differ > MaxSize)
			return NULL;
		register unsigned int NewSize = (Differ | SETBITS(unsigned int,10)) + 1;
		if(Resize((NewSize > MaxSize)?MaxSize:NewSize))
		{
			//Если удачно, возвращаем новый адрес переменной
			return (LPINSIDE_DATA)((unsigned int)Differ + (unsigned int)StartVar) - 1;
		}
		return NULL;
	}

};

#endif
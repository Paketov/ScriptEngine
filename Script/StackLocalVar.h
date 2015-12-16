#ifndef __CALLDATASTACK_H__
#define __CALLDATASTACK_H__

#include "TypeData.h"

typedef struct FRAME_BORDER
{
#define _FRAME_BORDER_DATA			\
	struct							\
	{								\
		LPINSIDE_DATA StartVar;		\
		NUMBER_VAR Barrier;			\
		FRAME_BORDER *Previous;		\
	}

	union
	{
		_FRAME_BORDER_DATA;
		class
		{
			_FRAME_BORDER_DATA;
		public:
			inline operator NUMBER_VAR()
			{
				return Barrier;
			}
		} CountVar;

		class
		{
			_FRAME_BORDER_DATA;
		public:
			inline operator unsigned()
			{
				return Barrier * sizeof(INSIDE_DATA);
			}
		} LocalSize;

		class
		{
			_FRAME_BORDER_DATA;
		public:
			inline operator LPINSIDE_DATA()
			{
				return StartVar + Barrier;
			}
		} Top;
	};

	inline FRAME_BORDER(){}

	inline FRAME_BORDER(FRAME_BORDER* PrevFrame)
	{
		Barrier  = 0;
		StartVar = PrevFrame->StartVar + (Previous = PrevFrame)->Barrier;
	}
		
	inline FRAME_BORDER(NUMBER_VAR NewBarrier, FRAME_BORDER* PrevFrame)
	{
		Barrier	 = NewBarrier;
		StartVar = PrevFrame->StartVar + (Previous = PrevFrame)->Barrier;
	}


	inline void SetNullRegion(NUMBER_VAR NewCountVar)
	{
		NUMBER_VAR Cur = Barrier;
		Barrier = NewCountVar + 1;
		for(;Cur <= NewCountVar; Cur++)
			StartVar[Cur].SetNull();
	}

	//Если переменная выходит за пределы использованных переменных
	inline bool IsVarInScope(const NUMBER_VAR n)
	{
		return n < Barrier;
	}

	inline INSIDE_DATA & operator[] (NUMBER_VAR IndexVar)
	{
		return *(StartVar + IndexVar);
	}

	inline operator LPINSIDE_DATA()
	{
		return StartVar;
	}

	inline LPINSIDE_DATA operator =(LPINSIDE_DATA Var)
	{
		return StartVar = Var;
	}
} FRAME_BORDER, *LPFRAME_BORDER;


//Базовый класс для работы с переменными
//Стек интрпретатора для работы с данными
class STACK_LOCAL_VAR
{

#define __STACK_LOCAL_VAR_DATA struct{LPFRAME_BORDER ThisFrame;void *EndBlockAdr;unsigned MaxSize;FOR_UNION_DATA<FRAME_BORDER> StartFrame;}

public:
	union
	{
		struct
		{
			LPFRAME_BORDER ThisFrame;
			void *EndBlockAdr;
			unsigned MaxSize;
			FRAME_BORDER StartFrame;
		};

		class
		{
			__STACK_LOCAL_VAR_DATA;
		public:
			inline operator LPINSIDE_DATA()
			{
				return StartFrame->StartVar;
			}
			inline operator void*()
			{
				return StartFrame->StartVar;
			}
		} Bottom;

		class
		{
			__STACK_LOCAL_VAR_DATA;
		public:
			inline operator unsigned()
			{
				return (unsigned int)EndBlockAdr - (unsigned int)StartFrame->StartVar;
			}

			bool operator=(const unsigned NewSize)
			{
				if(!StartFrame->StartVar)
				{
					void * NewBottom = MEM_ALLOC(NewSize);
					if(NewBottom == NULL)
						return false;
					//Если впервые инициализируем
					StartFrame->StartVar = (LPINSIDE_DATA)NewBottom;
				}else
				{	
					void * NewBottom = MEM_REALLOC(StartFrame->StartVar, NewSize);
					if(NewBottom == NULL)
						return false;
					if(int SlideNewMem = (int)((unsigned)NewBottom - (unsigned)StartFrame->StartVar))
					{
						for(LPFRAME_BORDER i = ThisFrame; i != NULL; i = i->Previous)
							i->StartVar = (LPINSIDE_DATA)((char*)i->StartVar + SlideNewMem);
					}
				}
				EndBlockAdr = (char*)StartFrame->StartVar + NewSize;
				return true;
			}
		} Size;
	};

	STACK_LOCAL_VAR(unsigned int nStartSize, unsigned int nMaxSize) HAS_THROW;

	STACK_LOCAL_VAR()
	{
		ThisFrame = &StartFrame;
		EndBlockAdr = StartFrame.StartVar = NULL;
	}

	~STACK_LOCAL_VAR()
	{
		if(StartFrame.StartVar != NULL)
			MEM_FREE(StartFrame.StartVar);
	}
public:


	/*===================================================*/
	//Возвращает указатель на запрашиваемые данные для записи/чтения
	inline LPINSIDE_DATA GetVarToWrite(const NUMBER_VAR n) HAS_THROW;

	//Возвращает указатель на запрашиваемые данные для чтения
	inline LPINSIDE_DATA GetVarToRead(const NUMBER_VAR n)
	{
		return (n < ThisFrame->Barrier)?(ThisFrame->StartVar + n): &INSIDE_DATA::Null;
	}


	//Закладывание переменной в стек как аргумента
	LPINSIDE_DATA PushArg(LPNUMBER_VAR NumberArg) HAS_THROW;

	//Закладывание переменной в стек как аргумента
	LPINSIDE_DATA PushArgs(LPARG_FUNC Arg) HAS_THROW;

	//Очистка фрейма от аргументов
	inline void ClearArg()
	{
		ThisFrame->Barrier = 0;
	}

};




#endif
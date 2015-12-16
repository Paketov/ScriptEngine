#ifndef __EXEPTION_MANAGER_H__
#define __EXEPTION_MANAGER_H__

#include "TypeData.h"
#include <windows.h>

typedef struct EXEPT
{
private:

	char * Top, *Max;
	unsigned CountCreated;
	void * ExeptionStack[1];

public:
	void * operator new(size_t, size_t SizeArray)
	{
	    EXEPT * CurExcept;
		__asm{
			MOV EAX, dword ptr FS:[0x44]
			MOV CurExcept, EAX
		} 
		if(CurExcept != NULL)
		{
			((EXEPT*)CurExcept)->CountCreated++;
			return CurExcept;
		}else
		{
		     CurExcept = (EXEPT *)MEM_ALLOC(sizeof(EXEPT) + SizeArray  * sizeof(void *));
			 if(CurExcept == NULL)
				 return NULL;
			 CurExcept->CountCreated = 1;
			 CurExcept->Max = (char*)CurExcept + (SizeArray- 3) * sizeof(void*);
			 CurExcept->Top = (char*)CurExcept->ExeptionStack;
			 __asm{
				MOV EAX, CurExcept
				MOV dword ptr FS:[0x44], EAX
			}
			 return CurExcept;
		}
		 
	}

	void operator delete(void * Cur)
	{
		 EXEPT * CurExcept;
		__asm{
			MOV EAX, dword ptr FS:[0x44]
			MOV CurExcept, EAX
		} 
		if(CurExcept != NULL)
		{
		  if(CurExcept->CountCreated <= 1)
		  {
		     MEM_FREE(CurExcept);
			 __asm{
			 MOV dword ptr FS:[0x44], 0
			 }
		  }else
		     CurExcept->CountCreated--;
		}
	}

	static EXEPT * GetCurrent()
	{
		__asm
		{
		    MOV EAX, dword PTR FS:[0x44]
			RETN
		}
	}

	inline bool isNotFull()
	{
		return Top <=  Max;
	}

	inline bool isEmpty()
	{
		return Top == (char*)ExeptionStack;
	}

	inline void Pop()
	{
		Top -= 24;
	}

} EXEPT, *LPEXEPT;


__declspec(naked) static unsigned __stdcall __SetExept()
{
	__asm{

		MOV EDX, dword PTR FS:[0x44]		//Копируем аргумент
		MOV EAX, dword PTR [EDX]			//Получаем адрес стека обработчиков
		ADD dword PTR [EDX], 24

		//Сохранение регистров в стеке обработчиков
		POP dword PTR [EAX]					//Сохраняем адрес возврата
		MOV dword PTR [EAX + 4], EBP		//EBP предыдущего стека
		MOV dword PTR [EAX + 8], ESP		//ESP предыдущего стека
		MOV dword PTR [EAX + 12], EDI		//Сохраняем EDI
		MOV dword PTR [EAX + 16], ESI		//Сохраняем ESI
		MOV dword PTR [EAX + 20], EBX		//Сохраняем EBX
		//////////////////////////////////

		JMP dword PTR [ESP - 4]
	}
}
__declspec(naked) static void __stdcall __ThrowExept()
{
	__asm{
		    MOV EDX, dword ptr fs:[0x44]
			SUB dword PTR [EDX], 24
			MOV EDX, dword PTR [EDX]

			MOV EBP, dword ptr [EDX + 4]		//EBP
			MOV ESP, dword ptr [EDX + 8]		//ESP
			MOV EDI, dword ptr [EDX + 12]		//EDI
			MOV ESI, dword ptr [EDX + 16]		//ESI
			MOV EBX, dword ptr [EDX + 20]		//EBX

			XOR EAX, EAX
			JMP dword PTR [EDX]				//Возвращаемся в код
	}
}

__declspec(naked) static void __stdcall __PopExcept()
{
		__asm
		{
		    MOV EAX, dword PTR FS:[0x44]
			SUB dword PTR [EAX], 24
			RETN
		}
}


#endif
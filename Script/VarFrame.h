#ifndef __VAR_H__
#define __VAR_H__
#include "TypeData.h"




class VAR_FRAME
{
protected:
	/*===================================================*/
	LPINSIDE_DATA	
		StartVar,	//Указатель на первую переменную
		EndVar,		//Указатель на последнюю переменную
		EndBlockAdr;		//Адрес конца выделеннного блока памяти
	
	
	//Выделение памяти в текущем фрейме
	inline LPINSIDE_DATA ZELLI_API AllocNewVar(LPINSIDE_DATA AddressNewVar)
	{
		//Если переполнился
		if(AddressNewVar >=  EndBlockAdr)
			AddressNewVar = Rise(AddressNewVar);
		//Говрим что здесь пустота
		for(;AddressNewVar >= EndVar;EndVar++)
			EndVar->SetNull();
		return AddressNewVar;
	}
public:
	static INSIDE_DATA NullVar;


	//Если переменных полное количество
	inline bool isFull(NUMBER_VAR n)
	{
		return (StartVar + n) >= EndBlockAdr;
	}

	inline bool isFull()
	{
		return EndVar >= EndBlockAdr;
	}

	//Если нет переменных
	inline bool isEmpty()
	{
		return StartVar >= EndVar;
	}

	//Если переменная выходит за пределы использованных переменных
	inline bool isVarOutScope(const NUMBER_VAR n)
	{
		return (StartVar + n) >= EndVar;
	}

	//Если адрес переменной выходит за пределы
	inline bool isVarOutScope(const LPINSIDE_DATA AddressVar)
	{
		return AddressVar >= EndVar;
	}

		//Если адрес переменной выходит за пределы
	inline bool isVarInScope(const LPINSIDE_DATA AddressVar)
	{
		return AddressVar < EndVar;
	}

	//Если переменная выходит за пределы использованных переменных
	inline bool isVarInScope(const NUMBER_VAR n)
	{
		return (StartVar + n) < EndVar;
	}

	//Увеличение размеров кадра по элементу увеличения
	virtual LPINSIDE_DATA Rise(LPINSIDE_DATA BorderEndVar)
	{
		return NULL;
	}

	inline LPINSIDE_DATA GetLastVar()
	{
		return EndVar;
	}

	inline LPINSIDE_DATA GetFirstVar()
	{
		return StartVar;
	}
	//Количество переменных  в пространстве переменных
	inline NUMBER_VAR GetCountVar()
	{
		//Количество переменных
		return ((unsigned int)EndVar - (unsigned int)StartVar) / sizeof(INSIDE_DATA);
	}

	//Возвращает указатель на запрашиваемые данные для записи/чтения
	inline LPINSIDE_DATA ZELLI_API GetVarToWrite(const NUMBER_VAR n)
	{
		register LPINSIDE_DATA CurVar;
		return (isVarOutScope(CurVar = StartVar + n))?AllocNewVar(CurVar):CurVar;
	}

	//Возвращает указатель на запрашиваемые данные для чтения
	inline	LPINSIDE_DATA ZELLI_API GetVarToRead(const NUMBER_VAR n)
	{
		register LPINSIDE_DATA CurVar;
		return (isVarOutScope(CurVar = StartVar + n))? &NullVar:CurVar;
	}

	//Возвращает указатель на запрашиваемые данные для чтения
	inline	LPINSIDE_DATA ZELLI_API IsVarExist(const NUMBER_VAR n)
	{
		register LPINSIDE_DATA CurVar;
		return (isVarOutScope(CurVar = StartVar + n))? NULL:CurVar;
	}

	//Добавление данных в стек. Возвращает указатель на добавленный элемент.
	inline LPINSIDE_DATA PushVar()
	{
		register LPINSIDE_DATA CurVar;

		if(isFull())
			return (EndVar = Rise(EndVar + 1)) - 1;
		else
			EndVar = (CurVar = EndVar) + 1;
		return CurVar;
	}

	//Получение последней помещённой переменной
	inline LPINSIDE_DATA PopVar()
	{
		register LPINSIDE_DATA Ret;
		if((Ret = EndVar - 1) <= StartVar)
			return NULL;
		return EndVar = Ret;
	}

};

#endif
#ifndef __VAR_H__
#define __VAR_H__
#include "TypeData.h"




class VAR_FRAME
{
protected:
	/*===================================================*/
	LPINSIDE_DATA	
		StartVar,	//��������� �� ������ ����������
		EndVar,		//��������� �� ��������� ����������
		EndBlockAdr;		//����� ����� ������������ ����� ������
	
	
	//��������� ������ � ������� ������
	inline LPINSIDE_DATA ZELLI_API AllocNewVar(LPINSIDE_DATA AddressNewVar)
	{
		//���� ������������
		if(AddressNewVar >=  EndBlockAdr)
			AddressNewVar = Rise(AddressNewVar);
		//������ ��� ����� �������
		for(;AddressNewVar >= EndVar;EndVar++)
			EndVar->SetNull();
		return AddressNewVar;
	}
public:
	static INSIDE_DATA NullVar;


	//���� ���������� ������ ����������
	inline bool isFull(NUMBER_VAR n)
	{
		return (StartVar + n) >= EndBlockAdr;
	}

	inline bool isFull()
	{
		return EndVar >= EndBlockAdr;
	}

	//���� ��� ����������
	inline bool isEmpty()
	{
		return StartVar >= EndVar;
	}

	//���� ���������� ������� �� ������� �������������� ����������
	inline bool isVarOutScope(const NUMBER_VAR n)
	{
		return (StartVar + n) >= EndVar;
	}

	//���� ����� ���������� ������� �� �������
	inline bool isVarOutScope(const LPINSIDE_DATA AddressVar)
	{
		return AddressVar >= EndVar;
	}

		//���� ����� ���������� ������� �� �������
	inline bool isVarInScope(const LPINSIDE_DATA AddressVar)
	{
		return AddressVar < EndVar;
	}

	//���� ���������� ������� �� ������� �������������� ����������
	inline bool isVarInScope(const NUMBER_VAR n)
	{
		return (StartVar + n) < EndVar;
	}

	//���������� �������� ����� �� �������� ����������
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
	//���������� ����������  � ������������ ����������
	inline NUMBER_VAR GetCountVar()
	{
		//���������� ����������
		return ((unsigned int)EndVar - (unsigned int)StartVar) / sizeof(INSIDE_DATA);
	}

	//���������� ��������� �� ������������� ������ ��� ������/������
	inline LPINSIDE_DATA ZELLI_API GetVarToWrite(const NUMBER_VAR n)
	{
		register LPINSIDE_DATA CurVar;
		return (isVarOutScope(CurVar = StartVar + n))?AllocNewVar(CurVar):CurVar;
	}

	//���������� ��������� �� ������������� ������ ��� ������
	inline	LPINSIDE_DATA ZELLI_API GetVarToRead(const NUMBER_VAR n)
	{
		register LPINSIDE_DATA CurVar;
		return (isVarOutScope(CurVar = StartVar + n))? &NullVar:CurVar;
	}

	//���������� ��������� �� ������������� ������ ��� ������
	inline	LPINSIDE_DATA ZELLI_API IsVarExist(const NUMBER_VAR n)
	{
		register LPINSIDE_DATA CurVar;
		return (isVarOutScope(CurVar = StartVar + n))? NULL:CurVar;
	}

	//���������� ������ � ����. ���������� ��������� �� ����������� �������.
	inline LPINSIDE_DATA PushVar()
	{
		register LPINSIDE_DATA CurVar;

		if(isFull())
			return (EndVar = Rise(EndVar + 1)) - 1;
		else
			EndVar = (CurVar = EndVar) + 1;
		return CurVar;
	}

	//��������� ��������� ���������� ����������
	inline LPINSIDE_DATA PopVar()
	{
		register LPINSIDE_DATA Ret;
		if((Ret = EndVar - 1) <= StartVar)
			return NULL;
		return EndVar = Ret;
	}

};

#endif
#ifndef __EXEPTION_HANDLER_H__
#define __EXEPTION_HANDLER_H__

#include "TypeData.h"
#include "Object.h"
#include "String.h"

class EXEPTION_CLASS:  public HEADER_CLASS
{	
	friend HEADER_EXCEPTION;

    static LPHEADER_STRING			propId;
	static LPHEADER_STRING			propDescription;
#ifdef _DEBUG
	static LPHEADER_STRING			PropCppFile;
	static LPHEADER_STRING			PropLine;
#endif

public:

	EXEPTION_CLASS(LPSTRING_CLASS StrClass);

	virtual INSIDE_DATA CreateInstance(LPEXECUTE_CONTEXT, LPARG_FUNC);


	virtual INSIDE_DATA ReadMember(INSTANCE_CLASS, const LPINSIDE_DATA);

	virtual void WriteMember(INSTANCE_CLASS, const LPINSIDE_DATA, const LPINSIDE_DATA);

	virtual void RemoveElement(INSTANCE_CLASS, const LPINSIDE_DATA);

	virtual SIZE_STR InfoObject(INSTANCE_CLASS, LPINTERNAL_CHAR, SIZE_STR);

	virtual ZELLI_INTEGER GetLength(INSTANCE_CLASS);

	virtual void EnumKey(INSTANCE_CLASS, LPINSIDE_DATA);

	virtual bool OperatorEq(INSTANCE_CLASS, const LPINSIDE_DATA);

	virtual ZELLI_INTEGER OperatorToInt(INSTANCE_CLASS);

	virtual ZELLI_DOUBLE OperatorToDouble(INSTANCE_CLASS);

};


class HEADER_EXCEPTION
{
public:
	enum EN
	{
		SUCCESS,							//Без исключений
		BROKEN_VAR,							//Испорченная переменная
		VARIABLE_IS_NOT_EXCEPTION_HANDLER,
		PAUSE,								//Приостановка выполнения
		OVER_INTERATOR_TABLE,
		ZERO_DEEP_INTERATOR,
		NOT_HAVE_THIS_OBJECT,
		NONE_ARG,
		OUT_OF_ARGUMENT_RANGE,
		NULL_OBJECT,
		VAL_IS_NOT_FUNCTION,
		VM_NOT_ALLOC_MEM,
		INCORRECT_OPCODE,
		VARIABLE_OVER_LOCAL_SCOPE,
		NOT_ALLOC_MEM_FOR_OBJECT,
		NOT_WRITE_CONSTANT,
		NOT_WRITE_IN_CONST_OBJECT,
		NOT_WRITE_MEMBER,
		NOT_REMOVE_FROM_CONST_OBJ,
		BAD_TYPE_ARG,
		NOT_REALLOC_GLOBAL_SOPE,
		BAD_TYPE_INDEX,
		NOT_ALLOC_MEM_FOR_REG_CLASS,
		NOT_REALLOC_MEM,
		VAL_IS_NOT_OBJECT,
		LOCAL_SCOPE_NOT_HAVE_EXCEPT_HANDLERS,
	    FUNCTION_NOT_SUPPORTED,
		__END_EXCEPTION
	};
	

#ifdef _DEBUG
	typedef EN TYPE_ENUM; 
#else
	typedef unsigned short TYPE_ENUM; 
#endif
	TYPE_ENUM	ThrowId;
	LPHEADER_STRING Description;
#ifdef _DEBUG
    LPHEADER_STRING CppFile;
	ZELLI_INTEGER	Line;

	HEADER_EXCEPTION(LPHEADER_STRING Description, TYPE_ENUM Id, ZELLI_INTEGER Line, LPHEADER_STRING CppFile)
	{
	   this->CppFile = CppFile;
	   this->Line = Line;
	   this->Description = Description;
	   ThrowId = Id;
	}
#endif

	typedef struct 
	{
		TYPE_ENUM		ThrowId; 
		const void *	Description;
#ifdef _DEBUG
	    const void *	CppFile;
	    ZELLI_INTEGER	Line;
#endif
	} STATIC_INIT;

	HEADER_EXCEPTION()
	{

	}

	HEADER_EXCEPTION(LPHEADER_STRING Description, TYPE_ENUM  Id)
	{
	   this->Description = Description;
	   ThrowId = Id;
	}

	void PrepareForUse(LPSTRING_CLASS ListStrings);

};


struct UNHANDLED_EXCEPTION
{
	enum EN
	{
		SUCCESS,		//Без исключений
		NOT_ALLOC_MEMORY,
		NOT_REALLOC,
		STACK_OVERFLOW,
		__END_EXCEPTION
	};

	EN				    Type;
	LPINTERNAL_CHAR		Notice;
#ifdef _DEBUG
	LPINTERNAL_CHAR		File;
	unsigned			Line;
#endif
};

#endif
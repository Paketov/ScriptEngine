#ifndef __METHOD_H__
#define __METHOD_H__
#include "TypeData.h"
#include "Opcode.h"
#include "Context.h"
#include "Object.h"
#include "Class.h"


class METHOD_CLASS: public HEADER_CLASS
{
protected:

	LPHEADER_METHOD * Methods;
	unsigned MaxCountMethods;
	unsigned CountMethods;

	CHECK_USES CurCheckUses;

	void AddMethodToList(LPHEADER_METHOD Method)
	{
		if(CountMethods >= MaxCountMethods)
		{
			unsigned NewSize = (unsigned)((float)MaxCountMethods * GOLDEN_RATIO);
			Methods = (LPHEADER_METHOD*)MEM_REALLOC(Methods, NewSize * sizeof(LPHEADER_METHOD));
			if(Methods == NULL)
				THROW_UNHANDLED_EXCEPTION("METHOD_CLASS: Not realloc memory for method list.", UNHANDLED_EXCEPTION::NOT_ALLOC_MEMORY);
			MaxCountMethods = NewSize;
		}
		Methods[CountMethods++] = Method;
	}

	virtual INSIDE_DATA CreateInstance(LPEXECUTE_CONTEXT, LPARG_FUNC);

	virtual void MarkAsUsed(INSTANCE_CLASS);

	virtual void SetAllInstanceToUnused();

	virtual void FreeAllUnused();

	virtual bool OperatorEq(INSTANCE_CLASS, const LPINSIDE_DATA);

	virtual SIZE_STR InfoObject(INSTANCE_CLASS, LPINTERNAL_CHAR, SIZE_STR);

	virtual ZELLI_INTEGER OperatorToInt(INSTANCE_CLASS)
	{
		return 1;
	}

	virtual ZELLI_DOUBLE OperatorToDouble(INSTANCE_CLASS)
	{
		return 1.0;
	}

	virtual INSIDE_DATA OperatorCall(INSTANCE_CLASS, LPINSIDE_DATA, LPEXECUTE_CONTEXT, LPARG_FUNC);

public:
	INSIDE_DATA RegisterMethod(NATIVE_FUNCTION NativeMethod);
	INSIDE_DATA RegisterMethod(LPHEADER_METHOD ByteCodeMethod);

	METHOD_CLASS();
	~METHOD_CLASS();
};


class HEADER_METHOD
{	

public:
	enum
	{
		NULL_INTERATOR = (unsigned int)-1
	};

	union
	{
		struct
		{
			unsigned char Type:1;
			unsigned char Uses:7;
		};

		class
		{
			unsigned char Type:1;
			unsigned char Uses:7;
		public:
			inline operator bool()
			{
				return Type;
			}

			bool operator =(const bool v)
			{
				return Type = v;
			}
		} IsNative;

		class
		{
			unsigned char Type:1;
			unsigned char Uses:7;
		public:
			inline operator bool()
			{
				return !Type;
			}

			bool operator =(bool v)
			{
				Type = !v;
				return v;
			}
		} IsBytecode;
	};

	union
	{
		struct 
		{
			NUMBER_VAR				MaxLocalVar;
			LPOPCODE				Method;
			operator LPOPCODE()
			{
			   return Method;
			}

			LPOPCODE operator=(LPOPCODE v)
			{
			    return Method = v;
			}
		}Bytecode;

		struct 
		{
			NATIVE_FUNCTION			Method;

			operator NATIVE_FUNCTION()
			{
			   return Method;
			}

			NATIVE_FUNCTION operator=(NATIVE_FUNCTION v)
			{
			    return Method = v;
			}
		} Native;
	};

	struct
	{
		NUMBER_VAR	Count;
	private:
		INSIDE_DATA	ConstObjs[1];
	public:

		operator LPINSIDE_DATA()
		{
		   return ConstObjs;
		}
	} ConstObjs;

	unsigned Size()
	{
		return (sizeof(HEADER_METHOD) - sizeof(INSIDE_DATA)) + sizeof(INSIDE_DATA) * ConstObjs.Count;
	}

	static LPHEADER_METHOD New(NUMBER_VAR CountConstObjs);

	INSIDE_DATA operator()(LPEXECUTE_CONTEXT Context, LPARG_FUNC Arg = &ARG_FUNC::EmptyArgs);

	INSIDE_DATA operator()(LPEXECUTE_CONTEXT Context, LPINSIDE_DATA ThisScope, LPARG_FUNC Arg = &ARG_FUNC::EmptyArgs);

	inline HEADER_METHOD()
	{
	}

	inline HEADER_METHOD(NATIVE_FUNCTION NativeFunc)
	{
		IsNative = true;
		Native = NativeFunc;
	}

	inline HEADER_METHOD(LPOPCODE BytcodeMethod)
	{
		IsBytecode = true;
		Bytecode = BytcodeMethod;
	}
};


#endif

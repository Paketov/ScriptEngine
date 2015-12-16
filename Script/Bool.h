#ifndef __BOOL_H__
#define __BOOL_H__

#include "Class.h"
#include "AdditionalLib.h"
#include "String.h"

class BOOL_CLASS;

typedef BOOL_CLASS *LPBOOL_CLASS;

class BOOL_CLASS: public HEADER_CLASS
{
protected:

	//virtual INSIDE_DATA CreateInstance(LPEXECUTE_CONTEXT, LPARG_FUNC);

	//virtual INSIDE_DATA ReadMember(INSTANCE_CLASS, const LPINSIDE_DATA);

	//virtual INSIDE_DATA ReadMemberById(INSTANCE_CLASS, const ID_OBJMEMBER);

	//virtual void WriteMember(INSTANCE_CLASS, const LPINSIDE_DATA, const LPINSIDE_DATA);

	//virtual void WriteMemberById(INSTANCE_CLASS, const ID_OBJMEMBER, const LPINSIDE_DATA);

	//virtual void RemoveElement(INSTANCE_CLASS, const LPINSIDE_DATA);

	//virtual void RemoveElementById(INSTANCE_CLASS, const ID_OBJMEMBER);

	//virtual bool OperatorToBool(INSTANCE_CLASS Object)
	//{
	//   return (bool)Object;
	//}

	////Garbage Collector


	//virtual INSIDE_DATA OperatorAdd(INSTANCE_CLASS, LPINSIDE_DATA);

	//virtual ZELLI_INTEGER GetLength(INSTANCE_CLASS);

	//virtual void EnumKey(INSTANCE_CLASS, LPINSIDE_DATA);

	//virtual bool OperatorEq(INSTANCE_CLASS, LPINSIDE_DATA);

	//virtual ZELLI_DOUBLE OperatorToDouble(INSTANCE_CLASS);

	//virtual ZELLI_INTEGER OperatorToInt(INSTANCE_CLASS);

	//virtual INSIDE_DATA Clone(INSTANCE_CLASS);

	//virtual bool OperatorNeq(INSTANCE_CLASS, LPINSIDE_DATA);

	//virtual SIZE_STR InfoObject(INSTANCE_CLASS, LPINTERNAL_CHAR, SIZE_STR);

	//virtual SIZE_STR InfoClass(LPHEADER_CLASS, LPINTERNAL_CHAR, SIZE_STR);

	//virtual INSIDE_DATA OperatorOr(INSTANCE_CLASS Object, const LPINSIDE_DATA AnotherVar)
	//{
	//   return (bool)Object || AnotherVar->operator bool();
	//}

	//virtual INSIDE_DATA OperatorAnd(INSTANCE_CLASS Object, const LPINSIDE_DATA AnotherVar)
	//{
	//   return (bool)Object && AnotherVar->operator bool();
	//}

public:


	BOOL_CLASS(LPSTRING_CLASS StringClass)
	{
	   Name = StringClass->RegisterString("bool");
	}

};


#endif
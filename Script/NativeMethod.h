#ifndef __NATIVEMETHOD_H__
#define __NATIVEMETHOD_H__
#include "TypeData.h"
#include "Class.h"


class NATIVEMETHOD_CLASS: public HEADER_CLASS
{
protected:

	virtual bool OperatorEq(INSTANCE_CLASS , const LPINSIDE_DATA);

	virtual bool OperatorNeq(INSTANCE_CLASS , const LPINSIDE_DATA);

	virtual ZELLI_INTEGER OperatorToInt(INSTANCE_CLASS);

	virtual ZELLI_DOUBLE OperatorToDouble(INSTANCE_CLASS);

	virtual INSIDE_DATA OperatorCall(INSTANCE_CLASS , LPINSIDE_DATA , LPEXECUTE_CONTEXT , LPARG_FUNC );

	virtual SIZE_STR InfoObject(INSTANCE_CLASS , LPINTERNAL_CHAR , SIZE_STR );
public:
	NATIVEMETHOD_CLASS(LPSTRING_CLASS);
};

#endif



#include "Method.h"
#include "String.h"



LPHEADER_METHOD HEADER_METHOD::New(NUMBER_VAR CountConstObjs)
{
	LPHEADER_METHOD NewMethod = (LPHEADER_METHOD)MEM_ALLOC((sizeof(HEADER_METHOD) - sizeof(INSIDE_DATA)) + sizeof(INSIDE_DATA) * CountConstObjs);
	if(NewMethod == NULL)
		return NULL;
	NewMethod->ConstObjs.Count = CountConstObjs;
	return NewMethod;
}

void METHOD_CLASS::MarkAsUsed(INSTANCE_CLASS Object)
{
	LPHEADER_METHOD Method = (LPHEADER_METHOD)Object;
	Method->Uses = CurCheckUses;
	for(LPINSIDE_DATA StartMarkVar = Method->ConstObjs, EndMarkVar = StartMarkVar + Method->ConstObjs.Count;
		StartMarkVar < EndMarkVar;
		StartMarkVar++)
		StartMarkVar->MarkAsUsed();
}

void METHOD_CLASS::SetAllInstanceToUnused()
{
	CurCheckUses = 0x7f & (CurCheckUses + 1);
}

void METHOD_CLASS::FreeAllUnused()
{
	LPHEADER_METHOD * Methods = this->Methods;
	unsigned Count = CountMethods;
	for(unsigned i = 0; i < Count;)
	{
		if(Methods[i]->Uses != CurCheckUses)
		{
			MEM_FREE(Methods[i]);
			Methods[i] = Methods[Count - 1];
			Count--;
		}else
			i++;
	}
	unsigned NewSize = (float)Count * GOLDEN_RATIO;
	if(NewSize < MaxCountMethods)
	{
		if(NewSize < 10)
			return;
		Methods = (LPHEADER_METHOD*)MEM_REALLOC(Methods, NewSize * sizeof(LPHEADER_METHOD));
		if(Methods == NULL)
			THROW_UNHANDLED_EXCEPTION("METHOD_CLASS: Not realloc memory for method list.", UNHANDLED_EXCEPTION::NOT_ALLOC_MEMORY);
		MaxCountMethods = NewSize;
	}
}

INSIDE_DATA METHOD_CLASS::RegisterMethod(NATIVE_FUNCTION NativeMethod)
{
	LPHEADER_METHOD NewMethod = (LPHEADER_METHOD)MEM_ALLOC(sizeof(HEADER_METHOD));
	if(!NewMethod)
		return NULL;
	NewMethod->IsNative = true;
	NewMethod->Native.Method = NativeMethod;
	NewMethod->Uses = CurCheckUses;
	AddMethodToList(NewMethod);
	return OBJECT::New(this,NewMethod);
}

INSIDE_DATA METHOD_CLASS::RegisterMethod(LPHEADER_METHOD ByteCodeMethod)
{
	//ByteCodeMethod->Prototype = this;
	ByteCodeMethod->IsBytecode = true;
	ByteCodeMethod->Uses = CurCheckUses;
	AddMethodToList(ByteCodeMethod);
	return OBJECT::New(this, ByteCodeMethod);
}

INSIDE_DATA METHOD_CLASS::CreateInstance(LPEXECUTE_CONTEXT Context, LPARG_FUNC Arg)
{
	if(Arg->IsEmpty)
		return INSIDE_DATA::Null;

	LPINSIDE_DATA Source = Context->GetArg(Arg, 0);
	switch(Source->TypeData)
	{
	case OBJECT_CASE:
		return *Source;
	default:
		return INSIDE_DATA::Null;
	}
};

INSIDE_DATA METHOD_CLASS::OperatorCall(INSTANCE_CLASS Object, LPINSIDE_DATA This, LPEXECUTE_CONTEXT Context, LPARG_FUNC Arg)
{
	return ((LPHEADER_METHOD)Object)->operator()(Context, This, Arg);
}

METHOD_CLASS::METHOD_CLASS()
{
	Methods = (LPHEADER_METHOD*)MEM_ALLOC(sizeof(LPHEADER_METHOD) * 10);
	if(Methods == NULL)
		THROW_UNHANDLED_EXCEPTION("METHOD_CLASS: Not alloc memory for method list.", UNHANDLED_EXCEPTION::NOT_ALLOC_MEMORY);
	MaxCountMethods = 10;
	CountMethods = 0;
}

METHOD_CLASS::~METHOD_CLASS()
{
   if(Methods != NULL)
	   MEM_FREE(Methods);
}

bool METHOD_CLASS::OperatorEq(INSTANCE_CLASS ThisObj, LPINSIDE_DATA SecondObj)
{
    return SecondObj->IsObject && (SecondObj->Object.Prototype == this) && (ThisObj == SecondObj->Object);
}


INSIDE_DATA HEADER_METHOD::operator()(LPEXECUTE_CONTEXT Context, LPARG_FUNC Arg)
{
	INSIDE_DATA Result, NullVal;
	NullVal.SetNull();
	if(IsNative)
	{
		FRAME_BORDER FrameForNative(Context->ThisFrame);
		Context->ThisFrame = &FrameForNative;
		Result = Native.Method(&NullVal, Context, Arg);
	}else
		Result = Context->Execute(this, Arg, &NullVal);
	Context->ThisFrame = Context->ThisFrame->Previous;
	return Result;
}


INSIDE_DATA HEADER_METHOD::operator()(LPEXECUTE_CONTEXT Context, LPINSIDE_DATA ThisScope, LPARG_FUNC Arg)
{
	INSIDE_DATA Result;
	if(IsNative)
	{
		FRAME_BORDER FrameForNative(Context->ThisFrame);
		Context->ThisFrame = &FrameForNative;
		Result = Native.Method(ThisScope, Context, Arg);
	}else
		Result = Context->Execute(this, Arg, ThisScope);
	Context->ThisFrame = Context->ThisFrame->Previous;
	return Result;
}

SIZE_STR METHOD_CLASS::InfoObject(INSTANCE_CLASS Obj, LPINTERNAL_CHAR Buffer, SIZE_STR LenInBuf)
{
	HEADER_METHOD * Hs = (LPHEADER_METHOD)Obj;
	return toz(sprintf_s(Buffer, LenInBuf, "METHOD: IsNative %s", (Hs->IsNative)?"true":"false"));
}
#include "Context.h"
#include "Opcode.h"
#include "String.h"
#include "Method.h"
#include "GlobalScope.h"
#include "InlineTypeData.h"

#ifdef WIN32
HANDLE  ProcessHeapHandle = GetProcessHeap();
#endif

static INSIDE_DATA _InitConstNull()
{
	INSIDE_DATA Ret;
	Ret.SetNull();
	return Ret;
}

INSIDE_DATA INSIDE_DATA::Null = _InitConstNull();
INSIDE_DATA INSIDE_DATA::Zero = 0;

ARG_FUNC ARG_FUNC::EmptyArgs = {0};

OBJECT::ARR_FOR_CLASS			OBJECT::ClassList(50);
METHOD_CLASS					EXECUTE_CONTEXT::FunctionClass;
STRING_CLASS					EXECUTE_CONTEXT::MainStringClass;
NATIVEMETHOD_CLASS              EXECUTE_CONTEXT::NativeMethodClass(&EXECUTE_CONTEXT::MainStringClass);
CLASS_MANAGER					EXECUTE_CONTEXT::MainObjectManager;
EXEPTION_CLASS					EXECUTE_CONTEXT::ExceptionClass(&EXECUTE_CONTEXT::MainStringClass);
EXECUTE_CONTEXT					EXECUTE_CONTEXT::DefaultContext;
THREAD_DATA LPEXECUTE_CONTEXT   EXECUTE_CONTEXT::ForThisThread = &EXECUTE_CONTEXT::DefaultContext;
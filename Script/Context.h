#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <stdio.h>
#include <math.h>

#include "ExeptionHandler.h"
#include "StackLocalVar.h"
#include "TypeData.h"
#include "Object.h"
#include "AcselerateInterator.h"
#include "NativeMethod.h"



class EXECUTE_CONTEXT: public STACK_LOCAL_VAR
{ 
	

public:
	enum GC_STAGE
	{
		 CLEANING,
	     CLEANING_COMPLETE,
		 SETTING_OBJECT_UNUSED,
		 SETTING_OBJECT_UNUSED_COMPLETE,
		 MARKING_OBJECTS,
		 ALL_OBJECT_HAS_MARKS,
		 DISABLE_GC
	};

	

	INSIDE_DATA							  GlobalObject;
	LPCLASS_MANAGER						  ObjManager;
	LPSTRING_CLASS					      StringClass;
	GC_STAGE						      LastGcState;

	static CLASS_MANAGER				  MainObjectManager;
	static STRING_CLASS					  MainStringClass;
	static METHOD_CLASS					  FunctionClass;
	static EXEPTION_CLASS			      ExceptionClass;
	static THREAD_DATA LPEXECUTE_CONTEXT  ForThisThread;
	static EXECUTE_CONTEXT                DefaultContext;
	static NATIVEMETHOD_CLASS             NativeMethodClass;

	EXECUTE_CONTEXT();
	EXECUTE_CONTEXT(unsigned int nStartSizeStack,unsigned int nMaxSizeStack);
	EXECUTE_CONTEXT
	(
		LPINSIDE_DATA CommonScope,
		LPINSIDE_DATA CommonConst,
		LPCLASS_MANAGER CommonObjectManager,
		LPSTRING_CLASS CommonStringClass,
		unsigned  nStartSizeStack = 5000,
		unsigned  nMaxSizeStack = 10000
	);

	~EXECUTE_CONTEXT();

	INSIDE_DATA __fastcall Execute(LPHEADER_METHOD HeaderMethod, LPARG_FUNC Arg, LPINSIDE_DATA ThisScope) HAS_THROW;

	//Возвращает указатель на запрашиваемые данные для чтения
	inline LPINSIDE_DATA GetVar(NUMBER_VAR n) { return ThisFrame->StartVar + n; }

	//Возвращает указатель на запрашиваемые данные для чтения
	inline LPINSIDE_DATA GetVar(NUMBER_VAR n, LPFRAME_BORDER FrameWithVar) { return FrameWithVar->StartVar + n; }

	inline LPINSIDE_DATA GetArg(const LPARG_FUNC Arg, NUMBER_VAR n, LPFRAME_BORDER FrameWithVar) { return FrameWithVar->StartVar + Arg->Args[n]; }

	inline LPINSIDE_DATA GetArg(const LPARG_FUNC Arg, NUMBER_VAR n) { return ThisFrame->Previous->StartVar + Arg->Args[n]; }

	//Возвращает указатель на запрашиваемые данные для записи/чтения
	inline LPINSIDE_DATA GetArg(NUMBER_ARG n, LPFRAME_BORDER FrameWithVar) { return FrameWithVar->StartVar + n; }

	inline LPINSIDE_DATA GetArg(NUMBER_ARG n) { return ThisFrame->Previous->StartVar + n; }

	static bool StaticMarkAllInstance();

	static bool StaticSetAllInstanceToUnused();

	static bool StaticFreeAllUnusedInstance();

	static bool StaticCleaning();

	void MarkAllInstance();

	void SetAllInstanceToUnused();

	void FreeAllUnusedInstance();

    bool Cleaning();

};

#endif
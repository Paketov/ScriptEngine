#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include <stdio.h>
#include <math.h>

#include "ExeptionHandler.h"
#include "StackLocalVar.h"
#include "TypeData.h"
#include "Object.h"
#include "AcselerateInterator.h"
#include "ConstScope.h"
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
	INSIDE_DATA							  ConstValues;
	LPCLASS_MANAGER						  ObjManager;
	LPSTRING_CLASS					      StringClass;
	GC_STAGE						      LastGcState;

	static CLASS_MANAGER				  MainObjectManager;
	static STRING_CLASS					  MainStringClass;
	static METHOD_CLASS					  FunctionClass;
	static EXEPTION_CLASS			      ExceptionClass;
	static THREAD_DATA LPEXECUTE_CONTEXT  ForThisThread;
	static EXECUTE_CONTEXT                DefaultContext;
	static CONST_SCOPE_CLASS			  MainConstScope;
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
	inline LPINSIDE_DATA GetVar(const NUMBER_VAR n)
	{
		return ThisFrame->StartVar + n;
	}

	//Возвращает указатель на запрашиваемые данные для чтения
	inline LPINSIDE_DATA GetVar(const NUMBER_VAR n, LPFRAME_BORDER FrameWithVar)
	{
		return FrameWithVar->StartVar + n;
	}

	inline LPINSIDE_DATA GetArg(const LPARG_FUNC Arg, NUMBER_VAR n, LPFRAME_BORDER FrameWithVar)
	{
		return FrameWithVar->StartVar + Arg->Args[n];
	}

	inline LPINSIDE_DATA GetArg(const LPARG_FUNC Arg, NUMBER_VAR n)
	{
		return ThisFrame->Previous->StartVar + Arg->Args[n];
	}

	//Возвращает указатель на запрашиваемые данные для записи/чтения
	inline LPINSIDE_DATA GetArg(const NUMBER_ARG n, LPFRAME_BORDER FrameWithVar)
	{
		return FrameWithVar->StartVar + n;
	}

	inline LPINSIDE_DATA GetArg(const NUMBER_ARG n)
	{
		return ThisFrame->Previous->StartVar + n;
	}

	static bool StaticMarkAllInstance()
	{
	    if(ForThisThread == NULL)
			return false;
		ForThisThread->MarkAllInstance();
		ForThisThread->LastGcState = GC_STAGE::ALL_OBJECT_HAS_MARKS;
		return true;
	}

	static bool StaticSetAllInstanceToUnused()
	{
	    if(ForThisThread == NULL)
			return false;
		ForThisThread->ObjManager->SetAllInstanceToUnused();
		ForThisThread->LastGcState = GC_STAGE::SETTING_OBJECT_UNUSED_COMPLETE;
		return true;
	}

	static bool StaticFreeAllUnusedInstance()
	{
	    if(ForThisThread == NULL)
			return false;
		ForThisThread->ObjManager->FreeAllUnusedInstance();
		ForThisThread->LastGcState = GC_STAGE::CLEANING_COMPLETE;
		return true;
	}

	static bool StaticCleaning()
	{
		switch(ForThisThread->LastGcState)
		{
			case GC_STAGE::CLEANING_COMPLETE:
				ForThisThread->MarkAllInstance();
			case GC_STAGE::SETTING_OBJECT_UNUSED_COMPLETE:
				ForThisThread->ObjManager->SetAllInstanceToUnused();
			case GC_STAGE::ALL_OBJECT_HAS_MARKS:
				ForThisThread->ObjManager->FreeAllUnusedInstance();
		}
		ForThisThread->LastGcState = GC_STAGE::CLEANING_COMPLETE;
		return true;
	}

	void MarkAllInstance()
	{
		if(ConstValues.IsObject)
			ConstValues.Object.MarkAsUsed();
		if(GlobalObject.IsObject)
			GlobalObject.Object.MarkAsUsed();
		LPINSIDE_DATA EndVar = ThisFrame->StartVar + ThisFrame->Barrier;
		LPINSIDE_DATA StartVar = StartFrame.StartVar;
		for(; StartVar < EndVar; StartVar++)
		{
			switch(StartVar->TypeData)
			{
			case INSIDE_DATA::TYPEDATA_STRING:
			case INSIDE_DATA::TYPEDATA_OBJECT:
				StartVar->Object.MarkAsUsed();
			}
		}
		LastGcState = GC_STAGE::ALL_OBJECT_HAS_MARKS;
	}


	inline void SetAllInstanceToUnused()
	{
		ObjManager->SetAllInstanceToUnused();
		LastGcState = GC_STAGE::ALL_OBJECT_HAS_MARKS;
	}

	inline void FreeAllUnusedInstance()
	{
		ObjManager->FreeAllUnusedInstance();
		LastGcState = GC_STAGE::CLEANING_COMPLETE;
	}

    bool Cleaning()
	{
		switch(LastGcState)
		{
			case GC_STAGE::CLEANING_COMPLETE:
				MarkAllInstance();
			case GC_STAGE::SETTING_OBJECT_UNUSED_COMPLETE:
				ObjManager->SetAllInstanceToUnused();
			case GC_STAGE::ALL_OBJECT_HAS_MARKS:
				ObjManager->FreeAllUnusedInstance();
		}
		LastGcState = GC_STAGE::CLEANING_COMPLETE;
	}

};

#endif
#include "StackLocalVar.h"
#include "ExeptionHandler.h"
#include "Context.h"

STACK_LOCAL_VAR::STACK_LOCAL_VAR(unsigned int nStartSize,unsigned int nMaxSize) HAS_THROW
{
	StartFrame.StartVar = NULL;
	MaxSize = nMaxSize;
	ThisFrame = &StartFrame;
	if(!Size.operator=(nStartSize))
		THROW_UNHANDLED_EXCEPTION("STACK_LOCAL_VAR: not alloc memory for stack.", UNHANDLED_EXCEPTION::NOT_ALLOC_MEMORY);
	StartFrame.Previous = NULL;
	StartFrame.Barrier = 0;
}


inline LPINSIDE_DATA STACK_LOCAL_VAR::PushArg(LPNUMBER_VAR NumberArg) HAS_THROW
{
	LPINSIDE_DATA FirstVar = ThisFrame->Top;
	void * AddressEndVar;
	if((AddressEndVar = FirstVar + 1) >= EndBlockAdr)
	{
		register unsigned int Differ = (unsigned int)AddressEndVar - (unsigned int)StartFrame.StartVar;
		if(Differ > MaxSize)
			goto lbl_ThrowStackOverflow;
		register unsigned int NewSize = (Differ | SETBITS(unsigned int,10)) + 1;
		if((AddressEndVar = MEM_REALLOC(StartFrame.StartVar,NewSize)) == NULL)
			goto lbl_ThrowNotAllocMem;
		if(int SlideNewMem = (int)((unsigned)AddressEndVar - (unsigned)StartFrame.StartVar))
		{
			StartFrame.StartVar = (LPINSIDE_DATA)AddressEndVar;
			for(LPFRAME_BORDER i = ThisFrame;i;i = i->Previous)
				i->StartVar = (LPINSIDE_DATA)((char*)i->StartVar + SlideNewMem);
			FirstVar = ThisFrame->StartVar + ThisFrame->Barrier;
		}
	}
	*NumberArg = ThisFrame->Barrier;
	ThisFrame->Barrier++;
	return FirstVar;

lbl_ThrowStackOverflow:
	THROW_UNHANDLED_EXCEPTION("STACK_LOCAL_VAR: stack overflow.", UNHANDLED_EXCEPTION::STACK_OVERFLOW);
lbl_ThrowNotAllocMem:
	THROW_UNHANDLED_EXCEPTION("STACK_LOCAL_VAR: not realloc mem for stack.", UNHANDLED_EXCEPTION::NOT_ALLOC_MEMORY);
}


LPINSIDE_DATA STACK_LOCAL_VAR::PushArgs(LPARG_FUNC Arg) HAS_THROW
{
	LPINSIDE_DATA FirstVar = ThisFrame->StartVar + ThisFrame->Barrier;
	void * AddressEndVar;
	if((AddressEndVar = FirstVar + Arg->CountArg) >= EndBlockAdr)
	{
		register unsigned int Differ = (unsigned int)AddressEndVar - (unsigned int)StartFrame.StartVar;
		if(Differ > MaxSize)
			goto lbl_ThrowStackOverflow;
		register unsigned int NewSize = (Differ | SETBITS(unsigned int,10)) + 1;
		if((AddressEndVar = MEM_REALLOC(StartFrame.StartVar,NewSize)) == NULL)
			goto lbl_ThrowNotAllocMem;
		if(int SlideNewMem = (int)((unsigned)AddressEndVar - (unsigned)StartFrame.StartVar))
		{
			StartFrame.StartVar = (LPINSIDE_DATA)AddressEndVar;
			for(LPFRAME_BORDER i = ThisFrame;i;i = i->Previous)
				i->StartVar = (LPINSIDE_DATA)((char*)i->StartVar + SlideNewMem);
			FirstVar = ThisFrame->StartVar + ThisFrame->Barrier;
		}
	}

	Arg->Args[0] = ThisFrame->Barrier;
	ThisFrame->Barrier += Arg->CountArg;
	return FirstVar;
lbl_ThrowStackOverflow:
	THROW_UNHANDLED_EXCEPTION("STACK_LOCAL_VAR: stack overflow.", UNHANDLED_EXCEPTION::STACK_OVERFLOW);
lbl_ThrowNotAllocMem:
	THROW_UNHANDLED_EXCEPTION("STACK_LOCAL_VAR: not realloc mem for stack.", UNHANDLED_EXCEPTION::NOT_ALLOC_MEMORY);
}

inline LPINSIDE_DATA STACK_LOCAL_VAR::GetVarToWrite(const NUMBER_VAR n) HAS_THROW
{
	if(n < ThisFrame->Barrier)
		return ThisFrame->StartVar + n;
	THROW_NATIVE_EXCEPT("STACK_LOCAL_VAR: variable out of local scope.", HEADER_EXCEPTION::VARIABLE_OVER_LOCAL_SCOPE);
}


FRAME_BORDER& STACK_LOCAL_VAR::operator[](unsigned IndexFrame)
{
	unsigned CurCountFrames = CountFrames;
	if(IndexFrame >= CurCountFrames)
		return *(LPFRAME_BORDER)nullptr;
	CurCountFrames -= IndexFrame - 1;
	LPFRAME_BORDER f = ThisFrame;
	for(unsigned i = 0;i < CurCountFrames; i++)
		f = f->Previous;
	return *f;
}
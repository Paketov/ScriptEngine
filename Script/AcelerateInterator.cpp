
#include "AcselerateInterator.h"
#include "Method.h"
#include "String.h"
#include "LoadFromFile.h"


bool INTERATOR::InitAll(LPBYTECODE_FILE CurLoad)
{
	LPINTERATOR CurInterator = (LPINTERATOR)this;
	unsigned IndexInterator = CurLoad->CountInterators;
	LPHEADER_STRING* Element = CurInterator->Element, *MaxElem = Element + CurInterator->Deep;
	while(true)
	{
		if(Element >= MaxElem)
		{
			if(--IndexInterator <= 0)
				return true;
			if(((LPINTERATOR)MaxElem)->Deep < MIN_DEEP_INTERATOR)
				return false;
			Element = ((LPINTERATOR)MaxElem)->Element;
			MaxElem = (LPHEADER_STRING*)((LPINTERATOR)MaxElem)->GetNext();
		}
		if((unsigned)*Element >= CurLoad->CountInStringArr)
			return false;
		*Element = CurLoad->StringArr[(unsigned)*Element];
		Element++;
	}
}

bool INSIDE_POINTER_INTERATOR::InitAll(LPBYTECODE_FILE CurLoad)
{
	LPINSIDE_POINTER_INTERATOR CurPointer = (LPINSIDE_POINTER_INTERATOR)this;
	LPINSIDE_POINTER_INTERATOR EndSection = (LPINSIDE_POINTER_INTERATOR)((char*)this + CurLoad->SizeLpInteratorSect);
	while(CurPointer < EndSection)
	{		
		if(CurPointer->iMethod >= CurLoad->CountInMetodArr)
			return false;

		LPOPCODE MethodBytecode = CurLoad->Methods[CurPointer->iMethod].Header->Bytecode;
		//Можно добавить проверку на входимость в границы метода
		for(unsigned i = 0; i < CurPointer->CountPointers; i++)
		{
			//LPOPCODE StartData = MethodBytecode + CurPointer->OffsetPointer[i];
			//switch(*(StartData - 1))
			//{
			//case OPCODE::SETM_obj_vb:
			//case OPCODE::SETM_obj_vw:
			//	{
			//		if((TINDEX_STRING)StartData >= CurLoad->CountInStringArr)
			//			return false;
			//		*(LPHEADER_STRING*)StartData = CurLoad->StringArr[(TINDEX_STRING)StartData];
			//	}
			//	break;
			//case OPCODE::SETM_method_vb:
			//case OPCODE::SETM_method_vw:
			//	{
			//		if((TINDEX_METHOD)StartData >= CurLoad->CountInMetodArr)
			//			return false;
			//		*(LPHEADER_METHOD*)StartData = CurLoad->Methods[(TINDEX_METHOD)StartData].Header;
			//	}
			//	break;
			//case OPCODE::RIOBJ_interator_vb_vb:
			//case OPCODE::RIOBJ_interator_vw_vw:
			//case OPCODE::RIGOBJ_interator_vb:
			//case OPCODE::RIGOBJ_interator_vw:
			//case OPCODE::WIOBJ_interator_vb_vb:
			//case OPCODE::WIOBJ_interator_vw_vw:
			//case OPCODE::WIGOBJ_interator_vb:
			//case OPCODE::WIGOBJ_interator_vw:
			//	{
			//		/*Если опкод - это интератор*/
			//		if((TOFFSET_INTERATOR)StartData >= CurLoad->SizeInteratorSection)
			//			return false;
			//		*(LPINTERATOR*)StartData = (LPINTERATOR)((char*)CurLoad->InteratorSection + (TOFFSET_INTERATOR)StartData);
			//	}
			//	break;
			//}
		}
	}
	return true;
}
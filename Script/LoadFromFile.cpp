#include <stdio.h>
#include "LoadFromFile.h"
#include "String.h"
#include "Method.h"
#include "AcselerateInterator.h"

#ifdef WIN32
#define ID_FILE HANDLE
#define OPEN_BINARY_FILE_FOR_READ(Name) CreateFileA(Name,FILE_ALL_ACCESS,\
			FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL)
#define READ_FILE(Id, Buf, SizeBuf, Readed) ReadFile(Id,Buf,SizeBuf,&Readed,NULL)
#define CLOSE_FILE(Id) CloseHandle(Id)
#define ERR_OPEN_VAL INVALID_HANDLE_VALUE
#else
#define ID_FILE FILE*
#define OPEN_BINARY_FILE_FOR_READ(Name) fopen(Name,"rb")
#define READ_FILE(Id, Buf, SizeBuf, Readed) Readed = fread(Buf,1,SizeBuf,Id)
#define CLOSE_FILE(Id) fclose(Id)
#define ERR_OPEN_VAL NULL
#endif

BYTECODE_FILE::ERRORS BYTECODE_FILE::LoadFile(LPEXTERNAL_CHAR Name)
{
	StringArr = NULL;
	Methods   = NULL;
	InteratorSection = NULL;
	LpInteratorSect = NULL;
	CountInMetodArr = 0;
	CountInStringArr = 0;
	ERRORS Result = ERRORS::SUCCESS;


	unsigned Readed = 0;
	ID_FILE InBtFile = OPEN_BINARY_FILE_FOR_READ(Name);
	DWORD SizeReaded;
	if(InBtFile == ERR_OPEN_VAL)
	{
		Result = ERRORS::NOT_OPEN_FILE;
		goto lblOut;
	}
	READ_FILE(InBtFile,&Signature,sizeof(Signature),SizeReaded); 
	if(SizeReaded < sizeof(Signature))
	{
		Result = ERRORS::NOT_READ_FILE;;
		goto lblOut;
	}
	Readed += sizeof(Signature);
	if(!Signature.CheckSignature())
	{
		Result = ERRORS::BAD_SIGNATURE;
		goto lblOut;
	}
	unsigned NumberSection = 0;
	SECTION_IMAGE CurSection;
	while((Readed < Signature.SizeFile) && (NumberSection < Signature.CountSections))
	{
		READ_FILE(InBtFile,&CurSection,sizeof(CurSection),SizeReaded); 
		if(SizeReaded < sizeof(CurSection))
		{
			Result = ERRORS::NOT_READ_FILE;
			goto lblOut;
		}
		Readed += sizeof(CurSection);
		switch(CurSection.Type)
		{
		case STRING_SECTION:
			{
				INTERNAL_CHAR InStr[2000];
				TSIZE_STR     Len;
				if(CountInStringArr == 0)
					StringArr = (LPHEADER_STRING*)MEM_ALLOC(sizeof(LPHEADER_STRING) * CurSection.CountElements);
				else
					StringArr = (LPHEADER_STRING*)MEM_REALLOC(StringArr, sizeof(LPHEADER_STRING) * (CountInStringArr + CurSection.CountElements));
				if(StringArr == NULL)
				{
					Result = ERRORS::NOT_ALLOC_STRING;
					goto lblOut;
				}
				unsigned i = CountInStringArr;
				CountInStringArr += CurSection.CountElements;
				for(;i < CountInStringArr;i++)
				{
					READ_FILE(InBtFile,&Len,sizeof(Len),SizeReaded); 
					if(SizeReaded < sizeof(Len))
					{
						Result = ERRORS::NOT_READ_FILE;
						goto lblOut;
					}
					READ_FILE(InBtFile,InStr,Len * sizeof(INTERNAL_CHAR),SizeReaded); 
					if(SizeReaded < (Len  * sizeof(INTERNAL_CHAR)))
					{
						Result = ERRORS::NOT_READ_FILE;
						goto lblOut;
					}
					Readed += sizeof(Len) + sizeof(INTERNAL_CHAR) * Len;
					StringArr[i] = StringClass->RegisterString(InStr, Len);
				}
			}
			break;
		case METHOD_SECTION:
			{
				if(CountInMetodArr == 0)
					Methods = (LPREG_METHOD)MEM_ALLOC(sizeof(REG_METHOD) * CurSection.CountElements);
				else
					Methods = (LPREG_METHOD)MEM_REALLOC(StringArr, sizeof(REG_METHOD) * (CountInMetodArr + CurSection.CountElements));

				if(Methods == NULL)
				{
					Result = ERRORS::NOT_ALLOC_METHODS;
					goto lblOut;
				}

				unsigned SizeMethod;
				unsigned i = CountInMetodArr;
				unsigned short CountLocalVar;
				CountInMetodArr += CurSection.CountElements;
				for(;i < CountInMetodArr;i++)
				{
					READ_FILE(InBtFile,&SizeMethod,sizeof(SizeMethod),SizeReaded); 
					if(SizeReaded < sizeof(SizeMethod))
					{
						Result = ERRORS::NOT_READ_FILE;
						goto lblOut;
					}

					READ_FILE(InBtFile,&CountLocalVar,sizeof(CountLocalVar),SizeReaded); 
					if(SizeReaded < sizeof(CountLocalVar))
					{
						Result = ERRORS::NOT_READ_FILE;
						goto lblOut;
					}
					
					Methods[i].Header = (LPHEADER_METHOD)MEM_ALLOC(SizeMethod + sizeof(HEADER_METHOD));
					READ_FILE(InBtFile, Methods[i].Header->Bytecode, SizeMethod, SizeReaded); 
					if(SizeReaded < SizeMethod)
					{
						Result = ERRORS::NOT_READ_FILE;
						goto lblOut;
					}
					Readed += sizeof(SizeMethod) + SizeMethod + sizeof(CountLocalVar);

					Methods[i].Header->IsBytecode = true;
					Methods[i].Header->Bytecode.MaxLocalVar = CountLocalVar;
					Methods[i].Size = SizeMethod;
					MethodClass->RegisterMethod(Methods[i].Header);
				}
			}
			break;
		case INTERATOR_SECTION:
			{
				if(InteratorSection != NULL)
				{
					Result = ERRORS::NO_ONE_INTERATOR_SECTION;
					goto lblOut;
				}

				InteratorSection = (void *)MEM_ALLOC(CurSection.Size);
				if(InteratorSection == NULL)
				{
					Result = ERRORS::NOT_ALLOC_INTERATORS;
					goto lblOut;
				}

				READ_FILE(InBtFile,InteratorSection,CurSection.Size,SizeReaded); 
			    if(SizeReaded < CurSection.Size)
				{
					Result = ERRORS::NOT_READ_FILE;
					goto lblOut;
				}
				SizeInteratorSection = CurSection.Size;
				CountInterators = CurSection.CountElements;
				Readed += CurSection.Size;
				if((NumberSection + 1) != Signature.CountSections)
				   break;
			}
			break;
		/*case DYNAMIC_OFFSET_TABLE:
			{
				if(LpInteratorSect != NULL)
				{
					Result = ERRORS::NO_ONE_OFFSET_INTERATOR_SECTION;
					goto lblOut;
				}

				LpInteratorSect = (void *)MEM_ALLOC(CurSection.Size);
				if(LpInteratorSect == NULL)
				{
					Result = ERRORS::NOT_ALLOC_INTERATORS;
					goto lblOut;
				}

				READ_FILE(InBtFile,LpInteratorSect,CurSection.Size,SizeReaded); 
			    if(SizeReaded < CurSection.Size)
				{
					Result = ERRORS::NOT_READ_FILE;
					goto lblOut;
				}
				SizeLpInteratorSect = CurSection.Size;
				Readed += CurSection.Size;
				if((NumberSection + 1) != Signature.CountSections)
				   break;
			}
			break;*/
		}
		NumberSection++;
	}
	if(InteratorSection != NULL)
	{
		if(!((LPINTERATOR) InteratorSection)->InitAll(this))
		{
			Result = ERRORS::BAD_INTERATOR_SECTION;
			goto lblOut;
		}
	}
	for(TINDEX_METHOD i = 0;i < CountInMetodArr;i++)
	{

	   if((Result = CheckMethod(Methods[i].Header, Methods[i].Size - Methods[i].Header->Size()))
	!= ERRORS::SUCCESS)
		   goto lblOut;
	}
	
	//if(LpInteratorSect != NULL)
	//{
	//   	if(!((LPINSIDE_POINTER_INTERATOR)LpInteratorSect)->InitAll(this))
	//	{
	//		Result = ERRORS::BAD_OFFSET_POINTER_TO_DYNAMIC_DATA;
	//		goto lblOut;
	//	}
	//}
	if(Signature.IndexMainMethod >= CountInMetodArr)
		Result = ERRORS::BAD_ENTRY;
	else
		MainMethod = Methods[Signature.IndexMainMethod].Header;
lblOut:
	if(InBtFile)
		CLOSE_FILE(InBtFile);
	return Result;
}
#define READ_OPERAND(Offset, Type) (*(Type*)(Code + (Offset)))
#define TEST_VAR_OPERAND(Offset, Type) if((*(Type*)(Code + (Offset))) >= MaxCountVar) goto lblBadVar

#define TEST_REL_ADDRESS(Offset, Type) 	{LPOPCODE AddressJmp = Code + READ_OPERAND(Offset,Type);\
			if((AddressJmp >= EndOpcode) || (AddressJmp <= Start)) goto lblBadRelAddress;}

#define TEST_ARG_METHOD(Offset, Code) {\
			LPARG_FUNC Arg = (LPARG_FUNC)(Code + Offset);\
			LPNUMBER_VAR Cur = Arg->Args, End = Cur + Arg->CountArg;\
			for(;Cur < End;Cur++)\
			  if(*Cur >= MaxCountVar) goto lblBadArgForFunction;\
			Code = (LPOPCODE)End;}

/*
*  Функция для верификации входного метода.
*  Проверяет корректность оперций а также инициализирует константы и интераторы.
*/
BYTECODE_FILE::ERRORS BYTECODE_FILE::CheckMethod(LPHEADER_METHOD Method, TSIZE_METHOD SizeMethod)
{
	LPOPCODE Code = Method->Bytecode, EndOpcode = Code + SizeMethod, Start = Code;
	NUMBER_VAR MaxCountVar = Method->Bytecode.MaxLocalVar;

	while(Code < EndOpcode) 
	switch(*Code)
	{
		case OPCODE::NOP:
		case OPCODE::RETURN:
			Code++;
		continue;
			#define IF_OPCODE_DEF(Type1, Type2) {\
			TEST_VAR_OPERAND(sizeof(OPCODE),Type1);\
			TEST_REL_ADDRESS(sizeof(OPCODE) + sizeof(Type1),Type2);\
			Code += (sizeof(OPCODE) + sizeof(Type1) + sizeof(Type2));continue;}
		case OPCODE::IF_vb_rab:
		case OPCODE::IFN_vb_rab: 
			IF_OPCODE_DEF(SHORT_NUMBER_VAR, SHORT_REL_ADDRESS)
		case OPCODE::IF_vw_raw:
		case OPCODE::IFN_vw_raw: 
			IF_OPCODE_DEF(NUMBER_VAR, REL_ADDRESS)

			#define IFEQ_OPCODE_DEF(Type1, Type2, Type3) {\
			TEST_VAR_OPERAND(sizeof(OPCODE),Type1);\
			TEST_VAR_OPERAND(sizeof(OPCODE) + sizeof(Type1),Type2);\
			TEST_REL_ADDRESS(sizeof(OPCODE) + sizeof(Type1) + sizeof(Type2),Type3);\
			Code += (sizeof(OPCODE) + sizeof(Type1) + sizeof(Type2) + sizeof(Type3));continue;}
		case OPCODE::IFEQ_vb_vb_rab: 
		case OPCODE::IFNEQ_vb_vb_rab:
		case OPCODE::IFLT_vb_vb_rab:
		case OPCODE::IFLE_vb_vb_rab:
			IFEQ_OPCODE_DEF(SHORT_NUMBER_VAR, SHORT_NUMBER_VAR, SHORT_REL_ADDRESS)

		case OPCODE::IFEQ_vw_vw_raw:
		case OPCODE::IFNEQ_vw_vw_raw:
		case OPCODE::IFLT_vw_vw_raw:
		case OPCODE::IFLE_vw_vw_raw:
			IFEQ_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR, REL_ADDRESS)

		#define MOV_OPCODE_DEF(Type1, Type2) {\
			TEST_VAR_OPERAND(sizeof(OPCODE),Type1);\
			TEST_VAR_OPERAND(sizeof(OPCODE) + sizeof(Type1),Type2);\
			Code += (sizeof(OPCODE) + sizeof(Type1) + sizeof(Type2));continue;}
		case OPCODE::MOV_vb_vb:
		case OPCODE::CLONE_vb_vb:
		case OPCODE::NEG_vb_vb:
		case OPCODE::NOT_vb_vb: 
		case OPCODE::DEL_vb_vb:
		case OPCODE::LEN_vb_vb:
		case OPCODE::TYPEOF_vb_vb:
		case OPCODE::NXTI_vb_vb:
			MOV_OPCODE_DEF(SHORT_NUMBER_VAR,SHORT_NUMBER_VAR)
		case OPCODE::MOV_vw_vw:
		case OPCODE::CLONE_vw_vw:
		case OPCODE::NEG_vw_vw:
		case OPCODE::NOT_vw_vw:
		case OPCODE::DEL_vw_vw:
		case OPCODE::LEN_vw_vw:
		case OPCODE::TYPEOF_vw_vw:
		case OPCODE::NXTI_vw_vb:
			MOV_OPCODE_DEF(NUMBER_VAR,NUMBER_VAR)

		#define RETURN_OPCODE_DEF(Type1) {\
			TEST_VAR_OPERAND(sizeof(OPCODE),Type1);\
			Code += (sizeof(OPCODE) + sizeof(Type1));continue;}
		case OPCODE::RETURN_vb:
		case OPCODE::INC_vb:
		case OPCODE::DEC_vb:
		case OPCODE::THROW_vb:
		case OPCODE::ARGC_vb:
		case OPCODE::GETTHIS_vb:
		case OPCODE::GETGLOB_vb:
			RETURN_OPCODE_DEF(SHORT_NUMBER_VAR)
		case OPCODE::RETURN_vw:
		case OPCODE::INC_vw:
		case OPCODE::DEC_vw:
		case OPCODE::THROW_vw:
		case OPCODE::ARGC_vw:
		case OPCODE::GETTHIS_vw:
		case OPCODE::GETGLOB_vw:
			RETURN_OPCODE_DEF(NUMBER_VAR)

		#define READARG_OPCODE_DEF(Type1, Type2) {\
			TEST_VAR_OPERAND(sizeof(OPCODE),Type1);\
			Code += (sizeof(OPCODE) + sizeof(Type1) + sizeof(Type2));continue;}
		case OPCODE::READARG_vb_vb: 
			READARG_OPCODE_DEF(SHORT_NUMBER_VAR,SHORT_NUMBER_VAR)
		case OPCODE::READARG_vw_vw:
			READARG_OPCODE_DEF(NUMBER_VAR,NUMBER_VAR)

		#define WRITEARG_OPCODE_DEF(Type1, Type2) {\
			TEST_VAR_OPERAND(sizeof(OPCODE) + sizeof(Type1),Type2);\
			Code += (sizeof(OPCODE) + sizeof(Type1) + sizeof(Type2));continue;}
	   	case OPCODE::WRITEARG_vb_vb: 
			WRITEARG_OPCODE_DEF(SHORT_NUMBER_VAR,SHORT_NUMBER_VAR)
		case OPCODE::WRITEARG_vw_vw:
			WRITEARG_OPCODE_DEF(NUMBER_VAR,NUMBER_VAR)

		#define MATH_OPCODE_DEF(Type1, Type2, Type3) {\
			TEST_VAR_OPERAND(sizeof(OPCODE),Type1);\
			TEST_VAR_OPERAND(sizeof(OPCODE) + sizeof(Type1),Type2);\
			TEST_VAR_OPERAND(sizeof(OPCODE) + sizeof(Type1) + sizeof(Type2),Type3);\
			Code += (sizeof(OPCODE) + sizeof(Type1) + sizeof(Type2) + sizeof(Type3));continue;}
		case OPCODE::AND_vb_vb_vb:
		case OPCODE::OR_vb_vb_vb:
		case OPCODE::XOR_vb_vb_vb:
		case OPCODE::SHR_vb_vb_vb:
		case OPCODE::SHL_vb_vb_vb:
		case OPCODE::ADD_vb_vb_vb:
		case OPCODE::SUB_vb_vb_vb:
		case OPCODE::MUL_vb_vb_vb:
		case OPCODE::DIV_vb_vb_vb:
		case OPCODE::MOD_vb_vb_vb:
		case OPCODE::EQ_vb_vb_vb:
		case OPCODE::NEQ_vb_vb_vb:
		case OPCODE::LT_vb_vb_vb:
		case OPCODE::LE_vb_vb_vb:
		case OPCODE::RBI_vb_vb_vb:
		case OPCODE::WBI_vb_vb_vb:
				MATH_OPCODE_DEF(SHORT_NUMBER_VAR, SHORT_NUMBER_VAR, SHORT_NUMBER_VAR)
		case OPCODE::AND_vw_vw_vw:
		case OPCODE::OR_vw_vw_vw:
		case OPCODE::XOR_vw_vw_vw:
		case OPCODE::SHR_vw_vw_vw:
		case OPCODE::SHL_vw_vw_vw:
		case OPCODE::ADD_vw_vw_vw: 
		case OPCODE::SUB_vw_vw_vw:
		case OPCODE::MUL_vw_vw_vw:
		case OPCODE::DIV_vw_vw_vw:
		case OPCODE::MOD_vw_vw_vw:
		case OPCODE::EQ_vw_vw_vw:
		case OPCODE::NEQ_vw_vw_vw:
		case OPCODE::LT_vw_vw_vw:
		case OPCODE::LE_vw_vw_vw:
		case OPCODE::RBI_vw_vw_vw:
		case OPCODE::WBI_vw_vw_vw:
				MATH_OPCODE_DEF(NUMBER_VAR,NUMBER_VAR, NUMBER_VAR)

		#define GOTO_OPCODE_DEF(Type1) {\
			TEST_REL_ADDRESS(sizeof(OPCODE),Type1);\
			Code += (sizeof(OPCODE) + sizeof(Type1));continue;}
		case OPCODE::GOTO_rab: 
		case OPCODE::UNEXCEPT_rab:
			GOTO_OPCODE_DEF(SHORT_REL_ADDRESS)
		case OPCODE::GOTO_raw:
		case OPCODE::UNEXCEPT_raw:
			GOTO_OPCODE_DEF(REL_ADDRESS)

		#define SETEXCEPT_OPCODE_DEF(Type1, Type2) {\
			TEST_REL_ADDRESS(sizeof(OPCODE),Type1);\
			TEST_VAR_OPERAND(sizeof(OPCODE) + sizeof(Type1),Type2);\
			Code += (sizeof(OPCODE) + sizeof(Type1) + sizeof(Type2));continue;}
		case OPCODE::SETEXEPT_rab_vb:
			SETEXCEPT_OPCODE_DEF(SHORT_REL_ADDRESS, SHORT_NUMBER_VAR)
		case OPCODE::SETEXEPT_raw_vw:
			SETEXCEPT_OPCODE_DEF(REL_ADDRESS, NUMBER_VAR)

		#define CALL_OPCODE_DEF(Type1){\
			TEST_VAR_OPERAND(sizeof(OPCODE),Type1);\
			TEST_ARG_METHOD(sizeof(OPCODE) + sizeof(Type1),Code);\
			continue;}
		case OPCODE::CALL_vb_w: 
			CALL_OPCODE_DEF(SHORT_NUMBER_VAR)
		case OPCODE::CALL_vw_w:
			CALL_OPCODE_DEF(NUMBER_VAR)

		#define VIRCALL_OPCODE_DEF(Type1, Type2)  {\
			TEST_VAR_OPERAND(sizeof(OPCODE),Type1);\
			TEST_VAR_OPERAND(sizeof(OPCODE) + sizeof(Type1),Type2);\
			TEST_ARG_METHOD(sizeof(OPCODE) + sizeof(Type1) + sizeof(Type2),Code);\
			continue;}
		case OPCODE::CALL_vb_vb_w: 
		case OPCODE::VIRCALL_vb_vb_w:
		case OPCODE::NEWOBJ_vb_vb_w:
			VIRCALL_OPCODE_DEF(SHORT_NUMBER_VAR,SHORT_NUMBER_VAR)
		case OPCODE::CALL_vw_vw_w: 
		case OPCODE::VIRCALL_vw_vw_w:
		case OPCODE::NEWOBJ_vw_vw_w:
			VIRCALL_OPCODE_DEF(NUMBER_VAR,NUMBER_VAR)

		#define VIRCALL_WITH_RET_OPCODE_DEF(ResType, Type1, Type2)  {\
			TEST_VAR_OPERAND(sizeof(OPCODE), ResType);\
			TEST_VAR_OPERAND(sizeof(OPCODE) + sizeof(ResType), Type1);\
			TEST_VAR_OPERAND(sizeof(OPCODE) + sizeof(ResType) + sizeof(Type1), Type2);\
			TEST_ARG_METHOD(sizeof(OPCODE) + sizeof(ResType) + sizeof(Type1) + sizeof(Type2),Code);\
			continue;}
		case OPCODE::VIRCALL_vb_vb_vb_w:
			VIRCALL_WITH_RET_OPCODE_DEF(SHORT_NUMBER_VAR,SHORT_NUMBER_VAR,SHORT_NUMBER_VAR)
		case OPCODE::VIRCALL_vw_vw_vw_w:
			VIRCALL_WITH_RET_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR,NUMBER_VAR)

		/*Загрузка константной строки*/
		#define LOAD_CONST_OPCODE(TypeString, Type1) {\
			TEST_VAR_OPERAND(sizeof(OPCODE) + sizeof(TypeString), Type1);\
			TINDEX_STRING iString = (TINDEX_STRING)READ_OPERAND(sizeof(OPCODE), TypeString);\
			if(iString >= CountInStringArr) goto lblBadStringIndex;\
			READ_OPERAND(sizeof(OPCODE), TypeString) = StringArr[iString];\
			Code += (sizeof(OPCODE) + sizeof(TypeString) + sizeof(Type1));continue;}
		/*Загрузка константного метода*/
		case OPCODE::SETCONST_vb_index:  LOAD_CONST_OPCODE(LPHEADER_STRING, SHORT_NUMBER_VAR)
		case OPCODE::SETCONST_vw_index:  LOAD_CONST_OPCODE(LPHEADER_STRING, NUMBER_VAR)
		
		#define INTOBJ_OPCODE(TypeInterator, Type1, Type2) {\
			TEST_VAR_OPERAND(sizeof(OPCODE) + sizeof(TypeInterator), Type1);\
			TEST_VAR_OPERAND(sizeof(OPCODE) + sizeof(TypeInterator) + sizeof(Type1), Type2);\
			TOFFSET_INTERATOR oInterator = (TOFFSET_INTERATOR)READ_OPERAND(sizeof(OPCODE), TypeInterator);\
			if(oInterator >= SizeInteratorSection) goto lblBadOffsetInterator;\
			READ_OPERAND(sizeof(OPCODE), TypeInterator) = (TypeInterator)((char*)InteratorSection + oInterator);\
			Code += (sizeof(OPCODE) + sizeof(TypeInterator) + sizeof(Type1) + sizeof(Type2));continue;}
		case OPCODE::RIOBJ_vb_vb_indexes: 
		case OPCODE::WIOBJ_vb_vb_indexes:
			INTOBJ_OPCODE(LPINTERATOR, SHORT_NUMBER_VAR,SHORT_NUMBER_VAR)
		case OPCODE::RIOBJ_vw_vw_indexes: 
		case OPCODE::WIOBJ_vw_vw_indexes:
			INTOBJ_OPCODE(LPINTERATOR, NUMBER_VAR,NUMBER_VAR)

			#define INTGOBJ_OPCODE(TypeInterator, Type1) {\
			TEST_VAR_OPERAND(sizeof(OPCODE) + sizeof(TypeInterator), Type1);\
			TOFFSET_INTERATOR oInterator = (TOFFSET_INTERATOR)READ_OPERAND(sizeof(OPCODE), TypeInterator);\
			if(oInterator >= SizeInteratorSection) goto lblBadOffsetInterator;\
			READ_OPERAND(sizeof(OPCODE), TypeInterator) = (TypeInterator)((char*)InteratorSection + oInterator);\
			Code += (sizeof(OPCODE) + sizeof(TypeInterator) + sizeof(Type1));continue;}
		case OPCODE::RIGOBJ_vb_indexes: 
		case OPCODE::WIGOBJ_vb_indexes:
			INTGOBJ_OPCODE(LPINTERATOR, SHORT_NUMBER_VAR)
		case OPCODE::RIGOBJ_vw_indexes:
		case OPCODE::WIGOBJ_vw_indexes:
			INTGOBJ_OPCODE(LPINTERATOR, NUMBER_VAR)

		default:
			goto lblBadOpcode;
	}

	return ERRORS::SUCCESS;

lblBadVar:
	BadOffset = (unsigned)Code - (unsigned)Start;
	return ERRORS::BAD_NUMBER_VAR;
lblBadRelAddress:
	BadOffset = (unsigned)Code - (unsigned)Start;
    return ERRORS::BAD_REL_ADDRESS;
lblBadArgForFunction:
	BadOffset = (unsigned)Code - (unsigned)Start;
    return ERRORS::BAD_ARG_FUNCTION;
lblBadOpcode:
	BadOffset = (unsigned)Code - (unsigned)Start;
	return ERRORS::BAD_OPCODE;	
lblBadOffsetInterator:
	BadOffset = (unsigned)Code - (unsigned)Start;
	return ERRORS::BAD_OFFSET_INTERATOR;\
lblBadStringIndex:
	BadOffset = (unsigned)Code - (unsigned)Start;
    return ERRORS::BAD_STRING_INDEX;\
lblBadMethodIndex:
	BadOffset = (unsigned)Code - (unsigned)Start;
	return ERRORS::BAD_METHOD_INDEX;

}
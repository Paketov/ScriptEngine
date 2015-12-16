#include "Context.h"
#include "Opcode.h"
#include "String.h"
#include "Method.h"
#include "GlobalScope.h"
#include "InlineTypeData.h"
#include "ConstScope.h"


#define READ_OPERAND(Offset, Type) (*(Type*)(ip + (Offset)))
#define GET_OPERAND_POINTER(Offset, Type) ((Type*)(ip + (Offset)))
#define GET_VAR(Offset, Type) (((LPINLINE_INSIDE_DATA)Frame.StartVar) + *(Type*)(ip + (Offset)))
#define GET_VAR_VAL(Offset, Type) (((LPINLINE_INSIDE_DATA)Frame.StartVar)[*(Type*)(ip + (Offset))])

	



EXECUTE_CONTEXT::EXECUTE_CONTEXT()
{
	LastGcState = GC_STAGE::CLEANING_COMPLETE;
	ObjManager = &MainObjectManager;
	StringClass = &MainStringClass;
	GlobalObject = INSIDE_DATA::Null;
	ConstValues = INSIDE_DATA::Null;
	ObjManager->ClassRegister(StringClass);
	ObjManager->ClassRegister(&FunctionClass);
	EXECUTE_CONTEXT::ForThisThread = this;
}

EXECUTE_CONTEXT::EXECUTE_CONTEXT(unsigned nStartSizeStack, unsigned nMaxSizeStack): STACK_LOCAL_VAR(nStartSizeStack, nMaxSizeStack)
{
	//Создаём интератор по умолчанию
	//Objects = new OBJECT_VAR;
	LastGcState = GC_STAGE::CLEANING_COMPLETE;
	ObjManager = &MainObjectManager;
	StringClass = &MainStringClass;
	LPGLOBAL_SCOPE_CLASS GlobalScope = new GLOBAL_SCOPE_CLASS(StringClass);
	GlobalObject = GlobalScope->CreateInstance(this);
	ConstValues = MainConstScope.CreateInstance(this);
	EXECUTE_CONTEXT::ForThisThread = this;
}

EXECUTE_CONTEXT::EXECUTE_CONTEXT
(
	LPINSIDE_DATA CommonScope,
	LPINSIDE_DATA CommonConst,
	LPCLASS_MANAGER CommonObjectManager,
	LPSTRING_CLASS CommonStringClass,
	unsigned  nStartSizeStack,
	unsigned  nMaxSizeStack
)
: STACK_LOCAL_VAR(nStartSizeStack, nMaxSizeStack)
{
	LastGcState = GC_STAGE::CLEANING_COMPLETE;
	ObjManager = CommonObjectManager;
	StringClass = CommonStringClass;
	GlobalObject = *CommonScope;
	ConstValues = *CommonConst;
	EXECUTE_CONTEXT::ForThisThread = this;
}


EXECUTE_CONTEXT::~EXECUTE_CONTEXT()
{

}


/*
* Функция итрпретирования байт-кода
*/
INSIDE_DATA __fastcall EXECUTE_CONTEXT::Execute
(
	LPHEADER_METHOD HeaderMethod,
	LPARG_FUNC Arg,
	LPINSIDE_DATA ThisScope
) HAS_THROW
{
	FRAME_BORDER Frame(HeaderMethod->Bytecode.MaxLocalVar, ThisFrame);
	ThisFrame = &Frame;
	NUMBER_VAR LastExeptionVar = ZELLI_EXCEPT_HANDLER::NONE_VARIABLE;
	LPOPCODE ip = HeaderMethod->Bytecode;

	while(true)
	{
		try
		{
			while (true)
			switch(*ip)
			{
				case OPCODE::NOP:
					ip++;
					continue;
					////////////////////////////////////////////////
					#define IF_OPCODE_DEF(Typ1 , Typ2) {\
					    if(GET_VAR(sizeof(OPCODE),Typ1)->operator bool()){\
					   	    ip += READ_OPERAND(sizeof(OPCODE) + sizeof(Typ1),Typ2);\
							continue;}\
					    ip += (sizeof(Typ1) + sizeof(Typ2) + sizeof(OPCODE));\
						continue;}
				case OPCODE::IF_vb_rab: IF_OPCODE_DEF(SHORT_NUMBER_VAR , SHORT_REL_ADDRESS) 
					///////////////////////////////////////////
					#define IFN_OPCODE_DEF(Typ1 , Typ2) {\
					    if(!GET_VAR(sizeof(OPCODE),Typ1)->operator bool()){\
					   	    ip += READ_OPERAND(sizeof(OPCODE) + sizeof(Typ1),Typ2);\
							continue;}\
					    ip += (sizeof(Typ1) + sizeof(Typ2) + sizeof(OPCODE));\
						continue;}
				case OPCODE::IFN_vb_rab: IFN_OPCODE_DEF(SHORT_NUMBER_VAR, SHORT_REL_ADDRESS)
					////////////////////////////////////////////////
					#define IFEQ_OPCODE_DEF(Typ1 , Typ2, Typ3) {\
						if(GET_VAR_VAL(sizeof(OPCODE), Typ1) == GET_VAR_VAL(sizeof(OPCODE) + sizeof(Typ1),Typ2))\
							ip += READ_OPERAND(sizeof(OPCODE) + sizeof(Typ1) + sizeof(Typ2),Typ3);\
						else ip += (sizeof(OPCODE) + sizeof(Typ1) + sizeof(Typ2) + sizeof(Typ3));\
						continue;}
				case OPCODE::IFEQ_vb_vb_rab:IFEQ_OPCODE_DEF(SHORT_NUMBER_VAR,SHORT_NUMBER_VAR, SHORT_REL_ADDRESS)
					#define IFNEQ_OPCODE_DEF(Typ1 , Typ2, Typ3) {\
						if(GET_VAR_VAL(sizeof(OPCODE), Typ1) != GET_VAR_VAL(sizeof(OPCODE) + sizeof(Typ1),Typ2))\
							ip += READ_OPERAND(sizeof(OPCODE) + sizeof(Typ1) + sizeof(Typ2),Typ3);\
						else ip += (sizeof(OPCODE) + sizeof(Typ1) + sizeof(Typ2) + sizeof(Typ3));\
						continue;}
				case OPCODE::IFNEQ_vb_vb_rab: IFNEQ_OPCODE_DEF(SHORT_NUMBER_VAR,SHORT_NUMBER_VAR, SHORT_REL_ADDRESS)
					#define IFLT_OPCODE_DEF(Typ1 , Typ2, Typ3) {\
						if(GET_VAR_VAL(sizeof(OPCODE), Typ1) < GET_VAR_VAL(sizeof(OPCODE) + sizeof(Typ1),Typ2))\
							ip += READ_OPERAND(sizeof(OPCODE) + sizeof(Typ1) + sizeof(Typ2),Typ3);\
						else ip += (sizeof(OPCODE) + sizeof(Typ1) + sizeof(Typ2) + sizeof(Typ3));\
						continue;}
				case OPCODE::IFLT_vb_vb_rab: IFLT_OPCODE_DEF(SHORT_NUMBER_VAR,SHORT_NUMBER_VAR, SHORT_REL_ADDRESS)
					#define IFLE_OPCODE_DEF(Typ1 , Typ2, Typ3) {\
						if(GET_VAR_VAL(sizeof(OPCODE), Typ1) < GET_VAR_VAL(sizeof(OPCODE) + sizeof(Typ1),Typ2))\
							ip += READ_OPERAND(sizeof(OPCODE) + sizeof(Typ1) + sizeof(Typ2),Typ3);\
						else ip += (sizeof(OPCODE) + sizeof(Typ1) + sizeof(Typ2) + sizeof(Typ3));\
						continue;}
				case OPCODE::IFLE_vb_vb_rab: IFLE_OPCODE_DEF(SHORT_NUMBER_VAR,SHORT_NUMBER_VAR, SHORT_REL_ADDRESS)
					//MOV==========================================
						#define MOV_OPCODE_DEF(Typ1 , Typ2) {\
						GET_VAR_VAL(sizeof(OPCODE),Typ1) = GET_VAR_VAL(sizeof(OPCODE) + sizeof(Typ1),Typ2);\
						ip += (sizeof(OPCODE) + sizeof(Typ1) + sizeof(Typ2));\
						continue;}
				case OPCODE::MOV_vb_vb: MOV_OPCODE_DEF(SHORT_NUMBER_VAR, SHORT_NUMBER_VAR)
						#define CLONE_OPCODE_DEF(Typ1 , Typ2) {\
						LPINSIDE_DATA s = GET_VAR(sizeof(OPCODE) + sizeof(Typ1),Typ2);\
						switch(s->TypeData){\
						case INSIDE_DATA::TYPEDATA_OBJECT:\
						    /**d = Objects->CloneObject(s->Object);*/break;\
						default:\
						GET_VAR_VAL(sizeof(OPCODE),Typ1) = *s;\
						}\
						ip += (sizeof(OPCODE) + sizeof(Typ1) + sizeof(Typ2));\
						continue;}
				case OPCODE::CLONE_vb_vb: CLONE_OPCODE_DEF(SHORT_NUMBER_VAR, SHORT_NUMBER_VAR)
				case OPCODE::RETURN:{POP_EXCEPT; return INSIDE_DATA::Null;}
					#define RETURN_OPCODE_DEF(Typ1) {POP_EXCEPT;return GET_VAR_VAL(sizeof(OPCODE),Typ1);}
				case OPCODE::RETURN_vb: RETURN_OPCODE_DEF(SHORT_NUMBER_VAR)
					#define READ_ARG_OPCODE_DEF(Typ1 , Typ2) {\
						GET_VAR_VAL(sizeof(OPCODE),Typ1)=\
						*(Frame.Previous->StartVar + READ_OPERAND(sizeof(OPCODE) + sizeof(Typ1),Typ2));\
						ip += (sizeof(OPCODE) + sizeof(Typ1) + sizeof(Typ2));continue;}
				case OPCODE::READARG_vb_vb: READ_ARG_OPCODE_DEF(SHORT_NUMBER_VAR, SHORT_NUMBER_VAR)
					#define WRITE_ARG_OPCODE_DEF(Typ1 , Typ2) {\
						Typ1 Operand1 = READ_OPERAND(sizeof(OPCODE),Typ1);\
						if(Operand1 >= Arg->CountArg){\
							THROW_NATIVE_EXCEPT("Out of argument range", HEADER_EXCEPTION::OUT_OF_ARGUMENT_RANGE);}\
						*(Frame.Previous->StartVar + Arg->Args[Operand1])=\
						GET_VAR_VAL(sizeof(OPCODE) + sizeof(Typ1),Typ2);\
						ip += (sizeof(OPCODE) + sizeof(Typ1) + sizeof(Typ2));continue;}
				case OPCODE::WRITEARG_vb_vb: WRITE_ARG_OPCODE_DEF(SHORT_NUMBER_VAR, SHORT_NUMBER_VAR)
						#define NEG_OPCODE_DEF(Type1, Type2) {\
			            GET_VAR_VAL(sizeof(OPCODE) + sizeof(Type1),Type2)=\
						-GET_VAR_VAL(sizeof(OPCODE),Type1);\
						ip += (sizeof(OPCODE) + sizeof(Type1) + sizeof(Type2));continue;}
				case OPCODE::NEG_vb_vb: NEG_OPCODE_DEF(SHORT_NUMBER_VAR, SHORT_NUMBER_VAR)
						#define NOT_OPCODE_DEF(Type1, Type2) {\
			            GET_VAR_VAL(sizeof(OPCODE) + sizeof(Type1),Type2) = \
						~GET_VAR_VAL(sizeof(OPCODE),Type1);\
						ip += (sizeof(OPCODE) + sizeof(Type1) + sizeof(Type2));continue;}
				case OPCODE::NOT_vb_vb: NOT_OPCODE_DEF(SHORT_NUMBER_VAR, SHORT_NUMBER_VAR)
						#define MATH_OPCODE_DEF(Type1, Type2, Type3,Act) {\
						GET_VAR_VAL(sizeof(OPCODE),Type1) =\
						GET_VAR_VAL(sizeof(OPCODE) + sizeof(Type1),Type2)\
						Act GET_VAR_VAL(sizeof(OPCODE) + sizeof(Type1) + sizeof(Type2),Type3);\
						ip += (sizeof(OPCODE) + sizeof(Type1) + sizeof(Type2) + sizeof(Type3));continue;}
				case OPCODE::AND_vb_vb_vb: MATH_OPCODE_DEF(SHORT_NUMBER_VAR, SHORT_NUMBER_VAR, SHORT_NUMBER_VAR,&)
				case OPCODE::OR_vb_vb_vb:  MATH_OPCODE_DEF(SHORT_NUMBER_VAR,SHORT_NUMBER_VAR,SHORT_NUMBER_VAR,|)
				case OPCODE::XOR_vb_vb_vb: MATH_OPCODE_DEF(SHORT_NUMBER_VAR,SHORT_NUMBER_VAR,SHORT_NUMBER_VAR, ^)
				case OPCODE::SHR_vb_vb_vb: MATH_OPCODE_DEF(SHORT_NUMBER_VAR,SHORT_NUMBER_VAR,SHORT_NUMBER_VAR, >>)
				case OPCODE::SHL_vb_vb_vb: MATH_OPCODE_DEF(SHORT_NUMBER_VAR,SHORT_NUMBER_VAR,SHORT_NUMBER_VAR,<<)
				case OPCODE::ADD_vb_vb_vb: MATH_OPCODE_DEF(SHORT_NUMBER_VAR,SHORT_NUMBER_VAR,SHORT_NUMBER_VAR,+)
				case OPCODE::SUB_vb_vb_vb: MATH_OPCODE_DEF(SHORT_NUMBER_VAR,SHORT_NUMBER_VAR,SHORT_NUMBER_VAR,-)
				case OPCODE::MUL_vb_vb_vb: MATH_OPCODE_DEF(SHORT_NUMBER_VAR,SHORT_NUMBER_VAR,SHORT_NUMBER_VAR,*)
				case OPCODE::DIV_vb_vb_vb: MATH_OPCODE_DEF(SHORT_NUMBER_VAR,SHORT_NUMBER_VAR,SHORT_NUMBER_VAR,/)
				case OPCODE::MOD_vb_vb_vb: MATH_OPCODE_DEF(SHORT_NUMBER_VAR,SHORT_NUMBER_VAR,SHORT_NUMBER_VAR,%)
				case OPCODE::EQ_vb_vb_vb: MATH_OPCODE_DEF(SHORT_NUMBER_VAR, SHORT_NUMBER_VAR, SHORT_NUMBER_VAR,==)
				case OPCODE::NEQ_vb_vb_vb: MATH_OPCODE_DEF(SHORT_NUMBER_VAR, SHORT_NUMBER_VAR, SHORT_NUMBER_VAR,!=)
				case OPCODE::LT_vb_vb_vb: MATH_OPCODE_DEF(SHORT_NUMBER_VAR, SHORT_NUMBER_VAR, SHORT_NUMBER_VAR,<)
				case OPCODE::LE_vb_vb_vb: MATH_OPCODE_DEF(SHORT_NUMBER_VAR, SHORT_NUMBER_VAR, SHORT_NUMBER_VAR,<=)
						#define INC_OPCODE_DEF(Type1) {\
						++GET_VAR_VAL(sizeof(OPCODE),Type1);\
						ip += (sizeof(OPCODE) + sizeof(Type1));continue;}
				case OPCODE::INC_vb: INC_OPCODE_DEF(SHORT_NUMBER_VAR)
						#define DEC_OPCODE_DEF(Type1) {\
						--GET_VAR_VAL(sizeof(OPCODE),Type1);\
						ip += (sizeof(OPCODE) + sizeof(Type1));continue;}
				case OPCODE::DEC_vb: DEC_OPCODE_DEF(SHORT_NUMBER_VAR)
						#define GOTO_OPCODE_DEF(Type1) {ip += READ_OPERAND(sizeof(OPCODE),Type1);continue;}
				case OPCODE::GOTO_rab: GOTO_OPCODE_DEF(SHORT_REL_ADDRESS)
						#define SETEXEPT_OPCODE_DEF(Type1, Type2) {\
						NUMBER_VAR ExceptVar = READ_OPERAND(sizeof(OPCODE) + sizeof(Type1),Type2);\
						LPINSIDE_DATA lpExceptVar = Frame.StartVar + ExceptVar;\
						lpExceptVar->TypeData = INSIDE_DATA::TYPEDATA_EXCEPT;\
						lpExceptVar->ExcHand.CatchAddress = READ_OPERAND(sizeof(OPCODE),Type1) + ip;\
						lpExceptVar->ExcHand.NextHandler = LastExeptionVar;\
						LastExeptionVar = ExceptVar;\
						ip += (sizeof(OPCODE) + sizeof(Type1) + sizeof(Type2));continue;}
				case OPCODE::SETEXEPT_rab_vb: SETEXEPT_OPCODE_DEF(SHORT_REL_ADDRESS, SHORT_NUMBER_VAR)
						#define UNEXCEPT_OPCODE_DEF(Type1) {\
						if (LastExeptionVar == ZELLI_EXCEPT_HANDLER::NONE_VARIABLE)\
							THROW_NATIVE_EXCEPT("Local scope not have exception handler type variable.", HEADER_EXCEPTION::LOCAL_SCOPE_NOT_HAVE_EXCEPT_HANDLERS);\
						LPINSIDE_DATA lpExceptVar = ThisFrame->StartVar + LastExeptionVar;\
						if (!lpExceptVar->IsExceptionHandler)\
							THROW_NATIVE_EXCEPT("This variable have not exception handler value.", HEADER_EXCEPTION::VARIABLE_IS_NOT_EXCEPTION_HANDLER);\
						lpExceptVar->SetNull();\
						LastExeptionVar = lpExceptVar->ExcHand.NextHandler;\
						ip += READ_OPERAND(sizeof(OPCODE),Type1);continue;}
				case OPCODE::UNEXCEPT_rab: UNEXCEPT_OPCODE_DEF(SHORT_REL_ADDRESS)
					    #define THROW_OPCODE_DEF(Type1) { throw GET_VAR(sizeof(OPCODE),Type1);}
				case OPCODE::THROW_vb: THROW_OPCODE_DEF(SHORT_NUMBER_VAR)
						#define CALL_OPCODE_DEF(Type1)  {\
						LPARG_FUNC Arg = (LPARG_FUNC)(ip + (sizeof(OPCODE) + sizeof(Type1)));\
						GET_VAR_VAL(sizeof(OPCODE),Type1)(this, Arg);\
					    ip = (LPOPCODE)(Arg->Args + Arg->CountArg); \
					    continue;}
				case OPCODE::CALL_vb_w: CALL_OPCODE_DEF(SHORT_NUMBER_VAR)
						#define CALL_WITH_RET_OPCODE_DEF(ResultType, Type1)  {\
						LPARG_FUNC Arg = (LPARG_FUNC)(ip + (sizeof(OPCODE) + sizeof(ResultType) + sizeof(Type1)));\
						GET_VAR_VAL(sizeof(OPCODE),ResultType) = GET_VAR_VAL(\
						sizeof(OPCODE) + sizeof(ResultType),Type1)(this, Arg);\
					    ip = (LPOPCODE)(Arg->Args + Arg->CountArg); \
					    continue;}
				case OPCODE::CALL_vb_vb_w: CALL_WITH_RET_OPCODE_DEF(SHORT_NUMBER_VAR,SHORT_NUMBER_VAR)
						#define VIRCALL_OPCODE_DEF(Type1, Type2)  {\
						LPARG_FUNC Arg = (LPARG_FUNC)(ip + (sizeof(OPCODE) + sizeof(Type1) + sizeof(Type2)));\
						GET_VAR_VAL(sizeof(OPCODE),Type1)(this,\
						(Frame.StartVar + READ_OPERAND(sizeof(OPCODE) + sizeof(Type1),Type2)), Arg);\
					    ip = (LPOPCODE)(Arg->Args + Arg->CountArg); \
					    continue;}
				case OPCODE::VIRCALL_vb_vb_w:VIRCALL_OPCODE_DEF(SHORT_NUMBER_VAR,SHORT_NUMBER_VAR)
						#define VIRCALL_WITH_RET_OPCODE_DEF(ResType, Type1, Type2)  {\
						LPARG_FUNC Arg = (LPARG_FUNC)(ip + (sizeof(OPCODE) + sizeof(ResType) + sizeof(Type1) + sizeof(Type2)));\
						GET_VAR_VAL(sizeof(OPCODE),ResType) = GET_VAR_VAL(sizeof(OPCODE) + sizeof(ResType),Type1)(this,\
						GET_VAR(sizeof(OPCODE) + sizeof(ResType) + sizeof(Type1),Type2)\
						,Arg);\
					    ip = (LPOPCODE)(Arg->Args + Arg->CountArg); \
					    continue;}
				case OPCODE::VIRCALL_vb_vb_vb_w:VIRCALL_WITH_RET_OPCODE_DEF(SHORT_NUMBER_VAR,SHORT_NUMBER_VAR,SHORT_NUMBER_VAR)
						#define NEWOBJ_OPCODE_DEF(Type1, Type2)  {\
						LPARG_FUNC Arg =  (LPARG_FUNC)(ip + (sizeof(OPCODE) + sizeof(Type1) + sizeof(Type2))); \
						GET_VAR_VAL(sizeof(OPCODE) + sizeof(Type1),Type2) = \
						ObjManager->CreateObject(GET_VAR(sizeof(OPCODE),Type1),this,Arg);\
					    ip = (LPOPCODE)(Arg->Args + Arg->CountArg);\
					    continue;}
				case OPCODE::NEWOBJ_vb_vb_w: NEWOBJ_OPCODE_DEF(SHORT_NUMBER_VAR, SHORT_NUMBER_VAR)
						#define RBI_OPCODE_DEF(Type1, Type2, Type3) {\
						LPINSIDE_DATA b = GET_VAR(sizeof(OPCODE) + sizeof(Type1),Type2);\
						if(!b->IsObject) THROW_NATIVE_EXCEPT("Type variable is not object.", HEADER_EXCEPTION::VAL_IS_NOT_OBJECT);\
						GET_VAR_VAL(sizeof(OPCODE),Type1) = b->Object.ReadMember(\
						GET_VAR(sizeof(OPCODE) + sizeof(Type1) + sizeof(Type2),Type3));\
						ip += (sizeof(OPCODE) + sizeof(Type1) + sizeof(Type2) + sizeof(Type3));continue;}
				case OPCODE::RBI_vb_vb_vb: RBI_OPCODE_DEF(SHORT_NUMBER_VAR, SHORT_NUMBER_VAR, SHORT_NUMBER_VAR)
						#define RBW_OPCODE_DEF(Type1, Type2, Type3) {\
						LPINSIDE_DATA a = GET_VAR(sizeof(OPCODE),Type1);\
						if(!a->IsObject) THROW_NATIVE_EXCEPT("Type variable is not object.", HEADER_EXCEPTION::VAL_IS_NOT_OBJECT);\
						a->Object.WriteMember(GET_VAR(sizeof(OPCODE) + sizeof(Type1),Type2),\
					    GET_VAR(sizeof(OPCODE) + sizeof(Type1) + sizeof(Type2),Type3));\
						ip += (sizeof(OPCODE) + sizeof(Type1) + sizeof(Type2) + sizeof(Type3));continue;}
				case OPCODE::WBI_vb_vb_vb: RBW_OPCODE_DEF(SHORT_NUMBER_VAR, SHORT_NUMBER_VAR, SHORT_NUMBER_VAR)
						#define DEL_OPCODE_DEF(Type1)  {ip += (sizeof(OPCODE) + sizeof(Type1));continue;}
				case OPCODE::DEL_vb_vb: DEL_OPCODE_DEF(SHORT_NUMBER_VAR)
					    #define ARGC_OPCODE_DEF(Type1) {\
						GET_VAR_VAL(sizeof(OPCODE),Type1) = Arg->CountArg;\
						ip += (sizeof(OPCODE) + sizeof(Type1));continue;}
				case OPCODE::ARGC_vb: ARGC_OPCODE_DEF(SHORT_NUMBER_VAR)
						#define LEN_OPCODE_DEF(Type1, Type2) {\
						GET_VAR_VAL(sizeof(OPCODE),Type1) = GET_VAR(sizeof(OPCODE) + sizeof(Type1),Type2)->Length;\
						ip += (sizeof(OPCODE) + sizeof(Type1) + sizeof(Type2));continue;}
				case OPCODE::LEN_vb_vb: LEN_OPCODE_DEF(SHORT_NUMBER_VAR, SHORT_NUMBER_VAR)
						#define TYPEOF_OPCODE_DEF(Type1, Type2) {\
						GET_VAR_VAL(sizeof(OPCODE),Type1) = \
						StringClass->TypeOf(GET_VAR(sizeof(OPCODE) + sizeof(Type1),Type2));\
						ip += (sizeof(OPCODE) + sizeof(Type1) + sizeof(Type2));continue;}
				case OPCODE::TYPEOF_vb_vb: TYPEOF_OPCODE_DEF(SHORT_NUMBER_VAR, SHORT_NUMBER_VAR)
					#define NXTI_OPCODE_DEF(Type1, Type2) {\
						LPINSIDE_DATA b = GET_VAR(sizeof(OPCODE) + sizeof(Type1),Type2);\
						if(!b->IsObject) THROW_NATIVE_EXCEPT("Type variable is not object.", HEADER_EXCEPTION::VAL_IS_NOT_OBJECT);\
						b->Object.EnumKey(GET_VAR(sizeof(OPCODE),Type1));\
						ip += (sizeof(OPCODE) + sizeof(Type1) + sizeof(Type2));continue;}
				case OPCODE::NXTI_vb_vb: NXTI_OPCODE_DEF(SHORT_NUMBER_VAR, SHORT_NUMBER_VAR)
						#define SETVAL_OPCODE_DEF(Type1, Type2) {\
						GET_VAR_VAL(sizeof(OPCODE), Type1) = HeaderMethod->ConstObjs[READ_OPERAND(sizeof(OPCODE) + sizeof(Type1), Type2)];\
						ip += (sizeof(OPCODE) + sizeof(Type1) + sizeof(Type2));continue;}
				case OPCODE::SETCONST_vb_index:  SETVAL_OPCODE_DEF(SHORT_NUMBER_VAR, NUMBER_VAR)
					    #define GETTHIS_OPCODE_DEF(Type1) {\
						GET_VAR_VAL(sizeof(OPCODE),Type1) = ThisScope;\
						ip += (sizeof(OPCODE) + sizeof(Type1));continue;}
				case OPCODE::GETTHIS_vb: GETTHIS_OPCODE_DEF(SHORT_NUMBER_VAR)
						//Read Index Object
					    #define RIOBJ_OPCODE_DEF(Type1) {\
						LPINSIDE_DATA ConstVals = HeaderMethod->ConstObjs;\
						Type1 * Indexes = GET_OPERAND_POINTER(sizeof(OPCODE) + sizeof(Type1) * 3,Type1);\
						INLINE_INSIDE_DATA a = GET_VAR_VAL(sizeof(OPCODE) + sizeof(Type1),Type1);\
						for(Type1 * EndElement = Indexes + READ_OPERAND(sizeof(OPCODE) + sizeof(Type1)*2,Type1);\
								Indexes < EndElement;Indexes++)\
								a = a[ConstVals + *Indexes];\
						GET_VAR_VAL(sizeof(OPCODE),Type1) = a;\
						ip = (LPOPCODE)Indexes; \
						continue;}
				case OPCODE::RIOBJ_vb_vb_indexes: RIOBJ_OPCODE_DEF(SHORT_NUMBER_VAR)
						//Read Index Global Object
					    #define RIGOBJ_OPCODE_DEF(Type1) {\
						LPINSIDE_DATA ConstVals = HeaderMethod->ConstObjs;\
						Type1 * Indexes = GET_OPERAND_POINTER(sizeof(OPCODE) + sizeof(Type1) * 2,Type1);\
						INLINE_INSIDE_DATA a = GlobalObject;\
						for(Type1 * EndElement = Indexes + READ_OPERAND(sizeof(OPCODE) + sizeof(Type1) ,Type1);\
						Indexes < EndElement;Indexes++)\
								a = a[ConstVals + *Indexes];\
						GET_VAR_VAL(sizeof(OPCODE),Type1) = a;\
						ip = (LPOPCODE)Indexes; \
						continue;}
				case OPCODE::RIGOBJ_vb_indexes: RIGOBJ_OPCODE_DEF(SHORT_NUMBER_VAR)
					    #define GETGLOB_OPCODE_DEF(Type1) {\
						GET_VAR_VAL(sizeof(OPCODE), Type1) = GlobalObject;\
						ip += (sizeof(OPCODE) + sizeof(Type1));\
						continue;}
				case OPCODE::GETGLOB_vb: GETGLOB_OPCODE_DEF(SHORT_NUMBER_VAR)
					    #define WIOBJ_OPCODE_DEF(Type1) {\
						LPINSIDE_DATA ConstVals = HeaderMethod->ConstObjs;\
						Type1 * Indexes = GET_OPERAND_POINTER(sizeof(OPCODE) + sizeof(Type1) * 3,Type1);\
						INLINE_INSIDE_DATA a = GET_VAR_VAL(sizeof(OPCODE) + sizeof(Type1),Type1);\
						for(Type1 * EndElement = Indexes + (READ_OPERAND(sizeof(OPCODE) + sizeof(Type1)*2 ,Type1) - 1);\
						Indexes < EndElement;Indexes++)\
								a = a[ConstVals + *Indexes];\
						a.Object.WriteMember(ConstVals + *Indexes, GET_VAR(sizeof(OPCODE), Type1));\
						ip = (LPOPCODE)(Indexes + 1); \
						continue;}
				case OPCODE::WIOBJ_vb_vb_indexes: WIOBJ_OPCODE_DEF(SHORT_NUMBER_VAR)
						#define WIGOBJ_OPCODE_DEF(Type1) {\
						LPINSIDE_DATA ConstVals = HeaderMethod->ConstObjs;\
						Type1 * Indexes = GET_OPERAND_POINTER(sizeof(OPCODE) + sizeof(Type1) * 2,Type1);\
						INLINE_INSIDE_DATA a = GlobalObject;\
						for(Type1 * EndElement = Indexes + (READ_OPERAND(sizeof(OPCODE) + sizeof(Type1),Type1) - 1);\
						Indexes < EndElement;Indexes++)\
								a = a[ConstVals + *Indexes];\
						a.Object.WriteMember(ConstVals + *Indexes, GET_VAR(sizeof(OPCODE), Type1));\
						ip = (LPOPCODE)(Indexes + 1); \
						continue;}
				case OPCODE::WIGOBJ_vb_indexes: WIGOBJ_OPCODE_DEF(SHORT_NUMBER_VAR)
				case OPCODE::IF_vw_raw:			IF_OPCODE_DEF(NUMBER_VAR , REL_ADDRESS) 
				case OPCODE::IFN_vw_raw:		IFN_OPCODE_DEF(NUMBER_VAR, REL_ADDRESS)
				case OPCODE::IFEQ_vw_vw_raw:	IFEQ_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR, REL_ADDRESS)
				case OPCODE::IFNEQ_vw_vw_raw:	IFNEQ_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR, REL_ADDRESS)
				case OPCODE::IFLT_vw_vw_raw:	IFLT_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR, REL_ADDRESS)
			    case OPCODE::IFLE_vw_vw_raw:	IFLE_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR, REL_ADDRESS)
				case OPCODE::MOV_vw_vw:			MOV_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR)
			    case OPCODE::CLONE_vw_vw:		CLONE_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR)
				case OPCODE::RETURN_vw:			RETURN_OPCODE_DEF(NUMBER_VAR)
				case OPCODE::READARG_vw_vw:		READ_ARG_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR)
				case OPCODE::WRITEARG_vw_vw:	WRITE_ARG_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR)
			    case OPCODE::NEG_vw_vw:			NEG_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR)
				case OPCODE::NOT_vw_vw:			NOT_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR)
				case OPCODE::AND_vw_vw_vw:		MATH_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR, NUMBER_VAR, &)
				case OPCODE::OR_vw_vw_vw:		MATH_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR, NUMBER_VAR,|)
				case OPCODE::XOR_vw_vw_vw:		MATH_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR, NUMBER_VAR,^)
				case OPCODE::SHR_vw_vw_vw:		MATH_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR, NUMBER_VAR, >>)
				case OPCODE::SHL_vw_vw_vw:		MATH_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR, NUMBER_VAR, <<)
				case OPCODE::ADD_vw_vw_vw:		MATH_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR, NUMBER_VAR, +)
				case OPCODE::SUB_vw_vw_vw:		MATH_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR, NUMBER_VAR, -)
				case OPCODE::MUL_vw_vw_vw:		MATH_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR, NUMBER_VAR,*)
				case OPCODE::DIV_vw_vw_vw:		MATH_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR, NUMBER_VAR,/)
				case OPCODE::MOD_vw_vw_vw:		MATH_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR, NUMBER_VAR, %)
				case OPCODE::EQ_vw_vw_vw:		MATH_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR, NUMBER_VAR, ==)
				case OPCODE::NEQ_vw_vw_vw:		MATH_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR, NUMBER_VAR, !=)
				case OPCODE::LT_vw_vw_vw:		MATH_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR, NUMBER_VAR, <)
				case OPCODE::LE_vw_vw_vw:		MATH_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR, NUMBER_VAR, <=)
				case OPCODE::INC_vw:			INC_OPCODE_DEF(NUMBER_VAR)
				case OPCODE::DEC_vw:			DEC_OPCODE_DEF(NUMBER_VAR)
				case OPCODE::GOTO_raw:			GOTO_OPCODE_DEF(REL_ADDRESS)
				case OPCODE::SETEXEPT_raw_vw:	SETEXEPT_OPCODE_DEF(REL_ADDRESS, NUMBER_VAR)
				case OPCODE::UNEXCEPT_raw:		UNEXCEPT_OPCODE_DEF(REL_ADDRESS)
				case OPCODE::THROW_vw:			THROW_OPCODE_DEF(NUMBER_VAR)
				case OPCODE::CALL_vw_w:			CALL_OPCODE_DEF(NUMBER_VAR)
				case OPCODE::CALL_vw_vw_w:		CALL_WITH_RET_OPCODE_DEF(SHORT_NUMBER_VAR,NUMBER_VAR)
				case OPCODE::VIRCALL_vw_vw_w:	VIRCALL_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR)
				case OPCODE::VIRCALL_vw_vw_vw_w:VIRCALL_WITH_RET_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR, NUMBER_VAR)
				case OPCODE::NEWOBJ_vw_vw_w:	NEWOBJ_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR)
				case OPCODE::RBI_vw_vw_vw:		RBI_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR, NUMBER_VAR)
				case OPCODE::WBI_vw_vw_vw:		RBW_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR, NUMBER_VAR)
				case OPCODE::DEL_vw_vw:			DEL_OPCODE_DEF(NUMBER_VAR)
				case OPCODE::ARGC_vw:			ARGC_OPCODE_DEF(NUMBER_VAR)
				case OPCODE::LEN_vw_vw:			LEN_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR)
				case OPCODE::TYPEOF_vw_vw:		TYPEOF_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR)
				case OPCODE::NXTI_vw_vb:		NXTI_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR)
				case OPCODE::SETCONST_vw_index: SETVAL_OPCODE_DEF(NUMBER_VAR, NUMBER_VAR)
				case OPCODE::GETTHIS_vw:		GETTHIS_OPCODE_DEF(NUMBER_VAR)
				case OPCODE::RIOBJ_vw_vw_indexes: RIOBJ_OPCODE_DEF(NUMBER_VAR)
				case OPCODE::RIGOBJ_vw_indexes: RIGOBJ_OPCODE_DEF(NUMBER_VAR)
				case OPCODE::GETGLOB_vw:		GETGLOB_OPCODE_DEF(NUMBER_VAR)
				case OPCODE::WIOBJ_vw_vw_indexes: WIOBJ_OPCODE_DEF(NUMBER_VAR)
				case OPCODE::WIGOBJ_vw_indexes: WIGOBJ_OPCODE_DEF(NUMBER_VAR)
			}/*End wile and switch*/
		}catch(LPINSIDE_DATA CurException)			   //Handled non native exception
		{
				if (LastExeptionVar == ZELLI_EXCEPT_HANDLER::NONE_VARIABLE)
					throw CurException;
				ThisFrame = &Frame;

				LPINSIDE_DATA ExceptVar = Frame.StartVar + LastExeptionVar;
				if (!ExceptVar->IsExceptionHandler)
				{
					//Если переменная не содержит тип перехвадчика исключения, 
					//то говорим что в данном фрейме нет корректных перехвадчиков,
					//а так же меняем тип исключения на некорректную переменную.
					THROW_NATIVE_EXCEPT("Local var is not have exception handler value.", HEADER_EXCEPTION::VARIABLE_IS_NOT_EXCEPTION_HANDLER);
				}
				LastExeptionVar = ExceptVar->ExcHand.NextHandler;
				ip = ExceptVar->ExcHand.CatchAddress;
				*ExceptVar = CurException;

		}catch(const LPHEADER_EXCEPTION Exeption)		//Handled native exception
		{
			if (LastExeptionVar == ZELLI_EXCEPT_HANDLER::NONE_VARIABLE)
				throw Exeption;
			ThisFrame = &Frame;
			
			LPINSIDE_DATA ExceptVar = Frame.StartVar + LastExeptionVar;
			if (!ExceptVar->IsExceptionHandler)
			{
				//Если переменная не содержит тип перехвадчика исключения, 
				//то говорим что в данном фрейме нет корректных перехвадчиков,
				//а так же меняем тип исключения на некорректную переменную.
				THROW_NATIVE_EXCEPT("Local var is not have exception handler value.", HEADER_EXCEPTION::VARIABLE_IS_NOT_EXCEPTION_HANDLER);
			}
			LastExeptionVar = ExceptVar->ExcHand.NextHandler;
			ip = ExceptVar->ExcHand.CatchAddress;
			//
			*ExceptVar = Exeption;
		}catch(const LPUNHANDLED_EXCEPTION Data)						//Unhandled notable exception
		{
			ThisFrame = &Frame;
			throw Data;
		}catch(...)									    //Unhandled unexpected exception
		{
		    ThisFrame = &Frame;
			throw "Unicknown error";
		}
	}
}


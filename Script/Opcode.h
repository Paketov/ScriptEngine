#ifndef _ZELLIOPCODE_H_
#define _ZELLIOPCODE_H_

struct COMMAND_PROPERTIES;

typedef COMMAND_PROPERTIES *LPCOMMAND_PROPERTIES;

class OPCODE
{
public:
	unsigned char Op;

	enum EN
	{
		//No OPeration
		NOP,

		// if(a) goto b
		IF_vb_rab,
		IF_vw_raw,

		// if(!a) goto b
		IFN_vb_rab,
		IFN_vw_raw,

		// if(a == b) goto c
		IFEQ_vb_vb_rab,
		IFEQ_vw_vw_raw,

		// if(a != b) goto c
		IFNEQ_vb_vb_rab,
		IFNEQ_vw_vw_raw,

		// if(a < b) goto c 
		IFLT_vb_vb_rab,
		IFLT_vw_vw_raw,

		// if(a <= b) goto c 
		IFLE_vb_vb_rab,
		IFLE_vw_vw_raw,

		// a = ((b == obj) | (b == str))? ref b: b
		MOV_vb_vb,
		MOV_vw_vw,

		// a = b
		CLONE_vb_vb,
		CLONE_vw_vw,

		// return
		RETURN,

		// return a
		RETURN_vb,
		RETURN_vw,

		// func(arg1, arg2,...argN){ a = arg1;...
		READARG_vb_vb,
		READARG_vw_vw,

		// func(ref arg1, arg2,...argN){ arg1 = b;...
		WRITEARG_vb_vb,
		WRITEARG_vw_vw,

		// a = -b
		NEG_vb_vb,
		NEG_vw_vw,

		// a = ~b
		NOT_vb_vb,
		NOT_vw_vw,

		// a = b & c
		AND_vb_vb_vb,
		AND_vw_vw_vw,

		// a = b | c
		OR_vb_vb_vb,
		OR_vw_vw_vw,

		// a = b ^ c
		XOR_vb_vb_vb,
		XOR_vw_vw_vw,

		// a = b >> c
		SHR_vb_vb_vb,
		SHR_vw_vw_vw,

		// a = b << c
		SHL_vb_vb_vb,
		SHL_vw_vw_vw,

		// a = b + c
		ADD_vb_vb_vb,
		ADD_vw_vw_vw,

		// a = b - c
		SUB_vb_vb_vb,
		SUB_vw_vw_vw,

		// a = b * c
		MUL_vb_vb_vb,
		MUL_vw_vw_vw,

		// a = b / c
		DIV_vb_vb_vb,
		DIV_vw_vw_vw,

		// a = b % c
		MOD_vb_vb_vb,
		MOD_vw_vw_vw,

		// a = b == c
		EQ_vb_vb_vb,
		EQ_vw_vw_vw,

		// a = b != c
		NEQ_vb_vb_vb,
		NEQ_vw_vw_vw,

		// a = b < c
		LT_vb_vb_vb,
		LT_vw_vw_vw,

		// a = b <= c
		LE_vb_vb_vb,
		LE_vw_vw_vw,

		// a++
		INC_vb,
		INC_vw,

		// a--
		DEC_vb,
		DEC_vw,

		// goto a
		GOTO_rab,
		GOTO_raw,

		// try{
		SETEXEPT_rab_vb,
		SETEXEPT_raw_vw,

		// }catch()
		UNEXCEPT_rab,
		UNEXCEPT_raw,

		// throw a
		THROW_vb,
		THROW_vw,

		// a(c , d, ..., b) 
		CALL_vb_w,
		CALL_vw_w,

		// a = b(d , e, ..., c) 
		CALL_vb_vb_w,
		CALL_vw_vw_w,

		// b.a(d, e, ..., c) 
		VIRCALL_vb_vb_w,
		VIRCALL_vw_vw_w,

		// a = b.c(e , f, ..., d) 
		VIRCALL_vb_vb_vb_w,
		VIRCALL_vw_vw_vw_w,

		// a = new b(d,e, ... to c)
		NEWOBJ_vb_vb_w,
		NEWOBJ_vw_vw_w,

		// a = b[c]
		//Read By Index
		RBI_vb_vb_vb,
		RBI_vw_vw_vw,

		// a[b] = c
		//Write By Index
		WBI_vb_vb_vb,
		WBI_vw_vw_vw,


		// delete a.b.c...n
		DEL_vb_vb,
		DEL_vw_vw,

		// a = <Count arguments>
		ARGC_vb,
		ARGC_vw,

		// a = b.length
		LEN_vb_vb,
		LEN_vw_vw,

		// a = typeof b
		TYPEOF_vb_vb,
		TYPEOF_vw_vw,

		// a = a in b
		NXTI_vb_vb,
		NXTI_vw_vb,

		// b = {...}
		SETCONST_vb_index,
		SETCONST_vw_index,

		// b = this
		GETTHIS_vb,
		GETTHIS_vw,

		//c = b.{interator}
		RIOBJ_vb_vb_indexes,
		RIOBJ_vw_vw_indexes,

		//c = glob_space.{interator}
		RIGOBJ_vb_indexes,
		RIGOBJ_vw_indexes,

		GETGLOB_vb,
		GETGLOB_vw,

		//c = b.{interator}
		WIOBJ_vb_vb_indexes,
		WIOBJ_vw_vw_indexes,

		//c = glob_space.{interator}
		WIGOBJ_vb_indexes,
		WIGOBJ_vw_indexes,

		SWITCH_vb_rab,
		SWITCH_vw_raw


	};

	inline EN & operator=(EN  & other)
	{
		Op = other;
		return other;
	}

	inline OPCODE & operator=(OPCODE  & other)
	{
		Op = other.Op;
		return other;
	}
	inline operator EN () const {return EN(Op);}
};


enum TYPE_OPERAND{
	//Нет никакого опреанда
	NOTHING = 0,
	//Опренды переменные
	VAR_BYTE,
	VAR_WORD,
	//Операнды относительные адреса
	RELADDRESS_BYTE,
	RELADDRESS_WORD,
	//Операнды массива или объекта с указанным индексом в качестве переменной
	COUNT_ARG_BYTE,
	COUNT_ARG_WORD,
	//Операнды номеров констант
	CONSTANT_INDEX,
	INTERATOR,

#ifdef DEF_OPERAND_DWORD
	OPERAND_VAR_DWORD,
	OPERAND_RELADDRESS_DWORD,
	OPERAND_ARRAY_DWORD,
	OPERAND_CONSTANT_DWORD,
#endif
	VARYING_COUNT_OPERAND=0xFF
};

typedef struct COMMAND_PROPERTIES
{
	char *        NameOpcode;
	OPCODE        Op; 
	unsigned char CountOperand;
	unsigned char OpType[4];
} COMMAND_PROPERTIES, *LPCOMMAND_PROPERTIES;

#ifndef __OPCODE_CPP__
extern COMMAND_PROPERTIES NameOpcode[];
extern unsigned CountOpcodes;
#endif

#endif
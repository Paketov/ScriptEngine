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
		IF_i8_r16,
		IF_i16_r16,
		IF_i32_r16,
		IF_i64_r16,
		IF_f32_r16,
		IF_f64_r16,

		// if(!a) goto b
		NIF_i8_r16,
		NIF_i16_r16,
		NIF_i32_r16,
		NIF_i64_r16,
		NIF_f32_r16,
		NIF_f64_r16,

		//MOV a, b
		MOV_i8_i8,
		MOV_i8_i16,
		MOV_i8_i32,
		MOV_i8_i64,
		MOV_i8_f32,
		MOV_i8_f64,

		MOV_i16_i8,
		MOV_i16_i16,
		MOV_i16_i32,
		MOV_i16_i64,
		MOV_i16_f32,
		MOV_i16_f64,

		MOV_i32_i8,
		MOV_i32_i16,
		MOV_i32_i32,
		MOV_i32_i64,
		MOV_i32_f32,
		MOV_i32_f64,
		
		MOV_i64_i8,
		MOV_i64_i16,
		MOV_i64_i32,
		MOV_i64_i64,
		MOV_i64_f32,
		MOV_i64_f64,

		MOV_f32_i8,
		MOV_f32_i16,
		MOV_f32_i32,
		MOV_f32_i64,
		MOV_f32_f32,
		MOV_f32_f64,
		
		MOV_f64_i8,
		MOV_f64_i16,
		MOV_f64_i32,
		MOV_f64_i64,
		MOV_f64_f32,
		MOV_f64_f64,

		MOV_f64_i8,
		MOV_f64_i16,
		MOV_f64_i32,
		MOV_f64_i64,
		MOV_f64_f32,
		MOV_f64_f64,

		MOV_o_o,

		SETI_i8,
		SETI_i32,
		SETI_i64,
		SETF_f32,
		SETF_f64,
		SETS_o,

		// return a
		RETURN_v,
		RETURN_i8,
		RETURN_i16,
		RETURN_i32,
		RETURN_i64,
		RETURN_f32,
		RETURN_f64,

		// func(arg1, arg2,...argN){ a = arg1;...
		READARG_vb_vb,
		READARG_vw_vw,

		// func(ref arg1, arg2,...argN){ arg1 = b;...
		WRITEARG_vb_vb,
		WRITEARG_vw_vw,

		// a = -b
		NEG_i8_i8,
		NEG_i16_i16,
		NEG_i32_i32,
		NEG_i64_i64,
		NEG_f32_f32,
		NEG_f64_f64,

		// a = ~b
		NOT_i8_i8,
		NOT_i16_i16,
		NOT_i32_i32,
		NOT_i64_i64,

		// a = b & c
		AND_i8_i8_i8,
		AND_i16_i16_i16,
		AND_i32_i32_i32,
		AND_i64_i64_i64,

		// a = b | c
		OR_i8_i8_i8,
		OR_i16_i16_i16,
		OR_i32_i32_i32,
		OR_i64_i64_i64,

		// a = b ^ c
		XOR_i8_i8_i8,
		XOR_i16_i16_i16,
		XOR_i32_i32_i32,
		XOR_i64_i64_i64,

		// a = b >> c
		SHR_i8_i8_i8,
		SHR_i16_i16_i16,
		SHR_i32_i32_i32,
		SHR_i64_i64_i64,

		// a = b << c
		SHL_i8_i8_i8,
		SHL_i16_i16_i16,
		SHL_i32_i32_i32,
		SHL_i64_i64_i64,

		// a = b + c
		ADD_i8_i8_i8,
		ADD_i16_i16_i16,
		ADD_i32_i32_i32,
		ADD_i64_i64_i64,
		ADD_f32_f32_f32,
		ADD_f64_f64_f64,

		// a = b - c
		SUB_i8_i8_i8,
		SUB_i16_i16_i16,
		SUB_i32_i32_i32,
		SUB_i64_i64_i64,
		SUB_f32_f32_f32,
		SUB_f64_f64_f64,

		// a = b * c
		MUL_i8_i8_i8,
		MUL_i16_i16_i16,
		MUL_i32_i32_i32,
		MUL_i64_i64_i64,
		MUL_f32_f32_f32,
		MUL_f64_f64_f64,

		// a = b / c
		DIV_i8_i8_i8,
		DIV_i16_i16_i16,
		DIV_i32_i32_i32,
		DIV_i64_i64_i64,
		DIV_f32_f32_f32,
		DIV_f64_f64_f64,

		// a = b % c
		MOD_i8_i8_i8,
		MOD_i16_i16_i16,
		MOD_i32_i32_i32,
		MOD_i64_i64_i64,
		MOD_f32_f32_f32,
		MOD_f64_f64_f64,

		// a = b == c
		EQ_i8_i8_i8,
		EQ_i8_i16_i16,
		EQ_i8_i32_i32,
		EQ_i8_i64_i64,
		EQ_i8_f32_f32,
		EQ_i8_f64_f64,

		// a = b != c
		NEQ_i8_i8_i8,
		NEQ_i8_i16_i16,
		NEQ_i8_i32_i32,
		NEQ_i8_i64_i64,
		NEQ_i8_f32_f32,
		NEQ_i8_f64_f64,

		// a = b < c
		LT_i8_i8_i8,
		LT_i8_i16_i16,
		LT_i8_i32_i32,
		LT_i8_i64_i64,
		LT_i8_f32_f32,
		LT_i8_f64_f64,

		// a = b <= c
		LE_i8_i8_i8,
		LE_i8_i16_i16,
		LE_i8_i32_i32,
		LE_i8_i64_i64,
		LE_i8_f32_f32,
		LE_i8_f64_f64,

		// goto a
		GOTO_p,

		// try{
		SETEXEPT_p,

		// }catch()
		UNEXCEPT_p,

		// throw a
		THROW_i8,
		THROW_i16,
	    THROW_i32,
		THROW_i64,
		THROW_f32,
		THROW_f64,
		THROW_o,

		// a(c , d, ..., b) 
		CALL_p_i16,
		CALL_i8_p_i16,
		CALL_i16_p_i16,
	    CALL_i32_p_i16,
		CALL_i64_p_i16,
		CALL_f32_p_i16,
		CALL_f64_p_i16,
		CALL_o_p_i16,

		CALLV_p_i16,
		CALLV_i8_p_i16,
		CALLV_i16_p_i16,
	    CALLV_i32_p_i16,
		CALLV_i64_p_i16,
		CALLV_f32_p_i16,
		CALLV_f64_p_i16,
		CALLV_o_p_i16,

		// a = new b(d,e, ... to c)
		NEWLOCAL_o_po_i16,
		NEWGLOBAL_o_po_i16,

		// delete a.b.c...n
		DEL_vb_vb,
		DEL_vw_vw,

		// b = this
		GETTHIS_vb,
		GETTHIS_vw,

		GETGLOB_vb,
		GETGLOB_vw,

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
	inline operator EN () const {return (EN)Op;}
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
	CONSTANT_INTEGER,
	CONSTANT_STRING,
	CONSTANT_METHOD,
	CONSTANT_DOUBLE,
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
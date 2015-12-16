#define __OPCODE_CPP__
#include "Opcode.h"


extern COMMAND_PROPERTIES NameOpcode[] =
{
	{"NOP", OPCODE::NOP,0},

	// if(a) goto b
	{"IF", OPCODE::IF_vb_rab,2, VAR_BYTE, RELADDRESS_BYTE},
	{"IF", OPCODE::IF_vw_raw,2, VAR_WORD, RELADDRESS_WORD},

	// if(!a) goto b
	{"IFN", OPCODE::IFN_vb_rab,2, VAR_BYTE, RELADDRESS_BYTE},
	{"IFN", OPCODE::IFN_vw_raw,2, VAR_WORD, RELADDRESS_WORD},

	// if(a == b) goto c
	{"IFEQ", OPCODE::IFEQ_vb_vb_rab, 3, VAR_BYTE, VAR_BYTE, RELADDRESS_BYTE},
	{"IFEQ", OPCODE::IFEQ_vw_vw_raw, 3, VAR_WORD, VAR_WORD, RELADDRESS_WORD},

	// if(a != b) goto c
	{"IFNEQ", OPCODE::IFNEQ_vb_vb_rab, 3, VAR_BYTE, VAR_BYTE, RELADDRESS_BYTE},
	{"IFNEQ", OPCODE::IFNEQ_vw_vw_raw, 3, VAR_WORD, VAR_WORD, RELADDRESS_WORD},

	// if(a < b) goto c 
	{"IFLT", OPCODE::IFLT_vb_vb_rab, 3, VAR_BYTE, VAR_BYTE, RELADDRESS_BYTE},
	{"IFLT", OPCODE::IFLT_vw_vw_raw, 3, VAR_WORD, VAR_WORD, RELADDRESS_WORD},

	// if(a <= b) goto c 
	{"IFLE", OPCODE::IFLE_vb_vb_rab, 3, VAR_BYTE, VAR_BYTE, RELADDRESS_BYTE},
	{"IFLE", OPCODE::IFLE_vw_vw_raw, 3, VAR_WORD, VAR_WORD, RELADDRESS_WORD},

	// a = ((b == obj) | (b == str))? ref b: b
	{"MOV", OPCODE::MOV_vb_vb, 2, VAR_BYTE, VAR_BYTE},
	{"MOV", OPCODE::MOV_vw_vw, 2, VAR_WORD, VAR_WORD},

	// a = b
	{"CLONE", OPCODE::CLONE_vb_vb, 2, VAR_BYTE, VAR_BYTE},
	{"CLONE", OPCODE::CLONE_vw_vw, 2, VAR_WORD, VAR_WORD},

	// return
	{"RETURN", OPCODE::RETURN, 0},

	// return a
	{"RETURN", OPCODE::RETURN_vb, 1, VAR_BYTE},
	{"RETURN", OPCODE::RETURN_vw, 1, VAR_WORD},

	// func(arg1, arg2,...argN){ a = arg1;...
	{"READARG", OPCODE::READARG_vb_vb, 2, VAR_BYTE, VAR_BYTE},
	{"READARG", OPCODE::READARG_vw_vw, 2, VAR_WORD, VAR_WORD},

	// func(ref arg1, arg2,...argN){ arg1 = b;...
	{"WRITEARG", OPCODE::WRITEARG_vb_vb, 2, VAR_BYTE, VAR_BYTE},
	{"WRITEARG", OPCODE::WRITEARG_vw_vw, 2, VAR_WORD, VAR_WORD},

	// a = -b
	{"NEG", OPCODE::NEG_vb_vb, 2, VAR_BYTE, VAR_BYTE},
	{"NEG", OPCODE::NEG_vw_vw, 2, VAR_WORD, VAR_WORD},

	// a = ~b
	{"NOT", OPCODE::NOT_vb_vb, 2, VAR_BYTE, VAR_BYTE},
	{"NOT", OPCODE::NOT_vw_vw, 2, VAR_WORD, VAR_WORD},

	// a = b & c
	{"AND", OPCODE::AND_vb_vb_vb, 3, VAR_BYTE, VAR_BYTE, VAR_BYTE},
	{"AND", OPCODE::AND_vw_vw_vw, 3, VAR_WORD, VAR_WORD, VAR_WORD},

	// a = b | c
	{"OR", OPCODE::OR_vb_vb_vb, 3, VAR_BYTE, VAR_BYTE, VAR_BYTE},
	{"OR", OPCODE::OR_vw_vw_vw, 3, VAR_WORD, VAR_WORD, VAR_WORD},

	// a = b ^ c
	{"XOR", OPCODE::XOR_vb_vb_vb, 3, VAR_BYTE, VAR_BYTE, VAR_BYTE},
	{"XOR", OPCODE::XOR_vw_vw_vw, 3, VAR_WORD, VAR_WORD, VAR_WORD},

	// a = b >> c
	{"SHR", OPCODE::SHR_vb_vb_vb, 3, VAR_BYTE, VAR_BYTE, VAR_BYTE},
	{"SHR", OPCODE::SHR_vw_vw_vw, 3, VAR_WORD, VAR_WORD, VAR_WORD},

	// a = b << c
	{"SHL", OPCODE::SHL_vb_vb_vb, 3, VAR_BYTE, VAR_BYTE, VAR_BYTE},
	{"SHL", OPCODE::SHL_vw_vw_vw, 3, VAR_WORD, VAR_WORD, VAR_WORD},

	// a = b + c
	{"ADD", OPCODE::ADD_vb_vb_vb, 3, VAR_BYTE, VAR_BYTE, VAR_BYTE},
	{"ADD", OPCODE::ADD_vw_vw_vw, 3, VAR_WORD, VAR_WORD, VAR_WORD},

	// a = b - c
	{"SUB", OPCODE::SUB_vb_vb_vb, 3, VAR_BYTE, VAR_BYTE, VAR_BYTE},
	{"SUB", OPCODE::SUB_vw_vw_vw, 3, VAR_WORD, VAR_WORD, VAR_WORD},

	// a = b * c
	{"MUL", OPCODE::MUL_vb_vb_vb, 3, VAR_BYTE, VAR_BYTE, VAR_BYTE},
	{"MUL", OPCODE::MUL_vw_vw_vw, 3, VAR_WORD, VAR_WORD, VAR_WORD},

	// a = b / c
	{"DIV", OPCODE::DIV_vb_vb_vb, 3, VAR_BYTE, VAR_BYTE, VAR_BYTE},
	{"DIV", OPCODE::DIV_vw_vw_vw, 3, VAR_WORD, VAR_WORD, VAR_WORD},

	// a = b % c
	{"MOD", OPCODE::MOD_vb_vb_vb, 3, VAR_BYTE, VAR_BYTE, VAR_BYTE},
	{"MOD", OPCODE::MOD_vw_vw_vw, 3, VAR_WORD, VAR_WORD, VAR_WORD},

	// a = b == c
	{"EQ", OPCODE::EQ_vb_vb_vb, 3, VAR_BYTE, VAR_BYTE, VAR_BYTE},
	{"EQ", OPCODE::EQ_vw_vw_vw, 3, VAR_WORD, VAR_WORD, VAR_WORD},

	// a = b != c
	{"NEQ", OPCODE::NEQ_vb_vb_vb, 3, VAR_BYTE, VAR_BYTE, VAR_BYTE},
	{"NEQ", OPCODE::NEQ_vw_vw_vw, 3, VAR_WORD, VAR_WORD, VAR_WORD},

	// a = b < c
	{"LT", OPCODE::LT_vb_vb_vb, 3, VAR_BYTE, VAR_BYTE, VAR_BYTE},
	{"LT", OPCODE::LT_vw_vw_vw, 3, VAR_WORD, VAR_WORD, VAR_WORD},

	// a = b <= c
	{"LE", OPCODE::LE_vb_vb_vb, 3, VAR_BYTE, VAR_BYTE, VAR_BYTE},
	{"LE", OPCODE::LE_vw_vw_vw, 3, VAR_WORD, VAR_WORD, VAR_WORD},

	// a++
	{"INC", OPCODE::INC_vb, 1, VAR_BYTE},
	{"INC", OPCODE::INC_vw, 1, VAR_WORD},

	// a--
	{"DEC", OPCODE::DEC_vb, 1, VAR_BYTE},
	{"DEC", OPCODE::DEC_vw, 1, VAR_WORD},

	// goto a
	{"GOTO", OPCODE::GOTO_rab, 1, RELADDRESS_BYTE},
	{"GOTO", OPCODE::GOTO_raw, 1, RELADDRESS_WORD},

	// try{
	{"SETEXEPT", OPCODE::SETEXEPT_rab_vb, 2, RELADDRESS_BYTE, VAR_BYTE},
	{"SETEXEPT", OPCODE::SETEXEPT_raw_vw, 2, RELADDRESS_WORD, VAR_WORD},

	// }catch()
	{"UNEXCEPT", OPCODE::UNEXCEPT_rab, 1, RELADDRESS_BYTE},
	{"UNEXCEPT", OPCODE::UNEXCEPT_raw, 1, RELADDRESS_WORD},

	// throw a
	{"THROW", OPCODE::THROW_vb, 1, VAR_BYTE},
	{"THROW", OPCODE::THROW_vw, 1, VAR_WORD},

	// a(c , d, ..., b) 
	{"CALL", OPCODE::CALL_vb_w, 2, VAR_BYTE, COUNT_ARG_BYTE},
	{"CALL", OPCODE::CALL_vw_w, 2, VAR_WORD, COUNT_ARG_WORD},

	// a = b(d , e, ..., c)
	{"CALL", OPCODE::CALL_vb_vb_w, 3, VAR_BYTE, VAR_BYTE, COUNT_ARG_BYTE},
	{"CALL", OPCODE::CALL_vw_vw_w, 3, VAR_WORD, VAR_WORD, COUNT_ARG_WORD},

	// b.a(d, e, ..., c) 
	{"VIRCALL", OPCODE::VIRCALL_vb_vb_w, 3, VAR_BYTE, VAR_BYTE, COUNT_ARG_BYTE},
	{"VIRCALL", OPCODE::VIRCALL_vw_vw_w, 3, VAR_WORD, VAR_WORD, COUNT_ARG_WORD},

	// a = b.c(e , f, ..., d) 
	{"VIRCALL", OPCODE::VIRCALL_vb_vb_vb_w, 4, VAR_BYTE, VAR_BYTE, VAR_BYTE, COUNT_ARG_BYTE},
	{"VIRCALL", OPCODE::VIRCALL_vw_vw_vw_w, 4, VAR_WORD, VAR_WORD, VAR_WORD, COUNT_ARG_WORD},

	// a = new b(d,e, ... to c)
	{"NEWOBJ", OPCODE::NEWOBJ_vb_vb_w, 3, VAR_BYTE, VAR_BYTE, COUNT_ARG_BYTE},
	{"NEWOBJ", OPCODE::NEWOBJ_vw_vw_w, 3, VAR_WORD, VAR_WORD, COUNT_ARG_WORD},

	// a = b[c]
	//Read By Index
	{"RBI", OPCODE::RBI_vb_vb_vb, 3, VAR_BYTE, VAR_BYTE, VAR_BYTE},
	{"RBI", OPCODE::RBI_vw_vw_vw, 3, VAR_WORD, VAR_WORD, VAR_WORD},

	// a[b] = c
	//Write By Index
	{"WBI", OPCODE::WBI_vb_vb_vb, 3, VAR_BYTE, VAR_BYTE, VAR_BYTE},
	{"WBI", OPCODE::WBI_vw_vw_vw, 3, VAR_WORD, VAR_WORD, VAR_WORD},

	// delete a.b.c...n
	{"DEL", OPCODE::DEL_vb_vb, 2, VAR_BYTE, VAR_BYTE},
	{"DEL", OPCODE::DEL_vw_vw, 2, VAR_WORD, VAR_WORD},

	// a = <Count arguments>
	{"ARGC", OPCODE::ARGC_vb, 1, VAR_BYTE},
	{"ARGC", OPCODE::ARGC_vb, 1, VAR_WORD},

	// a = b.length
	{"LEN", OPCODE::LEN_vb_vb, 2, VAR_BYTE, VAR_BYTE},
	{"LEN", OPCODE::LEN_vw_vw, 2, VAR_WORD, VAR_WORD},

	// a = typeof b
	{"TYPEOF", OPCODE::TYPEOF_vb_vb, 2, VAR_BYTE, VAR_BYTE},
	{"TYPEOF", OPCODE::TYPEOF_vw_vw, 2, VAR_WORD, VAR_WORD},

	// a = a in b
	{"NXTI", OPCODE::NXTI_vb_vb, 2, VAR_BYTE, VAR_BYTE},
	{"NXTI", OPCODE::NXTI_vw_vb, 2, VAR_WORD, VAR_WORD},

	//// b = function(){...}
	{"SETCONST", OPCODE::SETCONST_vb_index, 2, VAR_BYTE, CONSTANT_INDEX},
	{"SETCONST", OPCODE::SETCONST_vw_index, 2, VAR_WORD, CONSTANT_INDEX},

	// b = this
	{"GETTHIS", OPCODE::GETTHIS_vb, 1, VAR_BYTE},
	{"GETTHIS", OPCODE::GETTHIS_vw, 1, VAR_WORD},

	////c = b.{interator}
	//{"RIOBJ", OPCODE::RIOBJ_interator_vb_vb, 3, INTERATOR, VAR_BYTE, VAR_BYTE},
	//{"RIOBJ", OPCODE::RIOBJ_interator_vw_vw, 3, INTERATOR, VAR_WORD, VAR_WORD},

	//c = glob_space.{interator}
	{"GETGLOB", OPCODE::GETGLOB_vb, 1, VAR_BYTE},
	{"GETGLOB", OPCODE::GETGLOB_vw, 1, VAR_WORD},

	////c = b.{interator}
	//{"WIOBJ", OPCODE::WIOBJ_interator_vb_vb, 3, INTERATOR, VAR_BYTE, VAR_BYTE},
	//{"WIOBJ", OPCODE::WIOBJ_interator_vw_vw, 3, INTERATOR, VAR_WORD, VAR_WORD},

	////c = glob_space.{interator}
	//{"WIGOBJ", OPCODE::WIGOBJ_interator_vb, 2, INTERATOR, VAR_BYTE},
	//{"WIGOBJ", OPCODE::WIGOBJ_interator_vw, 2, INTERATOR, VAR_WORD}

};

unsigned CountOpcodes = sizeof(NameOpcode) / sizeof(NameOpcode[0]);
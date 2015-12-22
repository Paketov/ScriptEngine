#include <stdio.h>
#include <windows.h>
#include "Context.h"
#include "Object.h"
#include "TypeData.h"
#include "GlobalVar.h"
#include "Opcode.h"
#include "AdditionalLib.h"
#include "AcselerateInterator.h"
#include "String.h"
#include "ObjectClass.h"
#include "GlobalScope.h"
#include "LoadFromFile.h"
#include "Method.h"


INSIDE_DATA ZELLI_API PrintVal(LPINSIDE_DATA This, LPEXECUTE_CONTEXT CurContext, LPARG_FUNC Arg)
{
	INTERNAL_CHAR Buf[50];
	NUMBER_ARG CountArg = Arg->CountArg;
	for(NUMBER_ARG i = 0;i < CountArg;i++)
	{
		LPINSIDE_DATA Result = CurContext->GetArg(Arg->Args[i]);
		
		TSIZE_STR     Len = 50;
		LPINTERNAL_CHAR OutStr = CurContext->StringClass->ToString(Result,Buf,&Len);
		WriteConsoleA(GetStdHandle(STD_OUTPUT_HANDLE),OutStr,Len,NULL,NULL);
	}
	getchar();
	return INSIDE_DATA::Null;
}

INSIDE_DATA ZELLI_API ReadVal(LPINSIDE_DATA This, LPEXECUTE_CONTEXT CurContext, LPARG_FUNC Arg)
{
	INTERNAL_CHAR Buf[1000];
	DWORD CountReaded;
	ReadConsoleA(GetStdHandle(STD_INPUT_HANDLE),Buf,1000,&CountReaded,NULL);
	return CurContext->StringClass->RegisterString(Buf,(TSIZE_STR)(CountReaded - 2));
}


int main(int argc, char * argv[])
{
		printf("Size INSIDE_DATA: %d\n",sizeof(INSIDE_DATA));
		printf("Offset double: %d\n",OFFSET_FIELD(INSIDE_DATA,Double));
		printf("Offset integer: %d\n",OFFSET_FIELD(INSIDE_DATA,Integer));
		printf("Offset object: %d\n",OFFSET_FIELD(INSIDE_DATA,Object));
		printf("Offset ExcHand: %d\n",OFFSET_FIELD(INSIDE_DATA,ExcHand));
		printf("Offset TypeData: %d - %d\n", OFFSET_FIELD(INSIDE_DATA,TypeData), OFFSET_FIELD(INSIDE_DATA,TypeData) + sizeof(((LPINSIDE_DATA)0)->TypeData));
		printf("Offset class index: %d - %d\n",OFFSET_FIELD(INSIDE_DATA,Object.Prototype),OFFSET_FIELD(INSIDE_DATA,Object.Prototype) + sizeof(OBJECT::PROTOTYPE));

		//printf("Offset ShortStr: %d\n",OFFSET_FIELD(INSIDE_DATA,ShortStr));
		

		//////////////////
		EXECUTE_CONTEXT  l(1024,6000);
		



		INSIDE_DATA t;
		LPOBJECT_CLASS ObjClass = new OBJECT_CLASS(l.StringClass);
		l.ObjManager->ClassRegister(ObjClass);

		INSIDE_DATA NameWrite = OBJECT::New(l.StringClass, l.StringClass->RegisterString("write"));
		l.GlobalObject[NameWrite] = NameWrite;
		t = l.GlobalObject[NameWrite];
	


		t = OBJECT::New(l.StringClass, l.StringClass->RegisterString("write"));

		for(;;)
		{
		    t = t + t;
			l.SetAllInstanceToUnused();
			l.MarkAllInstance();
			t.MarkAsUsed();
			l.FreeAllUnusedInstance();
		}

		LPHEADER_STRING WriteGlobalFunc = l.StringClass->RegisterString("write");
		t = PrintVal;
		//l.GlobalObject.Object.WriteMember(WriteGlobalFunc,&t);

		WriteGlobalFunc = l.StringClass->RegisterString("read");
		t = ReadVal;
		//l.GlobalObject.Object.WriteMember(WriteGlobalFunc,&t);

		BYTECODE_FILE File(l.StringClass, &l.FunctionClass);

		if(BYTECODE_FILE::SUCCESS != File.LoadFile("C:\\Users\\andr\\Desktop\\ZelliBytecode\\test.bin"))
			return 0;
		(*File.MainMethod)(&l);

		char Buffer[5000] = "";

		l.GlobalObject.Object.Prototype->InfoClass(Buffer, sizeof(Buffer));

		l.StringClass->InfoClass(Buffer, sizeof(Buffer));
		//Garbage collector
		l.SetAllInstanceToUnused();
		l.MarkAllInstance();
		l.FreeAllUnusedInstance();
		return 0;
}



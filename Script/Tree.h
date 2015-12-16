#ifndef __TREE_H__
#define __TREE_H__

#ifndef NULL
#define NULL 0
#endif

#ifndef max
#define max(a,b) (((a)>(b))?(a):(b))
#endif

//�������� ������ ������
template <typename TypeData, typename TypeKey, typename TypeDeep = unsigned short>
class TREE
{
public:

	typedef struct TREE_ELEMENT: public TypeData
	{		
		TREE_ELEMENT *		L;						//����� �����
		TREE_ELEMENT *		R;						//������ �����
		TypeDeep			MaxDeep;
		TypeKey				Key;					//���� ������
	} TREE_ELEMENT, *LPTREE_ELEMENT;

private:
	//������� ����� � ����
	inline static void __fastcall TurnBranchL(LPTREE_ELEMENT * Dest)
	{
		register LPTREE_ELEMENT a = *Dest, b = a->R, c = b->R;
		if ((c != NULL) && ((c->MaxDeep + 1) == b->MaxDeep))
		{
			*Dest = b;
			a->R = b->L;
			b->L = a;
			a->MaxDeep = max(a->L ? a->L->MaxDeep : 0, a->R ? a->R->MaxDeep : 0) + 1;
			b->MaxDeep = max(a->MaxDeep, c->MaxDeep) + 1;
		}
		else
		{
			*Dest = c = b->L;
			b->L = c->R;
			c->R = b;
			a->R = c->L;
			c->L = a;
			a->MaxDeep = max(a->L ? a->L->MaxDeep : 0, a->R ? a->R->MaxDeep : 0) + 1;
			b->MaxDeep = max(b->L ? b->L->MaxDeep : 0, b->R ? b->R->MaxDeep : 0) + 1;
			c->MaxDeep = max(a->MaxDeep, b->MaxDeep) + 1;
		}
	}

	//������� ����� � �����
	inline static void __fastcall TurnBranchR(LPTREE_ELEMENT * Dest)
	{
		register LPTREE_ELEMENT a = *Dest, b = a->L, c = b->L;
		if ((c != NULL) && ((c->MaxDeep + 1) == b->MaxDeep))
		{
			*Dest = b;
			a->L = b->R;
			b->R = a;
			a->MaxDeep = max(a->L ? a->L->MaxDeep : 0, a->R ? a->R->MaxDeep : 0) + 1;
			b->MaxDeep = max(a->MaxDeep, c->MaxDeep) + 1;
		}
		else
		{
			*Dest = c = b->R;
			b->R = c->L;
			c->L = b;
			a->L = c->R;
			c->R = a;
			a->MaxDeep = max(a->L ? a->L->MaxDeep : 0, a->R ? a->R->MaxDeep : 0) + 1;
			b->MaxDeep = max(b->L ? b->L->MaxDeep : 0, b->R ? b->R->MaxDeep : 0) + 1;
			c->MaxDeep = max(a->MaxDeep, b->MaxDeep) + 1;
		}
	}

protected:
	LPTREE_ELEMENT TempElement;
	TypeKey	TempKey;

	//������� �� ��������� ���������� �������� �� ����� AddNode
	virtual TypeDeep AppendToBranchElement(LPTREE_ELEMENT * AddNode) = 0;

	virtual void SetExistMember(LPTREE_ELEMENT FindedNode)
	{
		TempElement = FindedNode;
	}

	virtual void DeleteFromBranchElement(LPTREE_ELEMENT DelNode) = 0;

	//����������� ���������� ���������
	TypeDeep RecursiveAppend(LPTREE_ELEMENT * AddNode)
	{
		//���� ������� ����� ��� ������ ���������� �������
		if (!(*AddNode))
		    AppendToBranchElement(AddNode);

		register TypeDeep CurDeep;
		if (TempKey < (*AddNode)->Key)
		{
			CurDeep = RecursiveAppend(&(*AddNode)->L);
			auto RationLR = ((*AddNode)->R ? (*AddNode)->R->MaxDeep : 0) - CurDeep;
			if (RationLR < 0)
			{
				(*AddNode)->MaxDeep = CurDeep + 1;

				if (RationLR <= -2)//Balance branch
					TurnBranchR(AddNode);
			}
		}else if (TempKey > (*AddNode)->Key)
		{
			CurDeep = RecursiveAppend(&(*AddNode)->R);
			auto RationLR = CurDeep - ((*AddNode)->L ? (*AddNode)->L->MaxDeep : 0);
			if (RationLR > 0)
			{
				(*AddNode)->MaxDeep = CurDeep + 1;
				if (RationLR >= 2)//Balance branch
					TurnBranchL(AddNode);
			}
		}else  //���� ����� ������� ����������, ���������� ��� ��������.
			SetExistMember(*AddNode);
		return (*AddNode)->MaxDeep;
	}

	//����������� �������� ��������
	TypeDeep RecursiveDel(LPTREE_ELEMENT * Node)
	{
		if (*Node == NULL)
			return 0;
		TypeDeep CurDeep;
		if (TempKey > (*Node)->Key)
		{
			CurDeep = RecursiveDel(&(*Node)->R);
			auto RatLR = CurDeep - ((*Node)->L ? (*Node)->L->MaxDeep : 0);
			if (RatLR <= 0)
			{
				(*Node)->MaxDeep = CurDeep - RatLR + 1;
				if (RatLR <= -2)
					TurnBranchR(Node);
			}
			else
				(*Node)->MaxDeep = CurDeep + 1;
		}
		else if (TempKey < (*Node)->Key)
		{
			CurDeep = RecursiveDel(&(*Node)->L);
			auto RatLR = ((*Node)->R ? (*Node)->R->MaxDeep : 0) - CurDeep;
			if (RatLR > 0)
			{
				(*Node)->MaxDeep = CurDeep + RatLR + 1;
				if (RatLR >= 2)
					TurnBranchL(Node);
			}else
				(*Node)->MaxDeep = CurDeep + 1;
		}
		else
		{
			if ((*Node)->MaxDeep == 1)//���� ������� �������� ������
			{
					DeleteFromBranchElement(TempElement = *Node);
					*Node = NULL;
					return 0;
			}
			if ((*Node)->L && (*Node)->R) //���� ������� ��� ����������
			{
				if ((*Node)->L->MaxDeep > (*Node)->R->MaxDeep)
				{
					TempKey = (TempElement = (*Node)->L)->Key;
					RecursiveAppend(&(*Node)->R); //��������� � ������������� � ������ �����
					TempElement = (*Node)->R;
				}else
				{
					TempKey = (TempElement = (*Node)->R)->Key;
					RecursiveAppend(&(*Node)->L); //��������� � ������������� � ����� �����
					TempElement = (*Node)->L;
				}
			}else 
				TempElement = (*Node)->L ? (*Node)->L: (*Node)->R;

			DeleteFromBranchElement(*Node);		//���� ������� ��� �����
			return (*Node = TempElement)->MaxDeep;
		}
		return (*Node)->MaxDeep;
	}

	inline static LPTREE_ELEMENT *  GetNearPointerToMember(LPTREE_ELEMENT * Obj, TypeKey Key)
	{
		for (;*Obj && ((*Obj)->Key != Key); Obj = (Key > (*Obj)->Key) ? &(*Obj)->R : &(*Obj)->L);
		return Obj;
	}

public:


	static inline TypeData * GetElement(LPTREE_ELEMENT * Obj, TypeKey Key)
	{
		for (;*Obj && ((*Obj)->Key != Key); Obj = (Key > (*Obj)->Key) ? &(*Obj)->R : &(*Obj)->L);
		return *Obj;
	}

	//���������� ������ ��������
	LPTREE_ELEMENT AppendElement(LPTREE_ELEMENT * RootTree, TypeKey Key)
	{
		TempElement = NULL;
		TempKey = Key;
		RecursiveAppend(RootTree);
		return TempElement;
	}

	//�������� ������ ��������
	bool DeleteElement(LPTREE_ELEMENT * RootTree, TypeKey Key)
	{
		TempKey = Key;
		return RecursiveDel(RootTree);
	}

	//�������� ��������� ������ ������
	void ReleaseTree(LPTREE_ELEMENT Branch)
	{
		if (!Branch)
			return;
		ReleaseTree(Branch->L);
		ReleaseTree(Branch->R);
		DeleteFromBranchElement(Branch);
	}

};

#endif
#pragma once
#include "heap.hpp"
#include <iostream>
using namespace std;

template <typename W>
struct HuffmanTreeNode
{
	HuffmanTreeNode(const W& weight)
	: _pLeft(NULL)
	, _pRight(NULL)
	, _parent(NULL)
	, _weight(weight)
	{}

	HuffmanTreeNode* _pLeft;
	HuffmanTreeNode* _pRight;
	HuffmanTreeNode* _parent;
	W _weight;
};

template <typename W>
class HuffmanTree
{
	typedef HuffmanTreeNode<W>* PNode;
public:
	HuffmanTree()
		: _pRoot(NULL)
	{}

	HuffmanTree(W* array, size_t size,W invalide)
	{
		_CreateHuffmanTree(array, size,invalide);
	}

	PNode GetRoot()
	{
		return _pRoot;
	}

	~HuffmanTree()
	{
		_Destroy(_pRoot);
	}

	struct _Less
	{
		typedef HuffmanTreeNode<W>* pNode;
		bool operator() (pNode left, pNode right)
		{
			return left->_weight < right->_weight;
		}
	};

private:
	void _CreateHuffmanTree(W* array, size_t size,W invalide)
	{
		//������ȨֵΪ���Ķ�����ɭ��,����Щ����������һ��С����
		Heap<HuffmanTreeNode<W>*, _Less> hp;
		for (size_t i = 0; i < size; i++)
		{
			if (array[i] != invalide)
			{
				PNode p = new HuffmanTreeNode<W>(array[i]);
				hp.Push(p);
			}
		}

		//������������
		while (hp.Size()>1)
		{
			//ȡȨֵ��С�Ŀ����ֱ���������
			PNode pLeft = hp.Top();
			hp.Pop();
			PNode pRight = hp.Top();
			hp.Pop();

			//�����������Ϊ���Һ��Ӵ���һ����
			PNode Root = new HuffmanTreeNode<W>(pLeft->_weight + pRight->_weight);
			Root->_pLeft = pLeft;
			pLeft->_parent = Root;
			Root->_pRight = pRight;
			pRight->_parent = Root;

			//���´��������Żص�С����
			hp.Push(Root);
		}
		_pRoot = hp.Top();
	}

	void _Destroy(PNode& pRoot)
	{
		if (pRoot)
		{
			_Destroy(pRoot->_pLeft);
			_Destroy(pRoot->_pRight);
			delete pRoot;
			pRoot = NULL;
		}
	}

private:
	PNode _pRoot;
};

void FunTestHuffmanTree()
{
	int arr[] = { 5, 3, 1, 7 };
	HuffmanTree<int> hf(arr, 4,0);
}
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
		//创建以权值为根的二叉树森林,将这些二叉树放入一个小堆中
		Heap<HuffmanTreeNode<W>*, _Less> hp;
		for (size_t i = 0; i < size; i++)
		{
			if (array[i] != invalide)
			{
				PNode p = new HuffmanTreeNode<W>(array[i]);
				hp.Push(p);
			}
		}

		//创建哈夫曼树
		while (hp.Size()>1)
		{
			//取权值最小的棵树分别记作左和右
			PNode pLeft = hp.Top();
			hp.Pop();
			PNode pRight = hp.Top();
			hp.Pop();

			//根据左和右作为左右孩子创建一棵树
			PNode Root = new HuffmanTreeNode<W>(pLeft->_weight + pRight->_weight);
			Root->_pLeft = pLeft;
			pLeft->_parent = Root;
			Root->_pRight = pRight;
			pRight->_parent = Root;

			//将新创建的树放回到小堆中
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
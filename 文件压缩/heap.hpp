#pragma once
#include <iostream>
using namespace std;
#include <vector>
#include <assert.h>

template <typename T>
struct Less
{
	bool operator() (T left, T right)
	{
		return left< right;
	}
};

template <typename T>
struct Greate
{
	bool operator() (T left, T right)
	{
		return left> right;
	}
};

template <typename T, typename Compare = Less<T>>
class Heap
{
public:
	Heap()
	{}

	Heap(const T* array, size_t size)//带参构造函数
	{
		_array.resize(size);
		for (size_t i = 0; i < size; i++)
		{
			_array[i] = array[i];
		}
		_AdjustDown((size - 2) >> 1);
	}

	void Push(const T& data)
	{
		_array.push_back(data);
		_AdjustUp(_array.size() - 1);

	}

	void Pop()
	{
		if (_array.empty())
			return;

		swap(_array[0], _array[Size() - 1]);
		_array.pop_back();
		_AdjustDown(0);
	}

	T Top()const
	{
		assert(!_array.empty());
		return _array[0];

	}

	bool Empty()const
	{
		return _array.size() == 0;
	}

	size_t Size()const
	{
		return _array.size();
	}

private:
	void _AdjustDown(size_t parent)
	{
		if (_array.size()<2)
			return;

		size_t size = _array.size();
		size_t child = 2 * parent + 1;
		while (child > 0 && size>1)
		{
			size_t tmp = parent;
			while (parent<size)
			{
				child = parent * 2 + 1;
				if (child >= size)
					break;

				if (child + 1<size)
					child = Compare()(_array[child], _array[child + 1]) ? child : child + 1;


				if (Compare()(_array[child], _array[parent]))
				{
					swap(_array[child], _array[parent]);
					parent = child;
					child = parent * 2 + 1;
				}
				else
					break;
			}
			child = tmp;
			parent = parent - 1;
		}
	}

	void _AdjustUp(size_t child)
	{
		size_t parent = 0;
		while (child > 0)
		{
			parent = (child - 1) >> 1;
			if (Compare()(_array[child], _array[parent]))
			{
				swap(_array[child], _array[parent]);
				child = parent;
			}
			else
				break;
		}
	}
private:
	vector<T> _array;
};


void FunTest()
{
	int arr[] = { 2, 5, 7, 6, 3, 4, 8, 9 };
	Heap<int> hp(arr, 8);
	cout << hp.Size() << endl;
	cout << hp.Top() << endl;

	hp.Push(1);
	cout << hp.Size() << endl;
	cout << hp.Top() << endl;

	hp.Pop();
	cout << hp.Size() << endl;
	cout << hp.Top() << endl;

}


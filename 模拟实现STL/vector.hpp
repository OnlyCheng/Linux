#include <iostream>
using namespace std;
#include <assert.h>

template <class T>
class Vector
{
public:
	typedef T* Iterator;
public:
	Vector()
		: _start(0)
		, _finish(0)
		, _endOfStorage(0)
	{}

	Vector(const T* array, size_t size)
		:_start(new T[size])
		, _finish(_start + size)
		, _endOfStorage(_finish)
	{
		for (size_t i = 0; i < size; i++)
		{
			_start[i] = array[i];
		}
	}

	Vector(const Vector<T>& v)
	{
		size_t size = v.Size();
		size_t capacity = v.Capacity();

		T* temp = new T[capacity];
		_start = temp;
		for (size_t i = 0; i < size; i++)
		{
			_start[i] = v[i];
		}
		_finish = _start + size;
		_endOfStorage = _start + capacity;
	}

	Vector& operator=(const Vector& s)
	{
		size_t size = s.Size();
		size_t capacity = s.Capacity();

		T* temp = new T[capacity];
		_start = temp;
		for (int i = 0; i < size; i++)
		{
			_start[i] = s._start[i];
		}
	}

	~Vector()
	{
		if (_start)
		{
			delete[] _start;
			_start = _finish = _endOfStorage = NULL;
		}
	}

	//////////////Iterator///////////////////////////////// 
	Iterator Begin()
	{
		return _start;
	}
	Iterator End()
	{
		return _finish;
	}
	void PushBack(const T& data);
	void PopBack();
	void Insert(size_t pos, const T& data);
	void Erase(size_t pos);

	//////////////////capacity//////////////////////////// 
	size_t Size()const;
	size_t Capacity()const;
	bool Empty()const;
	void Resize(size_t newSize, const T& data = T());

	//////////////Acess/////////////////////////// 
	T& operator[](size_t index);
	const T& operator[](size_t index)const;
	T& Front();
	const T& Front()const;
	T& Back();
	const T& Back()const;
	void Clear();

private:
	void _CheckCapacity();

	friend ostream& operator<<(ostream& _cout, const Vector& v)
	{
		size_t size = v.Size();
		size_t capacity = v.Capacity();
		for (size_t i = 0; i < size; i++)
		{
			cout << v._start[i] << " ";
		}
		return _cout;
	}

private:
	T* _start;
	T* _finish;
	T* _endOfStorage;
};




template<class T>
void Vector<T>::PushBack(const T& data)
{
	_CheckCapacity();
	*_finish++ = data;
}

template<class T>
void  Vector<T>::PopBack()
{
	if (_start != _finish)
		_finish--;
}

template<class T>
void Vector<T>::Insert(size_t pos, const T& data)
{
	size_t size = Size();
	_CheckCapacity();
	if (pos < size)
	{
		for (size_t i = size; i > pos; i--)
		{
			_start[i] = _start[i - 1];
		}
		_start[pos] = data;
		_finish++;
	}
}

template<class T>
void  Vector<T>::Erase(size_t pos)
{
	size_t size = Size();
	if (pos < size)
	{
		for (size_t i = pos; i < size - 1; i++)
		{
			_start[i] = _start[i + 1];
		}
		_finish--;
	}
}

template<class T>
size_t Vector<T>::Size()const
{
	return _finish - _start;
}

template<class T>
size_t Vector<T>::Capacity()const
{
	return _endOfStorage - _start;
}

template<class T>
bool Vector<T>::Empty()const
{
	return _start == _finish;
}

template<class T>
void Vector<T>::Resize(size_t newSize, const T& data = T())
{
	assert(_start != NULL);
	size_t oldsize = Size();
	size_t capacity = Capacity();
	if (newSize <= oldsize)
	{
		_finish = _start + newSize;
	}
	else if (newSize > oldsize && newSize < capacity)
	{
		for (size_t i = oldsize; i < newSize; i++)
		{
			_start[i] = data;
		}
		_finish = _start + newSize;
	}
	else
	{
		T* temp = new T[newSize];
		for (size_t i = 0; i < oldsize; ++i)
			temp[i] = _start[i];

		for (size_t i = oldsize; i < newSize; ++i)
			temp[i] = data;

		delete[] _start;
		_start = temp;
		_finish = _start + newSize;
		_endOfStorage = _start + newSize;
	}
}

 
template<class T>
T& Vector<T>::operator[](size_t index)
{
	return *(_start + index);
}

template<class T>
const T& Vector<T>::operator[](size_t index)const
{
	return *(_start + index);
}

template<class T>
T& Vector<T>::Front()
{
	assert(_start != NULL);
	return *_start;
}

template<class T>
const T& Vector<T>::Front()const
{
	assert(_start != NULL);
	return *_start;
}

template<class T>
T& Vector<T>::Back()
{
	assert(_finish != NULL);
	return *(_finish - 1);
}

template<class T>
const T& Vector<T>::Back()const
{
	assert(_finish != NULL);
	return *(_finish - 1);
}

template<class T>
void Vector<T>::Clear()
{
	_finish = _start;
}

template<class T>
void Vector<T>::_CheckCapacity()
{
	size_t oldsize = Size();
	size_t capacity = Capacity();
	if (oldsize==capacity)
	{
		size_t newsize = capacity * 2 + 3;
		T* temp = new T[newsize];
		for (size_t i = 0; i < oldsize; i++)
		{
			temp[i] = _start[i];
		}
		delete[] _start;
		_start = temp;
		_finish = _start + oldsize;
		_endOfStorage = _start + newsize;
	}
}
#pragma once
#include <vector>
#include <algorithm>

template<class T>
class Heap
{
	std::vector<T> heap;

private:
	typedef typename std::vector<T>::iterator iter;
	iter getHeap(T x)
	{
		for (auto it = heap.begin(); it != heap.end(); it++)
		{
			if (*it == x) return it;
		}
		return heap.end();
	}

public:
	Heap();
	Heap(std::vector<T>);
	std::vector<T> toVector() const;

	int size() const;
	bool empty() const;
	// remove an element
	// return true if the element exist
	bool remove(T);
	// add or modify an element
	// return false if the element exist and modify it anyway
	bool push(T);
	// check if the element exist
	bool contain(T) const;
	T pop();
	T top() const;
};

template<class T>
inline Heap<T>::Heap()
{
}

template<class T>
inline Heap<T>::Heap(std::vector<T> vec) : heap(vec)
{
	make_heap(heap.begin(), heap.end());
}

template<class T>
inline std::vector<T> Heap<T>::toVector() const
{
	return heap;
}

template<class T>
inline int Heap<T>::size() const
{
	return heap.size();
}

template<class T>
inline bool Heap<T>::empty() const
{
	return heap.empty();
}

template<class T>
inline bool Heap<T>::remove(T x)
{
	if (auto it = getHeap(x); it != heap.end())
	{
		heap.erase(it);
		return true;
	}
	else
	{
		return false;
	}
}

template<class T>
inline bool Heap<T>::push(T x)
{
	if (auto it = getHeap(x); it != heap.end())
	{
		*it = x;
		make_heap(heap.begin(), heap.end());
		return false;
	}
	else
	{
		heap.push_back(x);
		make_heap(heap.begin(), heap.end());
		return true;
	}
}

template<class T>
inline bool Heap<T>::contain(T x) const
{
	return getHeap(x) != heap.end();
}

template<class T>
inline T Heap<T>::pop()
{
	if (empty()) return T();
	T x = heap.front();
	pop_heap(heap.begin(), heap.end());
	heap.pop_back();
	return x;
}

template<class T>
inline T Heap<T>::top() const
{
	return empty() ? T() : heap.front();
}
#ifndef INCLUDE_VEC_H
#define INCLUDE_VEC_H

#include <cstddef>
#include <memory>
#include <stdexcept>
#include <utility>

#ifdef DB
#include <iostream>
#endif

#define ALLOC(vec, size) \
	vec->track = vec->first = alloc.allocate(size); \
	vec->last = vec->first + size;

template <class T> class Vec
{
	T* first;
	T* track;
	T* last;
	std::allocator<T> alloc;

	protected:
		void destroy();
		void grow(std::size_t);
	public:
		Vec(): first(0), last(0) {};
		explicit Vec(std::size_t s) { ALLOC(this, s); };
		Vec(int, T);
		Vec(T*, T*);
		Vec(const Vec&);
		~Vec() { destroy(); }

		T operator[] (std::size_t);

		std::ptrdiff_t size() const { return first ? last - first : 0; };

		void erase() { destroy(); }
		void push_back(T);
		void pop_back();
};

template<class T> Vec<T>::Vec(int n, T val)
{
	ALLOC(this, n);
	
	this->track = std::uninitialized_fill_n(this->first, n, val);
}

template<class T> Vec<T>::Vec(T* iteratorF, T* iteratorL)
{
	std::ptrdiff_t size = iteratorL - iteratorF;
	
	ALLOC(this, size);

	this->track = std::uninitialized_copy(iteratorF, iteratorL, this->first);
}

template<class T> Vec<T>::Vec(const Vec& v)
{
	if(this != &v && v.first != NULL)
	{
		ALLOC(this, v.size());
		this->track = std::uninitialized_copy(v.first, v.last, this->first);
	}
}

template<class T> void Vec<T>::destroy()
{
	if(this->first)
	{
		for(T* i = last; i != first; --i)
		{
#ifndef DB
			i->~T();
#else
			std::cout << *i;
#endif
		}

		alloc.deallocate(this->first, this->last - this->first);
	}

	this->first = this->track = this->last = 0;
}

template<class T> void Vec<T>::grow(std::size_t size)
{
	std::pair<T*, int> temp = std::get_temporary_buffer<T>(this->size());
	std::uninitialized_copy(temp.first, temp.first + size, this->first);
	
	destroy();

	ALLOC(this, size);

	this->track = std::uninitialized_copy(temp.first, temp.first + size, this->first);

	std::return_temporary_buffer(temp.first);
}

template<class T> void Vec<T>::push_back(T val)
{
	if(this->track != this->last)
	{
		alloc.construct(this->track++, val);
	}
	else
	{
		grow(this->size() * 2);
		push_back(val);
	}
}

template<class T>  void Vec<T>::pop_back()
{
	if(this->first)
	{
		(this->track--)->~T();
	}
}

template<class T> T Vec<T>::operator[](std::size_t index)
{
	return *(this->first + index);
}

#endif

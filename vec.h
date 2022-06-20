#ifndef INCLUDE_VEC_H
#define INCLUDE_VEC_H

#include <cstddef>
#include <memory>

#define ALLOC(vec, size) \
	vec->first = alloc.allocate(size); \
	vec->last = vec->first + size;

template <class T> class Vec
{
	T* first;
	T* last;
	std::allocator<T> alloc;

	protected:
		void destroy();

	public:
		Vec(): first(0), last(0) {};
		Vec(int, T);
		Vec(T*, T*);
		Vec(const Vec&);
		~Vec() { destroy(); };

		T operator[] (std::size_t);

		std::ptrdiff_t size() const { return last - first; };
};

template<class T> Vec<T>::Vec(int n, T val)
{
	ALLOC(this, n);

	std::uninitialized_fill(this->first, this->last, val);
}

template<class T> Vec<T>::Vec(T* iteratorF, T* iteratorL)
{
	std::ptrdiff_t size = iteratorL - iteratorF;
	
	ALLOC(this, size);

	std::uninitialized_copy(iteratorF, iteratorL, this->first);
}

template<class T> Vec<T>::Vec(const Vec& v)
{
	if(this != &v && v.first != NULL)
	{
		ALLOC(this, v.size());
		std::uninitialized_copy(v.first, v.last, this->first);
	}
}

template<class T> void Vec<T>::destroy()
{
	while(this->last-- != this->first)
	{
		alloc.destroy(this->last);
	}

	alloc.deallocate(this->first, this->last - this->first);
}

template<class T> T Vec<T>::operator[](std::size_t index)
{
	return *(this->first + index);
}

#endif

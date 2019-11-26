#ifndef  __STL_UNINITIALIZED_H_
#define __STL_UNINITIALIZED_H_
#include"../iterator/type_traits.h"
#include"../iterator/stl_iterator_base.h"


template<typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_copy_aux(InputIterator begin, InputIterator end, ForwardIterator be, _true_type)
{
	return copy(begin, end, be); //？
}
template<typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_copy_aux(InputIterator begin, InputIterator end, ForwardIterator be, _false_type)
{
	ForwardIterator cur = be;
	for (; begin != end; begin++, cur++)
	{
		construct(&*cur, &*begin);
	}
	return cur;
}
template<typename InputIterator, typename ForwardIterator, typename T>
ForwardIterator _uninitialized_copy(InputIterator begin, InputIterator end, ForwardIterator be, T t)
{
	typedef typename _type_traits<T>::is_POD_type is_POD;
	uninitialized_copy_aux(begin, end, be, is_POD());
}
//copy
//思考 为什么fill不特化成char呢
template<typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator begin, InputIterator end, ForwardIterator be)
{
	return _uninitialized_copy(begin, end, be, value_type(be));
}




//特化版本
inline char * uninitialized_copy(const char*begin, const char*end, char *result)
{
	memmove(result, begin, end - begin);
	return result + (end - begin);
}

inline wchar_t* uninitialized_copy(const wchar_t*begin, const wchar_t*end, wchar_t*result)
{
	memmove(result, begin, sizeof(wchar_t) * (end - begin));
	return result + (end - begin);
}

//fill
template<typename ForwardIterator, typename T>
inline void uninitialized_fill(ForwardIterator begin, ForwardIterator end, const T& a)
{
	return _unininialized_fill(begin, end, a, value_type(begin));
}

template<typename ForwardIterator, typename T, typename T1>
inline void _uninitialized_fill(ForwardIterator begin, ForwardIterator end, const T&value, T1 type)
{
	typedef typename _type_traits<T1>::is_POD_type is_POD;
	return _uninitialized_fill_aux(begin, end, value, is_POD());
}

template<typename ForwardIterator, typename T> //由于循环较多，就不用inline
void uninitialized_fill_aux(ForwardIterator begin, ForwardIterator end, const T&value, _false_type)
{
	ForwardIterator cur = begin;
	for (; cur != end; cur++)
		construct(&*cur, value);
}

template<typename ForwardIterator, typename T>
inline void uninitialized_fill_aux(ForwardIterator begin, ForwardIterator end, const T &value, _true_type)
{
	fill(begin, end, value);
}

//fill_n
template<typename ForwardIterator, typename Size, typename T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator begin, Size n, const T& a)
{
	return _unininialized_fill_n(begin, n, a, value_type(begin));
}

template<typename ForwardIterator, typename Size, typename T, typename T1>
inline ForwardIterator _uninitialized_fill_n(ForwardIterator begin, Size n, const T &value, T1 type)
{
	typedef typename _type_traits<T1>::is_POD_type is_POD;
	return _uninitialized_fill_aux(begin, n, value, is_POD());
}


//为什么都是返回的最后的
template<typename ForwardIterator, typename Size, typename T> //由于循环较多，就不用inline
ForwardIterator uninitialized_fill_n_aux(ForwardIterator begin, Size n, const T &value, _false_type)
{
	ForwardIterator cur = begin;
	for (; n > 0; --n, cur++)
		construct(&*cur, value);
	return cur;
}

template<typename ForwardIterator, typename Size, typename T>
inline ForwardIterator  uninitialized_fill_aux(ForwardIterator begin, Size n, const T&value, _true_type)
{
	return fill_n(begin, n, value);
}

#endif

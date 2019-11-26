#ifndef __STL_ALGOBASE_H_
#define __STL_ALGOBASE_H_

template<typename InpurIterator, typename OutputIterator>
inline OutputIterator copy(InpurIterator first, InpurIterator last, OutputIterator result)
{
	return __copy_dispatch<InpurIterator, OutputIterator>()(first, last, result);
}

//是否可以接受非const的版本，或者是否会优于模板先接受
inline char * copy(const char*first, const char *last, char *result)
{
	memmove(result, first, last - first);
	return result + (last - first);
}

inline wchar_t * copy(const wchar_t*first, const wchar_t *last, wchar_t *result)
{
	memmove(result, first, last - first);
	return result + (last - first);
}


//结构体，操作符()重载
template<typename InpurIterator, template OutputIterator>
struct __copy_dispatch
{
	OutputIterator operator()(InpurIterator first, InpurIterator last, OutputIterator result)
	{
		return __copy(first, last, result, iterator_category(first));
	}
}


//偏特化版本，两个参数都是T*形式
template<typename T>
struct __copy_dispatch<T*, T*>
{
	T* operator()(T* first, T* last, T*result)
	{
		typedef typename __type_traits<T>::has_trivial_assigment_operator t;
		return __copy_t(first, last, result, t());
	}
}
//第一个参数为const T*版本，第二个参数为T*版本。
template<typename T>
struct __copy_dispatch<const T*, T*>
{
	T* operator()(const T* first, const T* last, T*result)
	{
		typedef typename __type_traits<T>::has_trivial_assigment_operator t;
		return __copy_t(first, last, result, t());
	}
}


//如果只是input_iterator_tag类型
template<typename InpurIterator, typename OutputIterator>
OutputIterator __copy(InpurIterator first, InpurIterator last, OutputIterator result, input_iterator_tag)
{
	//以迭代器等同与否决定是否继续，速度慢
	for (; first != last; first++, result++)
		*result = *first;
	return result;
}

//如果是random的
template<typename InpurIterator, typename OutputIterator>
OutputIterator __copy(InpurIterator first, InpurIterator last, OutputIterator result, random_iterator_tag)
{
	//重新调用以便其他函数可以调用这个__copy_d的实现。
	return __copy_d(first, last, result, distance_type(first));
}

template<typename InpurIterator, typename OutputIterator, typename Distance>
OutputIterator __copy_d(InpurIterator first, InpurIterator last, OutputIterator result, Distance* )
{
	//以n决定循环的迭代次数，速度快
	for (Distance n = last - first; n > 0; n--, first++, result++)
		*result = *first;
	return result;
}


//如果拥有non_trivial_assigmengt_operator，则可以用memmove
template<typename T>
inline T* __copy_t(const T* first, const T*last, T* result, __true_type)
{
	memmove(result, first, sizeof(T) * (last - first));
	return result + sizeof(T) * (last - first);
}
//如果拥有的是trivial assignment operator，则还是用原本的一步步赋值
template<typename T>
inline T* __copy_t(const T* first, const T*last, T* result, __false_type)
{
	return __copy_d(first, last, result, (ptrdiff_t*)0);
}
#endif

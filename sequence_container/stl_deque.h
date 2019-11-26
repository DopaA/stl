#ifndef __STL_DEQUE_H_
#define __STL_DEQUE_H_
//全局
inline __deque_buf_size(size_t n, size_t size)
{
	return n != 0 ? n : (size < 512 ? size_t(512 / size) : size_t(1));
}

template<class T, class Ref, class Ptr, size_t BufSiz>
struct __deque_iterator //没有继承iterator
{
	typedef __deque_iterator<T, T&, T*, BufSiz> iterator;
	typedef __deque_iterator<T, const T&, const T*, BufSiz> const_iterator;
	//缓冲区大小
	static size_t buffer_size()
	{
		return __deque_buf_size(BufSiz, sizeof(T));
	}
	//由于没有继承iterator,所以一定要定义。
	typedef random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef Ref reference;
	typedef Ptr pointer;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T** map_pointer;
	typedef __deque_iterator self;

	T* current;
	T* first;
	T* last;
	map_pointer map;
}
//BufSiz为0的话默认为512bytes
template<class T, class Alloc = alloc, size_t BufSiz = 0>
class deque
{
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type& reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
protected://内部的typedef
	typedef pointer* map_pointer;//指针的指针
protected:
	map_pointer map;
	size_type map_size;
}
#endif

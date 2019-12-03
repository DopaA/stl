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
	map_pointer node; //管控中心
	//跳入新的缓冲区
	//不需要检测是否越界吗
	void set_node(map_pointer new_node)
	{
		node = new_node;
		first = *new_node;
		last = first + difference_type(buffer_size());
	}
	//关键的操作符重载
	reference operator*() const
	{
		return *current;
	}
	pointer operator->() const
	{
		return &(operator*());   //直接调用上一个函数。。。应该比*current更快
	}
	difference_type operator-(const self& x)const
	{
		//由于是this-x,this在后，因此后面的是要取前部分的
		return difference_type(buffer_size()) * (node - x.node - 1) +
		       (current - first) + (x.last - x.current);
	}
	self & operator++()
	{
		++current;
		if (current == last)
		{
			set_node(node + 1);
			current = first;
		}
		return *this;
	}
	self &operator++(int)
	{
		self tmp = *this;
		++*this;
		return tmp;
	}
	self &operator--()
	{
		/*--current;
		if(current=first){
		    set_node(node-1);
		    current=last-1;
		}
		*/
		if (current = first) //确实应该先测是否已经在头部
		{
			set_node(node - 1);
			current = last;
		}
		--current;
		return *this;
	}
	self &operator--(int)
	{
		self tmp = *this;
		--*this;
		return tmp;
	}
	self &operator+=(difference_type n) //n也可能是负数
	{
		difference_type offset = n + (current - first);
		if (offset >= 0 && offset < difference_type(buffer_size()))
		{
			current += n;
		}
		else
		{
			//负数情况下两个减一的用处：第一个是为了事刚好是整数倍的时候达成减一的效果，因为如果是
			//整数倍那么也可以在那一个缓冲区表示，没必要跳过，而第二个-1是由于负数本身就要向上一个迁移
			difference_type node_offset = offset > 0 ? offset / difference_type(buffer_size())
			                              : -difference_type(-offset - 1) / buffer_size() - 1;
			set_node(node + node_offset);
			current = first + (offset - node_offset * difference_type(buffer_size()));
		}
		return *this;
	}
	//没有引用标识符，只是一个临时的
	self operator+(difference_type n)
	{
		self tmp = *this;
		return tmp += n;
	}
	self &operator-=(difference_type n)
	{
		return *this += -n;
	}
	self operator-(difference_type n)
	{
		self tmp = *this;
		return tmp -= n;
	}
	reference operator[](difference_type n) const
	{
		return *(*this + n);//*this是self类型，再次*才是调用的运算符重载
	}
	bool operator==(const self &x)
	{
		return current == x.current;//必须地址相同?不应该是内容相同吗
	}
	bool operator!=(const self &x)
	{
		return !(*this == x);
	}
	bool operator<(const self &x){
		return 
	}
	
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

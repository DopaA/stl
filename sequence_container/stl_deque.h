#ifndef __STL_DEQUE_H_
#define __STL_DEQUE_H_
#include"../stl_config.h"
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

	//构造函数
	__deque_iterator(T* x, map_pointer y): current(x), first(y), last(*y + buffer_size()), node(y) {}
	__deque_iterator(): current(0), first(0), last(0), node(0) {}
	__deque_iterator(const iterator&x): current(x.current), first(y.first), last(x.last), node(x.node) {}

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

	//由于此处是对迭代器的相等与否的重载，因此是比较指针而不是具体内容
	bool operator==(const self &x)
	{
		return current == x.current;
	}
	bool operator!=(const self &x)
	{
		return !(*this == x);
	}
	bool operator<(const self &x)
	{
		return (node == x.node) ? (current < x.current) : (node < x.node);
	}
	bool operator>(const self&x)
	{
		return x < *this;
	}
	bool operator<=(const self &x)
	{
		return !(x < *this);
	}
	bool operator>=(const self&x)
	{
		return !(*this < x);
	}
}


template<class T, class Alloc>
class deque_base
{
public:
	typedef __deque_iterator<T, T&, T*> iterator;
	//const_iterator的作用？
	typedef Alloc allocator_type;
	allocator_type get_allocator() const
	{
		return allocator_type();
	}

	deque_base(const allocator_type&, size_t num_elements)
		: map(0), map_size(0), start(), finish()
	{
		initialize_map(num_elements);
	}
	deque_base(const allocator_type&, size_t num_elements)
		: map(0), map_size(0), start(), finish() {}
	~deque_base();
protected:
	iterator start;
	iterator finish;
	T** map;
	size_t map_size;

	//分配空间和回收空间
	typedef simple_alloc<T, Alloc> node_alloc_type;
	typedef simple_alloc<T*, Alloc> map_alloc_type;

	T* allocate_node()
	{
		return node_alloc_type::allocate(__deque_buf_size(sizeof(T)));
	}
	void deallocate_node(T *p)
	{
		node_alloc_type::deallocate(p.sizeof(T));
	}
	T** allocate_map(size_t n)
	{
		return   map_alloc_type::allocate(n);
	}
	void deallocate_map(T** p, size_t n)
	{
		map_alloc_type::deallocate(p, n);
	}

protected:
	void initialize_map(size_t);
	void create_nodes(T** start, T** finish);
	void destory_nodes(T** start, T** finish);
	enum {initialize_map_size = 8};//此处的作用?
}

template<class T, class Alloc>
deque_base<T, Alloc>::~deque_base()
{
	if (map)
	{
		destory_nodes(start.node, finish.node + 1); //为什么加一
		deallocate_map(map, map_size);
	}
}

template<class T, class Alloc>
void deque_base<T, Alloc>::initialize_map(size_t num_elements)
{
	size_t num_nodes = num_elements / __deque_buf_size(sizeof(T)) + 1;
	map_size = max((size_t)initialize_map_size, num_nodes + 2); //可见一个map最少管理8个节点，最多是“所需节点数加2”
	map = allocate_map(map_size);

	T** nstart = map + (map_size - num_nodes) / 2; //start和finish放在最中间，以便向两端扩展
	T**  nfinish = start + num_nodes;

	__STL_TRY
	{
		create_nodes(nstart, nfinish);
	}
	__STL_UNWIND((deallocate_map(map, map_size), map = 0, map_size = 0));

	start.set_node(nstart);
	finish.set_node(nfinish);
	start.cur = start.first;
	finish.cur = finish.first + num_elements % __deque_buf_size(sizeof(T));
}
template<class T, class Alloc>
void deque_base<T, Alloc>::create_node(T** start, T**finish)
{
	T** cur;
	__STL_TRY
	{
		for (cur = start; cur != finish; cur++)
			*cur = allocate_node();
	}
	__STL_UNWIND(destory_nodes(start, cur));
}
template<class T, class Alloc>
void deque_base<T, Alloc>::destory_nodes(T** start, T**finish)
{
	T **cur;
	for (cur = start, cur != finish, cur++)
		deallocate_node(*cur);
}


//BufSiz为0的话默认为512bytes
template<class T, class Alloc = alloc, size_t BufSiz = 0>
class deque : protected deque_base<T, Alloc>
{
private:
	typedef  deque_base<T, Alloc> Base;
public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef value_type& reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	typedef typename Base::allocator_type allocator_type;
	allocator_type get_allocator() const
	{
		return Base::get_allocator();
	}
public:
	typedef __deque_iterator<T, T&, T*>  iterator;
protected://内部的typedef
	typedef pointer* map_pointer;//指针的指针
	static size_t buffer_size()
	{
		return __deque_buf_size(sizeof(T));
	}
protected:
//这些命名空间的用处是什么，不是可以直接使用？
#ifdef __STL_USE_NAMESPACES
	using _Base::_M_initialize_map;
	using _Base::_M_create_nodes;
	using _Base::_M_destroy_nodes;
	using _Base::_M_allocate_node;
	using _Base::_M_deallocate_node;
	using _Base::_M_allocate_map;
	using _Base::_M_deallocate_map;

	using _Base::_M_map;
	using _Base::_M_map_size;
	using _Base::_M_start;
	using _Base::_M_finish;
#endif /* __STL_USE_NAMESPACES */
protected:

	void reallocate_map(size_type nodes_to_add,bool add_at_front){
		size_type old_num_nodes=finish.node -start.node+1;
		size_type new_num_nodes=old_num_nodes+nodes_to_add;
		map_pointer new_start;
		//如果总的map的大小仍然大于新的用已经使用的node数，则不用再分配空间，只需要平衡一下
		if(map_size>2*new_num_nodes){
			//如果是加在前方的那前方就多一点空间
			new_start=map+(map_size-new_num_nodes)/2+
				(add_at_front?nodes_to_add:0);
			if(new_start<start.node)//新的开始节点在前方
				copy(start.node,finish.node+1,new_start);
			else//如果在后方，那么就要反过来复制迁移，第三个参数为迁移之后的最后的地方
				copy_backward(start.node,finish.node+1,new_node+old_num_nodes);
		}else{//否则重新分配空间
			size_type new_map_size=map_size+max(map_size,nodes_to_add)+2;//最少扩大两倍
			map_pointer new_map=allocate_map(map_size);
			new_start=new_map+(new_map_size-new_num_nodes)/2+(add_at_front?nodes_to_add:0);
			copy(start.node,finish.node+1,new_start);
			deallocate_map(map,map_size);
			map=new_map;
			map_size=new_map_size;
		}
		//不能直接赋值吗
		start.set_node(new_start);
		finish.set_node(new_start+old_num_nodes-1);
	}
	//默认添加一个node
	//如果要在后方储备的节点数大于剩余的空的nodes，就调用reallocate_map,
	void reserve_map_at_back(size_type nodes_to_add=1){
		if(nodes_to_add+1>map_size-(finish.node-map))
			reallocate_map(nodes_to_add,false);
	}
	void reserve_map_at_front(){
		if(nodes_to_add+1>(start.node-map))
			reallocate_map();
	}
public:
	void    fill_initialize(size_type n, const value_type & value);
	deque(size_type n, const value_type &value): start(), finish(), map(0), map_size(0)
	{
		fill_initialize(n, value);
	}
	iterator begin(){return start;}
iterator	end(){return finish;}
	reference operator[](size_type n)
	{
		return start[difference_type(n)];
	}
	reference front()
	{
		return *start;
	}
	reference back()
	{
		iterator tmp = finish;
		--tmp;
		return *tmp;//不能直接用*(finish-1)吗
	}
	size_type size() const
	{
		return finish - start;
	}
	size_type max_size() const
	{
		return size_type(-1);
	}
	bool empty()
	{
		return start == finish;
	}
	
public:
	//从push和pop可以看出front出current可以直接取值，但是finish处实在最后一个的下一个地址
	void push_back(const T& x){
		if(finish.current!=finish.last-1){//如果有充足备用空间
			construct(finish.current,x);//不用先加一吗
			++finish.current;
		}else{
			push_back_aux(x);
		}
	}
	void push_front(const T& x){
		if(start.current!=start.first){
			construct(start.current-1,x)
			--start.current;//不会越界，只会到达最前面从而无法进行下次添加
		}else{
			push_front_aux(x);
		}
	}
	void pop_back(){
		if(finish.current!=finish.first){
			--finish.current;
			destory(finish.current);
		}else{
			pop_back_aux();
		}
	}
	void pop_front(){
		if(start.current!=start.last-1){//如果等于的话++操作需要转向下一个node
			destory(start.current);
			++start.current;
		}else{
			pop_front_aux();
		}
	}
	//
	void push_back_aux(const value_type &x){
		value_type x_copy = x;
		reserve_map_at_back();//
		*(finish.node+1)=allocate_node();//分配一个新的node空间
		__STL_TRY{
			construct(finish.current,x_copy);//注意push_back是在恰好剩最后一个空间的时候调用的这个函数，因此先在最后一个上面进行构造，然后将finish移入新的节点
			finish.set_node(finish.node+1);
			finish.current=finish.first;
		}
		__STL_UNWIND(deallocate_node(*(finish.node+1)));
	}
	void push_front_aux(const value_type &x){
		value_type x_copy=x;//为什么要用x_copy而不直接用x
		reserve_map_at_front();
		*(start.node-1) = allocate_node();
		__STL_TRY{
			start.set_node(start.node-1);
			start.current=start.last-1;
			construct(start.current,x_copy);
		}
		__STL_UNWIND((++start,deallocate_node(*(start.node-1))));
	}
	void pop_back_aux(){
		deallocate_node(finish.first);
		finish.set_node(finish.node-1);//这里不是已经被deallocate了吗
		finish.current=finish.last-1;
		destory(finish.current);//这里才是最后一个数
	}
	void pop_front_aux(){
		destory(start.current);
		deallocate_node(start.first);
		start.set_node(start.node+1);
		start.current=start.first;
	}

public:
	//此处的插入好像是在所指位置的插入,不是所指位置的前方！
	iterator insert(iterator position,const value_type& x){
		if(position.cur==start.current){
			push_front(x);
			return start;
		}else if(position.cur == finish.current){
			push_back(x);
			iterator tmp=finish;
			--tmp;//减一下才是真正的地方
			return tmp;
		}else{
			insert_aux(position,x);
		}
	}
	//通过判断哪边元素最少进行移动，尽量减少移动次数
	//但是这个辅助函数不是在所指位置前方插入啊，而是在所指位置插入。
	iterator insert_aux(iterator position,const value_type& x){
		difference_type index=position -start;
		value_type x_copy=x;
		if(size_type(index)<size()/2){
			push_front(front());
			iterator front1=start;
			++front1;
			iterator front2=front1;
			++front2;
			position = start+index;
			iterator position1=position;
			++position1;
			copy(front2,pos1,front1);
		}else{
			push_back(back());
			iterator back1=finish;
			--back1;
			iterator back2=back1;
			--back2;
			pos=start+index;
			copy_backward(position,back2,back1);
		}
		*pos=x_copy;
		return pos;
	}

	//erase 和clear
	iterator erase(iterator pos){
		iterator next=pos;
		++next;
		difference_type index=pos-start;
		if(size_type(index)<(size()>>1)){//如果擦除位置靠前，那么前面的移动会少
			copy_backward(start,pos,next);
			pop_front();
		}else{
			copy(next,finish,pos);
			pop_back();
		}
		return start+index;
	}
	iterator erase(iterator first,iterator last){
		if(first==start&&last==finish){
			clear();
			return finish;
		}else{
			difference_type n=first-last;
			difference_type elems_before=first-start;
			if(elems_before<(size()-n)/2){
				copy_backward(start,first,last);
				iterator new_start=start+n;
				destory(start,new_start);
				for(map_pointer cur=start.node;cur<new_start.node;++cur){
					data_allacator::deallocate(*cur,buffer_size());
				}
				start=new_start;
			}else{
				copy(last,finish,first);
				iterator new_finish=finish-n;
				destory(new_finish,finish);
				for(map_pointer cur=new_finish.node;cur<finish.node;++cur){
					data_allacator::deallocate(*cur,buffer_size());
				}
				finish=new_finish;
			}
			return start+elems_before;
		}
	}
	void clear(){
		for(map_pointer node=start.node+1;node<finish.node;++node){
			destory(*node,*node+buffer_size());
			data_allacator::deallocate(*node,buffer_size());
		}
		if(start.node!=finish.node){//至少有头尾两个缓冲区
			destory(start.first,start.last);
			destory(finish.first,finish.last);
			data_allacator::deallocate(finish.first,buffer_size());
		}else{
			destory(start.first,start.last);
		}
		finish=start;
	}
}



//暂置
template<class T, class Alloc, size_t  BufSiz>
void deque<T, Alloc, BufSiz>::fill_initialize(size_type n, const value_type &value)
{
	
}

#endif

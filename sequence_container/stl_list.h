
//list的节点类型,双向链表
template<typename T>
struct __list_node{
	typedef void* void_pointer;
	void_pointer  prev;//也可以直接为__list_node<T> *
	void_pointer  next;
	T 			 data;
}

template<>typename T,typename Ref,typename Ptr>
struct  __list_iterator{
	typedef __list_iterator<T,T&,T*>  	iterator;
	typedef __list_iterator<T,Ref,Ptr>  self;

	typedef bidirectional_iterator_tag  iterator_category;
	typedef T 	value_type;
	typedef Ref reference;
	typedef Ptr pointer;
	typedef __list_node<T>* link_type;
	typedef size_t size_type;//??
	typedef ptrdiff_t difference_type;//??

	link_type	node;

	__list_iterator(){}
	__list_iterator(link_type x):node(x){}
	__list_iterator(const iterator& x):node(x.node){}//??

	bool operator==(const self& x) const {
		return this.node==x.node;
	}
	bool operator!=(const self& x) const {
		return this.node!=x.node;
	}
	reference operator*() const{
		return (*node).data;
	}
	pointere operator->() const{
		return &(operator*());
	}
	self& operator++(){
		node=(link_type)((*node).next);
		return *this;
	}

	self& operator++(int){
		self tmp= *this;
		++*this;
		return tmp;
	}

	self& operator--(){
		node=(link_type)((*node).prev);
		return *this;
	}

	self& operator--(int){
		self tmp= *this;
		--*this;
		return tmp;
	}
}



template<typename T,typename Alloc=alloc>
class list{
	protected:
		typedef __list_node<T> list_node;
		typedef __list_iterator<T,T&,T*>  	iterator;
		typedef simple_alloc<list_node,Alloc> list_node_allocator;
	public:
		typedef list_node* link_type;
	protected:
		link_type node;
	public:
		iterator begin(){return (link_type)((*node).next);}
		iterator end(){return node;}//这里返回后自动转换了？调用iterator的构造函数？
		bool empty(){return node->next==node;}
		size_type size(){
			size_type result=0;
			distance(begin(),end(),result);//全局函数，第三章迭代器的
			return result;
		}
		//取元素值
		reference front(){
			return *begin();
		}
		reference back(){
			return *(--end()):
		}
		void push_back(const T& x){
			insert(end(),x);//重载函数，有多种形式
		}
		void push_front(const T&x){
			insert(begin(),x);
		}
		void pop_front(){
			erase(begin());//begin()是link_type类型，erase参数为iterator类型
		}
		void pop_back(){
			iterator tmp=end();
			erase(--tmp);//重载的--
		}
		//swap函数
		void swap(list<T,Alloc>&x){
			STD::swap(node,x.node);
		}
		//清除所有元素
		void clear();
		//将某个数值的元素移除
		void remove(const T &);
		//移除连续而相同的元素//必须连续+相同
		void unique();
		void merge(list &x);//是因为在类中，不用list<T,Alloc>??
		void reverse();
		void sort();
		//将x接合于position之前，x必须不同于*this
		void splice(iterator position,list &x);
		//将i所指元素接合于position所指位置之前，position和i可指向同一个list
		void splice(iterator position,list &,iterator i);
		//将[first,last)内的元素接合于position所指位置之前。
		//可以指向同一个list
		//但是position不能在[first,last)内
		//此处的list&是什么用处?
		void splice(iterator position,,list&,iterator first,iterator last);
	//辅助函数
	protected:
		//插入节点
		iterator insert(iterator position,const T& x);
		//移除节点
		iterator erase(iterator position){
		//将[first,last)中的所有元素移动到position之前
		void transfer(iterator position,iterator first,iterator last);
	//节点的配置和销毁
	protected:
		//配置一个节点
		link_type get_node(){return list_node_allocator::allocate();}
		void put_node(link_type p){ list_node_allocator::deallocate(p);}
		link_type create_node(const T&x){
			link_type p=get_node();
			construct(&p->data,x);//全局函数
			return p;
		}
		void destory_node(link_type p){
			destory(&p->data);//全局函数，
			put_node(p);
		}
		//构造函数
	public:
		list(){empty_initialize();}
	protected:
		empty_initialize(){
			node=get_node();
			node->prev=node;
			node->next=node;
		}
	
}


template<typename T,typename Alloc>
void list<T,Alloc>::clear(){
	link_type cur=(link_type)node->next;//强制转换是因为next是void*
	while(cur!=node){
		link_type tmp=cur;
		cur=(link_type)cur->next;
		destory_node(tmp);
		//personal:
		//link_type tmp=cur;
		//destory_node(cur);
		//cur=(link_type)tmp->next;
	}
	node->next=node;
	node->prev=node;
}


template<typename T,typename Alloc>
void list<T,Alloc>::remove(const T &x){
	iterator first=begin();
	iterator last=end();
	while(first!=last){
		iterator tmp=first;
		++tmp;
		if(*first == x){
			erase(first);
		}
		first=tmp;
	}
}

template<typename T,typename Alloc>
void list<T,Alloc>::unique(){
	iterator first=begin();
	iterator last=end();
	if(first==last)return;
	iterator next=first;
	while(++next!=last){
		if(*next==first)
			erase(next);
		else
			first=next;
		next=first;
	}
}


//为什么书上这里不用->?????
//可以在同一个list中
template<typename T,typename Alloc>
void list<T,Alloc>::transfer(iterator position,iterator first,iterator last){
	if(position!=last){
		//本书中没有用->而用的.的原因
		//要考虑到新到的指针的连接和旧有的地方的连接
		//先不改变某个要改变指向的prev，先使用prev
		last.node->prev->next=position.node;
		first.node->prev->next=last.node;
		position.node->prev->next=first.node;
		//使用完prev->next之后在改变prev的值
		link_type tmp=(link_type)position.node->prev;
		position->prev=last.node->prev;
		last.node.prev=first.node->prev;
		first.node->prev=tmp;
	}
}


		//插入节点
template<typename T,typename Alloc>
iterator list<T,Alloc>::insert(iterator position,const T& x){
	link_type p=create_node(x);
	//p->prev=position->prev;
	//position->prev->next=p;
	//p->next=position;
	//position->prev=p;
	p->prev=position.node->prev;//position不应该就是node类型吗//what is iterator
	p->next=position.node;
	(link_type(position.node->prev))->next = p;
	position.node->prev=p;
	return p;
}


		//移除节点
template<typename T,typename Alloc>
iterator list<T,Alloc>::erase(iterator position){
	link_type prev_node=(link_type)(position.node->prev);
	link_type next_node=(link_type)(position.node->next);
	prev_node->next=next_node;
	next_node->prev=prev_node;
	//position.node->prev->next=position.node->next;
	//不用这种方式的原因？
	//position.node->next->prev=position.node->prev;
	destory_node(position);
	return iterator(next_node);//这里调用构造函数了，但是end()和begin()没有
}

//注意transfer的使用
//三个splice的重载版本
template<typename T,typename Alloc>
void list<T,Alloc>::splice(iterator position,list &x){
	if(!x.empty())
		transfer(position,x.begin(),x.end());
}

template<typename T,typename Alloc>
void list<T,Alloc>::splice(iterator position,list&,iterator i){
	iterator j=i;
	++j;
	//如果其本来就在position之前或者就是position，则直接返回
	if(position==i||position==j) return;
	transfer(position,i,j);
}

template<typename T,typename Alloc>
void list<T,Alloc>::splice(iterator position,list&,iterator first,iterator last){
	if(first!=last)
		transfer(position,first,last);
}

//两个list都必须已经经过递增排序
//将x合并到*this上
template<typename T,typename Alloc>
void list<T,Alloc>::merge(list<T,Alloc> &x){
	iterator first1=begin();
	iterator last1=end();
	iterator first2=begin();
	iterator last2=end();
	while(first1!=last1&&first2!=last2){
		if(*first1>*first2){
			iterator next=first2;
			//直接在表达式中++first2可能会不可控？
			transfer(first1,first2,++next);
			first2=next;
		}else
			++first1;
	}
	if(first2!=last2)transfer(last1,first2,last2);
}

//将*this的内容逆向重置
template<typename T,typename Alloc>
void list<T,Alloc>::reverse(){
	//两个元素才有重置的必要
	if(node->next!=node&&link_type(node->next)->next!=node){
		iterator first=begin();
		++first;
		while(first!=end()){
			iterator old=first;
			++first;
			transfer(begin(),old,first);
		}
	}
}

//stl的sort算法只接受RandomAccessIterator
//因此list必须有自己的sort算法
//书上说这里为快速排序，怎么看都是归并排序
//理解排序 原理
template<typename T,typename Alloc>
void list<T,Alloc>::sort(){
	//多于一个元素才有排序的必要
	if(node->next!=node&&link_type(node->next)->next!=node){
		list<T,Alloc> carry;
		list<T,Alloc> counter[64];//为什么是64//所以上限是2^64个数的排序吗
		int fill=0;
		while(!empty()){
			carry.splice(carry.begin(),*this,begin());
			int i=0;
			while(i<fill&&!counter[i].empty()){
				counter[i].merge(carry);
				carry.swap(counter[i++]);
			}
			carry.swap(counter[i]);
			if(i==fill)fill++;
		}
		for(int i=1;i<fill;i++)
			counter[i].merge(counter[i-1]);
		swap(count[fill-1]);
	}
}


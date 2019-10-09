#####   有几个需要注意的地方。
1. sgi_stl内部是将配置内存和构造对象分离的，通过*:operator new和:operator delete*配置内存,通过构造函数和析构函数构造对象和析构对象。
2. value_type()返回指针类型的原因:

	template <class ForwardIterator, class T>
	inline void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x){
		 __uninitialized_fill(first,last,x,value_type(first));
	}

	template <class ForwardIterator, class T, class T1>
	inline void __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x, T1*){
		typedef typename __type_traits<T1>::is_POD_type is_POD;
		__uninitialized_fill_aux(first, last, x, is_POD());
	}

对于类型T1来说，我们不知道如何构造和移动或者复制它，所以直接传递一个value_type是不可能的，可能会产生一个完全不必要的复制品(产生不必要的开销)并且其副作用是不可消除的，但是传递一个以0初始化的指针的话就不会有这些问题，这种方法是handy hack的。

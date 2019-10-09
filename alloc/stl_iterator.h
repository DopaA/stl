struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag:public input_iterator_tag{};
struct bidirectional_iterator_tag:public forward_iterator_tag{};
struct random_access_iterator_tag:public bidirectional_iterator_tag{};


template<typename Category,typename T,typename Distance=ptrdiff_t,
			typename Pointer=T*,typename Reference =T&>
struct iterator{
	typedef Category  iterator_category;
	typedef T 			value_type;
	typedef Distance  difference_type;
	typedef Pointer	pointer;
	typedef Reference reference;
};

//萃取
template<typename Iterator>
sturct iterator_traits{
	typedef typename Iterator::iterator_category iterator_category;
	typedef typename Iterator::value_type 			value_type;
	typedef typename Iterator::difference_type 	difference_type;
	typedef typename Iterator::pointer				pointer;
	typedef typename Iterator::reference			reference;
};

template<typename Iterator>
inline typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator &){
	typedef typename iterator_traits<Iterator>::iterator_category category;
	return category();
}


template<typename Iterator>
inline typename iterator_traits<Iterator>::difference_type* //为什么是指针类型
distance_type(const Iterator &){
	return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

template<typename Iterator>
inline typename iterator_traits<Iterator>::value_type* //为什么是指针类型
value_type(const Iterator &){
	return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

template<typename InputIterator>
inline iterator_traits<InputIterator>::difference_type
__distance(InputIterator first,InputIterator last,input_iterator_tag){
	iterator_traits<InputIterator>::difference_type n=0;
	for(;first!=last;first++)
		n++;
	return n;
}

template<typename RandomAccessIterator>
inline iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first,RandomAccessIterator last,random_access_iterator_tag){
	return last-first;
}


template<typename InputIterator>
inline iterator_traits<InputIterator>::difference_type
distance(InputIterator first,InputIterator last){
	typedef typename iterator_traits<InputIterator>::iterator_category category;
	return __distance(first,last,category());
	//没有直接调用iterator_category(first)的原因;
}

template<typename InputIterator,typename Distance>
inline void __advance(InputIterator &i,Distance n,input_iterator_tag){
	while(n-- ) ++i;
}

template<typename BidiectionalIterator,typename Distance>
inline void __advance(BidiectionalIterator &i,Distance n,bidirectional_iterator_tag){
	if(n>0)
		while(n--) ++i;
	else
		while(n++) --i;
}

template<typename RandomAccessIterator,typename Distance>
inline void __advance(RandomAccessIterator &i,Distance n,random_access_iterator_tag){
	i+=n;
}

template<typename InputIterator,typename Distance>
inline void advance(InputIterator &i,Distance n){
	__advance(i,n,iterator_category(i));
}

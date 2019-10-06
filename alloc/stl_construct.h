#include<new.h>

// 	value_type(begin) 用法 看起来像是函数调用	
// 	trival destructor() 带括号的原因


//构造
//使用placement new
template<typename T1,typename T2>
inline void construct(T1 *p,T2 &value){//注意T1为要构造的类型，T2为参数类型
	new(p) T1(value);
}

//析构第一版本
template<typename T>
inline void destory(T *p){
	p->~T();//直接调用析构函数
}

//第二个版本
//传入迭代器，进行类型推导，视类型情况进行析构
template<typename Iterator>
inline void destory(Iterator begin,Iterator end){
	//此处value_type(begin)的用法存疑
	_destory(begin,end,value_type(begin)); 
}


template<typename Iterator,typename T>
inline void _destory(Iterator begin,Iterator end,T){
	typedef typename _type_traits<T>::has_trivial_destructor trivial_destrcutor;
	//此处带括号的用法存疑
	_destory_aux(begin,end,trivial_destrcutor());//注意要带括号//通过这里分为两种情况
}


//一个一个删除
template<typename Iterator,typename T>
inline void _destory_aux(Iterator begin,Iterator end,_false_type){
	for(;begin!=end;begin++){
		destory(&*begin);
	}
}

//什么也不用做
template<typename Iterator,typename T>
inline void _destory_aux(Iterator begin,Iterator end,_true_type){
}

//以下为第二个版本的特化版本
inline void destory(char *,char*){}
inline void destory(wchar_t *,wchar_t *);

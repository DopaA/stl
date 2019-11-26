/*#include"stl_config.h"*/
#ifndef __TYPE_TRAITS_H_
#define __TYPE_TRAITS_H_

struct _true_type{};
struct _false_type{};
template<typename type>
struct _type_traits{
	//第一个，通知有能力将_type_traits特化的编译器这个是特殊的
	//不懂
	typedef _true_type  this_dummy_memeber_must_be_first;
	typedef _false_type has_trivial_default_constructor;
	typedef _false_type has_trivial_copy_constructor;
	typedef _false_type has_trivial_assigment_operator;
	typedef _false_type has_trivial_destructor;
	typedef _false_type is_POD_type;
};

template<>
struct _type_traits<char>{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assigment_operator;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};

template<>
 struct _type_traits<signed char>{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assigment_operator;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
template<>
 struct _type_traits<unsigned char>{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assigment_operator;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
template<>
 struct _type_traits<short>{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assigment_operator;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
template<>
 struct _type_traits<unsigned short>{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assigment_operator;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
template<>
 struct _type_traits<int>{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assigment_operator;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
template<>
 struct _type_traits<unsigned int>{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assigment_operator;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
template<>
 struct _type_traits<long>{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assigment_operator;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
template<>
 struct _type_traits<unsigned long>{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assigment_operator;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
template<>
 struct _type_traits<float>{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assigment_operator;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
template<>
 struct _type_traits<double>{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assigment_operator;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
template<>
 struct _type_traits<long double>{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assigment_operator;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};
template<typename T>
struct _type_traits<T *>{
	typedef _true_type has_trivial_default_constructor;
	typedef _true_type has_trivial_copy_constructor;
	typedef _true_type has_trivial_assigment_operator;
	typedef _true_type has_trivial_destructor;
	typedef _true_type is_POD_type;
};

#endif

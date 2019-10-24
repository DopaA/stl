#include"stl_iterator_base.h"

//output类型
template<typename Container>
class back_insert_iterator{
	protected:
		//what is container
		Container *container;
	public:
		//这个的作用
		typedef Container 				container_type;
		typedef output_iterator_tag 	iterator_category;
		typedef void						value_type;
		typedef void						difference_type;
		typedef void						pointer;
		typedef void 						reference;
		//构造函数，可以用来赋值
		explicit back_insert_iterator(Container &x)container(x){}
		//=的操作是插入
		back_insert_iterator<Container>&
			operator=(const typename Container::value_type &value){
				container->push_back(value);
				return *this;
			}
		//*和++操作什么都不做
		back_insert_iterator<Container>& operator*(){return *this}
		back_insert_iterator<Container>& operator++(){return *this}
		back_insert_iterator<Container>& operator++(int){return *this}
}

//类型判断
template<typename Container>
inline output_iterator_tag
iterator_category(const back_insert_iterator(Container &x)){
	return output_iterator_tag();
}
//构造迭代器
template<typename Container>
inline back_insert_iterator<Container> back_inserter(Container &x){
	return back_insert_iterator<Container>(x);
}

//-----------------------------------------------------------------------------------
//front_insert
template<typename Container>
class front_insert_iterator{
	protected:
		Container *container;
	public:
		typedef Container 				container_type;
		typedef output_iterator_tag 	iterator_category;
		typedef void						value_type;
		typedef void						difference_type;
		typedef void						pointer;
		typedef void 						reference;
		explicit front_insert_iterator(Container &x):container(&x){}
		front_insert_iterator<Container>&
		operator=(const typename Container::value_type &value){
			container->push_back(value);
			return *this;
		}
		front_insert_iterator<Container>& operator*(){return *this}
		front_insert_iterator<Container>& operator++(){return *this}
		front_insert_iterator<Container>& operator++(int){return *this}
}

template<typename Container>
inline output_iterator_tag
iterator_category(const front_insert_iterator<Container>&){
	return output_iterator_tag();
}

template<typename Container>
inline front_insert_iterator<Container> front_inserter(Container &x){
	return front_insert_iterator<Container>(x);
}

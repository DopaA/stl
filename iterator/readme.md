##### 迭代器设计思维
	stl的中心思想：将数据容器(container)和算法(algorithm)分开，彼此独立设计，最后以迭代器将他们撮合在一起。
	容器和算法的泛型化，可以通过C++的class templates和function templates分别达成目标。
	迭代器就是一种智能指针，指针最重要的就是内容提领(deference)和成员访问(member access),因此迭代器最重要的就是对operator*和operator->进行重载(overloading)。

#include"stl_construct.h"
#include"stl_alloc.h"
#include"stl_uninitialized.h"

template<typename T>
class auto_ptr{
private:
	T *pointer;
public:
	explicit auto_ptr(T *p=0): pointer(p){}
	template<typename U>
	auto_ptr(auto_ptr<U>&ptr)pointer(ptr.release()){}
	~auto_ptr(){delete pointer;}
	//赋值操作，将自身的pointer转换给另一个。
	template<typename U>
	auto_ptr<T>& operator=(auto_ptr<U> &ptr) {
		if(this!=ptr) reset(ptr.release());
		return *this;
	}
	T& operator*() const{return *pointer;}//此处const的作用
	T* operator->() const{return pointer;}
	T *release(){
		T *tmp=pointer;
		pointer=0;
		return tmp;
	}
	void reset(T *ptr=0){
		if(ptr!=0){
			delete pointer;
			pointer=ptr;
		}
	}
	T *get() const{return pointer;}
}

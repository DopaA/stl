#ifndef __STL_ALLOC_H_
#define __STL_ALLOC_H_
#include<new>
#include<stddef.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include<algorithm>

//配置的小于128bytes时采用第二级配置器，使用内存池

//单纯的转调用
template<typename T,typename Alloc>
class simple_alloc{
	public:
		static T *allocate(size_t n){
			return 0==n?0:(T*)Alloc::allocate(n*sizeof(T));
		}
		static T*allocate(void){
			return (T*)Alloc::allocate(sizeof(T));
		}
		static void deallocate(T*p,size_t n){
			if(n!=0)
				Alloc::deallocate(p,n*sizeof(n));
		}
		static void deallocate(T*p){
			Alloc::deallocate(p);
		}
};



#ifndef __THROW_BAD_ALLOC
#	if defined(__STD_NO_BAD_ALLOC) || !defined(__STL_USER_EXCEPTIONS)
#	include<stdio.h>
#	include<stdlib.h>
# 	define __THROW_BAD_ALLOC fprintf(stderr,"out of memory\n");exit(1)
#else
#	include<new>
#	define __THROW_BAD_ALLOC throw std::bad_alloc();
#	endif
#endif


template<int inst>
class __malloc_alloc_template{
	private:
		//内存不足的情况的处理函数指针
		static void *oom_malloc(size_t);
		static void *oom_realloc(void*p,size_t);
		static void (* __malloc_alloc_oom_handler)();
	public:
		static void *allocate(size_t n){
			void *result=malloc(n);
			if(0==result) result=oom_malloc(n);
			return result;
		}
		static void *deallocate(void *p,size_t ){
			free(p);//不用检测吗
		}
		static void *reallocate(void *p,size_t old,size_t newl){
			void *result=realloc(p,newl);
			if(0==result) oom_realloc(p,newl);
			return newl;
		}
		//仿真set_new_handler()
		//通过这个指定out-of-memory-handler
		static void (*set_malloc_handler(void(*f)()))(){
			void (*old)()=__malloc_alloc_oom_handler;
			__malloc_alloc_oom_handler=f;
			return (old);
		}
};

template<int inst>
void (*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)()=0;

template<int inst>
void *__malloc_alloc_template<inst>::oom_malloc(size_t n){
	void (*my_malloc_handler)();
	void *result;
	while(true){//while和for的区别，为什么会用for(;;)
		my_malloc_handler=__malloc_alloc_oom_handler;
		//if(my_malloc_handler==0) __THROW_BAD_ALLOC;
		__malloc_alloc_oom_handler();
		result=malloc(n);
		return result;
	}
}

template<int inst>
void *__malloc_alloc_template<inst>::oom_realloc(void *p,size_t n){
	void (*my_malloc_handler)();
	void *result;
	while(true){//while和for的区别，为什么会用for(;;)
		my_malloc_handler=__malloc_alloc_oom_handler;
		if(my_malloc_handler==0) __THROW_BAD_ALLOC;
		__malloc_alloc_oom_handler();
		result=realloc(p,n);
		return result;
	}
}


typedef __malloc_alloc_template<0> malloc_alloc;



//第二级配置器
//主要为内存池和union类型的链表
//小于128用第一层，否则第二层
enum {__ALIGIN=8};
enum {__MAX_BYTES=128};
enum {__LIST_COUNT=__MAX_BYTES/__ALIGIN};

template<bool threads,int inst>
class __default_alloc_template{
private:
	inline int ROUND_UP(size_t nbytes){//提升到8的倍数
		return (nbytes+__ALIGIN-1)&~(__ALIGIN-1);
	}
	union obj{
		union obj * freelist_link;
		char client_data[1];//为1的原因应该只是提供一下地址吧
	};
	static obj *volatile my_freelist[__LIST_COUNT];
	int FREELIST_INDEX(size_t nbytes){//查找索引
		return (nbytes+__ALIGIN-1)/(__ALIGIN-1);
	}
	static	void *refill(size_t n);
	static	char *chunk_alloc(size_t size,int& nobjs);//nobjs可变
	//内存池的起始和结束
	static char *start_free;
	static char *end_free;
	static size_t heap_size;
public:
	static	void *allocate(size_t n);
	static	void deallocate(void *p,size_t n);
	static	void *reallocate(void *p,size_t old,size_t newl);
};

//初始化
template<bool threads,int inst>
char* __default_alloc_template<threads,inst>::start_free=0;

template<bool threads,int inst>
char* __default_alloc_template<threads,inst>::end_free=0;

template<bool threads,int inst>
size_t __default_alloc_template<threads,inst>::heap_size=0;


//这样初始化没问题吗?
template<bool threads,int inst>
typename __default_alloc_template<threads,inst>::obj *volatile
__default_alloc_template<threads,inst>::my_freelist[__LIST_COUNT]=
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

template<bool threads,int inst>
void* __default_alloc_template<threads,inst>::allocate(size_t n){
	obj *result;
	obj *volatile free_list;//
	//大于128bytes用第一级配置器
	if(n>__MAX_BYTES){
		return malloc_alloc::allocate(n);
	}
	free_list=my_freelist+FREELIST_INDEX(n);
	result=*free_list;
	//如果没有了
	if(result==0){
		void *r=refill(ROUND_UP(n));
		return r;
	}
	*my_freelist=*free_list->freelist_link;
	return result;
}

//注意回收到free_list
template<bool threads,int inst>
void __default_alloc_template<threads,inst>::deallocate(void *p,size_t n){
	if(n>__MAX_BYTES){
		malloc_alloc::deallocate(p,n);
		return;
	}
	obj *volatile free_list;
	free_list=my_freelist+FREELIST_INDEX(n);
	obj *tmp=(obj*)p;
	tmp->freelist_link=*free_list;//my_freelist是链表中第一个的地址,也是数组的某个地址
	*free_list=tmp;
}

template<bool threads,int inst>
void* __default_alloc_template<threads,inst>::reallocate(void *p,size_t old,size_t newl){
	void *ret;
	if(old>__MAX_BYTES&&newl>__MAX_BYTES){//如果不是同时大于就用第二层？
		ret=malloc_alloc::reallocate(p,old,newl);
		return ret;
	}
	//由于分配都是8的倍数，所以如果两个转为8的倍数相等就不用操作了
	if(ROUND_UP(old)==ROUND_UP(newl))return p;
	ret=allocate(newl);
	int copy_size=newl>old?old:newl;
	memcpy(ret,p,copy_size);
	deallocate(p,old);
	return ret;
}


//用来重新填充
template<bool threads,int inst>
void *__default_alloc_template<threads,inst>::refill(size_t size){//此处的size已经ROUND_UP过
	int nobjs=20;//一次分配20个
	void* chunk=chunk_alloc(size,nobjs);
	if(1==nobjs){
		return chunk;
	}
	obj *volatile *freelist=my_freelist+FREELIST_INDEX(size);
	obj *current_obj,*next_obj;
	void *result=chunk;
	*freelist=current_obj=(obj*)(chunk+size);
	for(int i=1;i<nobjs;i++){
		current_obj=next_obj;
		next_obj=(obj*)((char*)next_obj+size);
		if(i=nobjs-1){
			current_obj->freelist_link=0;
		}else{
			current_obj->freelist_link=next_obj;
		}
	}
	return result;
}


//内存池管理
template<bool threads,int inst>
char *__default_alloc_template<threads,inst>::chunk_alloc(size_t n,int &nobjs){
	char *result;
	size_t bytes_toget=n*nobjs;
	size_t bytes_left=start_free-end_free;

	if(bytes_left>bytes_toget){//如果完全满足
		result=start_free;
		start_free+=bytes_toget;
		return result;
	}else if(bytes_left>n){//如果不够，但是有剩余
		nobjs=bytes_left/n;
		bytes_toget=nobjs*n;
		result=start_free;
		start_free+=bytes_toget;
		return result;
	}else{//完全没有
		//这里的heap_size的用处是什么
		size_t bytes_need=2*bytes_toget+ROUND_UP(heap_size>>4);
		//如果有零头，先用了..可以确定的是这里的零头一定是8的倍数
		if(bytes_left>0){
			obj *volatile *free_list=my_freelist+FREELIST_INDEX(bytes_left);
			((obj*)start_free)->freelist_link=*free_list;
			*free_list=(obj*)start_free;
		}
		start_free=(char*)malloc(bytes_need);
		//如果内存空间不够
		if(start_free==0){
			obj* volatile *free_list,*p;
			for(int i=n;i<=__MAX_BYTES;i+=__ALIGIN){
				free_list=my_freelist+FREELIST_INDEX(i);
				p=*free_list;
				if(*p!=0){
					*free_list=*free_list->freelist_link;
					start_free=(char*)p;
					end_free=start_free+i;
					return (chunk_alloc(n,nobjs));
				}
			}
			end_free=0;//都没内存了
			start_free=(char*)malloc_alloc::allocate(bytes_need);
		}
		heap_size+=bytes_need;
		end_free=start_free+bytes_need;
		return (chunk_alloc(n,nobjs));
	}
}


//确定是第一级配置器还是第二级配置器
#ifdef _USE_MALLOC
typedef _malloc_alloc_template<0> malloc_alloc;
typedef malloc_alloc alloc; //第一级配置器
#else
//__NODE_ALLOCATOR_THREADS为bool类型，这里暂时替换为false
typedef __default_alloc_template<false,0> alloc; //第二级配置器
#endif

#endif

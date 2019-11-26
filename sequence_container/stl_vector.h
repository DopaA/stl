#ifndef __STL_VECTOR_H_
#define __STL_VECTOR_H_
#include"../alloc/stl_alloc.h"
#include"../alloc/stl_construct.h"
#include"../alloc/stl_uninitialized.h"
#include<algorithm>
template<typename T, typename Alloc = alloc>
class vector
{
public:
	typedef T              value_type;
	typedef value_type*  pointer;
	typedef value_type*  iterator;//?
	typedef value_type&  reference;
	typedef size_t         size_type;
	typedef ptrdiff_t      difference_type;
protected:
	//定义自己的空间配置器
	typedef simple_alloc<value_type, Alloc> data_allocator;
	iterator start;
	iterator finish;//目前使用空间的尾
	iterator end_of_storage;//目前可用空间的尾
	void insert_aux(iterator position, const T& x); //为什么是引用类型。
	void deallocate()
	{
		if (start)
			data_allocator::deallocate(start, end_of_storage - start);
	}
	void fill_initialize(size_type n, const T& value)
	{
		start = allocate_and_fill(n, value);
		finish = start + n;
		end_of_storage = finish;
	}
public:
	iterator begin()
	{
		return start;
	}
	iterator end()
	{
		return finish;
	}
	size_type size()
	{
		return size_type(end() - begin());
	}
	size_type capacity() const
	{
		return size_type(end_of_storage() - begin());
	}
	bool empty()
	{
		return begin() == end();
	}
	reference operator[](size_type i)
	{
		return *(begin() + i);
	}

	vector(): start(0), finish(0), end_of_storage(0) {}
	vector(size_type n, const T&value)
	{
		fill_initialize(n, value);
	}
	vector(int n, const T&value)
	{
		fill_initialize(n, value);
	}
	vector(long n, const T&value)
	{
		fill_initialize(n, value);
	}
	explicit vector(size_type n)
	{
		fill_initialize(n, T());
	}
	~vector()
	{
		destory(start, finish); //先试着析构
		deallocate();//释放空间
	}
	void insert(iterator position, size_type n, const T& x);
	reference front()
	{
		return *begin();
	}
	reference back()
	{
		return *(end() - 1);
	}
	void push_back(const T&x)
	{
		if (finish != end_of_storage)
		{
			construct(finish, x);
			++finish;
		}
		else
		{
			insert_aux(end(), x);
		}
	}
	void pop_back()
	{
		--finish;
		destory(finish);
	}
	iterator erase(iterator position) //清除某位置上的元素
	{
		if (position + 1 != end())
			copy(position + 1, finish, position); //后续元素向前移动
		--finish;
		destory(finish);
		return position;
	}
	iterator erase(iterator first, iterator last) //清除[first,last)上的元素
	{
		iterator i = copy(last, finish, first); //全局函数
		destory(i, finish); //全局函数
		finish = finish - (last - first);
		return first;
	}
	void resize(size_type new_size, const T&x)
	{
		if (new_size < size)
			erase(begin() + new_size, end());
		else
			insert(end(), new_size - size(), x);
	}
	void resize(size_type new_size)
	{
		resize(new_size, T());
	}
	void clear()
	{
		erase(begin(), end());
	}
protected:
	iterator allocate_and_fill(size_type n, const T&x)
	{
		iterator result = data_allocator::allocate(n);
		uninitialized_fill_n(result, n, x);
		return result;
	}
};


template<typename T, typename Alloc>
void vector<T, Alloc>::insert_aux(iterator position, const T&x )
{
	if (finish != end_of_storage) //不是已经判断过了?
	{
		construct(finish, *(finish - 1));
		++finish;
		T x_copy = x;
		copy_backward(position, finish - 2, finish - 1);
		*position = x_copy;
	}
	else
	{
		const size_type old_size = size();
		const size_type new_size = old_size != 0 ? 2 * old_size : 1;
		iterator new_start = data_allocator::allocate(new_size);
		iterator new_finish = new_start;
		try
		{
			new_finish = uninitialized_copy(start, position, new_start);
			construct(new_finish, x);
			++new_finish;
			//将备用空间中的内容也拷贝过来
			new_finish = uninitialized_copy(position, finish, new_finish);
		}
		catch (...)
		{
			destory(new_start, new_finish);
			data_allocator::deallocate(new_start, new_size);
			throw;
		}
		destory(begin(), end());
		deallocate();
		start = new_start;
		finish = new_finish;
		end_of_storage = start + new_size;
	}
}

//从position开始，插入n个元素，元素初值为x
template<typename T, typename Alloc>
void vector<T, Alloc>::insert(iterator position, size_type n, const T& x)
{
	if (n != 0)
	{
		//如果备用空间够用
		if (size_type(end_of_storage - finish) >= n)
		{
			T x_copy = x;
			const size_type elems_after = finish - position;
			iterator old_finish = finish;
			//如果插入点后的元素个数大于新增元素个数
			//可以直接后移n个位置
			//两个的前提都是备用空间大于要插入的元素个数
			if (elems_after > n)
			{
				uninitialized_copy(finish - n, finish, finish);
				finish += n;
				copy_backward(position, old_finish - n, old_finish);
				fill(position, position + n, x_copy);
			}
			else
			{
				uninitialized_fill_n(finish, n - elems_after, x_copy);
				finish += n - elems_after;
				uninitialized_copy(position, old_finish, finish);
				finish += elems_after;
				fill(position, old_finish, x_copy);
			}
		}
		else
		{
			const size_type old_size = size();
			const size_type max_two = old_size > n ? old_size : n;
			const size_type new_size = old_size + max_two;
			iterator new_start = data_allocator::allocate(new_size);
			iterator new_finish = new_start;
			try
			{
				new_finish = uninitialized_copy(start, position, new_start);
				new_finish = uninitialized_fill_n(new_finish, n, x);
				new_finish = uninitialized_copy(position, finish, new_finish);
			}
			catch (...)
			{
				destory(new_start, new_finish);
				data_allocator::deallocate(new_start, new_size);
				throw;
			}
			/*
			#ifdef __STL_USE_EXCEPTIONS
			    catch(...){//如果有异常发生，实现commit and rollback
			       destory(new_start,new_finish);
			       data_allocator::deallocate(new_start,new_size);
			       throw;
			    }
			#endif
			*/
			destory(start, finish);
			deallocate();
			start = new_start;
			finish = new_finish;
			end_of_storage = start + new_size;
		}
	}
}
#endif

#include <iostream>
#include <new>

/*
* BaseAllocator is just a pack of operator new and operator delete
*/
template <typename T>
class BaseAllocator
{

public:
    using ValueType = T;
    using Reference = T &;
    using Pointer = T *;
    using SizeType = std::size_t;
    using DifferenceType = std::ptrdiff_t;

public:
    Pointer Allocate(SizeType Num)
    {
        set_new_handler(nullptr);
        T *Tmp = (T *)(::operator new(Size * sizeof(T)));
        if (Tmp == nullptr)
        {
            std::cerr << "out of memory" << std::endl;
            exit(1);
        }
        return Tmp;
    }

    void DeAllocate(Pointer Mem)
    {
        ::operator delete(Mem);
    }
};

template <typename T>
class DefaultAllocator
{

public:
    using ValueType = T;
    using Reference = T &;
    using Pointer = T *;
    using SizeType = std::size_t;
    using DifferenceType = std::ptrdiff_t;
}
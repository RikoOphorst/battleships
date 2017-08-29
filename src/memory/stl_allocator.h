#pragma once

#include "../memory/allocator.h"

namespace igad
{
	namespace memory
	{
		template <typename T>
		class StlAllocator : public std::allocator<T>
		{
		public:
			typedef size_t size_type;
			typedef T* pointer;
			typedef const T* const_pointer;

			template<typename _Tp1>
			struct rebind
			{
				typedef StlAllocator<_Tp1> other;
			};

			pointer allocate(size_type n, const void *hint = 0)
			{
				return MemoryManager::AllocateArray<T>(allocator_, n);
			}

			void deallocate(pointer p, size_type n)
			{
				return MemoryManager::DeallocateArray<T>(allocator_, p);
			}

			Allocator* GetAllocator()
			{
				return allocator_;
			}

			Allocator* GetAllocator() const
			{
				return allocator_;
			}

			StlAllocator(Allocator* allocator) throw() : std::allocator<T>() 
			{ 
				allocator_ = allocator;
			}
			StlAllocator(const StlAllocator &a) throw() : std::allocator<T>(a) 
			{ 
				allocator_ = a.GetAllocator();
			}
			template <class U>
			StlAllocator(const StlAllocator<U> &a) throw() : std::allocator<T>(a) 
			{
				allocator_ = a.GetAllocator();
			}
			~StlAllocator() throw() 
			{
				
			}
		private:
			Allocator* allocator_;
		};
	}
}
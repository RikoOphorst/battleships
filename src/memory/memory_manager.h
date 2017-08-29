#pragma once

#define BB_MEMORY_SIZE 100000000

#include "../memory/free_list_allocator.h"
#include "../memory/linear_allocator.h"
#include "../memory/stack_allocator.h"
#include "../memory/pool_allocator.h"

namespace igad
{
	namespace memory
	{
		class MemoryPool;
		class FreeListAllocator;
		class LinearAllocator;
		class Allocator;
		
		class MemoryManager
		{
		private:
			MemoryManager(MemoryPool* memory_pool);
		public:
			~MemoryManager();
			static MemoryManager* Create();
			static MemoryManager* Instance();

			template<typename T, typename...Args>
			static T* Allocate(Allocator* allocator, Args...args);

			template<typename T, typename...Args>
			static T* AllocateArray(Allocator* allocator, const size_t& length, Args...args);

			template<typename T>
			static void Deallocate(Allocator* allocator, T* ptr);

			template<typename T>
			static void DeallocateArray(Allocator* allocator, T* arr);

			template<typename T>
			T* ConstructAllocator(const size_t& size);

			template<typename T, typename B>
			T* ConstructAllocator(const size_t& size);

			void DestructAllocator(Allocator* allocator);
		private:
			FreeListAllocator* all_allocators_;
			FreeListAllocator* memory_allocator_;
			MemoryPool* memory_pool_;

			static MemoryManager* instance_;
		};

		template<typename T, typename...Args>
		inline T* MemoryManager::Allocate(Allocator* allocator, Args...args)
		{
			return new (allocator->Allocate(sizeof(T), __alignof(T))) T(args...);
		}

		template<typename T, typename...Args>
		inline T* MemoryManager::AllocateArray(Allocator* allocator, const size_t& length, Args...args)
		{
			BB_ASSERT(length > 0, "Length of the array needs to be at least 1.");

			uint32_t header_size = sizeof(size_t) / sizeof(T); 
			
			if (sizeof(size_t) % sizeof(T) > 0)
				header_size += 1;

			T* ptr = ((T*) allocator->Allocate(sizeof(T) * (length + header_size), __alignof(T))) + header_size;

			*(((size_t*)ptr) - 1) = length;

			for (int i = 0; i < length; i++)
			{
				new (&ptr[i]) T(args...);
			}

			return ptr;
		}

		template<typename T>
		inline void MemoryManager::Deallocate(Allocator* allocator, T* ptr)
		{
			ptr->~T();
			allocator->Deallocate(ptr);
		}

		template<typename T>
		inline void MemoryManager::DeallocateArray(Allocator* allocator, T* arr)
		{
			size_t length = *(((size_t*)arr) - 1);

			for (int i = 0; i < length; i++)
			{
				arr[i].~T();
			}

			uint32_t header_size = sizeof(size_t) / sizeof(T);

			if (sizeof(size_t) % sizeof(T) > 0)
				header_size += 1;

			allocator->Deallocate(arr - header_size);
		}

		template<typename T>
		inline T* MemoryManager::ConstructAllocator(const size_t& size)
		{
			void* new_alloc = all_allocators_->Allocate(sizeof(T), __alignof(T));
			void* new_memory_zone = memory_allocator_->Allocate(size);

			return new (new_alloc)T(new_memory_zone, size);
		}

		template<typename T, typename B>
		inline T* MemoryManager::ConstructAllocator(const size_t& size)
		{
			void* new_alloc = all_allocators_->Allocate(sizeof(T), __alignof(T));
			void* new_memory_zone = memory_allocator_->Allocate(size);

			return new (new_alloc)T(new_memory_zone, size, sizeof(B), __alignof(B));
		}
	}
}
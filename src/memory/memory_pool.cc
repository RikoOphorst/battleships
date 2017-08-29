#include "memory_pool.h"

#include "../memory/pointer_util.h"

#include <iostream>

namespace igad
{
	namespace memory
	{
		//------------------------------------------------------------------------------------------------------
		MemoryPool::MemoryPool(void* start, const size_t& size) :
			memory_pool_(start),
			size_(size),
			start_offset_(0)
		{

		}

		//------------------------------------------------------------------------------------------------------
		MemoryPool::~MemoryPool()
		{
			
		}

		//------------------------------------------------------------------------------------------------------
		MemoryPool* MemoryPool::Create(const size_t& size)
		{
			void* memory = malloc(size);

			void* aligned_address = PointerUtil::AlignForward(memory, __alignof(MemoryPool));
			uint32_t adjustment = PointerUtil::AlignForwardAdjustment(memory, __alignof(MemoryPool));

			MemoryPool* memory_pool = new (aligned_address) MemoryPool(memory, size);
			memory_pool->SetStartOffset(adjustment + sizeof(MemoryPool));

			return memory_pool;
		}

		//------------------------------------------------------------------------------------------------------
		void* MemoryPool::GetStartOfMemoryPool()
		{
			return (void*)((uintptr_t)memory_pool_ + (uintptr_t)start_offset_);
		}

		//------------------------------------------------------------------------------------------------------
		void* MemoryPool::GetActualStartOfMemoryPool()
		{
			return memory_pool_;
		}

		//------------------------------------------------------------------------------------------------------
		const size_t& MemoryPool::GetSizeOfMemoryPool()
		{
			return size_;
		}
		
		//------------------------------------------------------------------------------------------------------
		const size_t& MemoryPool::GetStartOffset()
		{
			return start_offset_;
		}
		
		//------------------------------------------------------------------------------------------------------
		void MemoryPool::SetStartOffset(const size_t& offset)
		{
			start_offset_ = offset;
		}
	}
}
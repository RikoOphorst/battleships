#pragma once

#include "allocator.h"

namespace igad
{
	namespace memory
	{
		class FreeListAllocator : public Allocator
		{
		public:
			FreeListAllocator(void* start, const size_t& size);
			~FreeListAllocator();
			
			void* Allocate(const size_t& size, const uint32_t& alignment = 4);
			void Deallocate(void* ptr);
		private:
			struct AllocationHeader
			{
				size_t size;
				uint8_t adjustment;
			};

			struct FreeBlock
			{
				size_t size;
				FreeBlock* next;
			};

			FreeBlock* first_block_;
		};
	}
}
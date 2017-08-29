#pragma once

#include "allocator.h"

namespace igad
{
	namespace memory
	{
		class StackAllocator : public Allocator
		{
		public:
			StackAllocator(void* start, const size_t& size);
			~StackAllocator();

			void* Allocate(const size_t& size, const uint32_t& alignment = 4);
			void Deallocate(void* ptr);
		private:
			struct AllocationHeader
			{
				uint32_t adjustment;
				void* prev_alloc;
			};

			void* top_;
			void* prev_alloc_;
		};
	}
}
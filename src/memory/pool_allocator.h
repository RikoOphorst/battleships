#pragma once

#include "../memory/allocator.h"

namespace igad
{
	namespace memory
	{
		class PoolAllocator : public Allocator
		{
		public:
			PoolAllocator(void* start, const size_t& size, const size_t& object_size, const uint32_t& object_alignment);
			~PoolAllocator();

			void* Allocate(const size_t& size, const uint32_t& alignment = 4);
			void Deallocate(void* ptr);
		private:
			size_t object_size_;
			uint32_t object_alignment_;

			void** free_list_;
		};
	}
}
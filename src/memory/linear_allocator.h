#pragma once

#include "allocator.h"

namespace igad
{
	namespace memory
	{
		class LinearAllocator : public Allocator
		{
		public:
			LinearAllocator(void* start, const size_t& size);
			~LinearAllocator();

			void* Allocate(const size_t& size, const uint32_t& alignment = 4);
			void Deallocate(void* ptr);
		private:
			void* marker_;
		};
	}
}
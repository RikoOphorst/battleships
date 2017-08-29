#include "stack_allocator.h"

#include "../memory/pointer_util.h"
#include "../util/defines.h"

namespace igad
{
	namespace memory
	{
		StackAllocator::StackAllocator(void* start, const size_t& size) :
			Allocator(start, size),
			top_(start),
			prev_alloc_(nullptr)
		{

		}

		StackAllocator::~StackAllocator()
		{
		
		}

		void* StackAllocator::Allocate(const size_t& size, const uint32_t& alignment)
		{
			BB_ASSERT(size != 0 && alignment != 0, "Size and/or alignment cannot be null");
			
			uint32_t adjustment = PointerUtil::AlignForwardAdjustmentWithHeader(top_, alignment, sizeof(AllocationHeader));

			BB_ASSERT(used_memory_ + size + adjustment < size_, "Not enough space left in StackAllocator");
			
			uintptr_t aligned_address = (uintptr_t)top_ + adjustment;

			AllocationHeader* header = (AllocationHeader*)(aligned_address - sizeof(AllocationHeader));
			header->prev_alloc = prev_alloc_;
			header->adjustment = adjustment;

			prev_alloc_ = (void*)aligned_address;
			top_ = (void*)(aligned_address + size);

			used_memory_ += size + adjustment;
			num_allocations_++;

			return (void*)aligned_address;
		}

		void StackAllocator::Deallocate(void* ptr)
		{
			BB_ASSERT(ptr == prev_alloc_, "You can only deallocate the last allocated object in this allocator");

			AllocationHeader* header = (AllocationHeader*)(PointerUtil::Subtract(ptr, sizeof(AllocationHeader)));

			used_memory_ -= (uintptr_t)top_ - (uintptr_t)ptr + header->adjustment;
			num_allocations_--;

			top_ = PointerUtil::Subtract(ptr, header->adjustment);
			prev_alloc_ = header->prev_alloc;
		}
	}
}
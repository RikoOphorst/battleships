#include "free_list_allocator.h"

#include "../memory/pointer_util.h"
#include "../util/defines.h"

namespace igad
{
	namespace memory
	{
		FreeListAllocator::FreeListAllocator(void* start, const size_t& size) :
			Allocator(start, size),
			first_block_((FreeBlock*)start)
		{
			BB_ASSERT(size > 0, "An allocator of size 0 is not possible.");

			first_block_->size = size;
			first_block_->next = nullptr;
		}

		FreeListAllocator::~FreeListAllocator()
		{
			first_block_ = nullptr;
		}

		void* FreeListAllocator::Allocate(const size_t& size, const uint32_t& alignment)
		{
			BB_ASSERT(size != 0 && alignment != 0, "Size and/or alignment of the block to be allocated are zero, which is not possible.");

			FreeBlock* previous_block = nullptr;
			FreeBlock* current_block = first_block_;

			while (current_block != nullptr)
			{
				uint32_t adjustment = PointerUtil::AlignForwardAdjustmentWithHeader(current_block, alignment, sizeof(AllocationHeader));

				size_t total_size = size + adjustment;

				if (current_block->size < total_size)
				{
					previous_block = current_block;
					current_block = current_block->next;
					continue;
				}

				if (current_block->size - total_size <= sizeof(AllocationHeader))
				{
					// no further allocations are possible within the size of the current block, so increase the total size of the allocation to the size of the current block
					total_size = current_block->size;

					// this eliminates the current block from the linked list so that it is no longer considered for allocation
					if (previous_block != nullptr)
					{
						previous_block->next = current_block->next;
					}
					else
					{
						first_block_ = current_block->next;
					}
				}
				else
				{
					// more allocations are possible with the remaining memory of the current block, so we split it up

					FreeBlock* next_block = (FreeBlock*)(PointerUtil::Add(current_block, (uint32_t)total_size));
					next_block->size = current_block->size - total_size;
					next_block->next = current_block->next;

					if (previous_block != nullptr)
					{
						previous_block->next = next_block;
					}
					else
					{
						first_block_ = next_block;
					}
				}

				uintptr_t aligned_address = (uintptr_t)current_block + adjustment;

				AllocationHeader* header = (AllocationHeader*)(aligned_address - sizeof(AllocationHeader));
				header->size = total_size;
				header->adjustment = adjustment;

				used_memory_ += total_size;
				num_allocations_++;

				BB_ASSERT(PointerUtil::AlignForwardAdjustment((void*)aligned_address, alignment) == 0, "Somewhere the alignments of the memory block got messed up.");

				return (void*)aligned_address;
			}

			BB_ASSERT(false, "There is not enough space in the free list allocator left or the allocator is too fragmented to be used again.");
			return nullptr;
		}

		void FreeListAllocator::Deallocate(void* ptr)
		{
			BB_ASSERT(ptr != nullptr, "Cannot deallocate a null pointer.");

			AllocationHeader* header = (AllocationHeader*)(PointerUtil::Subtract(ptr, sizeof(AllocationHeader)));

			uintptr_t block_start = (uintptr_t)ptr - header->adjustment;
			size_t block_size = header->size;
			uintptr_t block_end = block_start + block_size;

			FreeBlock* previous_block = nullptr;
			FreeBlock* current_block = first_block_;

			while (current_block != nullptr)
			{
				if ((uintptr_t)current_block >= block_end)
				{
					break;
				}

				previous_block = current_block;
				current_block = current_block->next;
			}

			if (previous_block == nullptr)
			{
				previous_block = (FreeBlock*)block_start;
				previous_block->size = block_size;
				previous_block->next = first_block_;

				first_block_ = previous_block;
			}
			else if ((uintptr_t)previous_block + previous_block->size == block_start)
			{
				previous_block->size += block_size;
			}
			else
			{
				FreeBlock* temp = (FreeBlock*)block_start;
				temp->size = block_size;
				temp->next = previous_block->next;
				previous_block->next = temp;
				
				previous_block = temp;
			}

			if (current_block != nullptr && (uintptr_t)current_block == block_end)
			{
				previous_block->size += current_block->size;
			}

			num_allocations_--;
			used_memory_ -= block_size;
		}
	}
}
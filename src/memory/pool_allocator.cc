#include "pool_allocator.h"

#include "../memory/pointer_util.h"
#include "../util/defines.h"

namespace igad
{
	namespace memory
	{
		PoolAllocator::PoolAllocator(void* start, const size_t& size, const size_t& object_size, const uint32_t& object_alignment) :
			Allocator(start, size),
			object_size_(object_size),
			object_alignment_(object_alignment)
		{
			BB_ASSERT(object_size >= sizeof(void*), "The size of an object in a pool allocator has to be at least the size of a void* (usually 8 bytes).");

			uint32_t adjustment = PointerUtil::AlignForwardAdjustment(start_, object_alignment_);

			// The rest of this function is very complex, I will try my best to explain how this all works

			// We offset (align) the starting point of the memory pool that was assigned to this allocator.
			// Then we convert the aligned pointer to a void**, meaning it a a pointer that points to other pointers.
			// We then store the pointer in the allocator's memory, no memory pool manipulation has been done at this point.
			free_list_ = (void**)PointerUtil::Add(start_, adjustment);
			
			// Calculate the number of objects that fit in the pool. Subtract the adjustment from the size,
			// because we have just positively aligned the memory pool.
			size_t num_objects = (size_ - adjustment) / object_size_;

			// This is where things get real complex. Hang in there with me.
			// We create a new void** which is going to act as a memory block in the pool.
			// It is initialized as a copy of the start of the memory pool.
			// At this point, the memory pool and the free_list_ are not manipulated at all.
			void** new_block = free_list_;

			// Since we want to create num_objects blocks in the pool, we iterate
			// for all of them in the loop, and consequently create them.
			for (size_t i = 0; i < num_objects - 1; i++)
			{
				// The lvalue here is *new_block - this means that it directly "accesses" the void* that
				// the current new_block is pointing to. Initially, this is still the start of the memory pool.
				// After that, it is the point at which the free_list_ is at (the newest block in the list)
				// --
				// The rvalue here is a void*, which is calculated by taking the newest block in the list,
				// and adding the object size to it. Note: it is not required to align this block,
				// because objects always have a size of power of 2, which means that if the first block
				// is aligned and the rest is contiguously added after the first block, they are automatically
				// aligned properly.
				// --
				// The result will be that the newest block in the list (at this point, the previous one)
				// will have its pointer point to the next block - like a linked list, just way more abstract.
				*new_block = PointerUtil::Add(new_block, (uint32_t)object_size_);

				// The lvalue here is new_block. It is a void**, and stored in the allocators memory.
				// This is NOT in the memory pool itself, which is easily mistaken. At this point,
				// before the assignment executes, it is still the previous block in the list.
				// -- 
				// The rvalue here is a tad more complicated. What we want, is the pointer we have
				// created in the previous instruction, and set that as the new block in the list.
				// It is calculated by accessing the pointer itself we have just assigned with *new_block.
				// We then convert it to a block in the list, by converting it to a void**.
				// --
				// This results in that the new_block is indeed a new block in the linked list, pointing
				// to a pointer that points to the correct memory block. It is automatically in the linked list,
				// and the next iteration of the loop will take care of where it should point to.
				// If nothing tells it where it should point to, this will cause undefined behaviour.
				new_block = (void**)*new_block;
			}

			// Right now, new_block is the newest block in the entire list. If the loop executed properly,
			// the newest block points to unknown memory. Because it is the newest block, and also the "last"
			// block of the list, it should point to a nullptr, to signal that there are no more new blocks in
			// the list left.
			*new_block = nullptr;
		}

		PoolAllocator::~PoolAllocator()
		{
			free_list_ = nullptr;
		}

		void* PoolAllocator::Allocate(const size_t& size, const uint32_t& alignment)
		{
			BB_ASSERT(size == object_size_ && alignment == object_alignment_, "Object with wrong size or alignment is trying to be aligned in the allocator, which is not possible.");
			BB_ASSERT(free_list_ != nullptr, "There is not enough memory left in the pool.");

			// The address at which the object should be allocated is equal to what the first
			// block in the free list points to.
			void* address = free_list_;

			// We need to set the free list to what currently the first block in the list is
			// pointing to. Therefore, we dereference the first block in the free list (*free_list_),
			// and then convert that to a void**, which is in effect a linked list.
			free_list_ = (void**)*free_list_;

			used_memory_ += size;
			num_allocations_++;

			return address;
		}

		void PoolAllocator::Deallocate(void* ptr)
		{
			// The lvalue here is complicated. It takes the ptr we want to deallocate,
			// converts that to a void** - which is a pointer linked list, and the same
			// format we use for our free_list. Then, we want to dereference that, so
			// that we can set what the block is pointing to.
			// --
			// The rvalue here is just the current free_list_, which is at the PREVIOUS
			// block in the list compared to the one we want to deallocate.
			// --
			// The result here is that the memory at the given ptr is now changed so that
			// it points to the previous (compared to the given ptr) point in the list.
			// The free list itself has NOT been changed yet, we have just overwritten
			// the memory that is under ptr.
			*((void**)ptr) = free_list_;

			// The lvalue here is the free list, which lives in the allocators memory,
			// NOT in the memory pool that has been assigned for this allocator.
			// --
			// The rvalue here is just the ptr, which then gets converted to a void**.
			// This will have the effect that it is now in essense a linked list, because
			// every pointer that is pointed to points to the next pointer.
			// --
			// The result this has, is that the free list now starts at the given ptr.
			// This means that whenever a new allocation occurs, it will start with the one
			// we have last deallocated.
			free_list_ = (void**)ptr;

			used_memory_ -= object_size_;
			num_allocations_--;
		}
	}
}
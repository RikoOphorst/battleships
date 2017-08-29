#pragma once

#include <memory>

namespace igad
{
	namespace memory
	{
		class Allocator
		{
		public:
			Allocator(void* start, const size_t& size);
			virtual ~Allocator();

			virtual void* Allocate(const size_t& size, const uint32_t& alignment = 4) = 0;
			virtual void Deallocate(void* ptr) = 0;

			void* GetStart();
			const size_t& GetSize();

			const size_t& GetUsedMemory();
			const size_t& GetNumAllocations();

			void SetHeapInspectorNotifications(const bool& enabled);
			const bool& GetHeapInspectorNotifications();
		protected:
			void* start_;
			size_t size_;

			size_t used_memory_;
			size_t num_allocations_;

			bool heap_inspector_notifications_enabled_;
		};
	}
}
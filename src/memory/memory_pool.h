#pragma once

#include <new>

namespace igad
{
	namespace memory
	{
		class MemoryPool
		{
		private:
			MemoryPool(void* start, const size_t& size);
			~MemoryPool();

		public:
			static MemoryPool* Create(const size_t& size);

			void* GetStartOfMemoryPool();
			void* GetActualStartOfMemoryPool();

			const size_t& GetSizeOfMemoryPool();

			const size_t& GetStartOffset();
			void SetStartOffset(const size_t& offset);

		private:
			size_t start_offset_;
			size_t size_;
			void* memory_pool_;
		};
	}
}
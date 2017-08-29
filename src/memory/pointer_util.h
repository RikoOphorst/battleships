#pragma once

#include <memory>
#include <string>
#include <bitset>

namespace igad
{
	namespace memory
	{
		class PointerUtil
		{
		public:
			template<typename T>
			static std::string ConvertToBinaryString(T val);

			static void* AlignForward(void* address, const uint32_t& alignment);
			static const void* AlignForward(const void* address, const uint32_t& alignment);
			
			static void* AlignBackward(void* address, const uint32_t& alignment);
			static const void* AlignBackward(const void* address, const uint32_t& alignment);

			static uint32_t AlignForwardAdjustment(const void* address, const uint32_t& alignment);
			static uint32_t AlignForwardAdjustmentWithHeader(const void* address, const uint32_t& alignment, const uint32_t& header_size);
			
			static uint32_t AlignBackwardAdjustment(const void* address, const uint32_t& alignment);

			static void* Add(void* ptr, const uint32_t& value);
			static const void* Add(const void* ptr, const uint32_t& value);
			static void* Subtract(void* ptr, const uint32_t& value);
			static const void* Subtract(const void* ptr, const uint32_t& value);
		};


		template<typename T>
		inline std::string PointerUtil::ConvertToBinaryString(T val)
		{
			std::string result = "";

			const char* beg = reinterpret_cast<const char*>(&val);
			const char* end = beg + sizeof(val);
			while (beg != end)
			{
				result += std::bitset<8>(*beg++).to_string() + ' ';
			}

			return result;
		}
	}
}
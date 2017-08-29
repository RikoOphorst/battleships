#pragma once

#include "../../memory/allocator.h"

namespace igad
{
	namespace memory
	{
		/**
		* @class blowbox::RefCount
		* @brief Keeps the count of references a pointer has
		* @author Riko Ophorst
		*/
		template<typename T>
		class RefCount
		{
		public:
			/**
			* @brief Default RefCount constructor
			*/
			RefCount() :
				allocator_(nullptr),
				references_(1),
				ptr_(nullptr)
			{
				int i = 0;
			}

			/**
			* @brief Default RefCount constructor with custom pointer
			*/
			RefCount(T* ptr, Allocator* allocator) :
				references_(1),
				ptr_(ptr),
				allocator_(allocator)
			{
				int i = 0;
			}

			/**
			* @brief Default RefCount destructor
			*/
			~RefCount();

			/**
			* @brief Increases the refcount
			*/
			inline void Increase()
			{
				++references_;
			}

			/**
			* @brief Decreases the refcount and destroys if necessary
			*/
			inline void Decrease()
			{
				if (--references_ == 0)
					Destroy();
			}

			/**
			* @brief Destroys the pointer and decreases the allocated memory
			*/
			inline void Destroy()
			{
				ptr_->~T();
				allocator_->Deallocate(ptr_);

				allocator_->Deallocate(this);
			}
		private:
			Allocator* allocator_;
			T* ptr_;
			unsigned int references_;
		};
	}
}
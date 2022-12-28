module;

#include <bx/allocator.h>

export module allocators;

export namespace annileen
{
	class Allocators final
	{
	private:
		static bx::DefaultAllocator s_Allocator;

	public:
		static bx::DefaultAllocator* getAllocator() { return &s_Allocator; }

		static void releaseMem(void* _ptr, void* _userData) { delete[] _ptr; }
	};
}

bx::DefaultAllocator annileen::Allocators::s_Allocator{};
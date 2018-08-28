#pragma once

#include <windows.h>
#include <memory>

class VMTHook
{
public:
	VMTHook( void* ppClass );
	~VMTHook();

	auto Hook( const std::size_t index, void* fnNew ) -> HRESULT;
	auto Unhook( const std::size_t index ) -> HRESULT;

	template< class Type >
	auto GetOriginal( const std::size_t index ) -> Type
	{
		return reinterpret_cast< Type >( this->_pOriginalVMT[ index ] );
	};

private:
	std::unique_ptr< std::uintptr_t[] > _pNewVMT = nullptr;
	std::uintptr_t** _ppBaseClass = nullptr;
	std::uintptr_t* _pOriginalVMT = nullptr;
	std::size_t _indexCount = 0;
};
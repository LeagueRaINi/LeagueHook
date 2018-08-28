#include "VMTHook.h"

VMTHook::VMTHook( void* ppClass )
{
	this->_ppBaseClass = static_cast< std::uintptr_t** >( ppClass );

	while ( static_cast< std::uintptr_t* >( *this->_ppBaseClass )[ this->_indexCount ] )
		++this->_indexCount;

	const std::size_t kSizeTable = this->_indexCount * sizeof( std::uintptr_t );

	this->_pOriginalVMT = *this->_ppBaseClass;
	this->_pNewVMT = std::make_unique< std::uintptr_t[] >( this->_indexCount );

	std::memcpy( this->_pNewVMT.get(), this->_pOriginalVMT, kSizeTable );

	*this->_ppBaseClass = this->_pNewVMT.get();
}

VMTHook::~VMTHook()
{
	*this->_ppBaseClass = this->_pOriginalVMT;
}

auto VMTHook::Hook( const std::size_t index, void* fnNew ) -> HRESULT
{
	if ( index > this->_indexCount )
		return E_INVALIDARG;

	this->_pNewVMT[ index ] = reinterpret_cast< std::uintptr_t >( fnNew );

	return S_OK;
}

auto VMTHook::Unhook( const std::size_t index ) -> HRESULT
{
	if ( index > this->_indexCount )
		return E_INVALIDARG;

	this->_pNewVMT[ index ] = this->_pOriginalVMT[ index ];

	return S_OK;
}

#include "Vector.h"

#include <sstream>

Vector::Vector()
{
	Invalidate();
}

Vector::Vector( const float x, const float y, const float z )
{
	this->Init( x, y, z );
}

Vector::Vector( const float* clr )
{
	this->Init( clr[ 0 ], clr[ 1 ], clr[ 2 ] );
}

Vector::Vector( const Vector& vec )
{
	this->Init( vec.x, vec.y, vec.z );
}

auto Vector::Init( const float x, const float y, const float z ) -> void
{
	this->x = x;
	this->y = y;
	this->z = z;
}

auto Vector::IsValid() const -> bool
{
	return std::isfinite( this->x )
		&& std::isfinite( this->y )
		&& std::isfinite( this->z );
}

auto Vector::Invalidate() -> void
{
	this->x = this->y = this->z = std::numeric_limits< float >::infinity();
}

auto Vector::IsZero() const -> bool
{
	return !this->x
		&& !this->y
		&& !this->z;
}

auto Vector::Zero() -> void
{
	this->x = this->y = this->z = 0.0f;
}

auto Vector::NormalizeInPlace() -> void
{
	*this = this->Normalized();
}

auto Vector::Normalized() const -> Vector
{
	Vector result = *this;
	const float lengh = result.Length();

	if ( lengh != 0.0f )
	{
		result /= lengh;
	}
	else
	{
		result.x = result.y = result.z = 0.0f;
	}

	return result;
}

auto Vector::DistTo( const Vector& vec ) const -> float
{
	return ( *this - vec ).Length();
}

auto Vector::DistToSqr( const Vector& vec ) const -> float
{
	return ( *this - vec ).LengthSqr();
}

auto Vector::Dot( const Vector& vec ) const -> float
{
	return this->x * vec.x + this->y * vec.y + this->z * vec.z;
}

auto Vector::CrossProduct( const Vector& vec_src, const Vector& vec_src2 ) -> Vector
{
	return Vector(
		vec_src.y * vec_src2.z - vec_src.z * vec_src2.y,
		vec_src.z * vec_src2.x - vec_src.x * vec_src2.z,
		vec_src.x * vec_src2.y - vec_src.y * vec_src2.x );
}

auto Vector::Length() const -> float
{
	return float( sqrt( this->x * this->x + this->y * this->y + this->z * this->z ) );
}

auto Vector::LengthSqr() const -> float
{
	return this->x * this->x + this->y * this->y + this->z * this->z;
}

auto Vector::Length2D() const -> float
{
	return float( sqrt( this->x * this->x + this->y * this->y ) );
}

VectorAligned::VectorAligned( float x, float y, float z )
{
	this->Init( x, y, z );
}

VectorAligned::VectorAligned( const Vector& vec )
{
	this->Init( vec.x, vec.y, vec.z );
}

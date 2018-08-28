#include "Vector2D.h"

#include <sstream>

Vector2D::Vector2D()
{
	this->Invalidate();
}

Vector2D::Vector2D( float x, float y )
{
	this->Init( x, y );
}

Vector2D::Vector2D( float* clr )
{
	this->Init( clr[ 0 ], clr[ 1 ] );
}

Vector2D::Vector2D( const Vector2D& vec_2d )
{
	this->Init( vec_2d.x, vec_2d.y );
}

auto Vector2D::Init( float x, float y ) -> void
{
	this->x = x;
	this->y = y;
}

auto Vector2D::IsValid() const -> bool
{
	return std::isfinite( this->x )
		&& std::isfinite( this->y );
}

auto Vector2D::Invalidate() -> void
{
	this->x = this->y = std::numeric_limits< float >::infinity();
}

auto Vector2D::IsZero() const -> bool
{
	return !this->x
		&& !this->y;
}

auto Vector2D::Zero() -> void
{
	this->x = this->y = 0.0f;
}

auto Vector2D::NormalizeInPlace() -> void
{
	*this = this->Normalized();
}

auto Vector2D::Normalized() const -> Vector2D
{
	Vector2D result = *this;
	const float lengh = result.Length();

	if ( lengh != 0.0f )
	{
		result /= lengh;
	}
	else
	{
		result.x = result.y = 0.0f;
	}

	return result;
}

auto Vector2D::DistTo( const Vector2D& vec_2d ) const -> float
{
	return ( *this - vec_2d ).Length();
}

auto Vector2D::DistToSqr( const Vector2D& vec_2d ) const -> float
{
	return ( *this - vec_2d ).LengthSqr();
}

auto Vector2D::Dot( const Vector2D& vec_2d ) -> float
{
	return this->DotProduct2D( *this, vec_2d );
}

auto Vector2D::DotProduct2D( const Vector2D& vec_src, const Vector2D& vec_src2 ) -> float
{
	return vec_src.x * vec_src2.x + vec_src.y * vec_src2.y;
}

auto Vector2D::Length() const -> float
{
	return float( sqrt( this->x * this->x + this->y * this->y ) );
}

auto Vector2D::LengthSqr() const -> float
{
	return this->x * this->x + this->y * this->y;
}
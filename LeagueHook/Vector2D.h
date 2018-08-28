#pragma once

class Vector2D
{
public:
	Vector2D();
	Vector2D( float x, float y );
	Vector2D( float* clr );
	Vector2D( const Vector2D& vec_2d );

	auto Init( float x = 0.0f, float y = 0.0f ) -> void;

	auto IsValid() const -> bool;
	auto Invalidate() -> void;

	auto IsZero() const -> bool;
	auto Zero() -> void;

	auto NormalizeInPlace() -> void;
	auto Normalized() const -> Vector2D;

	auto DistTo( const Vector2D& vec_2d ) const -> float;
	auto DistToSqr( const Vector2D& vec_2d ) const -> float;

	auto Dot( const Vector2D& vec_2d ) -> float;
	auto DotProduct2D( const Vector2D& vec_src, const Vector2D& vec_src2 ) -> float;

	auto Length() const -> float;
	auto LengthSqr() const -> float;

	auto operator[]( const int i ) -> float&
	{
		return ( ( float* )this )[ i ];
	}

	auto operator[]( const int i ) const -> float
	{
		return ( ( float* )this )[ i ];
	}

	auto operator==( const Vector2D& vec_2d ) const -> bool
	{
		return vec_2d.x == this->x
			&& vec_2d.y == this->y;
	}

	auto operator!=( const Vector2D& vec_2d ) const -> bool
	{
		return vec_2d.x != this->x
			|| vec_2d.y != this->y;
	}

	auto operator+=( const Vector2D& vec_2d ) -> Vector2D&
	{
		this->x += vec_2d.x;
		this->y += vec_2d.y;

		return *this;
	}

	auto operator-=( const Vector2D& vec_2d ) -> Vector2D&
	{
		this->x -= vec_2d.x;
		this->y -= vec_2d.y;

		return *this;
	}

	auto operator*=( const float fl ) -> Vector2D&
	{
		this->x *= fl;
		this->y *= fl;

		return *this;
	}

	auto operator*=( const Vector2D& vec_2d ) -> Vector2D&
	{
		this->x *= vec_2d.x;
		this->y *= vec_2d.y;

		return *this;
	}

	auto operator/=( const Vector2D& vec_2d ) -> Vector2D&
	{
		this->x /= vec_2d.x;
		this->y /= vec_2d.y;

		return *this;
	}

	auto operator+=( const float fl ) -> Vector2D&
	{
		this->x += fl;
		this->y += fl;

		return *this;
	}

	auto operator/=( const float fl ) -> Vector2D&
	{
		this->x /= fl;
		this->y /= fl;

		return *this;
	}

	auto operator-=( const float fl ) -> Vector2D&
	{
		this->x -= fl;
		this->y -= fl;

		return *this;
	}

	auto operator=( const Vector2D& vec_2d ) -> Vector2D&
	{
		this->x = vec_2d.x;
		this->y = vec_2d.y;

		return *this;
	}

	auto operator-() const -> Vector2D
	{
		return Vector2D( -this->x, -this->y );
	}

	auto operator+( const Vector2D& vec_2d ) const -> Vector2D
	{
		return Vector2D( this->x + vec_2d.x, this->y + vec_2d.y );
	}

	auto operator+( const float fl ) const -> Vector2D
	{
		return Vector2D( this->x + fl, this->y + fl );
	}

	auto operator-( const Vector2D& vec_2d ) const -> Vector2D
	{
		return Vector2D( this->x - vec_2d.x, this->y - vec_2d.y );
	}

	auto operator-( const float fl ) const -> Vector2D
	{
		return Vector2D( this->x - fl, this->y - fl );
	}

	auto operator*( const float fl ) const -> Vector2D
	{
		return Vector2D( this->x * fl, this->y * fl );
	}

	auto operator*( const Vector2D& vec_2d ) const -> Vector2D
	{
		return Vector2D( this->x * vec_2d.x, this->y * vec_2d.y );
	}

	auto operator/( const float fl ) const -> Vector2D
	{
		return Vector2D( this->x / fl, this->y / fl );
	}

	auto operator/( const Vector2D& vec_2d ) const -> Vector2D
	{
		return Vector2D( this->x / vec_2d.x, this->y / vec_2d.y );
	}

	float x, y;
};
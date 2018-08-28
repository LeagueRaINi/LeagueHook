#pragma once

class Vector
{
public:
	Vector();
	Vector( float x, float y, float z );
	Vector( const float* clr );
	Vector( const Vector& vec );

	auto Init( float x = 0.0f, float y = 0.0f, float z = 0.0f ) -> void;

	auto IsValid() const -> bool;
	auto Invalidate() -> void;

	auto IsZero() const -> bool;
	auto Zero() -> void;

	auto NormalizeInPlace() -> void;
	auto Normalized() const->Vector;

	auto DistTo( const Vector& vec ) const -> float;
	auto DistToSqr( const Vector& vec ) const -> float;

	auto Dot( const Vector& vec ) const -> float;
	auto CrossProduct( const Vector& vec_src, const Vector& vec_src2 ) -> Vector;

	auto Length() const -> float;
	auto LengthSqr() const -> float;
	auto Length2D() const -> float;

	auto operator[]( const int i ) -> float&
	{
		return ( ( float* )this )[ i ];
	}

	auto operator[]( const int i ) const -> float
	{
		return ( ( float* )this )[ i ];
	}

	auto operator==( const Vector& vec ) const -> bool
	{
		return vec.x == this->x
			&& vec.y == this->y
			&& vec.z == this->z;
	}

	auto operator!=( const Vector& vec ) const -> bool
	{
		return vec.x != this->x
			|| vec.y != this->y
			|| vec.z != this->z;
	}

	auto operator+=( const Vector& vec ) -> Vector&
	{
		this->x += vec.x;
		this->y += vec.y;
		this->z += vec.z;

		return *this;
	}

	auto operator-=( const Vector& vec ) -> Vector&
	{
		this->x -= vec.x;
		this->y -= vec.y;
		this->z -= vec.z;

		return *this;
	}

	auto operator*=( const float fl ) -> Vector&
	{
		this->x *= fl;
		this->y *= fl;
		this->z *= fl;

		return *this;
	}

	auto operator*=( const Vector& vec ) -> Vector&
	{
		this->x *= vec.x;
		this->y *= vec.y;
		this->z *= vec.z;

		return *this;
	}

	auto operator/=( const Vector& vec ) -> Vector&
	{
		this->x /= vec.x;
		this->y /= vec.y;
		this->z /= vec.z;

		return *this;
	}

	auto operator+=( const float fl ) -> Vector&
	{
		this->x += fl;
		this->y += fl;
		this->z += fl;

		return *this;
	}

	auto operator/=( const float fl ) -> Vector&
	{
		this->x /= fl;
		this->y /= fl;
		this->z /= fl;

		return *this;
	}

	auto operator-=( const float fl ) -> Vector&
	{
		this->x -= fl;
		this->y -= fl;
		this->z -= fl;

		return *this;
	}

	auto operator=( const Vector& vec ) -> Vector&
	{
		this->x = vec.x;
		this->y = vec.y;
		this->z = vec.z;

		return *this;
	}

	auto operator-() const -> Vector
	{
		return Vector( -this->x, -this->y, -this->z );
	}

	auto operator+( const Vector& vec ) const -> Vector
	{
		return Vector( this->x + vec.x, this->y + vec.y, this->z + vec.z );
	}

	auto operator+( const float fl ) const -> Vector
	{
		return Vector( this->x + fl, this->y + fl, this->z + fl );
	}

	auto operator-( const Vector& vec ) const -> Vector
	{
		return Vector( this->x - vec.x, this->y - vec.y, this->z - vec.z );
	}

	auto operator-( const float fl ) const -> Vector
	{
		return Vector( this->x - fl, this->y - fl, this->z - fl );
	}

	auto operator*( const float fl ) const -> Vector
	{
		return Vector( this->x * fl, this->y * fl, this->z * fl );
	}

	auto operator*( const Vector& vec ) const -> Vector
	{
		return Vector( this->x * vec.x, this->y * vec.y, this->z * vec.z );
	}

	auto operator/( const float fl ) const -> Vector
	{
		return Vector( this->x / fl, this->y / fl, this->z / fl );
	}

	auto operator/( const Vector& vec ) const -> Vector
	{
		return Vector( this->x / vec.x, this->y / vec.y, this->z / vec.z );
	}

	float x, y, z;
};

class __declspec( align( 16 ) ) VectorAligned : public Vector
{
public:
	VectorAligned() = default;
	VectorAligned( float x, float y, float z );
	VectorAligned( const Vector& vec );

	auto operator=( const Vector& vec ) -> VectorAligned&
	{
		this->Init( vec.x, vec.y, vec.z );
		return *this;
	}

	auto operator=( const VectorAligned& vec ) -> VectorAligned&
	{
		this->Init( vec.x, vec.y, vec.z );
		return *this;
	}
};
<<<<<<< HEAD
//#include "stdafx.h"
#include <cmath>
#include <cstring>
#include <algorithm>
#include "VCNVector.h"
#include "VCNMatrix.h"
#include "VCNQuat.h"

template <typename T>
static T clampValue(T value, T minValue, T maxValue) {
	return std::min(std::max(value, minValue), maxValue);
}

//-------------------------------------------------------------
/// Angle between this vector and \a v .
//-------------------------------------------------------------
VCNFloat VCNVector::AngleWith( const VCNVector &v ) 
{
	return (VCNFloat)acos(clampValue(((*this) * v) / (this->GetLength() * v.GetLength()), -1.0, 1.0));
}

//-------------------------------------------------------------
/// Normalize this vector, in-place.
//-------------------------------------------------------------
void VCNVector::Normalize()
{
	VCNFloat f = GetLength();
	if( f != 0.0f ) *this /= f;
}


//-------------------------------------------------------------
/// Build cross product of two vectors (overwriting this vector).
//-------------------------------------------------------------
void VCNVector::Cross( const VCNVector &v1, const VCNVector &v2 )
{
	x = v1.y * v2.z - v1.z * v2.y;
	y = v1.z * v2.x - v1.x * v2.z;
	z = v1.x * v2.y - v1.y * v2.x;
	w = 1.0f;
}


//-------------------------------------------------------------
/// Rotate the vector with the given matrix, ignoring translation information in matrix if present.
//-------------------------------------------------------------
void VCNVector::RotateWith( const VCNMatrix &m ) 
{
	// applying rotational part of matrix only
	VCNFloat _x = x * m._11 + y * m._21 + z * m._31;
	VCNFloat _y = x * m._12 + y * m._22 + z * m._32;
	VCNFloat _z = x * m._13 + y * m._23 + z * m._33;

	x = _x;
	y = _y;
	z = _z;
}


//-------------------------------------------------------------
/// Rotate the vector with the inverse rotation part of the given matrix, ignoring translation of the matrix if present.
//-------------------------------------------------------------
void VCNVector::InvRotateWith( const VCNMatrix &m ) 
{
	// using transposed matrix
	VCNFloat _x = x * m._11 + y * m._12 + z * m._13;
	VCNFloat _y = x * m._21 + y * m._22 + z * m._23;
	VCNFloat _z = x * m._31 + y * m._32 + z * m._33;

	x = _x;
	y = _y;
	z = _z;
}


//-------------------------------------------------------------
/// Multiply a vector and a quaternion.
//-------------------------------------------------------------
VCNQuat VCNVector::operator *( const VCNQuat &q ) const 
{
	return VCNQuat( q.w*x + q.z*y - q.y*z,
					q.w*y + q.x*z - q.z*x,
					q.w*z + q.y*x - q.x*y,
					-(q.x*x + q.y*y + q.z*z)
					);
}


//-------------------------------------------------------------
/// Multiply a vector and a matrix.
//-------------------------------------------------------------
VCNVector VCNVector::operator *( const VCNMatrix &m ) const
{
	VCNVector vcResult;

	vcResult.x = x*m._11 + y*m._21 + z*m._31 + m._41;
	vcResult.y = x*m._12 + y*m._22 + z*m._32 + m._42;
	vcResult.z = x*m._13 + y*m._23 + z*m._33 + m._43;
	vcResult.w = x*m._14 + y*m._24 + z*m._34 + m._44;

	vcResult.x = vcResult.x/vcResult.w;
	vcResult.y = vcResult.y/vcResult.w;
	vcResult.z = vcResult.z/vcResult.w;
	vcResult.w = 1.0f;

	return vcResult;
}
=======
//#include "stdafx.h"
#include <cmath>
#include <cstring>
#include <algorithm>
#include "VCNVector.h"
#include "VCNMatrix.h"
#include "VCNQuat.h"

template <typename T>
static T clampValue(T value, T minValue, T maxValue) {
	return std::min(std::max(value, minValue), maxValue);
}

//-------------------------------------------------------------
/// Angle between this vector and \a v .
//-------------------------------------------------------------
VCNFloat VCNVector::AngleWith( const VCNVector &v ) 
{
	return (VCNFloat)acos(clampValue(((*this) * v) / (this->GetLength() * v.GetLength()), -1.0, 1.0));
}

//-------------------------------------------------------------
/// Normalize this vector, in-place.
//-------------------------------------------------------------
void VCNVector::Normalize()
{
	VCNFloat f = GetLength();
	if( f != 0.0f ) *this /= f;
}


//-------------------------------------------------------------
/// Build cross product of two vectors (overwriting this vector).
//-------------------------------------------------------------
void VCNVector::Cross( const VCNVector &v1, const VCNVector &v2 )
{
	x = v1.y * v2.z - v1.z * v2.y;
	y = v1.z * v2.x - v1.x * v2.z;
	z = v1.x * v2.y - v1.y * v2.x;
	w = 1.0f;
}


//-------------------------------------------------------------
/// Rotate the vector with the given matrix, ignoring translation information in matrix if present.
//-------------------------------------------------------------
void VCNVector::RotateWith( const VCNMatrix &m ) 
{
	// applying rotational part of matrix only
	VCNFloat _x = x * m._11 + y * m._21 + z * m._31;
	VCNFloat _y = x * m._12 + y * m._22 + z * m._32;
	VCNFloat _z = x * m._13 + y * m._23 + z * m._33;

	x = _x;
	y = _y;
	z = _z;
}


//-------------------------------------------------------------
/// Rotate the vector with the inverse rotation part of the given matrix, ignoring translation of the matrix if present.
//-------------------------------------------------------------
void VCNVector::InvRotateWith( const VCNMatrix &m ) 
{
	// using transposed matrix
	VCNFloat _x = x * m._11 + y * m._12 + z * m._13;
	VCNFloat _y = x * m._21 + y * m._22 + z * m._23;
	VCNFloat _z = x * m._31 + y * m._32 + z * m._33;

	x = _x;
	y = _y;
	z = _z;
}


//-------------------------------------------------------------
/// Multiply a vector and a quaternion.
//-------------------------------------------------------------
VCNQuat VCNVector::operator *( const VCNQuat &q ) const 
{
	return VCNQuat( q.w*x + q.z*y - q.y*z,
					q.w*y + q.x*z - q.z*x,
					q.w*z + q.y*x - q.x*y,
					-(q.x*x + q.y*y + q.z*z)
					);
}


//-------------------------------------------------------------
/// Multiply a vector and a matrix.
//-------------------------------------------------------------
VCNVector VCNVector::operator *( const VCNMatrix &m ) const
{
	VCNVector vcResult;

	vcResult.x = x*m._11 + y*m._21 + z*m._31 + m._41;
	vcResult.y = x*m._12 + y*m._22 + z*m._32 + m._42;
	vcResult.z = x*m._13 + y*m._23 + z*m._33 + m._43;
	vcResult.w = x*m._14 + y*m._24 + z*m._34 + m._44;

	vcResult.x = vcResult.x/vcResult.w;
	vcResult.y = vcResult.y/vcResult.w;
	vcResult.z = vcResult.z/vcResult.w;
	vcResult.w = 1.0f;

	return vcResult;
}
>>>>>>> d02eaa7ea9173b0b8b3cf013213142d8f06f22fb

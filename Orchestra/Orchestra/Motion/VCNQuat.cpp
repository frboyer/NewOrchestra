//#include "stdafx.h"

#include <cmath>
#include <cstring>
#include "VCNConstants.h"
#include "VCNMatrix.h"
#include "VCNQuat.h"
#include "VCNVector.h"


//-------------------------------------------------------------
/// Normalize the quaternion
//-------------------------------------------------------------
void VCNQuat::Normalize() 
{
	VCNFloat m = GetMagnitude();

	if( m != 0 ) *this /= copysign(m, w);
}


//-------------------------------------------------------------
/// Rotate quaternion \a q2 by \a q1, overwriting this quaternion.
//-------------------------------------------------------------
void VCNQuat::Rotate( const VCNQuat &q1, const VCNQuat &q2 ) 
{ 
	*this = q1 * q2 * (~q1); 
}


//-------------------------------------------------------------
/// Rotate the vector
//-------------------------------------------------------------
VCNVector VCNQuat::Rotate( const VCNVector &v ) const
{ 
	const VCNQuat& t = *this;
	return ( t*v*(~t) ).GetImag();
}


//-------------------------------------------------------------
/// Get a rotation matrix out of the quaternion
//-------------------------------------------------------------
void VCNQuat::GetMatrix( VCNMatrix *pMat ) const
{
	VCNFloat wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

	// set matrix to zero/1
	memset( pMat, 0, sizeof(VCNMatrix) );
	pMat->_44 = 1.0f;

	x2 = x + x;
	y2 = y + y;
	z2 = z + z;

	xx = x * x2;
	xy = x * y2;
	xz = x * z2;

	yy = y * y2;
	yz = y * z2;
	zz = z * z2;

	wx = w * x2;
	wy = w * y2;
	wz = w * z2;

	pMat->_11 = 1.0f - (yy + zz);
	pMat->_21 = xy - wz;
	pMat->_31 = xz + wy;

	pMat->_12 = xy + wz;
	pMat->_22 = 1.0f - (xx + zz);
	pMat->_32 = yz - wx;

	pMat->_13 = xz - wy;
	pMat->_23 = yz + wx;
	pMat->_33 = 1.0f - (xx + yy);
}


//-------------------------------------------------------------
/// Construct quaternion from Euler angles
//-------------------------------------------------------------
void VCNQuat::SetFromEuler( VCNFloat pitch, VCNFloat yaw, VCNFloat roll ) 
{
	VCNFloat cX, cY, cZ, sX, sY, sZ, cYcZ, sYsZ, cYsZ, sYcZ;

	pitch *= 0.5f;
	yaw   *= 0.5f;
	roll  *= 0.5f;

	cX = cos( pitch );
	cY = cos( yaw );
	cZ = cos( roll );

	sX = sin( pitch );
	sY = sin( yaw );
	sZ = sin( roll );

	cYcZ = cY * cZ;
	sYsZ = sY * sZ;
	cYsZ = cY * sZ;
	sYcZ = sY * cZ;

	w = cX * cYcZ + sX * sYsZ;
	x = sX * cYcZ - cX * sYsZ;
	y = cX * sYcZ + sX * cYsZ;
	z = cX * cYsZ - sX * sYcZ;
}

//-------------------------------------------------------------
/// Converts quaternion into euler angles
//-------------------------------------------------------------
void VCNQuat::GetEulers( VCNFloat* pitch, VCNFloat* yaw, VCNFloat* roll ) const
{
	double   r11, r21, r31, r32, r33, r12, r13;
	double   q00, q11, q22, q33;
	double   tmp;

	q00 = w * w;
	q11 = x * x;
	q22 = y * y;
	q33 = z * z;

	r11 = q00 + q11 - q22 - q33;
	r21 = 2 * (x*y + w*z);
	r31 = 2 * (x*z - w*y);
	r32 = 2 * (y*z + w*x);
	r33 = q00 - q11 - q22 + q33;

	tmp = fabs(r31);
	if(tmp > 0.999999)
	{
		r12 = 2 * (x*y - w*z);
		r13 = 2 * (x*z + w*y);

		*pitch = 0.0f;
		*yaw   = (VCNFloat)-((VCNPI/2) * r31/tmp);
		*roll  = (VCNFloat)atan2(-r12, -r31*r13);
	}
	else
	{
		*pitch = (VCNFloat)atan2(r32, r33);
		*yaw   = (VCNFloat)asin(-r31);
		*roll  = (VCNFloat)atan2(r21, r11);
	}
}


//-------------------------------------------------------------
/// Multiplies two quaternions
//-------------------------------------------------------------
void VCNQuat::operator *= ( const VCNQuat &q ) 
{
	VCNFloat _w = w*q.w - x*q.x - y*q.y - z*q.z;
	VCNFloat _x = w*q.x + x*q.w + y*q.z - z*q.y;
	VCNFloat _y = w*q.y + y*q.w + z*q.x - x*q.z;
	VCNFloat _z = w*q.z + z*q.w + x*q.y - y*q.x;

	x = _x;
	y = _y;
	z = _z;
	w = _w;
}


//-------------------------------------------------------------
/// Multiplies two quaternions
//-------------------------------------------------------------
VCNQuat VCNQuat::operator * ( const VCNQuat &q ) const
{
	VCNQuat qResult;

	qResult.w = w*q.w - x*q.x - y*q.y - z*q.z;
	qResult.x = w*q.x + x*q.w + y*q.z - z*q.y;
	qResult.y = w*q.y + y*q.w + z*q.x - x*q.z;
	qResult.z = w*q.z + z*q.w + x*q.y - y*q.x;

	return qResult;
}


//-------------------------------------------------------------
/// Multiplies a vector with a quaternion
//-------------------------------------------------------------
VCNQuat VCNQuat::operator * ( const VCNVector &v ) const 
{
	return VCNQuat(  w*v.x + y*v.z - z*v.y,
					w*v.y + z*v.x - x*v.z,
					w*v.z + x*v.y - y*v.x,
					-(x*v.x + y*v.y + z*v.z) );
}


//-------------------------------------------------------------
/// Spherically linearly interpolate between two unit 
/// quaternions, a and b
//-------------------------------------------------------------
const VCNQuat VCNQuat::Slerp( const VCNQuat& a, const VCNQuat& b, const float u )
{
	VCNQuat c;
	VCNFloat dot = a.DotProduct( b );

	// Don't bother interpolating after a certain amount, LERP and jet
	if( dot > 0.97f )
	{
		// LERP
		c = a + (b-a)*u;

		// return value must be normalized
		c.Normalize();
		
		return c;
	}

	// if (dot < 0), q1 and q2 are more than 90 degrees apart,
	// so we can invert one to reduce spinning
	if( dot < 0 )
	{
		dot = -dot;
		c = b * -1;
	}
	else
	{
		c = b;
	}

	VCNFloat angle = acos( dot );
	VCNFloat sina = sin( angle );
	VCNFloat sinat = sin( angle * u );
	VCNFloat sinaomt = sin( angle * (1-u) );
	return (a*sinaomt+c*sinat)/sina;
}

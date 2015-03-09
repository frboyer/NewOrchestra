#pragma once
#include <iostream>
#include <cmath>	// for sin, cos, atan2.
#include <limits>	// for epsilon()
#undef M_PI
static const double M_PI = 3.1415926535897932384626; ///< Pi. Use a const instead of the #define in cmath.

#ifndef NO_CMT
#include "TorsoFilePacket.h" // for CmtEuler
#include "EulerAngles.h"     // Must be include before VCNQuat to use them together.
#endif
#undef VCNBool
#undef VCNFloat
typedef bool   VCNBool;  ///< Booleen used by VCN classes.
typedef double VCNFloat; ///< Float used by VCN classes (object files must be compiled with same type).
#include "VCNQuat.h"
#include "VCNVector.h"
#include "VCNMatrix.h"

template <typename T> inline T toDegrees(T x) { return T(x * (180 / M_PI)); }
template <typename T> inline T toRadians(T x) { return T(x * (M_PI / 180)); }
static inline double moduloAngle(double angle) {
	return angle - 2*M_PI * floor(angle / (2*M_PI) + 0.5);
}
static inline double moduloAngleDegrees(double angle) {
	return angle - 360 * floor((angle + 180) / 360);
}

#ifndef NO_CMT
/// Convert EulerAngles to CmtEuler.
inline CmtEuler toCmtEuler(const EulerAngles& ea) { return CmtEuler{ toDegrees(ea.x), toDegrees(ea.y), toDegrees(ea.z) }; }
/// Convert CmtEuler to EulerAngles.
inline EulerAngles toEulerAngles(const CmtEuler& ce) { return EulerAngles{ toRadians(ce.m_roll), toRadians(ce.m_pitch), toRadians(ce.m_yaw), EulOrdXYZs }; }
// Note that CmtEuler is in degrees (in XYZ static axis order), while EulerAngle is in radians.
#endif

#ifdef __IRR_QUATERNION_H_INCLUDED__
static inline irr::core::quaternion toIrrQuat(const VCNQuat& q) { return irr::core::quaternion(q.x, q.y, q.z, q.w); }
static inline VCNQuat toVcnQuat(const irr::core::quaternion& q) { return VCNQuat(q.X, q.Y, q.Z, q.W); }
static inline irr::core::vector3df toIrrVector(const VCNVector& v) { return irr::core::vector3df(v.x, v.y, v.z); }
static inline VCNVector toVcnVector(const irr::core::vector3df& v) { return VCNVector(v.X, v.Y, v.Z); }

static irr::core::vector3df vcnQuatToIrrRotation(const VCNQuat& q)
{
	irr::core::vector3df ea; toIrrQuat(q).toEuler(ea);
	return toDegrees(ea);
}
static inline irr::core::matrix4 toIrrMatrix(const VCNMatrix& m) { irr::core::matrix4 r; for (int i = 0; i < 16; i++) r[i] = m[i]; return r; }
static inline VCNMatrix toVcnMatrix(const irr::core::matrix4& m) { VCNMatrix r;          for (int i = 0; i < 16; i++) r[i] = m[i]; return r; }
#endif

inline double sq(double x) { return x*x; }
inline bool isZero(double x) { return fabs(x) < 16*std::numeric_limits<double>::epsilon(); }

/// Get quaternion from rotation axis and angle aroud that axis.
VCNQuat axisAngleToQuaternion(const VCNVector& axis, double angle)
{
	if (isZero(angle)) return VCNQuat();
	return VCNQuat( axis * (sin(angle/2) / axis.GetLength()), cos(angle/2) );
}

/// Get minimal quaternion to rotate v1 to v2.  A factor can be given to rotate only partway.
VCNQuat quaternionBetweenVectors(VCNVector v1, VCNVector v2, double factor = 1.0)
{
	v1.Normalize(); v2.Normalize();
	VCNVector axis; axis.Cross(v1,v2);
	if (isZero(axis.GetLength()))
		return VCNQuat(); 
	double angle = acos(v1*v2);
	return axisAngleToQuaternion(axis, angle*factor);
}

static inline const VCNVector& GetMatrixAxis(const VCNMatrix& m, int axisIndex)
{
	return *reinterpret_cast<const VCNVector*>(&m[axisIndex * 4]);
}

void scaleMatrix(VCNMatrix& m, const VCNVector& scale) // Pre-scale the matrix in x,y,z but not the translation.
{
	for (int axis = 0; axis < 3; axis++)
		for (int i = 0; i < 3; i++)
			m[axis * 4 + i] *= scale[axis];
}

// If we suppose a scale, followed by a rotation, and some translation (translation has no impact on calculation), we want to get the scale.
static inline VCNVector GetMatrixPreScale(const VCNMatrix& m, int mirrorAxis = 1)
{
	enum { X, Y, Z};
	// A rotation matrix will have norm of each row and each column equal 1.
	// Scaling will make the norm of each vector in the basis equal to the scale, except for sign.
	VCNVector scale;
	for (int axis = X; axis <= Z; axis++)
		scale[axis] = GetMatrixAxis(m, axis).GetLength();
	// We want a single mirror (only one negative component) as mirroring cannot be represented as a rotation. The axis can be chosen as parameter.
	if ((GetMatrixAxis(m, (mirrorAxis==Z) ? X : (mirrorAxis + 1)) ^ GetMatrixAxis(m, (mirrorAxis==X) ? Z : (mirrorAxis - 1))) * GetMatrixAxis(m, mirrorAxis) < 0.0)
		scale[mirrorAxis] = -scale[mirrorAxis];
	return scale;
}

// Internal function for MatrixToQuaternion.
static inline VCNQuat MatrixToQuaternion_internal(
	const VCNMatrix& m,             // matrix
	int k0, int k1, int k2, int k3, // order (0,1,2,3)
	int s0, int s1, int s2          // signs (-1 or +1)
	)
{
	// Based on J.M.P. van Waveren, From Quaternion to Matrix and Back, Id Software Inc, 2005, http://software.intel.com/sites/default/files/m/d/4/1/d/8/293748.pdf .
	VCNFloat q[4];
	VCNFloat t = s0 * m._11 + s1 * m._22 + s2 * m._33 + 1.0f;
	VCNFloat s = 0.5f / sqrt(t);
	q[k0] = s * t;
	q[k1] = ( m._12 - s2 * m._21 ) * s;
	q[k2] = ( m._31 - s1 * m._13 ) * s;
	q[k3] = ( m._23 - s0 * m._32 ) * s;
	return VCNQuat(q[0],q[1],q[2],q[3]);
}

/// Get quaternion from rotation matrix.
VCNQuat MatrixToQuaternion(const VCNMatrix& mScaled)
{
	VCNVector scale = GetMatrixPreScale(mScaled);
	VCNMatrix m = mScaled; 
	//-----------------------------------------------------------------------------------------------------------------
	//scaleMatrix(m, VCNFloat(1.0) / scale);
	//-----------------------------------------------------------------------------------------------------------------


	// Based on J.M.P. van Waveren, From Quaternion to Matrix and Back, Id Software Inc, 2005, http://software.intel.com/sites/default/files/m/d/4/1/d/8/293748.pdf .  But without code repetition, keeping optimized code.
	if ( m._11 + m._22 + m._33 > 0.0f )
		return MatrixToQuaternion_internal(m, 3,2,1,0, +1,+1,+1);
	
	else if ( m._11 > m._22 && m._11 > m._33 )
		return MatrixToQuaternion_internal(m, 0,1,2,3, +1,-1,-1);
	
	else if ( m._22 > m._33 )
		return MatrixToQuaternion_internal(m, 1,0,3,2, -1,+1,-1);
	
	else
		return MatrixToQuaternion_internal(m, 2,3,0,1, -1,-1,+1);
}

#ifndef NO_CMT
/// Rate of turn to angle (in quaternion).
VCNQuat rateOfTurnToAngleQuaternion(
	const CmtVector& gyr, ///< Rate of turn in rad/second on each axis.
	double time           ///< Duration in seconds by which the rate will be multipled to give a rotation angle.
	)                     /// \return Rotation angle in quaternion form.
{	// See http://www.euclideanspace.com/physics/kinematics/angularvelocity/ 
	double angularVelocity =  VCNVector(gyr).GetLength(); // in rad/second.
	double angularRotation = angularVelocity * time;      // in rad.
	return axisAngleToQuaternion( gyr, angularRotation );
}

/// Angle (in quaternion) to rate of turn; inverse of ::rateOfTurnToAngleQuaternion(const CmtVector& gyr, double time).
CmtVector angleQuaternionToRateOfTurn(
	const VCNQuat& q,     ///< Rotation angle in quaternion form.
	double time           ///< Duration in seconds by which the rotation will be divided to give a rate of turn.
	)                     /// \return Rate of turn in rad/second on each axis.
{
	double imaginaryMagnitude = q.GetImagMagnitude();
	double factor = 2/time * (
		isZero(imaginaryMagnitude) // Avoid 0/0,
			? 1.0 // the limit when imaginaryMagnitude->0 is 1, since d/dy atan2(y,1) = 1  (rotation quaternions are unit length).
			: atan2(imaginaryMagnitude, q.w) / imaginaryMagnitude
		);
	return CmtVector(q.GetImag() * factor);
}
#endif

/// Get a quaternion orientation from relative up (gravity) and north (magnetic) vectors, that is the rotation to bring these relative vectors to absolute up and north.
VCNQuat getOrientationFromVectors(VCNVector gravity, VCNVector magneticNorth)
{
	gravity.Normalize(); magneticNorth.Normalize();
	VCNVector& up = gravity;
	VCNVector  west;  west.Cross(up, magneticNorth); west.Normalize();
	VCNVector  north; north.Cross(west, up);
	// MTX coordinates are right-handed: x=front, y=left, z=up
	VCNMatrix  m;
	m._11 = north.x; m._12 = north.y; m._13 = north.z; m._14 = 0;
	m._21 = west .x; m._22 = west .y; m._23 = west .z; m._24 = 0;
	m._31 = up   .x; m._32 = up   .y; m._33 = up   .z; m._34 = 0;
	m._41 =          m._42 =          m._43 = 0;       m._44 = 1;
	
	return ~MatrixToQuaternion(m); // Inverted to have orientation relative to axis, not axis relative to MTX.
}

/// Similar to the other version of getOrientationFromVectors, but using the down component of magneticNorth; of the 3 degrees of freedom, here magneticNorth fixes 2 and gravity fixes 1, while in the other version it is the opposite.
VCNQuat getOrientationFromVectors(VCNVector gravity, VCNVector magneticNorth, double downMagneticNorthAngle)
{
	gravity.Normalize(); magneticNorth.Normalize();
	VCNVector  west;  west.Cross(gravity, magneticNorth); west.Normalize();
	VCNVector  north = axisAngleToQuaternion(west, -downMagneticNorthAngle).Rotate(magneticNorth);
	VCNVector  up;    up.Cross(north, west);
	// MTX coordinates are right-handed: x=front, y=left, z=up
	VCNMatrix  m;
	m._11 = north.x; m._12 = north.y; m._13 = north.z; m._14 = 0;
	m._21 = west .x; m._22 = west .y; m._23 = west .z; m._24 = 0;
	m._31 = up   .x; m._32 = up   .y; m._33 = up   .z; m._34 = 0;
	m._41 =          m._42 =          m._43 = 0;       m._44 = 1;
	
	return ~MatrixToQuaternion(m); // Inverted to have orientation relative to axis, not axis relative to MTX.
}
/// Converts vector relative to some orientation (axis relative to MTX) to absolute vector (axis relative to earth poles).
inline VCNVector relativeToAbsolute(const VCNQuat& orientation, const VCNVector& relativeVector)
{
	return orientation.Rotate(relativeVector);
}

/// Converts vector relative to some orientation (axis relative to MTX) to absolute vector (axis relative to earth poles).
inline VCNVector absoluteToRelative(const VCNQuat& orientation, const VCNVector& absoluteVector)
{
	return (~orientation).Rotate(absoluteVector);
}


// For output to streams:
#ifndef NO_CMT
/// Write a vector to a stream.
std::ostream& operator <<(std::ostream& o, const VCNVector& v) { o << *(const CmtVector*)&v[0]; return o; }

/// Write a matrix to a stream.
std::ostream& operator <<(std::ostream& o, const VCNMatrix& m) {
	o << *(const CmtVector*)&m._11 << std::endl
		<< *(const CmtVector*)&m._21 << std::endl
		<< *(const CmtVector*)&m._31 << std::endl
		<< *(const CmtVector*)&m._41 << std::endl;
	return o;
}

/// Write euler angles to a stream
std::ostream& operator <<(std::ostream& o, const EulerAngles& e) {
	o << e.x << "x " << e.y << "y " << e.z << "z " << e.w << "order";
	return o;
}
#endif

/// Write a quaternion to a stream.
std::ostream& operator <<(std::ostream& o, const VCNQuat& q) {
	o << q.x << "i + " << q.y << "j + " << q.z << "k + " << q.w;
	return o; 
}

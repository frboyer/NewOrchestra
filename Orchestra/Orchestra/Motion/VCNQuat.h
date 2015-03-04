#ifndef VICUNA_QUATERNION
#define VICUNA_QUATERNION

#include "VCNVector.h" // Needed for inlining of Vector/Quaternion operations.
class VCNMatrix;

//-------------------------------------------------------------
/// Our basic quaternion class
//-------------------------------------------------------------
class VCNQuat
{
public:
	VCNFloat x; ///< i
	VCNFloat y; ///< j
	VCNFloat z; ///< k
	VCNFloat w; ///< Real part

	//---------------------------------------

	inline VCNQuat& Set( VCNFloat _x, VCNFloat _y, VCNFloat _z, VCNFloat _w );
	inline VCNQuat& SetImag( const VCNVector& v );

	inline VCNQuat() { Set(0.0, 0.0, 0.0, 1.0); }
	inline VCNQuat( VCNFloat _x, VCNFloat _y, VCNFloat _z, VCNFloat _w ) { Set(_x, _y, _z, _w); }
	inline VCNQuat( const VCNVector& v, VCNFloat _w ) { Set( v.x, v.y, v.z, _w ); }
	~VCNQuat() { }

	inline VCNVector GetImag() const;
	
	void  GetEulers( VCNFloat* pitch, VCNFloat* yaw, VCNFloat* roll ) const;
	void  SetFromEuler( VCNFloat pitch, VCNFloat yaw, VCNFloat roll );
	void  Normalize();
	void  Conjugate( const VCNQuat& q ) { Set( -q.x,  -q.y,  -q.z,  q.w ); }
	void  GetMatrix( VCNMatrix *m ) const;
	VCNFloat GetMagnitude() const;
	VCNFloat GetImagMagnitude() const;

	inline VCNFloat DotProduct( const VCNQuat& q ) const;

	void    operator /=( const VCNFloat f );
	VCNQuat operator /( const VCNFloat f ) const;

	void    operator *=( const VCNFloat f );
	VCNQuat operator *( const VCNFloat f ) const;

	VCNQuat operator *( const VCNVector& v ) const;

	VCNQuat operator *( const VCNQuat& q ) const;
	void    operator *=( const VCNQuat& q );

	void    operator +=( const VCNQuat& q );
	VCNQuat operator +( const VCNQuat& q ) const;

	void    operator -=( const VCNQuat& q );
	VCNQuat operator -( const VCNQuat& q ) const;

	VCNQuat operator~() const { return VCNQuat(-x, -y, -z, w); } ///< Conjugate.

	void Rotate( const VCNQuat& q1, const VCNQuat& q2 );

	VCNVector Rotate( const VCNVector& v ) const;

	static const VCNQuat Slerp( const VCNQuat& a, const VCNQuat& b, const float u );

#ifdef _H_EulerAngles
	/// \note Using EulerAngles by Ken Shoemake, 1993, to convert to/from Euler angles of any axis order. \class VCNQuat
	inline VCNQuat& Set(const Quat& q) { return Set(q.x, q.y, q.z, q.w); }     ///< Set from his Quat type.
	inline VCNQuat& Set(const EulerAngles& ea) { return Set(Eul_ToQuat(ea)); } ///< Set from his EulerAngles type.
	inline VCNQuat& operator =(const Quat& q) { return Set(q); }               ///< Set from his Quat type.
	inline VCNQuat& operator =(const EulerAngles& ea) { return Set(ea); }      ///< Set from his EulerAngles type.
	inline VCNQuat(const Quat& q) { Set(q); }                                  ///< Construct from his Quat type.
	inline VCNQuat(const EulerAngles& ea) { Set(ea); }                         ///< Construct from his EulerAngles type.
	inline operator Quat() const { Quat q = { x, y, z, w}; return q; }         ///< Convert to his Quat type.
	inline EulerAngles GetEulers( unsigned int order_code ) { return Eul_FromQuat(*this, order_code); } ///< Get Euler angles in the specified order.
#endif
};

//-------------------------------------------------------------
/// Set from components.
inline VCNQuat& VCNQuat::Set( VCNFloat _x, VCNFloat _y, VCNFloat _z, VCNFloat _w )
{
	x = _x;  y = _y;  z = _z;  w = _w; return *this;
}

//-------------------------------------------------------------
/// Sets pure imaginary part from vector (keeping real part).
inline VCNQuat& VCNQuat::SetImag( const VCNVector& v )
{
	return Set( v.x, v.y, v.z, w );
}

//-------------------------------------------------------------
/// Returns pure imaginary part as vector.
inline VCNVector VCNQuat::GetImag() const
{
	return VCNVector( x, y, z );
}

//-------------------------------------------------------------
inline VCNFloat VCNQuat::DotProduct( const VCNQuat& q ) const
{
	return x * q.x + y*q.y + z*q.z + w*q.w;
}

//-------------------------------------------------------------
/// Get the magnitude of a quaternion.
inline VCNFloat VCNQuat::GetMagnitude() const
{
	return sqrt( x*x + y*y + z*z + w*w );
}

//-------------------------------------------------------------
/// Get the magnitude of the imaginary part of quaternion.
inline VCNFloat VCNQuat::GetImagMagnitude() const
{
	return sqrt( x*x + y*y + z*z );
}

//-------------------------------------------------------------
// Operators, modifying and non-modifying, with floats.  Each component is affected by same value. (w is also affected)
#define M_opF(OP) \
	inline void VCNQuat::operator OP##=( const VCNFloat f ) { \
		x OP##= f;  y OP##= f;  z OP##= f; w OP##= f; \
	} \
	inline VCNQuat VCNQuat::operator OP( const VCNFloat f ) const { \
		return VCNQuat( x OP f,  y OP f,  z OP f,  w OP f ); \
	}

M_opF(*)
M_opF(/)
#undef M_opF

//-------------------------------------------------------------
// Operators, modifying and non-modifying, with quaternions.  Component by component operation. (w is also affected)
#define M_opQ(OP) \
	inline void VCNQuat::operator OP##=( const VCNQuat& q ) { \
		x OP##= q.x;  y OP##= q.y;  z OP##= q.z;  w OP##= q.w; \
	} \
	inline VCNQuat VCNQuat::operator OP( const VCNQuat& q ) const { \
		return VCNQuat( x OP q.x,  y OP q.y,  z OP q.z,  w OP q.w ); \
	}
M_opQ(+)
M_opQ(-)
#undef M_V

#endif
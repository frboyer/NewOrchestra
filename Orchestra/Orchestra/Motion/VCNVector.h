<<<<<<< HEAD
#ifndef VICUNA_VECTOR
#define VICUNA_VECTOR

#include <cmath>
#include <cassert>
class VCNMatrix;
class VCNQuat;

//-------------------------------------------------------------
/// Our basic 4D vector class.
//-------------------------------------------------------------
class VCNVector
{
public:
	// Note: This was a union between four separate floats (x,y,z,w), and an array (floatArray[4]), but that kind of structure has undefined behavior according to the C++ standard.  It is now replaced by an array accessor.
	VCNFloat x, y, z, w;

	//---------------------------------------

	VCNVector(); ///< Default constructor, initializes to (0,0,0,1).
	VCNVector( VCNFloat _x, VCNFloat _y, VCNFloat _z ); /// Constructor, initializes to (x,y,z,1)

	VCNFloat& operator[](int index) { assert(index >=0 && index < 4); return (&x)[index]; } ///< Array accessor.
	const VCNFloat& operator[](int index) const { assert(index >=0 && index < 4); return (&x)[index]; } ///< Const array accessor.
	
	inline void Clear();
	void Set( VCNFloat _x, VCNFloat _y, VCNFloat _z, VCNFloat _w = 1.0f );
	VCNFloat GetLength() const;
	inline VCNFloat GetSqrLength() const;
	inline void Negate();
	void Normalize();

	// Calculates the angle we make with another vector.
	VCNFloat AngleWith( const VCNVector &v );

	// Applies the rotation contained in a matrix.
	void RotateWith( const VCNMatrix& );
	// Applies the inverse rotation contained in a matrix.
	void InvRotateWith( const VCNMatrix& );
		
	void operator +=( const VCNVector &v );
	void operator -=( const VCNVector &v );
	void operator *=( VCNFloat f );
	void operator /=( VCNFloat f );
	void operator +=( VCNFloat f );
	void operator -=( VCNFloat f );
	VCNVector operator -( ) const;
	VCNVector operator +( VCNFloat f ) const; ///< Add \a f to each component.
	VCNVector operator -( VCNFloat f ) const; ///< Subtract \a f from each component.
	VCNVector operator *( VCNFloat f ) const;
	VCNVector operator /( VCNFloat f ) const;
	VCNQuat   operator *( const VCNQuat &q ) const;
	VCNVector operator *( const VCNMatrix &m ) const;
	VCNVector operator +( const VCNVector &v ) const;
	VCNVector operator -( const VCNVector &v ) const;

	/// Multiply two vectors (dot product).
	VCNFloat  operator *(const VCNVector &v) const;

	// Cross product
	void Cross( const VCNVector &v1, const VCNVector &v2 );
	VCNVector operator ^( const VCNVector &v2 ) const { VCNVector cross; cross.Cross(*this, v2); return cross; }
	
	VCNVector hadamardProduct( const VCNVector &v ) const { return VCNVector(x*v.x, y*v.y, z*v.z); }
	
#ifdef CMT_VECTORS // conversion to/from Xsens vectors
	inline void Set(const CmtVector& v) { Set(v.m_data[0], v.m_data[1], v.m_data[2]); }
	inline VCNVector(const CmtVector& v) { Set(v); }
	operator CmtVector() const { CmtVector v; v.m_data[0] = x; v.m_data[1] = y; v.m_data[2] = z; return v; }
#endif
}; 

//-------------------------------------------------------------
inline VCNVector::VCNVector() :
x( 0.0f ),
y( 0.0f ),
z( 0.0f ),
w( 1.0f )
{
}

//-------------------------------------------------------------
inline VCNVector::VCNVector( VCNFloat _x, VCNFloat _y, VCNFloat _z ) :
x( _x ),
y( _y ),
z( _z ),
w( 1.0f )
{
}

//-------------------------------------------------------------
inline void VCNVector::Set( VCNFloat _x, VCNFloat _y, VCNFloat _z, VCNFloat _w ) 
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
} 

//-------------------------------------------------------------
inline void VCNVector::Clear()
{
	Set(0.0, 0.0, 0.0);
}

//-------------------------------------------------------------
/// Returns the squared length (magnitude) of the vector.
inline VCNFloat VCNVector::GetSqrLength() const
{
	return ( x*x + y*y + z*z ); 
}

//-------------------------------------------------------------
/// Returns the length (magnitude) of the vector.
inline VCNFloat VCNVector::GetLength() const
{
	return sqrt( GetSqrLength() );
}

//-------------------------------------------------------------
/// In-place negation of vector.
inline void VCNVector::Negate() 
{
	x = -x;
	y = -y;
	z = -z;
}

//-------------------------------------------------------------
/// Returns negation of vector.
inline VCNVector VCNVector::operator -() const
{
	return VCNVector(-x, -y, -z);
}

//-------------------------------------------------------------
// Operators, modifying and non-modifying, with floats.  Each component is affected by same value. (w is not affected)
#define M_opF(OP) \
	inline void VCNVector::operator OP##=( const VCNFloat f ) { \
		x OP##= f;  y OP##= f;  z OP##= f; \
	} \
	inline VCNVector VCNVector::operator OP( const VCNFloat f ) const { \
		return VCNVector( x OP f,  y OP f,  z OP f ); \
	}

M_opF(+)
M_opF(-)
M_opF(*)
M_opF(/)
#undef M_opF

//-------------------------------------------------------------
// Operators, modifying and non-modifying, with vectors.  Component by component operation. (w is not affected)
#define M_opV(OP) \
	inline void VCNVector::operator OP##=( const VCNVector& v ) { \
		x OP##= v.x;  y OP##= v.y;  z OP##= v.z; \
	} \
	inline VCNVector VCNVector::operator OP( const VCNVector& v ) const { \
		return VCNVector( x OP v.x,  y OP v.y,  z OP v.z ); \
	}
M_opV(+)
M_opV(-)
#undef M_V

//-------------------------------------------------------------
// Multiply two vectors (dot product).
inline VCNFloat VCNVector::operator *( const VCNVector &v ) const
{
	return ( v.x*x + v.y*y + v.z*z );
}

=======
#ifndef VICUNA_VECTOR
#define VICUNA_VECTOR

#include <cmath>
#include <cassert>
class VCNMatrix;
class VCNQuat;

//-------------------------------------------------------------
/// Our basic 4D vector class.
//-------------------------------------------------------------
class VCNVector
{
public:
	// Note: This was a union between four separate floats (x,y,z,w), and an array (floatArray[4]), but that kind of structure has undefined behavior according to the C++ standard.  It is now replaced by an array accessor.
	VCNFloat x, y, z, w;

	//---------------------------------------

	VCNVector(); ///< Default constructor, initializes to (0,0,0,1).
	VCNVector( VCNFloat _x, VCNFloat _y, VCNFloat _z ); /// Constructor, initializes to (x,y,z,1)

	VCNFloat& operator[](int index) { assert(index >=0 && index < 4); return (&x)[index]; } ///< Array accessor.
	const VCNFloat& operator[](int index) const { assert(index >=0 && index < 4); return (&x)[index]; } ///< Const array accessor.
	
	inline void Clear();
	void Set( VCNFloat _x, VCNFloat _y, VCNFloat _z, VCNFloat _w = 1.0f );
	VCNFloat GetLength() const;
	inline VCNFloat GetSqrLength() const;
	inline void Negate();
	void Normalize();

	// Calculates the angle we make with another vector.
	VCNFloat AngleWith( const VCNVector &v );

	// Applies the rotation contained in a matrix.
	void RotateWith( const VCNMatrix& );
	// Applies the inverse rotation contained in a matrix.
	void InvRotateWith( const VCNMatrix& );
		
	void operator +=( const VCNVector &v );
	void operator -=( const VCNVector &v );
	void operator *=( VCNFloat f );
	void operator /=( VCNFloat f );
	void operator +=( VCNFloat f );
	void operator -=( VCNFloat f );
	VCNVector operator -( ) const;
	VCNVector operator +( VCNFloat f ) const; ///< Add \a f to each component.
	VCNVector operator -( VCNFloat f ) const; ///< Subtract \a f from each component.
	VCNVector operator *( VCNFloat f ) const;
	VCNVector operator /( VCNFloat f ) const;
	VCNQuat   operator *( const VCNQuat &q ) const;
	VCNVector operator *( const VCNMatrix &m ) const;
	VCNVector operator +( const VCNVector &v ) const;
	VCNVector operator -( const VCNVector &v ) const;

	/// Multiply two vectors (dot product).
	VCNFloat  operator *(const VCNVector &v) const;

	// Cross product
	void Cross( const VCNVector &v1, const VCNVector &v2 );
	VCNVector operator ^( const VCNVector &v2 ) const { VCNVector cross; cross.Cross(*this, v2); return cross; }
	
	VCNVector hadamardProduct( const VCNVector &v ) const { return VCNVector(x*v.x, y*v.y, z*v.z); }
	
#ifdef CMT_VECTORS // conversion to/from Xsens vectors
	inline void Set(const CmtVector& v) { Set(v.m_data[0], v.m_data[1], v.m_data[2]); }
	inline VCNVector(const CmtVector& v) { Set(v); }
	operator CmtVector() const { CmtVector v; v.m_data[0] = x; v.m_data[1] = y; v.m_data[2] = z; return v; }
#endif
}; 

//-------------------------------------------------------------
inline VCNVector::VCNVector() :
x( 0.0f ),
y( 0.0f ),
z( 0.0f ),
w( 1.0f )
{
}

//-------------------------------------------------------------
inline VCNVector::VCNVector( VCNFloat _x, VCNFloat _y, VCNFloat _z ) :
x( _x ),
y( _y ),
z( _z ),
w( 1.0f )
{
}

//-------------------------------------------------------------
inline void VCNVector::Set( VCNFloat _x, VCNFloat _y, VCNFloat _z, VCNFloat _w ) 
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
} 

//-------------------------------------------------------------
inline void VCNVector::Clear()
{
	Set(0.0, 0.0, 0.0);
}

//-------------------------------------------------------------
/// Returns the squared length (magnitude) of the vector.
inline VCNFloat VCNVector::GetSqrLength() const
{
	return ( x*x + y*y + z*z ); 
}

//-------------------------------------------------------------
/// Returns the length (magnitude) of the vector.
inline VCNFloat VCNVector::GetLength() const
{
	return sqrt( GetSqrLength() );
}

//-------------------------------------------------------------
/// In-place negation of vector.
inline void VCNVector::Negate() 
{
	x = -x;
	y = -y;
	z = -z;
}

//-------------------------------------------------------------
/// Returns negation of vector.
inline VCNVector VCNVector::operator -() const
{
	return VCNVector(-x, -y, -z);
}

//-------------------------------------------------------------
// Operators, modifying and non-modifying, with floats.  Each component is affected by same value. (w is not affected)
#define M_opF(OP) \
	inline void VCNVector::operator OP##=( const VCNFloat f ) { \
		x OP##= f;  y OP##= f;  z OP##= f; \
	} \
	inline VCNVector VCNVector::operator OP( const VCNFloat f ) const { \
		return VCNVector( x OP f,  y OP f,  z OP f ); \
	}

M_opF(+)
M_opF(-)
M_opF(*)
M_opF(/)
#undef M_opF

//-------------------------------------------------------------
// Operators, modifying and non-modifying, with vectors.  Component by component operation. (w is not affected)
#define M_opV(OP) \
	inline void VCNVector::operator OP##=( const VCNVector& v ) { \
		x OP##= v.x;  y OP##= v.y;  z OP##= v.z; \
	} \
	inline VCNVector VCNVector::operator OP( const VCNVector& v ) const { \
		return VCNVector( x OP v.x,  y OP v.y,  z OP v.z ); \
	}
M_opV(+)
M_opV(-)
#undef M_V

//-------------------------------------------------------------
// Multiply two vectors (dot product).
inline VCNFloat VCNVector::operator *( const VCNVector &v ) const
{
	return ( v.x*x + v.y*y + v.z*z );
}

>>>>>>> d02eaa7ea9173b0b8b3cf013213142d8f06f22fb
#endif
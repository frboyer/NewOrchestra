<<<<<<< HEAD
#ifndef VICUNA_MATRIX
#define VICUNA_MATRIX

#include "VCNVector.h"

//-------------------------------------------------------------
/// Vicuna's basic matrix class.
/// The matrix is 4x4, for multiplication by row vector on left, placed in row major order.
//-------------------------------------------------------------
class VCNMatrix 
{
public:

	// Note: This was a union between sixteen separate floats, and an array (floatArray), but that kind of structure has undefined behavior according to the C++ standard.  It is now replaced by an array accessor.
	VCNFloat _11, _12, _13, _14;
	VCNFloat _21, _22, _23, _24;
	VCNFloat _31, _32, _33, _34;
	VCNFloat _41, _42, _43, _44; // Translation

	//---------------------------------------

	VCNMatrix() { }		///< Default constructor (does NOT initialialize matrix).

	VCNFloat& operator[](int index) { assert(index >= 0 && index < 16); return (&_11)[index]; } ///< Array accessor.
	const VCNFloat& operator[](int index) const { return (*const_cast<VCNMatrix*>(this))[index]; } ///< Const array accessor.
	
	void				Identity();
	void				RotX( VCNFloat angle );
	void				RotY( VCNFloat angle );
	void				RotZ( VCNFloat angle );
	void				Rot( VCNFloat x, VCNFloat y, VCNFloat z );
	inline void			Rot( const VCNVector& vect );
	void				RotArbi( const VCNVector &vcAxis, VCNFloat a );
	void				ApplyInverseRota( VCNVector *pvc );
	void				Translate( VCNFloat dx, VCNFloat dy, VCNFloat dz );
	void				SetTranslation( VCNVector vect, VCNBool clear=false );
	inline VCNVector	GetTranslation() const;

	void Billboard( VCNVector pos, VCNVector dir, VCNVector worldUp = VCNVector(0,1,0) );
	void LookAt( VCNVector pos, VCNVector lookAt, VCNVector worldUp = VCNVector(0,1,0) );

	void TransposeOf( const VCNMatrix &m );
	void InverseOf( const VCNMatrix &m );

	// VCNBool operator!= ( const VCNMatrix &m ) const { return memcmp( arrayPtr(), m.arrayPtr(), arraySize ) != 0; }
	// VCNBool operator== ( const VCNMatrix &m ) const { return memcmp( arrayPtr(), m.arrayPtr(), arraySize ) == 0; }
	VCNMatrix operator* ( const VCNMatrix &m ) const;
	VCNVector operator* ( const VCNVector &vect ) const;
	
private:
	static const int arraySize = sizeof(VCNFloat)*16;
	VCNFloat* arrayPtr() { return &_11; }             ///< Private accessor to matrix as an array.
	const VCNFloat* arrayPtr() const { return &_11; } ///< Private accessor to matrix as a const array.
};

//-------------------------------------------------------------
inline void VCNMatrix::Rot( const VCNVector& vect )
{
	Rot( vect.x, vect.y, vect.z );
}

//-------------------------------------------------------------
inline VCNVector VCNMatrix::GetTranslation() const
{
	return VCNVector(_41, _42, _43); 
}

=======
#ifndef VICUNA_MATRIX
#define VICUNA_MATRIX

#include "VCNVector.h"

//-------------------------------------------------------------
/// Vicuna's basic matrix class.
/// The matrix is 4x4, for multiplication by row vector on left, placed in row major order.
//-------------------------------------------------------------
class VCNMatrix 
{
public:

	// Note: This was a union between sixteen separate floats, and an array (floatArray), but that kind of structure has undefined behavior according to the C++ standard.  It is now replaced by an array accessor.
	VCNFloat _11, _12, _13, _14;
	VCNFloat _21, _22, _23, _24;
	VCNFloat _31, _32, _33, _34;
	VCNFloat _41, _42, _43, _44; // Translation

	//---------------------------------------

	VCNMatrix() { }		///< Default constructor (does NOT initialialize matrix).

	VCNFloat& operator[](int index) { assert(index >= 0 && index < 16); return (&_11)[index]; } ///< Array accessor.
	const VCNFloat& operator[](int index) const { return (*const_cast<VCNMatrix*>(this))[index]; } ///< Const array accessor.
	
	void				Identity();
	void				RotX( VCNFloat angle );
	void				RotY( VCNFloat angle );
	void				RotZ( VCNFloat angle );
	void				Rot( VCNFloat x, VCNFloat y, VCNFloat z );
	inline void			Rot( const VCNVector& vect );
	void				RotArbi( const VCNVector &vcAxis, VCNFloat a );
	void				ApplyInverseRota( VCNVector *pvc );
	void				Translate( VCNFloat dx, VCNFloat dy, VCNFloat dz );
	void				SetTranslation( VCNVector vect, VCNBool clear=false );
	inline VCNVector	GetTranslation() const;

	void Billboard( VCNVector pos, VCNVector dir, VCNVector worldUp = VCNVector(0,1,0) );
	void LookAt( VCNVector pos, VCNVector lookAt, VCNVector worldUp = VCNVector(0,1,0) );

	void TransposeOf( const VCNMatrix &m );
	void InverseOf( const VCNMatrix &m );

	// VCNBool operator!= ( const VCNMatrix &m ) const { return memcmp( arrayPtr(), m.arrayPtr(), arraySize ) != 0; }
	// VCNBool operator== ( const VCNMatrix &m ) const { return memcmp( arrayPtr(), m.arrayPtr(), arraySize ) == 0; }
	VCNMatrix operator* ( const VCNMatrix &m ) const;
	VCNVector operator* ( const VCNVector &vect ) const;
	
private:
	static const int arraySize = sizeof(VCNFloat)*16;
	VCNFloat* arrayPtr() { return &_11; }             ///< Private accessor to matrix as an array.
	const VCNFloat* arrayPtr() const { return &_11; } ///< Private accessor to matrix as a const array.
};

//-------------------------------------------------------------
inline void VCNMatrix::Rot( const VCNVector& vect )
{
	Rot( vect.x, vect.y, vect.z );
}

//-------------------------------------------------------------
inline VCNVector VCNMatrix::GetTranslation() const
{
	return VCNVector(_41, _42, _43); 
}

>>>>>>> d02eaa7ea9173b0b8b3cf013213142d8f06f22fb
#endif
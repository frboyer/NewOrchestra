
//#include "TorsoFilePacket.h"
//#include "EulerAngles.h"
#include "VCNQuat.h"
#include "VCNVector.h"
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

template <class T>
struct NormalizationFactor {
	static const double value;
};
template <> const double NormalizationFactor<float>::value = 1;
template <> const double NormalizationFactor<int>::value = 0x7FFFFFFF;
template <> const double NormalizationFactor<unsigned int>::value = 0xFFFFFFFF;
template <> const double NormalizationFactor<short>::value = 0x7FFF;
template <> const double NormalizationFactor<unsigned short>::value = 0xFFFF;
template <> const double NormalizationFactor<signed char>::value = 0x7F;
template <> const double NormalizationFactor<unsigned char>::value = 0xFF;

template <class ValType> inline ValType roundTo(double x) { return ValType(roundl(x)); }
template <>              inline float   roundTo(double x) { return float(x); }

/// A value in interval [-1, 1] (if ValType is signed) or [0, 1] (if ValType is unsigned).
template <class ValType>
class NormalizedValue {
	ValType v_;
public:
	operator double() const { return v_ / NormalizationFactor<ValType>::value; }
	NormalizedValue& operator =(double value) { v_ = roundTo<ValType>(value * NormalizationFactor<ValType>::value); return *this; }
	// double get() { return v_ / NormalizationFactor<ValType>::value; }
	// void set(double value) { v_ = ValType(value * NormalizationFactor<ValType>::value); }
};

class SmallCommon {
public:

	template <typename DerivedT, typename T>
	static T nonLinear(const T& v)        { return DerivedT::applyFactor(v, nonLinearFactor(DerivedT::sinMagnitude(v))); }
	template <typename DerivedT, typename T>
	static T nonLinearInverse(const T& v) { return DerivedT::applyFactor(v, nonLinearInverseFactor(DerivedT::sinMagnitude(v))); }

	static inline double nonLinearFactor(double len) {
		return (len < epsilon())
			? 1.0 / (M_PI / 2.0) // The limit when len -> 0.
			: asin(std::min(1.0, len)) / len / (M_PI / 2.0);
	}
	static inline double nonLinearInverseFactor(double len) {
		return (len < epsilon())
			? (M_PI / 2.0) // The limit when len -> 0.
			: sin(len * (M_PI / 2.0)) / len;
	}
	template <typename T>
	static inline T normalize(T v) { v.Normalize(); return v; }

	static inline double square(double x) { return x * x; }
	static inline double epsilon() { return 1E-10; }
};

/// A direction is a normalized vector; the small direction is represented by 2 values.
template <class ValType> // "short" gives better than ~0.003 degrees of error (~ a pixel in 19000), "signed char" gives better than ~0.8 degrees.
class SmallDirection : SmallCommon {
	typedef SmallDirection<ValType> ThisT;
	NormalizedValue<ValType> v[2];
public:
	//NOTE: Keeping only de x,y of a normalized 3D vector loses the sign of z and has some unused values in the [-1,1]x[-1,1] square. Instead, a mapping of the sphere on an octahedron is used, and the top and bottom triangles are unfolded in the center and perimeter of the square, respectively. We also use a non-linear transform to equalize errors (otherwise there would be much higher errors when z is close to 0).
	operator VCNVector() const {
		VCNVector unfolded = nonLinearInverse<ThisT>(unfold(VCNVector(v[0], v[1], 0.0)));
		double squaredZ = 1.0 - square(unfolded.x) - square(unfolded.y);
		unfolded.z = squaredZ <= 0 ? 0.0 : copysign(sqrt(squaredZ), unfolded.z);
		return unfolded;
	}
	SmallDirection& operator =(const VCNVector& vector) {
		VCNVector folded = fold(nonLinear<ThisT>(normalize(vector)));
		v[0] = folded.x; v[1] = folded.y;
		return *this;
	}
private:
	//NOTE: z of result is sign of z of parameter.
	static VCNVector fold(const VCNVector& normalizedVector) {
		return foldTriangleMirrors(sphereToOctahedron(normalizedVector), (normalizedVector.z < 0));
	}
	//NOTE: z of result is sign that z should have, z of parameter is unused.
	static VCNVector unfold(const VCNVector& foldedVector) {
		return octahedronToSphere(foldTriangleMirrors(foldedVector, (fabs(foldedVector.x) + fabs(foldedVector.y) > 1)));
	}

	static VCNVector sphereToOctahedron(const VCNVector& vector) { return applyFactor(vector, sphereToOctahedronFactor(vector)); }
	static VCNVector octahedronToSphere(const VCNVector& vector) { return applyFactor(vector, 1.0 / sphereToOctahedronFactor(vector)); }

	static double sphereToOctahedronFactor(const VCNVector& v) {
		double len = sinMagnitude(v);
		return (len < epsilon())
			? 1 / 1.3 // The limit len -> 0 does not exist, it is something between 1.0 and 1/sqrt(2)=0.707, the average might be around 1/1.3.
			: len / (fabs(v.x) + fabs(v.y));
	}
	static VCNVector foldTriangleMirrors(const VCNVector& foldedVector, bool doMirror = true) {
		return (doMirror)
			? VCNVector(copysign(1 - fabs(foldedVector.y), foldedVector.x), copysign(1 - fabs(foldedVector.x), foldedVector.y), -1.0)
			: VCNVector(foldedVector.x, foldedVector.y, 1.0);
	}

	friend SmallCommon;
	static VCNVector applyFactor(const VCNVector& v, double factor) { return VCNVector(v.x * factor, v.y * factor, v.z); }
	static double sinMagnitude(const VCNVector& v)                  { return sqrt(v.x*v.x + v.y*v.y); }
};

/// An orientation is a normalized quaternion with positive w; the small orientation is represented by 3 values.  Non linear values are also used, as with SmallDirection, but no folding.
template <class ValType> // "short" gives better than ~0.005 degrees of error, "signed char" gives better than ~1.2 degrees.
class SmallOrientation : SmallCommon {
	typedef SmallOrientation<ValType> ThisT;
	NormalizedValue<ValType> v[3];
public:
	operator VCNQuat() const {
		VCNQuat q = nonLinearInverse<ThisT>(VCNQuat(v[0], v[1], v[2], 0.0));
		q.w = sqrt(std::max(0.0, 1.0 - square(q.x) - square(q.y) - square(q.z)));
		return q;
	}
	SmallOrientation& operator =(const VCNQuat& quat) {
		VCNQuat q = nonLinear<ThisT>(normalize(quat));
		v[0] = q.x; v[1] = q.y; v[2] = q.z;
		return *this;
	}
	
private:
	friend SmallCommon;
	static VCNQuat applyFactor(const VCNQuat& q, double factor) { return VCNQuat(q.x * factor, q.y * factor, q.z * factor, q.w); }
	static double sinMagnitude(const VCNQuat& q)                { return q.GetImagMagnitude(); }
};

/// A finger has a direction relative to hand orientation, and a flexion (third joint has no data from glove).
struct SmallFinger {
	typedef unsigned char ValType; // As the angle range of a finger is never > 100 degrees, the resolution is better than 0.4 degree (~ a pixel in 143).
	//SmallDirection<ValType> relativeDirection;
	SmallOrientation<ValType> relativeOrientation;
	NormalizedValue<ValType> flexion;
};

struct SmallArm { // 22bytes
	//SmallDirection<short> armDirections[2];  // 2*2bytes
	SmallOrientation<short> armOrientation[2];  // 3*2bytes
	SmallOrientation<short> handOrientation; // 3bytes
	SmallFinger fingers[5];           // 5*4bytes
};

// TorsoFilePacket has size 8+6*(3*3*8+3*8)=584, and GloveFilePacket has 4+2*14*2=60, total of 644 (~133MB/hour).
// MotionFilePacket, with short arm orientations and char finger flexions, has 44 bytes (~9MB/hour).
// Glove and torso data is resampled to be synchronized in time.
// (For smaller file size, it could be stored differential and compressed).
struct MotionFilePacket {
	SmallArm armsDirections[2];
};
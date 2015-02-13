// Class for range based for loops, supporting enums.
// By Francois-R.Boyer@PolyMtl.ca
//
// Requires C++11 (-std=c++0x on GCC 4.6, -std=c++11 on GCC 4.7+)

// mostly from http://stackoverflow.com/questions/7185437/is-there-a-range-class-in-c11-for-use-with-range-based-for-loops
// added template and increment
template <typename Value = int, typename Increment = int>
class Range {
public:
	class iterator {
		friend class Range;
public:
		Value operator *() const { return i_; }
		const iterator &operator ++() { i_ += inc_; return *this; }
		iterator operator ++(int) { iterator copy(*this); i_ += inc_; return copy; }

		bool operator ==(const iterator &other) const { return i_ == other.i_; }
		bool operator !=(const iterator &other) const { return i_ != other.i_; }

protected:
		iterator(Value start, Increment inc) : i_ (start), inc_(inc) { }

private:
		Value i_;
		Increment inc_;
	};

	iterator begin() const { return begin_; }
	iterator end() const { return end_; }
	Range(Value begin, Value end, Increment inc=Increment(1)) : begin_(begin,inc), end_(end,inc) {}
private:
	iterator begin_;
	iterator end_;
};

template <typename T1, typename T2 = int> inline Range<T1,T2> range(T1 begin, T1 end, T2 inc=T2(1)) {
	return Range<T1,T2>(begin, end, inc);
}

//NOTE: For an enum to be iterable, all integer values in the given range must be valid.
#define ITERABLE_ENUM(EnumClassName) \
	EnumClassName operator+(EnumClassName a, int b) { return EnumClassName(int(a)+b); }\
	EnumClassName& operator+=(EnumClassName& a, int b) { a = a+b; return a; }

//NOTE: For an enum to be enumerable, it must be iterable and have _FIRST and _LAST defined.
template <typename EnumClass>
inline Range<EnumClass,unsigned> enumerate() {
	return Range<EnumClass,unsigned>(EnumClass::_FIRST, EnumClass::_LAST+1, 1);
}

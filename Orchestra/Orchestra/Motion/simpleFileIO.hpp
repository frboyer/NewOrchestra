/**
*  Simple wrappers for read/write operations in binary files.
*  \author Francois-R.Boyer@PolyMtl.ca
*  \date 2014-10-15
*  \file
*/
#pragma once
#include <iostream>
#include <fstream>
#include <vector>
// POSIX (not ISO C/C++):
#include <sys/stat.h>

template <typename T>
T readItem(std::istream& is) {
	T v;
	is.read((char*)&v, sizeof(v));
	return v;
}

template <typename T>
void readItem(std::istream& is, T& v_out) {
	is.read(reinterpret_cast<char*>(&v_out), sizeof(v_out));
}

template <typename T>
void writeItem(std::ostream& os, const T& v) {
	os.write(reinterpret_cast<const char*>(&v), sizeof(v));
}

template <typename T>
void readVector(std::istream& is, T& v_out, size_t nItems) {
	v_out.resize(nItems);
	is.read(reinterpret_cast<char*>(&v_out[0]), nItems * sizeof(v_out[0]));
}

template <typename T>
void writeVector(std::ostream& os, const T& v) {
	os.write(reinterpret_cast<const char*>(&v[0]), v.size() * sizeof(v[0]));
}

static inline std::streamoff getFileSize(std::istream& is) {
	std::streampos oldPos = is.tellg();
	is.seekg(0, std::ios::end);
	std::streamoff size = is.tellg();
	is.seekg(oldPos);
	return size;
}

static inline bool isEndReached(std::istream& is) {
	return is.eof() || is.peek() == EOF;
}

//{ POSIX (not ISO C/C++):
static inline bool fileExists(const std::string& fileName) {
#if 0 // ISO C++ version is a bit slower.
	std::ifstream file(fileName, std::ios::binary);
	return file.is_open();
#else
	struct stat s;
	return stat(fileName.c_str(), &s) == 0;
#endif
}
static inline time_t getModificationTime(const std::string& fileName) {
	struct stat s;
	if (stat(fileName.c_str(), &s) != 0)
		return 0;
	return s.st_mtime;
}
//}
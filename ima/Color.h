#ifndef _COLOR_H_
#define _COLOR_H_

#include <string.h>

template<class T>
class Color {
private:
	unsigned char length;
	T* components;
public:
	Color(unsigned char length) {
		this->length = length;
	}
	Color(const Color& src) {
		this->length = src.length;
		memcpy(this->components, src.components, length*sizeof(T));
	}
	~Color() {
		free(components);
	}
	const TVector3<Real>& operator[](const unsigned long index) const {
		return components[index];
	}
};

#endif
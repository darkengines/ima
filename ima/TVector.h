#ifndef _TVECTOR_H_
#define _TVECTOR_H_

#include <string.h>
#include <math.h>
#include "Precision.h"

template<class Real, int Size>
class TVector {
protected:
	Real* _values;
public:
	TVector() {
		_values = new Real[Size];
		unsigned int i = 0;
		while (i<Size) {
			_values[i] = 0;
			++i;
		}
	}
	TVector(const Real* prValues) {
		_values = new Real[Size];
		unsigned int i = 0u;
		while (i<Size) {
			_values[i] = prValues[i];
			++i;
		}
	}
	TVector(const TVector& rtVector) {
		_values = new Real[Size];
		*this = rtVector;
	}
	virtual ~TVector() {
		delete[] _values;
	}
	virtual Real& operator[](const unsigned int iIndex) {
		return _values[iIndex];
	}
	virtual const Real& operator[](const unsigned int iIndex) const {
		return _values[iIndex];
	}
	virtual bool operator==(const TVector& rtVector) const {
		return !memcmp(_values, rtVector._values, Size*sizeof(Real));
	}
	virtual bool operator!=(const TVector& rtVector) const {
		return !!memcmp(_values, rtVector._values, Size*sizeof(Real));
	}
	virtual bool operator>(const TVector& rtVector) const {
		return memcmp(_values, rtVector._values, Size*sizeof(Real)) > 0;
	}
	virtual bool operator>=(const TVector& rtVector) const {
		return memcmp(_values, rtVector._values, Size*sizeof(Real)) >= 0;
	}
	virtual bool operator<(const TVector& rtVector) const {
		return memcmp(_values, rtVector._values, Size*sizeof(Real)) < 0;
	}
	virtual bool operator<=(const TVector& rtVector) const {
		return memcmp(_values, rtVector._values, Size*sizeof(Real)) <= 0;
	}
	virtual TVector& operator=(const TVector& rtVector) {
		unsigned int i = 0u;
		while (i<Size) {
			_values[i] = rtVector._values[i];
			++i;
		}
		return *this;
	}
	virtual TVector operator+(const TVector& rtVector) const {
		TVector result(_values);
		result += rtVector;
		return result;
	}
	virtual void operator+=(const TVector& rtVector) {
		unsigned int i = 0u;
		while (i<Size) {
			_values[i] += rtVector._values[i];
			++i;
		}
	}
	virtual TVector operator-(const TVector& rtVector) const {
		TVector result(_values);
		result -= rtVector;
		return result;
	}
	virtual TVector operator-() const {
		TVector result(_values);
		int i = 0;
		while (i<Size) {
			_values[i] = -_values[i];
			++i;
		}
		return result;
	}
	virtual void operator-=(const TVector& rtVector) {
		unsigned int i = 0u;
		while (i<Size) {
			_values[i] -= rtVector._values[i];
			++i;
		}
	}
	virtual Real operator*(const TVector& rtVector) const {
		Real result = 0;
		unsigned int i = 0u;
		while (i<Size) {
			result += _values[i]*rtVector._values[i];
			++i;
		}
		return result;
	}
	virtual TVector operator+(const Real& rrValue) const {
		TVector result(_values);
		result += rrValue;
		return result;
	}
	virtual void operator+=(const Real& rrValue) {
		unsigned int i = 0u;
		while (i<Size) {
			_values[i] += rrValue;
			++i;
		}
	}
	virtual TVector operator-(const Real& rrValue) const {
		TVector result(_values);
		result -= rrValue;
		return result;
	}
	virtual void operator-=(const Real& rrValue) {
		unsigned int i = 0u;
		while (i<Size) {
			_values[i] -= rrValue;
			++i;
		}
	}
	virtual TVector operator*(const Real& rrValue) const {
		TVector result(_values);
		result *= rrValue;
		return result;
	}
	virtual void operator*=(const Real& rrValue) {
		unsigned int i = 0u;
		while (i<Size) {
			_values[i] *= rrValue;
			++i;
		}
	}
	virtual TVector operator/(const Real& rrValue) const {
		TVector result(_values);
		result /= rrValue;
		return result;
	}
	virtual void operator/=(const Real& rrValue) {
		unsigned int i = 0u;
		while (i<Size) {
			_values[i] /= rrValue;
			++i;
		}
	}
	virtual Real SquaredMagnitude() const{
		Real result = 0;
		unsigned int i = 0u;
		while (i<Size) {
			result+=_values[i]*_values[i];
			++i;
		}
		return result;
	}
	virtual Real Magnitude() {
		return sqrt(SquaredMagnitude());
	}
	virtual void Normalize() {
		Real magnitude = Magnitude();
		*this /= magnitude;
	}
	TVector Normalized() {
		TVector result(_values);
		result.Normalize();
		return result;
	}
	void AddScaledVector(const TVector& rtVector, const Real& rrScale) {
		*this+=rtVector*rrScale;
	}
	TVector AddedScaledVector(const TVector& rtVector, const Real& rrScale) {
		TVector result(_values);
		result.AddScaledVector(rtVector, rrScale);
		return result;
	}
	void Invert() {
		unsigned int i = 0u;
		while (i<Size) {
			_values[i] = -_values[i];
			++i;
		}
	}
	TVector Inverted() const {
		TVector result(_values);
		result.Invert();
		return result;
	}
	void SetAll(const Real rValue) {
		unsigned int i = 0;
		while (i<Size) {
			_values[i] = rValue;
			++i;
		}
	}
	void Switch(const unsigned int iIndex0, unsigned int iIndex1) {
		Real tmp = _values[iIndex0];
		_values[iIndex0] = _values[iIndex1];
		_values[iIndex1] = tmp;
	}
};
#endif
#ifndef _TVECTOR_H_
#define _TVECTOR_H_

#include <string.h>
#include <math.h>
#include "Precision.h"
#include "Hashable.h"
#include <math.h>

template<class Real>
class TVector: public Hashable {
protected:
	
	int _size;
	Real* _values;
public:
	static const int _primes[3];
	TVector(int size) {
		_size = size;
		_values = new Real[_size];
		int i = 0;
		while (i<_size) {
			_values[i] = 0;
			++i;
		}
	}
	TVector(const Real* prValues, int size) {
		_size = size;
		_values = new Real[_size];
		int i = 0u;
		while (i<_size) {
			_values[i] = prValues[i];
			++i;
		}
	}
	TVector(const TVector& rtVector) {
		_size = rtVector._size;
		_values = new Real[_size];
		*this = rtVector;
	}
	virtual ~TVector() {
		delete[] _values;
	}
	virtual Real& operator[](const int iIndex) {
		return _values[iIndex];
	}
	virtual const Real& operator[](const int iIndex) const {
		return _values[iIndex];
	}
	virtual bool operator==(const TVector& rtVector) const {
		int i = _size;
		bool r = 1;
		while (r && i--) {
			r &= rtVector._values[i] == _values[i];
		}
		return r;
	}
	virtual bool operator!=(const TVector& rtVector) const {
		return !!memcmp(_values, rtVector._values, _size*sizeof(Real));
	}
	virtual bool operator>(const TVector& rtVector) const {
		return memcmp(_values, rtVector._values, _size*sizeof(Real)) > 0;
	}
	virtual bool operator>=(const TVector& rtVector) const {
		return memcmp(_values, rtVector._values, _size*sizeof(Real)) >= 0;
	}
	virtual bool operator<(const TVector& rtVector) const {
		return memcmp(_values, rtVector._values, _size*sizeof(Real)) < 0;
	}
	virtual bool operator<=(const TVector& rtVector) const {
		return memcmp(_values, rtVector._values, _size*sizeof(Real)) <= 0;
	}
	virtual TVector& operator=(const TVector& rtVector) {
		int i = 0u;
		while (i<_size) {
			_values[i] = rtVector._values[i];
			++i;
		}
		return *this;
	}
	virtual TVector operator+(const TVector& rtVector) const {
		TVector result(_values, _size);
		result += rtVector;
		return result;
	}
	virtual void operator+=(const TVector& rtVector) {
		int i = 0u;
		while (i<_size) {
			_values[i] += rtVector._values[i];
			++i;
		}
	}
	virtual TVector operator-(const TVector& rtVector) const {
		TVector result(_values, _size);
		result -= rtVector;
		return result;
	}
	virtual TVector operator-() const {
		TVector result(_values, _size);
		int i = 0;
		while (i<_size) {
			_values[i] = -_values[i];
			++i;
		}
		return result;
	}
	virtual void operator-=(const TVector& rtVector) {
		int i = 0u;
		while (i<_size) {
			_values[i] -= rtVector._values[i];
			++i;
		}
	}
	virtual Real operator*(const TVector& rtVector) const {
		Real result = 0;
		int i = 0u;
		while (i<_size) {
			result += _values[i]*rtVector._values[i];
			++i;
		}
		return result;
	}
	virtual TVector operator+(const Real& rrValue) const {
		TVector result(_values, _size);
		result += rrValue;
		return result;
	}
	virtual void operator+=(const Real& rrValue) {
		int i = 0u;
		while (i<_size) {
			_values[i] += rrValue;
			++i;
		}
	}
	virtual TVector operator-(const Real& rrValue) const {
		TVector result(_values, _size);
		result -= rrValue;
		return result;
	}
	virtual void operator-=(const Real& rrValue) {
		int i = 0u;
		while (i<_size) {
			_values[i] -= rrValue;
			++i;
		}
	}
	virtual TVector operator*(const Real& rrValue) const {
		TVector result(_values, _size);
		result *= rrValue;
		return result;
	}
	virtual void operator*=(const Real& rrValue) {
		int i = 0u;
		while (i<_size) {
			_values[i] *= rrValue;
			++i;
		}
	}
	virtual TVector operator/(const Real& rrValue) const {
		TVector result(_values, _size);
		result /= rrValue;
		return result;
	}
	virtual void operator/=(const Real& rrValue) {
		int i = 0u;
		while (i<_size) {
			_values[i] /= rrValue;
			++i;
		}
	}
	virtual Real SquaredMagnitude() const{
		Real result = 0;
		int i = 0u;
		while (i<_size) {
			result+=_values[i]*_values[i];
			++i;
		}
		return result;
	}
	virtual Real Magnitude() {
		return sqrt((float)SquaredMagnitude());
	}
	virtual void Normalize() {
		Real magnitude = Magnitude();
		*this /= magnitude;
	}
	TVector Normalized() {
		TVector result(_values, _size);
		result.Normalize();
		return result;
	}
	void AddScaledVector(const TVector& rtVector, const Real& rrScale) {
		*this+=rtVector*rrScale;
	}
	TVector AddedScaledVector(const TVector& rtVector, const Real& rrScale) {
		TVector result(_values, _size);
		result.AddScaledVector(rtVector, rrScale);
		return result;
	}
	void Invert() {
		int i = 0u;
		while (i<_size) {
			_values[i] = -_values[i];
			++i;
		}
	}
	TVector Inverted() const {
		TVector result(_values, _size);
		result.Invert();
		return result;
	}
	void SetAll(const Real rValue) {
		int i = 0;
		while (i<_size) {
			_values[i] = rValue;
			++i;
		}
	}
	void Switch(const int iIndex0, int iIndex1) {
		Real tmp = _values[iIndex0];
		_values[iIndex0] = _values[iIndex1];
		_values[iIndex1] = tmp;
	}
	int Hash() {
		int i = _size;
		int result = (int)(_values[i]*_primes[i]);
		i--;
		while (i--) {
			result = result^(int)(_values[i]*_primes[i]);
		}
		return result;
	}
};
const int TVector<unsigned char>::_primes[3] = {73856093,19349663,83492791};
#endif
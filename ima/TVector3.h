#ifndef _TVECTOR3_H_
#define _TVECTOR3_H_

#include "TVector.h"

template<class Real>
class TVector3: public TVector<Real, 3> {
private:
public:
	TVector3(): TVector() {

	}
	TVector3(const Real* prValues): TVector(prValues) {
	}
	Real& X() {
		return _values[0];
	}
	Real& Y() {
		return _values[1];
	}
	Real& Z() {
		return _values[2];
	}
	TVector3& operator=(const TVector3& rTVector) {
		return static_cast<TVector3&>(TVector::operator=(rTVector));
	}
	TVector3 operator+(const TVector3& rTVector) const {
		return static_cast<TVector3&>(TVector::operator+(rTVector));
	}
	TVector3 operator-(const TVector3& rTVector) const {
		return static_cast<TVector3&>(TVector::operator-(rTVector));
	}
	TVector3 operator%(const TVector3& rtVector) const {
		TVector3 result(_values);
		return result %= rtVector;
	}
	TVector3& operator%=(const TVector3& rtVector) {
		Real x,y,z;
		x = _values[0];
		y = _values[1];
		z = _values[2];
		_values[0] = y * rtVector._values[2] - z * rtVector[1];
		_values[1] = z * rtVector._values[0] - x * rtVector[2];
		_values[2] = x * rtVector._values[1] - y * rtVector[0];
		return *this;
	}
	void UnitCross(const TVector3& rtVector) {
		*this%=rtVector;
		Normalize();
	}
	TVector3 UnitCrossed(const TVector3& rtVector) {
		TVector3 result(_values);
		return result.UnitCross();
	}
	Real operator*(const TVector3& rTVector) const {
		return static_cast<Real>(TVector::operator*(rTVector));
	}
	TVector3 operator+(const Real& rrValue) {
		return static_cast<TVector3&>(TVector::operator+(rrValue));
	}
	TVector3 operator-(const Real& rrValue) {
		return static_cast<TVector3&>(TVector::operator-(rrValue));
	}
	TVector3 operator*(const Real& rrValue) {
		return static_cast<TVector3&>(TVector::operator*(rrValue));
	}
	TVector3 operator/(const Real& rrValue) {
		return static_cast<TVector3&>(TVector::operator/(rrValue));
	}
	TVector3 Normalized() {
		return static_cast<TVector3&>(TVector::Normalized());
	}
	TVector3 AddedScaledVector(const TVector& rtVector, const Real& rrScale) {
		return static_cast<TVector3&>(TVector::AddedScaledVector(rtVector, rrScale));
	}
	TVector3 Inverted() const {
		return static_cast<TVector3&>(TVector::Inverted());
	}
	
	void Perp() {
		Real temp = _values[0];
		_values[0] = _values[1];
		_values[1] = -temp;
	}
	TVector3 UnitPerped() const {
		TVector3 result = Perped();
		result.Normalize();
		return result;
	}
	Real DotPerp(const TVector3& rtVector) {
		return *this*rtVector.Perped();
	}
	static void Orthonormalize(TVector3& rtVector1, TVector3& rtVector2, TVector3& rtVector3) {
		rtVector1.Normalize();

		rtVector2 -= rtVector1*(rtVector1*rtVector2);
		rtVector2.Normalize();

		rtVector3 -= ((rtVector1 * (rtVector1*rtVector3)) + (rtVector2 * (rtVector2*rtVector3)));
		rtVector3.Normalize();
	}
	static void GenerateOrthonormalBasis(TVector3& rtVectorU, TVector3& rtVectorV, TVector3& rtVectorW, bool bUIsNormalized) {
		if (!bUIsNormalized) {
			rtVectorU.Normalize();
		}
		if (abs(rtVectorU._values[0]) > abs(rtVectorU._values[1])) {
			rtVectorV._values[0] = rtVectorU._values[2];
			rtVectorV._values[2] = -rtVectorU._values[1];
			rtVectorV._values[1] = 0;
		} else {
			rtVectorV._values[1] = rtVectorU._values[2];
			rtVectorV._values[2] = -rtVectorU._values[0];
			rtVectorV._values[0] = 0;
		}
		rtVectorV.Normalize();
		rtVectorW = rtVectorU.UnitCrossed(rtVectorV);
	}
private:
};
#endif
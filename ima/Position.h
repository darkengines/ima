#ifndef _POSITION_H_
#define _POSITION_H_

class Position {
private:
	long x, y;
public:
	Position(long x, long y) {
		this->x = x;
		this->y = y;
	}
	Position(const Position& position) {
		this->x = src.x;
		this->y = src.y;
	}
	~Position() {
	}
	long GetX() {
		return x;
	}
	long GetY() {
		return y;
	}
}

#endif
#pragma once

class Point
{
	private:
		int x, y;

	public:
		Point(int newX, int newY) { x = newX; y = newY; };
		int getX() { return x; };
		int getY() { return y; };
};
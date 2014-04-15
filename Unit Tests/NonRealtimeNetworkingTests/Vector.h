class Vector {

public:
	int x;
	int y;
	Vector(int a, int b) : x(a), y(b) { };
	Vector() { };
	~Vector() { };
	float getLength();

};
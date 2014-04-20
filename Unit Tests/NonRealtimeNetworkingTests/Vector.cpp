#include <cmath> 
#include "Vector.h"

float Vector::getLength() {
	return sqrt(static_cast<float>(x*x + y*y));
}
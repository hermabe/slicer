#include "vector.h"

Vector3d cross(const Vector3d& lhs, const Vector3d& rhs) {
	return {
		lhs[1] * rhs[2] - lhs[2] * rhs[1],
		lhs[2] * rhs[0] - lhs[0] * rhs[2],
		lhs[0] * rhs[1] - lhs[1] * rhs[0]
	};
}
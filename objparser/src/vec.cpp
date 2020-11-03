#include "vec.h"

Vec3 operator+( Vec3 const& left, Vec3 const& right ){
    return Vec3(left.x + right.x,
                left.y + right.y,
                left.z + right.z);
}

Vec3 operator-( Vec3 const& left, Vec3 const& right ){
    return Vec3(left.x - right.x,
                left.y - right.y,
                left.z - right.z);
}

Vec3 operator/( Vec3 const& left, float const& right ){
    return Vec3(left.x / right,
                left.y / right,
                left.z / right);
}

Vec3 operator/( float const& left, Vec3 const& right ){
    return Vec3(left / right.x,
                left / right.y,
                left / right.z);
}

bool operator==( Vec3 const& left, Vec3 const& right ){
    return left.x == right.x &&
            left.y == right.y &&
            left.z == right.z;
}

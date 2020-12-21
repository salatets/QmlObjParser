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

float operator*( Vec3 const& left, Vec3 const& right ){
    return left.x * right.x +
            left.y * right.y +
            left.z * right.z;
}

bool operator==( Vec3 const& left, Vec3 const& right ){
    return left.x == right.x &&
            left.y == right.y &&
            left.z == right.z;
}

Vec3 operator^(Vec3 const& left, Vec3 const& right){
    return Vec3(left.y * right.z - left.z * right.y,
                left.z * right.x - left.x * right.z,
                left.x * right.y - left.y * right.x);
}

#ifndef VEC_H
#define VEC_H


struct Vec3{
    float x;
    float y;
    float z;
    Vec3(float x, float y, float z)
        : x(x), y(y), z(z){}
};

Vec3 operator+( Vec3 const& left, Vec3 const& right );
Vec3 operator-( Vec3 const& left, Vec3 const& right );
Vec3 operator/( Vec3 const& left, float const& right );
float operator*( Vec3 const& left, Vec3 const& right );
bool operator==( Vec3 const& left, Vec3 const& right );
Vec3 operator^(Vec3 const& left, Vec3 const& right);

struct Vec2{
    float x;
    float y;
    Vec2(float x, float y)
        : x(x), y(y){}
};

#endif // VEC_H

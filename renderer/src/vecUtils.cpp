#include "vecUtils.h"

const QVector3D& vec3ToQVector3D(const Vec3& vec){
    return reinterpret_cast<const QVector3D&>(vec);
}

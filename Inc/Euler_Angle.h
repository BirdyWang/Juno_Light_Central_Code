#ifndef EULER_ANGLE_H
#define EULER_ANGLE_H

#include <math.h>

#define PI						3.14159265359f

typedef struct Quaternion
{
    float q0, q1, q2, q3;
}Quaternion;

typedef struct EulerAngle
{
    float yaw, pitch, roll;
}EulerAngle;

void QuatToEulerAngle(Quaternion * q, EulerAngle * euler);

#endif

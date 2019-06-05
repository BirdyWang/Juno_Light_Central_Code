#ifndef EULER_ANGLE_H
#define EULER_ANGLE_H

#include <math.h>
#include "LED1642GW.h"

#define PI						3.14159265359f
#define TWO_PI              	6.28318530718f
#define Q_COEFF                 1073741824.0f

typedef struct Quaternion
{
    float q0, q1, q2, q3;
}Quaternion;

typedef struct EulerAngle
{
    float yaw, pitch, roll;
}EulerAngle;

void ConvertQuaternion(long * q_in, Quaternion * q_out);
void QuatToEulerAngle(Quaternion * q, EulerAngle * euler);
void EulerAngleToLED(EulerAngle * euler, uint8_t * channel, rgb_led* color);

#endif

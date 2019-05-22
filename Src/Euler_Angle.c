#include "Euler_Angle.h"

void QuatToEulerAngle(Quaternion * q, EulerAngle * euler)
{
    float sinr_cosp = 2.0f * (q->q0 * q->q1 + q->q2 + q->q3);
    float cosr_cosp = 1.0f - 2.0f * (q->q1 * q->q1 + q->q2 * q->q2);
    euler->roll = atan2(sinr_cosp, cosr_cosp);
    
    float sinp = 2.0f * (q->q0 * q->q2 - q->q3 * q->q1);
    if (fabs(sinp) >= 1)
    {
        euler->pitch = copysign(PI /2, sinp);
    }
    else
    {
        euler->pitch = asin(sinp);
    }
    
    float siny_cosp = 2.0f * (q->q0 * q->q3 + q->q1 * q->q2);
    float cosy_cosp = 1.0f - 2.0f * (q->q2 * q->q2 + q->q3 * q->q3);
    euler->yaw = atan2(siny_cosp, cosy_cosp);
}


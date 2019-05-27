#include "Euler_Angle.h"

void ConvertQuaternion(long * q_in, Quaternion * q_out)
{
    q_out->q0 = q_in[0] / Q_COEFF;
    q_out->q1 = q_in[1] / Q_COEFF;
    q_out->q2 = q_in[2] / Q_COEFF;
    q_out->q3 = q_in[3] / Q_COEFF;    
}    
    
void QuatToEulerAngle(Quaternion * q, EulerAngle * euler)
{
    float sinr_cosp = 2.0f * (q->q0 * q->q1 + q->q2 * q->q3);
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

void EulerAngleToLED(EulerAngle * euler, uint8_t * channel, rgb_led* color)
{
    // mapping yaw to planer LED channel
    if(euler->yaw > 0)
    {
        *channel = floor(euler->yaw * 2.54f);
    }
    else if(euler->yaw <= 0)
    {
        *channel = 15 - floor((0 - euler->yaw) * 2.54f);
    }
    
    color->b = 0;    
    //mapping roll to LED color:
    float output1 = 1 + sin(PI/2 + euler->roll);
    color->r = output1 * 127.5f;
    float output2 = 1 + sin(euler->pitch);
    color->g = output2 * 127.5f;
}

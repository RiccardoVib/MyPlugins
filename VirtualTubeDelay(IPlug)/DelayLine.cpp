//
//  DelayLine.cpp
//  VirtualTubeDelay
//
//  Created by Riccardo Simionato on 03/10/18.
//
//

#include "DelayLine.hpp"
#include <iostream>
#include <math.h>

void DelayLine::setDelayL(double samples){
    
    rptrLeft = wptrLeft - (long)samples;
    //while (wptrLeft - bufferLeft >= len) { wptrLeft -= len; }
    while (rptrLeft < bufferLeft) { rptrLeft += len; }
    crossfadeNeededL = true;
    memcpy(bufferLeft_copy, bufferLeft, len*sizeof(double));
    rptrLeft_copy = rptrLeft;
    t_L = -1;
    
    fracDelaySamplesLeft = samples - (long)samples;
}

void DelayLine::setDelayR(double samples){
    
    rptrRight = wptrRight - (long)samples;
    //while (wptrRight - bufferRight >= len) { wptrRight -= len; }
    while (rptrRight < bufferRight) { rptrRight += len; }
    crossfadeNeededR = true;
    memcpy(bufferRight_copy, bufferRight, len*sizeof(double));
    rptrRight_copy = rptrRight;
    t_R = -1;
    
    fracDelaySamplesRight = samples - (long)samples;
}


void DelayLine::setDelay_Ref_L(double samples){
    
    rptrLeft_Ref = wptrLeft_Ref - (long)samples;
    //while (wptrLeft_Ref - bufferLeft >= len) { wptrLeft_Ref -= len; }
    while (rptrLeft_Ref < bufferLeft_Ref) { rptrLeft_Ref += len_ref; }
    
    fracDelaySamplesLeft_Ref = samples - (long)samples;
}

void DelayLine::setDelay_Ref_R(double samples){
    
    rptrRight_Ref = wptrRight_Ref - (long)samples;
    //while (wptrRight_Ref - bufferRight >= len) { wptrRight_Ref -= len; }
    while (rptrRight_Ref < bufferRight_Ref) { rptrRight_Ref += len_ref; }
    
    fracDelaySamplesRight_Ref = samples - (long)samples;
}


void suspend(){

    memset(&bufferLeft, 0, len*sizeof(double));
    memset(&bufferRight, 0, len*sizeof(double));
}


double DelayLine::delayLineL(double x)
{
    
    double y;
    
  /*  y = *rptrLeft++; //better first read
    *wptrLeft++ = x;
    if (rptrLeft - bufferLeft >= len) { rptrLeft -= len; }
    if (wptrLeft - bufferLeft >= len) { wptrLeft -= len; }*/
    
    double *rpi = (rptrLeft+1);
    if (rpi - bufferLeft >= len) { rpi -= len; }
    
    y = (1 - fracDelaySamplesLeft) * *rptrLeft + fracDelaySamplesLeft * *(rpi) - y_1_L * (1 - fracDelaySamplesLeft);
    
    y_1_L = y;
    rptrLeft += 1;
    
    *wptrLeft++ = x;
    
    if (rptrLeft - bufferLeft >= len) { rptrLeft -= len; }
    if (wptrLeft - bufferLeft >= len) { wptrLeft -= len; }
    
    rptrLeft_copy = rptrLeft;
    
    if(crossfadeNeededL){
        y += *rptrLeft_copy++ * sqrt(0.5f * (1.0f + t_L));
        while (t_L < 1){ t_L += 0.008;}
    }
    
    return y;
}

double DelayLine::delayLineR(double x)
{
    
    double y;
    
    double *rpi = (rptrRight+1);
    if (rpi - bufferRight >= len) { rpi -= len; }
    
    y = (1 - fracDelaySamplesRight) * *rptrRight + fracDelaySamplesRight * *(rpi) - y_1_R * (1 - fracDelaySamplesRight);
    y_1_R = y;
    rptrRight += 1;

    *wptrRight++ = x;
    
    if (rptrRight - bufferRight >= len) { rptrRight -= len; }
    if (wptrRight - bufferRight >= len) { wptrRight -= len; }
    
    return y;
}

double DelayLine::delayLine_Ref_L(double x)
{
    
    double y;
    
    *wptrLeft_Ref++ = x;
    
    double *rpi = (rptrLeft_Ref+1);
    if (rpi - bufferLeft_Ref >= len_ref) { rpi -= len_ref; }
    
    y = (1 - fracDelaySamplesLeft_Ref) * *rptrLeft_Ref + fracDelaySamplesLeft_Ref * *(rpi) - y_1_L_ref * (1 - fracDelaySamplesLeft_Ref);
    
    y_1_L_ref = y;
    rptrLeft_Ref += 1;
    
    if (rptrLeft_Ref - bufferLeft_Ref >= len_ref) { rptrLeft_Ref -= len_ref; }
    if (wptrLeft_Ref - bufferLeft_Ref >= len_ref) { wptrLeft_Ref -= len_ref; }
    
    return y;

}

double DelayLine::delayLine_Ref_R(double x)
{
    
    double y;
    
    *wptrRight_Ref++ = x;
    
    double *rpi = (rptrRight_Ref+1);
    if (rpi - bufferRight_Ref >= len_ref) { rpi -= len_ref; }
    
    y = (1 - fracDelaySamplesRight_Ref) * *rptrRight_Ref + fracDelaySamplesRight_Ref * *(rpi) - y_1_R_ref * (1 - fracDelaySamplesRight_Ref);
    
    y_1_R_ref = y;
    rptrRight_Ref += 1;
    
    if (rptrRight_Ref - bufferRight_Ref >= len_ref) { rptrRight_Ref -= len_ref; }
    if (wptrRight_Ref - bufferRight_Ref >= len_ref) { wptrRight_Ref -= len_ref; }
    
    return y;
}

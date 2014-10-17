#include <math.h>

#include "BasicDef.h"

double dot(double vec0[], double vec1[])
{
    return(vec0[X] * vec1[X] + vec0[Y] * vec1[Y] + vec0[Z] * vec1[Z]);
}

void cross(double vec0[], double vec1[], double vec2[])
{
    vec2[X] = vec0[Y] * vec1[Z] - vec0[Z] * vec1[Y];
    vec2[Y] = vec0[Z] * vec1[X] - vec0[X] * vec1[Z];
    vec2[Z] = vec0[X] * vec1[Y] - vec0[Y] * vec1[X];
}

void normVec(double vec[])
{
    double norm;
    norm = sqrt(vec[X] * vec[X] + vec[Y] * vec[Y] + vec[Z] * vec[Z]);
    vec[X] /= norm;
    vec[Y] /= norm;
    vec[Z] /= norm;
}

void normal(double p0[], double p1[], double p2[], double normal[])
{
    unsigned int i;
    double v0[3], v1[3];
    
    for (i = 0; i < 3; i++) {
        v0[i] = p2[i] - p1[i];
        v1[i] = p0[i] - p1[i];
    }
    
    cross(v0, v1, normal);
    
    normVec(normal);
}
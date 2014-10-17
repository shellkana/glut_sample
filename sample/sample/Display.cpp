#include <GLUT/glut.h>
#include <math.h>

#include "BasicDef.h"

double eye[3];
double center[3] = {0.0, 0.0, 0.0};
double up[3];

double x_axis[3], y_axis[3], z_axis[3];

double pixel_size, ref_far, ref_near;


extern double point[MAX_NUM_POINTS][3];
extern unsigned int num_points;

extern double dot(double vec0[], double vec1[]);
extern void cross(double vec0[], double vec1[], double vec2[]);
extern void normVec(double vec[]);
extern void normal(double p0[], double p1[], double p2[], double normal[]);

void defineViewMatrix(double phi, double theta, unsigned int width, unsigned int height, double margin)
{
    unsigned int i, j;
    double c, s, xy_dist;
    double vec[3];
    double left, right, bottom, top, farVal, nearVal;
    double dx, dy, d_aspect, w_aspect, d;
    
    eye[Z] = sin(theta * PI / 180.0);
    xy_dist = cos(theta * PI / 180.0);
    c = cos(phi * PI / 180.0);
    s = sin(phi * PI / 180.0);
    eye[X] = xy_dist * c;
    eye[Y] = xy_dist * s;
    up[X] = - c * eye[Z];
    up[Y] = - s * eye[Z];
    up[Z] = s * eye[Y] + c * eye[X];
    normVec(up);
    
    for (i = 0; i < 3; i++)
        z_axis[i] = eye[i] - center[i];
    normVec(z_axis);
    cross(up, z_axis, x_axis);
    normVec(x_axis);
    cross(z_axis, x_axis, y_axis);
    
    left = bottom = farVal = LARGE;
    right = top = nearVal = - LARGE;
    for (i = 0; i < num_points; i++) {
        for (j = 0; j < 3; j++)
            vec[j] = point[i][j] - eye[j];
        if (dot(x_axis, vec) < left)
            left = dot(x_axis, vec);
        if (dot(x_axis, vec) > right)
            right = dot(x_axis, vec);
        if (dot(y_axis, vec) < bottom)
            bottom = dot(y_axis, vec);
        if (dot(y_axis, vec) > top)
            top = dot(y_axis, vec);
        if (dot(z_axis, vec) < farVal)
            farVal = dot(z_axis, vec);
        if (dot(z_axis, vec) > nearVal)
            nearVal = dot(z_axis, vec);
    }
    
    margin += (right - left) * 0.05;
    left -= margin;
    right += margin;
    bottom -= margin;
    top += margin;
    farVal -= margin;
    nearVal += margin;
    ref_far = farVal;
    ref_near = nearVal;
    
    dx = right - left;
    dy = top - bottom;
    d_aspect = dy / dx;
    w_aspect = (double)height / (double)width;
    
    if (w_aspect > d_aspect) {
        d = (dy * (w_aspect / d_aspect - 1.0)) * 0.5;
        bottom -= d;
        top += d;
        
    } else {
        d = (dx * (d_aspect / w_aspect - 1.0)) * 0.5;
        left -= d;
        right += d;
    }
    
    pixel_size = (right - left) / width;
    
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(left, right, bottom, top, - nearVal, - farVal);
    glViewport(0, 0, width, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eye[X], eye[Y], eye[Z], center[X], center[Y], center[Z], up[X], up[Y], up[Z]);
}
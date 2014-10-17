#include <GLUT/glut.h>
#include <math.h>
#include "BasicDef.h"
#include <iostream>
#define PHI 30.0
#define THETA 30.0
double phi = PHI;
double theta = THETA;

#define INIT_X_POS 128
#define INIT_Y_POS 128
#define INIT_WIDTH 512
#define INIT_HEIGHT 512

unsigned int window_width = INIT_WIDTH;
unsigned int window_height = INIT_HEIGHT;
double point[MAX_NUM_POINTS][3];
unsigned int num_points;
int GLframe = 0;
int GLtimenow = 0;
int GLtimebase = 0;

extern double eye[3];

int mouse_old_x, mouse_old_y;
bool motion_p;

float anim_dt = 0.0166f;

float pot_pos[3] = {0.0f, 0.0f, 2.45f};
float pot_v[3] = {0.0f, 1.0f, 0.0f};
float pot_a[3] = {0.0f, 0.0f, -9.8f};

float world_size[4] = {4.9f, 4.9f, 4.9f, 1.0f};
float world_origin[4] = {-2.45f, -2.45f, -2.45f, 1.0f};

double camera_pos[3] = {0.0,0.0,0.0};
double camera_center_pos[3] = {0.0,0.0,0.0};
double camera_up[3] = {0.0,0.0,1.0};
bool useCamera = true;

extern void defineViewMatrix(double phi, double theta, unsigned int width, unsigned int height, double margin);

void updatePot()
{
    pot_v[2] += pot_a[2] * anim_dt;
    pot_pos[2] += pot_v[2] * anim_dt;
    if (pot_pos[2] < -2.45) {
        pot_pos[2] = -2.45;
        pot_v[2] = -0.8 * pot_v[2];
    }
    double xy_dist = cos(theta * PI / 180.0);
    double c = cos(phi * PI / 180.0);
    double s = sin(phi * PI / 180.0);
    camera_pos[0] = pot_pos[0] + xy_dist * c * 2;
    camera_pos[1] = pot_pos[1] + xy_dist * s * 2;
    camera_pos[2] = pot_pos[2] + sin(theta * PI / 180.0) * 2;
    camera_center_pos[0] = pot_pos[0];
    camera_center_pos[1] = pot_pos[1];
    camera_center_pos[2] = pot_pos[2];
}

void display(void)
{
    GLframe++;
    GLtimenow = glutGet(GLUT_ELAPSED_TIME);
    if (GLtimenow - GLtimebase > 1000) {
        printf("fps:%f\r", GLframe * 1000.0 / (GLtimenow - GLtimebase));
        GLtimebase = GLtimenow;
        GLframe = 0;
    }
    updatePot();
    GLfloat light_pos[4];

    if (useCamera) {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(30.0, (double)window_width/ (double)window_height, 1.0, 100.0);
        glViewport(0, 0, window_width, window_height);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        gluLookAt(camera_pos[0], camera_pos[1], camera_pos[2], camera_center_pos[0], camera_center_pos[1], camera_center_pos[2], camera_up[0], camera_up[1], camera_up[2]);
        light_pos[0] = -5;
        light_pos[1] = -5;
        light_pos[2] = 5;
        light_pos[3] = 0.0f;
    } else {
        defineViewMatrix(phi, theta, window_width, window_height, 0.0);
        light_pos[0] = (float)eye[X];
        light_pos[1] = (float)eye[Y];
        light_pos[2] = (float)eye[Z];
        light_pos[3] = 0.0f;
    }

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glutWireCube(4.8);
    glEnable(GL_LIGHTING);
    glColor3f(1.0f, 1.0f, 1.0f);
    
    glPushMatrix();
    glTranslated(pot_pos[0], pot_pos[1], pot_pos[2]);
    glRotatef(90, 1, 0, 0);
    glutSolidTeapot(0.1);
    glPopMatrix();
    glDisable(GL_LIGHTING);

    glutSwapBuffers();
}

void resize(int width, int height)
{
    window_width = width;
    window_height = height;
}

void keyboard(unsigned char key, int /*x*/, int /*y*/)
{
    switch (key) {
        case 'q':
        case 'Q':
        case '\033':
            pot_v[2] = 10;
            break;
        case 'w':
            useCamera=!useCamera;
            break;
    }
    glutPostRedisplay();
}

void mouse_button(int button, int state, int x, int y)
{
    if ((state == GLUT_DOWN) && (button == GLUT_LEFT_BUTTON))
        motion_p = true;
    else if (state == GLUT_UP)
        motion_p = false;
    mouse_old_x = x;
    mouse_old_y = y;
}

void mouse_motion(int x, int y)
{
    int dx, dy;
    dx = x - mouse_old_x;
    dy = y - mouse_old_y;
    if (motion_p) {
        phi -= dx * 0.2;
        theta += dy * 0.2;
    }
    mouse_old_x = x;
    mouse_old_y = y;
    glutPostRedisplay();
}

void idle(void)
{
    glutPostRedisplay();
}

void initGL(void)
{
    glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClearDepth(1.0);
    glDepthFunc(GL_LESS);
    glEnable(GL_LIGHT0);
}

int main(int argc, char** argv)
{
    unsigned int i, j, k;
    num_points = 8;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            for (k = 0; k < 2; k++) {
                point[4 * i + 2 * j + k][X] = world_origin[0] + world_size[0] * i;
                point[4 * i + 2 * j + k][Y] = world_origin[1] + world_size[1] * j;
                point[4 * i + 2 * j + k][Z] = world_origin[2] + world_size[2] * k;
            }
        }
    }
    

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowPosition(INIT_X_POS, INIT_Y_POS);
    glutInitWindowSize(INIT_WIDTH, INIT_HEIGHT);
    glutCreateWindow("jumping Teapot");
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutMouseFunc(mouse_button);
    glutMotionFunc(mouse_motion);
    glutKeyboardFunc(keyboard);
    glutIdleFunc(idle);
    
    initGL();
    glutMainLoop();
    return 0;
}
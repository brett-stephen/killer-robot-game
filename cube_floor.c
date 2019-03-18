/*
Move a cube around with WASD

The cube is only allowed to along a 2 way street,
unless it is at an intersection, which are evenly
spaced every `BLOCK_LENGTH` units, where it may
go FORWARD, BACK, RIGHT, or LEFT.

The cube should face the direction it is pointing.

Ex. Intersections every 2 units
  |  |  |  |  |
--|--|--|--|--|--
  |  |  |  |  |
--|--|--|--|--|--
  |  |  |  |  |
*/
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

int BLOCK_LENGTH = 2;
int X_POS = 0;
int Y_POS = 0;
int Z_POS = 0;

enum facing
{
    FACE_FORWARD,
    FACE_RIGHT,
    FACE_BACK,
    FACE_LEFT
};
int FACING_STATE = FACE_FORWARD;

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 2.0, -5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    glTranslatef((GLfloat)X_POS, (GLfloat)Y_POS, (GLfloat)Z_POS);
    switch (FACING_STATE)
    {
    // The following directions are based on a teapot
    // that is drawn facing left. It is likely that we
    // will draw the robot initally facing the camera,
    // so these rotations should be modified as such.
    case FACE_FORWARD:
        glRotatef(270, 0.0, 1.0, 0.0);
        break;
    case FACE_RIGHT:
        glRotatef(180, 0.0, 1.0, 0.0);
        break;
    case FACE_BACK:
        glRotatef(90, 0.0, 1.0, 0.0);
        break;
    case FACE_LEFT:
        glRotatef(0.0, 0.0, 1.0, 0.0);
        break;
    default:
        break;
    }
    glutWireTeapot(2);
    glutSwapBuffers();
}

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-4.0, 4.0, -4.0, 4.0, -1.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int can_move_x()
{
    // The cube can move left or right
    // if is on a horizontal street.
    return !(Z_POS % BLOCK_LENGTH);
}

int can_move_z()
{
    // The cube can move foward or back
    // if it is on a vertical street.
    return !(X_POS % BLOCK_LENGTH);
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 97: // a
        if (can_move_x())
        {
            printf("Moving left\n");
            FACING_STATE = FACE_LEFT;
            X_POS += 1;
        }
        break;
    case 115: // s
        if (can_move_z())
        {
            printf("Moving back\n");
            FACING_STATE = FACE_BACK;
            Z_POS -= 1;
        }
        break;
    case 100: // d
        if (can_move_x())
        {
            printf("Moving right\n");
            FACING_STATE = FACE_RIGHT;
            X_POS -= 1;
        }
        break;
    case 119: // w
        if (can_move_z())
        {
            FACING_STATE = FACE_FORWARD;
            printf("Moving forward\n");
            Z_POS += 1;
        }
        break;
    default:
        break;
    }
    printf("X_POS %d Y_POS %d Z_POS %d\n", X_POS, Y_POS, Z_POS);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(250, 250);
    glutInitWindowPosition(100, 100);
    glutCreateWindow(argv[0]);

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(display);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}

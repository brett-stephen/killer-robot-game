/*
Move a robot around with WASD. 

The robot can only move along the street (line) it 
is on. It can always turn around and can turn right or 
left it is at an intersection (in place), otherwise, 
nothing happens. 

Also, the follows the robot, by default from an elevated
position directly behind - looking at its position. 

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

#include "draw_robot.h"

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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    int *p = get_camera_offset();
    gluLookAt(p[0], p[1], p[2], X_POS, Y_POS, Z_POS, 0.0, 1.0, 0.0);

    // Draw the vertical lines
    glPushMatrix();
    glColor3f(0.0, 1.0, 0.0);
    for (int i = -2; i < 5; i += 2)
    {
        glBegin(GL_LINES);
        glVertex3f((GLfloat)i, 0.0, -100.0);
        glVertex3f((GLfloat)i, 0.0, 100.0);
        glEnd();
    }
    glPopMatrix();

    // Draw the horizontal lines
    glPushMatrix();
    glColor3f(0.0, 0.0, 1.0);
    for (int i = -2; i < 5; i += 2)
    {
        glBegin(GL_LINES);
        glVertex3f(-100.0, 0.0, (GLfloat)i);
        glVertex3f(100.0, 0.0, (GLfloat)i);
        glEnd();
    }
    glPopMatrix();

    glTranslatef((GLfloat)X_POS, (GLfloat)Y_POS, (GLfloat)Z_POS);

    switch (FACING_STATE)
    // Rotate the robot according to the direction it is facing. 
    {
    case FACE_FORWARD:
        glRotatef(0.0, 0.0, 1.0, 0.0);
        break;
    case FACE_RIGHT:
        glRotatef(270.0, 0.0, 1.0, 0.0);
        break;
    case FACE_BACK:
        glRotatef(180.0, 0.0, 1.0, 0.0);
        break;
    case FACE_LEFT:
        glRotatef(90.0, 0.0, 1.0, 0.0);
        break;
    default:
        break;
    }

    draw_robot();

    glutSwapBuffers();
}

void init(void)
{
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_FLAT);
    glEnable(GL_DEPTH_TEST);
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

int *get_camera_offset()
{
    // Return the offset (array of x, y, z) from the
    // robot center to the camera 'eye' coordinates. 
    static int xyz[3];

    xyz[1] = Y_POS + 2;

    switch (FACING_STATE)
    {
    case FACE_FORWARD:
        xyz[0] = X_POS;
        xyz[2] = Z_POS - 5;
        break;
    case FACE_BACK:
        xyz[0] = X_POS;
        xyz[2] = Z_POS + 5;
        break;
    case FACE_RIGHT: 
        xyz[0] = X_POS + 5; 
        xyz[2] = Z_POS;
        break;
    case FACE_LEFT: 
        xyz[0] = X_POS - 5;
        xyz[2] = Z_POS;
    default:
        break;
    }
    return xyz;
}

int can_move_x()
{
    // Assert that the robot is on a horizontal street.
    return !(Z_POS % BLOCK_LENGTH);
}

int can_move_z()
{
    // Assert that the robot is on a vertical street. 
    return !(X_POS % BLOCK_LENGTH);
}

void turn_right_if_possible() 
{
    // If we are at an intersection, turn the robot right.
    if (can_move_x() && can_move_z()) 
    {
        switch(FACING_STATE) 
        {
            case FACE_FORWARD: 
                FACING_STATE = FACE_RIGHT;
                break;
            case FACE_BACK: 
                FACING_STATE = FACE_LEFT;
                break;
            case FACE_RIGHT: 
                FACING_STATE = FACE_BACK;
                break;
            case FACE_LEFT: 
                FACING_STATE = FACE_FORWARD;
                break;
        }
    }   
}

void turn_left_if_possible() 
{
    // If we are at an intersection, turn the robot left.
    if (can_move_x() && can_move_z()) 
    {
        switch(FACING_STATE) 
        {
        case FACE_BACK: 
            FACING_STATE = FACE_RIGHT;
            break;
        case FACE_FORWARD: 
            FACING_STATE = FACE_LEFT;
            break;
        case FACE_RIGHT: 
            FACING_STATE = FACE_FORWARD;
            break;
        case FACE_LEFT: 
            FACING_STATE = FACE_BACK;
            break;
        }
    }
}

void turn_around() 
{
    // Turn the robot in the opposite direction it is facing.
    switch(FACING_STATE) 
    {
        case FACE_BACK: 
            FACING_STATE = FACE_FORWARD; 
            break;
        case FACE_FORWARD: 
            FACING_STATE = FACE_BACK;
            break;
        case FACE_RIGHT: 
            FACING_STATE = FACE_LEFT;
            break;
        case FACE_LEFT: 
            FACING_STATE = FACE_RIGHT;
            break;
    }
}

void move_forward() 
{
    // Move the robot forward in the direction it is facing.
    if (can_move_x()) 
    {
        // Move right or left.
        switch(FACING_STATE) 
        {
            case FACE_RIGHT: 
                X_POS -= 1; 
                break;
            case FACE_LEFT: 
                X_POS += 1;
                break;
            default: 
                break;
        }
    }
    if (can_move_z())
    {
        // Move up or down.
        switch(FACING_STATE) 
        {
            case FACE_BACK: 
                Z_POS -= 1; 
                break;
            case FACE_FORWARD: 
                Z_POS += 1;
                break;
            default: 
                break;
        }
    }
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 119: // "w"
        move_forward();
        break;
    case 97: // "a"
        turn_left_if_possible();
        break;
    case 100: // "d"
        turn_right_if_possible();
        break;
    case 115: // "s"
        turn_around();
        break;
    default:
        printf("Not a movement key: %d\n", key);
        fflush(stdout);
        break;
    }
    printf("X_POS %d Y_POS %d Z_POS %d\n", X_POS, Y_POS, Z_POS);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
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

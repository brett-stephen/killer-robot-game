// Project: Manhattan Massacre
// University of Lethbridge - CPSC3710
// Project Deadline: April 1, 2019
// Gideon Richter, Brett Dziedzic, Michelle Le, Sean Herridge-Berry

/*
Move a robot around with WASD. 

The robot can only move along the street (line) it 
is on. It can always turn around and can turn right or 
left it is at an intersection (in place), otherwise, 
nothing happens. 

Also, the camera follows the robot, by default from an elevated
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

#include "cube_floor.h"
#include "draw_robot.h"

int BLOCK_LENGTH = 2;
int X_POS = 0;
int Y_POS = 0;
int Z_POS = 0;

int FACING_STATE = FACE_FORWARD;

int ALT_CAMERA_STATE = DEFAULT;

// void display(void)
// {
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//     glMatrixMode(GL_MODELVIEW);
//     glLoadIdentity();

//     float *p = get_camera_offset();
//     gluLookAt(p[0], p[1], p[2], X_POS, Y_POS, Z_POS, 0.0, 1.0, 0.0);

//     // Draw the vertical lines
//     glPushMatrix();
//     glColor3f(0.0, 1.0, 0.0);
//     for (int i = -2; i < 5; i += 2)
//     {
//         glBegin(GL_LINES);
//         glVertex3f((GLfloat)i, 0.0, -4.0);
//         glVertex3f((GLfloat)i, 0.0, 4.0);
//         glEnd();
//     }
//     glPopMatrix();

//     // Draw the horizontal lines
//     glPushMatrix();
//     glColor3f(0.0, 0.0, 1.0);
//     for (int i = -2; i < 5; i += 2)
//     {
//         glBegin(GL_LINES);
//         glVertex3f(-3.0, 0.0, (GLfloat)i);
//         glVertex3f(3.0, 0.0, (GLfloat)i);
//         glEnd();
//     }
//     glPopMatrix();

//     glTranslatef((GLfloat)X_POS, (GLfloat)Y_POS, (GLfloat)Z_POS);

//     switch (FACING_STATE)
//     // Rotate the robot according to the direction it is facing. 
//     {
//     case FACE_FORWARD:
//         glRotatef(0.0, 0.0, 1.0, 0.0);
//         break;
//     case FACE_RIGHT:
//         glRotatef(270.0, 0.0, 1.0, 0.0);
//         break;
//     case FACE_BACK:
//         glRotatef(180.0, 0.0, 1.0, 0.0);
//         break;
//     case FACE_LEFT:
//         glRotatef(90.0, 0.0, 1.0, 0.0);
//         break;
//     default:
//         break;
//     }

//     draw_robot();

//     glutSwapBuffers();
// }

// void init(void)
// {
//     glClearColor(0.0, 0.0, 0.0, 0.0);
//     glShadeModel(GL_FLAT);
//     glEnable(GL_DEPTH_TEST);
// }

// void reshape(int w, int h)
// {
//     glViewport(0, 0, (GLsizei)w, (GLsizei)h);
//     glMatrixMode(GL_PROJECTION);
//     glLoadIdentity();
//     glOrtho(-4.0, 4.0, -4.0, 4.0, -1.0, 500.0);
//     glMatrixMode(GL_MODELVIEW);
//     glLoadIdentity();
// }

// Method to calculate the new location of the camera after an F-key has been selected
float *get_camera_offset()
{
    // Return the offset (array of x, y, z) from the
    // robot center to the camera 'eye' coordinates. 
    static float xyz[3];

    xyz[1] = Y_POS + 2;

    switch (FACING_STATE)
    {
    case FACE_FORWARD:
        xyz[0] = X_POS;
        xyz[2] = Z_POS - 5;

        switch (ALT_CAMERA_STATE)
        {
        case BACK_LEFT: 
            xyz[0]+=5;
			break;
        case BACK_LEFT_FAR: 
            xyz[0]+=100;
            xyz[1]+=100;
            xyz[2]-=100;
			break;
		case BACK_RIGHT: 
            xyz[0]-=5;
			break;
        case BACK_RIGHT_FAR: 
            xyz[0]-=100;
            xyz[1]+=100;
            xyz[2]-=100;
			break;            
        case FRONT_LEFT: 
            xyz[0]+=5;
            xyz[2]+=10;
			break;
        case FRONT_LEFT_FAR: 
            xyz[0]+=100;
            xyz[1]+=100;
            xyz[2]+=100;
			break;              
		case FRONT_RIGHT: 
            xyz[0]-=5;
            xyz[2]+=10;
			break;
        case FRONT_RIGHT_FAR: 
            xyz[0]-=100;
            xyz[1]+=100;
            xyz[2]+=100;
			break;             
		default: 
			break;
        }
        break;
    case FACE_BACK:
        xyz[0] = X_POS;
        xyz[2] = Z_POS + 5;

        switch (ALT_CAMERA_STATE)
        {
        case BACK_LEFT: 
            xyz[0]-=5;
			break;
        case BACK_LEFT_FAR: 
            xyz[0]-=100;
            xyz[1]+=100;
            xyz[2]+=100;
			break;
		case BACK_RIGHT: 
            xyz[0]+=5;
			break;
        case BACK_RIGHT_FAR: 
            xyz[0]+=100;
            xyz[1]+=100;
            xyz[2]+=100;
			break;            
        case FRONT_LEFT: 
            xyz[0]-=5;
            xyz[2]-=10;
			break;
        case FRONT_LEFT_FAR: 
            xyz[0]-=100;
            xyz[1]+=100;
            xyz[2]-=100;
			break;              
		case FRONT_RIGHT: 
            xyz[0]+=5;
            xyz[2]-=10;
			break;
        case FRONT_RIGHT_FAR: 
            xyz[0]+=100;
            xyz[1]+=100;
            xyz[2]-=100;
			break;             
		default: 
			break;
        }
        break;
    case FACE_RIGHT: 
        xyz[0] = X_POS + 5; 
        xyz[2] = Z_POS;

        switch (ALT_CAMERA_STATE)
        {
        case BACK_LEFT: 
            xyz[2]+=5;
			break;
        case BACK_LEFT_FAR: 
            xyz[0]+=100;
            xyz[1]+=100;
            xyz[2]+=100;
			break;
		case BACK_RIGHT: 
            xyz[2]-=5;
			break;
        case BACK_RIGHT_FAR: 
            xyz[0]+=100;
            xyz[1]+=100;
            xyz[2]+=100;
			break;            
        case FRONT_LEFT: 
            xyz[0]-=10;
            xyz[2]+=5;
			break;
        case FRONT_LEFT_FAR: 
            xyz[0]-=100;
            xyz[1]+=100;
            xyz[2]+=100;
			break;              
		case FRONT_RIGHT: 
            xyz[0]-=10;
            xyz[2]-=5;
			break;
        case FRONT_RIGHT_FAR: 
            xyz[0]-=100;
            xyz[1]+=100;
            xyz[2]-=100;
			break;             
		default: 
			break;
        }
        break;        
    case FACE_LEFT: 
        xyz[0] = X_POS - 5;
        xyz[2] = Z_POS;
        switch (ALT_CAMERA_STATE)
        {
        case BACK_LEFT: 
            xyz[2]-=5;
			break;
        case BACK_LEFT_FAR: 
            xyz[0]-=100;
            xyz[1]+=100;
            xyz[2]-=100;
			break;
		case BACK_RIGHT: 
            xyz[2]+=5;
			break;
        case BACK_RIGHT_FAR: 
            xyz[0]-=100;
            xyz[1]+=100;
            xyz[2]+=100;
			break;            
        case FRONT_LEFT: 
            xyz[0]+=10;
            xyz[2]-=5;
			break;
        case FRONT_LEFT_FAR: 
            xyz[0]+=100;
            xyz[1]+=100;
            xyz[2]-=100;
			break;              
		case FRONT_RIGHT: 
            xyz[0]+=10;
            xyz[2]+=5;
			break;
        case FRONT_RIGHT_FAR: 
            xyz[0]+=100;
            xyz[1]+=100;
            xyz[2]+=100;
			break;             
		default: 
			break;
        }
        break;         
    default:
        break;
    }
    return xyz;
}

// Method to test if the robot is capable of moving in the x direction
//
int can_move_x()
{
    // Assert that the robot is on a horizontal street.
    return !(Z_POS % BLOCK_LENGTH);
}

// Method to test if the robot is capable of moving in the z direction
//
int can_move_z()
{
    // Assert that the robot is on a vertical street. 
    return !(X_POS % BLOCK_LENGTH);
}

// Method to implement the movement of the robot to the right
//  Calls both can_move_x() and can_move_z() to ensure movement is possible
//
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

// Method to implement the movement of the robot to the left
//  Calls both can_move_x() and can_move_z() to ensure movement is possible
//
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

// Method to implement the movement of the robot in the opposite direction
//
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

// Keyboard callback function for movement of the robot in the 4 cardinal directions
//
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
//TODO: implement the "pause" and "return" functions from the project description
    default:
        printf("Not a movement key: %d\n", key);
        fflush(stdout);
        break;
    }
    printf("X_POS %d Y_POS %d Z_POS %d\n", X_POS, Y_POS, Z_POS);
}

void special_keyboard_up(int key, int x, int y)
{
	switch(key)
	{
    case 1: // "F1"
    case 2: // "F2"
    case 3: // "F3" 
        ROBOT_HEAD_DIRECTION = FORWARD;
        break;
	default: 
		printf("Not a special control key up: %d\n", key);
		fflush(stdout);
		break;
	}
}

// Keyboard callback function for movement of the observing camera in various directions
//
void special_keyboard(int key, int x, int y)
{
	switch(key)
	{
        case 1: // "F1"
            ROBOT_HEAD_DIRECTION = FORWARD;
            break;
        case 2: // "F2"
            ROBOT_HEAD_DIRECTION = LEFT;
            break;
        case 3: // "F3" 
            ROBOT_HEAD_DIRECTION = RIGHT;
            break;
		case 4: // "F4" 
			ALT_CAMERA_STATE = DEFAULT;
			printf("Camera state: default\n");
			fflush(stdout);
			break;
		case 5: // "F5"
			ALT_CAMERA_STATE = BACK_LEFT;
			printf("Camera state: Back left\n");
			fflush(stdout);
			break;
		case 6: // "F6"
			ALT_CAMERA_STATE = BACK_RIGHT;
			printf("Camera state: Back right\n");
			fflush(stdout);
			break;
		case 7: // "F7"
			ALT_CAMERA_STATE = FRONT_RIGHT;
			printf("Camera state: Front right\n");
			fflush(stdout);
			break;
		case 8: // "F8"
			ALT_CAMERA_STATE = FRONT_LEFT;
			printf("Camera state: Front left\n");
			fflush(stdout);
			break;
		case 9: // "F9"
			ALT_CAMERA_STATE = BACK_LEFT_FAR;
			printf("Camera state: Back left far\n");
			fflush(stdout);
			break;
		case 10: // "F10"
			ALT_CAMERA_STATE = BACK_RIGHT_FAR;
			printf("Camera state: Back right far\n");
			fflush(stdout);
			break;
		case 11: // "F11"
			ALT_CAMERA_STATE = FRONT_RIGHT_FAR;
			printf("Camera state: Front right far\n");
			fflush(stdout);
			break;
		case 12: // "F12"
			ALT_CAMERA_STATE = FRONT_LEFT_FAR;
			printf("Camera state: Front left far\n");
			fflush(stdout);
			break;
		default: 
			printf("Not a special control key: %d\n", key);
			fflush(stdout);
			break;
		
	}
}

// //Main Loop
// int main(int argc, char **argv)
// {
//     glutInit(&argc, argv);
//     glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
//     glutInitWindowSize(250, 250);
//     glutInitWindowPosition(100, 100);
//     glutCreateWindow(argv[0]);

//     init();

//     glutDisplayFunc(display);
//     glutReshapeFunc(reshape);
//     glutIdleFunc(display);
//     glutKeyboardFunc(keyboard);
//     glutSpecialFunc(special_keyboard);
//     glutSpecialUpFunc(special_keyboard_up);

//     glutMainLoop();

//     return 0;
// }

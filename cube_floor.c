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

int BLOCK_LENGTH = 6; // Defines the size of a city block
int X_POS = 0;
int Y_POS = 0;
int Z_POS = 0;

int FACING_STATE = FACE_FORWARD;

int ALT_CAMERA_STATE = DEFAULT;

// Method to calculate the new location of the camera after an F-key has been selected
float *get_camera_offset()
{
    // Return the offset (array of x, y, z) from the
    // robot center to the camera 'eye' coordinates. 
    static float xyz[3];

    float camera_height = Y_POS + 4.0;
    xyz[1] = camera_height;

    // Equal XYZ distance to move camera for F9, F10, F11, and F12 angles
    float far_camera_distance = 20.0;
    // Distance left/right of the robot for F5, F6, F7, and F8 angles
    float close_camera_lateral_offset = 3.0;
    // Distance to follow the robot from behind
    float close_camera_follow_distance = 5.0;
    // Distance to move camera such that it is facing the front of the robot
    float close_camera_front_offset = close_camera_follow_distance * 2.0;

    // Here begins the,
    // The Unholy Relative Camera Movement Switch Statement of Doom.
    switch (FACING_STATE)
    {
    case FACE_FORWARD:
        xyz[0] = X_POS;
        xyz[2] = Z_POS - close_camera_follow_distance;

        switch (ALT_CAMERA_STATE)
        {
        case BACK_LEFT: 
            xyz[0]+=close_camera_lateral_offset;
			break;
        case BACK_LEFT_FAR: 
            xyz[0]+=far_camera_distance;
            xyz[1]+=far_camera_distance;
            xyz[2]-=far_camera_distance;
			break;
		case BACK_RIGHT: 
            xyz[0]-=close_camera_lateral_offset;
			break;
        case BACK_RIGHT_FAR: 
            xyz[0]-=far_camera_distance;
            xyz[1]+=far_camera_distance;
            xyz[2]-=far_camera_distance;
			break;            
        case FRONT_LEFT: 
            xyz[0]+=close_camera_lateral_offset;
            xyz[2]+=close_camera_front_offset;
			break;
        case FRONT_LEFT_FAR: 
            xyz[0]+=far_camera_distance;
            xyz[1]+=far_camera_distance;
            xyz[2]+=far_camera_distance;
			break;              
		case FRONT_RIGHT: 
            xyz[0]-=close_camera_lateral_offset;
            xyz[2]+=10;
			break;
        case FRONT_RIGHT_FAR: 
            xyz[0]-=far_camera_distance;
            xyz[1]+=far_camera_distance;
            xyz[2]+=far_camera_distance;
			break;             
		default: 
			break;
        }
        break;
    case FACE_BACK:
        xyz[0] = X_POS;
        xyz[2] = Z_POS + close_camera_follow_distance;

        switch (ALT_CAMERA_STATE)
        {
        case BACK_LEFT: 
            xyz[0]-=close_camera_lateral_offset;
			break;
        case BACK_LEFT_FAR: 
            xyz[0]-=far_camera_distance;
            xyz[1]+=far_camera_distance;
            xyz[2]+=far_camera_distance;
			break;
		case BACK_RIGHT: 
            xyz[0]+=close_camera_lateral_offset;
			break;
        case BACK_RIGHT_FAR: 
            xyz[0]+=far_camera_distance;
            xyz[1]+=far_camera_distance;
            xyz[2]+=far_camera_distance;
			break;            
        case FRONT_LEFT: 
            xyz[0]-=close_camera_lateral_offset;
            xyz[2]-=close_camera_front_offset;
			break;
        case FRONT_LEFT_FAR: 
            xyz[0]-=far_camera_distance;
            xyz[1]+=far_camera_distance;
            xyz[2]-=far_camera_distance;
			break;              
		case FRONT_RIGHT: 
            xyz[0]+=close_camera_lateral_offset;
            xyz[2]-=close_camera_front_offset;
			break;
        case FRONT_RIGHT_FAR: 
            xyz[0]+=far_camera_distance;
            xyz[1]+=far_camera_distance;
            xyz[2]-=far_camera_distance;
			break;             
		default: 
			break;
        }
        break;
    case FACE_RIGHT: 
        xyz[0] = X_POS + close_camera_follow_distance; 
        xyz[2] = Z_POS;

        switch (ALT_CAMERA_STATE)
        {
        case BACK_LEFT: 
            xyz[2]+=close_camera_lateral_offset;
			break;
        case BACK_LEFT_FAR: 
            xyz[0]+=far_camera_distance;
            xyz[1]+=far_camera_distance;
            xyz[2]+=far_camera_distance;
			break;
		case BACK_RIGHT: 
            xyz[2]-=close_camera_lateral_offset;
			break;
        case BACK_RIGHT_FAR: 
            xyz[0]+=far_camera_distance;
            xyz[1]+=far_camera_distance;
            xyz[2]-=far_camera_distance;
			break;            
        case FRONT_LEFT: 
            xyz[0]-=close_camera_front_offset;
            xyz[2]+=close_camera_lateral_offset;
			break;
        case FRONT_LEFT_FAR: 
            xyz[0]-=far_camera_distance;
            xyz[1]+=far_camera_distance;
            xyz[2]+=far_camera_distance;
			break;              
		case FRONT_RIGHT: 
            xyz[0]-=close_camera_front_offset;
            xyz[2]-=close_camera_lateral_offset;
			break;
        case FRONT_RIGHT_FAR: 
            xyz[0]-=far_camera_distance;
            xyz[1]+=far_camera_distance;
            xyz[2]-=far_camera_distance;
			break;             
		default: 
			break;
        }
        break;        
    case FACE_LEFT: 
        xyz[0] = X_POS - close_camera_follow_distance;
        xyz[2] = Z_POS;
        switch (ALT_CAMERA_STATE)
        {
        case BACK_LEFT: 
            xyz[2]-=close_camera_lateral_offset;
			break;
        case BACK_LEFT_FAR: 
            xyz[0]-=far_camera_distance;
            xyz[1]+=far_camera_distance;
            xyz[2]-=far_camera_distance;
			break;
		case BACK_RIGHT: 
            xyz[2]+=close_camera_lateral_offset;
			break;
        case BACK_RIGHT_FAR: 
            xyz[0]-=far_camera_distance;
            xyz[1]+=far_camera_distance;
            xyz[2]+=far_camera_distance;
			break;            
        case FRONT_LEFT: 
            xyz[0]+=close_camera_front_offset;
            xyz[2]-=close_camera_lateral_offset;
			break;
        case FRONT_LEFT_FAR: 
            xyz[0]+=far_camera_distance;
            xyz[1]+=far_camera_distance;
            xyz[2]-=far_camera_distance;
			break;              
		case FRONT_RIGHT: 
            xyz[0]+=close_camera_front_offset;
            xyz[2]+=close_camera_lateral_offset;
			break;
        case FRONT_RIGHT_FAR: 
            xyz[0]+=far_camera_distance;
            xyz[1]+=far_camera_distance;
            xyz[2]+=far_camera_distance;
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

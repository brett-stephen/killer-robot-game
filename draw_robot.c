// Project: Manhattan Massacre
// University of Lethbridge - CPSC3710
// Project Deadline: April 1, 2019
// Gideon Richter, Brett Dziedzic, Michelle Le, Sean Herridge-Berry

#include <stdio.h>
#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "draw_robot.h"

int ROBOT_HEAD_DIRECTION = FORWARD;

int ROBOT_ANTENNA_ANGLE = 0;

void set_color_red() { glColor3f(1.0, 0.0, 0.0); }

void set_color_green() { glColor3f(0.0, 1.0, 0.0); }

void set_color_default() { glColor3f(0.0, 0.0, 0.0); }

void set_color_black() { glColor3f(0.0, 0.0, 0.0); }

void set_color_blue() { glColor3f(0.0, 0.0, 1.0); }

void draw_triangle(double size)
{
  glBegin(GL_TRIANGLES);
  glVertex3f(-0.25, 0.0, 0.0);
  glVertex3f(0.25, 0.0, 0.0);
  glVertex3f(0.0, 0.5, 0.0);
  glEnd();
}

void draw_body()
{
  set_color_red();
  glPushMatrix();
  glScalef(1.0, 1.5, 1.0);
  glutSolidCube(1.0);
  glPopMatrix();
  set_color_default();
}

void draw_front_rectangle()
{
  glPushMatrix();
  set_color_green();
  glTranslatef(0.0, 0.0, 0.51);
  glScalef(0.5, 1.0, 0.0);
  glutSolidCube(1.0);
  set_color_default();
  glPopMatrix();
}

void draw_rear_triangles()
{
  glPushMatrix();
  set_color_green();
  glTranslatef(0.0, 0.0, -0.51);
  glPushMatrix();
  // Draw the upper triangle
  glTranslatef(0.0, 0.0, 0.0);
  draw_triangle(1.0);
  glPopMatrix();
  glPushMatrix();
  // Draw the lower triangle
  glTranslatef(0.0, -0.5, 0.0);
  draw_triangle(1.0);
  glPopMatrix();
  set_color_default();
  glPopMatrix();
}

void draw_neck()
{
  glPushMatrix();
  set_color_green();
  glTranslatef(0.0, 0.9, 0.0);
  glRotatef(90, 1.0, 0.0, 0.0);
  GLUquadricObj *quadratic;
  quadratic = gluNewQuadric();
  gluCylinder(quadratic, 0.3, 0.3, 0.2, 32, 32);
  set_color_default();
  glPopMatrix();
}

void draw_antenna()
{
  glPushMatrix();
  set_color_red();
  // Spin the antenna 30 deg every frame
  glRotatef(ROBOT_ANTENNA_ANGLE, 0.0, 1.0, 0.0);
  // Draw the cylinder
  glTranslatef(0.0, 0.65, 0.0);
  glRotatef(90, 1.0, 0.0, 0.0);
  GLUquadricObj *quadratic;
  quadratic = gluNewQuadric();
  gluCylinder(quadratic, 0.1, 0.1, 0.4, 32, 32);
  glPushMatrix();
  set_color_green();
  // Draw the rotating cone
  glTranslatef(0.0, 0.4, 0.0);
  glRotatef(90.0, 1.0, 0.0, 0.0);
  glutSolidCone(0.2, 0.5, 10, 10);
  glPopMatrix();
  set_color_black();
  set_color_default();
  ROBOT_ANTENNA_ANGLE += 30;
  glPopMatrix();
}

void draw_eyes()
{
  glPushMatrix();
  // Draw the left eye
  set_color_green();
  glTranslatef(-0.15, 0.0, 0.36);
  glScalef(1.0, 1.0, 0.0);
  glutSolidSphere(0.1, 10, 10);
  set_color_default();
  glPopMatrix();
  glPushMatrix();
  // Draw the right eye
  set_color_green();
  glTranslatef(0.15, 0.0, 0.36);
  glScalef(1.0, 1.0, 0.0);
  glutSolidSphere(0.1, 10, 10);
  set_color_default();
  glPopMatrix();
}

void draw_head()
{
  // Draw the head, eyes, and antenna
  glPushMatrix();
  switch (ROBOT_HEAD_DIRECTION)
  {
  case RIGHT:
    glRotatef(-90, 0.0, 1.0, 0.0);
    break;
  case LEFT:
    glRotatef(90, 0.0, 1.0, 0.0);
    break;
  case FORWARD:
    break;
  default:
    break;
  }
  glTranslatef(0.0, 1.2, 0.0);
  glPushMatrix();
  set_color_blue();
  glScalef(1.0, 0.8, 1.0);
  glutSolidCube(0.7);
  set_color_default();
  glPopMatrix();
  draw_eyes();
  draw_antenna();
  glPopMatrix();
}

void draw_robot(void)
{
  draw_body();
  draw_front_rectangle();
  draw_rear_triangles();
  draw_neck();
  draw_head();
}

void robot_keyboard_callback(unsigned char key, int x, int y)
{
  switch(key) {
    case 112: // f12
      ROBOT_HEAD_DIRECTION = FORWARD;
      // TODO: also change the lookAt for the robot
      break;
    default: 
      break;
  }
}

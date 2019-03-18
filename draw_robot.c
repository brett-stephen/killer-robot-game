#include <stdlib.h>
#include <stdio.h>

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include "draw_robot.h"


int y_rotate = 0;
int x_rotate = 0;


// void cb_keyboard(unsigned char key, int x, int y)
// {
//   switch(key)
//   {
//     case 32:
//       y_rotate += 15;
//       break;
//     case 97:
//       x_rotate += 15;
//       break;
//     default:
//       printf ("KP: No action for %d.\n", key);
//       break;
//   }
// }

void set_color_red() {glColor3f(1.0, 0.0, 0.0);}

void set_color_green() {glColor3f(0.0, 1.0, 0.0);}

void set_color_default() {glColor3f(0.0, 0.0, 0.0);}

void set_color_black() {glColor3f(0.0, 0.0, 0.0);}

void set_color_blue() {glColor3f(0.0, 0.0, 1.0);}

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
    set_color_black();
    glTranslatef(0.0, 0.65, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    gluCylinder(quadratic, 0.1, 0.1, 0.4, 32, 32);
    set_color_default();
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

// void display(void)
// {
//   glClearColor(1.0, 1.0, 1.0, 0.0);
//   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//   gluLookAt(0.1,0.1,0.5,0.0,0.0,0.0,0.0,1.0,0.0);
//     // Start drawing the robot
//   set_color_default();
//   glMatrixMode(GL_MODELVIEW);
//   glLoadIdentity();
//   // Press space to rotate object
//   // TODO: Remove this so no import conflict
//   glRotatef(y_rotate, 0.0, 1.0, 0.0);
//   glRotatef(x_rotate, 1.0, 0.0, 0.0);
//   draw_robot();
//   glutSwapBuffers();
// }

// void init (void)
// {
//    glClearColor (0.0, 0.0, 0.0, 0.0);
//    glMatrixMode (GL_PROJECTION);
//    glEnable(GL_DEPTH_TEST);
//  	 glLoadIdentity ();
//  	 glOrtho(-2.0, 2.0, -2.0, 2.0, -5.0, 5.0);
// }

// int main(int argc, char** argv)
// {
//    glutInit(&argc, argv);
//    glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
//    glutInitWindowSize (250, 250);
//    glutInitWindowPosition (100, 100);
//    glutCreateWindow ("draw robot");
//    glutKeyboardFunc(&cb_keyboard);
//    init ();
//    glutDisplayFunc(display);
//    glutIdleFunc(display);
//    glutMainLoop();
//    return 0;
// }

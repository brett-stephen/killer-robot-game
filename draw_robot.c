#include <stdlib.h>
#include <stdio.h>

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>

int y_rotate = 0;
int x_rotate = 0;

void cb_keyboard(unsigned char key, int x, int y)
{
  switch(key)
  {
    case 32:
      y_rotate += 15;
      break;
    case 97:
      x_rotate += 15;
      break;
    default:
      printf ("KP: No action for %d.\n", key);
      break;
  }
}

void draw_triangle(double size)
{
  glBegin(GL_TRIANGLES);
  glVertex3f(-0.25, 0.0, 0.0);
  glVertex3f(0.25, 0.0, 0.0);
  glVertex3f(0.0, 0.5, 0.0);
  glEnd();
}

void display(void)
{
  printf("Things is %d\n", y_rotate);
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 0.0, 0.0);

  glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho(-2.0, 2.0, -2.0, 2.0, -5.0, 5.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.1,0.1,0.5,0.0,0.0,0.0,0.0,1.0,0.0);

  // Press space to rotate object
  glRotatef(y_rotate, 0.0, 1.0, 0.0);
  glRotatef(x_rotate, 1.0, 0.0, 0.0);

  // Draw the main body
  glPushMatrix();
    glScalef(1.0, 1.5, 1.0);
    glutWireCube(1.0);
  glPopMatrix();

  // Draw rectangle on front of  body
  glPushMatrix();
    glTranslatef(0.0, 0.0, 0.5);
    glScalef(0.5, 1.0, 0.0);
    glutWireCube(1.0);
  glPopMatrix();

  // Draw two rectangles on back of body
  glPushMatrix();
    glTranslatef(0.0, 0.0, -0.5);
    glPushMatrix();
      glTranslatef(0.0, 0.0, 0.0);
      draw_triangle(1.0);
    glPopMatrix();
    glPushMatrix();
      glTranslatef(0.0, -0.5, 0.0);
      draw_triangle(1.0);
    glPopMatrix();
  glPopMatrix();

  // Draw the neck
  glPushMatrix();
    glTranslatef(0.0, 0.9, 0.0);
    glRotatef(90, 1.0, 0.0, 0.0);
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    gluCylinder(quadratic, 0.3, 0.3, 0.2, 32, 32);
  glPopMatrix();

  glutSwapBuffers();
}

void init (void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (250, 250);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("draw robot");
   glutKeyboardFunc(&cb_keyboard);
   init ();
   glutDisplayFunc(display);
   glutIdleFunc(display);
   glutMainLoop();
   return 0;   /* We'll never be here.*/
}

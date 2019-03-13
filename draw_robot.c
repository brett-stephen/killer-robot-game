#include <stdlib.h>

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>

int y_rotate = 0

void cb_keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 102: // f
        y_rotate += 10;
	      break;
      case 114: // r
         X_Speed=-X_Speed;
         Y_Speed=-Y_Speed;
      break;
   default:
      printf ("KP: No action for %d.\n", key);
      break;
    }
}

void display(void)
{
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 0.0, 0.0);

  glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glOrtho(-2.0, 2.0, -2.0, 2.0, -5.0, 5.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.1,0.1,0.5,0.0,0.0,0.0,0.0,1.0,0.0);

  // glRotate(y_rotate, 0.0, 1.0, 0.0);

  glPushMatrix();
  glScalef(1.0, 1.5, 1.5);
  glutWireCube(1.0);
  glPopMatrix();

  glPushMatrix();
  glScalef(0.5, 1.0, 0.0);
  glTranslatef(0.0, 0.0, 1.0);
  glutWireCube(1.0);
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
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (250, 250);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("draw robot");
   glutKeyboardFunc(&cb_keyboard);
   init ();
   glutDisplayFunc(display);
   glutMainLoop();
   return 0;   /* We'll never be here.*/
}

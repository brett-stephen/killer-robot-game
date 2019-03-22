#include <GL/glut.h>
#include <stdlib.h>


struct building
{
   int health;
   double x; //X-coor of building relative to block center
   double z; //Z-coor of building relative to block center
   double sideLength; //Side length of building
   double height; //Height length of building   
};

struct building build={3,-10,-10,25,25};


/*struct building generateB()
{
      struct building test;
      test.health = 2;
      test.x=-10;
      test.z=25;
      test.sideLength =25;
      test.height = -10;

      return test;
      }*/
   
//struct building build = generateB();

void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glPushMatrix();
   glColor3f(1.0, 1.0, 1.0);
   glRectf(-25.0, -25.0, 25.0, 25.0);
   glPopMatrix();
   glutSwapBuffers();
}

void damageA(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glPushMatrix();
   glColor3f(0.8, 1.0, 1.0);
   glRectf(-5.0, -5.0, 5.0, 5.0);
   glPopMatrix();

   glutSwapBuffers();

}

void damageB(void)
{
   glClear(GL_COLOR_BUFFER_BIT);
   glPushMatrix();
   glColor3f(0.8, 0.2, 1.0);
   glRectf(-1, -1.0, 1.0, 1.0);
   glPopMatrix();

   glutSwapBuffers();

}

/*void displayB(building)
{
   build = generateB();
   }*/

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
}

void reshape(int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-50.0, 50.0, -50.0, 50.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void mouse(int button, int state, int x, int y) 
{
   switch (button) {
      case GLUT_LEFT_BUTTON:
         if (state == GLUT_DOWN)
	    build.health = build.health -1; 
	    if(build.health == 2)
	       glutIdleFunc(damageA);
	    else
	       glutIdleFunc(damageB);
         break;
      default:
         break;
   }
}
   
/* 
 *  Request double buffer display mode.
 *  Register mouse input callback functions
 */
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
   glutInitWindowSize (250, 250); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape); 
   glutMouseFunc(mouse);
   glutMainLoop();
   return 0;   /* We never return. */
}

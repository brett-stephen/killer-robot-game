#include <stdlib.h>
#include <stdio.h>
#include <GL/glut.h>

struct building
{
   int health;
   double x; //X-coor of building relative to block center
   double z; //Z-coor of building relative to block center
   double sideLength; //Side length of building
   double height; //Height length of building   
};

struct building build={3,-10,-10,25,25};

void init()
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
}

void drawObjects(GLenum mode)
{

   if(mode == GL_SELECT)
      glLoadName(1);

   if (build.health ==3){
      glColor3f(1.0, 0.0, 0.0);
      glRectf(-10, -10, 10.0, 10.0);
      printf("health = %d\n", build.health);
   }
   if (build.health ==2){
      glColor3f(1.0, 0.0, 0.0);
      glRectf(-5, -5, 5.0, 5.0);
      printf("health = %d\n", build.health);
   }
   if (build.health ==1){
      glColor3f(1.0, 0.0, 0.0);
      glRectf(-1.0, -1.0, 1, 1);
      printf("health = %d\n", build.health);
   }

}


void display()
{
   glClear(GL_COLOR_BUFFER_BIT);
   drawObjects(GL_RENDER);
   glFlush();
}

/*  processHits prints out the contents of the
 *  selection array.
 */
void processHits (GLint hits, GLuint buffer[])
{
   unsigned int i, j;
   GLint names, *ptr;

   printf ("hits = %d\n", hits);
   ptr = (GLint *) buffer;
   for (i = 0; i < hits; i++) {	/*  for each hit*/
      names = *ptr;
      ptr+=3;
      for (j = 0; j < names; j++) { /*  for each name*/
         if(*ptr==1) printf ("red rectangle\n");
         else printf ("blue rectangle\n");
         ptr++;
      }
      printf ("\n");
   }

}

#define SIZE 512

void mouse(int button, int state, int x, int y)
{
   GLuint selectBuf[SIZE];
   GLint hits;
   GLint viewport[4];

   if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
   {
      glGetIntegerv (GL_VIEWPORT, viewport);

      glSelectBuffer (SIZE, selectBuf);
      glRenderMode(GL_SELECT);

      glInitNames();
      glPushName(0);

      glMatrixMode (GL_PROJECTION);
      glPushMatrix ();
      glLoadIdentity ();
      /*  create 5x5 pixel picking region near cursor location	*/
      gluPickMatrix ((GLdouble) x, (GLdouble) (viewport[3] - y),
                     0.5, 0.5, viewport);
      gluOrtho2D (-20.0, 20.0, -20.0, 20.0);
      drawObjects(GL_SELECT);


      glMatrixMode (GL_PROJECTION);
      glPopMatrix ();
      glFlush ();

      hits = glRenderMode (GL_RENDER);
      build.health = build.health - hits;
      processHits (hits, selectBuf);

      glutPostRedisplay();
   }
}


void reshape(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D (-20.0, 20.0, -20.0, 20.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

/* Main Loop */
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutReshapeFunc (reshape);
   glutDisplayFunc(display);
   glutMouseFunc (mouse);
   glutMainLoop();
   return 0;
}

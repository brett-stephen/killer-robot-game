//////////////////////////////////////////////////////////
// Q5 for graphics 3710 - Brett Dzedzic
// Uses depth and culling on pyramid, sphere, cube
//////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>

#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <unistd.h>

#include <math.h>       /* fmod */

static float eyeX =5.0f, eyeY = 5.0f, eyeZ = -1.0f;

void drawBlock(double x1, double y1, double sidelength,double c1,double c2,double c3)
{
    double halfside = sidelength / 2;

    glColor3d(c1,c2,c3);
    glBegin(GL_POLYGON);

    glVertex3d(x1 + halfside, 0.0, y1 + halfside);
    glVertex3d(x1 + halfside, 0.0, y1 - halfside);
    glVertex3d(x1 - halfside, 0.0, y1 - halfside);
    glVertex3d(x1 - halfside, 0.0, y1 + halfside);

    glEnd();
}

void generateBlocks(int numColumns, int numRows, double sideLength, double gapLength)
{
  double xLength = (double)numColumns * (sideLength+gapLength);
  double yLength = (double)numRows * (sideLength+gapLength);

  double xOffset = -(xLength/2);
  double yOffset = -(yLength/2);

  //printf("%f %f \n",xLength,yLength);

  int i,j=0;

  for (i = 0; i < numColumns; i++)
  {
    for (j = 0; j < numRows; j++)
    {
      drawBlock(xOffset + ((sideLength+gapLength)*i) , yOffset + ((sideLength+gapLength)*j) ,sideLength,0.0,1.0,0.0);
    }
  }
}

void display(void)
{
   usleep(50000);

   /* clear all pixels  */
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   /* Set the color to black */
   //glColor3f (1.0, 1.0, 1.0);

   /* Clear the current matrix */
   glLoadIdentity();

	//printf("%f %f %f \n",eyeX, eyeY, eyeZ);
   /* Viewing transformation */
   gluLookAt(eyeX, eyeY, eyeZ,   /* Eye */
             0.0, 0.0, 0.0,   /* Look at */
             0.0, 1.0, 0.0);  /* Up vector */

  generateBlocks(5,5,0.5,1);

  drawBlock(0,0,100,0.5,0.5,0.5);

//Z AXIS VISUALIZATION
  glPushMatrix();
    glBegin(GL_LINES);

    glLineWidth(1);
    glColor4f(1, 0, 0, 0);
    glVertex3f(10.0, 0.0, 0.0);
    glVertex3f(-10.0, 0.0, 0.0);

    glColor4f(0, 0, 1, 0);
    glVertex3f(0.0, 0.0, 10.0);
    glVertex3f(0.0, 0.0, -10.0);

    glColor4f(0, 1, 0, 0);
    glVertex3f(0.0, 10.0, 0.0);
    glVertex3f(0.0, -10.0, 0.0);

    glEnd();
  glPopMatrix();

   glFlush ();
}


void init (void)
{
   /* select clearing color 	*/
   glClearColor (0.3, 0.53, 0.92, 0.0);
   glShadeModel(GL_FLAT);
   glEnable (GL_DEPTH_TEST);
}

void reshape(int w, int h)
{
   /* Set the view port */
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);

   /* Projection transformation */
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-1.0, 1.0, /* Left and right boundary */
             -1.0, 1.0, /* bottom and top boundary */
             1.5, 20.0); /* near and far boundary */
   glMatrixMode(GL_MODELVIEW);
}

//////////////////////////////////////////////////////////
// changes camera view
//////////////////////////////////////////////////////////
void mouse(int button, int state, int x, int y)
{
   switch (button) {
      case GLUT_RIGHT_BUTTON:
         if (state == GLUT_DOWN){
      		eyeX = 5;
      		eyeY = 5;
      		eyeZ = 5;
      	} else {
          eyeX = -5;
          eyeY = 5;
          eyeZ = -5;
        }
         break;
     case GLUT_LEFT_BUTTON:
        if (state == GLUT_DOWN){
         eyeX = -5;
         eyeY = 5;
         eyeZ = 5;
       } else {
         eyeX = -5;
         eyeY = 5;
         eyeZ = -5;
       }
        break;
      default:
         break;
   }
}

//////////////////////////////////////////////////////////
// spins or stops objects
//////////////////////////////////////////////////////////
void myCBKey(unsigned char key, int x, int y)
{

}

//////////////////////////////////////////////////////////
// main func
//////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
   glutInit(&argc, argv);


   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init();
   glutDisplayFunc(display);
   glutIdleFunc(display);
   glutReshapeFunc(reshape);
   glutMouseFunc(&mouse);
   glutKeyboardFunc(&myCBKey);
   glutMainLoop();
   return 0;   /* We'll never be here.*/
}

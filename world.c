//World Generation

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

//Open GL
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>


#define MAX_BUILDINGS 15
#define MAX_BLOCKS 100

static float eyeX =5.0f, eyeY = 5.0f, eyeZ = -1.0f; // for visualization, delete later.

struct building
{
   double x; //X-coor of building relative to block center
   double z; //Z-coor of building relative to block center
   double sideLength; //Side length of building
   double height; //Height length of building
};

struct block
{
   double x; //X-coor of block relative to world origin
   double z; //Z-coor of block relative to world origin
   double sideLength; //Side length of block
   struct building buildings[MAX_BUILDINGS]; // array of buildings on the block
};

struct block city[MAX_BLOCKS];

// generateCity
// inputs: number of rows and colums of blocks in city, size length of block, gap size between blocks.
// generates the model of the city as an array of blocks that contain an array of buildings.
// this function is called once during program initialization.
// callback render function can then render by iterating through city struct.
void generateCity(int rows, int columns, double blockSz, double gapSz)
{
  int numBlocks = rows*columns;

  if (numBlocks > MAX_BLOCKS)
  {
    return;
  }

  double xLength = (double)rows * (blockSz+gapSz);
  double yLength = (double)columns * (blockSz+gapSz);

  double xOffset = -(xLength/2);
  double yOffset = -(yLength/2);

  int i,j;

  for (i = 0; i < rows; i++)
  {
    for (j = 0; j < columns; j++)
    {
      city[(i+1)*j].x = xOffset + ((blockSz+gapSz)*i);
      city[(i+1)*j].z = yOffset + ((blockSz+gapSz)*j);
      city[(i+1)*j].sideLength = blockSz;
      //city[(i+1)*j].buildings[] = };
      /* {.x = xOffset + ((blockSz+gapSz)*i),
          .z = zOffset + ((blockSz+gapSz)*j),
          .sideLength = blockSz}; */
    }
  }

  return;
}

/*
Everything under here is old code - to be refactored.
*/

void drawBuilding(double x1, double z1, double xlength, double hlength, double zlength, double c1,double c2,double c3)
{
    double xhalfside = xlength / 2;
    double zhalfside = zlength / 2;

    glTranslatef(x1,0,z1);

    glColor3d(c1,c2,c3);
    glBegin(GL_QUADS);

   // Far face.  Green, 50% opaque, non-uniform texture cooridinates.
   glNormal3f( 0.0f, 0.0f,-1.0f);

   glVertex3f(-xhalfside, 0.0f, -zhalfside);
   glVertex3f(-xhalfside, hlength, -zhalfside);
   glVertex3f( xhalfside, hlength, -zhalfside);
   glVertex3f( xhalfside, 0.0f, -zhalfside);

   // Right face.  Blue; 25% opaque
   glNormal3f( 1.0f, 0.0f, 0.0f);

   glVertex3f( xhalfside, 0.0f, -zhalfside);
   glVertex3f( xhalfside, hlength, -zhalfside);
   glVertex3f( xhalfside, hlength,  zhalfside);
   glVertex3f( xhalfside, 0.0f,  zhalfside);

   // Front face; offset.  Multi-colored, 50% opaque.
   glNormal3f( 0.0f, 0.0f, 1.0f);

   glVertex3f(-xhalfside, 0.0f, zhalfside);
   glVertex3f( xhalfside, 0.0f, zhalfside);
   glVertex3f( xhalfside, hlength, zhalfside);
   glVertex3f(-xhalfside, hlength, zhalfside);


   // Left Face; offset.  Yellow, varying levels of opaque.
   glNormal3f(-1.0f, 0.0f, 0.0f);

   glVertex3f(-xhalfside, 0.0f, -zhalfside);
   glVertex3f(-xhalfside, 0.0f,  zhalfside);
   glVertex3f(-xhalfside, hlength,  zhalfside);
   glVertex3f(-xhalfside, hlength, -zhalfside);

   // All polygons have been drawn.
   glEnd();
}

void drawBlock(double x1, double y1, double z1, double sidelength,double c1,double c2,double c3)
{
    double halfside = sidelength / 2;

    glColor3d(c1,c2,c3);
    glBegin(GL_POLYGON);

    glVertex3d(x1 + halfside, y1 , z1 + halfside);
    glVertex3d(x1 + halfside, y1 , z1 - halfside);
    glVertex3d(x1 - halfside, y1 , z1 - halfside);
    glVertex3d(x1 - halfside, y1 , z1 + halfside);

    glEnd();

    //drawBuilding(1,1, 0.5, 3, 0.5, 0.4,0.4,0.5);
}

void drawBlock2(double sidelength,double c1,double c2,double c3)
{
    double halfside = sidelength / 2;

    glColor3d(c1,c2,c3);
    glBegin(GL_POLYGON);

    glVertex3d(halfside, 0 , halfside);
    glVertex3d(halfside, 0 , -halfside);
    glVertex3d(-halfside, 0 , -halfside);
    glVertex3d(-halfside, 0 , halfside);

    glEnd();

    int numbBuildings = rand()%15+3;
    unsigned int i;
    for (i = 0; i < numbBuildings; i++) {
      double x = (rand()%100)/100;
      double y = (rand()%100)/100;
      int h = rand()%3;
      //double x1, double z1, double xlength, double hlength, double zlength, double c1,double c2,double c3
      drawBuilding(x,y, 0.5, h, 0.5, 0.4,0.4,0.5);
    }
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
      glPushMatrix();
      //glTranslatef(i,0,j);
      glTranslatef(xOffset + ((sideLength+gapLength)*i),0, yOffset + ((sideLength+gapLength)*j) );
      drawBlock2(1,0.0,1.0,0.0);
      glPopMatrix();
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

  generateBlocks(6,6,1,1);

  drawBlock(0,-0.05,0,100,0.5,0.5,0.5);

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
             1.5, 150.0); /* near and far boundary */
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

///////////////////////////////d///////////////////////////
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

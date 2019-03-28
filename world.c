// Project: Manhattan Massacre
// University of Lethbridge - CPSC3710
// Project Deadline: April 1, 2019
// Gideon Richter, Brett Dziedzic, Michelle Le, Sean Herridge-Berry

//World Generation

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

//Open GL
#include <GL/gl.h>
#include <GL/glut.h>
#include <GL/glu.h>

#include "draw_robot.h"
#include "cube_floor.h"

#define MAX_BUILDINGS 6
#define BLOCKS_ROWS 20
#define BLOCKS_COLUMNS 20
#define BLOCKS_SZ 5.0
#define GAPS_SZ 1.0

static float eyeX =5.0f, eyeY = 5.0f, eyeZ = -1.0f; // for visualization, delete later.

// A struct that represents a building in a city, which has a health, position and color
struct building
{
   double x; //X-coor of building relative to block center
   double z; //Z-coor of building relative to block center
   double color[3];
   double sideLength; //Side length of building
   double height; //Height length of building
   int health;
};

// A struct that represents a block in a city which can contain buildings
struct block
{
   double x; //X-coor of block relative to world origin
   double z; //Z-coor of block relative to world origin
   double sideLength; //Side length of block
   struct building buildings[MAX_BUILDINGS]; // array of buildings on the block
};

struct block city[BLOCKS_ROWS * BLOCKS_COLUMNS];

extern int X_POS; 
extern int Y_POS;
extern int Z_POS;


//returns random double between 0 & 1
// used for city generation
double randDouble()
{
    return (double)rand() / (double)RAND_MAX ;
}

// generateCity
// inputs: changed within the define section
// generates the model of the city as an array of blocks that contain an array of buildings.
// this function is called once during program initialization.
void generateCity()
{

  double xLength = (BLOCKS_ROWS * (BLOCKS_SZ+GAPS_SZ))-GAPS_SZ;
  double yLength = (BLOCKS_COLUMNS * (BLOCKS_SZ+GAPS_SZ))-GAPS_SZ;

  double xOffset = -(xLength/2);
  double yOffset = -(yLength/2);

  int currentBlock = 0;

  for (int i = 0; i < BLOCKS_ROWS; i++)
  {
    for (int j = 0; j < BLOCKS_COLUMNS; j++)
    {
      city[currentBlock].x = xOffset + ((BLOCKS_SZ+GAPS_SZ)*i);
      city[currentBlock].z = yOffset + ((BLOCKS_SZ+GAPS_SZ)*j);
      city[currentBlock].sideLength = BLOCKS_SZ;

      currentBlock++;

      for (size_t k = 0; k < MAX_BUILDINGS; k++) {
        city[currentBlock].buildings[k].sideLength = BLOCKS_SZ * (randDouble()*0.1+0.1); // The max sideleng is half the sideleng of the block
        city[currentBlock].buildings[k].x = -(BLOCKS_SZ*0.5)+(BLOCKS_SZ*randDouble());//BLOCKS_SZ*0.5;
        city[currentBlock].buildings[k].z = -(BLOCKS_SZ*0.5)+(BLOCKS_SZ*randDouble());//BLOCKS_SZ*0.5;
        city[currentBlock].buildings[k].height = (randDouble()*5);
        city[currentBlock].buildings[k].color[0] = (randDouble()*0.5);
        city[currentBlock].buildings[k].color[1] = (randDouble()*0.5);
        city[currentBlock].buildings[k].color[2] = (randDouble()*0.5);
      }
    }
  }

  return;
}


// dumpCity
// prints out city model for debugging
void dumpCity()
{
  for (int i = 0; i < (BLOCKS_ROWS * BLOCKS_COLUMNS); i++)
  {
    int currentBlock = i;
    printf("Block Data: %i x: %f z: %f sl: %f \n", currentBlock, city[currentBlock].x, city[currentBlock].z, city[currentBlock].sideLength);

    for (int k = 0; k < MAX_BUILDINGS; k++) {
      printf("Building Data: %d x: %f z: %f sl: %f h: %f \n", k,
      city[currentBlock].buildings[k].x,
      city[currentBlock].buildings[k].z,
      city[currentBlock].buildings[k].sideLength,
      city[currentBlock].buildings[k].height);
    }
  }
    return;
}

// drawBuilding
// draws a building
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

   // Top Face; offset.  Yellow, varying levels of opaque.
   glNormal3f(0.0f, 1.0f, 0.0f);

   glVertex3f(xhalfside, hlength, zhalfside);
   glVertex3f(xhalfside, hlength,  -zhalfside);
   glVertex3f(-xhalfside, hlength, -zhalfside);
   glVertex3f(-xhalfside, hlength, zhalfside);

   // All polygons have been drawn.
   glEnd();
}

// drawBlock
// draws a block
void drawBlock(double c1,double c2,double c3)
{
    double halfside = BLOCKS_SZ / 2;

    glColor3d(c1,c2,c3);
    glBegin(GL_POLYGON);

    glVertex3d(halfside, 0 , halfside);
    glVertex3d(halfside, 0 , -halfside);
    glVertex3d(-halfside, 0 , -halfside);
    glVertex3d(-halfside, 0 , halfside);

    glEnd();
}

// renderCity
// renders the model of the city that is generated from initialization.
void renderCity()
{
  for (int currentBlock = 0; currentBlock < (BLOCKS_ROWS*BLOCKS_COLUMNS); currentBlock++)
  {
      glPushMatrix();
      glTranslatef(city[currentBlock].x,0,city[currentBlock].z); //translates block
      drawBlock(0.4,0.4,0.4); //draws block


      for (int i = 0; i < MAX_BUILDINGS; i++) {
        glPushMatrix();
        //double x1, double z1, double xlength, double hlength, double zlength, double c1,double c2,double c3)
        drawBuilding(city[currentBlock].buildings[i].x,
        city[currentBlock].buildings[i].z,
        city[currentBlock].buildings[i].sideLength,
        city[currentBlock].buildings[i].height,
        city[currentBlock].buildings[i].sideLength,
        city[currentBlock].buildings[i].color[0],
        city[currentBlock].buildings[i].color[1],
        city[currentBlock].buildings[i].color[2]);
        glPopMatrix();
      }

      glPopMatrix();
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
  float *p = get_camera_offset();
  gluLookAt(p[0], p[1], p[2], X_POS, Y_POS, Z_POS, 0.0, 1.0, 0.0);
  // The Main function that renders blocks & buildings

  glPushMatrix();
  float robotStreetOffset = BLOCKS_SZ / 2.0;
  glTranslatef(robotStreetOffset, 0.0, robotStreetOffset);
  renderCity();
  glPopMatrix();

  glPushMatrix();
  glTranslatef((GLfloat)X_POS, (GLfloat)Y_POS, (GLfloat)Z_POS);
  draw_robot();
  glPopMatrix();

  //Render Ground
  glPushMatrix();
  glColor3d(0.2,0.2,0.2);
  glBegin(GL_POLYGON);
  glVertex3f(100.0, -0.05 , 100.0);
  glVertex3f(100.0, -0.05 , -100.0);
  glVertex3f(-100.0, -0.05 , -100.0);
  glVertex3f(-100.0, -0.05 , 100.0);
  glEnd();

  glPopMatrix();

  //AXIS VISUALIZATION
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
  //generaets model of the city
  generateCity();
  //dumps generate city data into terminal / for debugging purposes only.
  dumpCity();

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
   glutInitWindowSize(1000, 800);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init();
   glutDisplayFunc(display);
   glutIdleFunc(display);
   glutReshapeFunc(reshape);
   glutMouseFunc(&mouse);
  //  glutKeyboardFunc(&myCBKey);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special_keyboard);
  glutSpecialUpFunc(special_keyboard_up);
   glutMainLoop();
   return 0;   /* We'll never be here.*/
}

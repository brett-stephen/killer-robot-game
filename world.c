// Project: Manhattan Massacre
// University of Lethbridge - CPSC3710
// Project Deadline: April 1, 2019
// Gideon Richter, Brett Dziedzic, Michelle Le, Sean Herridge-Berry

// World Generation

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

// Open GL
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include "cube_floor.h"
#include "draw_robot.h"

#define MAX_BUILDINGS 4
#define BLOCKS_ROWS 14
#define BLOCKS_COLUMNS 14
#define BLOCKS_SZ 5.0
#define GAPS_SZ 1.0
#define PICK_TOL 1
#define PICK_SIZE_BUFFER 16

unsigned int PickBuffer[PICK_SIZE_BUFFER];
int RenderMode;
int Xmouse, Ymouse; // mouse values

// A struct that represents a building in a city, which has a health, position
// and color
struct building
{
  double x; // X-coor of building relative to block center
  double z; // Z-coor of building relative to block center
  int health;
  int style; // the style of the building.
  double color[3];
  double sideLength; // Side length of building
  double height;     // Height length of building
};

// A struct that represents a block in a city which can contain buildings
struct block
{
  double x;                                 // X-coor of block relative to world origin
  double z;                                 // Z-coor of block relative to world origin
  double sideLength;                        // Side length of block
  struct building buildings[MAX_BUILDINGS]; // array of buildings on the block
};

struct block city[BLOCKS_ROWS * BLOCKS_COLUMNS + 1];

// Coordinates of the robot, these are defined in `cube_floor.c`
extern int X_POS;
extern int Y_POS;
extern int Z_POS;
// Indicates the direction that the robot is facing (forward, right, left, back)
extern int FACING_STATE;
extern int ALT_CAMERA_STATE;

void clear_picking_buffer()
{
  for (int i = 0; i < PICK_SIZE_BUFFER; i++)
  {
    PickBuffer[i] = 0;
  }
}

// returns random double between 0 & 1
// used for city generation
double randDouble() { return (double)rand() / (double)RAND_MAX; }

double pos(double building_sz) {
  double available_area = BLOCKS_SZ - building_sz;
  return (randDouble()*available_area) - (available_area/2);
}

// generateCity
// inputs: changed within the define section
// generates the model of the city as an array of blocks that contain an array
// of buildings. this function is called once during program initialization.
void generateCity()
{

  double xLength = (BLOCKS_ROWS * (BLOCKS_SZ + GAPS_SZ)) - GAPS_SZ;
  double yLength = (BLOCKS_COLUMNS * (BLOCKS_SZ + GAPS_SZ)) - GAPS_SZ;

  double xOffset = -xLength / 2;
  double yOffset = -yLength / 2;

  int currentBlock = 0;
  srand(time(NULL));

  for (int i = 0; i < BLOCKS_ROWS; i++)
  {
    for (int j = 0; j < BLOCKS_COLUMNS; j++)
    {
      city[currentBlock].x = xOffset + ((BLOCKS_SZ + GAPS_SZ) * i);
      city[currentBlock].z = yOffset + ((BLOCKS_SZ + GAPS_SZ) * j);
      city[currentBlock].sideLength = BLOCKS_SZ;

      currentBlock++;

      for (int k = 0; k < MAX_BUILDINGS; k++)
      {
        int hardToDestroy = (rand() % 5) == 0;
        int impossibleToDestroy = (rand() % 10) == 0;

        ///((BLOCKS_SZ/3) * randDouble()); // The max
        /// sideleng is 1/3 the sideleng of the block
        city[currentBlock].buildings[k].sideLength = BLOCKS_SZ * 0.1;
        city[currentBlock].buildings[k].x = pos(city[currentBlock].buildings[k].sideLength);
        city[currentBlock].buildings[k].z = pos(city[currentBlock].buildings[k].sideLength);
        city[currentBlock].buildings[k].health = 1;
        city[currentBlock].buildings[k].style = (rand() % 2) + 1;
        city[currentBlock].buildings[k].height = (randDouble() * 5);

        if (hardToDestroy)
        {
          // Make the building bright green and
          // make its health 4.
          city[currentBlock].buildings[k].color[0] = 0.0;
          city[currentBlock].buildings[k].color[1] = 1.0;
          city[currentBlock].buildings[k].color[2] = 0.0;
          city[currentBlock].buildings[k].health = 4;
        }
        else if (impossibleToDestroy)
        {
          // Make the building bright blue and
          // make its health really high.
          city[currentBlock].buildings[k].color[0] = 0.0;
          city[currentBlock].buildings[k].color[1] = 0.0;
          city[currentBlock].buildings[k].color[2] = 1.0;
          city[currentBlock].buildings[k].health = 1000000;
        }
        else
        {
          // It's whatever color and has a health of one
          city[currentBlock].buildings[k].color[0] = (randDouble() * 0.5);
          city[currentBlock].buildings[k].color[1] = (randDouble() * 0.5);
          city[currentBlock].buildings[k].color[2] = (randDouble() * 0.5);
          city[currentBlock].buildings[k].health = 1;
        }
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
    printf("Block Data: %i x: %f z: %f sl: %f \n", currentBlock,
           city[currentBlock].x, city[currentBlock].z,
           city[currentBlock].sideLength);

    for (int k = 0; k < MAX_BUILDINGS; k++)
    {
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
void drawBuilding(double x1, double z1, double xlength, double hlength,
                  double zlength, double c1, double c2, double c3, int style)
{
  double xhalfside = xlength / 2;
  double zhalfside = zlength / 2;

  glTranslatef(x1, 0, z1);

  glColor3d(c1, c2, c3);
  if (style == 1)
  {
    glBegin(GL_QUADS);

    // Far face.  Green, 50% opaque, non-uniform texture cooridinates.
    glNormal3f(0.0f, 0.0f, -1.0f);

    glVertex3f(-xhalfside, 0.0f, -zhalfside);
    glVertex3f(-xhalfside, hlength, -zhalfside);
    glVertex3f(xhalfside, hlength, -zhalfside);
    glVertex3f(xhalfside, 0.0f, -zhalfside);

    // Right face.  Blue; 25% opaque
    glNormal3f(1.0f, 0.0f, 0.0f);

    glVertex3f(xhalfside, 0.0f, -zhalfside);
    glVertex3f(xhalfside, hlength, -zhalfside);
    glVertex3f(xhalfside, hlength, zhalfside);
    glVertex3f(xhalfside, 0.0f, zhalfside);

    // Front face; offset.  Multi-colored, 50% opaque.
    glNormal3f(0.0f, 0.0f, 1.0f);

    glVertex3f(-xhalfside, 0.0f, zhalfside);
    glVertex3f(xhalfside, 0.0f, zhalfside);
    glVertex3f(xhalfside, hlength, zhalfside);
    glVertex3f(-xhalfside, hlength, zhalfside);

    // Left Face; offset.  Yellow, varying levels of opaque.
    glNormal3f(-1.0f, 0.0f, 0.0f);

    glVertex3f(-xhalfside, 0.0f, -zhalfside);
    glVertex3f(-xhalfside, 0.0f, zhalfside);
    glVertex3f(-xhalfside, hlength, zhalfside);
    glVertex3f(-xhalfside, hlength, -zhalfside);

    // Top Face; offset.  Yellow, varying levels of opaque.
    glNormal3f(0.0f, 1.0f, 0.0f);

    glVertex3f(xhalfside, hlength, zhalfside);
    glVertex3f(xhalfside, hlength, -zhalfside);
    glVertex3f(-xhalfside, hlength, -zhalfside);
    glVertex3f(-xhalfside, hlength, zhalfside);

    // All polygons have been drawn.
    glEnd();
  }
  else if (style == 2)
  {
    glRotatef(270, 1.0, 0.0, 0.0);
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    gluCylinder(quadratic, xlength, zlength, hlength, 32, 32);
  }
}

// drawBlock
// draws a block
void drawBlock(double c1, double c2, double c3)
{
  double halfside = BLOCKS_SZ / 2;

  glColor3d(c1, c2, c3);
  glBegin(GL_POLYGON);

  glVertex3d(halfside, 0, halfside);
  glVertex3d(halfside, 0, -halfside);
  glVertex3d(-halfside, 0, -halfside);
  glVertex3d(-halfside, 0, halfside);

  glEnd();
}

// renderCity
// renders the model of the city that is generated from initialization.
void renderCity()
{
  int building_id_number = 0;
  for (int currentBlock = 0; currentBlock < (BLOCKS_ROWS * BLOCKS_COLUMNS);
       currentBlock++)
  {
    glPushMatrix();
    glTranslatef(city[currentBlock].x, 0,
                 city[currentBlock].z); // translates block
    drawBlock(0.4, 0.4, 0.4);           // draws block

    for (int i = 0; i < MAX_BUILDINGS; i++)
    {
      // Name the next shape so we can identify it while picking

      glPushMatrix();
      glLoadName(building_id_number);
      building_id_number++;

      // double x1, double z1, double xlength, double hlength, double zlength,
      // double c1,double c2,double c3)
      drawBuilding(city[currentBlock].buildings[i].x,
                   city[currentBlock].buildings[i].z,
                   city[currentBlock].buildings[i].sideLength,
                   city[currentBlock].buildings[i].height,
                   city[currentBlock].buildings[i].sideLength,
                   city[currentBlock].buildings[i].color[0],
                   city[currentBlock].buildings[i].color[1],
                   city[currentBlock].buildings[i].color[2],
                   city[currentBlock].buildings[i].style);
      glPopMatrix();
    }
    glPopMatrix();
  }
}

void display(void)
{
  usleep(50000);

  /* Projection transformation */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  // From picking tutorial, hehe.
  int viewport[4];
  int dx = glutGet(GLUT_WINDOW_WIDTH);
  int dy = glutGet(GLUT_WINDOW_HEIGHT);
  GLsizei vx = glutGet(GLUT_WINDOW_WIDTH);
  GLsizei vy = glutGet(GLUT_WINDOW_HEIGHT);
  GLsizei v = vx < vy ? vx : vy; // minimum dimension
  GLint xl = (vx - v) / 2;
  GLint yb = (vy - v) / 2;

  if (RenderMode == GL_SELECT)
  {
    viewport[0] = xl;
    viewport[1] = yb;
    viewport[2] = dx;
    viewport[3] = dy;
    gluPickMatrix(
      (double)Xmouse,
      (double)(dy - Ymouse),
      PICK_TOL,
      PICK_TOL,
      viewport);
  }

  glFrustum(-1.0, 1.0,   /* Left and right boundary */
            -1.0, 1.0,   /* bottom and top boundary */
            1.5, 150.0); /* near nd far boundary */

  if (RenderMode == GL_SELECT)
  {
    // Not sure why we need to do this but :shrug:
    glInitNames();
    glPushName(0xffffffff);
  }

  glMatrixMode(GL_MODELVIEW);

  /* clear all pixels  */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* Clear the current matrix */
  glLoadIdentity();

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
  switch (FACING_STATE)
  // Rotate the robot according to the direction it is facing.
  {
  case FACE_FORWARD:
    glRotatef(0.0, 0.0, 1.0, 0.0);
    break;
  case FACE_RIGHT:
    glRotatef(270.0, 0.0, 1.0, 0.0);
    break;
  case FACE_BACK:
    glRotatef(180.0, 0.0, 1.0, 0.0);
    break;
  case FACE_LEFT:
    glRotatef(90.0, 0.0, 1.0, 0.0);
    break;
  default:
    break;
  }
  draw_robot();
  glPopMatrix();

  // Render Ground
  glPushMatrix();
  glColor3d(0.2, 0.2, 0.2);
  glBegin(GL_POLYGON);
  glVertex3f(100.0, -0.05, 100.0);
  glVertex3f(100.0, -0.05, -100.0);
  glVertex3f(-100.0, -0.05, -100.0);
  glVertex3f(-100.0, -0.05, 100.0);
  glEnd();

  glPopMatrix();

  // AXIS VISUALIZATION
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

  glutSwapBuffers();
}

void init(void)
{
  // generates model of the city
  generateCity();
  // dumps generate city data into terminal / for debugging purposes only.
  // dumpCity();

  /* select clearing color 	*/
  glClearColor(0.3, 0.53, 0.92, 0.0);
  glShadeModel(GL_FLAT);
  glEnable(GL_DEPTH_TEST);
  RenderMode = GL_RENDER;
}

void reshape(int w, int h)
{
  /* Set the view port */
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);

  /* Projection transformation */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glFrustum(-1.0, 1.0,   /* Left and right boundary */
            -1.0, 1.0,   /* bottom and top boundary */
            1.5, 150.0); /* near nd far boundary */

  glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y)
{
  /*
  When a user clicks the mouse on a building, the building should
  disappear.

  Currently there is a bug where it is likely that the removal
  will be more successful if the mouse is double clicked.

  Also, sometimes more than one building disappears...
  Also, this GL_SELECT seems to lag the program while it runs.
  */
  Ymouse = y;
  Xmouse = x;
  int n_hits = -1;
  int n_items = -1;
  unsigned int z_min, z_max = 0;
  int item = -1;

  clear_picking_buffer();

  switch (button)
  {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN && ALT_CAMERA_STATE == DEFAULT)
    {
      RenderMode = GL_SELECT;
      glRenderMode(GL_SELECT);
      display();
      RenderMode = GL_RENDER;
      n_hits = glRenderMode(GL_RENDER);

      if (n_hits == 0)
      {
        RenderMode = GL_SELECT;
        glRenderMode(GL_SELECT);
        display();
        RenderMode = GL_RENDER;
        n_hits = glRenderMode(GL_RENDER);
      }

      int i;
      int index = 0;

      for (i=0; i < n_hits; i++)
      {
        n_items = PickBuffer[index++];
        z_min = PickBuffer[index++];
        z_max = PickBuffer[index++];
        printf("Hit # %d: found %d items on the same stack\n", i, n_items);
        printf("Zmin = %d, zmax = %d\n", z_min, z_max);
        fflush(stdout);
        for (int j = 0; j < n_items; j++)
        {
          // item is the name of the object
          // that we defined in `renderCity`.
          item = PickBuffer[index++];

          printf("ITEM: %d at %d\n", item, j);
          fflush(stdout);

          int building_id_number = 0;

          // Iterate every building searching for the one we just clicked on.
          // You could do this faster by doing some math about `item` and
          // the block sizes, then just iterating up to `MAX_BUILDINGS`.
          for (int currentBlock = 0;
               currentBlock < (BLOCKS_ROWS * BLOCKS_COLUMNS); currentBlock++)
          {
            for (int p = 0; p < MAX_BUILDINGS; p++)
            {
              if (building_id_number == item)
              {
                // It is likely that we will find more than one,
                // although I am not sure how to tell when to stop
                // looking for things to delete. The first and last
                // items found do not seem to correspond to near/far.
                printf("Found one\n");
                fflush(stdout);
                // There are three types of buildings:
                // - Normal, any color, 1 health
                // - Hard, green, 4 health (3 clicks to destroy)
                // - Impossible, blue, infinite health (cant destry)
                if (city[currentBlock].buildings[p].health <= 1)
                {
                // "Delete" a building by reducing its height to 0.
                  city[currentBlock].buildings[p].height = 0;
                  city[currentBlock].buildings[p].sideLength = 0;
                }
                else
                {
                  // Decrease the health by one
                  city[currentBlock].buildings[p].health--;
                }
              }
              building_id_number++;
            }
          }
        }
      }
    }
    // From the picking tutorial.
    Ymouse = y;
    Xmouse = x;
    // clear_picking_buffer();
    glutPostRedisplay();
  }
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(1000, 800);
  glutInitWindowPosition(100, 100);
  glutCreateWindow(argv[0]);

  glSelectBuffer(PICK_SIZE_BUFFER, PickBuffer);

  init();

  glutDisplayFunc(display);
  glutIdleFunc(display);
  glutReshapeFunc(reshape);

  glutMouseFunc(NULL);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(special_keyboard);
  glutSpecialUpFunc(special_keyboard_up);
  glutMouseFunc(mouse);

  glutMainLoop();

  return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifdef GOS_USE_FREEGLUT
#include <GL/freeglut.h>
#else
#include <GL/glut.h>
#endif

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <SOIL/SOIL.h>

#define GOS_CUBE_SIZE 256
#define GOS_CUBE_GCIT_COUNT 24

typedef struct gcco {
  GLint s;
  GLint t;
} gcco;

typedef struct gcve {
  GLfloat x;
  GLfloat y;
  GLfloat z;
} gcve;

typedef struct gcit {
  gcco co;
  gcve ve;
} gcit;

static gcit items[GOS_CUBE_GCIT_COUNT] = {
  { { 0, 0 }, { -1.0F, -1.0F, -1.0F } },
  { { 0, 1 }, { -1.0F, -1.0F,  1.0F } },
  { { 1, 1 }, { -1.0F,  1.0F,  1.0F } },
  { { 1, 0 }, { -1.0F,  1.0F, -1.0F } },

  { { 0, 0 }, {  1.0F, -1.0F, -1.0F } },
  { { 0, 1 }, {  1.0F, -1.0F,  1.0F } },
  { { 1, 1 }, {  1.0F,  1.0F,  1.0F } },
  { { 1, 0 }, {  1.0F,  1.0F, -1.0F } },

  { { 0, 0 }, { -1.0F, -1.0F, -1.0F } },
  { { 0, 1 }, { -1.0F, -1.0F,  1.0F } },
  { { 1, 1 }, {  1.0F, -1.0F,  1.0F } },
  { { 1, 0 }, {  1.0F, -1.0F, -1.0F } },

  { { 0, 0 }, { -1.0F,  1.0F, -1.0F } },
  { { 0, 1 }, { -1.0F,  1.0F,  1.0F } },
  { { 1, 1 }, {  1.0F,  1.0F,  1.0F } },
  { { 1, 0 }, {  1.0F,  1.0F, -1.0F } },

  { { 0, 0 }, { -1.0F, -1.0F, -1.0F } },
  { { 0, 1 }, { -1.0F,  1.0F, -1.0F } },
  { { 1, 1 }, {  1.0F,  1.0F, -1.0F } },
  { { 1, 0 }, {  1.0F, -1.0F, -1.0F } },

  { { 0, 0 }, { -1.0F, -1.0F,  1.0F } },
  { { 0, 1 }, { -1.0F,  1.0F,  1.0F } },
  { { 1, 1 }, {  1.0F,  1.0F,  1.0F } },
  { { 1, 0 }, {  1.0F, -1.0F,  1.0F } }
};

static int winw  = 500;
static int winh = 500;
static float alpha = 20.0F;


static void cube();
static void display(void);
static void reshape(int width, int height);
static void apply(const char* filename);
#ifndef __EMSCRIPTEN__
static void keyboard(unsigned char key, int x, int y);
#endif

/* Code modified from http://www.lousodrome.net/opengl/ */

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("You should supply one and only one parameter: the initial texture image\n");
    return -1;
  }

  /* Creation of the window */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(500, 500);
  glutCreateWindow("Render to texture");

  /* OpenGL settings */
  glEnable(GL_DEPTH_TEST);

  /* Texture setting */
  unsigned int texture_id;
  glEnable(GL_TEXTURE_2D);
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);

  /* Load and apply the texture */
  apply(argv[1]);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
#ifdef GL_CLAMP_TO_EDGE
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#else
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
#endif

  /* Declaration of the callbacks */
  glutDisplayFunc(&display);
  glutReshapeFunc(&reshape);
#ifndef __EMSCRIPTEN__
  glutKeyboardFunc(&keyboard);
#endif // !__EMSCRIPTEN__

  /* Loop */
  glutMainLoop();

  /* Never reached */
  return 0;
}

/*
** Just a textured cube
*/
void cube() {
  int i;
  glBegin(GL_QUADS);
  for (i = 0; i < GOS_CUBE_GCIT_COUNT; i++) {
    glTexCoord2i(items[i].co.s, items[i].co.t);
    glVertex3f(items[i].ve.x, items[i].ve.y, items[i].ve.z);
  }
  glEnd();
}

/*
** Function called to update rendering
*/
void display(void) {
  glLoadIdentity();
  glTranslatef(0, 0, -10);
  glRotatef(30, 1, 0, 0);
  glRotatef(alpha, 0, 1, 0);

  /* Define a view-port adapted to the texture */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(20, 1, 5, 15);
  glViewport(0, 0, GOS_CUBE_SIZE, GOS_CUBE_SIZE);
  glMatrixMode(GL_MODELVIEW);

  /* Render to buffer */
  glClearColor(1, 1, 1, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  cube();
  glFlush();

  /* Render to screen */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(20, winw / (float)winh, 5, 15);
  glViewport(0, 0, winw, winh);
  glMatrixMode(GL_MODELVIEW);
  glClearColor(1, 1, 1, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  cube();

  /* End */
  glFlush();
  glutSwapBuffers();

  /* Update again and again */
  alpha = alpha + 0.1F;
  glutPostRedisplay();
}

/*
** Function called when the window is created or resized
*/
void reshape(int width, int height) {
  winw = width;
  winh = height;
  glutPostRedisplay();
}

void apply(const char* filename) {
  int texture_width, texture_height;
  unsigned char* texture = SOIL_load_image(
    filename,
    &texture_width,
    &texture_height,
    0,
    SOIL_LOAD_RGB);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    GL_RGB,
    texture_width,
    texture_height,
    0,
    GL_RGB,
    GL_UNSIGNED_BYTE,
    texture);
  SOIL_free_image_data(texture);
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE void settexture(const char* filename) {
  apply(filename);
}
#else
/*
** Function called when a key is hit
*/
void keyboard(unsigned char key, int x, int y) {
  int foo;

  foo = x + y; /* Has no effect: just to avoid a warning */
  if ('q' == key || 'Q' == key || 27 == key)
    exit(0);
}
#endif // !__EMSCRIPTEN__

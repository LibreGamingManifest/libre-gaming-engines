//===================================
// @file   : showgalaxy.cpp
// @version: 2020-03-03
// @created: 2018-07-21
// @author : pyramid
// @brief  : libprocutest app 
//===================================
// @acknowledgement:
// This code was created by Jeff Molofee 1999 (ported to
// Linux/GLUT by Richard Campbell 1999).
// If you've found this code useful, please let me know.
// Visit me at [url]www.demonews.com/hosted/nehe[/url] 
// (email Richard Campbell at [email]ulmont@bellsouth.net[/email])
// YLP 2011-05-31 : modify it for only display a cube 
// source:
// https://www.opengl.org/discussion_boards/showthread.php/174771-OpenGL-3D-Texture-Simple-Cube-with-glsl/page2
//===================================


//-----------------------------------
// libraries headers
//-----------------------------------

// standard libraries
#include <iostream>
#include <string>
#include <typeinfo> // operator typeid
// opengl libraries
#include <GL/glut.h> // Header File For The GLUT Library 
#include <GL/gl.h> // Header File For The OpenGL32 Library
#include <GL/glu.h> // Header File For The GLu32 Library
#include <unistd.h> // needed to sleep
#define STB_IMAGE_IMPLEMENTATION // image loading library (PD)
//throws unused_func warnings: #define STBI_ONLY_JPEG // for loading jpeg images
#include "ext/stb_image.h" // image loading library (PD)
// libprocu library
#include "lib/libprocu-galaxy.hpp"


//using namespace std;
using namespace procu;


//-----------------------------------
// variables
//-----------------------------------


// ASCII code for the escape key
#define ESCAPE 27

// The GLUT window identifier
int window;

// current rotation angle for the object
float rcube = 0.0f;

// texture params
GLubyte* texData = 0;
GLuint texid;
GLuint texwidth = 2;
GLuint texheight = 2;
uint bytesPerPixel = 0;

const bool TEXTURE_LOAD = false;

//-----------------------------------
// texture image functions
//-----------------------------------

// forward declaration

struct Image {
  typedef uint8_t byte;
  uint imgId = 0;
  uint width;
  uint height;
  uint bytesPerPixel;
  std::vector<byte> image; // image data
  byte* imgPtr;            // pointer to image data

  Image() {};

  //byte* create(const uint imgW, const uint imgH, const uint bpp);
  byte* create(byte*& img, const uint imgW, const uint imgH, const uint bpp) {
    width = imgW;
    height = imgH;
    bytesPerPixel = bpp;

    const uint size = imgW*imgH*bpp;
    image.resize(size); // set image data size
    byte color = 255; // white
    std::fill(image.begin(), image.end(), color);
    img = image.data();
    imgPtr = img; // internal pointer

    //std::cout << "Image::create: img address = 0x" << std::hex << img << std::dec << std::endl;
    //#ifdef __ANDROID__
    //  LOGI("Image::create: img address = 0x%d", img);
    //#endif

    return img;
  };

}; // end struct

Image image;

//-----------------------------------
// object functions
//-----------------------------------

void DrawCube() {
  glColor3f(1.0f,1.0f,1.0f);

  // draw a cube (6 quadrilaterals)
  glBegin(GL_QUADS); // start drawing the quads

  // Front Face
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f); // bottom left of the texture and quad
  glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f); // bottom right of the texture and quad
  glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f); // top right of the texture and quad
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f); // top left of the texture and quad

  // Back Face
  glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // bottom right of the texture and quad
  glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f); // top right of the texture and quad
  glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f); // top left of the texture and quad
  glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f); // bottom left of the texture and quad

  // Top Face
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f); // top left of the texture and quad
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f); // bottom left of the texture and quad
  glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f); // bottom right of the texture and quad
  glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f); // top right of the texture and quad

  // Bottom Face
  glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // top right of the texture and quad
  glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f); // top left of the texture and quad
  glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f); // bottom left of the texture and quad
  glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f); // bottom right of the texture and quad

  // Right face
  glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f); // bottom right of the texture and quad
  glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f); // top right of the texture and quad
  glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f); // top left of the texture and quad
  glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f); // bottom left of the texture and quad

  // Left Face
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f); // bottom left of the texture and quad
  glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f); // bottom right of the texture and quad
  glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f); // top right of the texture and quad
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f); // top left of the texture and quad

  glEnd(); // done drawing the quads
}


void DrawFace() {
  glColor3f(1.0f,1.0f,1.0f);
  // draw a cube (6 quadrilaterals)
  glBegin(GL_QUADS); // start drawing the quads

  // Front Face
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f); // bottom left of the texture and quad
  glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f); // bottom right of the texture and quad
  glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f); // top right of the texture and quad
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f); // top left of the texture and quad

  glEnd(); // done drawing the quads

}


void InitTexture() {

  /** 
   * @brief Load an image from file and process on top of it
   */
  //
  if(TEXTURE_LOAD) {
  int width, height; 
  int nrChannels;
  texData = stbi_load("img/container.jpg", &width, &height, &nrChannels, 0);
  texwidth = width; // cast by copy
  texheight = height; // cast by copy
  bytesPerPixel = nrChannels;
  //debug:std::cout << "nr of bytesPerPixel = " << bytesPerPixel << "\n";
  //std::cout << "after: address of texData = 0x" << std::hex << (long)(texData) << std::endl;
  }

  /** 
   * @brief Create image area in memory and then generate on top of it
   */
  if(!TEXTURE_LOAD) {
  texwidth = 512;
  texheight = 512;
  bytesPerPixel = 3; // RGB
  // previous tests
  //std::cout << "before: address of texData = 0x" << std::hex << (long)(texData) << std::endl;
  image.create(texData, texwidth, texheight, bytesPerPixel);
  //std::cout << "after: address of texData = 0x" << std::hex << (long)(texData) << std::endl;

  // with returned imgage Id
  //uint imgId = procu.createImage(texData, texwidth, texheight, bytesPerPixel);
  // or without image Id
  //image.createImage(texData, texwidth, texheight, bytesPerPixel);
  //std::cout << "texData address = 0x" << std::hex << (long)(texData) << std::dec << std::endl;
  }

  // Bind the texture
  glEnable(GL_TEXTURE_2D);
  glGenTextures(1,&texid);
  glBindTexture(GL_TEXTURE_2D,texid); 	//Sélectionne ce n°
  glTexImage2D (
    GL_TEXTURE_2D, // texture type: 2D texture
    0,  // no mipmap level
    3,  // how many colors in GL texture: 4 (red,green,blue,alpha)
    texwidth, // width: 256
    texheight, // height: 256
    0, // border width: 0
    GL_RGB, //source image format: GL_RGB, or GL_RGBA
    GL_UNSIGNED_BYTE, // color type
    texData //image byte data address
   );
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

   //stbi_image_free(texData); // delete the loaded texture
}


// OpenGL initialization function.
// Sets all of the initial parameters.
// We call this right after our OpenGL window is created.
void InitGL(int Width, int Height) {
  glClearColor(0.1f, 0.1f, 0.4f, 1.0f); // set the given background color
  glClearDepth(1.0); // enables clearing of depth buffer
  glDepthFunc(GL_LESS); // type of depth test
  glEnable(GL_DEPTH_TEST); // enables depth testing
  glShadeModel(GL_SMOOTH); // enables smooth color shading

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity(); // reset the projection matrix

  // set perspective view and window aspect ratio
  gluPerspective(25.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);

  glMatrixMode(GL_MODELVIEW);

  // load the texture
  texwidth = Width;
  texheight = Height;
  InitTexture();
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void ReSizeGLScene(int Width, int Height) {
  if (Height==0) // prevent a divide by zero if the window is too small
    Height=1;

  glViewport(0, 0, Width, Height); // reset the current viewport and perspective transformation

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  gluPerspective(25.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
  glMatrixMode(GL_MODELVIEW);
}

/* The main drawing function. */
void DrawGLScene() {
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // clear the screen and the depth buffer

  glLoadIdentity(); // reset the transformation matrix.
  glTranslatef(0.0f,0.0f,-7.0f); // move right 3 units, and back into the screen 7
  glRotatef(rcube,1.0f,1.0f,1.0f); // rotate the cube on x, y, and z

  // update generated texture only while still being painted
  //if (!libprocu.isEmpty()) {
  //  glTexImage2D (GL_TEXTURE_2D,0,3,texwidth,texheight,0,GL_RGB,GL_UNSIGNED_BYTE,texData);
  //}
  //std::cout << "\rprocu.isEmpty = " << procu.isEmpty();

  // rotate the cube
  rcube += 0.1f;
  DrawCube();

  // swap the buffers to display, since double buffering is used.
  glutSwapBuffers();
}

/* The function called whenever a key is pressed. */
void keyPressed(unsigned char key, int x, int y) {
    /* avoid thrashing this call */
    usleep(100);

    // If escape is pressed, kill everything.
    if (key == ESCAPE) {
      /* shut down our window */
      glutDestroyWindow(window);

      // exit the program...normal termination.
      exit(0);
    }
}

void initOpenGL(const bool bShowFullscreen=true) {

  /* Select type of Display mode:
     Double buffer
     RGBA color
     Alpha components supported
     Depth buffered for automatic clipping */
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);

  // get a desktop window
  glutInitWindowSize(320, 320);
  /* the window starts at the upper left corner of the screen */
  glutInitWindowPosition(0, 0);
  /* Open a window */
  window = glutCreateWindow("ProcU TestApp Lin");

  /* Register the function to do all our OpenGL drawing. */
  glutDisplayFunc(&DrawGLScene);

  // make window full screen
  if (bShowFullscreen) {
    glutFullScreen();
  }
  // get screen information
  int scrx = glutGet(GLUT_SCREEN_WIDTH);
  //int scrx = glutGet(GLUT_WINDOW_WIDTH);
  std::cout << "screen size x: " << scrx;
  int scry = glutGet(GLUT_SCREEN_HEIGHT);
  //int scry = glutGet(GLUT_WINDOW_HEIGHT);
  std::cout << " | screen size y: " << scry << "\n";

  /* Even if there are no events, redraw our gl scene. */
  glutIdleFunc(&DrawGLScene);

  /* Register the function called when our window is resized. */
  glutReshapeFunc(&ReSizeGLScene);

  /* Register the function called when the keyboard is pressed. */
  glutKeyboardFunc(&keyPressed);

  // initialize the viewport
  InitGL(scrx, scry);

}


//-----------------------------------
// main program
//-----------------------------------

int main(int argc, char **argv) {
  std::cout << "--- showgalaxy | v0.00.10 | 2020-03-02 ---\n";

  /* Initialize GLUT state - glut will take any command line arguments that pertain to it or
     X Windows - look at its documentation at
     [url]https://www.opengl.org/resources/libraries/glut/spec3/node10.html[/url]
  */
  glutInit(&argc, argv);

  initOpenGL(false); // initOpenGL(bShowFullscreen)

  //---------------------------------
  // enter main GLUT drawing loop
  //---------------------------------
  // start the main drawing loop until it is terminated
  glutMainLoop();

  return 0;
} // end main

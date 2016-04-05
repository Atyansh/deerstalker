#include <iostream>

#ifdef __APPLE__
    #include <GLUT/glut.h>
#else
	 #include "client/Graphics/GL/freeglut.h"
#endif

#include "Window.h"
#include "Cube.h"
#include "Matrix4.h"
#include "Globals.h"

#include <cstdlib>
#include <cstring>



using boost::asio::ip::tcp;


enum { max_length = 1024 };

//graphic
int Window::width  = 512;   //Set window width in pixels here
int Window::height = 512;   //Set window height in pixels here
float toggle = 0.005;
float bx = 0;
float by = 0;

void Window::initialize()
{
    //Setup the light
    Vector4 lightPos(0.0, 10.0, 15.0, 1.0);
    Globals::light.position = lightPos;
    Globals::light.quadraticAttenuation = 0.02;
    
    //Initialize cube matrix:
    Globals::cube.toWorld.identity();
    
    //Setup the cube's material properties
    Color color(0x23ff27ff);
    Globals::cube.material.color = color;
}

//----------------------------------------------------------------------------
// Callback method called when system is idle.
// This is called at the start of every new "frame" (qualitatively)
void Window::idleCallback()
{
    //Set up a static time delta for update calls
    Globals::updateData.dt = 1.0/60.0;// 60 fps
    
    //Rotate cube; if it spins too fast try smaller values and vice versa
    Globals::cube.spin(toggle);
        
    //Call the update function on cube
    Globals::cube.update(Globals::updateData);
    
    
    //Call the display routine to draw the cube
    displayCallback();
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when graphics window is resized by the user
void Window::reshapeCallback(int w, int h)
{
    width = w;                                                       //Set the window width
    height = h;                                                      //Set the window height
    glViewport(0, 0, w, h);                                          //Set new viewport size
    glMatrixMode(GL_PROJECTION);                                     //Set the OpenGL matrix mode to Projection
    glLoadIdentity();                                                //Clear the projection matrix by loading the identity
    gluPerspective(60.0, double(width)/(double)height, 1.0, 1000.0); //Set perspective projection viewing frustum
}

//----------------------------------------------------------------------------
// Callback method called by GLUT when window readraw is necessary or when glutPostRedisplay() was called.
void Window::displayCallback()
{
    //Clear color and depth buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //Set the OpenGL matrix mode to ModelView
    glMatrixMode(GL_MODELVIEW);
    
    //Push a matrix save point
    //This will save a copy of the current matrix so that we can
    //make changes to it and 'pop' those changes off later.
    glPushMatrix();
    
    //Replace the current top of the matrix stack with the inverse camera matrix
    //This will convert all world coordiantes into camera coordiantes
    glLoadMatrixf(Globals::camera.getInverseMatrix().ptr());
    
    //Bind the light to slot 0.  We do this after the camera matrix is loaded so that
    //the light position will be treated as world coordiantes
    //(if we didn't the light would move with the camera, why is that?)
    Globals::light.bind(0);
    
     
	Globals::cube.draw(Globals::drawData);
    
    
    //Pop off the changes we made to the matrix stack this frame
    glPopMatrix();
    
    //Tell OpenGL to clear any outstanding commands in its command buffer
    //This will make sure that all of our commands are fully executed before
    //we swap buffers and show the user the freshly drawn frame
    glFlush();
    
    //Swap the off-screen buffer (the one we just drew to) with the on-screen buffer
    glutSwapBuffers();
}


//TODO: Keyboard callbacks!
void Window::processNormalKeys(unsigned char key, int x, int y) {
    Matrix4 change;
    change.identity();
    
    //print
    float px, py, pz;
	char request[max_length];
	char reply[max_length];

	size_t request_length;
	

	size_t reply_length;


    
    switch(key){
        case 'q': case 'Q':
            //quit
            exit(EXIT_SUCCESS);
            break;
        case 'r':
            //reset
            Globals::cube.toWorld = change;
            toggle = 0.005;
            break;
        case 'X':
			//server
			
			request[0] = key;
			request[1] = '\0';
			request_length = std::strlen(request);
			std::cout << request_length;
			boost::asio::write(Globals::socket, boost::asio::buffer(request, request_length));

            //move right
            change.makeTranslate(0.3, 0, 0);
            Globals::cube.toWorld = change * Globals::cube.toWorld;

			//server
			reply_length = boost::asio::read(Globals::socket,
				boost::asio::buffer(reply, request_length));
			
			std::cout << "Reply is: ";
			std::cout.write(reply, reply_length);
			std::cout << "\n";
            break;
        case 'x':
            //move left
            change.makeTranslate(-0.3, 0, 0);
            Globals::cube.toWorld = change * Globals::cube.toWorld;
            break;
        case 'Y':
            //move up
            change.makeTranslate(0, 0.3, 0);
            Globals::cube.toWorld = change * Globals::cube.toWorld;
            break;
        case 'y':
            //move down
            change.makeTranslate(0, -0.3, 0);
            Globals::cube.toWorld = change * Globals::cube.toWorld;
            break;
        case 'Z':
            //move out screen
            change.makeTranslate(0, 0, 0.3);
            Globals::cube.toWorld = change * Globals::cube.toWorld;
            break;
        case 'z':
            //move into screen
            change.makeTranslate(0, 0, -0.3);
            Globals::cube.toWorld = change * Globals::cube.toWorld;
            break;
        case 't':
            //toggle spin
            toggle = -toggle;
            break;
        case 's':
            //scale down
            change.makeScale(0.9);
            Globals::cube.toWorld = Globals::cube.toWorld * change;
            break;
        case 'S':
            //scale up
            change.makeScale(1.1);
            Globals::cube.toWorld = Globals::cube.toWorld * change;
            break;
        case 'o':
            //orbit around z counterclockwise
            change.makeRotateZ(0.3);
            Globals::cube.toWorld = change * Globals::cube.toWorld;
            break;
        case 'O':
            //orbit around z clockwise
            change.makeRotateZ(-0.3);
            Globals::cube.toWorld = change * Globals::cube.toWorld;
            break;
		}

        //print
        px = (Globals::cube.toWorld).get(3,0);
        py = (Globals::cube.toWorld).get(3,1);
        pz = (Globals::cube.toWorld).get(3,2);
        
        std::cout<<"["<<px<<", "<<py<<", "<<pz<<"]"<<std::endl;


}

//TODO: Function Key callbacks!
void Window::processSpecialKeys(int key, int x, int y) {
    
    switch(key){
        case GLUT_KEY_LEFT:
            std::cout<<"left"<<std::endl;
            break;
        case GLUT_KEY_RIGHT:
            std::cout<<"right"<<std::endl;
            break;
        case GLUT_KEY_UP:
            std::cout<<"up"<<std::endl;
            break;
        case GLUT_KEY_DOWN:
            std::cout<<"down"<<std::endl;
            break;
    }
}

//TODO: Mouse callbacks!

//TODO: Mouse Motion callbacks!

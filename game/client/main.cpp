//
// blocking_tcp_echo_client.cpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

enum { max_length = 1024 };

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 3)
		{
			std::cerr << "Usage: blocking_tcp_echo_client <host> <port>\n";
			return 1;
		}

		boost::asio::io_service io_service;

		tcp::socket s(io_service);
		tcp::resolver resolver(io_service);
		boost::asio::connect(s, resolver.resolve({ argv[1], argv[2] }));
		while (true) {
			std::cout << "Enter message: ";
			char request[max_length];
			std::cin.getline(request, max_length);
			size_t request_length = std::strlen(request);
			boost::asio::write(s, boost::asio::buffer(request, request_length));

			char reply[max_length];
			size_t reply_length = boost::asio::read(s,
				boost::asio::buffer(reply, request_length));
			std::cout << "Reply is: ";
			std::cout.write(reply, reply_length);
			std::cout << "\n";
		}
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}



//#include <stdio.h>
//#include <stdlib.h>
//#include <iostream>
//
//#ifdef __APPLE__
//	#include <GLUT/glut.h>
//#else
//	#include "Graphics/GL/freeglut.h"
//#endif
//
//#include "Graphics/Window.h"
//#include "Graphics/Cube.h"
//#include "Graphics/Matrix4.h"
//#include "Graphics/Globals.h"
//
//#include <boost/asio.hpp>
//
//using boost::asio::ip::tcp;
//
//
//enum { max_length = 1024 };
//
//
//int main(int argc, char *argv[])
//{
//
//	glutInit(&argc, argv);                                      //Initialize GLUT
//	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);   //Open an OpenGL context with double buffering, RGB colors, and depth buffering
//	glutInitWindowSize(Window::width, Window::height);          //Set initial window size
//	glutCreateWindow("UCSD CSE 167 - Project 1 - OpenGL Cube"); //Open window and set window title
//
//	glEnable(GL_DEPTH_TEST);                                    //Enable depth buffering
//	glClear(GL_DEPTH_BUFFER_BIT);                               //Clear depth buffer
//	glClearColor(0.0, 0.0, 0.0, 0.0);                           //Set clear color to black
//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);                  //Set polygon drawing mode to fill front and back of each polygon
//	glDisable(GL_CULL_FACE);                                    //Disable backface culling to render both sides of polygons
//	glShadeModel(GL_SMOOTH);                                    //Set shading to smooth
//
//	glEnable(GL_COLOR_MATERIAL);                                //Enable color materials
//	glEnable(GL_LIGHTING);                                      //Enable lighting
//
//	//Register callback functions:
//	glutDisplayFunc([=]() { Window::displayCallback; });
//	glutReshapeFunc(Window::reshapeCallback);
//	glutIdleFunc(Window::idleCallback);
//	//Register the callback for the keyboard
//	glutKeyboardFunc(Window::processNormalKeys);
//	//Register the callback for the keyboard function keys
//	glutSpecialFunc(Window::processSpecialKeys);
//	//Register the callback for the mouse
//	//Register the callback for the mouse motion
//
//	//Initialize the Window:
//	//The body of this function is a great place to load textures, shaders, etc.
//	//and do any operations/calculations/configurations that only need to happen once.
//
//	// server 
//
//
//	tcp::resolver resolver(Globals::io_service);
//	boost::asio::connect(Globals::socket, resolver.resolve({ argv[1], argv[2] }));
//
//	Window::initialize();
//
//	//Start up the render loop!
//	glutMainLoop();
//
//	return 0;
//}


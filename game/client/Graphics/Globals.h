#ifndef CSE167_Globals_h
#define CSE167_Globals_h

#include "Camera.h"
#include "Cube.h"
#include "Light.h"
#include "DrawData.h"
#include "UpdateData.h"
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class Globals
{
    
public:
    
    static Camera camera;
    static Cube cube;
    static Light light;
    static DrawData drawData;
    static UpdateData updateData;

	static boost::asio::io_service io_service;
	static tcp::socket socket;

    //Feel free to add more member variables as needed
        
};

#endif

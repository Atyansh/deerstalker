#ifndef CSE167_Window_h
#define CSE167_Window_h

class Window	  // OpenGL output window related routines
{

protected:
public:
    
    static int width, height; 	            // window size
    
	static void initialize();
    static void idleCallback(void);
    static void reshapeCallback(int, int);
    static void displayCallback(void);
    
    //New Methods
    static void processNormalKeys(unsigned char key, int x, int y);
    static void processSpecialKeys(int key, int x, int y);
    
};

#endif


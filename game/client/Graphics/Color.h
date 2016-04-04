#ifndef CSE167_Color_h
#define CSE167_Color_h

class Color
{
    
protected:
    
    float c[4];
    unsigned char u8bit = 0xFF;
    
public:
    
    Color(void);
    Color(float r, float g, float b);
    Color(float r, float g, float b, float a);
    Color(unsigned int hex);
    
    ~Color(void);
    
    float* ptr(void);
    float& operator [] (int);
    
    //Mathematical operations on colors
    Color interpolate(Color&, float);
    //Add
    //Subtract
    //Scalar Multiplication
    //Component-wise Multiplication
    //Clamping
    //Etc.
    
    //Pre-Defined Colors
    static Color red(void);
    static Color blue(void);
    static Color green(void);
    static Color yellow(void);
    static Color orange(void);
    static Color purple(void);
    static Color white(void);
    static Color black(void);
    static Color lightBrown(void);
    
    //Randomized Colors
    static Color randomPastel(void);
    static Color randomDarkPastel(void);
    static Color randomBrightPastel(void);
    static Color randomDarkShade(void);
    
    //Used by Light
    static Color ambientDefault(void);
    static Color diffuseDefault(void);
    static Color specularDefault(void);
    
    //Used by Material
    static Color ambientMaterialDefault(void);
    static Color diffuseMaterialDefault(void);
    static Color specularMaterialDefault(void);
    static Color emissionMaterialDefault(void);
    
};

#endif

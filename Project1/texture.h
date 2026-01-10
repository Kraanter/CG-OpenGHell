#ifndef TEXTURE_HPP
#define TEXTURE_HPP

// Load a .BMP file using our custom loader
GLuint loadBMP(const char * imagepath);

// Load a .PNG file using stb_image
GLuint loadPNG(const char* imagepath);

//// Since GLFW 3, glfwLoadTexture2D() has been removed. You have to use another texture loading library, 
//// or do it yourself (just like loadBMP_custom and loadDDS)
//// Load a .TGA file using GLFW's own loader
//GLuint loadTGA_glfw(const char * imagepath);

// Load a .DDS file using GLFW's own loader
GLuint loadDDS(const char * imagepath);

// Create a 1x1 solid color texture (RGB)
GLuint createSolidTexture(unsigned char r, unsigned char g, unsigned char b);

// Load texture based on file extension (.bmp/.png/.dds)
GLuint loadTexture(const char* imagepath);


#endif

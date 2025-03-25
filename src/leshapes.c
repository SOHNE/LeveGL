#include "lecore_context.h"

#include "levegl/levegl.h"

#include <math.h>
#include <string.h>

extern CoreContext core;

// Basic shapes shader code
static const char * basicShapesVS = "#version 330 core\n"
                                    "layout (location = 0) in vec2 aPos;\n"
                                    "uniform vec4 uColor;\n"
                                    "out vec4 vertexColor;\n"
                                    "void main()\n"
                                    "{\n"
                                    "   gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);\n"
                                    "   vertexColor = uColor;\n"
                                    "}\0";

static const char * basicShapesFS = "#version 330 core\n"
                                    "in vec4 vertexColor;\n"
                                    "out vec4 FragColor;\n"
                                    "void main()\n"
                                    "{\n"
                                    "   FragColor = vertexColor;\n"
                                    "}\0";

// Internal state for shapes rendering
typedef struct
{
    Shader shader;
    //GLuint VAO;
    //GLuint VBO;
    int currentBufferSize;
    int colorLocation;
} ShapesState;

static ShapesState shapesState = { 0 };

static void
InitShapesBuffers( void )
{
}

void
InitShapes( void )
{
}

void
CleanupShapes( void )
{
}

static void
UpdateVertexBuffer( const float * vertices, int size )
{
}

static void
ScreenToNDC( float x, float y, float * ndcX, float * ndcY )
{
}

static void
SetShapeColor( Color color )
{
}

void
DrawPixel( int x, int y, Color color )
{
}

void
DrawLine( int startX, int startY, int endX, int endY, Color color )
{
}

void
DrawTriangle( float x1, float y1, float x2, float y2, float x3, float y3, Color color )
{
}

void
DrawTriangleLines( float x1, float y1, float x2, float y2, float x3, float y3, Color color )
{
}

void
DrawRectangle( float x, float y, float width, float height, Color color )
{
}

void
DrawRectangleLines( float x, float y, float width, float height, Color color )
{
}

void
DrawCircle( float centerX, float centerY, float radius, Color color )
{
}

void
DrawCircleLines( float centerX, float centerY, float radius, Color color )
{
}

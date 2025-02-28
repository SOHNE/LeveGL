#include <math.h>
#include <string.h>
#include "lecore_context.h"
#include "levegl/levegl.h"

#include <GL/glew.h>

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
    GLuint VAO;
    GLuint VBO;
    int    currentBufferSize;
    int    colorLocation;
} ShapesState;

static ShapesState shapesState = { 0 };

static void
InitShapesBuffers( void )
{
    TRACE_INFO( "Initializing shapes buffers" );

    glGenVertexArrays( 1, &shapesState.VAO );
    glGenBuffers( 1, &shapesState.VBO );

    glBindVertexArray( shapesState.VAO );
    glBindBuffer( GL_ARRAY_BUFFER, shapesState.VBO );

    // Set up vertex attributes
    glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof( float ), (void *)0 );
    glEnableVertexAttribArray( 0 );

    shapesState.currentBufferSize = 0;

    TRACE_INFO( "Shapes buffers initialized successfully" );
}

void
InitShapes( void )
{
    TRACE_INFO( "Initializing shapes system" );

    // Load and compile the basic shapes shader
    shapesState.shader = LoadShaderFromMemory( basicShapesVS, basicShapesFS );
    if( shapesState.shader.id == 0 )
        {
            TRACE_ERROR( "Failed to load basic shapes shader" );
            return;
        }

    // Get shader uniform locations
    shapesState.colorLocation = GetShaderLocation( shapesState.shader, "uColor" );

    // Initialize vertex buffers
    InitShapesBuffers();

    TRACE_INFO( "Shapes system initialized successfully" );
}

void
CleanupShapes( void )
{
    TRACE_INFO( "Cleaning up shapes system" );

    UnloadShader( shapesState.shader );

    if( shapesState.VAO )
        {
            glDeleteVertexArrays( 1, &shapesState.VAO );
        }
    if( shapesState.VBO )
        {
            glDeleteBuffers( 1, &shapesState.VBO );
        }

    memset( &shapesState, 0, sizeof( ShapesState ) );

    TRACE_INFO( "Shapes system cleanup complete" );
}

static void
UpdateVertexBuffer( const float * vertices, int size )
{
    glBindBuffer( GL_ARRAY_BUFFER, shapesState.VBO );
    if( size > shapesState.currentBufferSize )
        {
            TRACE_INFO( "Resizing vertex buffer from %d to %d bytes", shapesState.currentBufferSize, size );
            glBufferData( GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW );
            shapesState.currentBufferSize = size;
        }
    else
        {
            glBufferSubData( GL_ARRAY_BUFFER, 0, size, vertices );
        }
}

static void
ScreenToNDC( float x, float y, float * ndcX, float * ndcY )
{
    *ndcX = 2.0f * x / core.window.screen.width - 1.0f;
    *ndcY = 1.0f - 2.0f * y / core.window.screen.height;
}

static void
SetShapeColor( Color color )
{
    float normalizedColor[4] = { color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f };

    BeginShaderMode( shapesState.shader );
    SetShaderValue( shapesState.shader, shapesState.colorLocation, normalizedColor, GL_FLOAT_VEC4 );
}

void
DrawPixel( int x, int y, Color color )
{
    float ndcX, ndcY;
    ScreenToNDC( (float)x, (float)y, &ndcX, &ndcY );

    float vertices[] = { ndcX, ndcY };

    SetShapeColor( color );
    UpdateVertexBuffer( vertices, sizeof( vertices ) );

    glBindVertexArray( shapesState.VAO );
    //glPointSize(1.0f);
    glDrawArrays( GL_POINTS, 0, 1 );

    EndShaderMode();
}

void
DrawLine( int startX, int startY, int endX, int endY, Color color )
{
    float ndcX1, ndcY1, ndcX2, ndcY2;
    ScreenToNDC( (float)startX, (float)startY, &ndcX1, &ndcY1 );
    ScreenToNDC( (float)endX, (float)endY, &ndcX2, &ndcY2 );

    float vertices[] = { ndcX1, ndcY1, ndcX2, ndcY2 };

    SetShapeColor( color );
    UpdateVertexBuffer( vertices, sizeof( vertices ) );

    glBindVertexArray( shapesState.VAO );
    glDrawArrays( GL_LINES, 0, 2 );

    EndShaderMode();
}

void
DrawTriangle( float x1, float y1, float x2, float y2, float x3, float y3, Color color )
{
    float ndcX1, ndcY1, ndcX2, ndcY2, ndcX3, ndcY3;
    ScreenToNDC( x1, y1, &ndcX1, &ndcY1 );
    ScreenToNDC( x2, y2, &ndcX2, &ndcY2 );
    ScreenToNDC( x3, y3, &ndcX3, &ndcY3 );

    float vertices[] = { ndcX1, ndcY1, ndcX2, ndcY2, ndcX3, ndcY3 };

    SetShapeColor( color );
    UpdateVertexBuffer( vertices, sizeof( vertices ) );

    glBindVertexArray( shapesState.VAO );
    glDrawArrays( GL_TRIANGLES, 0, 3 );

    EndShaderMode();
}

void
DrawTriangleLines( float x1, float y1, float x2, float y2, float x3, float y3, Color color )
{
    float ndcX1, ndcY1, ndcX2, ndcY2, ndcX3, ndcY3;
    ScreenToNDC( x1, y1, &ndcX1, &ndcY1 );
    ScreenToNDC( x2, y2, &ndcX2, &ndcY2 );
    ScreenToNDC( x3, y3, &ndcX3, &ndcY3 );

    float vertices[] = { ndcX1, ndcY1, ndcX2, ndcY2, ndcX2, ndcY2, ndcX3, ndcY3, ndcX3, ndcY3, ndcX1, ndcY1 };

    SetShapeColor( color );
    UpdateVertexBuffer( vertices, sizeof( vertices ) );

    glBindVertexArray( shapesState.VAO );
    glDrawArrays( GL_LINES, 0, 6 );

    EndShaderMode();
}

void
DrawRectangle( float x, float y, float width, float height, Color color )
{
    float vertices[12];
    float screenVertices[] = { x, y, x + width, y, x, y + height, x + width, y, x + width, y + height, x, y + height };

    // Convert all vertices to NDC
    for( int i = 0; i < 12; i += 2 )
        {
            float ndcX, ndcY;
            ScreenToNDC( screenVertices[i], screenVertices[i + 1], &ndcX, &ndcY );
            vertices[i]     = ndcX;
            vertices[i + 1] = ndcY;
        }

    SetShapeColor( color );
    UpdateVertexBuffer( vertices, sizeof( vertices ) );

    glBindVertexArray( shapesState.VAO );
    glDrawArrays( GL_TRIANGLES, 0, 6 );

    EndShaderMode();
}

void
DrawRectangleLines( float x, float y, float width, float height, Color color )
{
    float vertices[16];
    float screenVertices[] = { x,          y, x + width,  y, x + width,  y, x + width, y + height, x + width,
                               y + height, x, y + height, x, y + height, x, y };

    // Convert all vertices to NDC
    for( int i = 0; i < 16; i += 2 )
        {
            float ndcX, ndcY;
            ScreenToNDC( screenVertices[i], screenVertices[i + 1], &ndcX, &ndcY );
            vertices[i]     = ndcX;
            vertices[i + 1] = ndcY;
        }

    SetShapeColor( color );
    UpdateVertexBuffer( vertices, sizeof( vertices ) );

    glBindVertexArray( shapesState.VAO );
    glDrawArrays( GL_LINES, 0, 8 );

    EndShaderMode();
}

void
DrawCircle( float centerX, float centerY, float radius, Color color )
{
    const int segments    = 32;
    const int vertexCount = segments * 3;
    float     vertices[vertexCount * 2];

    float ndcCenterX, ndcCenterY;
    ScreenToNDC( centerX, centerY, &ndcCenterX, &ndcCenterY );

    float aspectRatio = (float)core.window.screen.width / core.window.screen.height;
    float radiusX     = radius * 2.0f / core.window.screen.width;
    float radiusY     = radius * 2.0f / core.window.screen.height;

    // Generate triangle vertices for the circle
    for( int i = 0; i < segments; i++ )
        {
            float angle1 = (float)i * 2.0f * PI / segments;
            float angle2 = (float)( i + 1 ) * 2.0f * PI / segments;

            // Center vertex
            vertices[i * 6]     = ndcCenterX;
            vertices[i * 6 + 1] = ndcCenterY;

            // First outer vertex
            vertices[i * 6 + 2] = ndcCenterX + cos( angle1 ) * radiusX;
            vertices[i * 6 + 3] = ndcCenterY + sin( angle1 ) * radiusY;

            // Second outer vertex
            vertices[i * 6 + 4] = ndcCenterX + cos( angle2 ) * radiusX;
            vertices[i * 6 + 5] = ndcCenterY + sin( angle2 ) * radiusY;
        }

    SetShapeColor( color );
    UpdateVertexBuffer( vertices, sizeof( vertices ) );

    glBindVertexArray( shapesState.VAO );
    glDrawArrays( GL_TRIANGLES, 0, vertexCount );

    EndShaderMode();
}

void
DrawCircleLines( float centerX, float centerY, float radius, Color color )
{
    const int segments    = 32;
    const int vertexCount = segments * 2;
    float     vertices[vertexCount * 2];

    float ndcCenterX, ndcCenterY;
    ScreenToNDC( centerX, centerY, &ndcCenterX, &ndcCenterY );

    float aspectRatio = (float)core.window.screen.width / core.window.screen.height;
    float radiusX     = radius * 2.0f / core.window.screen.width;
    float radiusY     = radius * 2.0f / core.window.screen.height;

    // Generate line vertices for the circle outline
    for( int i = 0; i < segments; i++ )
        {
            float angle1        = (float)i * 2.0f * PI / segments;
            float angle2        = (float)( ( i + 1 ) % segments ) * 2.0f * PI / segments;

            vertices[i * 4]     = ndcCenterX + cos( angle1 ) * radiusX;
            vertices[i * 4 + 1] = ndcCenterY + sin( angle1 ) * radiusY;
            vertices[i * 4 + 2] = ndcCenterX + cos( angle2 ) * radiusX;
            vertices[i * 4 + 3] = ndcCenterY + sin( angle2 ) * radiusY;
        }

    SetShapeColor( color );
    UpdateVertexBuffer( vertices, sizeof( vertices ) );

    glBindVertexArray( shapesState.VAO );
    glDrawArrays( GL_LINES, 0, vertexCount );

    EndShaderMode();
}

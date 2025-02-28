#include <math.h>
#include "levegl/levegl.h"

// Dracula theme color palette
#define DRACULA_BACKGROUND ( Color ){ 40, 42, 54, 255 }    // Background
#define DRACULA_CURRENT    ( Color ){ 68, 71, 90, 255 }    // Current line
#define DRACULA_PURPLE     ( Color ){ 189, 147, 249, 255 } // Purple
#define DRACULA_GREEN      ( Color ){ 80, 250, 123, 255 }  // Green
#define DRACULA_PINK       ( Color ){ 255, 121, 198, 255 } // Pink
#define DRACULA_CYAN       ( Color ){ 139, 233, 253, 255 } // Cyan
#define DRACULA_YELLOW     ( Color ){ 241, 250, 140, 255 } // Yellow
#define DRACULA_ORANGE     ( Color ){ 255, 184, 108, 255 } // Orange

// Global variables
const int screenWidth  = 480;
const int screenHeight = 272;
float     time         = 0.0f;
Transform leftGroup, centerGroup, rightGroup;

float
Oscillate( float time, float min, float max, float speed )
{
    return ( min + max ) / 2 + sin( time * speed ) * ( max - min ) / 2;
}

void
DrawRotatingTriangle( Transform transform, Color fillColor, Color lineColor )
{
    float radius = 50 * transform.scale;

    float x1     = transform.x + radius * cos( transform.rotation );
    float y1     = transform.y + radius * sin( transform.rotation );
    float x2 = transform.x + radius * cos( transform.rotation + 2.0f * PI / 3 );
    float y2 = transform.y + radius * sin( transform.rotation + 2.0f * PI / 3 );
    float x3 = transform.x + radius * cos( transform.rotation + 4.0f * PI / 3 );
    float y3 = transform.y + radius * sin( transform.rotation + 4.0f * PI / 3 );

    DrawTriangle( x1, y1, x2, y2, x3, y3, fillColor );
    DrawTriangleLines( x1, y1, x2, y2, x3, y3, lineColor );
}

void
DrawPulsatingCircle( Transform transform, Color fillColor, Color lineColor )
{
    DrawCircle( transform.x, transform.y, transform.scale, fillColor );
    DrawCircleLines( transform.x, transform.y, transform.scale + 5, lineColor );
}

void
DrawBreathingRectangle( Transform transform, Color fillColor, Color lineColor )
{
    float width  = 80 * transform.scale;
    float height = 60 * transform.scale;
    DrawRectangle( transform.x - width / 2, transform.y - height / 2, width,
                   height, fillColor );
    DrawRectangleLines( transform.x - width / 2, transform.y - height / 2,
                        width, height, lineColor );
}

void
UpdateDrawFrame( void )
{
    time += GetFrameTime();

    // Update transformations
    leftGroup.rotation   = time * 0.5f;
    leftGroup.scale      = Oscillate( time, 0.8f, 1.2f, 2.0f );

    centerGroup.rotation = -time * 0.7f;
    centerGroup.scale    = Oscillate( time, 0.9f, 15.0f, 1.5f );

    rightGroup.rotation  = time * 0.3f;
    rightGroup.scale     = Oscillate( time, 0.7f, 1.1f, 3.0f );

    BeginDrawing();
    ClearBackground( DRACULA_BACKGROUND );

    // Draw connecting lines between shape groups
    DrawLine( leftGroup.x, leftGroup.y, centerGroup.x, centerGroup.y,
              DRACULA_CURRENT );
    DrawLine( centerGroup.x, centerGroup.y, rightGroup.x, rightGroup.y,
              DRACULA_CURRENT );

    // Left group: Triangles
    DrawRotatingTriangle( leftGroup, DRACULA_PURPLE, DRACULA_PINK );

    // Center group: Circles
    DrawPulsatingCircle( centerGroup, DRACULA_GREEN, DRACULA_CYAN );

    // Right group: Rectangles
    DrawBreathingRectangle( rightGroup, DRACULA_YELLOW, DRACULA_ORANGE );

    // Draw pixels in a circular pattern around each shape
    for( float angle = 0; angle < 2 * PI; angle += PI / 8 )
        {
            float radius = 80 * ( 1 + sin( time * 2 + angle ) * 0.2f );
            DrawPixel( leftGroup.x + cos( angle ) * radius,
                       leftGroup.y + sin( angle ) * radius, DRACULA_PURPLE );
            DrawPixel( centerGroup.x + cos( angle ) * radius,
                       centerGroup.y + sin( angle ) * radius, DRACULA_GREEN );
            DrawPixel( rightGroup.x + cos( angle ) * radius,
                       rightGroup.y + sin( angle ) * radius, DRACULA_YELLOW );
        }

    EndDrawing();
}

int
main( void )
{
    InitWindow( screenWidth, screenHeight, "LeveGL Shapes showcase" );
    SetTargetFPS( 60 );

    // Define initial transforms for each shape group
    leftGroup =
        (Transform){ screenWidth * 0.25f, screenHeight * 0.5f, 0.0f, 1.0f };
    centerGroup =
        (Transform){ screenWidth * 0.5f, screenHeight * 0.5f, 0.0f, 1.0f };
    rightGroup =
        (Transform){ screenWidth * 0.75f, screenHeight * 0.5f, 0.0f, 1.0f };

    while( !ShouldQuit() )
        {
            UpdateDrawFrame();
        }

    CloseWindow();
    return 0;
}

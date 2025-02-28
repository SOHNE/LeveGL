#include <math.h>
#include <stdlib.h>
#include "levegl/levegl.h"

// Brazil Flag Colors
#define FLAG_GREEN  ( Color ){ 0, 156, 59, 255 }
#define FLAG_YELLOW ( Color ){ 255, 223, 0, 255 }
#define FLAG_BLUE   ( Color ){ 0, 39, 118, 255 }
#define FLAG_WHITE  ( Color ){ 255, 255, 255, 255 }

const int screenWidth  = 480;
const int screenHeight = 272;

void
UpdateDrawFrame( void )
{
    BeginDrawing();
    ClearBackground( FLAG_GREEN );

    // Draw yellow diamond
    DrawTriangle( 240, 0, 0, 136, 480, 136, FLAG_YELLOW );   // Upper triangle
    DrawTriangle( 0, 136, 480, 136, 240, 272, FLAG_YELLOW ); // Lower triangle

    // Draw blue circle
    DrawCircle( 240, 136, 85, FLAG_BLUE );

    // Draw simplified stars
    // Central larger star
    DrawCircle( 240, 136, 6, FLAG_WHITE );
    // Surrounding stars (cardinal directions)
    DrawCircle( 240, 136 - 50, 4, FLAG_WHITE );      // North
    DrawCircle( 240 + 45, 136 + 30, 4, FLAG_WHITE ); // South-East
    DrawCircle( 240 - 45, 136 + 30, 4, FLAG_WHITE ); // South-West
    DrawCircle( 240 + 25, 136 - 25, 4, FLAG_WHITE ); // North-East
    DrawCircle( 240 - 25, 136 - 25, 4, FLAG_WHITE ); // North-West

    EndDrawing();
}

int
main( void )
{
    SetConfigFlags( FLAG_MSAA_HINT | FLAG_VSYNC_HINT );
    InitWindow( screenWidth, screenHeight, "Minimalist Brazil Flag" );
    SetTargetFPS( 60 );

    atexit( CloseWindow );

    while( !ShouldQuit() )
        {
            UpdateDrawFrame();
        }

    return 0;
}

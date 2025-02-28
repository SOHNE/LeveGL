#include <math.h>
#include <stdlib.h>
#include "levegl/levegl.h"

int
main( void )
{
    const int screenWidth   = 480;
    const int screenHeight  = 272;
    float     rotationAngle = 0.0f;

    InitWindow( screenWidth, screenHeight, "LeveGL Triangle" );
    SetTargetFPS( 60 );

    if( atexit( CloseWindow ) )
        {
            return EXIT_SUCCESS;
        }

    while( !ShouldQuit() )
        {
            rotationAngle += 0.01f;

            float centerX = screenWidth / 2.0f;
            float centerY = screenHeight / 2.0f;
            float radius  = 100.0f;

            // Base angle with -PI/2 offset to point upward initially
            float baseAngle = rotationAngle - PI / 2;

            // Calculate vertices 120 degrees apart
            float x1 = centerX + radius * cos( baseAngle );
            float y1 = centerY + radius * sin( baseAngle );

            float x2 = centerX + radius * cos( baseAngle + 2 * PI / 3 );
            float y2 = centerY + radius * sin( baseAngle + 2 * PI / 3 );

            float x3 = centerX + radius * cos( baseAngle + 4 * PI / 3 );
            float y3 = centerY + radius * sin( baseAngle + 4 * PI / 3 );

            BeginDrawing();
            ClearBackground( WHITE );

            DrawTriangle( (int)x1, (int)y1, (int)x2, (int)y2, (int)x3, (int)y3,
                          BLACK );
            EndDrawing();
        }

    return EXIT_SUCCESS;
}

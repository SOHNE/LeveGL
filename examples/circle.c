#include "levegl/levegl.h"

int
main( void )
{
    InitWindow( 800, 600, "LeveGL Demo" );
    SetTargetFPS( 60 );

    while( !ShouldQuit() )
        {
            BeginDrawing();
            ClearBackground( WHITE );
            DrawCircle( 400, 300, 50, RED );
            EndDrawing();
        }

    CloseWindow();
    return 0;
}

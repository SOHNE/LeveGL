/*******************************************************************************************
*
*   LeveGL Core Example - Basic Window
*
*   Initially created with LeveGL v25.0.2
*
*   Demonstrates the basic setup and usage of LeveGL for window creation and
*   rendering.
*
*   Licensed under the zlib/libpng license.
*   Copyright (c) 2025 SOHNE, Leandro Peres (@zschzen)
*
********************************************************************************************/

#include "levegl/levegl.h"

int
main( void )
{
    /*------------------------- Setup -------------------------*/
    const int screenWidth  = 480;
    const int screenHeight = 272;

    InitWindow( screenWidth, screenHeight, "LeveGL: Basic Window" );
    SetTargetFPS( 60 );

    /*------------------------- Loop --------------------------*/
    while( !ShouldQuit() )
        {
            /*---------------- Internal Update ----------------*/
            if( IsKeyDown( KEY_ESCAPE ) )
                {
                    break;
                }

            /*-------------------- Render ---------------------*/
            BeginDrawing();
                ClearBackground( WHITE );
            EndDrawing();
        }

    /*----------------------- Shutdown ------------------------*/
    CloseWindow();
    return 0;
}

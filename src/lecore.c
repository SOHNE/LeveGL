/******************************** LECORE *********************************
 * lecore: Window management
 *
 *                           BACKEND PLATFORMS
 * ------------------------------------------------------------------------
 * DESKTOP:
 *     - GLFW (3.4)
 *
 *                                NOTES
 * ------------------------------------------------------------------------
 * WARN:
 *   - Early development stage
 *
 * TODO:
 *   - Design and implement an input system to handle user interactions.
 *   - Expand backend support for additional platforms.
 *
 *                               LICENSE
 * ------------------------------------------------------------------------
 * Copyright (c) 2024-2025 SOHNE, Leandro Peres (@zschzen)
 *
 * This software is provided "as-is", without any express or implied warranty. In no event
 * will the authors be held liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose, including commercial
 * applications, and to alter it and redistribute it freely, subject to the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not claim that you
 *   wrote the original software. If you use this software in a product, an acknowledgment
 *   in the product documentation would be appreciated but is not required.
 *
 *   2. Altered source versions must be plainly marked as such, and must not be misrepresented
 *   as being the original software.
 *
 *   3. This notice may not be removed or altered from any source distribution.
 *
 *************************************************************************/

//==============================================================================================================
// INCLUDES
//==============================================================================================================
#include "GL/glew.h"
#include "lecore_context.h"

#include "levegl/levegl.h"

#include <math.h>
#include <string.h>

//==============================================================================================================
// GLOBALS
//==============================================================================================================
CoreContext core = { 0 };

//==============================================================================================================
// MODULE FUNCTIONS DECLARATIONS
//==============================================================================================================
extern int  InitPlatform();
extern void ClosePlatform( void );

extern void InitShapes( void );
extern void CleanupShapes( void );

//==============================================================================================================
// MODULE FUNCTIONS DEFINITONS
//==============================================================================================================
void
InitWindow( int width, int height, const char * title )
{
    TRACE_INFO( "Initializing LeveGL - %s", LEVEGL_VERSION );

    core.window.screen.height = height;
    core.window.screen.width  = width;
    if( STR_NONEMPTY( title ) )
        {
            core.window.title = title;
        }

    InitPlatform();
}

void
CloseWindow()
{
    ClosePlatform();
    memset( &core, 0, sizeof( core ) );

    TRACE_INFO( "Window closed" );
}

void
SetTargetFPS( int fps )
{
    if( fps < 1 )
        {
            core.timing.targetFPS = 0.0;
        }
    else
        {
            core.timing.targetFPS = 1.0 / (double)fps;
        }
}

void
SetConfigFlags( unsigned int flags )
{
    FLAG_SET( core.window.flags, flags );
}

void
BeginDrawing( void )
{
    if( core.timing.targetFPS > 0 )
        {
            const double elapsedTime = GetTime() - core.timing.lastFrameTime;

            // Wait for remaining frame
            if( elapsedTime < core.timing.targetFPS )
                {
                    WaitTime( core.timing.targetFPS - elapsedTime );
                }
        }
}

void
EndDrawing( void )
{
    SwapBuffers();

    core.timing.lastFrameTime = GetTime();
    ++core.timing.frameCounter;
}

void
ClearBackground( Color color )
{
    glClearColor( color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f );
    glClear( GL_COLOR_BUFFER_BIT );
}

float
GetFrameTime( void )
{
    static double previousTime = 0.0;
    double        currentTime  = GetTime();
    float         deltaTime    = (float)( currentTime - previousTime );
    previousTime               = currentTime;
    return deltaTime;
}

int
GetFPS( void )
{
    return (int)roundf( 1.0f / GetFrameTime() );
}

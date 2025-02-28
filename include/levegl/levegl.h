/******************************** LEVEGL *********************************
*
* Version:    v0.0.1                   Author: Leandro Peres (@zschzen)
* Released:   2025-02-28               License: zlib/libpng (OSI Approved)
*
* A minimal graphics library designed for:
* - Educational purposes & computer graphics fundamentals
* - OpenGL-style API familiarity
* - C99 development workflows
*
*                            QUICK START
* ------------------------------------------------------------------------
* #include "levegl/levegl.h"
*
* int main( void ) {
*     SetConfigFlags( FLAG_MSAA_HINT | FLAG_WINDOW_RESIZABLE );
*     InitWindow(800, 600, "LeveGL Demo");
*     SetTargetFPS(60);
*
*     while (!ShouldQuit()) {
*         BeginDrawing();
*             ClearBackground(WHITE);
*             DrawCircle(400, 300, 50, RED);
*         EndDrawing();
*     }
*
*     CloseWindow();
*     return 0;
* }
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

#ifndef LEVEGL_H
#define LEVEGL_H

//==============================================================================================================
// INCLUDES
//==============================================================================================================
#include "levegl/leapi.h"
#include "levegl/leversion.h"

//==============================================================================================================
// DEFINES
//==============================================================================================================
#ifndef LEAPI
#    define LEAPI
#endif

// Boolean
#if !defined( __cplusplus ) && !defined( bool )
#    if defined( __has_include ) && __has_include( <stdbool.h> )
#        include <stdbool.h>
#    else
#        ifndef LEVE_BOOL_TYPE
typedef enum bool
{
    false = 0,
    true  = !false
} bool;
#            define LEVE_BOOL_TYPE
#        endif
#    endif
#endif

// Logging macros
// TODO: Implement a logging system
#if 0 //LE_DEBUG
#    define TRACE_LOG( level, fmt, ... ) LeTraceLog( level, __FILE__, __LINE__, fmt, ##__VA_ARGS__ )
#    define TRACE_INFO( fmt, ... )       TRACE_LOG( LOG_INFO, fmt, ##__VA_ARGS__ )
#    define TRACE_WARNING( fmt, ... )    TRACE_LOG( LOG_WARNING, fmt, ##__VA_ARGS__ )
#    define TRACE_ERROR( fmt, ... )      TRACE_LOG( LOG_ERROR, fmt, ##__VA_ARGS__ )
#else
#    define TRACE_LOG( level, fmt, ... )
#    define TRACE_INFO( fmt, ... )
#    define TRACE_WARNING( fmt, ... )
#    define TRACE_ERROR( fmt, ... )
#endif

// Predefined colors
#define BLACK     ( Color ) { 0, 0, 0, 255 }
#define WHITE     ( Color ) { 255, 255, 255, 255 }
#define GRAY      ( Color ) { 128, 128, 128, 255 }
#define DARKGRAY  ( Color ) { 64, 64, 64, 255 }
#define LIGHTGRAY ( Color ) { 192, 192, 192, 255 }
#define GREY      GRAY
#define DARKGREY  DARKGRAY
#define LIGHTGREY LIGHTGRAY

#define RED       ( Color ) { 255, 0, 0, 255 }
#define GREEN     ( Color ) { 0, 255, 0, 255 }
#define BLUE      ( Color ) { 0, 0, 255, 255 }

#define YELLOW    ( Color ) { 255, 255, 0, 255 }
#define ORANGE    ( Color ) { 255, 165, 0, 255 }
#define PURPLE    ( Color ) { 128, 0, 128, 255 }

#define PINK      ( Color ) { 255, 192, 203, 255 }
#define BROWN     ( Color ) { 165, 42, 42, 255 }
#define MAGENTA   ( Color ) { 255, 0, 255, 255 }
#define CYAN      ( Color ) { 0, 255, 255, 255 }
#define LIME      ( Color ) { 150, 255, 0, 255 }
#define MAROON    ( Color ) { 128, 0, 0, 255 }
#define NAVY      ( Color ) { 0, 0, 128, 255 }

// TODO: Design and implement lemath.h
#ifndef PI
#    define PI 3.14159265358979323846F
#endif

#ifndef TAU
#    define TAU 6.28318530717958647692F
#endif

#ifndef DEG2RAD
#    define DEG2RAD ( TAU / 360.0F )
#endif

#ifndef RAD2DEG
#    define RAD2DEG ( 360.0F / TAU )
#endif

#ifndef LERP
#    define LERP( a, b, t ) ( ( a ) + ( ( b ) - ( a ) ) * ( t ) )
#endif

// Flags operations
#define FLAG_SET( n, f )    ( ( n ) |= ( f ) )
#define FLAG_CLEAR( n, f )  ( ( n ) &= ~( f ) )
#define FLAG_TOGGLE( n, f ) ( ( n ) ^= ( f ) )
#define FLAG_CHECK( n, f )  ( ( n ) & ( f ) )

// String
#define STR_NONEMPTY( str ) ( ( str ) != NULL && ( str )[0] != '\0' )

//==============================================================================================================
// STRUCTS
//==============================================================================================================

// Color
typedef struct Color
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
} Color;

// Transform
typedef struct Transform
{
    float x;
    float y;
    float rotation;
    float scale;
} Transform;

// Shader
typedef struct Shader
{
    unsigned int * locations;
    unsigned int   id;
    int            locCount;
    bool           active;
} Shader;

//===========================================================================================================
// ENUMERATORS
//===========================================================================================================
typedef enum
{
    FLAG_NONE             = 0,
    FLAG_VSYNC_HINT       = 1 << 0, // 0x01: Enable vertical sync
    FLAG_WINDOW_RESIZABLE = 1 << 1, // 0x02: Allow window resizing
    FLAG_MSAA_HINT        = 1 << 2  // 0x04: Enable MSAA (Multi-Sample Anti-Aliasing)
} ConfigFlags;

// Log levels
typedef enum
{
    LOG_INFO = 0,
    LOG_WARNING,
    LOG_ERROR
} LogLevel;

//===========================================================================================================
// FUNCTIONS DECLARATIONS
//===========================================================================================================

#if defined( __cplusplus )
extern "C"
{
#endif

    //--- CORE --------------------------------------------------------------------------------------------------

    // Window
    LEAPI void   InitWindow( int width, int height, const char * title );
    LEAPI void   CloseWindow( void );
    LEAPI void   SetWindowTitle( const char * title );
    LEAPI bool   ShouldQuit( void );
    LEAPI void * GetWindowHandle( void );

    // Timing functions
    LEAPI void   SetTargetFPS( int fps );
    LEAPI float  GetFrameTime( void );
    LEAPI double GetTime( void );
    LEAPI int    GetFPS( void );
    LEAPI void   SwapBuffers( void );
    LEAPI void   WaitTime( double seconds );

    // Drawing functions
    LEAPI void ClearBackground( Color color );
    LEAPI void BeginDrawing( void );
    LEAPI void EndDrawing( void );

    // Shader functions
    LEAPI Shader LoadShader( const char * vsFileName, const char * fsFileName );
    LEAPI Shader LoadShaderFromMemory( const char * vsCode, const char * fsCode );
    LEAPI void   UnloadShader( Shader shader );
    LEAPI void   BeginShaderMode( Shader shader );
    LEAPI void   EndShaderMode( void );
    LEAPI int    GetShaderLocation( Shader shader, const char * uniformName );
    LEAPI void   SetShaderValue( Shader shader, int locIndex, const void * value, int uniformType );
    LEAPI void   SetShaderValueV( Shader shader, int locIndex, const void * value, int uniformType, int count );

    // Miscellaneous core functions
    LEAPI void LeTraceLog( LogLevel level, const char * file, int line, const char * fmt, ... );
    LEAPI void SetConfigFlags( unsigned int flags );

    //---------------------------------------------------------------------------------------------- CORE ---//

    //--- SHAPES ------------------------------------------------------------------------------------------------

    // Basic shapes drawing functions
    LEAPI void DrawPixel( int x, int y, Color color );
    LEAPI void DrawLine( int startX, int startY, int endX, int endY, Color color );
    LEAPI void DrawTriangle( float x1, float y1, float x2, float y2, float x3, float y3, Color color );
    LEAPI void DrawTriangleLines( float x1, float y1, float x2, float y2, float x3, float y3, Color color );
    LEAPI void DrawRectangle( float x, float y, float width, float height, Color color );
    LEAPI void DrawRectangleLines( float x, float y, float width, float height, Color color );
    LEAPI void DrawCircle( float centerX, float centerY, float radius, Color color );
    LEAPI void DrawCircleLines( float centerX, float centerY, float radius, Color color );

    //-------------------------------------------------------------------------------------------- SHAPES ---//

#if defined( __cplusplus )
}
#endif

#endif // LEVEGL_H

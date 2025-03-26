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

#include <stdarg.h> /* va_list */

//==============================================================================================================
// DEFINES
//==============================================================================================================
#ifndef LEAPI
#    define LEAPI
#endif

/// Compiler attributes
#if defined( __GNUC__ ) || defined( __clang__ )
#    define NORETURN      __attribute__( ( noreturn ) )
#    define PACKED        __attribute__( ( packed ) )
#    define INLINE        inline __attribute__( ( always_inline ) )
#    define ALIGNED( x )  __attribute__( ( aligned( x ) ) ) // Align variables/structs
#    define DEPRECATED    __attribute__( ( deprecated ) )   // Mark as deprecated
#    define LIKELY( x )   __builtin_expect( !!( x ), 1 )    // Branch prediction hint
#    define UNLIKELY( x ) __builtin_expect( !!( x ), 0 )
#elif defined( _MSC_VER )
#    define NORETURN      __declspec( noreturn )
#    define PACKED        __pragma( pack( push, 1 ) )
#    define INLINE        __forceinline
#    define ALIGNED( x )  __declspec( align( x ) )
#    define DEPRECATED    __declspec( deprecated )
#    define LIKELY( x )   ( x )
#    define UNLIKELY( x ) ( x )
#else
#    define NORETURN
#    define PACKED
#    define INLINE inline
#    define ALIGNED( x )
#    define DEPRECATED
#    define LIKELY( x )   ( x )
#    define UNLIKELY( x ) ( x )
#endif

// C++ compatibility, preventing name mangling
#if defined( __cplusplus )
/* clang-format off */
#    define CXX_GUARD_START extern "C" {
#    define CXX_GUARD_END   }
/* clang-format on */
#else
#    define CXX_GUARD_START
#    define CXX_GUARD_END
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

// Predefined colors
#define BLACK     ( Color ) { 000.F / 255.F, 000.F / 255.F, 000.F / 255.F, 1.F }
#define WHITE     ( Color ) { 255.F / 255.F, 255.F / 255.F, 255.F / 255.F, 1.F }
#define GRAY      ( Color ) { 128.F / 255.F, 128.F / 255.F, 128.F / 255.F, 1.F }
#define DARKGRAY  ( Color ) { 064.F / 255.F, 064.F / 255.F, 064.F / 255.F, 1.F }
#define LIGHTGRAY ( Color ) { 192.F / 255.F, 192.F / 255.F, 192.F / 255.F, 1.F }

#define RED       ( Color ) { 255.F / 255.F, 000.F / 255.F, 000.F / 255.F, 1.F }
#define GREEN     ( Color ) { 000.F / 255.F, 255.F / 255.F, 000.F / 255.F, 1.F }
#define BLUE      ( Color ) { 000.F / 255.F, 000.F / 255.F, 255.F / 255.F, 1.F }

#define YELLOW    ( Color ) { 255.F / 255.F, 255.F / 255.F, 000.F / 255.F, 1.F }
#define ORANGE    ( Color ) { 255.F / 255.F, 165.F / 255.F, 000.F / 255.F, 1.F }
#define PURPLE    ( Color ) { 128.F / 255.F, 000.F / 255.F, 128.F / 255.F, 1.F }
#define PINK      ( Color ) { 255.F / 255.F, 192.F / 255.F, 203.F / 255.F, 1.F }
#define BROWN     ( Color ) { 165.F / 255.F, 042.F / 255.F, 042.F / 255.F, 1.F }
#define MAGENTA   ( Color ) { 255.F / 255.F, 000.F / 255.F, 255.F / 255.F, 1.F }
#define CYAN      ( Color ) { 000.F / 255.F, 255.F / 255.F, 255.F / 255.F, 1.F }
#define LIME      ( Color ) { 150.F / 255.F, 255.F / 255.F, 000.F / 255.F, 1.F }
#define MAROON    ( Color ) { 128.F / 255.F, 000.F / 255.F, 000.F / 255.F, 1.F }
#define NAVY      ( Color ) { 000.F / 255.F, 000.F / 255.F, 128.F / 255.F, 1.F }

// Colors alias
#define GREY      GRAY
#define DARKGREY  DARKGRAY
#define LIGHTGREY LIGHTGRAY

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

#define UNUSED( x )         (void)( x )

//==============================================================================================================
// STRUCTS
//==============================================================================================================

// Color
typedef struct Color
{
    float r;
    float g;
    float b;
    float a;
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
    LOG_ALL = 0, // Display all logs
    LOG_TRACE,   // Trace logging, intended for internal use only
    LOG_DEBUG,   // Debug logging, used for internal debugging, it should be disabled on release builds
    LOG_INFO,    // Info logging, used for program execution info
    LOG_WARNING, // Warning logging, used on recoverable failures
    LOG_ERROR,   // Error logging, used on unrecoverable failures
    LOG_FATAL,   // Fatal logging, used to abort program: exit(EXIT_FAILURE)
    LOG_NONE     // Disable logging
} LogLevel;

//===========================================================================================================
// Functions callbacks
//===========================================================================================================
typedef void ( *TraceLogCallback )( int logLevel, const char * text, va_list args ); // Custom trace log

//===========================================================================================================
// FUNCTIONS DECLARATIONS
//===========================================================================================================

CXX_GUARD_START

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
LEAPI void SetTraceLogCallback( TraceLogCallback callback ); // Set custom trace log
LEAPI void TraceLog( int logLevel, const char * text, ... ); // Display a log message

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

CXX_GUARD_END

#endif // LEVEGL_H

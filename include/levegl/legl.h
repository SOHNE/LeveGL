/****************************** CameCore *********************************
 *
 * Module: leGL, Low-Level OpenGL
 *
 * Handles OpenGL extension loading and context initialization
 * using the GLAD loader for OpenGL 3.3+ contexts.
 *
 * Key Features:
 * - GLAD-based OpenGL extension loading
 * - OpenGL context verification
 * - Cross-platform extension management
 *
 *                               LICENSE
 * ------------------------------------------------------------------------
 * Copyright (c) 2025 SOHNE, Leandro Peres (@zschzen)
 *
 * This software is provided "as-is", without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from the use
 * of this software.
 *
 * Permission is granted to anyone to use this software for any purpose, including
 * commercial applications, and to alter it and redistribute it freely, subject to the
 * following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not claim that
 *      you wrote the original software. If you use this software in a product, an
 *      acknowledgment in the product documentation would be appreciated but is not required.
 *
 *   2. Altered source versions must be plainly marked as such, and must not be misrepresented
 *      as being the original software.
 *
 *   3. This notice may not be removed or altered from any source distribution.
 *
 *************************************************************************/

#ifndef LEGL_H
#define LEGL_H

#include "levegl/leapi.h"

//----------------------------------------------------------------------------------------------------------------------
// Module Defines and Macros
//----------------------------------------------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------------------------------------------
// Module Functions Declarations
//----------------------------------------------------------------------------------------------------------------------
// Initialize OpenGL extensions using platform-specific loader
LEAPI void leLoadExtensions( void * loaderPtr );               // Load the required required OpenGL extensions

LEAPI void leClearColor( float r, float g, float b, float a ); // Clear the color buffer with the given color
LEAPI void leClear( unsigned int mask );                       // Clear the given mask
LEAPI void leClearScreenBuffers( void );                       // Clear both color and depth buffers

LEAPI void leViewport( int x, int y, int width, int height );  // Set the viewport

//**********************************************************************************************************************
//
// Module Implementation
//
//**********************************************************************************************************************
#ifdef LEGL_IMPLEMENTATION

/* Include OpenGL related */
#    if defined( GRAPHICS_API_OPENGL_33 )
#        define GLAD_GL_IMPLEMENTATION
#        include "glad/gl.h"
#    elif defined( GRAPHICS_API_OPENGL_ES2 )
#        define GLAD_GLES2_IMPLEMENTATION
#        include "glad/gles2.h"
#    endif

/* Ensure TRACE macros */
#    if false == defined( TRACELOG )
#        define TRACELOG( level, ... ) ( (void)( 0 ) )
#        define TRACELOGD( ... )       ( (void)( 0 ) )
#    endif // !TRACELOG

//----------------------------------------------------------------------------------------------------------------------
// Module Functions Definitions
//----------------------------------------------------------------------------------------------------------------------
// Load OpenGL extensions using GLAD with platform-specific function loader
void
leLoadExtensions( void * loaderPtr )
{
#    if defined( GRAPHICS_API_OPENGL_33 )
    if( 0 == gladLoadGL( (GLADloadfunc)loaderPtr ) )
        {
            TRACELOG( LOG_WARNING, "GLAD: Unable to load the required OpenGL 3.3 extensions" );
        }
    else
        {
            TRACELOG( LOG_INFO, "GLAD: OpenGL 3.3 extensions loaded" );
        }
#    endif // GRAPHICS_API_OPENGL_33

#    if defined( GRAPHICS_API_OPENGL_ES2 )
    if( 0 == gladLoadGLES2( (GLADloadfunc)loaderPtr ) )
        {
            TRACELOG( LOG_WARNING, "GLAD: Unable to load the required OpenGL ES2.0 extensions" );
        }
    else
        {
            TRACELOG( LOG_INFO, "GLAD: OpenGL ES 2.0 loaded successfully" );
        }
#    endif // GRAPHICS_API_OPENGL_ES2

    /* Display OpenGL and GLSL information */
    {
        TRACELOG( LOG_INFO, "OpenGL Device Information:" );
        TRACELOG( LOG_INFO, "  - Vendor  : %s", glGetString( GL_VENDOR ) );
        TRACELOG( LOG_INFO, "  - Renderer: %s", glGetString( GL_RENDERER ) );
        TRACELOG( LOG_INFO, "  - Version : %s", glGetString( GL_VERSION ) );
        TRACELOG( LOG_INFO, "  - GLSL    : %s", glGetString( GL_SHADING_LANGUAGE_VERSION ) );
    }
}

// Clear the color buffer with the provided color values
void
leClearColor( float r, float g, float b, float a )
{
    glClearColor( r, g, b, a );
}

// Clear the given mask
void
leClear( unsigned int mask )
{
    glClear( (GLbitfield)mask );
}

// Clear both color and depth buffers
void
leClearScreenBuffers( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

// Set the viewport
void
leViewport( int x, int y, int width, int height )
{
    glViewport( x, y, width, height );
}

#endif // LEGL_IMPLEMENTATION
#endif // !LEGL_H

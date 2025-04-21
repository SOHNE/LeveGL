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
 *                            CONFIGURATIONS
 * ------------------------------------------------------------------------
 *  LEGL_SHOW_GL_INFO:
 *      - Display OpenGL info, extensions and capabilities.
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
#ifndef LEAPI
#    define LEAPI
#endif

//----------------------------------------------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------------------------------------------
// ...

//----------------------------------------------------------------------------------------------------------------------
// Module Functions Declarations
//----------------------------------------------------------------------------------------------------------------------
// Initialize OpenGL extensions using platform-specific loader
LEAPI void leLoadExtensions( void * loaderPtr );                  // Load the required required OpenGL extensions

LEAPI void leClearColor( float r, float g, float b, float a );    // Clear the color buffer with the given color
LEAPI void leClear( unsigned int mask );                          // Clear the given mask
LEAPI void leClearScreenBuffers( void );                          // Clear both color and depth buffers

LEAPI void leViewport( int x, int y, int width, int height );     // Set the viewport

LEAPI void leStencilFunc( int func, int ref, unsigned int mask ); // Set stencil test function, reference value and mask
LEAPI void leStencilOp( int sfail, int dpfail, int dppass );      // Set stencil operations for different test outcomes
LEAPI void leStencilMask( unsigned int mask );                    // Control which bits in stencil buffer are writable

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

/* Custom memory allocators */
#    ifndef LE_MALLOC
#        define LE_MALLOC( bytes ) malloc( bytes )
#    endif
#    ifndef LE_CALLOC
#        define LE_CALLOC( count, bytes ) calloc( ( count ), ( bytes ) )
#    endif
#    ifndef LE_REALLOC
#        define LE_REALLOC( ptr, bytes ) realloc( ( ptr ), ( bytes ) )
#    endif
#    ifndef LE_FREE
#        define LE_FREE( ptr ) free( ptr )
#    endif

//----------------------------------------------------------------------------------------------------------------------
// Types & Structures Definitions
//----------------------------------------------------------------------------------------------------------------------
#    if defined( GRAPHICS_API_OPENGL_33 ) || defined( GRAPHICS_API_OPENGL_ES2 )
typedef void * ( *leglLoadProc )( const char * name );
#    endif // defined( GRAPHICS_API_OPENGL_33 ) || defined ( GRAPHICS_API_OPENGL_ES2 )

//----------------------------------------------------------------------------------------------------------------------
// Module Functions Definitions
//----------------------------------------------------------------------------------------------------------------------
/* ------------------ Core OpenGL Functions ------------------ */
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
    // Store extensions for ES2.0
    const char * extensionsStr  = (const char *)glGetString( GL_EXTENSIONS );
    char *       extensionsCopy = NULL;
    char **      extensionList  = NULL;
    int          extensionCount = 0;
    if( NULL != extensionsStr )
        {
            // Create a copy of the extensions string to tokenize
            extensionsCopy = strdup( extensionsStr );
            // First pass: count extensions
            char * token   = strtok( extensionsCopy, " " );
            while( NULL != token )
                {
                    extensionCount++;
                    token = strtok( NULL, " " );
                }
            // Allocate array for extensions
            extensionList = (char **)malloc( extensionCount * sizeof( char * ) );
            // Reset copy and do second pass to store extensions
            free( extensionsCopy );
            extensionsCopy = strdup( extensionsStr );
            extensionCount = 0;
            token          = strtok( extensionsCopy, " " );
            while( token != NULL )
                {
                    extensionList[extensionCount] = strdup( token );
                    extensionCount++;
                    token = strtok( NULL, " " );
                }
        }
#    endif // GRAPHICS_API_OPENGL_ES2
    // Display OpenGL and GLSL information
    TRACELOG( LOG_INFO, "OpenGL Device Information:" );
    TRACELOG( LOG_INFO, "  - Vendor  : %s", glGetString( GL_VENDOR ) );
    TRACELOG( LOG_INFO, "  - Renderer: %s", glGetString( GL_RENDERER ) );
    TRACELOG( LOG_INFO, "  - Version : %s", glGetString( GL_VERSION ) );
    TRACELOG( LOG_INFO, "  - GLSL    : %s", glGetString( GL_SHADING_LANGUAGE_VERSION ) );

    // Display extension count
#    if defined( GRAPHICS_API_OPENGL_33 )
    GLint numExtensions = 0;
    glGetIntegerv( GL_NUM_EXTENSIONS, &numExtensions );
    TRACELOG( LOG_INFO, "Extensions Count: %d", numExtensions );
#    elif defined( GRAPHICS_API_OPENGL_ES2 )
    TRACELOG( LOG_INFO, "Extensions Count: %d", extensionCount );
#    endif

#    if defined( LEGL_SHOW_GL_INFO )
    TRACELOG( LOG_INFO, "Supported OpenGL Extensions:" );
#        if defined( GRAPHICS_API_OPENGL_33 )
    for( GLint i = 0; i < numExtensions; ++i )
        {
            const char * extensionName = (const char *)glGetStringi( GL_EXTENSIONS, i );
            TRACELOG( LOG_INFO, "  - %s", extensionName );
        }
#        elif defined( GRAPHICS_API_OPENGL_ES2 )
    if( NULL != extensionsStr )
        {
            for( int i = 0; i < extensionCount; ++i )
                {
                    TRACELOG( LOG_INFO, "  - %s", extensionList[i] );
                }
        }
    else
        {
            TRACELOG( LOG_WARNING, "No extensions found" );
        }
#        endif // GRAPHICS_API_OPENGL_ES2
#    endif     // LEGL_SHOW_GL_INFO
               // Cleanup for ES2.0
#    if defined( GRAPHICS_API_OPENGL_ES2 )
    if( NULL != extensionList )
        {
            for( int i = 0; i < extensionCount; ++i )
                {
                    LE_FREE( extensionList[i] );
                }
            LE_FREE( extensionList );
            LE_FREE( extensionsCopy );
        }
#    endif
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

/* -------------------- Stencil Functions -------------------- */
// Set the stencil test function and reference value
void
leStencilFunc( int func, int ref, unsigned int mask )
{
    // Controls how stencil test is performed and which fragments pass
    // func: stencil test function (e.g., GL_EQUAL, GL_LESS)
    // ref: reference value to compare against
    // mask: bit mask applied to reference and stored stencil values before comparison
    glStencilFunc( func, ref, mask );
}

// Set the stencil buffer operations
void
leStencilOp( int sfail, int dpfail, int dppass )
{
    // Specifies what happens to the stencil buffer value under different test outcomes
    // sfail: action when stencil test fails
    // dpfail: action when stencil test passes but depth test fails
    // dppass: action when both stencil and depth tests pass
    glStencilOp( sfail, dpfail, dppass );
}

// Control which bits in the stencil buffer are writable
void
leStencilMask( unsigned int mask )
{
    // Sets a bit mask that controls which bits in the stencil buffer can be modified
    // mask: bit mask where 1 means writable, 0 means protected
    glStencilMask( mask );
}

#endif // LEGL_IMPLEMENTATION
#endif // !LEGL_H


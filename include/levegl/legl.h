/******************************** LEGL **********************************
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

#ifndef INLINE
#    define INLINE inline
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

/* Ensure TRACELOG macros */
#ifndef TRACELOG
#    define TRACELOG( level, ... ) ( (void)( 0 ) )
#    define TRACELOGD( ... )       ( (void)( 0 ) )
#endif // !TRACELOG

/* Custom memory allocators */
#ifndef LE_MALLOC
#    define LE_MALLOC( b ) malloc( b )
#endif
#ifndef LE_CALLOC
#    define LE_CALLOC( c, b ) calloc( ( c ), ( b ) )
#endif
#ifndef LE_REALLOC
#    define LE_REALLOC( p, b ) realloc( ( p ), ( b ) )
#endif
#ifndef LE_FREE
#    define LE_FREE( p ) free( p )
#endif

/* Ensure OpenGL API version */
#if !defined( GRAPHICS_API_OPENGL_33 ) && !defined( GRAPHICS_API_OPENGL_ES2 ) && !defined( GRAPHICS_API_OPENGL_ES3 )
#    define GRAPHICS_API_OPENGL_33
#endif

#if defined( GRAPHICS_API_OPENGL_ES2 )
#    define glClearDepth glClearDepthf
#endif

//----------------------------------------------------------------------------------------------------------------------
// Types & Structures Definitions
//----------------------------------------------------------------------------------------------------------------------
#if defined( GRAPHICS_API_OPENGL_33 ) || defined( GRAPHICS_API_OPENGL_ES2 )

// Current legl State and Configs
typedef struct leglContext
{
    struct
    {
        int framebufferWidth;  // Current framebuffer width
        int framebufferHeight; // Current framebuffer height

    } State;

    struct
    {
        unsigned int defaultVShaderId; // Default vertex shader id
        unsigned int defaultFShaderId; // Default fragment shader id

    } Shader;

} leglContext;

typedef void * ( *leglLoadProc )( const char * name );
#endif // defined( GRAPHICS_API_OPENGL_33 ) || defined ( GRAPHICS_API_OPENGL_ES2 )

//----------------------------------------------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------------------------------------------
#if defined( GRAPHICS_API_OPENGL_33 ) || defined( GRAPHICS_API_OPENGL_ES2 )
static leglContext leState = { 0 };
#endif // GRAPHICS_API_OPENGL_33 || GRAPHICS_API_OPENGL_ES2

//----------------------------------------------------------------------------------------------------------------------
// Module Specific Functions Declarations
//----------------------------------------------------------------------------------------------------------------------
#if defined( GRAPHICS_API_OPENGL_33 ) || defined( GRAPHICS_API_OPENGL_ES2 )
// Load default shader
static void leLoadDefaultShaders( void );
// Unload default shader
static void leUnloadShaderDefault( void );

#endif // GRAPHICS_API_OPENGL_33 || GRAPHICS_API_OPENGL_ES2

//----------------------------------------------------------------------------------------------------------------------
// Module Functions Declarations
//----------------------------------------------------------------------------------------------------------------------

CXX_GUARD_START

LEAPI void leInit( int width, int height ); // Initialize OpenGL states

// Initialize OpenGL extensions using platform-specific loader
LEAPI void leLoadExtensions( void * loaderPtr );               // Load the required required OpenGL extensions
LEAPI void leEnable( unsigned int capability, int enable );    // Enable or disable a GL capability

LEAPI void leClearColor( float r, float g, float b, float a ); // Clear the color buffer with the given color
LEAPI void leClear( unsigned int mask );                       // Clear the given mask
LEAPI void leClearScreenBuffers( void );                       // Clear both color and depth buffers

LEAPI void leViewport( int x, int y, int width, int height );  // Set the viewport

// Framebuffer
LEAPI unsigned int leCreateFramebuffer( void );                                // Create a rendering Framebuffer
LEAPI void         leDeleteFramebuffer( unsigned int fb );                     // Delete a given framebuffer object
LEAPI void leBindFramebuffer( unsigned int target, unsigned int framebuffer ); // Bind the given framebuffer (FBO)
LEAPI void leUnbindFramebuffer( unsigned int target );                         // Unbind the given framebuffer(FBO)
LEAPI int  leCheckFramebufferStatus( unsigned int target );                    // Check if a framebuffer is complete

// Shader
LEAPI unsigned int leCompileShader( const char * shaderCode, int type );              // Compile shader. Return its id
LEAPI unsigned int leLoadShaderProgram( unsigned int vertexId, unsigned int fragId ); // Load custom shader program

// Drawing
LEAPI void leDrawArrays( unsigned int mode, int first, int count ); //Draw arrays (GL_TRIANGLES, GL_LINES, ...)
LEAPI void leDrawVertexArray( int offset, int count );              // Draw vertex array

// Stencil
LEAPI void leStencilFunc( int func, int ref, unsigned int mask ); // Set stencil test function, reference value and mask
LEAPI void leStencilOp( int sfail, int dpfail, int dppass );      // Set stencil operations for different test outcomes
LEAPI void leStencilMask( unsigned int mask );                    // Control which bits in stencil buffer are writable

CXX_GUARD_END

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

//----------------------------------------------------------------------------------------------------------------------
// Module Functions Definitions
//----------------------------------------------------------------------------------------------------------------------
/* ------------------ Core OpenGL Functions ------------------ */

// Initialize OpenGL states
INLINE void
leInit( int width, int height )
{
#    if defined( GRAPHICS_API_OPENGL_33 ) || defined( GRAPHICS_API_OPENGL_ES2 )
    leLoadDefaultShaders();
#    endif

    // Initialize OpenGL
    //----------------------------------------------------------
    // Depth test
    glDepthFunc( GL_LEQUAL ); // Depth testing to apply

    // Blending mode
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ); // Color blending function
    glEnable( GL_BLEND );                                // Enable color blending, allowing transparencies

    // Culling
    glCullFace( GL_BACK );                    // Cull the back face
    glFrontFace( GL_CCW );                    // Front face as counter clockwise
    glEnable( GL_CULL_FACE );                 // Enable backface culling

#    if defined( GRAPHICS_API_OPENGL_33 )
    glEnable( GL_TEXTURE_CUBE_MAP_SEAMLESS ); // Enable seamless cubemap texture accesses
#    endif

#    if defined( GRAPHICS_API_OPENGL_33 ) || defined( GRAPHICS_API_OPENGL_ES2 )
    // Keep screen size
    leState.State.framebufferWidth  = width;
    leState.State.framebufferHeight = height;
#    endif

    TRACELOG( LOG_INFO, "LEGL: Default OpenGL state initialized successfully" );

    // Color/Depth buffers clear
    //----------------------------------------------------------
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );               // Set clear color
    glClearDepth( 1.0f );                                 // Set clear depth value
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); // Clear color and depth buffers
}

// Load OpenGL extensions using GLAD with platform-specific function loader
INLINE void
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
            size_t len     = strlen( extensionsStr ) + 1; // +1 for null terminator
            extensionsCopy = (char *)malloc( len );
            if( extensionsCopy != NULL )
                {
                    memcpy( extensionsCopy, extensionsStr, len );
                }

            // First pass: count extensions
            char * token = strtok( extensionsCopy, " " );
            while( NULL != token )
                {
                    extensionCount++;
                    token = strtok( NULL, " " );
                }
            // Allocate array for extensions
            extensionList = (char **)LE_MALLOC( extensionCount * sizeof( char * ) );
            // Reset copy and do second pass to store extensions
            free( extensionsCopy );

            // Do another custom strdup
            len            = strlen( extensionsStr ) + 1;
            extensionsCopy = (char *)LE_MALLOC( len );
            if( NULL != extensionsCopy )
                {
                    memcpy( extensionsCopy, extensionsStr, len );
                }

            extensionCount = 0;
            token          = strtok( extensionsCopy, " " );
            while( NULL != token )
                {
                    size_t tokenLen               = strlen( token ) + 1;
                    extensionList[extensionCount] = (char *)LE_MALLOC( tokenLen );
                    if( NULL != extensionList[extensionCount] )
                        {
                            memcpy( extensionList[extensionCount], token, tokenLen );
                        }

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

// Enable or disable a GL capability
INLINE void
leEnable( uint32_t capability, int enable )
{
    if( enable )
        glEnable( capability );
    else
        glDisable( capability );
}

// Clear the color buffer with the provided color values
INLINE void
leClearColor( float r, float g, float b, float a )
{
    glClearColor( r, g, b, a );
}

// Clear the given mask
INLINE void
leClear( unsigned int mask )
{
    glClear( (GLbitfield)mask );
}

// Clear both color and depth buffers
INLINE void
leClearScreenBuffers( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

// Set the viewport
INLINE void
leViewport( int x, int y, int width, int height )
{
    glViewport( x, y, width, height );
}

/* -------------------- Shader Functions --------------------- */
// Load default shader
INLINE void
leLoadDefaultShaders( void )
{
    // Shader configuration based on graphics API
    //----------------------------------------------------------
    struct ShaderConfig
    {
        const char * glslVersion;
        const char * inKeyword;
        const char * outKeyword;
        const char * varyingKeyword;
        const char * textureFunc;
        const char * fragColorOutput;
        const char * fragColorDeclare;
    };

    struct ShaderConfig config = { 0 };

    // Populate `config`
    //----------------------------------------------------------
#    if defined( GRAPHICS_API_OPENGL_33 )
    config.glslVersion      = "#version 330 core\n";
    config.inKeyword        = "in";
    config.outKeyword       = "out";
    config.varyingKeyword   = "in";
    config.textureFunc      = "texture";
    config.fragColorOutput  = "finalColor";
    config.fragColorDeclare = "out vec4 finalColor;\n";

#    elif defined( GRAPHICS_API_OPENGL_ES3 )
    config.glslVersion      = "#version 300 es\nprecision mediump float;\n";
    config.inKeyword        = "in";
    config.outKeyword       = "out";
    config.varyingKeyword   = "in";
    config.textureFunc      = "texture";
    config.fragColorOutput  = "fragColor";
    config.fragColorDeclare = "out vec4 fragColor;\n";

#    elif defined( GRAPHICS_API_OPENGL_ES2 )
    config.glslVersion      = "#version 100\nprecision mediump float;\n";
    config.inKeyword        = "attribute";
    config.outKeyword       = "varying";
    config.varyingKeyword   = "varying";
    config.textureFunc      = "texture2D";
    config.fragColorOutput  = "gl_FragColor";
    config.fragColorDeclare = ""; // gl_FragColor is built-in
#    endif

    // Build vertex shader
    //----------------------------------------------------------
    char defaultVShaderCode[1024];
    snprintf( defaultVShaderCode, sizeof( defaultVShaderCode ),
              "%s"                        // GLSL version
              "%s vec3 vertexPosition;\n" // Vertex position input
              "%s vec2 vertexTexCoord;\n" // Texture coordinate input
              "%s vec4 vertexColor;\n"    // Vertex color input
              "%s vec2 fragTexCoord;\n"   // Texture coordinate output
              "%s vec4 fragColor;\n"      // Vertex color output
              "uniform mat4 mvp;\n"       // Model-view-projection matrix
              "void main()\n"
              "{\n"
              "    fragTexCoord = vertexTexCoord;\n"
              "    fragColor = vertexColor;\n"
              "    gl_Position = mvp * vec4(vertexPosition, 1.0);\n"
              "}\n",
              config.glslVersion, config.inKeyword, config.inKeyword, config.inKeyword, // Vertex inputs
              config.outKeyword, config.outKeyword                                      // Vertex outputs
    );

    // Build fragment shader
    //----------------------------------------------------------
    char defaultFShaderCode[1024];
    snprintf( defaultFShaderCode, sizeof( defaultFShaderCode ),
              "%s"                            // GLSL version
              "%s vec2 fragTexCoord;\n"       // Texture coordinate input from vertex shader
              "%s vec4 fragColor;\n"          // Color input from vertex shader
              "%s"                            // Fragment color output declaration
              "uniform sampler2D texture0;\n" // Texture sampler
              "uniform vec4 colDiffuse;\n"    // Diffuse color uniform
              "void main()\n"
              "{\n"
              "    vec4 texelColor = %s(texture0, fragTexCoord);\n"
              "    %s = texelColor * colDiffuse * fragColor;\n"
              "}\n",
              config.glslVersion, config.varyingKeyword, config.varyingKeyword, // Fragment inputs
              config.fragColorDeclare,                                          // Output declaration
              config.textureFunc,                                               // Texture sampling function
              config.fragColorOutput                                            // Fragment output target
    );

    // Compile shaders
    //----------------------------------------------------------
    leState.Shader.defaultVShaderId = leCompileShader( defaultVShaderCode, GL_VERTEX_SHADER );
    leState.Shader.defaultFShaderId = leCompileShader( defaultFShaderCode, GL_FRAGMENT_SHADER );
}

// Unload default shader
INLINE void
leUnloadShaderDefault( void )
{
    // TODO: Implement unloading default shaders and textures
}

// Compile shader. Return its id
INLINE unsigned int
leCompileShader( const char * shaderCode, int type )
{
    unsigned int shader = 0;

#    if defined( GRAPHICS_API_OPENGL_33 ) || defined( GRAPHICS_API_OPENGL_ES2 )
    shader = glCreateShader( type );
    glShaderSource( shader, 1, &shaderCode, NULL );

    GLint success = 0;
    glCompileShader( shader );

#    endif

    return shader;
}

/* ------------------ Framebuffer Functions ------------------ */
// Create an empty framebuffer object
INLINE unsigned int
leCreateFramebuffer( void )
{
    unsigned int fboId = 0;

#    if defined( GRAPHICS_API_OPENGL_33 ) || defined( GRAPHICS_API_OPENGL_ES2 )
    glGenFramebuffers( 1, &fboId );         // Create a single framebuffer object
    glBindFramebuffer( GL_FRAMEBUFFER, 0 ); // Unbind any framebuffer
#    endif

    return fboId;
}

// Delete a framebuffer object
INLINE void
leDeleteFramebuffer( unsigned int fb )
{
#    if defined( GRAPHICS_API_OPENGL_33 ) || defined( GRAPHICS_API_OPENGL_ES2 )

    // TODO: automatically delete depth or any attachment

    glDeleteFramebuffers( 1, &fb );

    TRACELOG( LOG_INFO, "Framebuffer [ID %i] released from VRAM", fb );

#    endif
}

// Bind framebuffer (FBO)
INLINE void
leBindFramebuffer( unsigned int target, unsigned int framebuffer )
{
#    if defined( GRAPHICS_API_OPENGL_33 ) || defined( GRAPHICS_API_OPENGL_ES2 )
    glBindFramebuffer( target, framebuffer );
#    endif
}

// Unbind framebuffer
INLINE void
leUnbindFramebuffer( unsigned int target )
{
#    if defined( GRAPHICS_API_OPENGL_33 ) || defined( GRAPHICS_API_OPENGL_ES2 )
    glBindFramebuffer( target, 0 );
#    endif
}

INLINE int
leCheckFramebufferStatus( unsigned int target )
{
    int status = 0;

#    if defined( GRAPHICS_API_OPENGL_33 ) || defined( GRAPHICS_API_OPENGL_ES2 )
    status = ( GL_FRAMEBUFFER_COMPLETE == glCheckFramebufferStatus( target ) ) ? 1 : 0;
#    endif

    return status;
}

/* -------------------- Drawing Functions -------------------- */
//Draw arrays (GL_TRIANGLES, GL_LINES, ...)
INLINE void
leDrawArrays( unsigned int mode, int first, int count )
{
    glDrawArrays( mode, first, count );
}

// Draw vertex array
INLINE void
leDrawVertexArray( int offset, int count )
{
    leDrawArrays( GL_TRIANGLES, offset, count );
}

/* -------------------- Stencil Functions -------------------- */
// Set the stencil test function and reference value
INLINE void
leStencilFunc( int func, int ref, unsigned int mask )
{
    // Controls how stencil test is performed and which fragments pass
    // func: stencil test function (e.g., GL_EQUAL, GL_LESS)
    // ref: reference value to compare against
    // mask: bit mask applied to reference and stored stencil values before comparison
    glStencilFunc( func, ref, mask );
}

// Set the stencil buffer operations
INLINE void
leStencilOp( int sfail, int dpfail, int dppass )
{
    // Specifies what happens to the stencil buffer value under different test outcomes
    // sfail: action when stencil test fails
    // dpfail: action when stencil test passes but depth test fails
    // dppass: action when both stencil and depth tests pass
    glStencilOp( sfail, dpfail, dppass );
}

// Control which bits in the stencil buffer are writable
INLINE void
leStencilMask( unsigned int mask )
{
    // Sets a bit mask that controls which bits in the stencil buffer can be modified
    // mask: bit mask where 1 means writable, 0 means protected
    glStencilMask( mask );
}

#endif // LEGL_IMPLEMENTATION
#endif // !LEGL_H

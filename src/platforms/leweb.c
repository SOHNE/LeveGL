//==============================================================================================================
// INCLUDES
//==============================================================================================================
#include "lecore_context.h"

#include "levegl/leutils.h"
#include "levegl/levegl.h"

#undef LEGL_IMPLEMENTATION
#include "levegl/legl.h"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <emscripten/emscripten.h>

//==============================================================================================================
// TYPES
//==============================================================================================================
typedef struct PlatformContext
{
    GLFWwindow * handle;
} PlatformContext;

//==============================================================================================================
// GLOBALS
//==============================================================================================================
extern CoreContext     core;
static PlatformContext platform = { 0 };

//==============================================================================================================
// MODULE FUNCTIONS DECLARATIONS
//==============================================================================================================
int         InitPlatform();
extern void InitShapes( void );
extern void CleanupShapes( void );
static void FramebufferSizeCallback( GLFWwindow * window, int width, int height );

//==============================================================================================================
// MISSING GETTIME IMPLEMENTATION
//==============================================================================================================
double
GetTime( void )
{
    return glfwGetTime(); // GLFW's built-in timer
}

//==============================================================================================================
// MODULE FUNCTIONS DEFINITIONS
//==============================================================================================================
int
InitPlatform()
{
    TRACELOG( LOG_INFO, "Initializing window: %s (%dx%d)", core.window.title, core.window.screen.width,
              core.window.screen.height );

    if( !glfwInit() )
        {
            TRACELOG( LOG_ERROR, "Failed to initialize GLFW" );
            return -1;
        }

    // Emscripten-specific context configuration
    glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_ES_API );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
    glfwWindowHint( GLFW_RESIZABLE, GLFW_TRUE );

    platform.handle
        = glfwCreateWindow( core.window.screen.width, core.window.screen.height, core.window.title, NULL, NULL );
    if( !platform.handle )
        {
            TRACELOG( LOG_ERROR, "Failed to create GLFW window" );
            glfwTerminate();
            return -1;
        }

    glfwMakeContextCurrent( platform.handle );
    leLoadExtensions( glfwGetProcAddress );
    InitShapes();

    // Configure timing
    core.timing.targetFPS     = 60;
    core.timing.lastFrameTime = GetTime();

    glfwSetFramebufferSizeCallback( platform.handle, FramebufferSizeCallback );

    TRACELOG( LOG_INFO, "Window initialized successfully" );
    return 0;
}

void
ClosePlatform( void )
{
    CleanupShapes();

    if( platform.handle )
        {
            glfwDestroyWindow( platform.handle );
            platform.handle = NULL;
        }

    glfwTerminate();
}

bool
ShouldQuit( void )
{
    return glfwWindowShouldClose( platform.handle );
}

void
WaitTime( double seconds )
{
    if( seconds > 0.0 )
        {
            emscripten_sleep( seconds * 1000 );
        }
}

void
SwapBuffers( void )
{
    glfwSwapBuffers( platform.handle );
}

static void
FramebufferSizeCallback( GLFWwindow * window, int width, int height )
{
    //glViewport( 0, 0, width, height );
    core.window.screen.width  = width;
    core.window.screen.height = height;
    TRACELOG( LOG_INFO, "Window resized to %dx%d", width, height );
}

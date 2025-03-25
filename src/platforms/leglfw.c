//==============================================================================================================
// INCLUDES
//==============================================================================================================
#include "lecore_context.h"

#include "levegl/leutils.h"
#include "levegl/levegl.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

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
int InitPlatform();

extern void InitShapes( void );
extern void CleanupShapes( void );

// GLFW callbacks and window management
static void FramebufferSizeCallback( GLFWwindow * window, int width, int height );

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

    glfwDefaultWindowHints();

    // Set OpenGL version and profile.
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
#ifdef __APPLE__
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE );
#endif

    // Apply window hints based on our flag configuration:
    // Set whether the window should be resizable.
    glfwWindowHint( GLFW_RESIZABLE, FLAG_CHECK( core.window.flags, FLAG_WINDOW_RESIZABLE ) ? GLFW_TRUE : GLFW_FALSE );

    // Configure MSAA (multi-sample anti-aliasing) sample count.
    glfwWindowHint( GLFW_SAMPLES, FLAG_CHECK( core.window.flags, FLAG_MSAA_HINT ) ? 4 : 0 );

    // Create the window.
    platform.handle
        = glfwCreateWindow( core.window.screen.width, core.window.screen.height, core.window.title, NULL, NULL );
    if( !platform.handle )
        {
            TRACELOG( LOG_ERROR, "Failed to create GLFW window" );
            glfwTerminate();
            return -1;
        }

    glfwMakeContextCurrent( platform.handle );

    InitShapes();

    // Set VSync based on our flag configuration.
    glfwSwapInterval( FLAG_CHECK( core.window.flags, FLAG_VSYNC_HINT ) ? 1 : 0 );

    // Configure timing settings.
    core.timing.targetFPS     = 60;
    core.timing.lastFrameTime = GetTime();

    glfwSetFramebufferSizeCallback( platform.handle, FramebufferSizeCallback );

    TRACELOG( LOG_INFO, "Window initialized successfully" );
    return 0;
}

void
ClosePlatform( void )
{
    // First clean up any OpenGL resources
    CleanupShapes();

    // Check if window exists before destroying
    if( NULL != platform.handle )
        {
            glfwDestroyWindow( platform.handle );
            platform.handle = NULL;
        }

    // Terminate GLFW and reset error callback
    glfwSetErrorCallback( NULL );
    glfwTerminate();
}

bool
ShouldQuit( void )
{
    return glfwWindowShouldClose( platform.handle );
}

void
SetWindowTitle( const char * title )
{
    core.window.title = title;
    glfwSetWindowTitle( platform.handle, title );
}

double
GetTime( void )
{
    return glfwGetTime();
}

void
SwapBuffers( void )
{
    glfwSwapBuffers( platform.handle );
}

void
WaitTime( double seconds )
{
    if( 0.0 >= seconds )
        {
            return;
        }

    double start   = GetTime();
    double elapsed = 0.0;

    while( ( elapsed = GetTime() - start ) < seconds )
        {
            const double remaining = seconds - elapsed;
            glfwWaitEventsTimeout( remaining );
        }
}

// Window size getters
int
GetScreenWidth( void )
{
    int width, height;
    glfwGetFramebufferSize( platform.handle, &width, &height );
    return width;
}

int
GetScreenHeight( void )
{
    int width, height;
    glfwGetFramebufferSize( platform.handle, &width, &height );
    return height;
}

void *
GetWindowHandle( void )
{
    return (void *)platform.handle;
}

static void
FramebufferSizeCallback( GLFWwindow * window, int width, int height )
{
    //glViewport( 0, 0, width, height );
    core.window.screen.width  = width;
    core.window.screen.height = height;
    TRACELOG( LOG_INFO, "Window resized to %dx%d", width, height );
}

//==============================================================================================================
// INCLUDES
//==============================================================================================================
#include "lecore_context.h"

#include "levegl/levegl.h"
#include "levegl/leutils.h"

#include <stddef.h>
#include <string.h>

#undef LEGL_IMPLEMENTATION
#include "levegl/legl.h"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
//#include "GLFW/glfw3native.h"

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

extern void PollInputEvents( void );

// GLFW callbacks and window management
static void FramebufferSizeCallback( GLFWwindow * window, int width, int height );
static void KeyCallback( GLFWwindow * window, int key, int scancode, int action, int mods );

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

    leLoadExtensions( glfwGetProcAddress );

    InitShapes();

    // Set VSync based on our flag configuration.
    glfwSwapInterval( FLAG_CHECK( core.window.flags, FLAG_VSYNC_HINT ) ? 1 : 0 );

    // Configure timing settings.
    core.timing.targetFPS     = 60;
    core.timing.lastFrameTime = GetTime();

    glfwSetFramebufferSizeCallback( platform.handle, FramebufferSizeCallback );
    glfwSetKeyCallback( platform.handle, KeyCallback );

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
    return core.window.shouldQuit;
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
    leViewport( 0, 0, width, height );
    core.window.screen.width  = width;
    core.window.screen.height = height;
    TRACELOG( LOG_INFO, "Window resized to %dx%d", width, height );
}

static void
KeyCallback( GLFWwindow * window, int key, int scancode, int action, int mods )
{
    // Filter invalid key codes
    if( UNLIKELY( KEY_NULL > key ) ) return;

    switch( action )
        {
        case GLFW_RELEASE:
            {
                // Clear key state immediately on release
                core.input.keyboard.currKeyState[key] = 0;
                break;
            }
        case GLFW_PRESS:
            {
                // Update state and record press event
                core.input.keyboard.currKeyState[key] = 1;
                ++core.input.keyboard.pressedKeyCount;
                break;
            }
        case GLFW_REPEAT:
            {
                // Track sustained key repeats
                core.input.keyboard.keyRepeats[key] = 1;
                break;
            }
        }

    // Force lock keys to active state when modifiers match
    if( ( KEY_CAPS_LOCK == key && ( mods & GLFW_MOD_CAPS_LOCK ) )
        || ( KEY_NUM_LOCK == key && ( mods & GLFW_MOD_NUM_LOCK ) ) )
        {
            core.input.keyboard.currKeyState[key] = 1;
        }
}

void
PollInputEvents( void )
{
    /* Store previous states */
    for( size_t i = 0; i < KEYBOARD_KEY_COUNT; ++i )
        {
            core.input.keyboard.prevKeyState[i] = core.input.keyboard.currKeyState[i];
            core.input.keyboard.keyRepeats[i]   = 0;
        }

    /* Clear states */
    core.input.keyboard.pressedKeyCount = 0;

    /* Poll events */
    // TODO: Implement an event waiter to correctly store pressed keys when drawing is paused
    glfwPollEvents();

    /* Handle quit event */
    core.window.shouldQuit = glfwWindowShouldClose( platform.handle );
    glfwSetWindowShouldClose( platform.handle, GLFW_FALSE );
}

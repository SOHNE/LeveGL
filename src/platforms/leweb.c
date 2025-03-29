//==============================================================================================================
// INCLUDES
//==============================================================================================================
#include "lecore_context.h"

#include "levegl/levegl.h"
#include "levegl/leutils.h"

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
int         InitPlatform( void );
extern void InitShapes( void );
extern void CleanupShapes( void );
static void FramebufferSizeCallback( GLFWwindow * window, int width, int height );
static void KeyCallback( GLFWwindow * window, int key, int scancode, int action, int mods );

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
InitPlatform( void )
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

    platform.handle = glfwCreateWindow( (int)core.window.screen.width, (int)core.window.screen.height,
                                        core.window.title, NULL, NULL );
    if( !platform.handle )
        {
            TRACELOG( LOG_ERROR, "Failed to create GLFW window" );
            glfwTerminate();
            return -1;
        }

    glfwMakeContextCurrent( platform.handle );
    leLoadExtensions( (void *)glfwGetProcAddress );

    // Configure timing
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
    return false;
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
    UNUSED( window );

    leViewport( 0, 0, width, height );
    core.window.screen.width  = (unsigned int)width;
    core.window.screen.height = (unsigned int)height;
    TRACELOG( LOG_INFO, "Window resized to %dx%d", width, height );
}

static void
KeyCallback( GLFWwindow * window, int key, int scancode, int action, int mods )
{
    UNUSED( window );
    UNUSED( scancode );

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
        default: break;
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

    core.window.shouldQuit = glfwWindowShouldClose( platform.handle );
    glfwSetWindowShouldClose( platform.handle, GLFW_FALSE );
}

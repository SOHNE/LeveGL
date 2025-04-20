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

#include <SDL2/SDL.h>
#include <SDL2/SDL_version.h>

//==============================================================================================================
// TYPES
//==============================================================================================================
typedef struct PlatformContext
{
    SDL_Window *  window;
    SDL_GLContext gl_context;
} PlatformContext;

//==============================================================================================================
// GLOBALS
//==============================================================================================================
extern CoreContext     core;
static PlatformContext platform = { 0 };

//==============================================================================================================
// MODULE FUNCTIONS DECLARATIONS
//==============================================================================================================
int  InitPlatform( void );
void ClosePlatform( void );

int GetScreenWidth( void );
int GetScreenHeight( void );

extern void PollInputEvents( void );

// Key translation helper
static int TranslateSDLKeycode( SDL_Keycode sdl_key );

//==============================================================================================================
// MODULE INTERNAL FUNCTIONS DEFINITIONS
//==============================================================================================================
// Convert SDL Key code into LeveGL Key code
static int
TranslateSDLKeycode( SDL_Keycode sdl_key )
{
    switch( sdl_key )
        {
        case SDLK_ESCAPE:       return KEY_ESCAPE;
        case SDLK_SPACE:        return KEY_SPACE;
        case SDLK_a:            return KEY_A;
        case SDLK_b:            return KEY_B;
        case SDLK_c:            return KEY_C;
        // TODO: Add other keys
        case SDLK_CAPSLOCK:     return KEY_CAPS_LOCK;
        case SDLK_NUMLOCKCLEAR: return KEY_NUM_LOCK;
        default:                return KEY_NULL;
        }
}

//==============================================================================================================
// MODULE FUNCTIONS DEFINITIONS
//==============================================================================================================
int
InitPlatform( void )
{
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
        {
            TRACELOG( LOG_ERROR, "Failed to initialize SDL: %s", SDL_GetError() );
            return -1;
        }

    // Set OpenGL attributes
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
#ifdef __APPLE__
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG );
#endif

    // Configure MSAA
    if( FLAG_CHECK( core.window.flags, FLAG_MSAA_HINT ) )
        {
            SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1 );
            SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 4 );
        }

    // Create window
    Uint32 window_flags = SDL_WINDOW_OPENGL;
    if( FLAG_CHECK( core.window.flags, FLAG_WINDOW_RESIZABLE ) )
        {
            window_flags |= SDL_WINDOW_RESIZABLE;
        }

    platform.window = SDL_CreateWindow( core.window.title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        core.window.screen.width, core.window.screen.height, window_flags );

    if( !platform.window )
        {
            TRACELOG( LOG_ERROR, "Failed to create SDL window: %s", SDL_GetError() );
            SDL_Quit();
            return -1;
        }

    // Create OpenGL context
    platform.gl_context = SDL_GL_CreateContext( platform.window );
    if( !platform.gl_context )
        {
            TRACELOG( LOG_ERROR, "Failed to create OpenGL context: %s", SDL_GetError() );
            SDL_DestroyWindow( platform.window );
            SDL_Quit();
            return -1;
        }

    leLoadExtensions( (void *)SDL_GL_GetProcAddress );

    // Set VSync
    SDL_GL_SetSwapInterval( FLAG_CHECK( core.window.flags, FLAG_VSYNC_HINT ) ? 1 : 0 );

    // Initialize timing
    core.timing.targetFPS     = 60;
    core.timing.lastFrameTime = GetTime();

    {
        SDL_version linked_version;
        SDL_GetVersion( &linked_version );
        TRACELOG( LOG_INFO, "SDL Initialized: %d.%d.%d", linked_version.major, linked_version.minor,
                  linked_version.patch );
    }

    return 0;
}

void
ClosePlatform( void )
{
    if( platform.gl_context )
        {
            SDL_GL_DeleteContext( platform.gl_context );
        }
    if( platform.window )
        {
            SDL_DestroyWindow( platform.window );
        }
    SDL_Quit();
}

bool
ShouldQuit( void )
{
    return core.window.shouldQuit;
}

void
SetWindowTitle( const char * title )
{
    SDL_SetWindowTitle( platform.window, title );
}

double
GetTime( void )
{
    return (double)SDL_GetTicks() / 1000.0;
}

void
SwapBuffers( void )
{
    SDL_GL_SwapWindow( platform.window );
}

void
WaitTime( double seconds )
{
    if( seconds <= 0.0 ) return;

    double start     = GetTime();
    double remaining = seconds;
    while( remaining > 0.0 )
        {
            SDL_WaitEventTimeout( NULL, (int)( remaining * 1000 ) );
            remaining = seconds - ( GetTime() - start );
        }
}

int
GetScreenWidth( void )
{
    int width, height;
    SDL_GL_GetDrawableSize( platform.window, &width, &height );
    return width;
}

int
GetScreenHeight( void )
{
    int width, height;
    SDL_GL_GetDrawableSize( platform.window, &width, &height );
    return height;
}

void *
GetWindowHandle( void )
{
    return platform.window;
}

void
PollInputEvents( void )
{
    // Store previous states
    for( size_t i = 0; i < KEYBOARD_KEY_COUNT; ++i )
        {
            core.input.keyboard.prevKeyState[i] = core.input.keyboard.currKeyState[i];
            core.input.keyboard.keyRepeats[i]   = 0;
        }

    // Clear pressed key count
    core.input.keyboard.pressedKeyCount = 0;

    SDL_Event event;
    while( SDL_PollEvent( &event ) )
        {
            switch( event.type )
                {
                case SDL_QUIT: core.window.shouldQuit = true; break;

                case SDL_WINDOWEVENT:
                    if( event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED )
                        {
                            int width  = event.window.data1;
                            int height = event.window.data2;
                            leViewport( 0, 0, width, height );
                            core.window.screen.width  = width;
                            core.window.screen.height = height;
                            TRACELOG( LOG_INFO, "Window resized to %dx%d", width, height );
                        }
                    break;

                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    {
                        SDL_Keycode sdl_key = event.key.keysym.sym;
                        int         key     = TranslateSDLKeycode( sdl_key );

                        if( key < KEY_NULL ) continue;

                        if( event.type == SDL_KEYDOWN )
                            {
                                if( event.key.repeat )
                                    {
                                        core.input.keyboard.keyRepeats[key] = 1;
                                    }
                                else
                                    {
                                        core.input.keyboard.currKeyState[key] = 1;
                                        core.input.keyboard.pressedKeyCount++;
                                    }
                            }
                        else
                            {
                                core.input.keyboard.currKeyState[key] = 0;
                            }

                        // Handle lock keys
                        SDL_Keymod mod = SDL_GetModState();
                        if( ( key == KEY_CAPS_LOCK && ( mod & KMOD_CAPS ) )
                            || ( key == KEY_NUM_LOCK && ( mod & KMOD_NUM ) ) )
                            {
                                core.input.keyboard.currKeyState[key] = 1;
                            }
                        break;
                    }
                }
        }
}

/**************************** LECORE_CONTEXT *****************************
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

#ifndef LEVEGL_CORE_CONTEXT_H
#define LEVEGL_CORE_CONTEXT_H

#ifndef KEYBOARD_KEY_COUNT
#    define KEYBOARD_KEY_COUNT 512 // The maximum number of supported keyboard keys
#endif

typedef struct Coordinate
{
    int x;
    int y;
} Coordinate;

typedef struct Dimension
{
    unsigned int width;
    unsigned int height;
} Dimension;

/// @brief Main context structure holding core application state and configuration
typedef struct CoreContext
{
    /// Window configuration group containing title and dimensions
    struct window
    {
        const char * title;      /// Window title string (memory managed externally)
        unsigned int flags;      /// Configuration bits
        int          shouldQuit; /// Is main window closing?

        Dimension screen;        /// Window Size

    } window;

    /// Timing configuration group for frame rate control
    struct timing
    {
        double       lastFrameTime; /// Timestamp of last frame in seconds
        double       targetFPS;     /// Target FPS for the application
        unsigned int frameCounter;

    } timing;

    struct input
    {
        struct keyboard
        {
            char currKeyState[KEYBOARD_KEY_COUNT]; // Current frame's key states
            char prevKeyState[KEYBOARD_KEY_COUNT]; // Previous frame's key states
            char keyRepeats[KEYBOARD_KEY_COUNT];   // Key repeat states for current frame

            int pressedKeyCount;                   // Number of pressed keys

        } keyboard;

    } input;

} CoreContext;

#endif // !LEVEGL_CORE_CONTEXT_H

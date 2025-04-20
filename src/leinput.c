/******************************* LEINPUT **********************************
 * leinput: Input Handling
 *
 *                              INTERFACES
 * ------------------------------------------------------------------------
 * DESKTOP:
 *     - Keyboard (US layout)
 *
 *                                NOTES
 * ------------------------------------------------------------------------
 * TODO:
 *   - Design and impement input handle for Gamepad and touch.
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

#include "levegl/levegl.h"

#include "lecore_context.h"

//----------------------------------------------------------------------------------------------------------------------
// Globals
//----------------------------------------------------------------------------------------------------------------------
extern CoreContext core;

//----------------------------------------------------------------------------------------------------------------------
// Keyboard
//----------------------------------------------------------------------------------------------------------------------
// Check if any key is been pressed
INLINE bool
IsAnyKeyPressed( void )
{
    return 0 < core.input.keyboard.pressedKeyCount;
}

// Check if the given key is been pressed
INLINE bool
IsKeyPressed( int key )
{
    if( UNLIKELY( KEY_NULL >= key || KEYBOARD_KEY_COUNT <= key ) ) return false;

    return ( !core.input.keyboard.prevKeyState[key] && core.input.keyboard.currKeyState[key] );
}

// Check if the given key is repeated across frames
INLINE bool
IsKeyPressedRepeated( int key )
{
    if( UNLIKELY( KEY_NULL >= key || KEYBOARD_KEY_COUNT <= key ) ) return false;

    return (bool)core.input.keyboard.keyRepeats[key];
}

// Check if the given key is being pressed
INLINE bool
IsKeyDown( int key )
{
    if( UNLIKELY( KEY_NULL >= key || KEYBOARD_KEY_COUNT <= key ) ) return false;

    return (bool)core.input.keyboard.currKeyState[key];
}

// Check if the given key has been released once
INLINE bool
IsKeyReleased( int key )
{
    if( UNLIKELY( KEY_NULL >= key || KEYBOARD_KEY_COUNT <= key ) ) return false;

    return ( core.input.keyboard.prevKeyState[key] && !core.input.keyboard.currKeyState[key] );
}

// Check if the key is NOT being pressed
INLINE bool
IsKeyUp( int key )
{
    if( UNLIKELY( KEY_NULL >= key || KEYBOARD_KEY_COUNT <= key ) ) return false;

    return false == core.input.keyboard.currKeyState[key];
}

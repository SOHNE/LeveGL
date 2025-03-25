/******************************* LEUTILS *********************************
 * leutils: Just general utilities
 *
 *                                NOTES
 * ------------------------------------------------------------------------
 * INFO:
 * - DEFINES:
 *   - LOG_SUPPORT: Enable Logging system
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

#ifndef LEUTILS_H
#define LEUTILS_H

//----------------------------------------------------------------------------------------------------------------------
// Logging Macros
//----------------------------------------------------------------------------------------------------------------------
#if defined( LOG_SUPPORT )
#    define TRACELOG( level, ... ) TraceLog( level, __VA_ARGS__ )

#    if defined( LOG_SUPPORT_DEBUG )
#        define TRACELOGD( ... ) TraceLog( LOG_DEBUG, __VA_ARGS__ )
#    else
#        define TRACELOGD( ... ) ( (void)0 )
#    endif
#else  // !LOG_SUPPORT
#    define TRACELOG( level, ... ) ( (void)( 0 ) )
#    define TRACELOGD( ... )       ( (void)( 0 ) )
#endif // LOG_SUPPORT

#endif // !LEUTILS_H

/******************************* LEUTILS *********************************
 * leutils: Just general utilities
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

#include "levegl/leutils.h"

#include "levegl/levegl.h"

#include <stdarg.h> /* va_start, va_end, va_list */
#include <stdio.h>  /* fprintf, vfprintf, stdout */
#include <stdlib.h> /* abort */

//----------------------------------------------------------------------------------------------------------------------
// Module Declaration
//----------------------------------------------------------------------------------------------------------------------
void SetLogLevel( int logType );                       // Set a minimum log level
void TraceLog( int logLevel, const char * text, ... ); // Display a log message

//----------------------------------------------------------------------------------------------------------------------
// Variables Definition
//----------------------------------------------------------------------------------------------------------------------
static int              logLevel = LOG_INFO; // Current log level
static TraceLogCallback traceLog = NULL;     // Custom trace log function

//----------------------------------------------------------------------------------------------------------------------
// Callbacks
//----------------------------------------------------------------------------------------------------------------------
// Set a custom trace log callback
void
SetTraceLogCallback( TraceLogCallback callback )
{
    traceLog = callback;
}

//----------------------------------------------------------------------------------------------------------------------
// Module Functions Definition: Utilities
//----------------------------------------------------------------------------------------------------------------------
// Set a minimum log level
void
SetLogLevel( int logType )
{
    logLevel = logType;
}

// Display the given message
void
TraceLog( int logType, const char * text, ... )
{
    // Skip logging
    if( logLevel > logType ) return;

    va_list args;
    va_start( args, text );

    // Use custom callback if available
    if( NULL != traceLog )
        {
            traceLog( logType, text, args );
            va_end( args );
            return;
        }

    // Determine log level string
    char * levelString;
    switch( logType )
        {
        case LOG_TRACE:   levelString = "TRACE"; break;
        case LOG_DEBUG:   levelString = "DEBUG"; break;
        case LOG_INFO:    levelString = "INFO"; break;
        case LOG_WARNING: levelString = "WARNING"; break;
        case LOG_ERROR:   levelString = "ERROR"; break;
        case LOG_FATAL:   levelString = "FATAL"; break;
        default:          levelString = "UNKNOWN"; break;
        }

    // Print log message
    fprintf( stdout, "[%s] ", levelString );
    vfprintf( stdout, text, args );
    fprintf( stdout, "\n" );

    va_end( args );

    // Handle fatal errors
    if( UNLIKELY( logType == LOG_FATAL ) ) abort();
}

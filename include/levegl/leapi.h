#ifndef LEVEGL_LEAPI_H
#define LEVEGL_LEAPI_H

#include "levegl/leexport.h"

#if defined( LEVEGL_STATIC_DEFINE )
#    define LEAPI
#else
#    if defined( _WIN32 ) || defined( __CYGWIN__ )
#        ifdef LeveGL_EXPORTS
#            ifdef __GNUC__
#                define LEAPI __attribute__( ( dllexport ) )
#            else
#                define LEAPI __declspec( dllexport )
#            endif
#        else
#            ifdef __GNUC__
#                define LEAPI __attribute__( ( dllimport ) )
#            else
#                define LEAPI __declspec( dllimport )
#            endif
#        endif
#    else
#        if __GNUC__ >= 4
#            define LEAPI __attribute__( ( visibility( "default" ) ) )
#        else
#            define LEAPI
#        endif
#    endif
#endif

#endif // LEVEGL_LEAPI_H

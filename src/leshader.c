#include "levegl/leutils.h"
#include "levegl/levegl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static Shader currentShader = { 0 };

static char *
LoadFileText( const char * fileName )
{
    FILE * file = fopen( fileName, "rt" );
    if( !file )
        {
            TRACELOG( LOG_ERROR, "Failed to open shader file: %s", fileName );
            return NULL;
        }

    fseek( file, 0, SEEK_END );
    int size = ftell( file );
    fseek( file, 0, SEEK_SET );

    char * text  = (char *)malloc( ( size + 1 ) * sizeof( char ) );
    int    count = fread( text, sizeof( char ), size, file );
    text[count]  = '\0';

    fclose( file );
    return text;
}

static bool
CompileShader( int id, const char * shaderCode, int type )
{
    return false;
}

Shader
LoadShader( const char * vsFileName, const char * fsFileName )
{
    return (Shader) { 0 };
}

Shader
LoadShaderFromMemory( const char * vsCode, const char * fsCode )
{
    return (Shader) { 0 };
}

void
UnloadShader( Shader shader )
{
}

void
BeginShaderMode( Shader shader )
{
}

void
EndShaderMode( void )
{
}

int
GetShaderLocation( Shader shader, const char * uniformName )
{
    return 0;
}

void
SetShaderValue( Shader shader, int locIndex, const void * value, int uniformType )
{
    SetShaderValueV( shader, locIndex, value, uniformType, 1 );
}

void
SetShaderValueV( Shader shader, int locIndex, const void * value, int uniformType, int count )
{
}

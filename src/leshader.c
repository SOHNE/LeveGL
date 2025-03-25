#include "levegl/levegl.h"

#include <GL/glew.h>
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
            TRACE_ERROR( "Failed to open shader file: %s", fileName );
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
CompileShader( GLuint id, const char * shaderCode, GLenum type )
{
    GLint success = 0;
    char  infoLog[512];

    glShaderSource( id, 1, &shaderCode, NULL );
    glCompileShader( id );
    glGetShaderiv( id, GL_COMPILE_STATUS, &success );

    if( !success )
        {
            glGetShaderInfoLog( id, 512, NULL, infoLog );
            TRACE_ERROR( "Shader compilation error: %s", infoLog );
            return false;
        }
    return true;
}

Shader
LoadShader( const char * vsFileName, const char * fsFileName )
{
    TRACE_INFO( "Loading shader from files: VS=%s, FS=%s", vsFileName, fsFileName );

    char * vsCode = LoadFileText( vsFileName );
    char * fsCode = LoadFileText( fsFileName );

    Shader result = LoadShaderFromMemory( vsCode, fsCode );

    free( vsCode );
    free( fsCode );

    return result;
}

Shader
LoadShaderFromMemory( const char * vsCode, const char * fsCode )
{
    Shader shader         = { 0 };

    GLuint vertexShader   = glCreateShader( GL_VERTEX_SHADER );
    GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

    if( !CompileShader( vertexShader, vsCode, GL_VERTEX_SHADER )
        || !CompileShader( fragmentShader, fsCode, GL_FRAGMENT_SHADER ) )
        {
            TRACE_ERROR( "Failed to compile shaders" );
            return shader;
        }

    shader.id = glCreateProgram();
    glAttachShader( shader.id, vertexShader );
    glAttachShader( shader.id, fragmentShader );
    glLinkProgram( shader.id );

    GLint success;
    glGetProgramiv( shader.id, GL_LINK_STATUS, &success );
    if( !success )
        {
            char infoLog[512];
            glGetProgramInfoLog( shader.id, 512, NULL, infoLog );
            TRACE_ERROR( "Shader linking error: %s", infoLog );
            shader.id = 0;
        }

    glDeleteShader( vertexShader );
    glDeleteShader( fragmentShader );

    shader.active = false;

    TRACE_INFO( "Shader loaded successfully with ID: %u", shader.id );
    return shader;
}

void
UnloadShader( Shader shader )
{
    if( shader.id != 0 )
        {
            TRACE_INFO( "Unloading shader ID: %u", shader.id );
            glDeleteProgram( shader.id );
            if( shader.locations )
                {
                    free( shader.locations );
                }
        }
}

void
BeginShaderMode( Shader shader )
{
    if( shader.id != 0 && !shader.active )
        {
            currentShader = shader;
            glUseProgram( shader.id );
            shader.active = true;
        }
}

void
EndShaderMode( void )
{
    if( currentShader.id != 0 && currentShader.active )
        {
            glUseProgram( 0 );
            currentShader.active = false;
        }
}

int
GetShaderLocation( Shader shader, const char * uniformName )
{
    int location = glGetUniformLocation( shader.id, uniformName );
    if( location == -1 )
        {
            TRACE_WARNING( "Uniform '%s' not found in shader ID: %u", uniformName, shader.id );
        }
    return location;
}

void
SetShaderValue( Shader shader, int locIndex, const void * value, int uniformType )
{
    SetShaderValueV( shader, locIndex, value, uniformType, 1 );
}

void
SetShaderValueV( Shader shader, int locIndex, const void * value, int uniformType, int count )
{
    if( shader.id == 0 || locIndex == -1 )
        {
            return;
        }

    glUseProgram( shader.id );

    switch( uniformType )
        {
        case GL_FLOAT:      glUniform1fv( locIndex, count, (float *)value ); break;
        case GL_INT:        glUniform1iv( locIndex, count, (int *)value ); break;
        case GL_FLOAT_VEC2: glUniform2fv( locIndex, count, (float *)value ); break;
        case GL_FLOAT_VEC3: glUniform3fv( locIndex, count, (float *)value ); break;
        case GL_FLOAT_VEC4: glUniform4fv( locIndex, count, (float *)value ); break;
        case GL_FLOAT_MAT4: glUniformMatrix4fv( locIndex, count, GL_FALSE, (float *)value ); break;
        default:            TRACE_WARNING( "Uniform type not supported" ); break;
        }

    if( shader.id != currentShader.id )
        {
            glUseProgram( currentShader.id );
        }
}

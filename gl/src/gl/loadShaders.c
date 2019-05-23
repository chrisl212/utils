#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>

static char *_readFile(const char *fpath) {
    size_t len;
    char *contents;
    FILE *f = fopen(fpath, "r");
    if (!f) {
        fprintf(stderr, "Could not open file at path: %s\n", fpath);
        return NULL;
    }
    
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);
    contents = malloc(sizeof(*contents) * (len+1));
    fread(contents, sizeof(*contents), len, f);
    contents[len] = '\0';

    fclose(f);
    return contents;
}

static GLuint _compileShader(const char *fpath, GLint type) {
    GLuint ID;
    GLint res, logLen;
    char *source, *log;

    source = _readFile(fpath);
    if (!source) {
        return 0;
    }

    printf("Compiling %s shader: %s\n", (type == GL_VERTEX_SHADER) ? "vertex" : "fragment", fpath);
    ID = glCreateShader(type);
    glShaderSource(ID, 1, (const char * const *)&source, NULL);
    glCompileShader(ID);

    glGetShaderiv(ID, GL_COMPILE_STATUS, &res);
    glGetShaderiv(ID, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0) {
        log = malloc(sizeof(*log) * (logLen+1));
        glGetShaderInfoLog(ID, logLen, NULL, log);
        printf("%s\n", log);
        free(log);
    }

    free(source);
    return ID;
}

static GLuint _linkProgram(GLuint vertexShaderID, GLuint fragmentShaderID) {
    GLuint programID;
    GLint res, logLen;
    char *log;

    printf("Linking shaders\n");
    programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    glGetProgramiv(programID, GL_LINK_STATUS, &res);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLen);
    if (logLen > 0) {
        log = malloc(sizeof(*log) * (logLen+1));
        glGetProgramInfoLog(programID, logLen, NULL, log);
        printf("%s\n", log);
        free(log);
    }

    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);

    return programID;
}

GLuint loadShaders(const char *vertexFilePath, const char *fragmentFilePath) {
    GLuint vertexShaderID, fragmentShaderID, programID;
    
    //compile and link shaders
    vertexShaderID = _compileShader(vertexFilePath, GL_VERTEX_SHADER);
    fragmentShaderID = _compileShader(fragmentFilePath, GL_FRAGMENT_SHADER);
    programID = _linkProgram(vertexShaderID, fragmentShaderID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return programID;
}

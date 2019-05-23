#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "mat/src/mat/mat.h"
#include "gl/loadShaders.h"

static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f,  0.0f,
     1.0f, -1.0f,  0.0f,
     0.0f,  1.0f,  0.0f
};

static GLfloat *_contig(mat_t matrix) {
    uint64_t i, j;
    GLfloat *vals = malloc(sizeof(*vals) * matrix.rows * matrix.cols);
    for (i = 0; i < matrix.rows; i++) {
        for (j = 0; j < matrix.cols; j++) {
            vals[i*matrix.cols+j] = (GLfloat)matrix.vals[i][j];
        }
    }
    return vals;
}

int main(int argc, char **argv) {
    GLFWwindow *window;
    GLuint VertexArrayID, vertexBuffer, programID;
    GLint matrixID;
    vec_t angles;
    mat_t tmp1, tmp2, MVP;
    GLfloat *vals;

    angles = vec_create(3, 0.0, 0.0, 1.3);
    tmp1 = mat_rotation(angles);
    vec_free(angles);

    angles = vec_create(3, -0.25, -0.25, 0.0);
    tmp2 = mat_translation(angles);
    vec_free(angles);

    MVP = mat_mult(2, tmp1, tmp2);
    mat_free(tmp1);
    mat_free(tmp2);

    if (!glfwInit()) {
        fprintf(stderr, "Error initializing GLFW\n");
        return EXIT_FAILURE;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1024, 768, "Window", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Error initializing GLEW\n");
        return EXIT_FAILURE;
    }

    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    programID = loadShaders("src/shaders/vertexShader.glsl", "src/shaders/fragmentShader.glsl");

    matrixID = glGetUniformLocation(programID, "MVP");
    vals = _contig(MVP);
    mat_print(MVP);
    glUseProgram(programID);
    glUniformMatrix4fv(matrixID, 1, GL_TRUE, (const GLfloat *)vals);
    free(vals);

    do {
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(programID);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));

    mat_free(MVP);
    return EXIT_SUCCESS;
}

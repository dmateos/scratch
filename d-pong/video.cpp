#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include "config.h"
#include "util.h"

using namespace std;

static struct {
	GLuint vertex_buffer, element_buffer;

	GLuint vertex_shader, fragment_shader, program;

	struct {
		GLint position;
	} attributes;

	struct {
		GLint timer;
	} uniforms;

	GLfloat timer;
	int count;

} g_resources;

static const GLfloat g_vertex_buffer_data[] = { 
    -1.0f, -1.0f, 0.0f, 1.0f,
     1.0f, -1.0f, 0.0f, 1.0f,
    -1.0f,  1.0f, 0.0f, 1.0f,
     1.0f,  1.0f, 0.0f, 1.0f
};

static const GLushort g_element_buffer_data[] = { 0, 1, 2, 3 };

void video_error(int error, const char *message) {

}

void update_timer(void) {
    int milliseconds = g_resources.count += 10;
    g_resources.timer = (float)milliseconds * 0.001f;
}

GLFWwindow *video_init() {
	GLFWwindow *window;

	g_resources.count = 0;
	glfwSetErrorCallback(video_error);

	if(!glfwInit()) {
		return NULL;
	}

	if(!(window = glfwCreateWindow(GL_XRES, GL_YRES, "Test", NULL, NULL))) {
		glfwTerminate();
		return NULL;
	}


	glfwMakeContextCurrent(window);
	return window;
}

void video_render(GLFWwindow *window) {
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(g_resources.program);

	glUniform1f(g_resources.uniforms.timer, g_resources.timer);

	glBindBuffer(GL_ARRAY_BUFFER, g_resources.vertex_buffer);
	glVertexAttribPointer(
		g_resources.attributes.position,
		4,
		GL_FLOAT,
		GL_FALSE,
		sizeof(GLfloat)*4,
		(void*)0
	);

	glEnableVertexAttribArray(g_resources.attributes.position);
	glPolygonMode(GL_FRONT, GL_LINE);
	glDrawElements(
		GL_TRIANGLE_STRIP,
		4,
		GL_UNSIGNED_SHORT,
		(void*)0
	);

	glDisableVertexAttribArray(g_resources.attributes.position);
	glfwSwapBuffers(window);
}

static int make_buffer(GLenum target, const void *data, GLsizei bsize) {
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(target, buffer);
	glBufferData(target, bsize, data, GL_STATIC_DRAW);
	return buffer;
}

static GLuint make_shader(GLenum type, const char *filename) {
	GLint length;
	GLchar *source = (GLchar*)file_contents(filename, &length);
	GLuint shader;
	GLint shader_ok;

	if(!source) {
		return 0;
	}

	shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar**)&source, &length);
	free(source);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
	if(!shader_ok) {
		printf("Failed to compile %s:\n", filename);
		glDeleteShader(shader);
		return 0;
	}
	printf("made shader from %s\n", filename);
	return shader;
}

static GLuint make_program(GLuint vertex_shader, GLuint frag_shader) {
	GLint program_ok;

	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, frag_shader);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
	if(!program_ok) {
		printf("failed to link shaders\n");
//		show_info_log(program, glGetProgramiv, glGetProgramInfoLog);
		glDeleteProgram(program);
		return 0;
	}
	return program;
}

int make_resources() {
	g_resources.vertex_buffer = make_buffer(
		GL_ARRAY_BUFFER,
		g_vertex_buffer_data,
		sizeof(g_vertex_buffer_data)
	);

	g_resources.element_buffer = make_buffer(
		GL_ELEMENT_ARRAY_BUFFER,
		g_element_buffer_data,
		sizeof(g_element_buffer_data)
	);

	g_resources.vertex_shader = make_shader(
		GL_VERTEX_SHADER,
		"shaders/shader.v.glsl"
	);

	g_resources.fragment_shader = make_shader(
		GL_FRAGMENT_SHADER,
		"shaders/shader.f.glsl"
	);

	g_resources.program = make_program(g_resources.vertex_shader, g_resources.fragment_shader);
	if(g_resources.program == 0) {
		return 0;
	}

	g_resources.attributes.position = glGetAttribLocation(g_resources.program, "position");
	g_resources.uniforms.timer = glGetUniformLocation(g_resources.program, "timer");
	return 1;
}



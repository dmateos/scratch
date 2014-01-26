#include "main.h"

using namespace std;

GLuint make_shader(const char *path, GLenum stype) {
	int length;
	GLchar *data = (GLchar*)file_contents(path, &length);
	GLint shader_ok;
	GLuint shader = glCreateShader(stype);

	glShaderSource(shader, 1, (const GLchar**)&data, &length);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &shader_ok);
	if(!shader_ok) {
		printf("shader %s did not compile\n", path);
		glDeleteShader(shader);
		return 1;
	}

	free(data);
	return shader;
}

GLint make_program(GLint vshader, GLint fshader) {
	GLint shader_program = glCreateProgram();
	glAttachShader(shader_program, vshader);
	glAttachShader(shader_program, fshader);
	glLinkProgram(shader_program);

	GLint program_ok;
	glGetProgramiv(shader_program, GL_LINK_STATUS, &program_ok);
	if(!program_ok) {
		printf("program did not compile\n");
		return 1;
	}

	return shader_program;
}

GLint make_buffer(void *data, int size, GLenum type) {
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, type);
	return vbo;
}

int main(int argc, char **argv) {
	if(!glfwInit()) {
		fprintf(stderr, "error: could not start glfw3\n");
		return 1;
	}

	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(640, 480, "Hello world", NULL, NULL);

	if(!window) {
		fprintf(stderr, "erorr: could not open window with glfw3\n");
		return 1;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	const GLubyte* renderer = glGetString (GL_RENDERER);
	const GLubyte* version = glGetString (GL_VERSION);
	printf("Renderer: %s\n", renderer);
	printf ("OpenGL version supported %s\n", version);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	GLfloat points[] = {
		0.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f
	};

	GLfloat colours[] = {
	  1.0f, 0.0f,  0.0f,
	  0.0f, 1.0f,  0.0f,
	  0.0f, 0.0f,  1.0f
	};

	GLuint points_vbo = make_buffer(points, 9*sizeof(GLfloat), GL_STATIC_DRAW);
	GLuint colors_vbo = make_buffer(colours, 9*sizeof(GLfloat), GL_STATIC_DRAW);

	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	GLint vshader = make_shader("shaders/vshader.glsx", GL_VERTEX_SHADER);
	GLint fshader = make_shader("shaders/fshader.glsx", GL_FRAGMENT_SHADER);
	GLint shader_program = make_program(vshader, fshader);

	while(!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader_program);
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

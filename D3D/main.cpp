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

float x,y,z;
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch(key) {
		case GLFW_KEY_DOWN:
		case GLFW_KEY_S:
			z -= 0.1;
			break;
		case GLFW_KEY_UP:
		case GLFW_KEY_W:
			z += 0.1;
			break;
		case GLFW_KEY_LEFT:
		case GLFW_KEY_A:
			x -= 0.1;
			break;
		case GLFW_KEY_RIGHT:
		case GLFW_KEY_D:
			x += 0.1;
			break;
	}
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

	GLFWwindow *window = glfwCreateWindow(1024, 768, "Hello world", NULL, NULL);
	glfwSetKeyCallback(window, key_callback);


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

	GLint vshader = make_shader("shaders/vshader.glsx", GL_VERTEX_SHADER);
	GLint fshader = make_shader("shaders/fshader.glsx", GL_FRAGMENT_SHADER);
	GLint shader_program = make_program(vshader, fshader);

	D3DWorldObject obj1(argv[1], 0.0f, 0.0f, 10.0f);
	D3DWorldObject obj2("meshes/torus.dae", 0.0f, 0.0f, 5.0f);
	D3DWorldObject obj3("meshes/torus.dae", -3.0f, 0.0f, 5.0f);
	D3DWorldObject obj4("meshes/torus.dae", 3.0f, 0.0f, 5.0f);

	glm::mat4 projection = glm::mat4(
		glm::vec4(3.0/4.0f, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 1.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.5f, 0.5f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);

	while(!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		obj1.update_coord_x(x);
		obj1.update_coord_y(y);
		obj1.update_coord_z(z);

		obj1.draw(shader_program, projection);
		obj2.draw(shader_program, projection);
		obj3.draw(shader_program, projection);
		obj4.draw(shader_program, projection);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

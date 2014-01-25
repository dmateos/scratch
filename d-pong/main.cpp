#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "video.h"
#include "paddle.h"
#include <stdio.h>

using namespace std;

GLFWwindow *window;

static void key_callback(GLFWwindow *window, int key, int scode, int action, int mods) {
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
		//exit(0);
	}
}

int main(void) {
//	Paddle paddle1, paddle2;
	if(!(window = video_init())) {
		exit(1);
	}

	if(!make_resources()) {
		printf("failed to load resources\n");
		return 1;
	}

    glfwSetKeyCallback(window, key_callback);

	while(!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		float ratio;
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;

		update_timer();
		video_render(window);

		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
}

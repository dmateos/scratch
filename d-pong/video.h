#ifndef _VIDEO_H_
#define _VIDEO_H_

//#include <GL/glew.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

void video_error(int error, const char *message);
GLFWwindow *video_init();
void video_render(GLFWwindow *window);
int make_resources();
void update_timer();

#endif

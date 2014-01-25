#include "paddle.h"
#include <iostream>

Paddle::Paddle() {
	std::cout << "paddle init" << std::endl;
}

Paddle::~Paddle() {
	std::cout << "paddle destruct" << std::endl;
}

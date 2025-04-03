#define WEBGPU_CPP_IMPLEMENTATION
#include <webgpu-raii.hpp>

#include <vector>
#include <iostream>
#include <exception>

// NB: This is a very minimal test file, in which we add test whenever a new
// issue is reported to make sure there is no regression in future updates.

// Issue #1
void testMovingNullObjects() throw() {
	std::vector<wgpu::raii::Texture> textures;
	textures.push_back({});

	wgpu::raii::Texture a;
	wgpu::raii::Texture b;
	b = a;
}

int main(int, char**) {

	try {
		testMovingNullObjects();
	} catch (std::exception& e) {
		std::cout << "Error! " << e.what() << std::endl;
		return 1;
	} catch (...) {
		std::cout << "Unknown Error!" << std::endl;
		return 1;
	}

	return 0;
}

#pragma once

#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"


std::vector<float> getSphereCords(int resolution) {
	std::vector<float> result;

	for (int i = 0; i < resolution; i++) {
		for (int j = 0; j < resolution; j++) {
			float u = (float)i / (resolution - 1);
			float v = (float)j / (resolution - 1);
			float theta = u * 2.0f * 3.14159f;
			float phi = v * 3.14159f;

			float x = cos(theta) * sin(phi);
			float y = sin(theta) * sin(phi);
			float z = cos(phi);

			result.push_back(x);
			result.push_back(y);
			result.push_back(z);
		}
	}
	return result;
}

std::vector<unsigned int> getSphereIndices(int resolution) {
	std::vector<unsigned int> result;
	for (int i = 0; i < resolution - 1; i++) {
		for (int j = 0; j < resolution - 1; j++) {
			// First triangle
			result.push_back(i * resolution + j);
			result.push_back((i + 1) * resolution + j);
			result.push_back(i * resolution + j + 1);

			// Second triangle
			result.push_back(i * resolution + j + 1);
			result.push_back((i + 1) * resolution + j);
			result.push_back((i + 1) * resolution + j + 1);
		}
	}
	return result;
}
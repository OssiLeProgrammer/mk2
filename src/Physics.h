#pragma once

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#endif
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp" 
#include "Model.h"
#include "Geometry.h"


float std_Gravity_Pull = 5.0f;

class Planet {
public:
	Model sphere;
	float mass = 1.0f;
	float radius = 1.0f;
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	Planet(float mass, float radius, glm::vec3 position, glm::vec3 velocity) : mass(mass), radius(radius), position(position), velocity(velocity) {
		std::vector <float> vertices = getSphereCords(64);
		std::vector <unsigned int> indices = getSphereIndices(64);
		sphere.appendBuffer(vertices.data(), vertices.size(), indices.data(), indices.size());
		sphere.translate(position);
		sphere.scale(radius * glm::vec3(1.0f, 1.0f, 1.0f));
	}
	void resetModel(float radius, glm::vec3 position, glm::vec3 velocity) {
		sphere.resetModel();
		sphere.translate(position);
		sphere.scale(radius * glm::vec3(1.0f, 1.0f, 1.0f));
		velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	}
	void update(float dt, Planet& planet2) {
		float distance = glm::distance(position, planet2.position);

		// Prevent division by zero
		if (distance < 0.0001f) {
			distance = 0.0001f;
		}

		float force = std_Gravity_Pull * (mass * planet2.mass) / (distance * distance);
		glm::vec3 direction = glm::normalize(planet2.position - position);

		// Update velocity based on the force
		velocity += direction * force * dt;

		// Update position based on velocity
		position += velocity * dt;

		// Update sphere model for rendering
		sphere.resetModel();
		sphere.scale(radius * glm::vec3(1.0f, 1.0f, 1.0f));
		sphere.translate(position);
	}
	glm::mat4 getModel() {
		return sphere.getModel();
	}
	void draw() {
		sphere.draw();
	}
};



class World {
public:
	std::vector<Planet*>& planets;
	World(std::vector<Planet*>& planets) : planets(planets) {}
	void update(float dt) {
		std::vector<glm::vec3> forces(planets.size(), glm::vec3(0.0f)); // Store computed forces

		// 1st pass: Compute forces
		for (int i = 0; i < planets.size(); i++) {
			for (int j = i + 1; j < planets.size(); j++) {
				float distance = glm::distance(planets[i]->position, planets[j]->position);

				if (distance < 0.0001f) {
					distance = 0.0001f;
				}

				float force = std_Gravity_Pull * (planets[i]->mass * planets[j]->mass) / (distance * distance);
				glm::vec3 direction = glm::normalize(planets[j]->position - planets[i]->position);
				glm::vec3 forceVector = direction * force;

				// Apply equal and opposite forces
				forces[i] += forceVector;
				forces[j] -= forceVector; // Newton's 3rd law
			}
		}

		// 2nd pass: Apply forces and update velocities & positions
		for (int i = 0; i < planets.size(); i++) {
			planets[i]-> velocity += (forces[i] / planets[i]-> mass) * dt; // Update velocity
			planets[i]-> position += planets[i]->velocity * dt;           // Update position



			// Update sphere for rendering
			planets[i]->sphere.resetModel();
			planets[i]->sphere.scale(planets[i]->radius * glm::vec3(1.0f, 1.0f, 1.0f));
			planets[i]->sphere.translate(planets[i]->position);
		}
	}
};
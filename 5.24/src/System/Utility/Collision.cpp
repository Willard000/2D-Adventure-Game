#include "Collision.h"

#include <iostream>
#include <math.h>

#include "Environment.h"
#include "Window.h"

const double degree_to_rad = (M_PI / 180);

std::array<Vertex, 4> make_vertices(const SDL_Rect &rect, const double &rot) {
	std::array<Vertex, 4> vertices;
	vertices[0] = { (double)rect.x, (double)rect.y };
	vertices[1] = { (double)rect.x, double(rect.y + rect.h) };
	vertices[2] = { double(rect.x + rect.w), (double)rect.y };
	vertices[3] = { double(rect.x + rect.w), double(rect.y + rect.h) };

	if (rot != 0) {
		double rad_rot = rot * degree_to_rad;
		double cx = rect.x + rect.w / 2;
		double cy = rect.y + rect.h / 2;
		for (auto &vert : vertices) {
			double x = vert.x - cx;
			double y = vert.y - cy;
			vert.x = (x * cos(rad_rot) - y * sin(rad_rot)) + cx;
			vert.y = (x * sin(rad_rot) + y * cos(rad_rot)) + cy;
		}
	}

	return vertices;
}

Vertex get_edge_normal(const Vertex &a, const Vertex &b) {
	return { -(b.x - a.x), (b.y - a.y) };
}

std::array<Vertex, 2> make_axes(const std::array<Vertex, 4> &a) {
	std::array<Vertex, 2> axes;

	axes[0] = get_edge_normal(a[0], a[1]);
	axes[1] = get_edge_normal(a[0], a[2]);

	return axes;
}

double dot_product(const Vertex &a, const Vertex &b) {
	return (a.x * b.x) + (a.y * b.y);
}

Projection make_projection(const std::array<Vertex, 4> &vertices, const Vertex &axis) {
	double min = dot_product(vertices[0], axis);
	double max = min;

	for (int i = 1; i < (int)vertices.size(); ++i) {
		double dot = dot_product(vertices[i], axis);
		if (dot > max) {
			max = dot;
		}
		else if (dot < min) {
			min = dot;
		}
	}

	return { min, max };
}

bool overlap(const Projection &a, const Projection &b) {
	return (b.min <= a.max) && (a.min < b.max);
}

bool sat_collision(const SDL_Rect &a, const SDL_Rect &b, const double &a_rot, const double &b_rot) {
	std::array<Vertex, 4> a_rect = make_vertices(a, a_rot);
	std::array<Vertex, 4> b_rect = make_vertices(b, b_rot);
	Projection projection1, projection2;

	std::array<Vertex, 2> a_axes = make_axes(a_rect);
	for (auto &axis : a_axes) {
		projection1 = make_projection(a_rect, axis);
		projection2 = make_projection(b_rect, axis);
		if (!overlap(projection1, projection2)) {
			return false;
		}
	}

	std::array<Vertex, 2> b_axes = make_axes(b_rect);
	for (auto &axis : b_axes) {
		projection1 = make_projection(a_rect, axis);
		projection2 = make_projection(b_rect, axis);
		if (!overlap(projection1, projection2)) {
			return false;
		}
	}

	return true;
}
#pragma once

#include <cmath>
#include "Mesh.h"

template<typename MeshVertex>
void join_sub_meshes_into_first(
	  std::vector<MeshVertex>&       vertices_first
	, std::vector<uint32_t>&         indices_first
	, std::vector<MeshVertex> const& vertices_second
	, std::vector<uint32_t>&         indices_second
) {
	std::size_t ofs = vertices_first.size();
	for(auto& v : vertices_second) {
		vertices_first.push_back(v);
	}
	for(auto& i : indices_second) {
		indices_first.push_back(i + ofs);
	}
}

template<typename MeshVertex>
Mesh<MeshVertex> make_simple_shere(
	  GL_Window*  parent
	, double      radius = 1.0
	, std::size_t stacks = 24
	, std::size_t sectors = 24
) {
	auto make_vertices = [&]() {
		std::vector<MeshVertex> vertices;
		vertices.reserve((stacks + 1)*(sectors + 1));
		for(std::size_t i = 0; i <= stacks; ++i) {
			double stackAngle = M_PI / 2.0 - i * M_PI / stacks;
			double xy = radius * std::cos(stackAngle);
			double z  = radius * std::sin(stackAngle);
			for(int j = 0; j <= sectors; ++j) {
				double sectorAngle = j * 2 * M_PI / sectors;
				double x = xy * std::cos(sectorAngle);
				double y = xy * std::sin(sectorAngle);
				MeshVertex vertex;
				vertex.position[0] = x;
				vertex.position[1] = y;
				vertex.position[2] = z;
				vertex.normal[0] = x / radius;
				vertex.normal[1] = y / radius;
				vertex.normal[2] = z / radius;
				vertex.texture[0] = (double)j / sectors;
				vertex.texture[1] = (double)i / stacks;
				vertices.push_back(vertex);
			}
		}
		return vertices;
	};
	auto make_indices = [&]() {
		std::vector<uint32_t> indices;
		for(std::size_t i = 0; i < stacks; ++i) {
			uint32_t k1 = i * (sectors + 1);     // beginning of current stack
			uint32_t k2 = k1 + sectors + 1;      // beginning of next stack

			for(int j = 0; j < sectors; ++j, ++k1, ++k2) {
				// 2 triangles per sector excluding first and last stacks
				// k1 => k2 => k1+1
				if(i != 0) {
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k1 + 1);
				}
				// k1+1 => k2 => k2+1
				if(i != (stacks-1)) {
					indices.push_back(k1 + 1);
					indices.push_back(k2);
					indices.push_back(k2 + 1);
				}
			}
		}
		return indices;
	};
	return Mesh<MeshVertex>(parent, make_vertices(), make_indices());
}

template<typename MeshVertex>
Mesh<MeshVertex> make_simple_torus(
	  GL_Window*  parent
	, double radius_1
	, double radius_2
	, std::size_t res_1
	, std::size_t res_2
) {
	++res_1;
	++res_2;
	auto make_vertices = [&]() {
		std::vector<MeshVertex> vertices(res_1 * res_2);
		// primary circle
		for(std::size_t i = 0; i < res_1; ++i) {
			double alpha = i * 2.0 * M_PI / (res_1 - 1.0);
			// secondary circle
			for(std::size_t j = 0; j < res_2; ++j) {
				double beta = j * 2.0 * M_PI / (res_2 - 1.0);
				MeshVertex& v = vertices[i * res_2 + j];
				double E_xy = radius_1 + radius_2 * cos(beta);
				v.position[0] = E_xy * cos(alpha);
				v.position[1] = E_xy * sin(alpha);
				v.position[2] = radius_2 * sin(beta);
				v.normal[0] = cos(beta) * cos(alpha);
				v.normal[1] = cos(beta) * sin(alpha);
				v.normal[2] = sin(beta);
				v.texture[0] = i/(res_1 - 1.0);
				v.texture[1] = j/(res_2 - 1.0);
			}
		}
		return vertices;
	};
	auto make_indices = [&]() {
		std::vector<uint32_t> indices;
		indices.reserve(res_1 * res_2 * 2 * 3);
		for(std::size_t i = 0; i < res_1; ++i) {
			std::size_t row_1 = i;
			std::size_t row_2 = row_1 + 1;
			if(row_2 >= res_1) {
				row_2 = 0;
			}
			for(std::size_t j = 0; j < res_2; ++j) {
				std::size_t col_1 = j;
				std::size_t col_2 = col_1 + 1;
				if(col_2 >= res_2) {
					col_2 = 0;
				}
				auto add = [&](std::size_t row, std::size_t col) {
					indices.push_back(
						row * res_2 + col
					);
				};
				add(row_1, col_1);
				add(row_2, col_1);
				add(row_2, col_2);
				add(row_1, col_1);
				add(row_2, col_2);
				add(row_1, col_2);
			}
		}
		return indices;
	};
	return Mesh<MeshVertex>(parent, make_vertices(), make_indices());
}

template<typename MeshVertex, typename HeightFunction>
Mesh<MeshVertex> make_simple_height_map(
	  GL_Window*  parent
	, double length_x
	, double length_y
	, double scale_z
	, std::size_t res_x
	, std::size_t res_y
	, HeightFunction f
) {
	auto make_vertices = [&]() {
		std::vector<MeshVertex> vertices(res_x * res_y);
		for(std::size_t i = 0; i < res_y; ++i) {
			for(std::size_t j = 0; j < res_x; ++j) {
				MeshVertex& v = vertices[i * res_x + j];
				double x = j / (res_x - 1.0);
				double y = i / (res_y - 1.0);
				double z = f(x,y);
				v.position[0] = (x - 0.5) * length_x;
				v.position[1] = (y - 0.5) * length_y;
				v.position[2] = z * scale_z;
				double delta = 1e-4;
				double pAx = (x + delta - 0.5) * length_x;
				double pAy = v.position[1];
				double pAz = f(x+delta,y) * scale_z;
				double pBx = v.position[0];
				double pBy = (y + delta - 0.5) * length_y;
				double pBz = f(x,y+delta) * scale_z;
				double dAx = (pAx - v.position[0]) / delta;
				double dAy = (pAy - v.position[1]) / delta;
				double dAz = (pAz - v.position[2]) / delta;
				double dBx = (pBx - v.position[0]) / delta;
				double dBy = (pBy - v.position[1]) / delta;
				double dBz = (pBz - v.position[2]) / delta;
				v.normal[0] = dAy*dBz - dAz*dBy;
				v.normal[1] = dAz*dBx - dAx*dBz;
				v.normal[2] = dAx*dBy - dAy*dBx;
				double l = sqrt(
					  v.normal[0]*v.normal[0]
					+ v.normal[1]*v.normal[1]
					+ v.normal[2]*v.normal[2]
				);
				v.normal[0] /= l;
				v.normal[1] /= l;
				v.normal[2] /= l;
				
				v.texture[0] = j / (res_x - 1.0);
				v.texture[1] = i / (res_y - 1.0);
			}
		}
		return vertices;
	};
	auto make_indices = [&]() {
		std::vector<uint32_t> indices;
		indices.reserve(6 * (res_x-1) * (res_y-1));
		for(std::size_t i = 1; i < res_y; ++i) {
			std::size_t row_1 = i - 1;
			std::size_t row_2 = i;
			for(std::size_t j = 1; j < res_x; ++j) {
				std::size_t col_1 = j - 1;
				std::size_t col_2 = j;
				auto add = [&](std::size_t row, std::size_t col) {
					indices.push_back(
						row * res_x + col
					);
				};
				add(row_1, col_1);
				add(row_2, col_1);
				add(row_2, col_2);
				add(row_1, col_1);
				add(row_2, col_2);
				add(row_1, col_2);
			}
		}
		return indices;
	};
	return Mesh<MeshVertex>(parent, make_vertices(), make_indices());
}


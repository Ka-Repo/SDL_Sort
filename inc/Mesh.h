#pragma once

#include <cassert>
#include <vector>
#include "GL_Window.h"

template<
	/**
	 * -Vertex has:
	 *   -position: 3 components of float/double, contiguous, random access
	 *   -normal:   3 components of float/double, contiguous, random access
	 *   -texture:  2 components of float/double, contiguous, random access
	 * 
	 * -Vertex is:
	 *   -contiguous
	 */
	typename Vertex
>
class Mesh {
private:
	GL_Window const* parent;
	unsigned int     vertex_VBO_id;
	unsigned int     index_VBO_id;
	std::size_t      indices_size;

private:
	Mesh(Mesh const&) = delete;
	Mesh& operator=(Mesh const&) = delete;

public:
	Mesh(Mesh&& other)
		: parent(other.parent)
		, vertex_VBO_id(other.vertex_VBO_id)
		, index_VBO_id(other.index_VBO_id)
		, indices_size(other.indices_size)
	{
		other.vertex_VBO_id = 0;
		other.index_VBO_id  = 0;
	}

	Mesh& operator=(Mesh&& other) {
		if(&other != this) {
			assert(other.parent == parent && "Mesh moved to wrong GL_Context");
			vertex_VBO_id = other.vertex_VBO_id;
			index_VBO_id  = other.index_VBO_id;
			indices_size  = other.indices_size;
			other.vertex_VBO_id = 0;
			other.index_VBO_id  = 0;
		}
		return *this;
	}

	~Mesh() {
		parent->do_operation(
			[&](){
				if(vertex_VBO_id) {
					glDeleteBuffers(1, &vertex_VBO_id);
				}
				if(index_VBO_id) {
					glDeleteBuffers(1, &index_VBO_id);
				}
			}
		);
	}

	Mesh(GL_Window const* parent)
		: parent(parent)
		, vertex_VBO_id(0)
		, index_VBO_id(0)
		, indices_size(0)
	{
		parent->do_operation(
			[&](){
				glGenBuffers(1, &vertex_VBO_id);
				glGenBuffers(1, &index_VBO_id);
			}
		);
	}

	template<GLenum usage = GL_STATIC_DRAW>
	Mesh( GL_Window const* parent
		, std::vector<Vertex> const& vertices
		, std::vector<uint32_t> const& indices
	)
		: Mesh(parent)
	{
		load<usage>(vertices, indices);
	}

	template<GLenum usage = GL_STATIC_DRAW>
	void load(
		  std::vector<Vertex> const& vertices
		, std::vector<uint32_t> const& indices
	) {
		if(!vertex_VBO_id || !index_VBO_id) {
			return;
		}
		parent->do_operation(
			[&](){
				glBindBuffer(GL_ARRAY_BUFFER, vertex_VBO_id);
				glBufferData(
					GL_ARRAY_BUFFER
					, sizeof(Vertex) * vertices.size()
					, vertices.data()
					, usage
				);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_VBO_id);
				glBufferData(
					GL_ELEMENT_ARRAY_BUFFER
					, sizeof(uint32_t) * indices.size()
					, indices.data()
					, usage
				);

				indices_size = indices.size();
				glBindBuffer(GL_ARRAY_BUFFER, 0);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			}
		);
	}

	class Lock {
	private:
		Mesh const* parent;

	private:
		Lock(Lock const&) = delete;
		Lock& operator=(Lock const&) = delete;

	public:
		Lock(Lock&& other)
			: parent(other.parent)
		{
			other.parent = nullptr;
		}
		Lock& operator=(Lock&& other) {
			if(&other != this) {
				parent = other.parent;
				other.parent = nullptr;
			}
			return *this;
		}
		Lock(Mesh const* parent)
			: parent(parent)
		{
			static constexpr Vertex probe{};
			auto buffer_offset = [](Vertex const& probe, auto const& probe_member) {
				std::size_t diff
					= reinterpret_cast<char const*>(&probe_member)
					- reinterpret_cast<char const*>(&probe)
				;
				return reinterpret_cast<void*>(diff);
			};
			auto gl_float_type = [](auto& probe_member) {
				using T = std::decay_t<decltype(probe_member[0])>;
				constexpr bool is_double = std::is_same_v<T, double>;
				constexpr bool is_float  = std::is_same_v<T, float>;
				static_assert(is_float || is_double);
				return is_float
					? GL_FLOAT
					: GL_DOUBLE
				;
			};
			glBindBuffer(GL_ARRAY_BUFFER, parent->vertex_VBO_id);
			glEnableClientState(GL_VERTEX_ARRAY);
			glVertexPointer(  3, gl_float_type(probe.position), sizeof(Vertex), buffer_offset(probe, probe.position));
			glClientActiveTexture(GL_TEXTURE0);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, gl_float_type(probe.texture), sizeof(Vertex), buffer_offset(probe, probe.texture));
			glNormalPointer(     gl_float_type(probe.normal),  sizeof(Vertex), buffer_offset(probe, probe.normal));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, parent->index_VBO_id);
		}
		~Lock() {
			glDisableClientState(GL_VERTEX_ARRAY);
			glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			glDisableClientState(GL_NORMAL_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		void draw() const {
			if(parent) {
				glDrawElements(GL_TRIANGLES, parent->indices_size, GL_UNSIGNED_INT, nullptr);
			}
		}
	};
	Lock lock() const {
		return Lock(this);
	}
};

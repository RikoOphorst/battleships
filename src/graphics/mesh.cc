#include <map>
#include <sstream>
#include "Mesh.h"
#include "../util/Defines.h"
#include "../util/Utils.h"

//#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader\tiny_obj_loader.h"

using namespace std;
using namespace igad;

namespace
{

ulong trihash(short k, short l, short m)
{
	ulong h = k;
	h <<= 20;
	h += l;
	h <<= 20;
	h += m;
	return h;
}

}

Mesh::Mesh(const std::string& filename) : _indexCount(0), _vbo{0, 0}, path_(filename), _topology(GL_TRIANGLES)
{
	HotReload();
}

Mesh::Mesh() : _indexCount(0), _vbo{ 0, 0 }, _topology(GL_TRIANGLES)
{
}



igad::Mesh::~Mesh()
{
	if (HasVertexBuffers())
	{
		glDeleteBuffers(2, _vbo);
		GL_GET_ERROR();
	}
}

//------------------------------------------------------------------------------------------------------
Mesh* igad::Mesh::GetQuad()
{
	Mesh* mesh = new Mesh();

	mesh->SetVertices({
		VertexFormat{ Vector3(-50.0f, -50.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2{ 0.0f, 0.0f } },
		VertexFormat{ Vector3(-50.0f,  50.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2{ 0.0f, 1.0f } },
		VertexFormat{ Vector3(50.0f,  50.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2{ 1.0f, 1.0f } },
		VertexFormat{ Vector3(50.0f, -50.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2{ 1.0f, 0.0f } }
	});

	mesh->SetIndices({
		0, 1, 2,
		0, 2, 3
	});

	mesh->Apply();

	return mesh;
}

//------------------------------------------------------------------------------------------------------
Mesh* igad::Mesh::GetCube()
{
	Mesh* mesh = new Mesh();

	mesh->SetVertices({
		VertexFormat{ Vector3(-1.0f, -1.0f,  1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2{ 0.0f, 0.0f } },
		VertexFormat{ Vector3(1.0f, -1.0f,  1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2{ 0.0f, 0.0f } },
		VertexFormat{ Vector3(-1.0f,  1.0f,  1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2{ 0.0f, 0.0f } },
		VertexFormat{ Vector3(1.0f,  1.0f,  1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2{ 0.0f, 0.0f } },
		VertexFormat{ Vector3(-1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2{ 0.0f, 0.0f } },
		VertexFormat{ Vector3(1.0f, -1.0f, -1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2{ 0.0f, 0.0f } },
		VertexFormat{ Vector3(-1.0f,  1.0f, -1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2{ 0.0f, 0.0f } },
		VertexFormat{ Vector3(1.0f,  1.0f, -1.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2{ 0.0f, 0.0f } }
	});

	mesh->SetIndices({
		0, 1, 2, 
		3, 7, 1, 
		5, 4, 7, 
		6, 2, 4, 
		0, 1
	});

	mesh->SetTopology(GL_TRIANGLE_STRIP);

	mesh->Apply();

	return mesh;
}

bool Mesh::Load(const string& filename)
{
	std::string input_file = filename;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string error;
	bool ret = tinyobj::LoadObj(shapes, materials, error, input_file.c_str());

	if (!error.empty())
	{
		std::cerr << error << std::endl;
	}

	if (!ret)
	{
		return false;
	}

	for (size_t i = 0; i < shapes.size(); i++) {
		assert((shapes[i].mesh.indices.size() % 3) == 0);
		for (size_t f = 0; f < shapes[i].mesh.indices.size() / 3; f++) {
			_indices.push_back(shapes[i].mesh.indices[3 * f + 0]);
			_indices.push_back(shapes[i].mesh.indices[3 * f + 1]);
			_indices.push_back(shapes[i].mesh.indices[3 * f + 2]);
		}

		assert((shapes[i].mesh.positions.size() % 3) == 0);
		for (size_t v = 0; v < shapes[i].mesh.positions.size() / 3; v++) {
			VertexFormat vert;
			vert.Position = Vector3(shapes[i].mesh.positions[3 * v + 0], shapes[i].mesh.positions[3 * v + 1], shapes[i].mesh.positions[3 * v + 2]);
			vert.Normal = Vector3(shapes[i].mesh.normals[3 * v + 0], shapes[i].mesh.normals[3 * v + 1], shapes[i].mesh.normals[3 * v + 2]);
			vert.Texture = Vector2{ shapes[i].mesh.texcoords[2 * v + 0], shapes[i].mesh.texcoords[2 * v + 1] };

			_vertices.push_back(vert);
		}
	}

	auto Vector2Cross = [](const Vector2& a, const Vector2& b)
	{
		Vector2 result;
		result.x = a.x * b.y - a.y * b.x;
		result.y = a.x * b.y - a.y * b.x;

		return result;
	};

	int idx1, idx2, idx3;

	Vector3 e1, e2, p;
	Vector3 uv, t1, t2;

	for (unsigned int i = 0; i < _indices.size(); i += 3)
	{
		idx1 = i;
		idx2 = i + 1;
		idx3 = i + 2;

		VertexFormat& v1 = _vertices.at(_indices.at(idx1));
		VertexFormat& v2 = _vertices.at(_indices.at(idx2));
		VertexFormat& v3 = _vertices.at(_indices.at(idx3));

		p = v1.Position;
		e1 = v2.Position - p;
		e2 = v3.Position - p;

		uv = Vector3(v1.Texture.x, v1.Texture.y, 1.0f);
		t1 = Vector3(v2.Texture.x, v2.Texture.y, 1.0f) - uv;
		t2 = Vector3(v3.Texture.x, v3.Texture.y, 1.0f) - uv;

		Vector2 cross = Vector2Cross(Vector2{ t1.x, t1.y }, Vector2{ t2.x, t2.y });
		float r = 1.0f / cross.x;

		if (r == std::numeric_limits<float>::infinity())
		{
			int i = 0;
		}

		Vector3 tangent = (e1 * t2.y - e2 * t1.y) * r;
		Vector3 bitangent = (e2 * t1.x - e1 * t2.x) * r;

		v1.Tangent = v2.Tangent = v3.Tangent = tangent;
		v1.Bitangent = v2.Bitangent = v3.Bitangent = bitangent;
	}

	return true;
}

void Mesh::SetVertices(vector<VertexFormat>&& vertices)
{
	_vertices = move(vertices);
}

void Mesh::SetIndices(vector<GLushort>&& indices)
{
	_indices = move(indices);
}

void Mesh::Apply()
{
	// Allocate two buffers
	glGenBuffers(2, _vbo);
	GL_GET_ERROR();

	// Array buffer contains the attribute data
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
	GL_GET_ERROR();

	// Copy into VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices[0]) * _vertices.size(), &_vertices[0], GL_STATIC_DRAW);
	GL_GET_ERROR();
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Unbind buffer
	GL_GET_ERROR();

	// Element array buffer contains the indices.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo[1]);
	GL_GET_ERROR();

	// Copy into VBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices[0]) * _indices.size(), &_indices[0], GL_STATIC_DRAW);
	GL_GET_ERROR();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // Unbind buffer
	GL_GET_ERROR();

	_indexCount =  static_cast<uint>(_indices.size());
	_vertices.clear();
	_indices.clear();
}

const GLuint* Mesh::GetVertexBuffers() const
{
	return _vbo;
}

void igad::Mesh::SetTopology(int topology)
{
	_topology = topology;
}

const int & igad::Mesh::GetTopology() const
{
	return _topology;
}

bool Mesh::HasVertexBuffers() const
{
	return _vbo[0] != 0 && _vbo[1] != 0;
}

uint Mesh::GetIndexCount() const
{
	return _indexCount;
}

//------------------------------------------------------------------------------------------------------
void igad::Mesh::HotReload()
{
	if (HasVertexBuffers())
	{
		glDeleteBuffers(2, _vbo);
		GL_GET_ERROR();
	}

	bool success = Load(path_);
	if (!success)
	{
		LOG("Mesh::Mesh(%s) - Unable to load mesh.", path_.c_str());
		return;
	}
	Apply();
}

#pragma once

#include <vector>
#include "../graphics/OpenGL.h"
#include "../math/Vector3.h"

namespace igad
{

struct VertexFormat
{
	Vector3 Position;
	Vector3 Normal;
	Vector3 Tangent;
	Vector3 Bitangent;
	Vector2 Texture;
};

class Mesh
{
public:

	/// Imports an OBJ model mesh
	Mesh(const std::string& filename);

	/// Create an empty mesh
	Mesh();

	/// Destroy 
	~Mesh();

	static Mesh* GetQuad();
	static Mesh* GetCube();

	/// Imports an OBJ model mesh
	bool Load(const std::string& filenam);

	/// Set the mesh vertices 
	void SetVertices(std::vector<VertexFormat>&& vertices);

	/// Set the mesh indices
	void SetIndices(std::vector<GLushort>&& indices);

	/// Call this method once you have set the vertices and indices
	/// it will create VBOs that can be rendered. It will also 
	/// clear the vertices and indices data from the CPU
	void Apply();

	/// Getter for VBO's
	const GLuint* GetVertexBuffers() const;

	void SetTopology(int topology);
	const int& GetTopology() const;

	/// Determine if this mesh uses VBO's
	bool HasVertexBuffers() const;

	/// Get the nummber of indieces
	uint GetIndexCount() const;

	/// Only valid if the mesh is not ready to be rendered yet
	std::vector<VertexFormat> GetVertices() const { return _vertices; }

	/// Only valid if the mesh is not ready to be rendered yet
	std::vector<GLushort>	GetIndices() const { return	_indices; }

	void HotReload();

protected:

	std::string path_;

	/// Mesh vertices, stored temporary when loading or creating a mesh
	std::vector<VertexFormat>    _vertices;

	/// Mesh indices, stored temporary when loading or creating a mesh
	std::vector<GLushort>		_indices;
	
	/// Primitive count
	uint						_indexCount;
	int							_topology;

	/// Vertex buffers object
	/// [0] is vertices
	/// [1] is indices
	GLuint						_vbo[2];
};

}

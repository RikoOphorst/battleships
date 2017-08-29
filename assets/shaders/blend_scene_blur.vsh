#version 430 core

// Vertex atributes
in vec3 a_position;
in vec3 a_normal;
in vec3 a_tangent;
in vec3 a_bitangent;
in vec2 a_texture;

// Out attributes
out vec2 v_texture;


void main()
{
  v_texture = a_texture;

  gl_Position = vec4(a_position, 1.0f);
}

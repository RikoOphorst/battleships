#version 430 core

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

// Vertex atributes
in vec3 a_position;
in vec3 a_normal;
in vec3 a_tangent;
in vec3 a_bitangent;
in vec2 a_texture;

// Out attributes
out vec3 v_texture;

void main()
{
  v_texture = a_position;
  gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0f);
}

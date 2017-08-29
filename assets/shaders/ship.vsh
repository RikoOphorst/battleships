#version 430 core

// Uniforms
uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;
uniform vec3 u_eye_pos;
uniform sampler2D material_normal_map;

// Vertex atributes
in vec3 a_position;
in vec3 a_normal;
in vec3 a_tangent;
in vec3 a_bitangent;
in vec2 a_texture;

// Out attributes
out vec2 v_texture;
out vec3 v_pos;
out vec3 v_normal;
out vec3 v_tangent;
out vec3 v_bitangent;
out float v_intensity;

void main()
{
  v_pos = a_position;
  v_normal = normalize(a_normal);
  v_tangent = normalize(a_tangent);
  v_bitangent = normalize(a_bitangent);
  v_texture = a_texture;

  v_intensity = (80 - length(u_view * u_model * vec4(a_position, 1.0f))) / (80 - 0);

  gl_Position = u_projection * u_view * u_model * vec4(a_position, 1.0f);
}

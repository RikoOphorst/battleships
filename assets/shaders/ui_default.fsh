#version 430 core

in vec2 v_texture;
in vec3 v_pos;
in vec3 v_normal;
in vec3 v_tangent;
in vec3 v_bitangent;

uniform sampler2D material_diffuse_map;

out vec4 fragColor;

void main()
{
  fragColor = texture(material_diffuse_map, v_texture);
}

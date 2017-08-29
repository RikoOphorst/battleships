#version 430 core

in vec2 v_texture;

uniform sampler2D u_material_diffuse_map;

out vec4 fragColor;

void main()
{
  fragColor = texture(u_material_diffuse_map, v_texture);
}

#version 430 core

in vec3 v_texture;

uniform samplerCube material_diffuse_map;

out vec4 fragColor;

void main()
{
  fragColor = texture(material_diffuse_map, v_texture);
}

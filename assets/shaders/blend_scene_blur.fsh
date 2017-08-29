#version 430 core

in vec2 v_texture;

uniform sampler2D u_frame_buffer_texture;
uniform sampler2D u_depth_buffer_texture;
uniform sampler2D u_input_texture0;

layout(location = 0) out vec4 fragColor;

void main()
{
  vec4 col = texture(u_frame_buffer_texture, v_texture);
  vec4 bloom = texture(u_input_texture0, v_texture);
  fragColor = col + bloom;
}

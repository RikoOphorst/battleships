#version 430 core

in vec2 v_texture;

uniform sampler2D u_frame_buffer_texture;
uniform sampler2D u_depth_buffer_texture;
uniform sampler2D u_input_texture0;
uniform sampler2D u_input_texture1;
uniform sampler2D u_input_texture2;

layout(location = 0) out vec4 fragColor;

uniform float exposure = 0.4;

void main()
{
  const float gamma = 0.9;

  vec3 hdrColor = texture(u_input_texture0, v_texture).rgb;
  vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure); // Exposure tone mapping

  mapped = pow(mapped, vec3(1.0 / gamma)); // gamma correction

  fragColor = vec4(mapped, 1.0);
}

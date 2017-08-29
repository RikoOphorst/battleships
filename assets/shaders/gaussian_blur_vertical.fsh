#version 430 core

in vec2 v_texture;

uniform sampler2D u_frame_buffer_texture;
uniform sampler2D u_depth_buffer_texture;
uniform sampler2D u_input_texture0;
uniform sampler2D u_input_texture1;
uniform sampler2D u_input_texture2;

uniform float weight[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec4 blurColor;

void main()
{
  vec2 tex_offset = 1.0 / textureSize(u_input_texture1, 0);
  vec3 result = texture(u_input_texture1, v_texture).rgb * weight[0];

  for (int i = 1; i < 5; i++)
  {
    result += texture(u_input_texture1, v_texture + vec2(0.0f, tex_offset.y * i)).rgb * weight[i];
    result += texture(u_input_texture1, v_texture - vec2(0.0f, tex_offset.y * i)).rgb * weight[i];
  }

  fragColor = vec4(result, 1);
  blurColor = fragColor;
}

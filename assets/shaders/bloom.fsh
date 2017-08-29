#version 430 core

in vec2 v_texture;

uniform sampler2D u_frame_buffer_texture;
uniform sampler2D u_depth_buffer_texture;
uniform vec3 u_brightness_threshold;

layout(location = 0) out vec4 fragColor;
layout(location = 1) out vec4 brightColor;

void main()
{
  fragColor = texture(u_frame_buffer_texture, v_texture);

  float brightness = dot(fragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
  if (brightness > 0.2f)
  {
    brightColor = fragColor;
  }
  else
  {
    brightColor = vec4(0, 0, 0, 1);
  }
}

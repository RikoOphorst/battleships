#version 430 core

in vec2 v_texture;
in vec3 v_pos;
in vec3 v_normal;
in vec3 v_tangent;
in vec3 v_bitangent;
in float v_intensity;

uniform sampler2D u_texture;
uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;
uniform vec3 u_eye_pos;

uniform bool material_has_emissive;
uniform vec3 material_emissive;

uniform bool material_has_ambient;
uniform float material_ambient_reflectance;

uniform bool material_has_diffuse_map;
uniform sampler2D material_diffuse_map;

uniform bool material_has_normal_map;
uniform sampler2D material_normal_map;

uniform bool material_apply_specular;
uniform bool material_has_specular_map;
uniform sampler2D material_specular_map;

const vec3 global_ambient = vec3(0.0f, 0.5f, 0.8f);

out vec4 fragColor;

struct DirectionalLight
{
	bool is_active;
	vec3 direction;
	vec3 color;
};

uniform DirectionalLight directional_lights[48];

struct PointLight
{
	bool is_active;
	vec3 position;
	vec3 color;
	float constant_attenuation;
	float linear_attenuation;
	float quadratic_attenuation;
};

uniform PointLight point_lights[48];

struct SpotLight
{
	bool is_active;
	vec3 position;
	vec3 color;
	vec3 direction;
	float inner_cone;
	float outer_cone;
	float constant_attenuation;
	float linear_attenuation;
	float quadratic_attenuation;
};

uniform SpotLight spot_lights[48];

void main()
{
	vec3 position_worldspace = (u_model * vec4(v_pos, 1.0f)).xyz;

	vec3 normal = v_normal;
	float diffuse_factor;
	vec3 diffuse_term = vec3(0, 0, 0);
	vec4 sampled_diffuse_map = vec4(1, 1, 1, 1);
	vec4 sampled_specular_map = vec4(1, 1, 1, 0.5f);
	vec3 specular_term = vec3(0, 0, 0);
	vec3 light_direction_to_position;
	vec3 eye_direction_to_position;
	vec3 half_vector_light_eye;
	float distance_light_to_position;
	float atten_factor;

	if (material_has_normal_map)
	{
		vec3 normal2 = (texture(material_normal_map, v_texture) * 2.0f - 1.0f).xyz;
	  normal = normalize(normal2.x * v_tangent + normal2.y * v_bitangent + normal2.z * v_normal);
	}

	if (material_has_diffuse_map)
	{
		sampled_diffuse_map = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	if (material_has_specular_map)
	{
		sampled_specular_map = texture(material_specular_map, v_texture);
	}

	vec3 color = vec3(0, 0, 0);

	if (material_has_emissive)
	{
		color += material_emissive;
	}

	if (material_has_ambient)
	{
		color += material_ambient_reflectance * global_ambient;
	}

	for (int i = 0; i < 48; i++)
  {
    if (directional_lights[i].is_active == true)
    {
      diffuse_factor = dot(normal, normalize(-directional_lights[i].direction));
      diffuse_term = sampled_diffuse_map.rgb * directional_lights[i].color * max(diffuse_factor, 0.0);

      light_direction_to_position = normalize(-directional_lights[i].direction - position_worldspace);		// Calculate L : direction between light position and surface point position
      eye_direction_to_position = normalize(u_eye_pos - position_worldspace);											// Calculate V : direction between eye position and surface point position
      half_vector_light_eye = normalize(light_direction_to_position + eye_direction_to_position);	// Calculate H : half vector L & V

			if (material_apply_specular)
			{
				if (diffuse_factor > 0)
				{
					specular_term =
					 	sampled_specular_map.rgb * // The color of the specular highlight
						directional_lights[i].color * // The color of the light
						pow(
							max(
								dot(
									normal,
									half_vector_light_eye
								),
								0.0f
							),
							sampled_specular_map.x * 100.0f
						);
				}
			}

			color += (diffuse_term + specular_term);
    }
  }

	for (int i = 0; i < 48; i++)
  {
    if (point_lights[i].is_active == true)
    {
      diffuse_factor = dot(normal, normalize(point_lights[i].position));
      diffuse_term = sampled_diffuse_map.rgb * point_lights[i].color * max(diffuse_factor, 0.0);

      light_direction_to_position = normalize(point_lights[i].position - position_worldspace);		// Calculate L : direction between light position and surface point position
      eye_direction_to_position = normalize(u_eye_pos - position_worldspace);											// Calculate V : direction between eye position and surface point position
      half_vector_light_eye = normalize(light_direction_to_position + eye_direction_to_position);	// Calculate H : half vector L & V

			if (material_apply_specular)
			{
				if (diffuse_factor > 0)
				{
					specular_term =
					 	sampled_specular_map.rgb * // The color of the specular highlight
						point_lights[i].color * // The color of the light
						pow(
							max(
								dot(
									normal,
									half_vector_light_eye
								),
								0.0f
							),
							sampled_specular_map.x * 100.0f
						);
				}
			}

			distance_light_to_position = distance(position_worldspace, point_lights[i].position);
			atten_factor = 1.0f / (point_lights[i].constant_attenuation + point_lights[i].linear_attenuation * distance_light_to_position + point_lights[i].quadratic_attenuation * distance_light_to_position * distance_light_to_position);

			color += atten_factor * (diffuse_term + specular_term);
    }
  }

	for (int i = 0; i < 48; i++)
	{
		if (spot_lights[i].is_active == true)
		{
			diffuse_factor = dot(normal, normalize(spot_lights[i].position));
			diffuse_term = sampled_diffuse_map.rgb * spot_lights[i].color * max(diffuse_factor, 0.0);

			light_direction_to_position = normalize(spot_lights[i].position - position_worldspace);		// Calculate L : direction between light position and surface point position
			eye_direction_to_position = normalize(u_eye_pos - position_worldspace);											// Calculate V : direction between eye position and surface point position
			half_vector_light_eye = normalize(light_direction_to_position + eye_direction_to_position);	// Calculate H : half vector L & V

			if (material_apply_specular)
			{
				if (diffuse_factor > 0)
				{
					specular_term =
						sampled_specular_map.rgb * // The color of the specular highlight
						spot_lights[i].color * // The color of the light
						pow(
							max(
								dot(
									normal,
									half_vector_light_eye
								),
								0.0f
							),
							sampled_specular_map.x * 100.0f
						);
				}
			}

			distance_light_to_position = distance(position_worldspace, spot_lights[i].position);

			vec3 surf_to_light = normalize(position_worldspace - spot_lights[i].position);
		  float cos_direction = dot(surf_to_light, normalize(spot_lights[i].direction));
		  float dual_cone_spot_light_factor = smoothstep(spot_lights[i].outer_cone, spot_lights[i].inner_cone, cos_direction);

			atten_factor = 1.0f / (spot_lights[i].constant_attenuation + spot_lights[i].linear_attenuation * distance_light_to_position + spot_lights[i].quadratic_attenuation * distance_light_to_position * distance_light_to_position);

			color += atten_factor * dual_cone_spot_light_factor * (diffuse_term + specular_term);
		}
	}

  fragColor = vec4(color, 1);
}

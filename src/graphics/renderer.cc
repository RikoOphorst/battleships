#include "Renderer.h"

#include "../core/entity.h"
#include "../core/widget.h"
#include "../graphics/material.h"
#include "../graphics/device.h"
#include "../graphics/directional_light.h"
#include "../graphics/point_light.h"
#include "../graphics/spot_light.h"

namespace igad
{
	const unsigned int Renderer::VAO_LIFETIME = 4;

	//------------------------------------------------------------------------------------------------------
	Renderer::Renderer(Material* material) : 
		material_(material)
	{
		Shader* shader = material->GetShader();

		uniforms_[shader] = MaterialUniforms(
			shader->GetUniform("u_projection"),
			shader->GetUniform("u_model"),
			shader->GetUniform("u_view"),
			shader->GetUniform("u_eye_pos")
		);

		position_attrib_ = shader->GetAttribute("a_position");
		normal_attrib_ = shader->GetAttribute("a_normal");
		tangent_attrib_ = shader->GetAttribute("a_tangent");
		bitangent_attrib_ = shader->GetAttribute("a_bitangent");
		tex_coord_attrib_ = shader->GetAttribute("a_texture");

		for (int i = 0; i < 48; i++)
		{
			uniforms_[shader].directional_light_uniforms_.push_back({
				shader->GetUniform(std::string("directional_lights[") + std::to_string(i) + std::string("].is_active")),
				shader->GetUniform(std::string("directional_lights[") + std::to_string(i) + std::string("].direction")),
				shader->GetUniform(std::string("directional_lights[") + std::to_string(i) + std::string("].color"))
			});
		}

		for (int i = 0; i < 48; i++)
		{
			uniforms_[shader].point_light_uniforms_.push_back({
				shader->GetUniform(std::string("point_lights[") + std::to_string(i) + std::string("].is_active")),
				shader->GetUniform(std::string("point_lights[") + std::to_string(i) + std::string("].position")),
				shader->GetUniform(std::string("point_lights[") + std::to_string(i) + std::string("].color")),
				shader->GetUniform(std::string("point_lights[") + std::to_string(i) + std::string("].constant_attenuation")),
				shader->GetUniform(std::string("point_lights[") + std::to_string(i) + std::string("].linear_attenuation")),
				shader->GetUniform(std::string("point_lights[") + std::to_string(i) + std::string("].quadratic_attenuation"))
			});
		}

		for (int i = 0; i < 48; i++)
		{
			uniforms_[shader].spot_light_uniforms_.push_back({
				shader->GetUniform(std::string("spot_lights[") + std::to_string(i) + std::string("].is_active")),
				shader->GetUniform(std::string("spot_lights[") + std::to_string(i) + std::string("].position")),
				shader->GetUniform(std::string("spot_lights[") + std::to_string(i) + std::string("].color")),
				shader->GetUniform(std::string("spot_lights[") + std::to_string(i) + std::string("].direction")),
				shader->GetUniform(std::string("spot_lights[") + std::to_string(i) + std::string("].inner_cone")),
				shader->GetUniform(std::string("spot_lights[") + std::to_string(i) + std::string("].outer_cone")),
				shader->GetUniform(std::string("spot_lights[") + std::to_string(i) + std::string("].constant_attenuation")),
				shader->GetUniform(std::string("spot_lights[") + std::to_string(i) + std::string("].linear_attenuation")),
				shader->GetUniform(std::string("spot_lights[") + std::to_string(i) + std::string("].quadratic_attenuation"))
			});
		}
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::Begin(Material* material, const Matrix44& view, const Matrix44& projection, const Vector3& eye_pos)
	{
		if (material != material_)
		{
			material_ = material;
			Shader* shader = material->GetShader();

			if (uniforms_.find(shader) == uniforms_.end())
			{
				uniforms_[shader] = MaterialUniforms(
					shader->GetUniform("u_projection"),
					shader->GetUniform("u_model"),
					shader->GetUniform("u_view"),
					shader->GetUniform("u_eye_pos")
					);

				for (int i = 0; i < 48; i++)
				{
					uniforms_[shader].directional_light_uniforms_.push_back({
						shader->GetUniform(std::string("directional_lights[") + std::to_string(i) + std::string("].is_active")),
						shader->GetUniform(std::string("directional_lights[") + std::to_string(i) + std::string("].direction")),
						shader->GetUniform(std::string("directional_lights[") + std::to_string(i) + std::string("].color"))
					});
				}

				for (int i = 0; i < 48; i++)
				{
					uniforms_[shader].point_light_uniforms_.push_back({
						shader->GetUniform(std::string("point_lights[") + std::to_string(i) + std::string("].is_active")),
						shader->GetUniform(std::string("point_lights[") + std::to_string(i) + std::string("].position")),
						shader->GetUniform(std::string("point_lights[") + std::to_string(i) + std::string("].color")),
						shader->GetUniform(std::string("point_lights[") + std::to_string(i) + std::string("].constant_attenuation")),
						shader->GetUniform(std::string("point_lights[") + std::to_string(i) + std::string("].linear_attenuation")),
						shader->GetUniform(std::string("point_lights[") + std::to_string(i) + std::string("].quadratic_attenuation"))
					});
				}

				for (int i = 0; i < 48; i++)
				{
					uniforms_[shader].spot_light_uniforms_.push_back({
						shader->GetUniform(std::string("spot_lights[") + std::to_string(i) + std::string("].is_active")),
						shader->GetUniform(std::string("spot_lights[") + std::to_string(i) + std::string("].position")),
						shader->GetUniform(std::string("spot_lights[") + std::to_string(i) + std::string("].color")),
						shader->GetUniform(std::string("spot_lights[") + std::to_string(i) + std::string("].direction")),
						shader->GetUniform(std::string("spot_lights[") + std::to_string(i) + std::string("].inner_cone")),
						shader->GetUniform(std::string("spot_lights[") + std::to_string(i) + std::string("].outer_cone")),
						shader->GetUniform(std::string("spot_lights[") + std::to_string(i) + std::string("].constant_attenuation")),
						shader->GetUniform(std::string("spot_lights[") + std::to_string(i) + std::string("].linear_attenuation")),
						shader->GetUniform(std::string("spot_lights[") + std::to_string(i) + std::string("].quadratic_attenuation"))
					});
				}
			}
		}

		material_->Activate();

		Shader* shader = material->GetShader();
		uniforms_[shader].projection_uniform_->SetValue(projection);
		uniforms_[shader].view_uniform_->SetValue(view);
		uniforms_[shader].eye_pos_uniform_->SetValue(eye_pos);

		const std::vector<DirectionalLight*>& directional_lights = Device::Instance()->GetDirectionalLights();
		const std::vector<PointLight*>& point_lights = Device::Instance()->GetPointLights();
		const std::vector<SpotLight*>& spot_lights = Device::Instance()->GetSpotLights();

		for (int i = 0; i < 48; i++)
		{
			if (i < directional_lights.size())
			{
				uniforms_[shader].directional_light_uniforms_[i].is_active->SetValue(true);
				uniforms_[shader].directional_light_uniforms_[i].direction->SetValue(directional_lights[i]->GetDirection());
				uniforms_[shader].directional_light_uniforms_[i].color->SetValue(directional_lights[i]->GetColor());
			}
			else
			{
				uniforms_[shader].directional_light_uniforms_[i].is_active->SetValue(false);
			}
		}

		for (int i = 0; i < 48; i++)
		{
			if (i < point_lights.size())
			{
				uniforms_[shader].point_light_uniforms_[i].is_active->SetValue(true);
				uniforms_[shader].point_light_uniforms_[i].position->SetValue(point_lights[i]->GetPosition());
				uniforms_[shader].point_light_uniforms_[i].color->SetValue(point_lights[i]->GetColor());
				uniforms_[shader].point_light_uniforms_[i].constant_attenuation->SetValue(point_lights[i]->GetConstantAttenuation());
				uniforms_[shader].point_light_uniforms_[i].linear_attenuation->SetValue(point_lights[i]->GetLinearAttenuation());
				uniforms_[shader].point_light_uniforms_[i].quadratic_attenuation->SetValue(point_lights[i]->GetQuadraticAttenuation());
			}
			else
			{
				uniforms_[shader].point_light_uniforms_[i].is_active->SetValue(false);
			}
		}

		for (int i = 0; i < 48; i++)
		{
			if (i < spot_lights.size())
			{
				uniforms_[shader].spot_light_uniforms_[i].is_active->SetValue(true);
				uniforms_[shader].spot_light_uniforms_[i].position->SetValue(spot_lights[i]->GetPosition());
				uniforms_[shader].spot_light_uniforms_[i].color->SetValue(spot_lights[i]->GetColor());
				uniforms_[shader].spot_light_uniforms_[i].direction->SetValue(spot_lights[i]->GetDirection());
				uniforms_[shader].spot_light_uniforms_[i].inner_cone->SetValue(spot_lights[i]->GetInnerCone());
				uniforms_[shader].spot_light_uniforms_[i].outer_cone->SetValue(spot_lights[i]->GetOuterCone());
				uniforms_[shader].spot_light_uniforms_[i].constant_attenuation->SetValue(spot_lights[i]->GetConstantAttenuation());
				uniforms_[shader].spot_light_uniforms_[i].linear_attenuation->SetValue(spot_lights[i]->GetLinearAttenuation());
				uniforms_[shader].spot_light_uniforms_[i].quadratic_attenuation->SetValue(spot_lights[i]->GetQuadraticAttenuation());
			}
			else
			{
				uniforms_[shader].spot_light_uniforms_[i].is_active->SetValue(false);
			}
		}
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::Render(Entity* entity)
	{
		GLuint vao;

		Mesh* mesh = entity->GetMesh();

		if (vaos_.find(mesh) == vaos_.end())
		{
			// Generate new VAO for this mesh
			glGenVertexArrays(1, &vao);
			GL_GET_ERROR();
			glBindVertexArray(vao);
			GL_GET_ERROR();

			const GLuint* vbo = mesh->GetVertexBuffers();

#ifdef DEBUG
			if (glIsBuffer(vbo[0]) != GL_TRUE)
			{
				LOG("The vertex buffer is not a valid buffer (VBO).");
			}
			if (glIsBuffer(vbo[1]) != GL_TRUE)
			{
				LOG("The index buffer is not a valid buffer (VBO)");
			}
#endif

			// Bind the buffers to the global state
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			GL_GET_ERROR();
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
			GL_GET_ERROR();

			// Default to VBO values, the pointer addresses are interpreted as byte-offsets.
			const void* first_position = reinterpret_cast<const void*>(offsetof(VertexFormat, Position));
			const void* first_normal = reinterpret_cast<const void*>(offsetof(VertexFormat, Normal));
			const void* first_texture = reinterpret_cast<const void*>(offsetof(VertexFormat, Texture));
			const void* first_tangent = reinterpret_cast<const void*>(offsetof(VertexFormat, Tangent));
			const void* first_bitangent = reinterpret_cast<const void*>(offsetof(VertexFormat, Bitangent));

			GLsizei size = sizeof(VertexFormat);
			position_attrib_->SetAttributePointer(3, GL_FLOAT, GL_FALSE, size, first_position);
			normal_attrib_->SetAttributePointer(3, GL_FLOAT, GL_FALSE, size, first_normal);
			tangent_attrib_->SetAttributePointer(3, GL_FLOAT, GL_FALSE, size, first_tangent);
			bitangent_attrib_->SetAttributePointer(3, GL_FLOAT, GL_FALSE, size, first_bitangent);
			tex_coord_attrib_->SetAttributePointer(2, GL_FLOAT, GL_FALSE, size, first_texture);

			vaos_[mesh] = { vao, VAO_LIFETIME };

			// Unbind the vertex arrays
			glBindVertexArray(0);
		}
		else
		{
			// We still have an unexpired VAO for this mesh
			auto& entry = vaos_[mesh];
			vao = entry.vao;
			entry.count = VAO_LIFETIME;
		}
		
		glBindVertexArray(vao);
		GL_GET_ERROR();

		Shader* shader = material_->GetShader();
		uniforms_[shader].model_uniform_->SetValue(entity->GetTransform());

		const void* first_index = reinterpret_cast<const void*>(0);
		glDrawElements(mesh->GetTopology(), mesh->GetIndexCount(), GL_UNSIGNED_SHORT, first_index);
		GL_GET_ERROR();

		// Unbind buffers from global state.
		glBindVertexArray(0);
		GL_GET_ERROR();
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::Render(Widget* widget)
	{
		GLuint vao;

		Mesh* mesh = widget->GetMesh();

		if (vaos_.find(mesh) == vaos_.end())
		{
			// Generate new VAO for this mesh
			glGenVertexArrays(1, &vao);
			GL_GET_ERROR();
			glBindVertexArray(vao);
			GL_GET_ERROR();

			const GLuint* vbo = mesh->GetVertexBuffers();

#ifdef DEBUG
			if (glIsBuffer(vbo[0]) != GL_TRUE)
			{
				LOG("The vertex buffer is not a valid buffer (VBO).");
			}
			if (glIsBuffer(vbo[1]) != GL_TRUE)
			{
				LOG("The index buffer is not a valid buffer (VBO)");
			}
#endif

			// Bind the buffers to the global state
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			GL_GET_ERROR();
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
			GL_GET_ERROR();

			// Default to VBO values, the pointer addresses are interpreted as byte-offsets.
			const void* first_position = reinterpret_cast<const void*>(offsetof(VertexFormat, Position));
			const void* first_normal = reinterpret_cast<const void*>(offsetof(VertexFormat, Normal));
			const void* first_texture = reinterpret_cast<const void*>(offsetof(VertexFormat, Texture));
			const void* first_tangent = reinterpret_cast<const void*>(offsetof(VertexFormat, Tangent));
			const void* first_bitangent = reinterpret_cast<const void*>(offsetof(VertexFormat, Bitangent));

			GLsizei size = sizeof(VertexFormat);
			position_attrib_->SetAttributePointer(3, GL_FLOAT, GL_FALSE, size, first_position);
			normal_attrib_->SetAttributePointer(3, GL_FLOAT, GL_FALSE, size, first_normal);
			tangent_attrib_->SetAttributePointer(3, GL_FLOAT, GL_FALSE, size, first_tangent);
			bitangent_attrib_->SetAttributePointer(3, GL_FLOAT, GL_FALSE, size, first_bitangent);
			tex_coord_attrib_->SetAttributePointer(2, GL_FLOAT, GL_FALSE, size, first_texture);

			vaos_[mesh] = { vao, VAO_LIFETIME };

			// Unbind the vertex arrays
			glBindVertexArray(0);
		}
		else
		{
			// We still have an unexpired VAO for this mesh
			auto& entry = vaos_[mesh];
			vao = entry.vao;
			entry.count = VAO_LIFETIME;
		}

		glBindVertexArray(vao);
		GL_GET_ERROR();

		Shader* shader = material_->GetShader();
		uniforms_[shader].model_uniform_->SetValue(widget->GetTransform());

		const void* first_index = reinterpret_cast<const void*>(0);
		glDrawElements(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_SHORT, first_index);
		//glDrawArrays(GL_TRIANGLES, 0, mesh->GetIndexCount());
		GL_GET_ERROR();

		// Unbind buffers from global state.
		glBindVertexArray(0);
		GL_GET_ERROR();
	}

	//------------------------------------------------------------------------------------------------------
	void Renderer::End()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		material_->Deactivate();

		for (auto entry = vaos_.begin(); entry != vaos_.end();)
		{
			entry->second.count--;
			if (entry->second.count == 0)
			{
				glDeleteVertexArrays(1, &entry->second.vao);
				entry = vaos_.erase(entry);
			}
			else
			{
				++entry;
			}
		}
	}

}
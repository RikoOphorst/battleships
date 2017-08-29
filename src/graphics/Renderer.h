#pragma once

#include "../math/matrix44.h"
#include "../graphics/mesh.h"
#include "../graphics/shader.h"
#include "../graphics/texture.h"

#include <vector>
#include <unordered_map>

namespace igad
{
	class Material;
	class Entity;
	class Widget;

	/**
	* @class igad::Renderer
	* @author Riko Ophorst
	* @brief Allows you to render a mesh with a given shader program
	*/
	class Renderer
	{
	public:
		/**
		* @brief Creates a renderer with given shader program
		* @param[in] shader (Shader*) the shader program you want this renderer to use
		*/
		Renderer(Material* material);
	
		/**
		* @brief Prepares the renderer so that it can render objects
		* @param[in] view (const Matrix44&) the camera's view matrix
		* @param[in] projection (const Matrix44&) the camera's projection matrix
		*/
		void Begin(Material* material, const Matrix44& view, const Matrix44& projection, const Vector3& eye_pos);

		/**
		* @brief Renders an entity
		*/
		void Render(Entity* entity);

		/**
		* @brief Renders a widget
		*/
		void Render(Widget* entity);

		/**
		* @brief Cleans up any objects used by this renderer to code anything
		*/
		void End();

		static const unsigned int VAO_LIFETIME;

	protected:
		Material* material_; //<! The shader program this renderer uses
		ShaderAttribute* position_attrib_; //<! the position vertex attribute
		ShaderAttribute* normal_attrib_; //<! the normal vertex attribute
		ShaderAttribute* tangent_attrib_; //<! the tangent vertex attribute
		ShaderAttribute* bitangent_attrib_; //<! the bitangent vertex attribute
		ShaderAttribute* tex_coord_attrib_; //<! the uv-coord vertex attribute

		struct DirectionalLightUniform
		{
			ShaderUniform* is_active;
			ShaderUniform* direction;
			ShaderUniform* color;
		};

		struct PointLightUniform
		{
			ShaderUniform* is_active;
			ShaderUniform* position;
			ShaderUniform* color;
			ShaderUniform* constant_attenuation;
			ShaderUniform* linear_attenuation;
			ShaderUniform* quadratic_attenuation;
		};

		struct SpotLightUniform
		{
			ShaderUniform* is_active;
			ShaderUniform* position;
			ShaderUniform* color;
			ShaderUniform* direction;
			ShaderUniform* inner_cone;
			ShaderUniform* outer_cone;
			ShaderUniform* constant_attenuation;
			ShaderUniform* linear_attenuation;
			ShaderUniform* quadratic_attenuation;
		};

		/**
		* @struct igad::Renderer::VAO
		* @author Riko Ophorst
		* @brief Represents a VAO and its lifetime
		*/
		struct VAO
		{
			GLuint vao; //<! The VAO this object represents
			uint count; //<! The lifetime this VAO has left
		};

		struct MaterialUniforms
		{
			MaterialUniforms() {}
			MaterialUniforms(
				ShaderUniform* projection, 
				ShaderUniform* model, 
				ShaderUniform* view, 
				ShaderUniform* eye_pos) :
				projection_uniform_(projection),
				model_uniform_(model),
				view_uniform_(view),
				eye_pos_uniform_(eye_pos)
			{}

			ShaderUniform* projection_uniform_; //<! the projection matrix uniform
			ShaderUniform* model_uniform_; //<! the model matrix uniform
			ShaderUniform* view_uniform_; //<! the view matrix uniform
			ShaderUniform* eye_pos_uniform_; //<! the eye pos uniform

			std::vector<DirectionalLightUniform> directional_light_uniforms_;
			std::vector<PointLightUniform> point_light_uniforms_;
			std::vector<SpotLightUniform> spot_light_uniforms_;
		};

		std::unordered_map<const Mesh*, VAO> vaos_; //<! All vaos that currently exist
		std::unordered_map<Shader*, MaterialUniforms> uniforms_; //<! Uniforms
	};
}
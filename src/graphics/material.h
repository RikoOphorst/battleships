#pragma once

#include "../math/vector3.h"

namespace igad
{
	class Texture;
	class Shader;
	class ShaderUniform;

	class Material
	{
	public:
		Material(Shader* shader);
		Material(Shader* shader, const Vector3& emissive, float ambient_reflectance, Texture* diffuse_map, Texture* normal_map, Texture* specular_map, bool apply_specular = true);
		~Material();

		void Activate();
		void Deactivate();

		Shader* GetShader() const;
		const Vector3& GetEmissive() const;
		const float& GetAmbientReflectance() const;
		Texture* GetDiffuseMap() const;
		Texture* GetNormalMap() const;
		Texture* GetSpecularMap() const;
		const bool& GetApplySpecular() const;

		void SetShader(Shader* shader);
		void SetEmissive(const Vector3& emissive);
		void SetAmbientReflectance(float ambient_reflectance);
		void SetDiffuseMap(Texture* diffuse_map);
		void SetNormalMap(Texture* normal_map);
		void SetSpecularMap(Texture* specular_map);
		void SetApplySpecular(bool apply_specular);

	private:
		void SetupUniforms();

		Shader* shader_;

		Vector3 emissive_;
		float ambient_reflectance_;
		Texture* diffuse_map_;
		Texture* normal_map_;
		bool apply_specular_;
		Texture* specular_map_;

		ShaderUniform* uniform_has_emissive_;
		ShaderUniform* uniform_emissive_;
		ShaderUniform* uniform_has_ambient_;
		ShaderUniform* uniform_ambient_reflectance_;
		ShaderUniform* uniform_has_diffuse_map_;
		ShaderUniform* uniform_diffuse_;
		ShaderUniform* uniform_has_normal_map_;
		ShaderUniform* uniform_normal_map_;
		ShaderUniform* uniform_apply_specular_;
		ShaderUniform* uniform_has_specular_map_;
		ShaderUniform* uniform_specular_map_;
	};
}
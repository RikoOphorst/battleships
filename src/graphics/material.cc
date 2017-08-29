#include "material.h"

#include "../graphics/shader.h"
#include "../graphics/texture.h"

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	Material::Material(Shader* shader) :
		shader_(shader),
		emissive_(0, 0, 0),
		ambient_reflectance_(0.1f),
		diffuse_map_(nullptr),
		normal_map_(nullptr),
		specular_map_(nullptr),
		apply_specular_(true)
	{
		SetupUniforms();
	}

	//------------------------------------------------------------------------------------------------------
	Material::Material(Shader* shader, const Vector3& emissive, float ambient_reflectance, Texture* diffuse_map, Texture* normal_map, Texture* specular_map, bool apply_specular_) :
		shader_(shader),
		emissive_(emissive),
		ambient_reflectance_(ambient_reflectance),
		diffuse_map_(diffuse_map),
		normal_map_(normal_map),
		specular_map_(specular_map),
		apply_specular_(apply_specular_)
	{
		SetupUniforms();
	}

	//------------------------------------------------------------------------------------------------------
	Material::~Material()
	{
		
	}

	//------------------------------------------------------------------------------------------------------
	void Material::Activate()
	{
		shader_->Activate();

		uniform_has_emissive_->SetValue(true);
		uniform_emissive_->SetValue(emissive_);

		uniform_has_ambient_->SetValue(true);
		uniform_ambient_reflectance_->SetValue(ambient_reflectance_);
		
		if (diffuse_map_ != nullptr)
		{
			uniform_has_diffuse_map_->SetValue(true);
			uniform_diffuse_->SetValue(*diffuse_map_);
		}
		else
		{
			uniform_has_diffuse_map_->SetValue(false);
		}
		
		if (normal_map_ != nullptr)
		{
			uniform_has_normal_map_->SetValue(true);
			uniform_normal_map_->SetValue(*normal_map_);
		}
		else
		{
			uniform_has_normal_map_->SetValue(false);
		}

		uniform_apply_specular_->SetValue(apply_specular_);

		if (specular_map_ != nullptr)
		{
			uniform_has_specular_map_->SetValue(true);
			uniform_specular_map_->SetValue(*specular_map_);
		}
		else
		{
			uniform_has_specular_map_->SetValue(false);
		}
	}

	//------------------------------------------------------------------------------------------------------
	void Material::Deactivate()
	{
		shader_->Deactivate();
	}
	
	//------------------------------------------------------------------------------------------------------
	Shader* Material::GetShader() const
	{
		return shader_;
	}

	//------------------------------------------------------------------------------------------------------
	const Vector3& Material::GetEmissive() const
	{
		return emissive_;
	}

	//------------------------------------------------------------------------------------------------------
	const float& Material::GetAmbientReflectance() const
	{
		return ambient_reflectance_;
	}

	//------------------------------------------------------------------------------------------------------
	Texture* Material::GetDiffuseMap() const
	{
		return diffuse_map_;
	}

	//------------------------------------------------------------------------------------------------------
	Texture* Material::GetNormalMap() const
	{
		return normal_map_;
	}

	//------------------------------------------------------------------------------------------------------
	Texture* Material::GetSpecularMap() const
	{
		return specular_map_;
	}

	//------------------------------------------------------------------------------------------------------
	const bool & Material::GetApplySpecular() const
	{
		return apply_specular_;
	}

	//------------------------------------------------------------------------------------------------------
	void Material::SetShader(Shader* shader)
	{
		shader_ = shader;
	}

	//------------------------------------------------------------------------------------------------------
	void Material::SetEmissive(const Vector3& emissive)
	{
		emissive_ = emissive;
	}

	//------------------------------------------------------------------------------------------------------
	void Material::SetAmbientReflectance(float ambient_reflectance)
	{
		ambient_reflectance_ = ambient_reflectance;
	}

	//------------------------------------------------------------------------------------------------------
	void Material::SetDiffuseMap(Texture* diffuse_map)
	{
		diffuse_map_ = diffuse_map;
	}

	//------------------------------------------------------------------------------------------------------
	void Material::SetNormalMap(Texture* normal_map)
	{
		normal_map_ = normal_map;
	}

	//------------------------------------------------------------------------------------------------------
	void Material::SetSpecularMap(Texture* specular_map)
	{
		specular_map_ = specular_map;
	}

	//------------------------------------------------------------------------------------------------------
	void Material::SetApplySpecular(bool apply_specular)
	{
		apply_specular_ = apply_specular;
	}
	
	//------------------------------------------------------------------------------------------------------
	void Material::SetupUniforms()
	{
		uniform_has_emissive_				= shader_->GetUniform("material_has_emissive");
		uniform_emissive_					= shader_->GetUniform("material_emissive");

		uniform_has_ambient_				= shader_->GetUniform("material_has_ambient");
		uniform_ambient_reflectance_		= shader_->GetUniform("material_ambient_reflectance");

		uniform_has_diffuse_map_			= shader_->GetUniform("material_has_diffuse_map");
		uniform_diffuse_					= shader_->GetUniform("material_diffuse_map");

		uniform_has_normal_map_				= shader_->GetUniform("material_has_normal_map");
		uniform_normal_map_					= shader_->GetUniform("material_normal_map");

		uniform_apply_specular_				= shader_->GetUniform("material_apply_specular");
		uniform_has_specular_map_			= shader_->GetUniform("material_has_specular_map");
		uniform_specular_map_				= shader_->GetUniform("material_specular_map");
	}
}
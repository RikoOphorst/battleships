#include "content_manager.h"
#include "../graphics/shader.h"
#include "../graphics/mesh.h"

#include <sys/stat.h>

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	ContentManager::ContentManager()
	{
		alloc_ = memory::MemoryManager::Instance()->ConstructAllocator<memory::FreeListAllocator>(500000);
	}

	//------------------------------------------------------------------------------------------------------
	ContentManager::~ContentManager()
	{
		
	}

	//------------------------------------------------------------------------------------------------------
	ContentManager* ContentManager::Instance()
	{
		static ContentManager* device = nullptr;
		static memory::PoolAllocator* alloc = nullptr;

		if (device == nullptr)
		{
			memory::MemoryManager::Create();
			alloc = memory::MemoryManager::Instance()->ConstructAllocator<memory::PoolAllocator, ContentManager>(sizeof(ContentManager));
			device = memory::MemoryManager::Allocate<ContentManager>(alloc);
		}

		return device;
	}

	//------------------------------------------------------------------------------------------------------
	Shader* ContentManager::GetShader(const std::string& vertex_shader_path, const std::string& fragment_shader_path, const std::string& geometry_shader_path)
	{
		auto& it = shaders_.find(vertex_shader_path + fragment_shader_path + geometry_shader_path);
		if (it == shaders_.end())
		{
			memory::SharedPtr<Shader> shader = memory::SharedPtr<Shader>(memory::MemoryManager::Allocate<Shader>(alloc_, fragment_shader_path, vertex_shader_path, geometry_shader_path), alloc_);
			shaders_[vertex_shader_path + fragment_shader_path + geometry_shader_path] = ContentItem<Shader>(shader, std::vector<std::string>{ fragment_shader_path, vertex_shader_path, geometry_shader_path });
			return shader.get();
		}
		else
		{
			return it->second.item.get();
		}
	}

	//------------------------------------------------------------------------------------------------------
	Mesh* ContentManager::GetMesh(const std::string& mesh_path)
	{
		auto& it = meshes_.find(mesh_path);
		if (it == meshes_.end())
		{
			memory::SharedPtr<Mesh> mesh = memory::SharedPtr<Mesh>(memory::MemoryManager::Allocate<Mesh>(alloc_, mesh_path), alloc_);
			meshes_[mesh_path] = ContentItem<Mesh>(mesh, std::vector<std::string>{ mesh_path });
			return mesh.get();
		}
		else
		{
			return it->second.item.get();
		}
	}

	//------------------------------------------------------------------------------------------------------
	Texture* ContentManager::GetTexture(const std::string& texture_path)
	{
		auto& it = textures_.find(texture_path);
		if (it == textures_.end())
		{
			memory::SharedPtr<Texture> texture = memory::SharedPtr<Texture>(memory::MemoryManager::Allocate<Texture>(alloc_, texture_path), alloc_);
			textures_[texture_path] = ContentItem<Texture>(texture, std::vector<std::string>{ texture_path });
			return texture.get();
		}
		else
		{
			return it->second.item.get();
		}
	}

	//------------------------------------------------------------------------------------------------------
	void ContentManager::Update()
	{
		for (auto& pair = shaders_.begin(); pair != shaders_.end(); pair++)
		{
			auto& it = pair->second;
			for (int i = 0; i < it.files.size(); i++)
			{
				if (it.files[i].size() == 0)
					continue;
				
				bool failed = false;
				time_t last_changed = ContentManager::GetTime(pair->second.files[i], failed);
				if (!failed)
				{
					if (last_changed != it.last_changed[i])
					{
						it.item->HotRelead();
						it.last_changed[i] = last_changed;
					}
				}
			}
		}

		for (auto& pair = meshes_.begin(); pair != meshes_.end(); pair++)
		{
			auto& it = pair->second;
			for (int i = 0; i < it.files.size(); i++)
			{
				if (it.files[i].size() == 0)
					continue;

				bool failed = false;
				time_t last_changed = ContentManager::GetTime(pair->second.files[i], failed);
				if (!failed)
				{
					if (last_changed != it.last_changed[i])
					{
						it.item->HotReload();
						it.last_changed[i] = last_changed;
					}
				}
			}
		}

		for (auto& pair = textures_.begin(); pair != textures_.end(); pair++)
		{
			auto& it = pair->second;
			for (int i = 0; i < it.files.size(); i++)
			{
				if (it.files[i].size() == 0)
					continue;

				bool failed = false;
				time_t last_changed = ContentManager::GetTime(pair->second.files[i], failed);
				if (!failed)
				{
					if (last_changed != it.last_changed[i])
					{
						it.item->HotReload();
						it.last_changed[i] = last_changed;
					}
				}
			}
		}
	}
	
	//------------------------------------------------------------------------------------------------------
	time_t ContentManager::GetTime(const std::string& path, bool& failed)
	{
		struct stat buffer;
		stat(path.c_str(), &buffer);

		return buffer.st_mtime;
	}
}
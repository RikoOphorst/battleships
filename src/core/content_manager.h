#pragma once

#include <string>
#include <map>
#include <Windows.h>
#include <WinBase.h>
#include "../memory/memory_manager.h"
#include "../memory/smart_ptrs/shared_ptr.h"

#include "../graphics/shader.h"
#include "../graphics/mesh.h"
#include "../graphics/texture.h"

namespace igad
{
	class Shader;
	class Mesh;
	class Texture;

	class ContentManager
	{
	public:
		ContentManager();
		~ContentManager();

		static ContentManager* Instance();

		Shader* GetShader(const std::string& vertex_shader_path, const std::string& fragment_shader_path, const std::string& geometry_shader_path);
		Mesh* GetMesh(const std::string& mesh_path);
		Texture* GetTexture(const std::string& texture_path);

		void Update();

		time_t GetTime(const std::string& path, bool& failed);

	private:
		template<typename T>
		struct ContentItem
		{
			ContentItem() {}
			ContentItem(memory::SharedPtr<T> item, std::vector<std::string> files);

			memory::SharedPtr<T> item;
			std::vector<std::string> files;
			std::vector<time_t> last_changed;
		};

		memory::FreeListAllocator* alloc_;

		std::map<std::string, ContentItem<Shader>> shaders_;
		std::map<std::string, ContentItem<Mesh>> meshes_;
		std::map<std::string, ContentItem<Texture>> textures_;
	};
	
	template<typename T>
	inline ContentManager::ContentItem<T>::ContentItem(memory::SharedPtr<T> item, std::vector<std::string> _files) :
		item(item),
		files(_files)
	{
		last_changed.resize(files.size());
	}
}
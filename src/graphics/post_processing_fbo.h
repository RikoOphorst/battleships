#pragma once

#include "../graphics/opengl.h"
#include "../graphics/mesh.h"
#include "../memory/memory_manager.h"

namespace igad
{
	class Shader;
	class PostProcessEffect;

	class PostProcessingFBO
	{
	public:
		PostProcessingFBO();
		~PostProcessingFBO();

		void PrepareFBO();
		void PreparePostProcessing();
		void ApplyPostProcessEffect(PostProcessEffect* effect);
		void EndPostProcessing();

	private:
		memory::StackAllocator* allocator_;
		Mesh* screen_quad_;
		GLuint screen_quad_vao_;

		PostProcessEffect* last_effect_;

		GLuint frame_buffer_texture_;
		GLuint depth_buffer_texture_;
		GLuint frame_buffer_object_;

		struct PPFBO
		{
			PPFBO(const std::vector<GLuint>& textures, const GLuint& depth_texture, const GLuint& fbo) :
				post_processing_textures_(textures),
				post_processing_depth_texture_(depth_texture),
				post_processing_object_(fbo)
			{}
			std::vector<GLuint> post_processing_textures_;
			GLuint post_processing_depth_texture_;
			GLuint post_processing_object_;
		};

		std::vector<PPFBO> post_processing_fbos_;

		bool first_post_process_fbo_;
	};
}
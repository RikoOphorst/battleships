#include "post_processing_fbo.h"

#include "../core/content_manager.h"
#include "../graphics/post_process_effect.h"
#include "../graphics/shader.h"

namespace igad
{
	//------------------------------------------------------------------------------------------------------
	PostProcessingFBO::PostProcessingFBO()
	{
		allocator_ = memory::MemoryManager::Instance()->ConstructAllocator<memory::StackAllocator>(sizeof(Mesh));

		glGenFramebuffers(1, &frame_buffer_object_);
		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_object_);

		glGenRenderbuffers(1, &depth_buffer_texture_);
		glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer_texture_);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1280, 720);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		glGenTextures(1, &frame_buffer_texture_);
		glBindTexture(GL_TEXTURE_2D, frame_buffer_texture_);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1280, 720, 0, GL_RGBA, GL_FLOAT, NULL);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, frame_buffer_texture_, 0);

		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth_buffer_texture_); 
		
		GLenum status;
		if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
			fprintf(stderr, "glCheckFramebufferStatus: error %p", status);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		for (int i = 0; i < 2; i++)
		{
			GLuint fbo;
			GLuint dbt;
			std::vector<GLuint> textures;

			glGenFramebuffers(1, &fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);

			glGenRenderbuffers(1, &dbt);
			glBindRenderbuffer(GL_RENDERBUFFER, dbt);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1280, 720);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, dbt);
			
			textures.resize(8);
			glGenTextures(8, &textures[0]);
			for (int i = 0; i < 8; i++)
			{
				glBindTexture(GL_TEXTURE_2D, textures[i]);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 1280, 720, 0, GL_RGBA, GL_FLOAT, NULL);
				glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, textures[i], 0);
			}

			if ((status = glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE) {
				fprintf(stderr, "glCheckFramebufferStatus: error %p", status);
			}

			post_processing_fbos_.push_back(PPFBO(textures, dbt, fbo));

			glBindTexture(GL_TEXTURE_2D, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		first_post_process_fbo_ = true;

		screen_quad_ = new Mesh();

		screen_quad_->SetVertices({
			VertexFormat{ Vector3(-1.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2{ 0.0f, 0.0f } },
			VertexFormat{ Vector3(-1.0f,  1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2{ 0.0f, 1.0f } },
			VertexFormat{ Vector3(1.0f,  1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2{ 1.0f, 1.0f } },
			VertexFormat{ Vector3(1.0f, -1.0f, 0.0f), Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f), Vector2{ 1.0f, 0.0f } }
		});

		screen_quad_->SetIndices({
			0, 1, 2,
			0, 2, 3
		});

		screen_quad_->Apply();

		glGenVertexArrays(1, &screen_quad_vao_);
		glBindVertexArray(screen_quad_vao_);
		glBindBuffer(GL_ARRAY_BUFFER, screen_quad_->GetVertexBuffers()[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, screen_quad_->GetVertexBuffers()[1]);

		Shader* shader = ContentManager::Instance()->GetShader("./assets/shaders/bloom.vsh", "./assets/shaders/bloom.fsh", "");

		GLsizei size = sizeof(VertexFormat);
		shader->GetAttribute("a_position")->SetAttributePointer(3, GL_FLOAT, GL_FALSE, size, reinterpret_cast<const void*>(offsetof(VertexFormat, Position)));
		shader->GetAttribute("a_normal")->SetAttributePointer(3, GL_FLOAT, GL_FALSE, size, reinterpret_cast<const void*>(offsetof(VertexFormat, Normal)));
		shader->GetAttribute("a_tangent")->SetAttributePointer(3, GL_FLOAT, GL_FALSE, size, reinterpret_cast<const void*>(offsetof(VertexFormat, Tangent)));
		shader->GetAttribute("a_bitangent")->SetAttributePointer(3, GL_FLOAT, GL_FALSE, size, reinterpret_cast<const void*>(offsetof(VertexFormat, Bitangent)));
		shader->GetAttribute("a_texture")->SetAttributePointer(2, GL_FLOAT, GL_FALSE, size, reinterpret_cast<const void*>(offsetof(VertexFormat, Texture)));

		glBindVertexArray(0);
	}

	//------------------------------------------------------------------------------------------------------
	PostProcessingFBO::~PostProcessingFBO()
	{

	}

	//------------------------------------------------------------------------------------------------------
	void PostProcessingFBO::PrepareFBO()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer_object_);
		GL_GET_ERROR();

		// set the buffers the shaders should draw (output) into
		GLenum attachment[] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, attachment);
		GL_GET_ERROR();

		// clear the buffers
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	//------------------------------------------------------------------------------------------------------
	void PostProcessingFBO::PreparePostProcessing()
	{
		last_effect_ = nullptr;
		glBindVertexArray(screen_quad_vao_);
	}

	//------------------------------------------------------------------------------------------------------
	void PostProcessingFBO::ApplyPostProcessEffect(PostProcessEffect* effect)
	{
		PPFBO ppfbo = post_processing_fbos_[0];
		if (first_post_process_fbo_ == false)
		{
			ppfbo = post_processing_fbos_[1];
		}
		
		glBindFramebuffer(GL_FRAMEBUFFER, ppfbo.post_processing_object_);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		BB_ASSERT(effect != nullptr, "Effect cannot be null");

		GLenum attachment[] = { 
			GL_COLOR_ATTACHMENT0,
			GL_COLOR_ATTACHMENT1,
			GL_COLOR_ATTACHMENT2,
			GL_COLOR_ATTACHMENT3,
			GL_COLOR_ATTACHMENT4,
			GL_COLOR_ATTACHMENT5,
			GL_COLOR_ATTACHMENT6,
			GL_COLOR_ATTACHMENT7,
			GL_COLOR_ATTACHMENT8,
			GL_COLOR_ATTACHMENT9 
		};
		glDrawBuffers(effect->GetNumberOfOutputs(), attachment);

		Shader* shader = effect->GetShader();

		glUseProgram(effect->GetShader()->GetProgram());
		ShaderUniform* fbt = effect->GetFrameBufferTexture();
		ShaderUniform* dbt = effect->GetDepthBufferTexture();

		if (fbt->IsValid())
		{
			ASSERT(fbt->GetType() == GL_SAMPLER_2D);
			glActiveTexture(GL_TEXTURE0 + fbt->GetSampler());
			GL_GET_ERROR();
			glBindTexture(GL_TEXTURE_2D, frame_buffer_texture_);
			GL_GET_ERROR();
			glUniform1i(fbt->GetLocation(), fbt->GetSampler());
			GL_GET_ERROR();
		}

		if (dbt->IsValid())
		{
			ASSERT(dbt->GetType() == GL_SAMPLER_2D);
			glActiveTexture(GL_TEXTURE0 + dbt->GetSampler());
			GL_GET_ERROR();
			glBindTexture(GL_TEXTURE_2D, depth_buffer_texture_);
			GL_GET_ERROR();
			glUniform1i(dbt->GetLocation(), dbt->GetSampler());
			GL_GET_ERROR();
		}

		if (last_effect_ != nullptr)
		{
			for (int i = 0; i < last_effect_->GetNumberOfOutputs(); i++)
			{
				ShaderUniform* input = shader->GetUniform("u_input_texture" + std::to_string(i));
				if (input->IsValid())
				{
					ASSERT(input->GetType() == GL_SAMPLER_2D);
					glActiveTexture(GL_TEXTURE0 + input->GetSampler());
					GL_GET_ERROR();
					glBindTexture(GL_TEXTURE_2D, post_processing_fbos_[first_post_process_fbo_ == true ? 1 : 0].post_processing_textures_[i]);
					GL_GET_ERROR();
					glUniform1i(input->GetLocation(), input->GetSampler());
					GL_GET_ERROR();
				}
			}
		}

		const void* first_index = reinterpret_cast<const void*>(0);
		glDrawElements(GL_TRIANGLES, screen_quad_->GetIndexCount(), GL_UNSIGNED_SHORT, first_index);
		GL_GET_ERROR();

		last_effect_ = effect;
		first_post_process_fbo_ = !first_post_process_fbo_;
	}

	//------------------------------------------------------------------------------------------------------
	void PostProcessingFBO::EndPostProcessing()
	{
		Shader* shader = ContentManager::Instance()->GetShader("./assets/shaders/default.vsh", "./assets/shaders/default.fsh", "");
		shader->Activate();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		GL_GET_ERROR();
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		GL_GET_ERROR();
		glBindVertexArray(screen_quad_vao_);
		GL_GET_ERROR();

		ShaderUniform* fbt = shader->GetUniform("u_material_diffuse_map");

		if (fbt->IsValid())
		{
			ASSERT(fbt->GetType() == GL_SAMPLER_2D);
			glActiveTexture(GL_TEXTURE0 + fbt->GetSampler());
			GL_GET_ERROR();
			glBindTexture(GL_TEXTURE_2D, post_processing_fbos_[first_post_process_fbo_ == true ? 1 : 0].post_processing_textures_[0]);
			GL_GET_ERROR();
			glUniform1i(fbt->GetLocation(), fbt->GetSampler());
			GL_GET_ERROR();
		}

		const void* first_index = reinterpret_cast<const void*>(0);
		glDrawElements(GL_TRIANGLES, screen_quad_->GetIndexCount(), GL_UNSIGNED_SHORT, first_index);
		GL_GET_ERROR();

		glBindVertexArray(0);
		GL_GET_ERROR();
	}
}
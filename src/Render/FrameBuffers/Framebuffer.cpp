#include "Framebuffer.h"

Framebuffer::Framebuffer(GLsizei samples) {
	glGenFramebuffers(1, &frameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);

	//generating color texture
	glGenTextures(1, &colorTextureID);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, colorTextureID);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

	//attaching texture to framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, colorTextureID, 0);

	//creating rendering buffer
	glGenRenderbuffers(1, &renderBufferID);
	glBindRenderbuffer(GL_RENDERBUFFER, renderBufferID);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//attaching render buffer to framebuffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBufferID);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer is not complete\n";
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	//creating FBO for stroing results from first frame buffer
	glGenFramebuffers(1, &intermidiateFrameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, intermidiateFrameBufferID);

	glGenTextures(1, &intermidiateTextureID);
	glBindTexture(GL_TEXTURE_2D, intermidiateTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, intermidiateTextureID, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer is not complete\n";
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//position, tex coord
	GLfloat vertices[] = {
		-1.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	};

	GLuint indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	BufferLayout layout = {
		{AttributeDataType::Float2, 2},	//Position
		{AttributeDataType::Float2, 2} //Tex coords
	};

	VAO = std::make_unique<VertexArray>();

	std::unique_ptr<VertexBuffer> VBO = std::make_unique<VertexBuffer>(vertices, sizeof(GLfloat) * 4 * 4, GL_STATIC_DRAW, layout);
	VAO->AddVertexBuffer(std::move(VBO));

	std::unique_ptr<IndexBuffer> IBO = std::make_unique<IndexBuffer>(indices, 6, GL_STATIC_DRAW);
	VAO->SetIndexBuffer(std::move(IBO));
}

Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(1, &frameBufferID);
	glDeleteTextures(1, &colorTextureID);
	glDeleteRenderbuffers(1, &renderBufferID);
}

void Framebuffer::Bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, frameBufferID);
}

void Framebuffer::Unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Draw() {
	Renderer::Instance().Clear(ClearType::Color);

	VAO->Bind();
	glDisable(GL_DEPTH_TEST);

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, intermidiateTextureID);

	Renderer::Instance().Draw(VAO->GetIBOCount());

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glEnable(GL_DEPTH_TEST);
}

void Framebuffer::BlitMultisampledBuffer() const {
	glBindFramebuffer(GL_READ_FRAMEBUFFER, frameBufferID);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermidiateFrameBufferID);
	glBlitFramebuffer(0, 0, Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT, 0, 0, Game::WINDOW_WIDTH, Game::WINDOW_HEIGHT, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}
#include "Text.h"

Text::Text(const std::string fontPath, float fontScale, glm::vec3 fontColor, unsigned int height, unsigned int width) :
	fontScale(fontScale), fontColor(fontColor), width(width), height(height) {
	
	if (FT_Init_FreeType(&ftLibrary)) {
		std::cout << "Could not init FreeType library\n";
		return;
	}

	if (FT_New_Face(ftLibrary, fontPath.c_str(), 0, &face)) {
		std::cout << "Failed to load font\n";
		return;
	}

	FT_Set_Pixel_Sizes(face, width, height);
	LoadCharacters();

	FT_Done_Face(face);
	FT_Done_FreeType(ftLibrary);

	VAO = std::make_unique<VertexArray>();

	BufferLayout layout = {
		{AttributeDataType::Float2, 2},	//Position
		{AttributeDataType::Float2, 2} //Tex coords
	};

	GLuint indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	//quad needs 6 vertices of 4 floats
	std::unique_ptr<VertexBuffer> VBO = std::make_unique<VertexBuffer>(nullptr, sizeof(GLfloat) * 4 * 4, GL_DYNAMIC_DRAW, layout);
	VAO->AddVertexBuffer(std::move(VBO));

	std::unique_ptr<IndexBuffer> IBO = std::make_unique<IndexBuffer>(indices, 6, GL_STATIC_DRAW);
	VAO->SetIndexBuffer(std::move(IBO));
	VAO->Unbind();
}

void Text::Draw(std::shared_ptr<Shader> shader, const std::string text, float x, float y) {
	shader->SetUniform("uTextColor", fontColor);
	unsigned char c;

	for (int i = 0; i < text.size(); i++) {
		c = text[i];

		glm::ivec2 size = characters[c]->GetSize();
		glm::ivec2 bearing = characters[c]->GetBearing();
		unsigned int advance = characters[c]->GetAdvance();

		float xpos = x + bearing.x * fontScale;
		float ypos = y - (size.y - bearing.y) * fontScale;

		float width = size.x * fontScale;
		float height = size.y * fontScale;

		//position, tex coords
		GLfloat vertices[4][4] = {
			{xpos, ypos + height, 0.0f, 0.0f},
			{xpos, ypos, 0.0f, 1.0f},
			{xpos + width, ypos, 1.0f, 1.0f},
			{xpos + width, ypos + height, 1.0f, 0.0f},
		};

		characters[c]->Bind(0);
		VAO->BindVBO(0);
		VAO->ChangeVBOData(0, sizeof(vertices), vertices);
		VAO->Bind();
		Renderer::Instance().Draw(VAO->GetIBOCount());
		x += (advance >> 6) * fontScale;
	}

	VAO->Unbind();
	characters[0]->Unbind();
}

void Text::SetFontScale(float scale) {
	fontScale = scale;
}

void Text::SetFontColor(glm::vec3 color) {
	fontColor = color;
}

void Text::LoadCharacters() {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned char c = 0; c < CHAR_NUM; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "Failed to load Glyph: " << c << "\n";
			continue;
		}
		
		std::unique_ptr<Character> character = std::make_unique<Character>(
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x,
			face->glyph->bitmap.buffer);
		characters[c] = std::move(character);
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
}
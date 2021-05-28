#include "Text.h"

Text::Text(const std::string fontPath, float fontScale, glm::vec4 fontColor, unsigned int height, unsigned int width) :
	fontScale(fontScale), fontColor(fontColor), height(height) {
	
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

	std::unique_ptr<VertexBuffer> VBO = std::make_unique<VertexBuffer>(nullptr, sizeof(GLfloat) * 4 * 4, GL_DYNAMIC_DRAW, layout);
	VAO->AddVertexBuffer(std::move(VBO));

	std::unique_ptr<IndexBuffer> IBO = std::make_unique<IndexBuffer>(indices, 6, GL_STATIC_DRAW);
	VAO->SetIndexBuffer(std::move(IBO));
	VAO->Unbind();
}

void Text::Draw(std::shared_ptr<Shader> shader, const std::string& text, float x, float y) {
	shader->SetUniform("uTextColor", fontColor);
	
	unsigned char c;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
	float xpos;
	float ypos;
	float width;
	float height;

	for (int i = 0; i < text.size(); i++) {
		c = text[i];

		size = characters[c]->GetSize();
		bearing = characters[c]->GetBearing();
		advance = characters[c]->GetAdvance();

		xpos = x + bearing.x * fontScale;
		ypos = y - (size.y - bearing.y) * fontScale;

		width = size.x * fontScale;
		height = size.y * fontScale;

		vertices[0][0] = xpos;
		vertices[0][1] = ypos + height;
		vertices[1][0] = xpos;
		vertices[1][1] = ypos;
		vertices[2][0] = xpos + width;
		vertices[2][1] = ypos;
		vertices[3][0] = xpos + width;
		vertices[3][1] = ypos + height;

		characters[c]->Bind(0);
		VAO->BindVBO(0);
		VAO->ChangeVBOData(0, sizeof(vertices), vertices);
		VAO->Bind();
		Renderer::Instance().Draw(VAO->GetIBOCount());
		x += (advance >> 6) * fontScale;
	}

	characters[0]->Unbind();
}

void Text::SetFontScale(float scale) {
	fontScale = scale;
}

void Text::SetFontColor(glm::vec4 color) {
	fontColor = color;
}

unsigned int Text::GetWidth() const {
	return width;
}

unsigned int Text::GetHeight() const {
	return height;
}

void Text::LoadCharacters() {
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for (unsigned char c = 32; c <= 122; c++) {
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

		if (c == 32) {
			c = 47;
		}
		else if (c == 58) {
			c = 64;
		}
		else if (c == 90) {
			c = 96;
		}
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	width = face->glyph->bitmap.width;
}
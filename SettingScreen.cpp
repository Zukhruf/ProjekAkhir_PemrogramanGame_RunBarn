#include "SettingScreen.h"

SettingScreen::SettingScreen()
{
}

SettingScreen::~SettingScreen()
{
}

void SettingScreen::Init()
{
	BuildBtnSetting();
	this->programSetting = BuildShader("shader.vert", "shader.frag");
	this->programSetting2 = BuildShader("spriteAnim.vert", "spriteAnim.frag");
	InputMapping("SelectButton", SDLK_RETURN);
	InputMapping("NextButton", SDLK_DOWN);
	InputMapping("PrevButton", SDLK_UP);
}

void SettingScreen::DeInit()
{
}

void SettingScreen::Update(float deltaTime)
{
}

void SettingScreen::Render()
{
	DrawBtnSetting();
}

void SettingScreen::BuildBtnSetting()
{
	string btnSetting[NUM_BTN_SETTING] = { "BGM.png", "SFX.png" };
	string btnSettingHover[NUM_BTN_SETTING] = { "BGM_HOVER.png", "SFX_HOVER.png" };
	string btnSettingChoosen[NUM_BTN_SETTING] = { "BGM_CHOOSEN.png", "SFX_CHOOSEN.png" };

	glGenTextures(2, &BtnSettingTex[0]);
	glGenTextures(2, &BtnSettingHoverTex[0]);
	glGenTextures(2, &BtnSettingChoosenTex[0]);

	for (int i = 0; i < NUM_BTN_SETTING; i++)
	{
		glBindTexture(GL_TEXTURE_2D, BtnSettingTex[i]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Load, create texture 
		int width, height;
		unsigned char* image = SOIL_load_image(btnSetting[i].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

										 // Set up vertex data (and buffer(s)) and attribute pointers
		BtnSettingWidth[i] = (float)width;
		BtnSettingWidth[i] = (float)height;

		// Load and create a texture 
		glBindTexture(GL_TEXTURE_2D, BtnSettingHoverTex[i]); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		image = SOIL_load_image(btnSettingHover[i].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

										 // Set up vertex data (and buffer(s)) and attribute pointers
		BtnSettingHoverWidth[i] = (float)width;
		BtnSettingHoverHeight[i] = (float)height;

		GLfloat vertices[] = {
			// Positions	// Texture Coords
			1,  1,			1.0f, 1.0f, // Bottom Right
			1,  0,			1.0f, 0.0f, // Top Right
			0,  0,			0.0f, 0.0f, // Top Left
			0,  1,			0.0f, 1.0f  // Bottom Left 
		};


		glGenVertexArrays(1, &BtnSettingVAO);
		glGenBuffers(1, &BtnSettingVBO);
		glBindVertexArray(BtnSettingVAO);
		glBindBuffer(GL_ARRAY_BUFFER, BtnSettingVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

void SettingScreen::DrawBtnSetting()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind Textures using texture units
	int texture_id[NUM_BTN_SETTING] = { GL_TEXTURE1, GL_TEXTURE2 };
	// Activate shader
	UseShader(this->programSetting);
	glUniform1i(glGetUniformLocation(this->programSetting, "text"), 0);

	glBindVertexArray(BtnSettingVAO);
	for (int i = 0; i < NUM_BTN_SETTING; i++) {

		glActiveTexture(texture_id[i]);
		glBindTexture(GL_TEXTURE_2D, (BtnActive == i) ? BtnSettingHoverTex[i] : BtnSettingTex[i]);
		glUniform1i(glGetUniformLocation(this->programSetting, "ourTexture"), i + 1);

		mat4 model;
		model = translate(model, vec3((GetScreenWidth()) / 2.8f, (i + 1) * 115, 0.0f));
		model = scale(model, vec3(BtnSettingWidth[i] / 2, BtnSettingHeight[i] / 2, 1));
		glUniformMatrix4fv(glGetUniformLocation(this->programSetting, "model"), 1, GL_FALSE, value_ptr(model));

		glDrawArrays(GL_QUADS, 0, 4);
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
}


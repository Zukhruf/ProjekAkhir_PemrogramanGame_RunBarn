#include "MainMenuScreen.h"

MainMenuScreen::MainMenuScreen()
{
}

MainMenuScreen::~MainMenuScreen()
{
}

void MainMenuScreen::Init()
{

	BuildLogo();
	BuildBtn();
	BuildBtnSetting();
	BuildLeaderboard();
	BuildCredit();
	BuildPlayer();
	BuildBG();
	BuildVacuum();
	BuildObstacle();
	this->program = BuildShader("shader.vert", "shader.frag");
	InputMapping("SelectButton", SDLK_RETURN);
	InputMapping("NextButton", SDLK_DOWN);
	InputMapping("PrevButton", SDLK_UP);
	InputMapping("BackButton", SDLK_ESCAPE);
	this->status = Status::MAIN_MENU;
}

void MainMenuScreen::DeInit()
{
}

void MainMenuScreen::Update(float deltaTime)
{
	if (Status::MAIN_MENU == status)
	{
		if (IsKeyDown("SelectButton")) {
			if (ActiveButtonIndex == 0) {
				this->status = Status::RUN;
			}
			if (ActiveButtonIndex == 1) {
				this->status = Status::LEADERBOARD;
			}
			if (ActiveButtonIndex == 2) {
				this->status = Status::SETTING;
			}
			if (ActiveButtonIndex == 3) {
				this->status = Status::CREDIT;
			}
			if (ActiveButtonIndex == 4) {
				SDL_Quit();
				exit(0);
			}
		}

		if (IsKeyUp("NextButton")) {
			if (ActiveButtonIndex < NUM_BUTTON - 1) {
				ActiveButtonIndex = ActiveButtonIndex + 1;
				SDL_Delay(150);
			}
		}

		if (IsKeyUp("PrevButton")) {
			if (ActiveButtonIndex > 0) {
				ActiveButtonIndex = ActiveButtonIndex - 1;
				SDL_Delay(150);
			}
		}
		if (IsKeyUp("BackButton")) {
			this->status = Status::MAIN_MENU;
		}
	}
	if (Status::RUN == status)
	{
		UpdatePlayerSpriteAnim(deltaTime);
		UpdateObstacleSpriteAnim(deltaTime);

		ControlObstacleSprite(deltaTime);
		ControlPlayerSprite(deltaTime);
		if (IsKeyUp("BackButton")) {
			this->status = Status::MAIN_MENU;
		}
	}
	if (Status::LEADERBOARD == status)
	{
		if (IsKeyUp("BackButton")) {
			this->status = Status::MAIN_MENU;
		}
	}
	if (Status::SETTING == status)
	{
		if (IsKeyDown("SelectButton"))
		{
			if (BtnActive == 0)
			{

			}
			if (BtnActive == 1)
			{

			}
		}
		if (IsKeyUp("NextButton")) {
			if (BtnActive < NUM_BTN_SETTING - 1) {
				BtnActive = BtnActive + 1;
				SDL_Delay(150);
			}
		}

		if (IsKeyUp("PrevButton")) {
			if (BtnActive > 0) {
				BtnActive = BtnActive - 1;
				SDL_Delay(150);
			}
		}
		if (IsKeyUp("BackButton")) {
			this->status = Status::MAIN_MENU;
		}
	}
	if (Status::CREDIT == status)
	{
		if (IsKeyUp("BackButton")) {
			this->status = Status::MAIN_MENU;
		}
	}
}

void MainMenuScreen::Render()
{
	//Setting viewport
	glViewport(0, 0, GetScreenWidth(), GetScreenHeight());

	//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set the background color
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	// Set orthographic projection
	mat4 projection;
	projection = ortho(0.0f, static_cast<GLfloat>(GetScreenWidth()), static_cast<GLfloat>(GetScreenHeight()), 0.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(glGetUniformLocation(this->program, "projection"), 1, GL_FALSE, value_ptr(projection));


	if (Status::MAIN_MENU == status)
	{
		DrawLogo();
		DrawBtn();
	}
	if (Status::RUN == status)
	{
		DrawBG();
		mat4 projection4;
		projection4 = ortho(0.0f, static_cast<GLfloat>(GetScreenWidth()), static_cast<GLfloat>(GetScreenHeight()), 0.0f, -1.0f, 1.0f);
		glUniformMatrix4fv(glGetUniformLocation(this->program4, "projection"), 1, GL_FALSE, value_ptr(projection4));

		DrawObstacle();
		mat4 projection2;
		projection2 = ortho(0.0f, static_cast<GLfloat>(GetScreenWidth()), static_cast<GLfloat>(GetScreenHeight()), 0.0f, -1.0f, 1.0f);
		glUniformMatrix4fv(glGetUniformLocation(this->program2, "projection"), 1, GL_FALSE, value_ptr(projection2));

		DrawPlayer();
		mat4 projection3;
		projection3 = ortho(0.0f, static_cast<GLfloat>(GetScreenWidth()), static_cast<GLfloat>(GetScreenHeight()), 0.0f, -1.0f, 1.0f);
		glUniformMatrix4fv(glGetUniformLocation(this->program3, "projection"), 1, GL_FALSE, value_ptr(projection3));

		DrawVacuum();
		mat4 projection5;
		projection5 = ortho(0.0f, static_cast<GLfloat>(GetScreenWidth()), static_cast<GLfloat>(GetScreenHeight()), 0.0f, -1.0f, 1.0f);
		glUniformMatrix4fv(glGetUniformLocation(this->program5, "projection"), 1, GL_FALSE, value_ptr(projection5));
	}
	if (Status::SETTING == status)
	{
		DrawBtnSetting();
	}
	if (Status::LEADERBOARD == status)
	{
		DrawLeaderboard();
	}
	if (Status::CREDIT == status)
	{
		DrawCredit();
	}
}

bool MainMenuScreen::IsCollided(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2)
{
	return (x1 < x2 + width2 && x1 + width1 > x2 && y1 < y2 + height2 && y1 + height1 > y2);
}

void MainMenuScreen::BuildObstacle()
{
	this->program2 = BuildShader("spriteAnim.vert", "spriteAnim.frag");
	UseShader(this->program2);

	// Load and create a texture 
	glGenTextures(1, &textureObstacle);
	glBindTexture(GL_TEXTURE_2D, textureObstacle); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

												   // Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load, create texture 
	int width, height;
	unsigned char* image = SOIL_load_image("crate.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

									 // Set up vertex data (and buffer(s)) and attribute pointers
	frame_width = ((float)width) / NUM_FRAMES;
	frame_height = (float)height;
	GLfloat vertices[] = {
		// Positions   // Colors           // Texture Coords
		1,  1, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // Bottom Right
		1,  0, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // Top Right
		0,  0, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Top Left
		0,  1, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Bottom Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 3, 2, 1
	};

	glGenVertexArrays(1, &VAOObstacle);
	glGenBuffers(1, &VBOObstacle);
	glGenBuffers(1, &EBOObstacle);

	glBindVertexArray(VAOObstacle);

	glBindBuffer(GL_ARRAY_BUFFER, VBOObstacle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOObstacle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

						  // Set orthographic projection
	mat4 projection2;
	projection2 = ortho(0.0f, static_cast<GLfloat>(GetScreenWidth()), static_cast<GLfloat>(GetScreenHeight()), 0.0f, -1.0f, 1.0f);
	UseShader(this->program2);
	glUniformMatrix4fv(glGetUniformLocation(this->program2, "projection"), 1, GL_FALSE, value_ptr(projection2));
	xpos = GetScreenWidth();
	ypos = GetScreenHeight() / 2;
}
void MainMenuScreen::DrawObstacle()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE10);
	glBindTexture(GL_TEXTURE_2D, textureObstacle);

	// Activate shader
	UseShader(this->program2);
	glUniform1i(glGetUniformLocation(this->program2, "ourTexture"), 10);

	mat4 model;
	// Translate sprite along x-axis
	model = translate(model, vec3(xpos, ypos, 0.0f));
	// Scale sprite 
	model = scale(model, vec3(frame_width * 8, frame_height, 1));
	glUniformMatrix4fv(glGetUniformLocation(this->program2, "model"), 1, GL_FALSE, value_ptr(model));

	// Draw sprite
	glBindVertexArray(VAOObstacle);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	glDisable(GL_BLEND);
}

void MainMenuScreen::BuildPlayer()
{
	this->program3 = BuildShader("playerSprite.vert", "playerSprite.frag");
	UseShader(this->program3);
	glUniform1f(glGetUniformLocation(this->program3, "n"), 1.0f / NUM_FRAMES);

	// Load and create a texture 
	glGenTextures(1, &texturePlayer);
	glBindTexture(GL_TEXTURE_2D, texturePlayer); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

												 // Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load, create texture 
	int widthPlayer, heightPlayer;
	unsigned char* image = SOIL_load_image("hori.png", &widthPlayer, &heightPlayer, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthPlayer, heightPlayer, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

									 // Set up vertex data (and buffer(s)) and attribute pointers
	frame_widthPlayer = ((float)widthPlayer) / NUM_FRAMES;
	frame_heightPlayer = (float)heightPlayer;
	GLfloat vertices[] = {
		// Positions   // Colors           // Texture Coords
		1,  1, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Bottom Right
		1,  0, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Top Right
		0,  0, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Top Left
		0,  1, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Bottom Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 3, 2, 1
	};

	glGenVertexArrays(1, &VAOPlayer);
	glGenBuffers(1, &VBOPlayer);
	glGenBuffers(1, &EBOPlayer);

	glBindVertexArray(VAOPlayer);

	glBindBuffer(GL_ARRAY_BUFFER, VBOPlayer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOPlayer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

						  // Set orthographic projection
	mat4 projection3;
	projection3 = ortho(0.0f, static_cast<GLfloat>(GetScreenWidth()), static_cast<GLfloat>(GetScreenHeight()), 0.0f, -1.0f, 1.0f);
	UseShader(this->program3);
	glUniformMatrix4fv(glGetUniformLocation(this->program3, "projection"), 1, GL_FALSE, value_ptr(projection3));
	xposPlayer = (GetScreenWidth() - frame_widthPlayer) / 2;
	yposGroundPlayer = GetScreenHeight() - frame_heightPlayer;
	yposPlayer = yposGroundPlayer;
	gravityPlayer = 0.05f;
	xVelocityPlayer = 0.1f;

	// Add input mapping
	// to offer input change flexibility you can save the input mapping configuration in a configuration file (non-hard code) !
	InputMapping("Move Right", SDLK_RIGHT);
	InputMapping("Move Left", SDLK_LEFT);
	InputMapping("Move Right", SDLK_d);
	InputMapping("Move Left", SDLK_a);
	InputMapping("Move Right", SDL_BUTTON_RIGHT);
	InputMapping("Move Left", SDL_BUTTON_LEFT);
	InputMapping("Jump", SDLK_SPACE);
}
void MainMenuScreen::DrawPlayer()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE11);
	glBindTexture(GL_TEXTURE_2D, texturePlayer);
	UseShader(this->program3);
	glUniform1i(glGetUniformLocation(this->program3, "ourTexture"), 11);

	// set flip
	glUniform1i(glGetUniformLocation(this->program3, "flip"), flip);
	mat4 model;
	// Translate sprite along x-axis
	model = translate(model, vec3(xposPlayer, yposPlayer, 0.0f));
	// Scale sprite 
	model = scale(model, vec3(frame_widthPlayer, frame_heightPlayer, 1));
	glUniformMatrix4fv(glGetUniformLocation(this->program3, "model"), 1, GL_FALSE, value_ptr(model));

	// Draw sprite
	glBindVertexArray(VAOPlayer);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	glDisable(GL_BLEND);
}

void MainMenuScreen::UpdateObstacleSpriteAnim(float deltaTime)
{
	// Update animation
	frame_dur += deltaTime;

	if (frame_dur > FRAME_DUR) {
		frame_dur = 0;
		if (frame_idx == NUM_FRAMES - 1) frame_idx = 0;  else frame_idx++;

		// Pass frameIndex to shader
		GLint location = glGetUniformLocation(this->program2, "frameIndex");
		UseShader(this->program2);
		glUniform1i(location, frame_idx);
	}
}
void MainMenuScreen::ControlObstacleSprite(float deltaTime)
{
	xpos += deltaTime * -0.3f;

	if (xpos < 0) {
		xpos = GetScreenWidth();
		ypos = (GetScreenHeight() / 2) + (rand() % (GetScreenHeight() / 2));
		if (ypos > GetScreenHeight()) {
			ypos = GetScreenHeight() - 100;
		}
		//ypos = 30 + rand() % (GetScreenHeight() + 400);
	}
	mat4 model;
	model = translate(model, vec3(xpos - frame_width, ypos, 0.0f));

	// Rotate sprite at origin
	model = translate(model, vec3(0.5f * frame_width, 0.5f * frame_height, 0.0f));
	model = translate(model, glm::vec3(-0.5f * frame_width, -0.5f * frame_height, 0.0f));

	// Scale sprite 
	model = scale(model, vec3(frame_width, frame_height, 1));


	UseShader(this->program2);
	glUniformMatrix4fv(glGetUniformLocation(this->program2, "model"), 1, GL_FALSE, value_ptr(model));
}

void MainMenuScreen::UpdatePlayerSpriteAnim(float deltaTime)
{
	// Update animation
	frame_dur += deltaTime;

	if (frame_dur > FRAME_DUR) {
		frame_dur = 0;
		if (frame_idx == NUM_FRAMES - 1) frame_idx = 0;  else frame_idx++;

		// Pass frameIndex to shader
		GLint location = glGetUniformLocation(this->program3, "frameIndex");
		UseShader(this->program3);
		glUniform1i(location, frame_idx);
	}
}
void MainMenuScreen::ControlPlayerSprite(float deltaTime)
{
	walk_anim = false;
	oldxposPlayer = xposPlayer;
	oldyposPlayer = yposPlayer;
	if (!IsKeyDown("Move Right")) {
		xposPlayer -= deltaTime * xVelocityPlayer * 2;
		flip = 0;
		walk_anim = true;
	}

	if (xposPlayer <= 10) {
		xposPlayer = 10;
	}

	if (IsKeyDown("Move Right")) {
		if (xposPlayer >= GetScreenWidth() / 2) {
			xposPlayer = GetScreenWidth() / 2;
		}
		xposPlayer += deltaTime * xVelocityPlayer;
		flip = 0;
		walk_anim = true;
	}

	if (IsKeyDown("Move Left")) {
		xposPlayer -= deltaTime * xVelocityPlayer;
		flip = 1;
		walk_anim = true;
	}

	if (IsKeyDown("Jump")) {
		if (onGround) {
			yVelocityPlayer = -5.0f;
			onGround = false;
		}
	}

	if (IsKeyUp("Jump")) {
		if (yVelocityPlayer < -6.0f) {
			yVelocityPlayer = -6.0f;
		}
	}

	yVelocityPlayer += gravityPlayer * deltaTime;
	yposPlayer += deltaTime * yVelocityPlayer;

	if (yposPlayer > yposGroundPlayer) {
		yposPlayer = yposGroundPlayer;
		yVelocityPlayer = 0;
		onGround = true;
	}

	//check collision between bart and crate
	if (IsCollided(xpos, ypos, frame_width, frame_height, xposPlayer, yposPlayer, frame_widthPlayer, frame_heightPlayer)) {
		xposPlayer = xpos - 70;
	}
}

void MainMenuScreen::BuildLogo()
{
	// Load and create a texture 
	string logo[NUM_LOGO] = { "Title.png" };

	glGenTextures(1, &LogoTexture[0]);

	for (int i = 0; i < NUM_LOGO; i++) {
		// Load and create a texture 
		glBindTexture(GL_TEXTURE_2D, LogoTexture[i]); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

													  // Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Load, create texture 
		int width, height;
		unsigned char* image = SOIL_load_image(logo[i].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

										 // Set up vertex data (and buffer(s)) and attribute pointers
		LogoWidth[i] = (float)width;
		LogoHeight[i] = (float)height; // Unbind texture when done, so we won't accidentily mess up our texture.

									   // Set up vertex data (and buffer(s)) and attribute pointers
		GLfloat vertices[] = {
			// Positions	// Texture Coords
			1,  1,			1.0f, 1.0f, // Bottom Right
			1,  0,			1.0f, 0.0f, // Top Right
			0,  0,			0.0f, 0.0f, // Top Left
			0,  1,			0.0f, 1.0f  // Bottom Left 
		};


		glGenVertexArrays(1, &LogoVAO);
		glGenBuffers(1, &LogoVBO);
		glBindVertexArray(LogoVAO);
		glBindBuffer(GL_ARRAY_BUFFER, LogoVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}
void MainMenuScreen::DrawLogo()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	int texture_id[NUM_LOGO] = { GL_TEXTURE0 };
	// Activate shader
	UseShader(this->program);
	glUniform1i(glGetUniformLocation(this->program, "text"), 0);

	glBindVertexArray(LogoVAO);
	for (int i = 0; i < NUM_LOGO; i++) {

		glActiveTexture(texture_id[i]);
		glBindTexture(GL_TEXTURE_2D, LogoTexture[i]);
		glUniform1i(glGetUniformLocation(this->program, "ourTexture"), i);

		mat4 model;
		model = translate(model, vec3((GetScreenWidth()) / 4.65f, (i) * 100, 0.0f));
		model = scale(model, vec3(LogoWidth[i], LogoHeight[i], 1));
		glUniformMatrix4fv(glGetUniformLocation(this->program, "model"), 1, GL_FALSE, value_ptr(model));

		glDrawArrays(GL_QUADS, 0, 4);
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);

}

void MainMenuScreen::BuildBtn()
{
	string buttons[NUM_BUTTON] = { "run_hover.png", "leaderboard_hover.png", "setting_hover.png", "credit_hover.png", "quit_hover.png" };
	string hover_buttons[NUM_BUTTON] = { "run.png", "leaderboard.png", "setting.png", "credit.png", "quit.png" };

	glGenTextures(5, &BtnTexture[0]);
	glGenTextures(5, &BtnHoverTexture[0]);

	for (int i = 0; i < NUM_BUTTON; i++) {
		// Load and create a texture 
		glBindTexture(GL_TEXTURE_2D, BtnTexture[i]); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

													 // Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Load, create texture 
		int width, height;
		unsigned char* image = SOIL_load_image(buttons[i].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

										 // Set up vertex data (and buffer(s)) and attribute pointers
		BtnWidth[i] = (float)width;
		BtnHeigt[i] = (float)height;

		// Load and create a texture 
		glBindTexture(GL_TEXTURE_2D, BtnHoverTexture[i]); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

														  // Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		image = SOIL_load_image(hover_buttons[i].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

										 // Set up vertex data (and buffer(s)) and attribute pointers
		BtnHoverWidth[i] = (float)width;
		BtnHoverHeight[i] = (float)height;
	}

	GLfloat vertices[] = {
		// Positions	// Texture Coords
		1,  1,			1.0f, 1.0f, // Bottom Right
		1,  0,			1.0f, 0.0f, // Top Right
		0,  0,			0.0f, 0.0f, // Top Left
		0,  1,			0.0f, 1.0f  // Bottom Left 
	};


	glGenVertexArrays(1, &BtnVAO);
	glGenBuffers(1, &BtnVBO);
	glBindVertexArray(BtnVAO);
	glBindBuffer(GL_ARRAY_BUFFER, BtnVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void MainMenuScreen::DrawBtn()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind Textures using texture units
	int texture_id[NUM_BUTTON] = { GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3, GL_TEXTURE4, GL_TEXTURE5 };
	// Activate shader
	UseShader(this->program);
	glUniform1i(glGetUniformLocation(this->program, "text"), 0);

	glBindVertexArray(BtnVAO);
	for (int i = 0; i < NUM_BUTTON; i++) {

		glActiveTexture(texture_id[i]);
		glBindTexture(GL_TEXTURE_2D, (ActiveButtonIndex == i) ? BtnHoverTexture[i] : BtnTexture[i]);
		glUniform1i(glGetUniformLocation(this->program, "ourTexture"), i + 1);

		mat4 model;
		model = translate(model, vec3((GetScreenWidth()) / 2.8f, (i + 1) * 115, 0.0f));
		model = scale(model, vec3(BtnWidth[i] / 2, BtnHeigt[i] / 2, 1));
		glUniformMatrix4fv(glGetUniformLocation(this->program, "model"), 1, GL_FALSE, value_ptr(model));

		glDrawArrays(GL_QUADS, 0, 4);
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
}

void MainMenuScreen::BuildBtnSetting()
{
	string btnSetting[NUM_BTN_SETTING] = { "bgm_hover.png", "sfx_hover.png" };
	string btnSettingHover[NUM_BTN_SETTING] = { "bgm.png", "sfx.png" };
	string btnSettingChoosen[NUM_BTN_SETTING] = { "bgm_choosen.png", "sfx_choosen.png" };

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

		// Load, create texture Btn Setting
		int width, height;
		unsigned char* image = SOIL_load_image(btnSetting[i].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

										 // Set up vertex data (and buffer(s)) and attribute pointers
		BtnSettingWidth[i] = (float)width;
		BtnSettingHeight[i] = (float)height;

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

		//Btn Setting Choosen Texture
		glBindTexture(GL_TEXTURE_2D, BtnSettingChoosenTex[i]); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		image = SOIL_load_image(btnSettingChoosen[i].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

										 // Set up vertex data (and buffer(s)) and attribute pointers
		BtnSettingChoosenWidth[i] = (float)width;
		BtnSettingChoosenHeight[i] = (float)height;

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
void MainMenuScreen::DrawBtnSetting()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind Textures using texture units
	int texture_id[NUM_BTN_SETTING] = { GL_TEXTURE6, GL_TEXTURE7 };
	// Activate shader
	UseShader(this->program);
	glUniform1i(glGetUniformLocation(this->program, "text"), 0);

	glBindVertexArray(BtnSettingVAO);
	for (int i = 0; i < NUM_BTN_SETTING; i++) {

		glActiveTexture(texture_id[i]);
		glBindTexture(GL_TEXTURE_2D, (BtnActive == i) ? BtnSettingHoverTex[i] : BtnSettingTex[i]);
		glUniform1i(glGetUniformLocation(this->program, "ourTexture"), i + 6);

		mat4 model;
		model = translate(model, vec3((GetScreenWidth()) / 3.25f, (i + 1) * 120, 0.0f));
		model = scale(model, vec3(BtnSettingWidth[i], BtnSettingHeight[i], 1));
		glUniformMatrix4fv(glGetUniformLocation(this->program, "model"), 1, GL_FALSE, value_ptr(model));

		glDrawArrays(GL_QUADS, 0, 4);
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
}

void MainMenuScreen::BuildLeaderboard()
{
	string leaderboardScreen[NUM_LEADERBOARD_SCREEN] = { "leaderboardScreen.png" };

	glGenTextures(1, &LeaderboardTex[0]);

	for (int i = 0; i < NUM_LEADERBOARD_SCREEN; i++) {
		// Load and create a texture 
		glBindTexture(GL_TEXTURE_2D, LeaderboardTex[i]); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

														 // Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Load, create texture 
		int width, height;
		unsigned char* image = SOIL_load_image(leaderboardScreen[i].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

										 // Set up vertex data (and buffer(s)) and attribute pointers
		LeaderboardWidth[i] = (float)width;
		LeaderboardHeight[i] = (float)height;
	}
	GLfloat vertices[] = {
		// Positions	// Texture Coords
		1,  1,			1.0f, 1.0f, // Bottom Right
		1,  0,			1.0f, 0.0f, // Top Right
		0,  0,			0.0f, 0.0f, // Top Left
		0,  1,			0.0f, 1.0f  // Bottom Left 
	};


	glGenVertexArrays(1, &LeaderboardVAO);
	glGenBuffers(1, &LeaderboardVBO);
	glBindVertexArray(LeaderboardVAO);
	glBindBuffer(GL_ARRAY_BUFFER, LeaderboardVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void MainMenuScreen::DrawLeaderboard()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind Textures using texture units
	int texture_id[NUM_LEADERBOARD_SCREEN] = { GL_TEXTURE8 };
	// Activate shader
	UseShader(this->program);
	glUniform1i(glGetUniformLocation(this->program, "text"), 0);

	glBindVertexArray(LeaderboardVAO);
	for (int i = 0; i < NUM_LEADERBOARD_SCREEN; i++) {

		glActiveTexture(texture_id[i]);
		glBindTexture(GL_TEXTURE_2D, LeaderboardTex[i]);
		glUniform1i(glGetUniformLocation(this->program, "ourTexture"), i + 8);

		mat4 model;
		model = translate(model, vec3((GetScreenWidth()) / 6.0f, (i + 1) * 0, 0.0f));
		model = scale(model, vec3((GetScreenWidth() / 1.5f), LeaderboardHeight[i], 1));
		glUniformMatrix4fv(glGetUniformLocation(this->program, "model"), 1, GL_FALSE, value_ptr(model));

		glDrawArrays(GL_QUADS, 0, 4);
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
}

void MainMenuScreen::BuildCredit()
{
	string creditScreen[NUM_CREDIT_SCREEN] = { "creditScreen.png" };

	glGenTextures(1, &CreditScreenTex[0]);

	for (int i = 0; i < NUM_CREDIT_SCREEN; i++) {
		// Load and create a texture 
		glBindTexture(GL_TEXTURE_2D, CreditScreenTex[i]); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

														  // Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Load, create texture 
		int width, height;
		unsigned char* image = SOIL_load_image(creditScreen[i].c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

										 // Set up vertex data (and buffer(s)) and attribute pointers
		CreditWidth[i] = (float)width;
		CreditHeight[i] = (float)height;
	}
	GLfloat vertices[] = {
		// Positions	// Texture Coords
		1,  1,			1.0f, 1.0f, // Bottom Right
		1,  0,			1.0f, 0.0f, // Top Right
		0,  0,			0.0f, 0.0f, // Top Left
		0,  1,			0.0f, 1.0f  // Bottom Left 
	};


	glGenVertexArrays(1, &CreditVAO);
	glGenBuffers(1, &CreditVBO);
	glBindVertexArray(CreditVAO);
	glBindBuffer(GL_ARRAY_BUFFER, CreditVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
void MainMenuScreen::DrawCredit()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind Textures using texture units
	int texture_id[NUM_CREDIT_SCREEN] = { GL_TEXTURE9 };
	// Activate shader
	UseShader(this->program);
	glUniform1i(glGetUniformLocation(this->program, "text"), 0);

	glBindVertexArray(CreditVAO);
	for (int i = 0; i < NUM_LEADERBOARD_SCREEN; i++) {

		glActiveTexture(texture_id[i]);
		glBindTexture(GL_TEXTURE_2D, CreditScreenTex[i]);
		glUniform1i(glGetUniformLocation(this->program, "ourTexture"), i + 9);

		mat4 model;
		model = translate(model, vec3((GetScreenWidth()) / 3.5f, (i + 1) * 0, 0.0f));
		model = scale(model, vec3(CreditWidth[i] / 1.52f, CreditHeight[i] / 1.52f, 1));
		glUniformMatrix4fv(glGetUniformLocation(this->program, "model"), 1, GL_FALSE, value_ptr(model));

		glDrawArrays(GL_QUADS, 0, 4);
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
}

void MainMenuScreen::BuildBG()
{
	this->program4 = BuildShader("sprite.vert", "sprite.frag");
	UseShader(this->program4);

	// Load and create a texture 
	glGenTextures(1, &BGTexture);
	glBindTexture(GL_TEXTURE_2D, BGTexture); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

											 // Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Load, create texture and generate mipmaps
	int width, height;

	unsigned char* image = SOIL_load_image("background2.png", &width, &height, 0, SOIL_LOAD_RGBA);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

									 // Set up vertex data (and buffer(s)) and attribute pointers
									 // Set up vertex data (and buffer(s)) and attribute pointers
	float x = width * 0.8f / height;
	float y = 1.0f;
	GLfloat vertices[] = {
		// Positions   // Colors           // Texture Coords
		x,  y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top Right
		x, -y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom Right
		-x, -y, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
		-x,  y, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Top Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 3, 2, 1
	};

	glGenVertexArrays(1, &BGVao);
	glGenBuffers(1, &BGVbo);
	glGenBuffers(1, &BGEbo);

	glBindVertexArray(BGVao);

	glBindBuffer(GL_ARRAY_BUFFER, BGVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BGEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO
}
void MainMenuScreen::DrawBG()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE12);
	glBindTexture(GL_TEXTURE_2D, BGTexture);

	// Activate shader
	UseShader(this->program4);
	glUniform1i(glGetUniformLocation(this->program4, "ourTexture"), 12);

	// Draw sprite
	glBindVertexArray(BGVao);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	glDisable(GL_BLEND);
}

int main(int argc, char** argv) {

	Layar::Screen &mainScreen = MainMenuScreen();
	mainScreen.Start("RUN BARN v1.0", 1080, 720, false, WindowFlag::WINDOWED, 60, 1);

	return 0;
}

void MainMenuScreen::BuildVacuum()
{
	this->program5 = BuildShader("crateSprite.vert", "crateSprite.frag");
	UseShader(this->program5);

	// Load and create a texture 
	glGenTextures(1, &textureVacuum);
	glBindTexture(GL_TEXTURE_2D, textureVacuum); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

												 // Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Load, create texture 
	int width, height;
	unsigned char* image = SOIL_load_image("vacuum.png", &width, &height, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

									 // Set up vertex data (and buffer(s)) and attribute pointers
	frame_widthVacuum = ((float)width) / NUM_FRAMES;
	frame_heightVacuum = (float)height;
	GLfloat vertices[] = {
		// Positions   // Colors           // Texture Coords
		1,  1, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Bottom Right
		1,  0, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Top Right
		0,  0, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Top Left
		0,  1, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Bottom Left 
	};

	GLuint indices[] = {  // Note that we start from 0!
		0, 3, 2, 1
	};

	glGenVertexArrays(1, &VAOVacuum);
	glGenBuffers(1, &VBOVacuum);
	glGenBuffers(1, &EBOVacuum);

	glBindVertexArray(VAOVacuum);

	glBindBuffer(GL_ARRAY_BUFFER, VBOVacuum);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOVacuum);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	// TexCoord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO

						  // Set orthographic projection
	mat4 projection5;
	projection5 = ortho(0.0f, static_cast<GLfloat>(GetScreenWidth()), static_cast<GLfloat>(GetScreenHeight()), 0.0f, -1.0f, 1.0f);
	UseShader(this->program5);
	glUniformMatrix4fv(glGetUniformLocation(this->program5, "projection"), 1, GL_FALSE, value_ptr(projection5));
	xposVacuum = 10;
	yposVacuum = 340;

}
void MainMenuScreen::DrawVacuum()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE13);
	glBindTexture(GL_TEXTURE_2D, textureVacuum);
	UseShader(this->program5);
	glUniform1i(glGetUniformLocation(this->program5, "ourTexture"), 13);

	mat4 model;
	// Translate sprite along x-axis
	model = translate(model, vec3(xposVacuum, yposVacuum, 0.0f));
	// Scale sprite 
	model = scale(model, vec3(frame_widthVacuum * 8, frame_heightVacuum, 1));
	glUniformMatrix4fv(glGetUniformLocation(this->program5, "model"), 1, GL_FALSE, value_ptr(model));

	// Draw sprite
	glBindVertexArray(VAOVacuum);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	glDisable(GL_BLEND);
}
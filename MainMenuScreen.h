#ifndef MAINMENUSCREEN_H
#define MAINMENUSCREEN_H


#include <SOIL/SOIL.h>

#include <SDL/SDL_mixer.h>
#include <SDL/SDL_thread.h>

#include <ft2build.h>
#include <freetype/freetype.h>
#include <map>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "Screen.h"

using namespace glm;

#define NUM_BUTTON 5
#define NUM_LOGO 1
#define NUM_BTN_SETTING 2
#define NUM_FRAMES 8
#define NUM_LEADERBOARD_SCREEN 1
#define NUM_CREDIT_SCREEN 1
#define FRAME_DUR 80

class MainMenuScreen :
	public Layar::Screen {
public:
	float frame_width = 0, frame_height = 0, frame_widthPlayer = 0, frame_heightPlayer = 0, frame_widthVacuum = 0, frame_heightVacuum = 0;
	MainMenuScreen();
	~MainMenuScreen();
	virtual void Init();
	virtual void DeInit();
	virtual void Update(float deltaTime);
	virtual void Render();

private:
	GLuint BtnTexture[NUM_BUTTON], BtnHoverTexture[NUM_BUTTON], LogoTexture[NUM_LOGO],
		BtnSettingTex[NUM_BTN_SETTING], BtnSettingHoverTex[NUM_BTN_SETTING], BtnSettingChoosenTex[NUM_BTN_SETTING],
		LeaderboardTex[NUM_LEADERBOARD_SCREEN], CreditScreenTex[NUM_CREDIT_SCREEN], CreditVAO, CreditVBO, LeaderboardVAO, LeaderboardVBO,
		BtnSettingVAO, BtnSettingVBO, BtnVAO, BtnVBO, LogoVAO, LogoVBO, LogoEBO, program, program2, program3, program4,program5,
		VBOObstacle, VAOObstacle, EBOObstacle, textureObstacle, VBOPlayer, VAOPlayer, EBOPlayer, texturePlayer,
		VBOVacuum, VAOVacuum, EBOVacuum, textureVacuum, BGTexture, BGVao, BGVbo, BGEbo;
	float BtnWidth[NUM_BUTTON], BtnHeigt[NUM_BUTTON], BtnHoverWidth[NUM_BUTTON],
		BtnHoverHeight[NUM_BUTTON], LogoWidth[NUM_LOGO], LogoHeight[NUM_LOGO],
		BtnSettingWidth[NUM_BTN_SETTING], BtnSettingHeight[NUM_BTN_SETTING], BtnSettingHoverWidth[NUM_BTN_SETTING],
		BtnSettingHoverHeight[NUM_BTN_SETTING], BtnSettingChoosenWidth[NUM_BTN_SETTING], BtnSettingChoosenHeight[NUM_BTN_SETTING],
		LeaderboardHeight[NUM_LEADERBOARD_SCREEN], LeaderboardWidth[NUM_LEADERBOARD_SCREEN], CreditHeight[NUM_CREDIT_SCREEN], CreditWidth[NUM_CREDIT_SCREEN],
		BGWidth, BGHeight,
		xpos = 0, ypos = 0, frame_dur = 0, xposPlayer = 0, yposPlayer = 0, xposVacuum = 0, yposVacuum = 0, gravityPlayer = 0, xVelocityPlayer = 0, yVelocityPlayer = 0, yposGroundPlayer = 0, oldxposPlayer = 0, oldyposPlayer = 0;
	int ActiveButtonIndex = 0;
	unsigned int frame_idx = 0, flip = 0, frame_idxPlayer = 0;
	bool walk_anim = false, onGround = true;
	bool IsCollided(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2);
	Status status;

	//Build Character
	void BuildObstacle();
	void DrawObstacle();

	//BuildPlayer
	void BuildPlayer();
	void DrawPlayer();

	//BuildVacuum
	void BuildVacuum();
	void DrawVacuum();

	void UpdateObstacleSpriteAnim(float deltaTime);
	void ControlObstacleSprite(float deltaTime);
	void UpdatePlayerSpriteAnim(float deltaTime);
	void ControlPlayerSprite(float deltaTime);
	void UpdateVacuumSpriteAnim(float deltaTime);
	void ControlVacuumSprite(float deltaTime);

	//Build Main Menu Screen
	void BuildLogo();
	void DrawLogo();
	void BuildBtn();
	void DrawBtn();

	//Build Button Screen
	int BtnActive = 0;
	void BuildBtnSetting();
	void DrawBtnSetting();

	//Build Leaderboard Screen
	void BuildLeaderboard();
	void DrawLeaderboard();

	//Build Credit Screen
	void BuildCredit();
	void DrawCredit();

	//Build Background
	void BuildBG();
	void DrawBG();
};

#endif
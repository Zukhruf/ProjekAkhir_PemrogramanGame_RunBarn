#ifndef SETTINGSCREEN_H
#define SETTINGSCREEN_H


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

#define NUM_BTN_SETTING 2


class SettingScreen :
	public Layar::Screen{
public:
	SettingScreen();
	~SettingScreen();
	virtual void Init();
	virtual void DeInit();
	virtual void Update(float deltaTime);
	virtual void Render();

private:
	GLuint BtnSettingTex[NUM_BTN_SETTING], BtnSettingHoverTex[NUM_BTN_SETTING], BtnSettingChoosenTex[NUM_BTN_SETTING], BtnSettingVAO, BtnSettingVBO, programSetting, programSetting2;
	float BtnSettingWidth[NUM_BTN_SETTING], BtnSettingHeight[NUM_BTN_SETTING], BtnSettingHoverWidth[NUM_BTN_SETTING], BtnSettingHoverHeight[NUM_BTN_SETTING], BtnSettingChoosenWidth[NUM_BTN_SETTING], BtnSettingChoosenHeight[NUM_BTN_SETTING];
	int BtnActive = 0;
	void BuildBtnSetting();
	void DrawBtnSetting();
};


#endif
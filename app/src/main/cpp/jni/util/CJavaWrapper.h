#pragma once

#include <jni.h>

#include <string>

#define EXCEPTION_CHECK(env) \
	if ((env)->ExceptionCheck()) \ 
	{ \
		(env)->ExceptionDescribe(); \
		(env)->ExceptionClear(); \
		return; \
	}

class CJavaWrapper
{
	jobject activity;

	jmethodID s_GetClipboardText;
	jmethodID s_CallLauncherActivity;

                  jmethodID s_RadarBR;

	jmethodID s_ShowInputLayout;
	jmethodID s_HideInputLayout;

    jmethodID s_showInventory;
    jmethodID s_closeInventory;
    jmethodID s_addItemToInv;

    jmethodID s_ShowClientSettings;
	jmethodID s_SetUseFullScreen;
	jmethodID s_MakeDialog;

	jmethodID s_showHud;
                  jmethodID s_hideHud;
	jmethodID s_updateHudInfo;

	jmethodID s_showradar;
                  jmethodID s_hideradar;

	jmethodID s_showGps;
                  jmethodID s_hideGps;

	jmethodID s_showZona;
	jmethodID s_hideZona;

                  jmethodID s_showx2;
	jmethodID s_hidex2;

	jmethodID s_showSpeed;
                  jmethodID s_hideSpeed;
	jmethodID s_updateSpeedInfo;

	jmethodID s_showNotification;
	jmethodID s_showMenu;

	jmethodID s_setPauseState;

	jmethodID s_showSplash;
	jmethodID s_updateSplash;
	jmethodID s_showRadial;
	
	jmethodID s_showHudFeatures;
	jmethodID s_hideHudFeatures;

	jmethodID s_showDonate;
	jmethodID s_updateDonate;
	jmethodID s_show_sc;
	
	jmethodID s_showHudAndLogo;
	jmethodID s_hideHudAndLogo;
public:
	JNIEnv* GetEnv();

	std::string GetClipboardString();
	void CallLauncherActivity(int type);

	void ShowInputLayout();
	void HideInputLayout();

	void ShowRadar();
	void HideRadar();

	void SetRadar();

	void ShowClientSettings();

	void ShowInventory(int inventory);

	void SetUseFullScreen(int b);

	// Trong CJavaWrapper class:
	void UpdateHudInfo(int health, int armour, int food, int water, int weaponid, int ammo, int playerid, int money, int wanted, int stress, int policeOnl, int medicOnl, int bankAccount);

	void ShowHud();
                  void HideHud();


	void ShowGPS();
                  void HideGPS();

	void ShowZona();
                  void HideZona();

	void ShowX2();
                  void HideX2();
                  
   void ShowHudFeatures();
   void HideHudFeatures();
   
   void ShowHudAndLogo();
   void HideHudAndLogo();

	void UpdateSpeedInfo(int speed, int fuel, int hp, int mileage, int engine, int light, int belt, int lock);
	void ShowSpeed();
                  void HideSpeed();

	void MakeDialog(int dialogId, int dialogTypeId, char* caption, char* content, char* leftBtnText, char* rightBtnText); // Диалоги
	void ShowNotification(int type, char* text, int duration, char* actionforBtn, char* textBtn);
	void ShowMenu();

	void SetPauseState(bool a1);

    void showInventory();
    void closeInventory();
    void addItemToInv(int id, int arg, int slot);

	void ShowSplash();
	void UpdateSplash(int progress);	
	
	void ShowRadial(bool park, bool key, bool doors, bool lights, bool suspension, bool launch_control, bool engine, bool turbo);

	void ShowDonate(int money, int bc);
	void UpdateDonate(int money, int bc);
	void show_sc(int money, int bc);

	CJavaWrapper(JNIEnv* env, jobject activity);
	~CJavaWrapper();

    void setTurnState();
};

extern CJavaWrapper* g_pJavaWrapper;
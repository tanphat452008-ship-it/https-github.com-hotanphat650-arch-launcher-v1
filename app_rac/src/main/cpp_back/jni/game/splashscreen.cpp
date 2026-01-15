#include "../main.h"
#include "RW/RenderWare.h"
#include "game.h"
#include "../gui/gui.h"

#include "../util/CJavaWrapper.h"

extern CGUI* pGUI;

RwTexture* splashTexture = nullptr;

#define COLOR_WHITE		0xFFFFFFFF
#define COLOR_BLACK 	                  0xFF000000
#define COLOR_ORANGE 	                  0xFFFAA500
#define COLOR_PURPLE	                  0xFFCE39DF
#define COLOR_ROSE		0xFFFF99FF
#define COLOR_GREY		0xFF5d5d5d
#define COLOR_BRED		0xFF9933FF
#define COLOR_BLUE		0xFF6C2713
#define COLOR_CYAN		0xFFCE6816
#define COLOR_1			0xFFB58891
#define COLOR_2			0xFF673F40
#define COLOR_PIZDEC	                  0xFFFF008b

struct stRect
{
	int x1;	// left
	int y1;	// top
	int x2;	// right
	int y2;	// bottom
};

struct stfRect
{
	float x1;
	float y1;
	float x2;
	float y2;
};

#define MAX_SCHEMAS 4
uint32_t colors[MAX_SCHEMAS][2] = {
	{ COLOR_BLACK,	COLOR_WHITE },
	{ COLOR_BLACK, 	COLOR_ORANGE },
	{ COLOR_BLACK,	COLOR_PIZDEC },
	{ COLOR_BLACK,	COLOR_BLUE }
};
unsigned int color_scheme = 0;

void LoadSplashTexture()
{
//	Log("CHUD: Loading radar texture..");
//	splashTexture = (RwTexture*)LoadTextureFromDB("samp", "hud_back");
}

void Draw(stRect* rect, uint32_t color, RwRaster* raster = nullptr, stfRect* uv = nullptr)
{

}


void RenderSplash()
{
	const float percent = *(float*)(g_libGTASA + 0x8F08C0);
	if (percent <= 0.0f) return;
	float mult = percent / 100.0f;

	int intMult = (int)percent;
	if (intMult > 50)
                  { 
                          intMult = 100; 
                  }
	g_pJavaWrapper->UpdateSplash(intMult);
}

void ImGui_ImplRenderWare_RenderDrawData(ImDrawData* draw_data);
void ImGui_ImplRenderWare_NewFrame();

void RenderBackgroundHud()
{
//                      if(CHUD::IsEnabled()) 
//                      {
//                                    Log("Create RADAR Raster()");
//			ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)splashTexture->raster,
//						ImVec2(CHUD::radarBgPos1.x1, CHUD::radarBgPos1.y1),
//						ImVec2(CHUD::radarBgPos2.x1, CHUD::radarBgPos2.y1),
//						ImVec2(0, 0), ImVec2(1, 1));
//                     }
}

void RenderSplashScreen()
{
	RenderSplash();
	g_pJavaWrapper->ShowSplash();

	if (!pGUI) return;

	ImGui_ImplRenderWare_NewFrame();
	ImGui::NewFrame();

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplRenderWare_RenderDrawData(ImGui::GetDrawData());
}
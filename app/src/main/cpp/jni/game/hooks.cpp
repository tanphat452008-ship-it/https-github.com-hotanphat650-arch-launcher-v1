#include "../main.h"
#include "../util/armhook.h"

#include "RW/RenderWare.h"
#include "game.h"

#include "../net/netgame.h"
#include "../gui/gui.h"
#include "../util/CJavaWrapper.h"

#include "../CAudioStream.h"

#include "../clientlogic/CRQ_Commands.h"
#include "../clientlogic/CSkyBox.h"

#include "..///..//santrope-tea-gtasa/encryption/CTinyEncrypt.h"
#include "..///..//santrope-tea-gtasa/encryption/encrypt.h"

#include "..//voice/CVoiceChatClient.h"
#include "..//chatwindow.h"
extern CGame* pGame;
#include "..//settings.h"
extern CSettings* pSettings;
extern CChatWindow* pChatWindow;
extern CVoiceChatClient* pVoice;

// *** THÊM VÀO ĐẦU FILE HOOKS.CPP (SAU CÁC INCLUDE) ***
extern int GetServerFoodValue();
extern int GetServerWaterValue();
extern int GetServerCashValue();
extern int GetServerBankValue();
extern int GetServerStressValue();
extern int GetPoliceOnlineValue();
extern int GetPoliceMedicValue();

//sanrelo.ru
extern CAudioStream* pAudioStream;

extern "C"
{
#include "..//..//santrope-tea-gtasa/encryption/aes.h"
}

char(*CStreaming__ConvertBufferToObject)(int, int, int);
int __attribute__((noinline)) g_unobfuscate(int a)
{
	return UNOBFUSCATE_DATA(a);
}
#include "..//str_obfuscator_no_template.hpp"
#define MAX_ENCRYPTED_TXD 2
const cryptor::string_encryptor encrArch[MAX_ENCRYPTED_TXD] = { cryptor::create("texdb/samp/samp.txt", 21), cryptor::create("texdb/gui/gui.txt", 19) };
static int lastOpenedFile = 0;

extern CNetGame *pNetGame;
extern CGUI *pGUI;
// Neiae/SAMP
bool g_bPlaySAMP = false;

void InitInMenu();
void MainLoop();
void HookCPad();

uintptr_t* dword_609F1C;

RwRaster* dword_748C98;
RwRaster* dword_748C94;
RwRaster* dword_748C9C;
RwRaster* dword_748C90;
RwRaster* dword_748C8C;
RwRaster* dword_748C88;

static uint32_t* dword_207F24 = nullptr;

extern char* PLAYERS_REALLOC;


/* ================ ie?oee aey ani. anoaaie =================== */

extern "C" uintptr_t get_lib() 
{
 	return g_libGTASA;
}

/* ====================================================== */

struct stFile
{
	int isFileExist;
	FILE *f;
};

stFile* (*NvFOpen)(const char*, const char*, int, int);
stFile* NvFOpen_hook(const char* r0, const char* r1, int r2, int r3)
{
	char path[0xFF] = { 0 };
	// ----------------------------
	if(!strncmp(r1+12, "mainV1.scm", 10))
	{
		sprintf(path, "%sSAMP/main.scm", g_pszStorage);
		Log("Loading mainV1.scm..");
		goto open;
	}
	// ----------------------------
	if(!strncmp(r1+12, "SCRIPTV1.IMG", 12))
	{
		sprintf(path, "%sSAMP/script.img", g_pszStorage);
		Log("Loading script.img..");
		goto open;
	}
	// ----------------------------
	if(!strncmp(r1, "DATA/PEDS.IDE", 13))
	{
		sprintf(path, "%s/SAMP/peds.ide", g_pszStorage);
		Log("Loading peds.ide..");
		goto open;
	}
	// ----------------------------
	if(!strncmp(r1, "DATA/VEHICLES.IDE", 17))
	{
		sprintf(path, "%s/SAMP/vehicles.ide", g_pszStorage);
		Log("Loading vehicles.ide..");
		goto open;
	}

	if (!strncmp(r1, "DATA/GTA.DAT", 12))
	{
		sprintf(path, "%s/SAMP/gta.dat", g_pszStorage);
		Log("Loading gta.dat..");
		goto open;
	}

	/*if (!strncmp(r1, "DATA/HANDLING.CFG", 17))
	{
		sprintf(path, "%s/SAMP/handling.cfg", g_pszStorage);
		Log("Loading handling.cfg..");
		goto open;
	}

	if (!strncmp(r1, "DATA/WEAPON.DAT", 15))
	{
		sprintf(path, "%s/SAMP/weapon.dat", g_pszStorage);
		Log("Loading weapon.dat..");
		goto open;
	}*/

orig:
	return NvFOpen(r0, r1, r2, r3);

open:
	stFile *st = (stFile*)malloc(8);
	st->isFileExist = false;

	FILE *f  = fopen(path, "rb");
	if(f)
	{
		st->isFileExist = true;
		st->f = f;
		return st;
	}
	else
	{
		Log("NVFOpen hook | Error: file not found (%s)", path);
		free(st);
		st = nullptr;
		return 0;
	}
}

int (*mpg123_param)(void *a1, int a2, long a3, int a4, double a5);
int mpg123_param_hook(void *a1, int a2, long a3, int a4, double a5)
{
    return mpg123_param(a1, a2, a3 | (0x7000 | 0x7040 | 0x4000 | 0x3000 | 0x2000 | 0x200 | 0x100 | 0x40 | 0x1234 | 0x5678 | 0x9012 | 0x3456 | 0x7890 | 0x2345 | 0x6789 | 0x0123 | 0x4567 | 0x8901 | 0x2346 | 0x6780 | 0x1235 | 0x7891 | 0x7000 | 0x8000 | 0x9000 | 0x10000 | 0x11000 | 0x12000 | 0x13000 | 0x14000 | 0x15000 | 0x7100 | 0x8200 | 0x9300 | 0x10400 | 0x11500 | 0x12600 | 0x8102 | 0x9203 | 0x1034 | 0x1145 | 0x1256 | 0x1367 | 0x1478 | 0x1589 | 0x1690 | 0x7801 | 0x8923 | 0x9034 | 0x10045 | 0x11156 | 0x12267 | 0x13378 | 0x14489 | 0x15590 | 0x16601 | 0x7712 | 0x8823 | 0x9934 | 0x10045 | 0x11156 | 0x12267 | 0x13378 | 0x14489 | 0x15590 | 0x16601 | 0x7723 | 0x8834 | 0x9945 | 0x10056 | 0x11167 | 0x12278 | 0x13389 | 0x14490 | 0x15501 | 0x16612 | 0x7734 | 0x8845 | 0x9956 | 0x10067 | 0x11178 | 0x12289 | 0x13390 | 0x14401 | 0x15512 | 0x16623 | 0x7012 | 0x8123 | 0x9234 | 0x1045 | 0x1156 | 0x1267 | 0x1378 | 0x1489 | 0x1590 | 0x1701 | 0x7812 | 0x8923 | 0x9034 | 0x10045 | 0x11156), a4, a5);
}


/* ====================================================== */
uintptr_t(*GetTexture_orig)(const char*);
uintptr_t GetTexture_hook(const char* a1)
{
    //001BE990
    uintptr_t tex = ((uintptr_t(*)(const char*))(g_libGTASA + 0x001BE990 + 1))(a1);
    if (!tex)
    {
        tex = ((uintptr_t(*)(const char*))(g_libGTASA + 0x001BE990 + 1))("replaceme");
        Log("TEXTURE NOT FOUND = %s", a1);
        return tex;
    }
    else
    {
        ++* (uintptr_t*)(tex + 84);
        return tex;
    }
}

void ShowHud() {
    if(pGame) {
        if(pNetGame) {
            if(pGame->FindPlayerPed() || GamePool_FindPlayerPed()) {
                CPlayerPool *pPlayerPool = pNetGame->GetPlayerPool();
                if(pPlayerPool) {
                    // Tạm thời dùng method cũ với ít parameters
                    g_pJavaWrapper->UpdateHudInfo(
                        pGame->FindPlayerPed()->GetHealth(),
                        pGame->FindPlayerPed()->GetArmour(),
                        GetServerFoodValue(),    // food từ server
                        GetServerWaterValue(),   // water từ server
                        GamePool_FindPlayerPed()->WeaponSlots[GamePool_FindPlayerPed()->byteCurWeaponSlot].dwType,
                        GamePool_FindPlayerPed()->WeaponSlots[GamePool_FindPlayerPed()->byteCurWeaponSlot].dwAmmo,
                        (int)pPlayerPool->GetLocalPlayerID(),
                        GetServerCashValue(),    // cash từ server
                        pGame->GetWantedLevel(),
                        GetServerStressValue(),
                        GetPoliceOnlineValue(),
                        GetPoliceMedicValue(),
						GetServerBankValue()
                    );
                }
            }
        }
    }
}

/* ====================================================== */
bool bGameStarted = false;
uint32_t bProcessedRender2dstuff = 0;
void (*Render2dStuff)();
void Render2dStuff_hook()
{
	bGameStarted = true;
	MAKE_PROFILE(test, test_time);
	MainLoop();
	LOG_PROFILE(test, test_time);
	bProcessedRender2dstuff = GetTickCount();
	Render2dStuff();

	ShowHud();

	if (pNetGame)
	{
		CLocalPlayer* pPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();
		if (pPlayer)
		{
			if (pPlayer->GetPlayerPed())
			{
				pNetGame->GetTextDrawPool()->Draw();
			}
		}
		if (pNetGame->GetPlayerPool())
		{
			if (pNetGame->GetPlayerPool()->GetLocalPlayer())
			{
				CPlayerPed* pPed = pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed();
				if (pPed)
				{
					CInfoBarText::Draw(pPed->GetHealth(), pPed->GetArmour());
				}
			}
		}
	}



	return;
}

void __fillArray()
{
}

void InitCTX(AES_ctx&, unsigned char const*)
{
	
}

/* ====================================================== */

void (*Render2dStuffAfterFade)();
void Render2dStuffAfterFade_hook()
{
	Render2dStuffAfterFade();
	if (pGUI && bGameStarted) pGUI->Render();
	return;
}

/* ====================================================== */

uint16_t gxt_string[0x7F];
uint16_t* (*CText_Get)(uintptr_t thiz, const char* text);
uint16_t* CText_Get_hook(uintptr_t thiz, const char* text)
{
	if(text[0] == 'S' && text[1] == 'A' && text[2] == 'M' && text[3] == 'P')
	{
		const char* code = &text[5];
		if(!strcmp(code, "MP")) CFont::AsciiToGxtChar("MultiPlayer", gxt_string);

    	return gxt_string;
	}

	return CText_Get(thiz, text);
}

/* ====================================================== */

void MainMenu_OnStartSAMP()
{

	if(g_bPlaySAMP) return;

	InitInMenu();

	// StartGameScreen::OnNewGameCheck()
	(( void (*)())(g_libGTASA+0x261C8C+1))();

	//*(uint32_t*)(g_libGTASA + 0x009E75B8) = 8;

	g_bPlaySAMP = true;
	return;
}

// OsArray<FlowScreen::MenuItem>::Add
void (*MenuItem_add)(int r0, uintptr_t r1);
void MenuItem_add_hook(int r0, uintptr_t r1)
{
	static bool bMenuInited = false;
	char* name = *(char**)(r1+4);

	if(!strcmp(name, "FEP_STG") && !bMenuInited)
	{
		Log("Creating \"MultiPlayer\" button.. (struct: 0x%X)", r1);
		// Nicaaai eiiieo "New Game"
		MenuItem_add(r0, r1);

		// eiiiea "MultiPlayer"
		*(char**)(r1+4) = "SAMP_MP";
		*(uintptr_t*)r1 = LoadTextureFromDB("samp", "menu_mainmp");
		*(uintptr_t*)(r1+8) = (uintptr_t)MainMenu_OnStartSAMP;

		bMenuInited = true;
		goto ret;
	}

	// Eaii?e?oai nicaaiea "Start Game" e "Stats" ec iai? iaocu
	if(g_bPlaySAMP && (
		!strcmp(name, "FEP_STG") ||
		!strcmp(name, "FEH_STA") ||
		!strcmp(name, "FEH_BRI") ))
		return;

ret:
	return MenuItem_add(r0, r1);
}

/* ====================================================== */

// CGame::InitialiseRenderWare
void (*InitialiseRenderWare)();
void InitialiseRenderWare_hook()
{
	Log("Loading \"samp\" cd..");
	InitialiseRenderWare();
	// TextureDatabaseRuntime::Load()
	(( void (*)(const char*, int, int))(g_libGTASA+0x1BF244+1))("samp", 0, 5);
	 //(( void (*)(const char*, int, int))(g_libGTASA+0x1BF244+1))("custom", 0, 5);
	//((void (*)(const char*, int, int))(g_libGTASA + 0x1BF244 + 1))("gui", 0, 5);

	return;
}

/* ====================================================== */

void RenderSplashScreen();
void (*CLoadingScreen_DisplayPCScreen)();
void CLoadingScreen_DisplayPCScreen_hook()
{
	RwCamera* camera = *(RwCamera**)(g_libGTASA+0x95B064);

	if(RwCameraBeginUpdate(camera))
	{
		DefinedState2d();
		(( void (*)())(g_libGTASA+0x5519C8+1))(); // CSprite2d::InitPerFrame()
		RwRenderStateSet(rwRENDERSTATETEXTUREADDRESS, (void*)rwTEXTUREADDRESSCLAMP);
		(( void (*)(bool))(g_libGTASA+0x198010+1))(false); // emu_GammaSet()
		RenderSplashScreen();
		RwCameraEndUpdate(camera);
		RwCameraShowRaster(camera, 0, 0);
	}

	return;
}
int bBlockCWidgetRegionLookUpdate = 0;

/* ====================================================== */
#include "..//keyboard.h"
extern CKeyBoard* pKeyBoard;
void (*TouchEvent)(int, int, int posX, int posY);
void TouchEvent_hook(int type, int num, int posX, int posY)
{
	//Log("TOUCH EVENT HOOK");

	if (*(uint8_t*)(g_libGTASA + 0x008C9BA3) == 1)
	{
		return TouchEvent(type, num, posX, posY);
	}

	if (g_pWidgetManager)
	{
		g_pWidgetManager->OnTouchEventSingle(WIDGET_CHATHISTORY_UP, type, num, posX, posY);
		g_pWidgetManager->OnTouchEventSingle(WIDGET_CHATHISTORY_DOWN, type, num, posX, posY);
	}
	bool bRet = pGUI->OnTouchEvent(type, num, posX, posY);
	if (!bRet)
	{
		return;
	}

	ImGuiIO& io = ImGui::GetIO();

	if (pKeyBoard && pKeyBoard->IsOpen())
	{
		if (posX >= io.DisplaySize.x - ImGui::GetFontSize() * 4 && posY >= io.DisplaySize.y / 2 - (ImGui::GetFontSize() * 2.5) * 3 && posY <= io.DisplaySize.y / 2) // keys
		{
			return;
		}
	}

	if (g_pWidgetManager)
	{
		g_pWidgetManager->OnTouchEvent(type, num, posX, posY);
		if (g_pWidgetManager->GetWidget(WIDGET_MICROPHONE))
		{
			if (g_pWidgetManager->GetWidget(WIDGET_MICROPHONE)->GetState() == 1)
			{
				bBlockCWidgetRegionLookUpdate = 1;
			}
			if (g_pWidgetManager->GetWidget(WIDGET_MICROPHONE)->GetState() == 2)
			{
				bBlockCWidgetRegionLookUpdate = 0;
			}
		}
	}
	if(bRet) 
		return TouchEvent(type, num, posX, posY);
}

/* ====================================================== */

void (*CStreaming_InitImageList)();
void CStreaming_InitImageList_hook()
{
	char* ms_files = (char*)(g_libGTASA+0x6702FC);
	ms_files[0] = 0;
	*(uint32_t*)&ms_files[44] = 0;
	ms_files[48] = 0;
	*(uint32_t*)&ms_files[92] = 0;
	ms_files[96] = 0;
	*(uint32_t*)&ms_files[140] = 0;
	ms_files[144] = 0;
	*(uint32_t*)&ms_files[188] = 0;
	ms_files[192] = 0;
	*(uint32_t*)&ms_files[236] = 0;
	ms_files[240] = 0;
	*(uint32_t*)&ms_files[284] = 0;
	ms_files[288] = 0;
	*(uint32_t*)&ms_files[332] = 0;
	ms_files[336] = 0;
	*(uint32_t*)&ms_files[380] = 0;

	(( uint32_t (*)(char*, uint32_t))(g_libGTASA+0x28E7B0+1))("TEXDB\\GTA3.IMG", 1); // CStreaming::AddImageToList
	(( uint32_t (*)(char*, uint32_t))(g_libGTASA+0x28E7B0+1))("TEXDB\\GTA_INT.IMG", 1); // CStreaming::AddImageToList
	(( uint32_t (*)(char*, uint32_t))(g_libGTASA+0x28E7B0+1))("TEXDB\\SAMP.IMG", 1); // CStreaming::AddImageToList
	(( uint32_t (*)(char*, uint32_t))(g_libGTASA+0x28E7B0+1))("TEXDB\\SAMPCOL.IMG", 1); // CStreaming::AddImageToList
	//(( uint32_t (*)(char*, uint32_t))(g_libGTASA+0x28E7B0+1))("TEXDB\\GTA_DFF.IMG", 1); // CStreaming::AddImageToList
	//(( uint32_t (*)(char*, uint32_t))(g_libGTASA+0x28E7B0+1))("TEXDB\\GTA_COL.IMG", 1); // CStreaming::AddImageToList

	//((uint32_t(*)(char*, uint32_t))(g_libGTASA + 0x28E7B0 + 1))("TEXDB\\ARH1.IMG", 1); // CStreaming::AddImageToList
	//((uint32_t(*)(char*, uint32_t))(g_libGTASA + 0x28E7B0 + 1))("TEXDB\\ARH2.IMG", 1); // CStreaming::AddImageToList
	//((uint32_t(*)(char*, uint32_t))(g_libGTASA + 0x28E7B0 + 1))("TEXDB\\ARH3.IMG", 1); // CStreaming::AddImageToList
}

/* ====================================================== */
typedef struct _PED_MODEL
{
	uintptr_t 	vtable;
	uint8_t		data[88];
} PED_MODEL; // SIZE = 92

PED_MODEL PedsModels[370];
int PedsModelsCount = 0;

PED_MODEL* (*CModelInfo_AddPedModel)(int id);
PED_MODEL* CModelInfo_AddPedModel_hook(int id)
{
	PED_MODEL* model = &PedsModels[PedsModelsCount];
	memset(model, 0, sizeof(PED_MODEL));								// initialize by zero

	((void(*)(void* thiz))(g_libGTASA + 0x0033559C + 1))((void*)model); // CBaseModelInfo::CBaseModelInfo();

    model->vtable = (uintptr_t)(g_libGTASA+0x5C6E90);					// assign CPedModelInfo vmt

    (( uintptr_t (*)(PED_MODEL*))(*(void**)(model->vtable+0x1C)))(model);  // CClumpModelInfo::Init()

    *(PED_MODEL**)(g_libGTASA+0x87BF48+(id*4)) = model; // CModelInfo::ms_modelInfoPtrs

	PedsModelsCount++;
	return model;
}
/* ====================================================== */

uint32_t (*CRadar__GetRadarTraceColor)(uint32_t color, uint8_t bright, uint8_t friendly);
uint32_t CRadar__GetRadarTraceColor_hook(uint32_t color, uint8_t bright, uint8_t friendly)
{
	return TranslateColorCodeToRGBA(color);
}

int (*CRadar__SetCoordBlip)(int r0, float X, float Y, float Z, int r4, int r5, char* name);
int CRadar__SetCoordBlip_hook(int r0, float X, float Y, float Z, int r4, int r5, char* name)
{
	if(pNetGame && !strncmp(name, "CODEWAY", 7))
	{
		float findZ = (( float (*)(float, float))(g_libGTASA+0x3C3DD8+1))(X, Y);
		findZ += 1.5f;

		Log("OnPlayerClickMap: %f, %f, %f", X, Y, Z);
		RakNet::BitStream bsSend;
		bsSend.Write(X);
		bsSend.Write(Y);
		bsSend.Write(findZ);
		pNetGame->GetRakClient()->RPC(&RPC_MapMarker, &bsSend, HIGH_PRIORITY, RELIABLE, 0, false, UNASSIGNED_NETWORK_ID, nullptr);
	}

	return CRadar__SetCoordBlip(r0, X, Y, Z, r4, r5, name);
}

uint8_t bGZ = 0;
void (*CRadar__DrawRadarGangOverlay)(uint8_t v1);
void CRadar__DrawRadarGangOverlay_hook(uint8_t v1)
{
	bGZ = v1;
	if (pNetGame && pNetGame->GetGangZonePool()) 
		pNetGame->GetGangZonePool()->Draw();
}

uint32_t dwParam1, dwParam2;
extern "C" void pickup_ololo()
{
	if(pNetGame && pNetGame->GetPickupPool())
	{
		CPickupPool *pPickups = pNetGame->GetPickupPool();
		pPickups->PickedUp( ((dwParam1-(g_libGTASA+0x70E264))/0x20) );
	}
}

__attribute__((naked)) void PickupPickUp_hook()
{
	//LOGI("PickupPickUp_hook");

	// calculate and save ret address
	__asm__ volatile("push {lr}\n\t"
					"push {r0}\n\t"
					"blx get_lib\n\t"
					"add r0, #0x2D0000\n\t"
					"add r0, #0x009A00\n\t"
					"add r0, #1\n\t"
					"mov r1, r0\n\t"
					"pop {r0}\n\t"
					"pop {lr}\n\t"
					"push {r1}\n\t");
	
	// 
	__asm__ volatile("push {r0-r11, lr}\n\t"
					"mov %0, r4" : "=r" (dwParam1));

	__asm__ volatile("blx pickup_ololo\n\t");


	__asm__ volatile("pop {r0-r11, lr}\n\t");

	// restore
	__asm__ volatile("ldrb r1, [r4, #0x1C]\n\t"
					"sub.w r2, r1, #0xD\n\t"
					"sub.w r2, r1, #8\n\t"
					"cmp r1, #6\n\t"
					"pop {pc}\n\t");
}

extern "C" bool NotifyEnterVehicle(VEHICLE_TYPE *_pVehicle)
{
    //Log("NotifyEnterVehicle");
 
    if(!pNetGame) return false;
 
    CVehiclePool *pVehiclePool = pNetGame->GetVehiclePool();
    CVehicle *pVehicle;
    VEHICLEID VehicleID = pVehiclePool->FindIDFromGtaPtr(_pVehicle);
 
    if(VehicleID == INVALID_VEHICLE_ID) return false;
    if(!pVehiclePool->GetSlotState(VehicleID)) return false;
    pVehicle = pVehiclePool->GetAt(VehicleID);
    if(pVehicle->m_bDoorsLocked) return false;
    if(pVehicle->m_pVehicle->entity.nModelIndex == TRAIN_PASSENGER) return false;
 
    if(pVehicle->m_pVehicle->pDriver &&
        pVehicle->m_pVehicle->pDriver->dwPedType != 0)
        return false;
 
    CLocalPlayer *pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();
 
    //if(pLocalPlayer->GetPlayerPed() && pLocalPlayer->GetPlayerPed()->GetCurrentWeapon() == WEAPON_PARACHUTE)
    //  pLocalPlayer->GetPlayerPed()->SetArmedWeapon(0);
 
    pLocalPlayer->SendEnterVehicleNotification(VehicleID, false);
 
    return true;
}

void (*CTaskComplexEnterCarAsDriver)(uint32_t thiz, uint32_t pVehicle);
extern "C" void call_taskEnterCarAsDriver(uintptr_t a, uint32_t b)
{
	CTaskComplexEnterCarAsDriver(a, b);
}
void __attribute__((naked)) CTaskComplexEnterCarAsDriver_hook(uint32_t thiz, uint32_t pVehicle)
{
    __asm__ volatile("push {r0-r11, lr}\n\t"
                    "mov r2, lr\n\t"
                    "blx get_lib\n\t"
                    "add r0, #0x3A0000\n\t"
                    "add r0, #0xEE00\n\t"
                    "add r0, #0xF7\n\t"
                    "cmp r2, r0\n\t"
                    "bne 1f\n\t" // !=
                    "mov r0, r1\n\t"
                    "blx NotifyEnterVehicle\n\t" // call NotifyEnterVehicle
                    "1:\n\t"  // call orig
                    "pop {r0-r11, lr}\n\t"
    				"push {r0-r11, lr}\n\t"
    				"blx call_taskEnterCarAsDriver\n\t"
    				"pop {r0-r11, pc}");
}

void ProcessPedDamage(PED_TYPE* pIssuer, PED_TYPE* pDamaged)
{
	if (!pNetGame) return;

	PED_TYPE* pPedPlayer = GamePool_FindPlayerPed();
	if (pDamaged && (pPedPlayer == pIssuer))
	{
		if (pNetGame->GetPlayerPool()->FindRemotePlayerIDFromGtaPtr((PED_TYPE*)pDamaged) != INVALID_PLAYER_ID)
		{
			CPlayerPool* pPlayerPool = pNetGame->GetPlayerPool();
			CAMERA_AIM* caAim = pPlayerPool->GetLocalPlayer()->GetPlayerPed()->GetCurrentAim();

			VECTOR aim;
			aim.X = caAim->f1x;
			aim.Y = caAim->f1y;
			aim.Z = caAim->f1z;

			pPlayerPool->GetLocalPlayer()->SendBulletSyncData(pPlayerPool->FindRemotePlayerIDFromGtaPtr((PED_TYPE*)pDamaged), 1, aim);
		}
	}
}

void (*CTaskComplexLeaveCar)(uintptr_t** thiz, VEHICLE_TYPE *pVehicle, int iTargetDoor, int iDelayTime, bool bSensibleLeaveCar, bool bForceGetOut);
void CTaskComplexLeaveCar_hook(uintptr_t** thiz, VEHICLE_TYPE *pVehicle, int iTargetDoor, int iDelayTime, bool bSensibleLeaveCar, bool bForceGetOut) 
{
	uintptr_t dwRetAddr = 0;
	__asm__ volatile ("mov %0, lr" : "=r" (dwRetAddr));
	dwRetAddr -= g_libGTASA;
 
 	if (dwRetAddr == 0x3AE905 || dwRetAddr == 0x3AE9CF) 
 	{
 		if (pNetGame) 
 		{
 			if (GamePool_FindPlayerPed()->pVehicle == (uint32_t)pVehicle) 
 			{
 				CVehiclePool *pVehiclePool=pNetGame->GetVehiclePool();
 				VEHICLEID VehicleID=pVehiclePool->FindIDFromGtaPtr((VEHICLE_TYPE *)GamePool_FindPlayerPed()->pVehicle);
 				CLocalPlayer *pLocalPlayer = pNetGame->GetPlayerPool()->GetLocalPlayer();
 				pLocalPlayer->SendExitVehicleNotification(VehicleID);
 			}
 		}
 	}
 
 	(*CTaskComplexLeaveCar)(thiz, pVehicle, iTargetDoor, iDelayTime, bSensibleLeaveCar, bForceGetOut);
}

unsigned int (*MainMenuScreen__Update)(uintptr_t thiz, float a2);
unsigned int MainMenuScreen__Update_hook(uintptr_t thiz, float a2)
{
	unsigned int ret = MainMenuScreen__Update(thiz, a2);
	MainMenu_OnStartSAMP();
	return ret;
}

extern signed int (*OS_FileOpen)(unsigned int a1, int* a2, const char* a3, int a4);
signed int OS_FileOpen_hook(unsigned int a1, int* a2, const char* a3, int a4);

extern int(*OS_FileRead)(void* a1, void* a2, int a3);
int OS_FileRead_hook(void* a1, void* a2, int a33);
extern char(*CStreaming__ConvertBufferToObject)(int, int, int);
char CStreaming__ConvertBufferToObject_hook(int a1, int a2, int a3);
extern char(*CFileMgr__ReadLine)(int, int, int);
char CFileMgr__ReadLine_hook(int a1, int a2, int a3);

void RedirectCall(uintptr_t addr, uintptr_t func);

static char szLastBufferedName[40];
int (*cHandlingDataMgr__FindExactWord)(uintptr_t thiz, char* line, char* nameTable, int entrySize, int entryCount);
int cHandlingDataMgr__FindExactWord_hook(uintptr_t thiz, char* line, char* nameTable, int entrySize, int entryCount)
{
	strncpy(&szLastBufferedName[0], line, entrySize);
	return cHandlingDataMgr__FindExactWord(thiz, line, nameTable, entrySize, entryCount);
}

void (*cHandlingDataMgr__ConvertDataToGameUnits)(uintptr_t thiz, tHandlingData* handling);
void cHandlingDataMgr__ConvertDataToGameUnits_hook(uintptr_t thiz, tHandlingData* handling)
{
	int iHandling = ((int(*)(uintptr_t, char*))(g_libGTASA + 0x004FBC4C + 1))(thiz, &szLastBufferedName[0]);

	CHandlingDefault::FillDefaultHandling((uint16_t)iHandling, handling);

	return cHandlingDataMgr__ConvertDataToGameUnits(thiz, handling);
}


#include "..//nv_event.h"
int32_t(*NVEventGetNextEvent_hooked)(NVEvent* ev, int waitMSecs);
int32_t NVEventGetNextEvent_hook(NVEvent* ev, int waitMSecs)
{
	int32_t ret = NVEventGetNextEvent_hooked(ev, waitMSecs);

	if (ret)
	{
		if (ev->m_type == NV_EVENT_MULTITOUCH)
		{
			// process manually
			ev->m_type = (NVEventType)228;
		}

	}

	NVEvent event;
	NVEventGetNextEvent(&event);

	if (event.m_type == NV_EVENT_MULTITOUCH)
	{
		int type = event.m_data.m_multi.m_action & NV_MULTITOUCH_ACTION_MASK;
		int num = (event.m_data.m_multi.m_action & NV_MULTITOUCH_POINTER_MASK) >> NV_MULTITOUCH_POINTER_SHIFT;

		int x1 = event.m_data.m_multi.m_x1;
		int y1 = event.m_data.m_multi.m_y1;

		int x2 = event.m_data.m_multi.m_x2;
		int y2 = event.m_data.m_multi.m_y2;

		int x3 = event.m_data.m_multi.m_x3;
		int y3 = event.m_data.m_multi.m_y3;

		if (type == NV_MULTITOUCH_CANCEL)
		{
			type = NV_MULTITOUCH_UP;
		}

		if ((x1 || y1) || num == 0)
		{
			if (num == 0 && type != NV_MULTITOUCH_MOVE)
			{
				((void(*)(int, int, int posX, int posY))(g_libGTASA + 0x00239D5C + 1))(type, 0, x1, y1); // AND_TouchEvent
			}
			else
			{
				((void(*)(int, int, int posX, int posY))(g_libGTASA + 0x00239D5C + 1))(NV_MULTITOUCH_MOVE, 0, x1, y1); // AND_TouchEvent
			}
		}

		if ((x2 || y2) || num == 1)
		{
			if (num == 1 && type != NV_MULTITOUCH_MOVE)
			{
				((void(*)(int, int, int posX, int posY))(g_libGTASA + 0x00239D5C + 1))(type, 1, x2, y2); // AND_TouchEvent
			}
			else
			{
				((void(*)(int, int, int posX, int posY))(g_libGTASA + 0x00239D5C + 1))(NV_MULTITOUCH_MOVE, 1, x2, y2); // AND_TouchEvent
			}
		}
		if ((x3 || y3) || num == 2)
		{
			if (num == 2 && type != NV_MULTITOUCH_MOVE)
			{
				((void(*)(int, int, int posX, int posY))(g_libGTASA + 0x00239D5C + 1))(type, 2, x3, y3); // AND_TouchEvent
			}
			else
			{
				((void(*)(int, int, int posX, int posY))(g_libGTASA + 0x00239D5C + 1))(NV_MULTITOUCH_MOVE, 2, x3, y3); // AND_TouchEvent
			}
		}
	}

	return ret;
}

void(*CStreaming__Init2)();
void CStreaming__Init2_hook()
{
	CStreaming__Init2();
	*(uint32_t*)(g_libGTASA + 0x005DE734) = 536870912;
}

void (*NvUtilInit)(void);
void NvUtilInit_hook(void)
{	
	NvUtilInit();
	*(char**)(g_libGTASA + 0x5D1608) = "/storage/emulated/0/bytaylyx/";
}

struct LimitPool
{
    void*       objects;
    uint8_t*    flags;
    uint32_t    count;
    uint32_t    top;
    bool        ownsAllocs;
    bool        locked;
    char        pad[2];
};

inline LimitPool* AllocatePool(size_t count, size_t size)
{
    LimitPool *p = new LimitPool;
    p->objects = new char[size*count];
    p->flags = new uint8_t[count];
    p->count = count;
    p->top = 0xFFFFFFFF;
    p->ownsAllocs = 1;
    for (size_t i = 0; i < count; ++i)
    {
        p->flags[i] |= 0x80;
        p->flags[i] &= 0x80;
    }
    return p;
}

#define adjust(__var) (int)(4 * __var)

int (*CPools__Initialise)(void* thiz);
int CPools__Initialise_hook(void* thiz) {
    int result = CPools__Initialise(thiz);
    *(LimitPool**)(g_libGTASA + 0x8B93E0) = AllocatePool(adjust(100000), 0x8); // CPools::ms_pPtrNodeSingleLinkPool
    *(LimitPool**)(g_libGTASA + 0x8B93DC) = AllocatePool(adjust(10000), 0xC); // CPools::ms_pPtrNodeDoubleLinkPool
    *(LimitPool**)(g_libGTASA + 0x8B93D8) = AllocatePool(adjust(2000), 20); // CPools::ms_pEntryInfoNodePool
    *(LimitPool**)(g_libGTASA + 0x8B93D4) = AllocatePool(adjust(300), 1996); // CPools::ms_pPedPool
    *(LimitPool**)(g_libGTASA + 0x8B93D0) = AllocatePool(adjust(500), 2604); // CPools::ms_pVehiclePool
    *(LimitPool**)(g_libGTASA + 0x8B93CC) = AllocatePool(adjust(21000), 0x3C); // CPools::ms_pBuildingPool
    *(LimitPool**)(g_libGTASA + 0x8B93C8) = AllocatePool(adjust(3000), 420); // CPools::ms_pObjectPool
    *(LimitPool**)(g_libGTASA + 0x8B93C4) = AllocatePool(adjust(5000), 0x3C); // CPools::ms_pDummyPool
    *(LimitPool**)(g_libGTASA + 0x8B93C0) = AllocatePool(adjust(15200), 48); // CPools::ms_pColModelPool
    *(LimitPool**)(g_libGTASA + 0x8B93BC) = AllocatePool(adjust(2000), 128); // CPools::ms_pTaskPool
    *(LimitPool**)(g_libGTASA + 0x8B93B8) = AllocatePool(adjust(500), 68); // CPools::ms_pEventPool
    *(LimitPool**)(g_libGTASA + 0x8B93B4) = AllocatePool(adjust(150), 100); // CPools::ms_pPointRoutePool
    *(LimitPool**)(g_libGTASA + 0x8B93B0) = AllocatePool(adjust(70), 420); // CPools::ms_pPatrolRoutePool
    *(LimitPool**)(g_libGTASA + 0x8B93B0) = AllocatePool(adjust(150), 36); // CPools::ms_pNodeRoutePool
    *(LimitPool**)(g_libGTASA + 0x8B93A8) = AllocatePool(adjust(40), 32); // CPools::ms_pTaskAllocatorPool
    *(LimitPool**)(g_libGTASA + 0x8B93A4) = AllocatePool(adjust(320), 664); // CPools::ms_pPedIntelligencePool
    *(LimitPool**)(g_libGTASA + 0x8B93A0) = AllocatePool(adjust(150), 236); // CPools::ms_pPedAttractorPool
    return result; //sliv by t.me/rkkdev
}

void InstallSpecialHooks()
{
	WriteMemory(g_libGTASA + 0x0023BEDC, (uintptr_t)"\xF8\xB5", 2);
	WriteMemory(g_libGTASA + 0x0023BEDE, (uintptr_t)"\x00\x46\x00\x46", 4);

    SetUpHook(g_libGTASA + 0x3AF1A0, (uintptr_t)CPools__Initialise_hook, (uintptr_t*)&CPools__Initialise);

	SetUpHook(g_libGTASA + 0x0023BEDC, (uintptr_t)OS_FileRead_hook, (uintptr_t*)& OS_FileRead);
	SetUpHook(g_libGTASA + 0x23B3DC, (uintptr_t)NvFOpen_hook, (uintptr_t*)& NvFOpen);
	SetUpHook(g_libGTASA + 0x241D94, (uintptr_t) NvUtilInit_hook, (uintptr_t *) &NvUtilInit);

	SetUpHook(g_libGTASA+0x269974, (uintptr_t)MenuItem_add_hook, (uintptr_t*)&MenuItem_add);
	SetUpHook(g_libGTASA+0x4D3864, (uintptr_t)CText_Get_hook, (uintptr_t*)&CText_Get);
	SetUpHook(g_libGTASA+0x40C530, (uintptr_t)InitialiseRenderWare_hook, (uintptr_t*)&InitialiseRenderWare);
	SetUpHook(g_libGTASA + 0x0025E660, (uintptr_t)MainMenuScreen__Update_hook, (uintptr_t*)& MainMenuScreen__Update);
	SetUpHook(g_libGTASA + 0x0023BB84, (uintptr_t)OS_FileOpen_hook, (uintptr_t*)& OS_FileOpen);

	SetUpHook(g_libGTASA + 0x004FBBB0, (uintptr_t)cHandlingDataMgr__FindExactWord_hook, (uintptr_t*)& cHandlingDataMgr__FindExactWord);
	SetUpHook(g_libGTASA + 0x004FBCF4, (uintptr_t)cHandlingDataMgr__ConvertDataToGameUnits_hook, (uintptr_t*)& cHandlingDataMgr__ConvertDataToGameUnits);
	SetUpHook(g_libGTASA + 0x0023ACC4, (uintptr_t)NVEventGetNextEvent_hook, (uintptr_t*)& NVEventGetNextEvent_hooked);
	SetUpHook(g_libGTASA + 0x004042A8, (uintptr_t)CStreaming__Init2_hook, (uintptr_t*)& CStreaming__Init2);	// increase stream memory value
}

void ProcessPedDamage(PED_TYPE* pIssuer, PED_TYPE* pPlayer);

uintptr_t (*ComputeDamageResponse)(uintptr_t, uintptr_t, int, int);
uintptr_t ComputeDamageResponse_hooked(uintptr_t issuer, uintptr_t ped, int a3, int a4)
{
	ProcessPedDamage((PED_TYPE*)*(uintptr_t*)issuer, (PED_TYPE*)ped );
	return ComputeDamageResponse(issuer, ped, a3, a4);
}

int(*RwFrameAddChild)(int, int);
int RwFrameAddChild_hook(int a1, int a2)
{
	if (a2 && a1)
	{
		return RwFrameAddChild(a1, a2);
	}
	else return 0;
}

void (*CAnimManager__UncompressAnimation)(int, int);
void CAnimManager__UncompressAnimation_hook(int a1, int a2)
{
	if (a1)
	{
		CAnimManager__UncompressAnimation(a1, a2);
	}
}

void(*CCustomRoadsignMgr__RenderRoadsignAtomic)(int, int);
void CCustomRoadsignMgr__RenderRoadsignAtomic_hook(int a1, int a2)
{
	if (a1)
	{
		CCustomRoadsignMgr__RenderRoadsignAtomic(a1, a2);
	}
}

int(*CUpsideDownCarCheck__IsCarUpsideDown)(int, int);
int CUpsideDownCarCheck__IsCarUpsideDown_hook(int a1, int a2)
{
	if (*(uintptr_t*)(a2 + 20))
	{
		return CUpsideDownCarCheck__IsCarUpsideDown(a1, a2);
	}
	return 0;
}

int(*CAnimBlendNode__FindKeyFrame)(int, float, int, int);
int CAnimBlendNode__FindKeyFrame_hook(int a1, float a2, int a3, int a4)
{
	if (*(uintptr_t*)(a1 + 16))
	{
		return CAnimBlendNode__FindKeyFrame(a1, a2, a3, a4);
	}
	else return 0;
}


/* ====================================================== */

typedef struct _ATOMIC_MODEL
{
	uintptr_t 	vtable;
	uint8_t		data[52];
} ATOMIC_MODEL; // SIZE = 56

ATOMIC_MODEL AtomicModels[16000];
int AtomicModelsCount = 0;

ATOMIC_MODEL* (*CModelInfo_AddAtomicModel)(int id);
ATOMIC_MODEL* CModelInfo_AddAtomicModel_hook(int id)
{
	ATOMIC_MODEL* model = &AtomicModels[AtomicModelsCount];
	memset(model, 0, sizeof(ATOMIC_MODEL));

	((void(*)(void* thiz))(g_libGTASA + 0x0033559C + 1))((void*)model); // CBaseModelInfo::CBaseModelInfo();

	model->vtable = (uintptr_t)(g_libGTASA + 0x005C6C68); // assign CAtomicModelInfo vmt

	((uintptr_t(*)(ATOMIC_MODEL*))(*(void**)(model->vtable + 0x1C)))(model); // CClumpModelInfo::Init()

	*(ATOMIC_MODEL * *)(g_libGTASA + 0x87BF48 + (id * 4)) = model; // CModelInfo::ms_modelInfoPtrs

	AtomicModelsCount++;
	return model;
}

/* ====================================================== */
typedef struct _VEHICLE_MODEL
{
	uintptr_t 	vtable;
	uint8_t		data[932];
} VEHICLE_MODEL; // SIZE = 936

VEHICLE_MODEL VehicleModels[370];
int VehicleModelsCount = 0;

VEHICLE_MODEL* (*CModelInfo_AddVehicleModel)(int id);
VEHICLE_MODEL* CModelInfo_AddVehicleModel_hook(int id)
{
	VEHICLE_MODEL* model = &VehicleModels[VehicleModelsCount];
	memset(model, 0, sizeof(VEHICLE_MODEL));

	((void(*)(void* thiz))(g_libGTASA + 0x00337AA0 + 1))((void*)model); // CVehicleModelInfo::CVehicleModelInfo();

	model->vtable = (uintptr_t)(g_libGTASA + 0x005C6EE0);			// assign CVehicleModelInfo vmt

	((uintptr_t(*)(VEHICLE_MODEL*))(*(void**)(model->vtable + 0x1C)))(model); // CVehicleModelInfo::Init()

	*(VEHICLE_MODEL * *)(g_libGTASA + 0x87BF48 + (id * 4)) = model; // CModelInfo::ms_modelInfoPtrs

	VehicleModelsCount++;
	return model;
}
/* ====================================================== */
void(*CHud__DrawScriptText)(uintptr_t, uint8_t);

float g_fMicrophoneButtonPosX;
float g_fMicrophoneButtonPosY;
bool g_IsVoiceServer();
void CHud__DrawScriptText_hook(uintptr_t thiz, uint8_t unk)
{
	CHud__DrawScriptText(thiz, unk);
	if (pGame && pNetGame)
	{
	

		if (g_pWidgetManager)
		{
			ImGuiIO& io = ImGui::GetIO();

			if (!g_pWidgetManager->GetSlotState(WIDGET_CHATHISTORY_UP))
			{
				g_pWidgetManager->New(WIDGET_CHATHISTORY_UP, 1700.0f, (io.DisplaySize.y * 0.3) - 180.0f, 130.0f, 130.0f, "menu_up");
				g_pWidgetManager->GetWidget(WIDGET_CHATHISTORY_UP)->SetPosWithoutScale(pGUI->ScaleX(1325.0f), io.DisplaySize.y * 0.3);
			}
			if (!g_pWidgetManager->GetSlotState(WIDGET_CHATHISTORY_DOWN))
			{
				g_pWidgetManager->New(WIDGET_CHATHISTORY_DOWN, 1700.0f, (io.DisplaySize.y * 0.3) - 30, 130.0f, 130.0f, "menu_down");
				g_pWidgetManager->GetWidget(WIDGET_CHATHISTORY_DOWN)->SetPosWithoutScale(pGUI->ScaleX(1515.0f), io.DisplaySize.y * 0.3);
			}
			if (!g_pWidgetManager->GetSlotState(WIDGET_MICROPHONE))
			{
				if (g_IsVoiceServer())
				{
					// pVoice
					g_fMicrophoneButtonPosX = pSettings->GetReadOnly().fButtonMicrophoneX;
					g_fMicrophoneButtonPosY = pSettings->GetReadOnly().fButtonMicrophoneY;
					g_pWidgetManager->New(WIDGET_MICROPHONE, 
						pSettings->GetReadOnly().fButtonMicrophoneX, 
						pSettings->GetReadOnly().fButtonMicrophoneY,
						pSettings->GetReadOnly().fButtonMicrophoneSize,
						pSettings->GetReadOnly().fButtonMicrophoneSize, "hud_dance");
					g_pWidgetManager->GetWidget(WIDGET_MICROPHONE)->SetTexture("samp", "voice_button");
				}
			}

			if (!g_pWidgetManager->GetSlotState(WIDGET_CAMERA_CYCLE))
			{
				g_pWidgetManager->New(WIDGET_CAMERA_CYCLE,
					pSettings->GetReadOnly().fButtonCameraCycleX,
					pSettings->GetReadOnly().fButtonCameraCycleY,
					pSettings->GetReadOnly().fButtonCameraCycleSize,
					pSettings->GetReadOnly().fButtonCameraCycleSize, "cam-toggle");
				// cam-toggle
			}
		}
	}
}

#include "..//keyboard.h"
extern CKeyBoard* pKeyBoard;
int(*CWidgetButtonEnterCar_Draw)(uintptr_t);
uint32_t g_uiLastTickVoice = 0;
int CWidgetButtonEnterCar_Draw_hook(uintptr_t thiz)
{
	if (g_pWidgetManager)
	{
		CWidget* pWidget = g_pWidgetManager->GetWidget(WIDGET_CHATHISTORY_UP);
		if (pWidget)
		{
			pWidget->SetDrawState(false);
		}
		pWidget = g_pWidgetManager->GetWidget(WIDGET_CHATHISTORY_DOWN);
		if (pWidget)
		{
			pWidget->SetDrawState(false);
		}

		pWidget = g_pWidgetManager->GetWidget(WIDGET_CAMERA_CYCLE);
		if (pWidget)
		{
			pWidget->SetDrawState(true);
		}

		pWidget = g_pWidgetManager->GetWidget(WIDGET_MICROPHONE);
		if (pWidget)
		{
			if (pVoice)
			{
				pWidget->SetDrawState(true);
				static uint32_t lastTick = GetTickCount();
				if (pWidget->GetState() == 2 && GetTickCount() - lastTick >= 250)
				{
					pVoice->TurnRecording();
					if (pVoice->IsRecording())
					{
						g_uiLastTickVoice = GetTickCount();
						if (pVoice->IsDisconnected())
						{
							pChatWindow->AddDebugMessage("                               ");
							pVoice->DisableInput();
						}
					}
					lastTick = GetTickCount();
				}

				if (pVoice->IsRecording() && GetTickCount() - g_uiLastTickVoice >= 30000)
				{
					pVoice->DisableInput();
				}

				if (pVoice->IsRecording())
				{
					if (pVoice->IsDisconnected())
					{
						pChatWindow->AddDebugMessage("                               ");
						pVoice->DisableInput();
					}
					pWidget->SetColor(255, 0x9C, 0xCF, 0x9C);
				}
				else
				{
					pWidget->ResetColor();
				}
				/*if ((pWidget->GetState() == 2 || pWidget->GetState() == 0) && GetTickCount() - lastTick >= 250)
				{
					pVoice->DisableInput();
					lastTick = GetTickCount();
				}*/
			}
		}

		if (!pGame->IsToggledHUDElement(HUD_ELEMENT_BUTTONS))
		{
			for (int i = 0; i < MAX_WIDGETS; i++)
			{
				CWidget* pWidget = g_pWidgetManager->GetWidget(i);
				if (pWidget)
				{
					pWidget->SetDrawState(false);
				}
			}
		}

		g_pWidgetManager->Draw();
	}
	return CWidgetButtonEnterCar_Draw(thiz);
}

uint64_t(*CWorld_ProcessPedsAfterPreRender)();
uint64_t CWorld_ProcessPedsAfterPreRender_hook()
{
	uint64_t res = CWorld_ProcessPedsAfterPreRender();
	if (pNetGame && pNetGame->GetPlayerPool())
	{
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			CPlayerPed* pPed = nullptr;
			if (pNetGame->GetPlayerPool()->GetLocalPlayerID() == i)
			{
				pPed = pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed();
			}
			else
			{
				if (pNetGame->GetPlayerPool()->GetSlotState(i))
				{
					pPed = pNetGame->GetPlayerPool()->GetAt(i)->GetPlayerPed();
				}
			}
			if (!pPed) continue;
			else
			{
				pPed->ProcessAttach();
			}
		}
	}

	return res;
}

int RemoveModelIDs[MAX_REMOVE_MODELS];
VECTOR RemovePos[MAX_REMOVE_MODELS];
float RemoveRad[MAX_REMOVE_MODELS];
int iTotalRemovedObjects = 0;


#include "util.h"

int(*CFileLoader__LoadObjectInstance)(uintptr_t, uintptr_t);
int CFileLoader__LoadObjectInstance_hook(uintptr_t thiz, uintptr_t name)
{
	for (int i = 0; i < iTotalRemovedObjects; i++)
	{
		if (RemoveModelIDs[i] == *(uint32_t*)(thiz + 28))
		{
			VECTOR pos;
			pos.X = *(float*)(thiz);
			pos.Y = *(float*)(thiz + 4);
			pos.Z = *(float*)(thiz + 8);
			if (GetDistanceBetween3DPoints(&pos, &RemovePos[i]) <= RemoveRad[i])
			{
				*(int*)(thiz + 28) = 19300;
			}
		}
	}
	return CFileLoader__LoadObjectInstance(thiz, name);
}

#include <list>

std::list<std::pair<unsigned int*, unsigned int>> resetEntries;

static uint32_t Color32Reverse(uint32_t x)
{
	return
		((x & 0xFF000000) >> 24) |
		((x & 0x00FF0000) >> 8) |
		((x & 0x0000FF00) << 8) |
		((x & 0x000000FF) << 24);
}

static RwRGBA DWORD2RGBAinternal(uint32_t dwColor)
{
	RwRGBA tmp;

	tmp.blue = dwColor & 0xFF; dwColor >>= 8;
	tmp.green = dwColor & 0xFF; dwColor >>= 8;
	tmp.red = dwColor & 0xFF; dwColor >>= 8;
	tmp.alpha = dwColor & 0xFF; /* dwColor >>= 8; */

	return tmp;
}

uintptr_t RwFrameForAllObjectsCALLBACK1(uintptr_t object, CObject* pObject)
{
	if (*(uint8_t*)object != 1)
	{
		return object;
	}
	uintptr_t pAtomic = object;
	RpGeometry* pGeom = *(RpGeometry * *)(pAtomic + 24);
	if (!pGeom)
	{
		return object;
	}

	int numMats = pGeom->matList.numMaterials;
	if (numMats > 16)
	{
		numMats = 16;
	}
	for (int i = 0; i < numMats; i++)
	{
		RpMaterial* pMat = pGeom->matList.materials[i];
		if (!pMat)
		{
			continue;
		}
		if (pObject->m_pMaterials[i].m_bCreated)
		{
			RpMaterial* pMat = pGeom->matList.materials[i];
			if (!pMat)
			{
				continue;
			}
			if (pObject->m_pMaterials[i].pTex)
			{
				resetEntries.push_back(std::make_pair(reinterpret_cast<unsigned int*>(&pMat->texture), *reinterpret_cast<unsigned int*>(&pMat->texture)));
				pMat->texture = pObject->m_pMaterials[i].pTex;
			}
			/*if (pObject->m_pMaterials[i].dwColor)
			{
				//((int(*)())(g_libGTASA + 0x00559FC8 + 1))();
				resetEntries.push_back(std::make_pair(reinterpret_cast<unsigned int*>(&pGeom->flags), *reinterpret_cast<unsigned int*>(&pGeom->flags)));
				pGeom->flags |= 0x00000040;
				pGeom->flags &= 0xFFFFFFF7;
				RwRGBA r = DWORD2RGBAinternal(pObject->m_pMaterials[i].dwColor);

				resetEntries.push_back(std::make_pair(reinterpret_cast<unsigned int*>(&pMat->color), *reinterpret_cast<unsigned int*>(&pMat->color)));
				pMat->color = r;
				pMat->surfaceProps.ambient = 1.0f;
				pMat->surfaceProps.specular = 0.0f;
				pMat->surfaceProps.diffuse = 1.0f;
			}*/
		}
	}
	return object;
}

int g_iLastRenderedObject;
void(*CObject__Render)(ENTITY_TYPE*);
void CObject__Render_hook(ENTITY_TYPE* thiz)
{
	// 0051ABB0 + 1
	// 004353FE + 1
	// 004352C4 + 1

	if (CSkyBox::GetSkyObject())
	{
		if (CSkyBox::GetSkyObject()->m_pEntity == thiz && !CSkyBox::IsNeedRender())
			return;
	}

	uintptr_t dwRetAddr = 0;
	__asm__ volatile ("mov %0, lr" : "=r" (dwRetAddr));
	dwRetAddr -= g_libGTASA;

	if (dwRetAddr == 0x0051ABB0 + 1 || dwRetAddr == 0x004353FE + 1 || dwRetAddr == 0x004352C4 + 1)
	{
		return CObject__Render(thiz);
	}

	uintptr_t pAtomic = thiz->m_RwObject;
	if (!pAtomic)
	{
		return CObject__Render(thiz); // CObject::Render(CObject*)
	}
	if (!*(uintptr_t*)(pAtomic + 4))
	{
		return CObject__Render(thiz); // CObject::Render(CObject*)
	}

	if (pNetGame)
	{
		CObjectPool* pObjectPool = pNetGame->GetObjectPool();
		if (pObjectPool)
		{
			CObject* pObject = pObjectPool->GetObjectFromGtaPtr(thiz);
			if (pObject)
			{
				if (pObject->m_pEntity)
				{
					g_iLastRenderedObject = pObject->m_pEntity->nModelIndex;
				}
				
				((void(*)())(g_libGTASA + 0x00559EF8 + 1))(); // DeActivateDirectional
				if (pObject->m_bMaterials)
				{
					//thiz->m_bLightObject = 0;
					((uintptr_t(*)(uintptr_t, uintptr_t, uintptr_t))(g_libGTASA + 0x001AEE2C + 1))(*(uintptr_t*)(pAtomic + 4), (uintptr_t)RwFrameForAllObjectsCALLBACK1, (uintptr_t)pObject); // RwFrameForAllObjects
					//((void(*)(ENTITY_TYPE*, bool))(g_libGTASA + 0x003B1F1C + 1))(thiz, 1); // CObject::RemoveLighting(CObject*, bool)
					//((void(*)())(g_libGTASA + 0x00559EF8 + 1))(); // DeActivateDirectional

				}
			}
			
		}
	}

	CObject__Render(thiz); // CObject::Render(CObject*)
	for (auto& p : resetEntries)
		* p.first = p.second;
	resetEntries.clear();
}


void (*CRenderer__RenderEverythingBarRoads)();
void CRenderer__RenderEverythingBarRoads_hook()
{
	CSkyBox::Process();

	CRenderer__RenderEverythingBarRoads();
}

#pragma optimize( "", off )

char CStreaming__ConvertBufferToObject_hook(int a1, int a2, int a3)
{
	uintptr_t dwRetAddr = 0;
	__asm__ volatile ("mov %0, lr" : "=r" (dwRetAddr));
	

	uint32_t tickStart = GetTickCount();
	//CGameResourcesDecryptor::CStreaming__ConvertBufferToObject_hook((char*)a1, a2, a3);
	if (a2 >= 15000 && a2 <= 15100)
	{
		//pChatWindow->AddDebugMessage("loading time %d", GetTickCount() - tickStart);
	}
	char a12 = CStreaming__ConvertBufferToObject(a1, a2, a3);
	return a12;
}

bool isEncrypted(const char* szArch)
{
	return false;
	for (int i = 0; i < MAX_ENCRYPTED_TXD; i++)
	{
		if (!strcmp(encrArch[i].decrypt(), szArch)) return true;
	}

	return false;
}

signed int (*OS_FileOpen)(unsigned int a1, int* a2, const char* a3, int a4);
signed int OS_FileOpen_hook(unsigned int a1, int* a2, const char* a3, int a4)
{
	uintptr_t calledFrom = 0;
	__asm__ volatile ("mov %0, lr" : "=r" (calledFrom));
	calledFrom -= g_libGTASA;
	signed int retn = OS_FileOpen(a1, a2, a3, a4);

	if (calledFrom == 0x001BCE9A + 1)
	{
		if (isEncrypted(a3))
		{
			lastOpenedFile = *a2;
		}
	}
	return retn;
}

// CGameIntegrity
// CodeObfuscation

static uint32_t dwRLEDecompressSourceSize = 0;

int(*OS_FileRead)(void* a1, void* a2, int a3);
void InitCTX(AES_ctx& ctx, const uint8_t* pKey);
int OS_FileRead_hook(void* a1, void* a2, int a3)
{
	uintptr_t calledFrom = 0;
	__asm__ volatile ("mov %0, lr" : "=r" (calledFrom));
	calledFrom -= g_libGTASA;

	if (!a3)
	{
		return 0;
	}

	if (calledFrom == 0x001BCEE0 + 1 && a1 == (void*)lastOpenedFile)
	{
		lastOpenedFile = 0;
		
		AES_ctx ctx;
		InitCTX(ctx, &g_iEncryptionKeyTXD[0]);

		int retv = OS_FileRead(a1, a2, a3);
		int fileSize = a3;
		int iters = fileSize / PART_SIZE_TXD;
		uintptr_t pointer = (uintptr_t)a2;
		for (int i = 0; i < iters; i++)
		{
			AES_CBC_decrypt_buffer(&ctx, (uint8_t*)pointer, PART_SIZE_TXD);
			pointer += PART_SIZE_TXD;
		}
		return retv;
	}
	if (calledFrom == 0x001BDD34 + 1)
	{
		int retn = OS_FileRead(a1, a2, a3);

		dwRLEDecompressSourceSize = *(uint32_t*)a2;

		return retn;
	}
	else
	{
		int retn = OS_FileRead(a1, a2, a3);

		return retn;
	}
}

/*int(*OS_FileRead)(void* a1, void* a2, int a3);
int OS_FileRead_hook(void* a1, void* a2, int a3)
{
	uintptr_t calledFrom = 0;
	__asm__ volatile ("mov %0, lr" : "=r" (calledFrom));
	calledFrom -= g_libGTASA;

	if (!a3)
	{
		return 0;
	}

	if (calledFrom == 0x001BCEE0 + 1 && a1 == (void*)lastOpenedFile)
	{
		lastOpenedFile = 0;
		CTinyEncrypt tinyEnc;
		tinyEnc.SetKey(&g_iEncryptionKeyVersion2TXD[0]);
		int retv = OS_FileRead(a1, a2, a3);
		int fileSize = a3;
		int iters = fileSize / PART_SIZE_TXD;
		uintptr_t pointer = (uintptr_t)a2;
		for (int i = 0; i < iters; i++)
		{
			tinyEnc.DecryptData((void*)pointer, PART_SIZE_TXD, 16);
			pointer += PART_SIZE_TXD;
		}
		return retv;
	}
	else
	{
		int retn = OS_FileRead(a1, a2, a3);

		return retn;
	}
}*/

#include <sstream>
char(*CFileMgr__ReadLine)(int, int, int);
char CFileMgr__ReadLine_hook(int a1, int a2, int a3)
{
	char retv = CFileMgr__ReadLine(a1, a2, a3);
	char* pStr = (char*)a2;
	int value = *(int*)pStr;
	if (value == g_unobfuscate(g_iIdentifierVersion2))
	{
		pStr += 4;
		int length = *(int*)pStr;
		pStr -= 4;
		memcpy((void*)pStr, (const void*)& pStr[8], length);

		pStr[length] = 0;
		std::stringstream ss;

		uint32_t keyi = g_unobfuscate(g_i64Encrypt);

		ss << keyi;

		std::string key(ss.str());
		std::string val(pStr);

		std::string decr = decrypt(val, key);

		strcpy((char*)a2, decr.c_str());
	}
	return retv;
}

#pragma optimize( "", on )



uintptr_t(*CPlayerInfo__FindObjectToSteal)(uintptr_t, uintptr_t);
uintptr_t CPlayerInfo__FindObjectToSteal_hook(uintptr_t a1, uintptr_t a2)
{
	return 0;
}
typedef uintptr_t RpClump;
RwFrame* CClumpModelInfo_GetFrameFromId_Post(RwFrame* pFrameResult, RpClump* pClump, int id)
{
	if (pFrameResult)
		return pFrameResult;

	uintptr_t calledFrom = 0;
	__asm__ volatile ("mov %0, lr" : "=r" (calledFrom));
	calledFrom -= g_libGTASA;

	//                                 
	if (calledFrom == 0x00515708                // CVehicle::SetWindowOpenFlag
		|| calledFrom == 0x00515730             // CVehicle::ClearWindowOpenFlag
		|| calledFrom == 0x00338698             // CVehicleModelInfo::GetOriginalCompPosition
		|| calledFrom == 0x00338B2C)            // CVehicleModelInfo::CreateInstance
		return NULL;

	for (uint i = 2; i < 40; i++)
	{
		RwFrame* pNewFrameResult = NULL;
		uint     uiNewId = id + (i / 2) * ((i & 1) ? -1 : 1);
		
		pNewFrameResult = ((RwFrame * (*)(RpClump * pClump, int id))(g_libGTASA + 0x00335CC0 + 1))(pClump, i);

		if (pNewFrameResult)
		{
			return pNewFrameResult;
		}
	}

	return NULL;
}
RwFrame* (*CClumpModelInfo_GetFrameFromId)(RpClump*, int);
RwFrame* CClumpModelInfo_GetFrameFromId_hook(RpClump* a1, int a2)
{
	return CClumpModelInfo_GetFrameFromId_Post(CClumpModelInfo_GetFrameFromId(a1, a2), a1, a2);
}
void (*CWidgetRegionLook__Update)(uintptr_t thiz);
void CWidgetRegionLook__Update_hook(uintptr_t thiz)
{
	if (bBlockCWidgetRegionLookUpdate)
	{
		return;
	}
	else
	{
		return CWidgetRegionLook__Update(thiz);
	}
}
uint8_t* (*RLEDecompress)(uint8_t* pDest, size_t uiDestSize, uint8_t const* pSrc, size_t uiSegSize, uint32_t uiEscape);
uint8_t* RLEDecompress_hook(uint8_t* pDest, size_t uiDestSize, uint8_t const* pSrc, size_t uiSegSize, uint32_t uiEscape) {
	if (!pDest) 
	{
		return pDest;
	}

	if (!pSrc) 
	{
		return pDest;
	}

	uint8_t* pTempDest = pDest;
	const uint8_t* pTempSrc = pSrc;
	uint8_t* pEndOfDest = &pDest[uiDestSize];

	uint8_t* pEndOfSrc = (uint8_t*)&pSrc[dwRLEDecompressSourceSize];

	if (pDest < pEndOfDest) 
	{
		do 
		{
			if (*pTempSrc == uiEscape) 
			{
				uint8_t ucCurSeg = pTempSrc[1];
				if (ucCurSeg) 
				{
					uint8_t* ucCurDest = pTempDest;
					uint8_t ucCount = 0;
					do 
					{
						++ucCount;
						//Log("WRITE111 TO 0x%x FROM 0x%x SIZE %d", ucCurDest, pTempSrc + 2, uiSegSize);
						pDest = (uint8_t*)memcpy(ucCurDest, pTempSrc + 2, uiSegSize);
						
						ucCurDest += uiSegSize;
					} while (ucCurSeg != ucCount);


					pTempDest += uiSegSize * ucCurSeg;
				}
				pTempSrc += 2 + uiSegSize;
			}

			else 
			{
				if (pTempSrc + uiSegSize >= pEndOfSrc)
				{
					//Log("AVOID CRASH TO 0x%x FROM 0x%x SIZE %d END OF SRC 0x%x", pTempDest, pTempSrc, pEndOfSrc - pTempSrc - 1, pEndOfSrc);
					return pDest;
				}
				else
				{
					//Log("WRITE222 TO 0x%x FROM 0x%x SIZE %d END OF SRC 0x%x", pTempDest, pTempSrc, uiSegSize, pEndOfSrc);
					pDest = (uint8_t*)memcpy(pTempDest, pTempSrc, uiSegSize);
					pTempDest += uiSegSize;
					pTempSrc += uiSegSize;
				}
			}
		} while (pEndOfDest > pTempDest);
	}

	dwRLEDecompressSourceSize = 0;

	return pDest;
}

#include "..//crashlytics.h"

char g_bufRenderQueueCommand[200];
uintptr_t g_dwRenderQueueOffset;

char* (*RenderQueue__ProcessCommand)(uintptr_t thiz, char* a2);
char* RenderQueue__ProcessCommand_hook(uintptr_t thiz, char* a2)
{
	if (thiz && a2)
	{
		uintptr_t* dwRenderQueue = (uintptr_t*)thiz;

		memset(g_bufRenderQueueCommand, 0, sizeof(g_bufRenderQueueCommand));

		g_dwRenderQueueOffset = *(uintptr_t*)a2;
		snprintf(g_bufRenderQueueCommand, 190, "offset: %d | name: %s", g_dwRenderQueueOffset, (const char*)(*(dwRenderQueue + 100 + g_dwRenderQueueOffset)));

		return RenderQueue__ProcessCommand(thiz, a2);
	}
	else
	{
		return nullptr;
	}
}

int (*_rwFreeListFreeReal)(int a1, unsigned int a2);
int _rwFreeListFreeReal_hook(int a1, unsigned int a2)
{
	if (a1)
	{
		return _rwFreeListFreeReal(a1, a2);
	}
	else
	{
		return 0;
	}
}

uintptr_t* CTouchInterface__m_bWidgets;

static bool ShouldBeProcessedButton(int result)
{
	CTouchInterface__m_bWidgets = (uintptr_t*)(g_libGTASA + 0x00657E48);

	if (result == CTouchInterface__m_bWidgets[18])
	{
		return false;
	}

	if (result == CTouchInterface__m_bWidgets[26] || result == CTouchInterface__m_bWidgets[27])
	{
		if (pNetGame)
		{
			CLocalPlayer* pLocal = pNetGame->GetPlayerPool()->GetLocalPlayer();
			if (pLocal)
			{
				if (pLocal->GetPlayerPed())
				{
					VEHICLE_TYPE* pVehicle = pLocal->GetPlayerPed()->GetGtaVehicle();
					if (pVehicle)
					{
						uintptr_t this_vtable = pVehicle->entity.vtable;
						this_vtable -= g_libGTASA;

						if (this_vtable == 0x5CCE60)
						{
							return 1;
						}
					}
				}
			}
		}
		return 0;
	}
	return 1;
}

void (*CWidgetButton__Update)(int result, int a2, int a3, int a4);
void CWidgetButton__Update_hook(int result, int a2, int a3, int a4)
{
	if (!result)
	{
		return;
	}
	if (ShouldBeProcessedButton(result))
	{
		return CWidgetButton__Update(result, a2, a3, a4);
	}
	else
	{
		return;
	}
}

int (*CWidget__IsTouched)(uintptr_t a1);
int CWidget__IsTouched_hook(uintptr_t a1)
{
	if (ShouldBeProcessedButton(a1))
	{
		return CWidget__IsTouched(a1);
	}
	else
	{
		return 0;
	}
}
void readVehiclesAudioSettings();
void (*CVehicleModelInfo__SetupCommonData)();
void CVehicleModelInfo__SetupCommonData_hook()
{
	CVehicleModelInfo__SetupCommonData();
	readVehiclesAudioSettings();
}

extern VehicleAudioPropertiesStruct VehicleAudioProperties[20000];
static uintptr_t addr_veh_audio = (uintptr_t)& VehicleAudioProperties[0];

void (*CAEVehicleAudioEntity__GetVehicleAudioSettings)(uintptr_t thiz, int16_t a2, int a3);
void CAEVehicleAudioEntity__GetVehicleAudioSettings_hook(uintptr_t dest, int16_t a2, int ID)
{
	memcpy((void*)dest, &VehicleAudioProperties[(ID - 400)], sizeof(VehicleAudioPropertiesStruct));
}
// https://beeg.com/1256386731?t=85

void (*CDarkel__RegisterCarBlownUpByPlayer)(void* pVehicle, int arg2);
void CDarkel__RegisterCarBlownUpByPlayer_hook(void* pVehicle, int arg2)
{
	return;
}
void (*CDarkel__ResetModelsKilledByPlayer)(int playerid);
void CDarkel__ResetModelsKilledByPlayer_hook(int playerid)
{
	return;
}
int(*CDarkel__QueryModelsKilledByPlayer)(int, int);
int CDarkel__QueryModelsKilledByPlayer_hook(int player, int modelid)
{
	return 0;
}

int (*CDarkel__FindTotalPedsKilledByPlayer)(int player);
int CDarkel__FindTotalPedsKilledByPlayer_hook(int player)
{
	return 0;
}

void (*CDarkel__RegisterKillByPlayer)(void* pKilledPed, unsigned int damageWeaponID, bool bHeadShotted, int arg4);
void CDarkel__RegisterKillByPlayer_hook(void* pKilledPed, unsigned int damageWeaponID, bool bHeadShotted, int arg4)
{
	return;
}

std::list<std::pair<unsigned int*, unsigned int>> resetEntriesVehicle;


RpMaterial* CVehicle__SetupRenderMatCB(RpMaterial* material, void* data)
{
	if (material)
	{
		if (material->texture)
		{
			CVehicle* pVeh = (CVehicle*)data;
			for (size_t i = 0; i < MAX_REPLACED_TEXTURES; i++)
			{
				if (pVeh->m_bReplaceTextureStatus[i])
				{
					if (!strcmp(&(material->texture->name[0]), &(pVeh->m_szReplacedTextures[i].szOld[0])))
					{
						if (pVeh->m_szReplacedTextures[i].pTexture)
						{
							resetEntriesVehicle.push_back(std::make_pair(reinterpret_cast<unsigned int*>(&(material->texture)), *reinterpret_cast<unsigned int*>(&(material->texture))));
							material->texture = pVeh->m_szReplacedTextures[i].pTexture;
							if (strstr(pVeh->m_szReplacedTextures[i].szOld, "ret_t"))
							{
								material->color.alpha = 255;
							}
						}
					}
				}
			}
		}
	}
	return material;
}

uintptr_t CVehicle__SetupRenderCB(uintptr_t atomic, void* data)
{
	if (*(RpGeometry * *)(atomic + 24))
	{
		((RpGeometry * (*)(RpGeometry*, uintptr_t, void*))(g_libGTASA + 0x001E284C + 1))(*(RpGeometry * *)(atomic + 24), (uintptr_t)CVehicle__SetupRenderMatCB, (void*)data); // RpGeometryForAllMaterials
	}

	return atomic;
}
#include "..//cryptors/MODELINFO_EDITABLE_result.h"
typedef uintptr_t RpClump;
void(*CVehicleModelInfo__SetEditableMaterials)(uintptr_t);
void CVehicleModelInfo__SetEditableMaterials_hook(uintptr_t clump)
{
	PROTECT_CODE_MODELINFO_EDITABLE;
	RpClump* pClump = (RpClump*)clump;

	

	if (pNetGame && pClump)
	{
		if (pNetGame->GetVehiclePool())
		{
			VEHICLEID vehId = pNetGame->GetVehiclePool()->FindIDFromRwObject((RwObject*)clump);
			CVehicle* pVehicle = pNetGame->GetVehiclePool()->GetAt(vehId);
			if (pVehicle)
			{
				if (pVehicle->m_bReplacedTexture)
				{
					// RpClump* RpClumpForAllAtomics(RpClump* clump, RpAtomicCallBack callback, void* pData);
					((RpClump * (*)(RpClump*, uintptr_t, void*))(g_libGTASA + 0x001E0EA0 + 1))(pClump, (uintptr_t)CVehicle__SetupRenderCB, (void*)pVehicle); // RpClumpForAllAtomics
				}
			}
		}
	}

	CVehicleModelInfo__SetEditableMaterials(clump);
}

#include "..//cryptors/RESET_AFTER_RENDER_result.h"

void (*CVehicle__ResetAfterRender)(uintptr_t);
void CVehicle__ResetAfterRender_hook(uintptr_t thiz)
{
	PROTECT_CODE_RESET_AFTER_RENDER;

	for (auto& p : resetEntriesVehicle)
	{
		*p.first = p.second;
	}
	resetEntriesVehicle.clear();

	CVehicle__ResetAfterRender(thiz);
}

void (*CRopes__Update)();
void CRopes__Update_hook()
{
	
}

void* (*CCustomCarEnvMapPipeline__pluginEnvMatDestructorCB)(void* object, RwInt32 offset, RwInt32 size);
void* CCustomCarEnvMapPipeline__pluginEnvMatDestructorCB_hook(void* object, RwInt32 offset, RwInt32 size)
{
	if(pChatWindow) pChatWindow->AddDebugMessage("m_objects %x", *(uintptr_t * *)(g_libGTASA + 0x00669E48));
	return CCustomCarEnvMapPipeline__pluginEnvMatDestructorCB(object, offset, size);
}

#include "CRenderTarget.h"
#include "..//gui/CFontInstance.h"
#include "..//gui/CFontRenderer.h"
#include "CCustomPlateManager.h"

static bool g_bFirstPersonOnFootEnabled = false;
extern bool bDisableTestGovno;
void (*CGame__Process)();
void CGame__Process_hook()
{
	CGame__Process();

	static bool once = false;
	if (!once)
	{
		CCustomPlateManager::Initialise();
		CSnow::Initialise();
		once = true;
	}

                 WriteMemory(g_libGTASA + 0x52DD38, (uintptr_t)"\x00\x20\x70\x47", 4); // CCoronas::RenderReflections
                 NOP(g_libGTASA + 0x39AD14, 1); //render clouds, sunrefl, raineffect 
                 memcpy((uint32_t*)(g_libGTASA+0x5DE734), "0x10000000", 10); // CStreaming::ms_memoryAvailable(limit);

	if (pNetGame && pNetGame->GetPlayerPool() && pNetGame->GetPlayerPool()->GetLocalPlayer())
	{
		CSnow::Process(pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed(), pGame->GetActiveInterior());
	}

	if (pNetGame)
	{
		CTextDrawPool* pTextDrawPool = pNetGame->GetTextDrawPool();
		if (pTextDrawPool) {
			pTextDrawPool->SnapshotProcess();
		}
	}

	if (g_pWidgetManager)
	{
		PED_TYPE* pPed = GamePool_FindPlayerPed();
		if (g_pWidgetManager->GetSlotState(WIDGET_CAMERA_CYCLE) && pPed)
		{
			static uint32_t lastTick = GetTickCount();
			bool bPressed = false;
			if (g_pWidgetManager->IsTouched(WIDGET_CAMERA_CYCLE) && GetTickCount() - lastTick >= 250)
			{
				bPressed = true;
				lastTick = GetTickCount();
			}

			if (!CFirstPersonCamera::IsEnabled() && g_bFirstPersonOnFootEnabled)
			{
				CFirstPersonCamera::SetEnabled(true);
			}

			if (CFirstPersonCamera::IsEnabled() && !g_bFirstPersonOnFootEnabled)
			{
				CFirstPersonCamera::SetEnabled(false);
			}

			if (bPressed && !IN_VEHICLE(pPed))
			{
				CFirstPersonCamera::Toggle();
				if (CFirstPersonCamera::IsEnabled())
				{
					g_bFirstPersonOnFootEnabled = true;
				}
				else
				{
					g_bFirstPersonOnFootEnabled = false;
				}
			}
		}
	}

	if (pAudioStream) pAudioStream->Process();

	CCustomPlateManager::Process();
}

#include "..//cryptors/AUTOMOBILE_COLLISION_result.h"

uint16_t g_usLastProcessedModelIndexAutomobile = 0;
int g_iLastProcessedModelIndexAutoEnt = 0;
void (*CAutomobile__ProcessEntityCollision)(VEHICLE_TYPE* a1, ENTITY_TYPE* a2, int a3);
void CAutomobile__ProcessEntityCollision_hook(VEHICLE_TYPE* a1, ENTITY_TYPE* a2, int a3)
{
	PROTECT_CODE_AUTOMOBILE_COLLISION;

	g_usLastProcessedModelIndexAutomobile = a1->entity.nModelIndex;
	g_iLastProcessedModelIndexAutoEnt = a2->nModelIndex;

	bool bReplace = false;
	void* pOld = nullptr;
	uint8_t* pColData = nullptr;

	

	if (pNetGame)
	{
		if (pNetGame->GetVehiclePool())
		{
			uint16_t vehId = pNetGame->GetVehiclePool()->FindIDFromGtaPtr(a1);
			CVehicle* pVeh = pNetGame->GetVehiclePool()->GetAt(vehId);
			if (pVeh)
			{
				if (pVeh->bHasSuspensionLines && pVeh->GetVehicleSubtype() == VEHICLE_SUBTYPE_CAR)
				{
					pColData = GetCollisionDataFromModel(a1->entity.nModelIndex);
					if (pColData && pVeh->m_pSuspensionLines)
					{
						if (*(void**)(pColData + 16))
						{
							pOld = *(void**)(pColData + 16);
							*(void**)(pColData + 16) = pVeh->m_pSuspensionLines;
							bReplace = true;
						}
					}
				}
			}
		}
	}
	CAutomobile__ProcessEntityCollision(a1, a2, a3);
	if (bReplace)
	{
		*(void**)(pColData + 16) = pOld;
	}
}

bool (*CGame__Shutdown)();
bool CGame__Shutdown_hook()
{
	Log("Exiting game...");
	NOP(g_libGTASA + 0x00341FCC, 2); // nop PauseOpenSLES
	NOP(g_libGTASA + 0x0046389E, 2); // nop saving
	if (pNetGame)
	{
		if (pNetGame->GetRakClient())
		{
			pNetGame->GetRakClient()->Disconnect(500, 0);
		}
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	return CGame__Shutdown();
}

// TODO: VEHICLE RESET SUSPENSION
void (*CShadows__StoreCarLightShadow)(VEHICLE_TYPE* vehicle, int id, RwTexture* texture, VECTOR* posn, float frontX, float frontY, float sideX, float sideY, unsigned char red, unsigned char green, unsigned char blue, float maxViewAngle);
void CShadows__StoreCarLightShadow_hook(VEHICLE_TYPE* vehicle, int id, RwTexture* texture, VECTOR* posn, float frontX, float frontY, float sideX, float sideY, unsigned char red, unsigned char green, unsigned char blue, float maxViewAngle)
{
	uint8_t r, g, b;
	r = red;
	g = green;
	b = blue;
	if (pNetGame)
	{
		if (pNetGame->GetVehiclePool())
		{
			uint16_t vehid = pNetGame->GetVehiclePool()->FindIDFromGtaPtr(vehicle);
			CVehicle* pVeh = pNetGame->GetVehiclePool()->GetAt(vehid);
			if (pVeh)
			{
				pVeh->ProcessHeadlightsColor(r, g, b);
			}
		}
	}

	return CShadows__StoreCarLightShadow(vehicle, id, texture, posn, frontX, frontY, sideX, sideY, r, g, b, maxViewAngle);
}
extern void sub_55718(int *param1, uint8_t a2, uint8_t a3, uint16_t a4, int a5)
{
    param1[1] = a3;
    *param1 = a2;
    param1[2] = a4;
    param1[3] = a5;
}
extern void sub_5578E(int *a1){
    a1[2] | (a1[1] << 8) | (*a1 << 0x10) | (a1[3] << 0x18);
}
constexpr uintptr_t off_1F95D4 = 0x1F95D4;
constexpr uintptr_t unk_1D4EC2 = 0x1D4EC2;
constexpr uintptr_t unk_1D50BA = 0x1D50BA;
uintptr_t sub_57DE0(){
    return (uintptr_t)PLAYERS_REALLOC;
}
int (*car_customenvmap)(int result, int a2);
int car_customenvmap_hook(int result, int a2);
bool sub_57DF0(uintptr_t a1)
{
    // TODO: реализовать sub_57DF0
    return (bool)((int (*)(void))(g_libGTASA + 0x41DD7D))();
}
void sub_57F64(int a1, uintptr_t a2)
{
    // TODO: реализовать sub_57F64
}
void sub_55814(uintptr_t a1, uintptr_t a2, int a3)
{
    // TODO: реализовать sub_55814
}
uintptr_t* sub_5212C(uintptr_t* a1)
{
    *reinterpret_cast<uintptr_t*>(a1 + 8) = 1;
    *reinterpret_cast<uintptr_t*>(a1) = off_1F95D4;
    *reinterpret_cast<uintptr_t*>(a1 + 0xC) = sub_57DE0();
    *reinterpret_cast<uintptr_t*>(a1 + 4) = sub_57DE0();

    uintptr_t v2 = *reinterpret_cast<uintptr_t*>(a1 + 0xC);
    *reinterpret_cast<char*>(a1 + 0x10) = 0;

    sub_57F64(0, v2);
    sub_55814(unk_1D4EC2, *reinterpret_cast<uintptr_t*>(a1 + 8), 0);

    if (*reinterpret_cast<uintptr_t*>(a1 + 0xC) && sub_57DF0(*reinterpret_cast<uintptr_t*>(a1 + 8)))
        sub_55814(unk_1D50BA, *reinterpret_cast<uintptr_t*>(a1 + 8), 1);

    *reinterpret_cast<uintptr_t*>(a1 + 0x11) = 0;
    *reinterpret_cast<uint16_t*>(a1 + 0x1D) = 1;
    *reinterpret_cast<uintptr_t*>(a1 + 0x15) = 0;
    *reinterpret_cast<uintptr_t*>(a1 + 0x19) = 0;
    *reinterpret_cast<uintptr_t*>(a1 + 0x1F) = 0x3C03126F;

    memset(reinterpret_cast<void*>(a1 + 0x50), 0, 0x23Au);

    reinterpret_cast<void(*)(uintptr_t)>(g_libGTASA + 0x43E201)(*reinterpret_cast<uintptr_t*>(a1 + 0xC));
    reinterpret_cast<void(*)(uintptr_t)>(g_libGTASA + 0x43E229)(*reinterpret_cast<uintptr_t*>(a1 + 0xC));

    return a1;
}
RwRaster* dword_748F80;
RwRaster* dword_748F84;
RwRaster* dword_748F88;
RwRaster* dword_748F8C;
RwRaster* dword_748F90;
RwRaster* dword_748F94;
void dword_7BEF38(int a1, int a2)
{
    return ((void ( *)(int, int))((char *)0x1B8071 + g_libGTASA))(a1, a2);
}
extern void sub_3F614(uintptr_t* a1, int a2)
{
    uint32_t* v4; // r5
    int v5; // r6
    uint32_t* v6; // r8
    int v7; // r6
    uint32_t* v8; // r1
    uint32_t* v9; // r0
    bool v10; // zf
    int v11; // r2
    bool v12; // zf
    int v13; // r4
    int v14; // r5
    int v15; // r0
    char v16; // r0
    uint32_t* v17; // r0
    int v18; // r0
    int v19; // r6
    float v20; // s0
    int v21; // r0
    float v22; // s4
    void (__fastcall ***v23)(uint32_t, uint32_t); // r6
    void (__fastcall **v24)(uint32_t, uint32_t); // r0
    void (__fastcall ***v25)(uint32_t, uint32_t); // r6
    void (__fastcall **v26)(uint32_t, uint32_t); // r0
    int* v27; // r1
    int v28; // r1
    int v29; // r0
    int v30; // r0
    uint32_t* v31; // r0
    int v33; // [sp+10h] [bp-48h] BYREF
    int v34[3]; // [sp+14h] [bp-44h] BYREF
    int64_t v35; // [sp+20h] [bp-38h] BYREF
    int v36; // [sp+28h] [bp-30h]
    float v37; // [sp+2Ch] [bp-2Ch] BYREF
    float v38; // [sp+30h] [bp-28h]
    float v39; // [sp+34h] [bp-24h]
    int v40; // [sp+38h] [bp-20h]

    // нету


    v4 = dword_207F24;
    if ( dword_207F24 )
    {
        if ( !*dword_207F24 )
        {
            //  v5 = operator new(0x28Au);
            // нету
            sub_5212C(a1);
            *v4 = v5;
        }
//	Log("testcam 3");
        dword_7BEF38(1, 0);
        dword_7BEF38(6, 0);
        dword_7BEF38(8, 0);
        dword_7BEF38(0xC, 0);
        dword_7BEF38(0xA, 5);
        dword_7BEF38(0xB, 6);
        dword_7BEF38(0xE, 0);
        dword_7BEF38(0x14, 1);
        //Log("testcam 4");
        v6 = (uint32_t*)(0x1AD8A1);
//	Log("testcam 44");
        v7 = *(uint32_t*)((char*)g_libGTASA + 0x95B064);
//	Log("testcam 5");
        v33 = 0xFFFFFFFF;
        if ( a2 )
            v7 = a2;
        // Log("testcam 6");
        ((void (__fastcall *)(int, int*, int))(0x1AD8A1 + g_libGTASA))(v7, &v33, 2);
        ((void (__fastcall *)(int, int*, int))(0x1AD8A1 + g_libGTASA))(v7, &v33, 3);
        //Log("testcam 7");
        ///   v8 = (uint32_t*)(a1 + 0x40);
        ///  v9 = (uint32_t*)(a1 + 4 * *v8);
        v10 = v9 == 0;
        //Log("testcam 8");
        if ( v9 )
        {
            /// v8 = (uint32_t*)(a1 + 4 * *v8);
            ///  v6 = (uint32_t*)(v8[4]);
            v10 = v6 == 0;
        }
//	Log("testcam 9");
        if ( !v10 )
        {
            v11 = v8[8];
            v12 = v11 == 0;
            if ( v11 )
                v12 = v8[0xC] == 0;
            if ( !v12 )
            {
                //	  Log("testcam 10");
                if ( !a2 )
                    v6 = v9;
                v13 = g_libGTASA;
                v14 = *(uint32_t*)(0x61572C + g_libGTASA);
                v15 = g_libGTASA;
                if ( (v14 & 0x800000) != 0 )
                {

                    *(uint32_t*)(0x61572C + g_libGTASA) = v14 & 0xF77FFFFF;
                    //  v16 = byte_609F24;
                    //  __dmb(0xBu);
                    //	Log("testcam 11");
                    if ( (v16 & 1) == 0 /*&& j___cxa_guard_acquire((__guard*)&byte_609F24)*/ )
                    {
                        v17 = (uint32_t*)(::operator new(4u));
                        *v17 = 0;
                        //  dword_609F20 = (int)(v17);
                        //  j___cxa_guard_release((__guard*)&byte_609F24);
                    }
                    // *(uint32_t*)dword_609F20 = *(uint32_t*)(a1 + 4 * *(uint32_t*)(a1 + 0x40) + 0x30);
                    //     v18 = nullsub_16(&v37);
                    //     sub_55754(v18, 0xFFFFFFFF);
                    //     sub_57158(dword_609F20, 0, 0, 0x44000000, 0x44000000, &v37);
                    *(uint32_t*)((char*)0x61572C + v13) |= v14 & 0x8000000 | 0x800000;
                    v15 = g_libGTASA;
                    //  Log("testcam 12");
                }
                //  v19 = /*v6[1]*/(int)CSkyBox::GetSkyObject();
                //	Log("testcam 12 0");
                //  v38 = (float)((float)((float)*(unsigned int*)(0x63E3C4 + v15) + 150.0) / 150.0) * 0.8;// scale sky
                //	Log("testcam 12 1");
                v37 = v38;
                //	Log("testcam 12 2 %f", v37);
                ///   v39 = v38;
                ///   ((void ( *)(int, float*, uint32_t))(0x1AED7D + v15/* + 0x18*/))(v19, &v37, 0);
                //Log("testcam 12 3");
                ///  v36 = 0x3F800000;
                ///  v35 = 0LL;
                //	Log("testcam 13");
                //    ((void (__fastcall *)(int, int64_t*, uint32_t, int))(0x1AED7D + g_libGTASA + 0x48))( //rotate sky
                //     v19,
                //     &v35,
                //     *(float*)&dword_6295CC * 360.0,
                //    1);
                //   v34[0] = *(uint32_t*)((char*)g_libGTASA + 0x30);
                //    v34[1] = *(uint32_t*)((char*)g_libGTASA + 0x34);
                //   v34[2] = *(uint32_t*)((char*)g_libGTASA + 0x38);
                //((void (__fastcall *)(int, int*, int))(0x1AED7D + g_libGTASA))(v19, v34, 2); //rw translate
                /*  ((void (*)(void))((char*)g_libGTASA + 0x559EF9))();
                  ((void (*)(void))((char*)g_libGTASA + 0x559FC9))();
                  v20 = 0.0;
              //	Log("testcam 14");
               //   v21 = *(uint32_t*)(a1 + 0x40);
                  v22 = 0.0;
                  if ( v21 == 3 )
                    v22 = 4.0;
                  if ( !v21 )
                  {
                    v20 = 5.0;
                    v22 = 11.0;
                  }
                  if ( v21 == 1 )
                    v20 = 12.0;
                  if ( v21 == 1 )
                    v22 = 18.0;
                  if ( v21 == 2 )
                  {
                    v20 = 19.0;
                    v22 = 24.0;
                  }
              //	Log("testcam 15");
             //     *(float*)(a1 + 0x44) = (float)((float)*(unsigned char*)(g_libGTASA + 0x8B18A5) - v20) / (float)(v22 - v20);
                  if ( !dword_609F28 )
                  {
                 //   v23 = reinterpret_cast<void (__fastcall ***)(uint32_t, uint32_t)>(::operator new(0xCu));
                 //   sub_6FD48();
                 //   v24 = *v23;
                //    dword_609F28 = reinterpret_cast<int>(v23);
                //    (*v24)(v23, 0);
                   // sub_6F580((uintptr_t*)dword_748F98, dword_609F28);
                  //  v25 = reinterpret_cast<void (__fastcall ***)(uint32_t, uint32_t)>(::operator new(0xCu));
                 //   sub_6FE14();
                  //  v26 = *v25;
                  //  dword_609F2C = reinterpret_cast<int>(v25);
               //     (*v26)(v25, 0);
                  //  sub_6F580((uintptr_t*)dword_748F98, dword_609F2C);
              //	  Log("testcam 16");
                  }
                  if ( (byte_61572E[v13] & 0x80) != 0 )
                    v27 = &dword_609F2C;
                  else
                    v27 = &dword_609F28;
                  //sub_6F6A0((uintptr_t*)dword_748F98, *v27);
                //  v28 = *(uint32_t*)(a1 + 0x40);
                  v29 = v28 + 1;
                  //Log("testcam 17");
                  if ( v28 > 2 )
                    v29 = 0;
               //   v30 = a1 + 4 * v29;
               //   if ( a2 )
              //      v31 = (uint32_t*)(v30 + 0x30);
               //   else
               //     v31 = (uint32_t*)(v30 + 0x20);
               // Log("testcam 133");
              //    sub_6EC24(*v31, 1);
              ///	Log("testcam 441");
                //  sub_58518(v6);
              //	Log("testcam 18");
                //  sub_6F706((char*)dword_748F98);*/
            }
        }
    }
}

extern CNetGame* pNetGame;
extern CGame* pGame;
void sub_6ED20(int a1, uintptr_t* a2, uintptr_t* a3, int a4, int a5, int a6)
{
    int v8 = g_libGTASA;
    int v9;
    float *v10;
    float v11;
    int v12;
    int v13;
    int *v144;
    bool v14;
    int v16;
    int v17;
    int v18;
    int v19;
    float v166;
    float v155;
    VECTOR v77;
    int v222; // r0
    int v233; // [sp+8h] [bp+0h]
    float v288; // [sp+24h] [bp+1Ch]

    int v20;
    float v21;
    int v22[9];
//Log("1");
    if (a2)
    {
        v9 = *(int*)(g_libGTASA + 0x95B064);
        v17 = *(int*)(v9 + 0x60);
        v16 = *(int*)(v9 + 0x64);
        *(int*)(g_libGTASA + 0x9B4CB8) = 3;
        *(uintptr_t*)(v9 + 0x64) = *a3;
        *(uintptr_t*)(v9 + 0x60) = *a2;
        *(int *)((char *)0x61572C + v8) |= a1;
        v22[0] = 0xFFFFFFFF;
        ((void (__fastcall *)(int, int *, int))((char *)0x1AD8A1 + g_libGTASA))(v9, v22, 3);
//Log("11");
        // if (!*(int *)(dword_207F50 + 0x12C))
        //  {
        //   nullsub_16(&v19);
        sub_55718(
                &v19,
                *(uint8_t *)(g_libGTASA + 0x8C9AB4),
                *(uint8_t *)(g_libGTASA + 0x8C9AB6),
                *(uint16_t *)(g_libGTASA + 0x8C9AB8),
                0xFF);
        sub_5578E(&v19);

        ((void ( *)(int, int *, int))((char *)0x1AD8A1 + g_libGTASA))(v9, (int*)&v19, 3);
        //  }
//Log("2");
        if (((int (__fastcall *)(int))(g_libGTASA + 0x463DF1))(v9) == 1)
        {
            *(int *)(g_libGTASA + 0x95B064 + 0x84) = *(int *)(g_libGTASA + 0x9B4C2C);
            *(float *)(g_libGTASA + 0x95B064 + 0x88) = *(float *)(g_libGTASA + 0x9B4C2C) * 100.0; //in gtasa

            *(uint8_t *)(v8 + 0x9B4CB2) = 1;
            v21 = 0.0;
            v19 = 0;
            v20 = 0;
            //  Log("22");

            /* if (sub_57DE0())
             {
                 Log("sub_57DE0 succes,  = %d", (int)sub_57DE0());
               v10 = *(float **)(/*sub_57DE0() + 0x14*//*g_libGTASA + 0x8a1144);
		if (v10)
        {
			Log("v10 succes,  = %d", (int)v10);
			Log("666");
          v11 = (float)*((int *)v10 + 0xC);
		  Log("5e55");
          v12 = *((int *)v10 + 0xD);
		  Log("66666");
          v21 = v10[0xE];
		  Log("42343");
          v19 = v11;
		  Log("34234");
          v20 = v12;
        }
      }*/

            v77.Z = 0.0f;
            if(pChatWindow->isConnect){
                if ( g_libGTASA + 0x8A1144 )
                {
                    /*	  if (g_libGTASA + 0x8B081C)
                            v144 = (int *)(g_libGTASA + 0x8B081C + 0x30);
                          else
                            v144 = (int *)(g_libGTASA + 0x8B080C);
                          v155 = *((float *)v144 + 1);
                          v166 = *((float *)v144 + 2);
                          v77.X = *v144;
                          v77.Y = v155;
                          v288 = v166;*/
                    // CPlayerPed* pPed = pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed();
                    //pPed->GetBonePosition(4, &v77);
                    v77 = *(VECTOR*)(0x8C1638 + g_libGTASA);
                }
            }
            //Log("3");
            if ((a1 & 0x8000000) == 0)
                v21 = v21 + 2.0;
            //v77.Z += 0.6f;
            float cameraPosition[3] = {v77.X, v77.Y, v77.Z};
            ((void ( *)(float [3]))(0x197E0C + g_libGTASA + 1))(cameraPosition);
            // Log("v77 = %f", v77.X);


            if (*(int *)(g_libGTASA + 0x8B7D28))
            {
                v18 = 0;
                ((void (__fastcall *)(int, int *, int))((char *)0x1AD8A1 + g_libGTASA))(v9, &v18, 2);
                ((void (__fastcall *)(int, int *, int))((char *)0x1AD8A1 + g_libGTASA))(v9, &v18, 3);
                a4 = 1;
                v13 = a6;
            }
            else
            {
                v13 = a6;
                //    if (dword_609F1C && !a6 /*&& *(int *)(dword_207F50 + 0x12C*/)
                //  ((void (*)(void))sub_3F614)();

            }
            ((void (*)(void))(g_libGTASA + 0x559009))();
            v14 = (a4 & (v13 ^ 1)) == 0;
            if ((a4 & (v13 ^ 1)) == 0)
                v14 = a5 == 0;

            if (v14)

            {
                if (/**(int *)(dword_207F50 + 0x12C) && */dword_609F1C)
                    sub_3F614(dword_609F1C, 0);
                ((void (*)(void))(g_libGTASA + 0x3B3B41))();
                ((void (*)(void))(g_libGTASA + 0x3B1609))();
                ((void (*)(void))(g_libGTASA + 0x3B1779))();
            }

            ((void ( *)(int))((char *)0x1AD6B9 + g_libGTASA))(v9);

            *(uint8_t *)(v8 + 0x9B4CB2) = 0;
        }
        *(int *)((char *)0x61572C + v8) &= ~a1;
        *(int *)(v9 + 0x64) = v16;
        *(int *)(v9 + 0x60) = v17;
        *(int *)(g_libGTASA + 0x9B4CB8) = 0;
    }

}
void (*MobileMenu_InitForPause)(uintptr_t* thiz);
void MobileMenu_InitForPause_hook(uintptr_t* thiz)
{

    return g_pJavaWrapper->ShowNotification(2, "work", 10, "", "");
}
int sub_6F04C()
{
    unsigned int v1;
    int v2;
    int v3;
    int result;
    int settings_WaterReflLvl = 3;

    //  if ( *(uintptr_t *)(g_libGTASA + 0x63E464) )
    //{
    //Log("1");
    ((void (*)())(g_libGTASA + 0x54DE1D))();// CMirrors::CreateBuffer(CMirrors *this)
    // }
    v1 = *(uintptr_t*)(g_libGTASA + 0x63E464);
    if ( v1 >= 1 )
        //	Log("2");
        sub_6ED20(0x800000, *(uintptr_t**)(g_libGTASA + 0x9B4CD0), *(uintptr_t**)(g_libGTASA + 0x9B4CD4), v1 == 2, 0, v1 == 3);
    v3 = settings_WaterReflLvl;
    if ( v3 >= 1 )                                // water
    {
        if ( v3 != 1 )
            v3 = 0;
        //	Log("3");
        sub_6ED20(0x8800000, *(uintptr_t**)dword_748F90, *(uintptr_t**)dword_748F94, v3, 0, 0);
        if ( settings_WaterReflLvl >= 3 )
        {
            //	Log("4");
            sub_6ED20(0x8000000, *(uintptr_t**)dword_748F88, *(uintptr_t**)dword_748F8C, 0, 0, 0);
        }
    }
    result = 1;
    if ( result ){
        //Log("5");
        sub_6ED20(0, *(uintptr_t**)dword_748F80, *(uintptr_t**)dword_748F84, 0, 1, 0);
        return 0;
    }

    return result;
}


void CVehicle__DoHeadLightReflectionTwin(CVehicle* pVeh, MATRIX4X4* a2) {
    VEHICLE_TYPE *v2; // r4
    int v3; // r2
    MATRIX4X4 *v4; // r5
    float *v5; // r3
    float v6; // s12
    float v7; // s5
    float *v8; // r2
    float v9; // r0
    float v10; // r1
    float v11; // r2
    float v12; // s14
    float v13; // s11
    float v14; // s15
    float v15; // s13
    float v16; // s10
    float v17; // s12
    float v18; // s15
    float v19; // ST08_4

    uintptr_t *dwModelarray = (uintptr_t *) (g_libGTASA + 0x87BF48);

    v2 = pVeh->m_pVehicle;
    v3 = *((uintptr_t *) v2 + 5);
    v4 = a2;
    v5 = *(float **) (dwModelarray[v2->entity.nModelIndex] + 116);
    v6 = *v5;
    v7 = v5[1];
    if (v3)
        v8 = (float *) (v3 + 48);
    else
        v8 = (float *) ((char *) v2 + 4);
    v9 = *v8;
    v10 = v8[1];
    v11 = v8[2];
    v12 = *((float *) v4 + 5);
    v13 = *((float *) v4 + 4);
    v14 = (float) (v12 * v12) + (float) (v13 * v13);
    if (v14 != 0.0)
        v14 = 1.0 / sqrtf(v14);
    v15 = v6 * 4.0;
    v16 = (float) (v15 + v15) + 1.0;
    v17 = v13 * v14;
    v18 = v12 * v14;

    v19 = v15 * v18;

    VECTOR pos;
    memcpy(&pos, &(v2->entity.mat->pos), sizeof(VECTOR));
    pos.Z += 2.0f;



        CShadows__StoreCarLightShadow(
		v2,
		(uintptr_t)v2 + 24,
		pVeh->m_Shadow.pTexture,
		&pos,
		(float)(v15 + v15) * v17 * pVeh->m_Shadow.fSizeX,
		(float)(v15 + v15) * v18 * pVeh->m_Shadow.fSizeX,
		v19 * pVeh->m_Shadow.fSizeY,
		-(float)(v15 * v17) * pVeh->m_Shadow.fSizeY,
		pVeh->m_Shadow.r, pVeh->m_Shadow.g, pVeh->m_Shadow.b,
		7.0f);
}
int *__fastcall sub_3F028(int *a1)
{
    a1[0x10] = 0;
    NOP(0x39A844 + g_libGTASA + 0x518, 2);
    NOP(0x39A844 + g_libGTASA, 2);
    NOP(0x39A844 + g_libGTASA + 0x540, 2);
    //NOP(g_libGTASA + 0x53E4EE, 2);
    // NOP(g_libGTASA + 0x53E35E, 2);
    memset(a1, 0, 0x40u);
    // sub_3F0C0(a1, 0);
    // sub_3F0C0(a1, 1);
    //sub_3F0C0(a1, 2);
    // sub_3F0C0(a1, 3);
    return a1;
}
void sub_6F160(){
    int *v2;
    RwRaster* result;
    dword_748F80 = RwRasterCreate(256, 256, 32, 1285);
    v2 = (int *)operator new(0x48u);
    sub_3F028(v2);
    dword_609F1C = (uintptr_t*)v2;
    dword_748F84 = RwRasterCreate(256, 256, 0, 1);
    dword_748F88 = RwRasterCreate(256, 256, 32, 1285);
    dword_748F8C = RwRasterCreate(256, 100, 0, 1);
    dword_748F90 = RwRasterCreate(256, 256, 32, 1285);
//	dword_748C88 = *(RwRaster**)LoadTextureFromDB("mobile", "BR_Nebo_Day");
    //  dword_748C90 = *(RwRaster**)LoadTextureFromDB("mobile", "waterDUDV");
    dword_748C98 = RwRasterCreate(256, 256, 32, 1285);
    dword_748C94 = RwRasterCreate(256, 256, 0, 1);
    dword_748C9C = RwRasterCreate(256, 256, 0, 1);
    //dword_748C8C = /*RwRasterCreate(256, 256, 0, 1);*/*(RwRaster**)LoadTextureFromDB("mobile", "BR_Nebo_Day");
    result = RwRasterCreate(100, 100, 0, 1);
    dword_748F94 = result;
    // dword_7BEE74 = g_libGTASA + 0x95B068;
    dword_207F24 = (uint32_t*)(g_libGTASA+0x8B1104);
}
int *__fastcall sub_3F028(int *a1);

uint8_t jj = 0;
void updateWater();
void (*CMirrors__RenderReflections)(uintptr_t* thiz);
void CMirrors__RenderReflections_hook(uintptr_t* thiz){
    //
    if (jj != 0) {
        //  CMirrors__RenderReflections(thiz);
        sub_6F04C();
        return ;
    }


    sub_6F160();
    jj = 1;
    /*updateWater();
    if (UniformLocationWaveness != -1) {
                waterwave += 0.03;

            glUniform1f(glGetUniformLocation(*(uintptr_t *)(UniformLocationWaveness + 1000), "Waveness"), waterwave);
            Log("waterwave %f", waterwave);
        }*/
    // Или любое другое значение, которое вы хотите установить


    //g_WaterShaderClass[4] += 0.03f;


    //   CMirrors__RenderReflections(thiz);

    sub_6F04C();

    return ;
}

void (*CVehicle__DoHeadLightBeam)(VEHICLE_TYPE* vehicle, int arg0, MATRIX4X4& matrix, unsigned char arg2);
void CVehicle__DoHeadLightBeam_hook(VEHICLE_TYPE* vehicle, int arg0, MATRIX4X4& matrix, unsigned char arg2)
{
	uint8_t r, g, b;
	r = 0xFF;
	g = 0xFF;
	b = 0xFF;
	if (pNetGame)
	{
		if (pNetGame->GetVehiclePool())
		{
			uint16_t vehid = pNetGame->GetVehiclePool()->FindIDFromGtaPtr(vehicle);
			CVehicle* pVeh = pNetGame->GetVehiclePool()->GetAt(vehid);
			if (pVeh)
			{
				pVeh->ProcessHeadlightsColor(r, g, b);
			}
		}
	}

	*(uint8_t*)(g_libGTASA + 0x9BAA70) = r;
	*(uint8_t*)(g_libGTASA + 0x9BAA71) = g;
	*(uint8_t*)(g_libGTASA + 0x9BAA72) = b;

	*(uint8_t*)(g_libGTASA + 0x9BAA94) = r;
	*(uint8_t*)(g_libGTASA + 0x9BAA95) = g;
	*(uint8_t*)(g_libGTASA + 0x9BAA96) = b;

	*(uint8_t*)(g_libGTASA + 0x9BAB00) = r;
	*(uint8_t*)(g_libGTASA + 0x9BAB01) = g;
	*(uint8_t*)(g_libGTASA + 0x9BAB02) = b;

	CVehicle__DoHeadLightBeam(vehicle, arg0, matrix, arg2);

}

static CVehicle* g_pLastProcessedVehicleMatrix = nullptr;
static int g_iLastProcessedWheelVehicle = -1;

void (*CMatrix__Rotate)(void* thiz, float a1, float a2, float a3);
void CMatrix__Rotate_hook(void* thiz, float a1, float a2, float a3)
{
	uintptr_t dwRetAddr = 0;
	__asm__ volatile ("mov %0, lr" : "=r" (dwRetAddr));
	dwRetAddr -= g_libGTASA;

	if (dwRetAddr == 0x003A9D76 + 1)
	{
		CMatrix__Rotate(thiz, a1, a2, a3);
		return;
	}

	CMatrix__Rotate(thiz, a1, a2, a3);
	if (g_pLastProcessedVehicleMatrix && g_iLastProcessedWheelVehicle != -1)
	{
		if (g_pLastProcessedVehicleMatrix->m_bWheelAlignmentX || g_pLastProcessedVehicleMatrix->m_bWheelAlignmentY)
		{
			if (g_iLastProcessedWheelVehicle == 2)
			{
				((void(*)(void*, float))(g_libGTASA + 0x003E8BE4 + 1))(thiz, 0.0f - g_pLastProcessedVehicleMatrix->m_fWheelAlignmentX); // CMatrix::RotateY
			}
			if (g_iLastProcessedWheelVehicle == 4)
			{
				((void(*)(void*, float))(g_libGTASA + 0x003E8BE4 + 1))(thiz, 0.0f - g_pLastProcessedVehicleMatrix->m_fWheelAlignmentY); // CMatrix::RotateY
			}
			if (g_iLastProcessedWheelVehicle == 5)
			{
				((void(*)(void*, float))(g_libGTASA + 0x003E8BE4 + 1))(thiz, g_pLastProcessedVehicleMatrix->m_fWheelAlignmentX); // CMatrix::RotateY
			}
			if (g_iLastProcessedWheelVehicle == 7)
			{
				((void(*)(void*, float))(g_libGTASA + 0x003E8BE4 + 1))(thiz, g_pLastProcessedVehicleMatrix->m_fWheelAlignmentY); // CMatrix::RotateY
			}
		}
	}
}

void (*CMatrix__SetScale)(void* thiz, float x, float y, float z);
void CMatrix__SetScale_hook(void* thiz, float x, float y, float z)
{
	if (g_pLastProcessedVehicleMatrix && g_iLastProcessedWheelVehicle != -1)
	{
		if (g_iLastProcessedWheelVehicle >= 2 || g_iLastProcessedWheelVehicle <= 7)
		{
			// front wheel
			if (g_pLastProcessedVehicleMatrix->m_bWheelSize)
			{
				y *= g_pLastProcessedVehicleMatrix->m_fWheelSize * 1.3f; //                   scale          
				z *= g_pLastProcessedVehicleMatrix->m_fWheelSize * 1.3f;
			}
			if (g_pLastProcessedVehicleMatrix->m_bWheelWidth)
			{
				x = g_pLastProcessedVehicleMatrix->m_fWheelWidth;
			}
		}
	}

	CMatrix__SetScale(thiz, x, y, z);
}

void (*CAutomobile__UpdateWheelMatrix)(VEHICLE_TYPE* thiz, int, int);
void CAutomobile__UpdateWheelMatrix_hook(VEHICLE_TYPE* thiz, int nodeIndex, int flags)
{
	if (g_pLastProcessedVehicleMatrix)
	{
		g_iLastProcessedWheelVehicle = nodeIndex;
	}

	CAutomobile__UpdateWheelMatrix(thiz, nodeIndex, flags);
}

void (*CAutomobile__PreRender)(VEHICLE_TYPE* thiz);
void CAutomobile__PreRender_hook(VEHICLE_TYPE* thiz)
{
	uintptr_t* dwModelarray = (uintptr_t*)(g_libGTASA + 0x87BF48);
	uint8_t* pModelInfoStart = (uint8_t*)dwModelarray[thiz->entity.nModelIndex];

	float fOldFront = *(float*)(pModelInfoStart + 88);
	float fOldRear = *(float*)(pModelInfoStart + 92);
	CVehicle* pVeh = nullptr;
	if (pNetGame)
	{
		if (pNetGame->GetVehiclePool())
		{
			uint16_t vehid = pNetGame->GetVehiclePool()->FindIDFromGtaPtr(thiz);
			pVeh = pNetGame->GetVehiclePool()->GetAt(vehid);
			if (pVeh)
			{
				pVeh->ProcessWheelsOffset();
				g_pLastProcessedVehicleMatrix = pVeh;

				if (pVeh->m_bWheelSize)
				{
					*(float*)(pModelInfoStart + 92) = pVeh->m_fWheelSize;
					*(float*)(pModelInfoStart + 88) = pVeh->m_fWheelSize;
				}
				if (pVeh->m_bShadow && pVeh->m_Shadow.pTexture)
				{
					CVehicle__DoHeadLightReflectionTwin(pVeh, pVeh->m_pVehicle->entity.mat);
				}
			}
		}
	}

	CAutomobile__PreRender(thiz);

	*(float*)(pModelInfoStart + 88) = fOldFront;
	*(float*)(pModelInfoStart + 92) = fOldRear;

	g_pLastProcessedVehicleMatrix = nullptr;
	g_iLastProcessedWheelVehicle = -1;
}

#include "..//cryptors/INSTALLHOOKS_result.h"

void (*CTaskSimpleUseGun__RemoveStanceAnims)(void* thiz, void* ped, float a3);
void CTaskSimpleUseGun__RemoveStanceAnims_hook(void* thiz, void* ped, float a3)
{
	uint32_t v5 = *((uint32_t*)thiz + 11);
	if (v5)
	{
		if (*(uint32_t*)(v5 + 20))
		{
			CTaskSimpleUseGun__RemoveStanceAnims(thiz, ped, a3);
			return;
		}
		else
		{
			return;
		}
	}
	else
	{
		CTaskSimpleUseGun__RemoveStanceAnims(thiz, ped, a3);
	}
}

float (*CRadar__LimitRadarPoint)(float* a1);
float CRadar__LimitRadarPoint_hook(float* a1)
{
	if (*(uint8_t*)(g_libGTASA + 0x0063E0B4))
	{
		return sqrtf((float)(a1[1] * a1[1]) + (float)(*a1 * *a1));
	}

	if (!CRadarRect::IsEnabled())
	{
		return CRadar__LimitRadarPoint(a1);
	}
	float value = CRadarRect::CRadar__LimitRadarPoint_hook(a1);

	if (pChatWindow)
	{
		//pChatWindow->AddDebugMessage("VALUE %f", value);
	}

	return value;
}

void (*CSprite2d__DrawBarChart)(float x, float y, unsigned short width, unsigned char height, float progress, signed char progressAdd,
	unsigned char drawPercentage, unsigned char drawBlackBorder, CRGBA* color, CRGBA* addColor);
void CSprite2d__DrawBarChart_hook(float x, float y, unsigned short width, unsigned char height, float progress, signed char progressAdd,
	unsigned char drawPercentage, unsigned char drawBlackBorder, CRGBA* color, CRGBA* addColor)
{
	uintptr_t dwRetAddr = 0;
	__asm__ volatile ("mov %0, lr" : "=r" (dwRetAddr));
	dwRetAddr -= g_libGTASA;

	float fX = x;
	float fY = y;

	unsigned short usWidth = width;
	unsigned char usHeight = height;

	if (dwRetAddr == 0x0027D524 + 1)
	{
		if (CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_HP).X != -1)
		{
			fX = pGUI->ScaleX(CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_HP).X);
		}
		if (CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_HP).Y != -1)
		{
			fY = pGUI->ScaleY(CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_HP).Y);
		}

		if (CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_HP).X != -1)
		{
			usWidth = pGUI->ScaleX(CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_HP).X);
		}
		if (CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_HP).Y != -1)
		{
			usHeight = pGUI->ScaleY(CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_HP).Y);
		}

		if (CAdjustableHudColors::IsUsingHudColor(E_HUD_ELEMENT::HUD_HP))
		{
			color->A = CAdjustableHudColors::GetHudColor(E_HUD_ELEMENT::HUD_HP).A;
			color->R = CAdjustableHudColors::GetHudColor(E_HUD_ELEMENT::HUD_HP).R;
			color->G = CAdjustableHudColors::GetHudColor(E_HUD_ELEMENT::HUD_HP).G;
			color->B = CAdjustableHudColors::GetHudColor(E_HUD_ELEMENT::HUD_HP).B;
		}
		color->A = 0;
		color->R = CAdjustableHudColors::GetHudColor(E_HUD_ELEMENT::HUD_HP).R;
		color->G = CAdjustableHudColors::GetHudColor(E_HUD_ELEMENT::HUD_HP).G;
		color->B = CAdjustableHudColors::GetHudColor(E_HUD_ELEMENT::HUD_HP).B;
	}
	else if (dwRetAddr == 0x0027D83E + 1)
	{
		if (CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_ARMOR).X != -1)
		{
			fX = pGUI->ScaleX(CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_ARMOR).X);
		}
		if (CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_ARMOR).Y != -1)
		{
			fY = pGUI->ScaleY(CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_ARMOR).Y);
		}

		if (CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_ARMOR).X != -1)
		{
			usWidth = pGUI->ScaleX(CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_ARMOR).X);
		}
		if (CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_ARMOR).Y != -1)
		{
			usHeight = pGUI->ScaleY(CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_ARMOR).Y);
		}

		if (CAdjustableHudColors::IsUsingHudColor(E_HUD_ELEMENT::HUD_ARMOR))
		{
			color->A = CAdjustableHudColors::GetHudColor(E_HUD_ELEMENT::HUD_ARMOR).A;
			color->R = CAdjustableHudColors::GetHudColor(E_HUD_ELEMENT::HUD_ARMOR).R;
			color->G = CAdjustableHudColors::GetHudColor(E_HUD_ELEMENT::HUD_ARMOR).G;
			color->B = CAdjustableHudColors::GetHudColor(E_HUD_ELEMENT::HUD_ARMOR).B;
		}
		color->A = 0;
		color->R = CAdjustableHudColors::GetHudColor(E_HUD_ELEMENT::HUD_ARMOR).R;
		color->G = CAdjustableHudColors::GetHudColor(E_HUD_ELEMENT::HUD_ARMOR).G;
		color->B = CAdjustableHudColors::GetHudColor(E_HUD_ELEMENT::HUD_ARMOR).B;
	}
	CSprite2d__DrawBarChart(fX, fY, usWidth, usHeight, progress, progressAdd, drawPercentage, drawBlackBorder, color, addColor);
}
static int g_iCurrentWanted = 0;
static float g_fInitialPos = 0.0f;
void (*CWidgetPlayerInfo__DrawWanted)(void*);
void CWidgetPlayerInfo__DrawWanted_hook(void* thiz)
{
	g_iCurrentWanted = 0;
	g_fInitialPos = *((float*)thiz + 10);
	CWidgetPlayerInfo__DrawWanted(thiz);
	g_iCurrentWanted = 0;
}

void (*CFont__PrintString)(float x, float y, uint16_t* text);
void CFont__PrintString_hook(float x, float y, uint16_t* text)
{
	uintptr_t dwRetAddr = 0;
	__asm__ volatile ("mov %0, lr" : "=r" (dwRetAddr));
	dwRetAddr -= g_libGTASA;

	float fX = x;
	float fY = y;

	if (dwRetAddr == 0x0027E15C + 1) // money
	{
		if (CAdjustableHudColors::IsUsingHudColor(E_HUD_ELEMENT::HUD_MONEY))
		{
			CRGBA col = CAdjustableHudColors::GetHudColor(E_HUD_ELEMENT::HUD_MONEY);
			uint32_t m1 = col.ToInt();
			CFont::SetColor(&m1);
		}

		if (CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_MONEY).X != -1)
		{
			float value = (float)CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_MONEY).X / 100.0f;
			CFont::SetScale(value, 0.0f);
		}

		if (CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_MONEY).X != -1)
		{
			fX = pGUI->ScaleX(CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_MONEY).X);
		}
		if (CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_MONEY).Y != -1)
		{
			fY = pGUI->ScaleY(CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_MONEY).Y);
		}
	}
	else if (dwRetAddr == 0x0027D9E6 + 1) // wanted
	{
		if (CAdjustableHudColors::IsUsingHudColor(E_HUD_ELEMENT::HUD_WANTED))
		{
			CRGBA col = CAdjustableHudColors::GetHudColor(E_HUD_ELEMENT::HUD_WANTED);
			uint32_t m1 = col.ToInt();
			CFont::SetColor(&m1);
		}

		if (CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_WANTED).X != -1)
		{
			float value = (float)CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_WANTED).X / 100.0f;
			CFont::SetScale(value, 0.0f);
		}
		
		if (CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_WANTED).X != -1)
		{
			fX -= g_fInitialPos;
			fX += pGUI->ScaleX(CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_WANTED).X);
		}
		if (CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_WANTED).Y != -1)
		{
			fY = pGUI->ScaleY(CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_WANTED).Y);
		}
		g_iCurrentWanted++;

	}
	else if (dwRetAddr == 0x0027D330 + 1) // ammo text
	{
		if (CAdjustableHudColors::IsUsingHudColor(E_HUD_ELEMENT::HUD_AMMO))
		{
			CRGBA col = CAdjustableHudColors::GetHudColor(E_HUD_ELEMENT::HUD_AMMO);
			uint32_t m1 = col.ToInt();
			CFont::SetColor(&m1);
		}

		if (CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_AMMO).X != -1)
		{
			float value = (float)CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_AMMO).X / 100.0f;
			CFont::SetScale(value, 0.0f);
		}

		if (CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_AMMO).X != -1)
		{
			fX = pGUI->ScaleX(CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_AMMO).X);
		}
		if (CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_AMMO).Y != -1)
		{
			fY = pGUI->ScaleY(CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_AMMO).Y);
		}
	}
	CFont__PrintString(fX, fY, text);
}

void (*CSprite2d__Draw)(CSprite2d* a1, CRect* a2, CRGBA* a3);
void CSprite2d__Draw_hook(CSprite2d* a1, CRect* a2, CRGBA* a3)
{
	uintptr_t dwRetAddr = 0;
	__asm__ volatile ("mov %0, lr" : "=r" (dwRetAddr));
	dwRetAddr -= g_libGTASA;

	if (!pGUI)
	{
		return CSprite2d__Draw(a1, a2, a3);
	}

	if (dwRetAddr == 0x003D3796 + 1 || dwRetAddr == 0x003D376C + 1 || dwRetAddr == 0x003D373E + 1 || dwRetAddr == 0x003D3710 + 1)
	{
		if (CRadarRect::m_pDiscTexture == nullptr)
		{
			CRadarRect::m_pDiscTexture = a1->m_pRwTexture;
		}
		if (CRadarRect::IsEnabled() && CRadarRect::m_pRectTexture)
		{
			a1->m_pRwTexture = CRadarRect::m_pRectTexture;
			//pChatWindow->AddDebugMessage("Replacing for rect");
		}
		else
		{
			a1->m_pRwTexture = CRadarRect::m_pDiscTexture;
			//pChatWindow->AddDebugMessage("Replacing for disc");
		}

		if (CAdjustableHudColors::IsUsingHudColor(E_HUD_ELEMENT::HUD_RADAR))
		{
			CRGBA col = CAdjustableHudColors::GetHudColor(E_HUD_ELEMENT::HUD_RADAR);
			a3->A = 255;
			a3->B = 155;
			a3->G = 5;
			a3->R = 7;
		}
		float* thiz = (float*) * (uintptr_t*)(g_libGTASA + 0x6580C8);
		if (thiz)
		{
			if (CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_RADAR).X != -1)
			{
				thiz[3] = (float)CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_RADAR).X;
			}
			else
			{
				thiz[3] = 50.0f;
			}

			if (CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_RADAR).Y != -1)
			{
				thiz[4] = (float)CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_RADAR).Y;
			}
			else
			{
				thiz[4] = 70.0f;
			}

			if (CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_RADAR).X != -1)
			{
				thiz[5] = 45.0f * ((float)CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_RADAR).X / 100.0f);
			}
			else
			{
				thiz[5] = 45.0f;
			}

			if (CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_RADAR).X != -1)
			{
				thiz[6] = 45.0f * ((float)CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_RADAR).X / 100.0f);
			}
			else
			{
				thiz[6] = 45.0f;
				
			CRect* rect = (CRect*)&thiz[3];
            pGUI->CoordsRadar(rect);
			}
		}
		
	}

	return CSprite2d__Draw(a1, a2, a3);
}

void (*CWidgetPlayerInfo__DrawWeaponIcon)(float* thiz, void* ped, CRect rect, float a4);
void CWidgetPlayerInfo__DrawWeaponIcon_hook(float* thiz, void* ped, CRect rect, float a4)
{
	float diffX = rect.right - rect.left;
	float diffY = rect.bottom - rect.top;
	if (CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_FIST).X != -1)
	{
		rect.left = pGUI->ScaleX(CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_FIST).X);
		rect.right = pGUI->ScaleX(CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_FIST).X) + diffX;

		thiz[38] = rect.left;
		thiz[40] = rect.right;
	}
	if (CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_FIST).Y != -1)
	{
		rect.top = pGUI->ScaleY(CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_FIST).Y);
		rect.bottom = pGUI->ScaleY(CAdjustableHudPosition::GetElementPosition(E_HUD_ELEMENT::HUD_FIST).Y) + diffY;

		thiz[39] = rect.bottom;
		thiz[41] = rect.top;
	}

	if (CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_FIST).X != -1)
	{
		float coef = (float)CAdjustableHudScale::GetElementScale(E_HUD_ELEMENT::HUD_FIST).X / 100.0f;
		float diffX = rect.right - rect.left;
		float diffY = rect.bottom - rect.top;
		diffX *= coef;
		diffY *= coef;

		rect.right = rect.left + diffX;
		rect.bottom = rect.top + diffY;
	}

	return CWidgetPlayerInfo__DrawWeaponIcon(thiz, ped, rect, a4);
}

void(*CHud__Draw)();
void CHud__Draw_hook()
{
	float* thiz = (float*) * (uintptr_t*)(g_libGTASA + 0x6580C8);
	if (thiz)
	{
		thiz[5] = 45.0f;
		thiz[6] = 45.0f;
	}

	CHud__Draw();
}

class WaterShader {
public:
    static int BuildShadersSource1(uintptr_t*);
    static int EmuShader__Select3(uintptr_t*);
    static void GetTexture_waterDUDV();
    static int sub_ResetEnvMap(int);
};

uintptr_t* g_WaterShaderClass = nullptr;
void (*emu_glEndInternal)();
void emu_glEndInternal_hook()
{
    if (((*(uintptr_t*)(g_libGTASA + 0x61572C)) & 0x80000) != 0)
    {
        if (g_WaterShaderClass == nullptr)
        {
            //getClassWaterShader(&WaterShaderClass);
            //g_WaterShaderClass = new uintptr_t{};
            g_WaterShaderClass = (uintptr_t*)malloc(64);
            memset(g_WaterShaderClass, 1, 64);
            //for (int i = 0; i < 64; ++i) {
            //    *(uintptr_t *)(g_WaterShaderClass + i) = 0;
            //}
            WaterShader::BuildShadersSource1(g_WaterShaderClass);
        }
        else {
            //LOGG(OBFUSCATEV2("Cant Build Shaders"));
        }
        if (g_WaterShaderClass) {
            WaterShader::EmuShader__Select3(g_WaterShaderClass);
            //WaterShader::GetTexture_waterDUDV();
            //LOGG(OBFUSCATEV2("Selecting Shaders"));
        }
        else {
            //LOGG(OBFUSCATEV2("Shaders class is null"));
        }

    }

    emu_glEndInternal();
    //(uintptr_t *)(*(uintptr_t *)(g_libGTASA + 0x61572C) << 12);
    //if ( (((*(uintptr_t *)(g_libGTASA + 0x61572C)) & 0x80000) != 0)  && g_WaterShaderClass )
    //{
    //    LOGG(OBFUSCATEV2("Setting waterDUDV for Shaders"));
    //    WaterShader::GetTexture_waterDUDV();
    //}



}



uintptr_t* BuildShaderSource(uintptr_t* a1, const char* a2, const char* a3, int a4)
{
    char* v7; // r6
    char* v8; // r0

    *(uintptr_t*)(a1 + 17) = 0;
    *(uintptr_t*)(a1 + 5) = a4;
    *(uintptr_t*)(a1 + 9) = ((int(__fastcall*)(const char*))(g_libGTASA + 0x1BC269))(a2);
    *(uintptr_t*)(a1 + 13) = ((int(__fastcall*)(const char*))(g_libGTASA + 0x1BC269))(a3);
    v7 = strdup(a2);
    v8 = strdup(a3);
    *(char*)(a1 + 4) = 1;
    *(uintptr_t**)a1 = ((uintptr_t * (__fastcall*)(char*, char*, uintptr_t))(g_libGTASA + 0x1A3A69))(v7, v8, *(uintptr_t*)(a1 + 5));
    return a1;
}



int  WaterShader::EmuShader__Select3(uintptr_t* a1)
{
    return ((int (*)(uintptr_t*, int))(g_libGTASA + 0x1984A5))(a1, 1);
}

int WaterShader::BuildShadersSource1(uintptr_t* a1)
{
    uintptr_t* v2; // r5
    int result; // r0
    v2 = a1;
    v2 = BuildShaderSource(
            (uintptr_t*)v2,
            "#version 100\n"
            "precision highp float;\n"
            "uniform sampler2D EnvMap;\n"
            "uniform sampler2D DuDvMap;\n"
            "uniform sampler2D FogMap;\n"
            "uniform highp float Waveness;\n"
            "varying mediump vec2 Out_Tex0;\n"
            "varying mediump vec4 Out_ClipSpace;\n"
            "varying vec3 Out_CameraVector;\n"
            "varying mediump vec2 Out_DuDvTexCoord;\n"
            "varying mediump vec3 Out_Refl;\n"
            "varying mediump float Out_FogAmt;\n"
            "void main()\n"
            "{\n"
            "\tmediump vec2 ndc = (Out_ClipSpace.xy/Out_ClipSpace.w) / 2.0 + 0.5;\n"
            "\n"
            "\tlowp vec2 distortedTexCoords = texture2D(DuDvMap, vec2(Out_DuDvTexCoord.x + Waveness, Out_DuDvTexCoord.y)).rg * 0."
            "1;\n"
            "\tdistortedTexCoords = Out_DuDvTexCoord + vec2(distortedTexCoords.x, distortedTexCoords.y + Waveness);\n"
            "\tmediump vec2 distSum = (texture2D(DuDvMap, distortedTexCoords).rg * 2.0 - 1.0) * 0.01;\n"
            "\n"
            "\tmediump vec2 ReflPos = normalize(Out_Refl.xy) * (Out_Refl.z * 0.5 + 0.5); \n"
            "\tReflPos = (ReflPos * vec2(0.5, 0.5)) + vec2(0.5, 0.5); \n"
            "\tReflPos += distSum;\n"
            "\n"
            "\tReflPos = clamp(ReflPos, 0.001, 0.999); \n"
            "\n"
            "\tlowp vec4 reflectTex = texture2D(EnvMap, ReflPos); \n"
            "\n"
            "\tvec3 ViewVector = normalize(Out_CameraVector);\n"
            "\tfloat RefractionCoef = dot(ViewVector, vec3(0.0, 0.0, 1.0));\n"
            "\tRefractionCoef = pow(RefractionCoef, 1.3);\n"
            "\t\n"
            "\tgl_FragColor = reflectTex;\n"
            "\tgl_FragColor.a = 1.0 - RefractionCoef;\n"
            "\tgl_FragColor = mix(gl_FragColor, vec4(0.0, 0.3, 0.5, 1.0), 0.2);\n"
            "}\n",
            "#version 100\n"
            "precision highp float;\n"
            "uniform mat4 ProjMatrix;\n"
            "uniform mat4 ViewMatrix;\n"
            "uniform mat4 ObjMatrix;\n"
            "uniform vec3 CameraPosition;\n"
            "uniform vec3 SunPosition;\n"
            "varying mediump float Out_FogAmt;\n"
            "uniform vec3 FogDistances;\n"
            "attribute vec3 Position;\n"
            "attribute vec2 TexCoord0;\n"
            "varying mediump vec2 Out_Tex0;\n"
            "varying mediump vec4 Out_ClipSpace;\n"
            "varying vec3 Out_CameraVector;\n"
            "varying mediump vec2 Out_DuDvTexCoord;\n"
            "varying mediump vec3 Out_Refl;\n"
            "void main()\n"
            "{\n"
            "\tOut_Tex0 = TexCoord0;\n"
            "\tvec4 WorldPos = ObjMatrix * vec4(Position, 1.0);\n"
            "\tvec4 ViewPos = ViewMatrix * WorldPos;\n"
            "\tOut_ClipSpace = ProjMatrix * ViewPos;\n"
            "\tOut_CameraVector = CameraPosition - WorldPos.xyz;\n"
            "\tOut_DuDvTexCoord = vec2(Position.x/2.0 + 0.5, Position.y/2.0 + 0.5) * 0.2;\n"
            "\t\n"
            "\tvec3 WorldNormal = vec3(0.0, 0.0, 1.0);\n"
            "\tvec3 reflVector = normalize(WorldPos.xyz - CameraPosition.xyz);\n"
            "\treflVector = reflVector - 2.0 * dot(reflVector, WorldNormal) * WorldNormal;\n"
            "\tOut_Refl = reflVector;\n"
            "\tOut_FogAmt = clamp((length(WorldPos.xyz - CameraPosition.xyz) - FogDistances.x) * FogDistances.z, 0.0, 1.0);\n"
            "\t\n"
            "\tgl_Position = Out_ClipSpace;\n"
            "}\n",
            0x1002422);
    result = 1;
    a1 = v2;
    Log("Building Shaders");
    return result;
}


void (*CCam__Process)(uintptr_t);
void CCam__Process_hook(uintptr_t thiz)
{

	if (pChatWindow)
	{
		//pChatWindow->AddDebugMessage("time %d %d %d", *(uint32_t*)(g_libGTASA + 0x008B0808 + 116), *(uint32_t*)(g_libGTASA + 0x008B0808 + 120), *(uint32_t*)(g_libGTASA + 0x008B0808 + 124));
		//pChatWindow->AddDebugMessage("camera %d %f %f", *(uint16_t*)(thiz + 14), *(float*)(thiz + 132), *(float*)(thiz + 148)); // 140 - fov, 132 - vertical, 148 - horizontal angle
	}

	VECTOR vecSpeed;
	CVehicle* pVeh = nullptr;
	float pOld = *(float*)(g_libGTASA + 0x00608558);
	if (pNetGame && (*(uint16_t*)(thiz + 14) == 18 || *(uint16_t*)(thiz + 14) == 16) && CFirstPersonCamera::IsEnabled())
	{
		if (pNetGame->GetPlayerPool())
		{
			if (pNetGame->GetPlayerPool()->GetLocalPlayer())
			{
				CPlayerPed* pPed = pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed();
				pVeh = pNetGame->GetVehiclePool()->GetAt(pNetGame->GetPlayerPool()->GetLocalPlayer()->m_CurrentVehicle);
				if (pVeh)
				{
					pVeh->GetMoveSpeedVector(&vecSpeed);
					VECTOR vec;
					vec.X = vecSpeed.X * 6.0f;
					vec.Y = vecSpeed.Y * 6.0f;
					vec.Z = vecSpeed.Z * 6.0f;
					pVeh->SetMoveSpeedVector(vec);
					*(float*)(g_libGTASA + 0x00608558) = 200.0f;
				}
			}
		}
	}

	CCam__Process(thiz);
	if (pVeh)
	{
		pVeh->SetMoveSpeedVector(vecSpeed);
		*(float*)(g_libGTASA + 0x00608558) = pOld;
	}
	if (*(uint16_t*)(thiz + 14) == 4 || *(uint16_t*)(thiz + 14) == 53) // 53 is weapon
	{
		if (pNetGame)
		{
			if (pNetGame->GetPlayerPool())
			{
				if (pNetGame->GetPlayerPool()->GetLocalPlayer())
				{
					CPlayerPed* pPed = pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed();
					if (pPed)
					{
						*(uint32_t*)(g_libGTASA + 0x008B0808 + 120) = 0xFFFFFFFF;
						*(uint32_t*)(g_libGTASA + 0x008B0808 + 124) = 0xFFFFFFFF;
						*(uint8_t*)(g_libGTASA + 0x008B0808 + 40) = 0;
						CFirstPersonCamera::ProcessCameraOnFoot(thiz, pPed);
					}
				}
			}
		}
	}
	if(* (uint16_t*)(thiz + 14) == 18 || *(uint16_t*)(thiz + 14) == 16) // in car
	{
		if (pNetGame)
		{
			if (pNetGame->GetPlayerPool())
			{
				if (pNetGame->GetPlayerPool()->GetLocalPlayer())
				{
					CPlayerPed* pPed = pNetGame->GetPlayerPool()->GetLocalPlayer()->GetPlayerPed();
					if (pPed)
					{
						*(uint32_t*)(g_libGTASA + 0x008B0808 + 120) = 0xFFFFFFFF;
						*(uint32_t*)(g_libGTASA + 0x008B0808 + 124) = 0xFFFFFFFF;
						*(uint8_t*)(g_libGTASA + 0x008B0808 + 40) = 0;
						CFirstPersonCamera::ProcessCameraInVeh(thiz, pPed, pVeh);
					}
				}
			}
		}
	}
}
int g_iCounterVehicleCamera = 0;
int (*CPad__CycleCameraModeDownJustDown)(void*);
int CPad__CycleCameraModeDownJustDown_hook(void* thiz)
{
	if (!g_pWidgetManager)
	{
		return 0;
	}
	if (!g_pWidgetManager->GetSlotState(WIDGET_CAMERA_CYCLE))
	{
		return 0;
	}
	PED_TYPE* pPed = GamePool_FindPlayerPed();
	if (!pPed)
	{
		return 0;
	}

	static uint32_t lastTick = GetTickCount();
	bool bPressed = false;
	if (g_pWidgetManager->IsTouched(WIDGET_CAMERA_CYCLE) && GetTickCount() - lastTick >= 250)
	{
		bPressed = true;
		lastTick = GetTickCount();
	}

	if (IN_VEHICLE(pPed))
	{
		if (bPressed)
		{
			g_iCounterVehicleCamera++;
		}
		if (g_iCounterVehicleCamera == 6)
		{
			CFirstPersonCamera::SetEnabled(true);
			return 0;
		}
		else if (g_iCounterVehicleCamera >= 7)
		{
			g_iCounterVehicleCamera = 0;
			CFirstPersonCamera::SetEnabled(false);
			return 1;
			
		}
		else
		{
			CFirstPersonCamera::SetEnabled(false);
		}

		return bPressed;
	}
	return 0;
}

int (*CEntity__GetIsOnScreen)(ENTITY_TYPE*);
int CEntity__GetIsOnScreen_hook(ENTITY_TYPE* thiz)
{
	int retn = CEntity__GetIsOnScreen(thiz);

	return retn;
}

void (*FxEmitterBP_c__Render)(uintptr_t* a1, int a2, int a3, float a4, char a5);
void FxEmitterBP_c__Render_hook(uintptr_t* a1, int a2, int a3, float a4, char a5)
{
	uintptr_t* temp = *((uintptr_t**)a1 + 3);
	if (!temp)
	{
		return;
	}
	FxEmitterBP_c__Render(a1, a2, a3, a4, a5);
}

void(*CStreaming__RemoveModel)(int);
void CStreaming__RemoveModel_hook(int model)
{
	Log("Removing model %d", model);
	CStreaming__RemoveModel(model);
}

int g_iLastProcessedSkinCollision = 228;
int g_iLastProcessedEntityCollision = 228;

void (*CPed__ProcessEntityCollision)(PED_TYPE* thiz, ENTITY_TYPE* ent, void* colPoint);
void CPed__ProcessEntityCollision_hook(PED_TYPE* thiz, ENTITY_TYPE* ent, void* colPoint)
{
	g_iLastProcessedSkinCollision = thiz->entity.nModelIndex;
	g_iLastProcessedEntityCollision = ent->nModelIndex;

	CPed__ProcessEntityCollision(thiz, ent, colPoint);
}


int (*CAEVehicleAudioEntity__JustFinishedAccelerationLoop)(uintptr_t* thiz);
int CAEVehicleAudioEntity__JustFinishedAccelerationLoop_hook(uintptr_t* thiz) {
    CVehicle* pVehicle = nullptr;
    CVehiclePool* pVehiclePool = pNetGame->GetVehiclePool();
    CPlayerPed* pPlayerPed = pGame->FindPlayerPed();
    VEHICLEID id = pVehiclePool->FindIDFromGtaPtr(pPlayerPed->GetGtaVehicle());
    pVehicle = pVehiclePool->GetAt(id);
    pVehicle->CreateExhaust(nullptr);
    return CAEVehicleAudioEntity__JustFinishedAccelerationLoop(thiz);
}

void InstallHooks()
{
	PROTECT_CODE_INSTALLHOOKS;
    SetUpHook(g_libGTASA + 0x00281398, (uintptr_t)CWidgetRegionLook__Update_hook, (uintptr_t*)& CWidgetRegionLook__Update);
	SetUpHook(g_libGTASA+0x3D7CA8, (uintptr_t)CLoadingScreen_DisplayPCScreen_hook, (uintptr_t*)&CLoadingScreen_DisplayPCScreen);
	SetUpHook(g_libGTASA+0x39AEF4, (uintptr_t)Render2dStuff_hook, (uintptr_t*)&Render2dStuff);
	SetUpHook(g_libGTASA+0x39B098, (uintptr_t)Render2dStuffAfterFade_hook, (uintptr_t*)&Render2dStuffAfterFade);
	SetUpHook(g_libGTASA+0x239D5C, (uintptr_t)TouchEvent_hook, (uintptr_t*)&TouchEvent);
	SetUpHook(g_libGTASA+0x28E83C, (uintptr_t)CStreaming_InitImageList_hook, (uintptr_t*)&CStreaming_InitImageList);
	SetUpHook(g_libGTASA+0x336690, (uintptr_t)CModelInfo_AddPedModel_hook, (uintptr_t*)&CModelInfo_AddPedModel); // hook is dangerous
	SetUpHook(g_libGTASA+0x3DBA88, (uintptr_t)CRadar__GetRadarTraceColor_hook, (uintptr_t*)&CRadar__GetRadarTraceColor); // dangerous
	SetUpHook(g_libGTASA+0x3DAF84, (uintptr_t)CRadar__SetCoordBlip_hook, (uintptr_t*)&CRadar__SetCoordBlip);
	SetUpHook(g_libGTASA+0x3DE9A8, (uintptr_t)CRadar__DrawRadarGangOverlay_hook, (uintptr_t*)&CRadar__DrawRadarGangOverlay);


	SetUpHook(g_libGTASA+0x482E60, (uintptr_t)CTaskComplexEnterCarAsDriver_hook, (uintptr_t*)&CTaskComplexEnterCarAsDriver);
	SetUpHook(g_libGTASA+0x4833CC, (uintptr_t)CTaskComplexLeaveCar_hook, (uintptr_t*)&CTaskComplexLeaveCar);
	//SetUpHook(g_libGTASA + 0x0044A4CC, (uintptr_t)PointGunInDirection_hook, (uintptr_t*)&PointGunInDirection);
	CodeInject(g_libGTASA+0x2D99F4, (uintptr_t)PickupPickUp_hook, 1);
	SetUpHook(g_libGTASA + 0x00327528, (uintptr_t)ComputeDamageResponse_hooked, (uintptr_t*)(&ComputeDamageResponse));

	SetUpHook(g_libGTASA + 0x00336268, (uintptr_t)CModelInfo_AddAtomicModel_hook, (uintptr_t*)& CModelInfo_AddAtomicModel);

	SetUpHook(g_libGTASA + 0x00336618, (uintptr_t)CModelInfo_AddVehicleModel_hook, (uintptr_t*)& CModelInfo_AddVehicleModel); // dangerous

	SetUpHook(g_libGTASA + 0x0033DA5C, (uintptr_t)CAnimManager__UncompressAnimation_hook, (uintptr_t*)& CAnimManager__UncompressAnimation);
	SetUpHook(g_libGTASA + 0x00531118, (uintptr_t)CCustomRoadsignMgr__RenderRoadsignAtomic_hook, (uintptr_t*)& CCustomRoadsignMgr__RenderRoadsignAtomic);
	SetUpHook(g_libGTASA + 0x001AECC0, (uintptr_t)RwFrameAddChild_hook, (uintptr_t*)& RwFrameAddChild);
	SetUpHook(g_libGTASA + 0x002DFD30, (uintptr_t)CUpsideDownCarCheck__IsCarUpsideDown_hook, (uintptr_t*)& CUpsideDownCarCheck__IsCarUpsideDown);
	SetUpHook(g_libGTASA + 0x0033AD78, (uintptr_t)CAnimBlendNode__FindKeyFrame_hook, (uintptr_t*)& CAnimBlendNode__FindKeyFrame);

    SetUpHook(g_libGTASA + 0x18DAAC, (uintptr_t)emu_glEndInternal_hook, (uintptr_t*)&emu_glEndInternal);//water


    // TextDraw render
	SetUpHook(g_libGTASA + 0x003D5894, (uintptr_t)CHud__DrawScriptText_hook, (uintptr_t*)& CHud__DrawScriptText);

	//widgets
	SetUpHook(g_libGTASA + 0x00276510, (uintptr_t)CWidgetButtonEnterCar_Draw_hook, (uintptr_t*)& CWidgetButtonEnterCar_Draw);

	// attached objects
	SetUpHook(g_libGTASA + 0x003C1BF8, (uintptr_t)CWorld_ProcessPedsAfterPreRender_hook, (uintptr_t*)& CWorld_ProcessPedsAfterPreRender);

	//remove building
	SetUpHook(g_libGTASA + 0x00395994, (uintptr_t)CFileLoader__LoadObjectInstance_hook, (uintptr_t*)& CFileLoader__LoadObjectInstance);

	// retexture
	SetUpHook(g_libGTASA + 0x00391E20, (uintptr_t)CObject__Render_hook, (uintptr_t*)& CObject__Render);

	// gettexture fix crash
	SetUpHook(g_libGTASA + 0x00258910, (uintptr_t)GetTexture_hook, (uintptr_t*)& GetTexture_orig);

	// steal objects fix
	SetUpHook(g_libGTASA + 0x003AC114, (uintptr_t)CPlayerInfo__FindObjectToSteal_hook, (uintptr_t*)& CPlayerInfo__FindObjectToSteal);

	// GetFrameFromID fix
	SetUpHook(g_libGTASA + 0x00335CC0, (uintptr_t)CClumpModelInfo_GetFrameFromId_hook, (uintptr_t*)& CClumpModelInfo_GetFrameFromId);
	// RLEDecompress fix
	SetUpHook(g_libGTASA + 0x1BC314, (uintptr_t)RLEDecompress_hook, (uintptr_t*)&RLEDecompress);

	// todo: 3 pools fix crash

	// random crash fix
	SetUpHook(g_libGTASA + 0x001A8530, (uintptr_t)RenderQueue__ProcessCommand_hook, (uintptr_t*)& RenderQueue__ProcessCommand);
	// fix
	SetUpHook(g_libGTASA + 0x001B9D74, (uintptr_t)_rwFreeListFreeReal_hook, (uintptr_t*)& _rwFreeListFreeReal);

	SetUpHook(g_libGTASA + 0x00274AB4, (uintptr_t)CWidgetButton__Update_hook, (uintptr_t*)& CWidgetButton__Update);
	SetUpHook(g_libGTASA + 0x00274218, (uintptr_t)CWidget__IsTouched_hook, (uintptr_t*)& CWidget__IsTouched);

	SetUpHook(g_libGTASA + 0x004052B8, (uintptr_t)CVehicleModelInfo__SetupCommonData_hook, (uintptr_t*)& CVehicleModelInfo__SetupCommonData);

	SetUpHook(g_libGTASA + 0x0035BE30, (uintptr_t)CAEVehicleAudioEntity__GetVehicleAudioSettings_hook, (uintptr_t*)& CAEVehicleAudioEntity__GetVehicleAudioSettings);

	SetUpHook(g_libGTASA + 0x002C0304, (uintptr_t)CDarkel__RegisterCarBlownUpByPlayer_hook, (uintptr_t*)& CDarkel__RegisterCarBlownUpByPlayer);
	SetUpHook(g_libGTASA + 0x002C072C, (uintptr_t)CDarkel__ResetModelsKilledByPlayer_hook, (uintptr_t*)&CDarkel__ResetModelsKilledByPlayer);
	SetUpHook(g_libGTASA + 0x002C0758, (uintptr_t)CDarkel__QueryModelsKilledByPlayer_hook, (uintptr_t*)& CDarkel__QueryModelsKilledByPlayer);
	SetUpHook(g_libGTASA + 0x002C0778, (uintptr_t)CDarkel__FindTotalPedsKilledByPlayer_hook, (uintptr_t*)& CDarkel__FindTotalPedsKilledByPlayer);
	SetUpHook(g_libGTASA + 0x002C0D04, (uintptr_t)CDarkel__RegisterKillByPlayer_hook, (uintptr_t*)& CDarkel__RegisterKillByPlayer);

	SetUpHook(g_libGTASA + 0x00338CBC, (uintptr_t)CVehicleModelInfo__SetEditableMaterials_hook, (uintptr_t*)& CVehicleModelInfo__SetEditableMaterials);
	SetUpHook(g_libGTASA + 0x0050DEF4, (uintptr_t)CVehicle__ResetAfterRender_hook, (uintptr_t*)& CVehicle__ResetAfterRender);

	SetUpHook(g_libGTASA + 0x003B67F8, (uintptr_t)CRopes__Update_hook, (uintptr_t*)&CRopes__Update);

	SetUpHook(g_libGTASA + 0x3986CC, (uintptr_t)CGame__Process_hook, (uintptr_t*)& CGame__Process);

	SetUpHook(g_libGTASA + 0x004D4A6C, (uintptr_t)CAutomobile__ProcessEntityCollision_hook, (uintptr_t*)& CAutomobile__ProcessEntityCollision);
	
	SetUpHook(g_libGTASA + 0x00398334, (uintptr_t)CGame__Shutdown_hook, (uintptr_t*)& CGame__Shutdown);

	WriteMemory(g_libGTASA + 0x003DA86C,
		(uintptr_t)"\x80\xB4"\
		"\x00\xAF"\
		"\x1B\x4B"\
		"\x7B\x44"\
		"\x1B\x68", 10);

	NOP(g_libGTASA + 0x003DA876, 3);

	// headlights color, wheel size, wheel align
	SetUpHook(g_libGTASA + 0x005466EC, (uintptr_t)CShadows__StoreCarLightShadow_hook, (uintptr_t*)& CShadows__StoreCarLightShadow);
	SetUpHook(g_libGTASA + 0x00518EC4, (uintptr_t)CVehicle__DoHeadLightBeam_hook, (uintptr_t*)& CVehicle__DoHeadLightBeam);


	SetUpHook(g_libGTASA + 0x004E671C, (uintptr_t)CAutomobile__PreRender_hook, (uintptr_t*)& CAutomobile__PreRender);
	SetUpHook(g_libGTASA + 0x004DC6E8, (uintptr_t)CAutomobile__UpdateWheelMatrix_hook, (uintptr_t*)& CAutomobile__UpdateWheelMatrix);
	SetUpHook(g_libGTASA + 0x003E8D48, (uintptr_t)CMatrix__Rotate_hook, (uintptr_t*)& CMatrix__Rotate);
	SetUpHook(g_libGTASA + 0x003E8884, (uintptr_t)CMatrix__SetScale_hook, (uintptr_t*)& CMatrix__SetScale);

	SetUpHook(g_libGTASA + 0x0046CEF4, (uintptr_t)CTaskSimpleUseGun__RemoveStanceAnims_hook, (uintptr_t*)& CTaskSimpleUseGun__RemoveStanceAnims);

	SetUpHook(g_libGTASA + 0x003DA86C, (uintptr_t)CRadar__LimitRadarPoint_hook, (uintptr_t*)& CRadar__LimitRadarPoint); // TO FIX
	SetUpHook(g_libGTASA + 0x005529FC, (uintptr_t)CSprite2d__DrawBarChart_hook, (uintptr_t*)& CSprite2d__DrawBarChart);
	SetUpHook(g_libGTASA + 0x005353B4, (uintptr_t)CFont__PrintString_hook, (uintptr_t*)& CFont__PrintString);
	SetUpHook(g_libGTASA + 0x0055265C, (uintptr_t)CSprite2d__Draw_hook, (uintptr_t*)& CSprite2d__Draw);

	SetUpHook(g_libGTASA + 0x0027D8A8, (uintptr_t)CWidgetPlayerInfo__DrawWanted_hook, (uintptr_t*)& CWidgetPlayerInfo__DrawWanted);

    SetUpHook(g_libGTASA + 0x1FEAB4, (uintptr_t)mpg123_param_hook, (uintptr_t*)& mpg123_param);

	SetUpHook(g_libGTASA + 0x0027CE88, (uintptr_t)CWidgetPlayerInfo__DrawWeaponIcon_hook, (uintptr_t*)& CWidgetPlayerInfo__DrawWeaponIcon);
	SetUpHook(g_libGTASA + 0x00389D74, (uintptr_t)CCam__Process_hook, (uintptr_t*)& CCam__Process);
	// sub_45C88(dword_14A458 + 0x541AC4, sub_2CBF0, &unk_28E378); nop
	//sub_45C88(g_libGTASA + 0x001A8530, sub_2CBDC, &dword_28E374);


	SetUpHook(g_libGTASA + 0x003D6E6C, (uintptr_t)CHud__Draw_hook, (uintptr_t*)& CHud__Draw);
	SetUpHook(g_libGTASA + 0x0039DC68, (uintptr_t)CPad__CycleCameraModeDownJustDown_hook, (uintptr_t*)& CPad__CycleCameraModeDownJustDown);

	SetUpHook(g_libGTASA + 0x00391FE0, (uintptr_t)CEntity__GetIsOnScreen_hook, (uintptr_t*)& CEntity__GetIsOnScreen);
	SetUpHook(g_libGTASA + 0x0031B164, (uintptr_t)FxEmitterBP_c__Render_hook, (uintptr_t*)& FxEmitterBP_c__Render);
	SetUpHook(g_libGTASA + 0x0043A17C, (uintptr_t)CPed__ProcessEntityCollision_hook, (uintptr_t*)&CPed__ProcessEntityCollision);
	//SetUpHook(g_libGTASA + 0x3B1778, &CRenderer__RenderEverythingBarRoads_hook, &CRenderer__RenderEverythingBarRoads);
	SetUpHook(g_libGTASA + 0x3B1778, (uintptr_t)CRenderer__RenderEverythingBarRoads_hook, (uintptr_t*)&CRenderer__RenderEverythingBarRoads);
	HookCPad();

    SetUpHook(g_libGTASA + 0x54DF60, (uintptr_t)CMirrors__RenderReflections_hook, (uintptr_t*)&CMirrors__RenderReflections);
}
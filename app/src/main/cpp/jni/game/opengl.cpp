#pragma once

#include <android/log.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include <strstream>
#include <android/asset_manager.h>
#include "main.h"
#include "util.h"
#include "../util/armhook.h"
#include "GLES2/gl2.h"
#include "game/RW/RenderWare.h"
#include "game.h"
#include "../net/netgame.h"

#define FLAG_ALPHA_TEST           0x01
#define FLAG_LIGHTING             0x02
#define FLAG_ALPHA_MODULATE       0x04
#define FLAG_COLOR_EMISSIVE       0x08
#define FLAG_COLOR                0x10
#define FLAG_TEX0                 0x20
#define FLAG_ENVMAP               0x40          // normal envmap
#define FLAG_BONE3                0x80
#define FLAG_BONE4                0x100
#define FLAG_CAMERA_BASED_NORMALS 0x200
#define FLAG_FOG                  0x400
#define FLAG_TEXBIAS              0x800
#define FLAG_BACKLIGHT            0x1000
#define FLAG_LIGHT1               0x2000
#define FLAG_LIGHT2               0x4000
#define FLAG_LIGHT3               0x8000
#define FLAG_DETAILMAP            0x10000
#define FLAG_COMPRESSED_TEXCOORD  0x20000
#define FLAG_PROJECT_TEXCOORD     0x40000
#define FLAG_WATER                0x80000
#define FLAG_COLOR2               0x100000
#define FLAG_SPHERE_XFORM         0x800000      // thiz renders the scene as a sphere map for vehicle reflections
#define FLAG_SPHERE_ENVMAP        0x1000000     // spherical real-time envmap
#define FLAG_TEXMATRIX            0x2000000
#define FLAG_GAMMA                0x4000000
//custom flags
#define FLAG_CUSTOM_SKYBOX         0x8000000
#define FLAG_CUSTOM_VEH            0x10000000
int dword_207F54;
extern CNetGame *pNetGame;
typedef struct {
    // Checks for GL_OES_depth24
    char has24BitDepthCap;                   // 0x00
    // Checks for GL_OES_packed_depth_stencil
    char hasPackedDepthStencilCap;           // 0x01
    // Checks for GL_NV_depth_nonlinear
    char hasDepthNonLinearCap;               // 0x02
    // Checks for GL_EXT_texture_compression_dxt1 or GL_EXT_texture_compression_s3tc
    char hasTextureCompressionDXT1OrS3TCCap; // 0x03
    // Checks for GL_AMD_compressed_ATC_texture
    char hasTextureCompressionATCCap;        // 0x04
    // Checks for GL_IMG_texture_compression_pvrtc
    char hasTextureCompressionPVRTCCap;      // 0x05
    // Checks for GL_OES_rgb8_rgba8
    char has32BitRenderTargetCap;            // 0x06
    // Checks for GL_EXT_texture_filter_anisotropic
    char hasAnisotropicFilteringCap;         // 0x07
    // Set when OS_SystemChip() <= 1
    char unk_08;                             // 0x08
    // Always set to 0
    char unk_09;                             // 0x09
    // Checks for GL_QCOM_binning_control
    char hasBinningControlCap;               // 0x0A
    // Checks for GL_QCOM_alpha_test
    char hasAlphaTestCap;                    // 0x0B
    // Checks for Adreno (TM) 320 or GL_AMD_compressed_ATC_texture
    char isAdreno;                           // 0x0C
    // Set when there is no compression support
    char isMaliChip;                         // 0x0D
    // Checks for 225 or 540
    char isSlowGPU;                          // 0x0E
    char unk_0f;                             // 0x0F
} RQCapabilities;

RQCapabilities *RQCaps;

char* byte_617290 = 0; //pxl
char* byte_619294 = 0; //vertex
char* byte_74AFA8 = 0;
uintptr_t* EmuShader;
uintptr_t* g_rqShader;
uintptr_t * g_HZClass;
uintptr_t* hzClass;
uintptr_t g_ES2Shader;

void (*BuildPixelSource)(unsigned int a1);
void BuildPixelSource_hook(unsigned int a1){
    int v2; // r8
  bool v3; // zf
  size_t v4; // r0
  const char *v5; // r2
  char s[4096]; // [sp+10h] [bp-220h] BYREF
  int v8; // [sp+210h] [bp-20h]
  
  
  
  strcpy(byte_617290, "\0");
  
  
  
  snprintf(s, 0x200u, "#version 100\n");
  strcat(byte_617290, s);
  
  snprintf(s, 0x200u, "precision mediump float;");
  strcat(byte_617290, s);
  
  if ( (a1 & 0x20) != 0 )
  {
    snprintf(s, 0x200u, "uniform sampler2D Diffuse;");
    strcat(byte_617290, s);
    
    snprintf(s, 0x200u, "varying mediump vec2 Out_Tex0;");
    strcat(byte_617290, s);
    
  }
  if ( (a1 & 0x1000040) != 0 )
  {
    snprintf(s, 0x200u, "uniform sampler2D EnvMap;");
    strcat(byte_617290, s);
    
    snprintf(s, 0x200u, "uniform lowp float EnvMapCoefficient;");
    strcat(byte_617290, s);
    
    if ( (a1 & 0x40) != 0 )
      snprintf(s, 0x200u, "varying mediump vec2 Out_Tex1;");
    else
      snprintf(s, 0x200u, "varying mediump vec3 Out_Refl;");
  }
  else
  {
    if ( (a1 & 0x10000) == 0 )
      goto LABEL_10;
    snprintf(s, 0x200u, "uniform sampler2D EnvMap;");
    strcat(byte_617290, s);
    
    snprintf(s, 0x200u, "uniform float DetailTiling;");
  }
  strcat(byte_617290, s);
  
LABEL_10:
  if ( (a1 & 0x400) != 0 )
  {
    snprintf(s, 0x200u, "varying mediump float Out_FogAmt;");
    strcat(byte_617290, s);
    
    snprintf(s, 0x200u, "uniform lowp vec3 FogColor;");
    strcat(byte_617290, s);
    
    snprintf(s, 0x200u, "uniform sampler2D FogMap;");
    strcat(byte_617290, s);
    
    snprintf(s, 0x200u, "varying lowp vec4 Out_ClipSpace;");
    strcat(byte_617290, s);
    
  }
  if ( (a1 & 0x12) != 0 )
  {
    snprintf(s, 0x200u, "varying lowp vec4 Out_Color;");
    strcat(byte_617290, s);
    
  }
  if ( (a1 & 4) != 0 )
  {
    snprintf(s, 0x200u, "uniform lowp float AlphaModulate;");
    strcat(byte_617290, s);
    
  }
  v2 = a1 & 0x80000;
  if ( (a1 & 0x80000) != 0 )
  {
    snprintf(s, 0x200u, "varying mediump vec2 Out_WaterDetail;");
    strcat(byte_617290, s);
    
    snprintf(s, 0x200u, "varying mediump vec2 Out_WaterDetail2;");
    strcat(byte_617290, s);
    
    snprintf(s, 0x200u, "varying mediump float Out_WaterAlphaBlend;");
    strcat(byte_617290, s);
    
  }
  if ( (a1 & 0x2000) != 0 )
  {
    snprintf(s, 0x200u, "varying lowp vec3 Out_Spec;");
    strcat(byte_617290, s);
    
  }
  if ( (a1 & 0x20) != 0 )
  {
	  snprintf(s, 0x200u, "bool isSky(vec4 targetColor){vec2 textureCoord = vec2(0.0, 1.0);vec4 colorAtPosition = texture2D(Diffuse, textureCoord);const float colorThreshold = 0.02;return abs(colorAtPosition.r-targetColor.r)<=colorThreshold&&abs(colorAtPosition.g-targetColor.g)<=colorThreshold&&abs(colorAtPosition.b-targetColor.b)<=colorThreshold&&colorAtPosition.a==1.0;}");

	  strcat(byte_617290, s);
	  
  }//skybox
  snprintf(s, 0x200u, "void main() {");
  strcat(byte_617290, s);
  
  snprintf(s, 0x200u, "lowp vec4 fcolor;");
  strcat(byte_617290, s);
  
  if ( (a1 & 0x400) != 0 )
  {
    snprintf(s, 0x200u, "lowp vec2 ndc = (Out_ClipSpace.xy/Out_ClipSpace.w) / 2.0 + 0.5;");
    strcat(byte_617290, s);
    
    snprintf(s, 0x200u, "lowp vec2 fogTexCoords = vec2(ndc.x, ndc.y);");
    strcat(byte_617290, s);
    
    snprintf(s, 0x200u, "lowp vec4 fogColorNew = texture2D(FogMap, fogTexCoords);");
    strcat(byte_617290, s);
    
  }
  if ( (a1 & 0x20) == 0 )
  {
    if ( (a1 & 0x12) != 0 )
      snprintf(s, 0x200u, "fcolor = Out_Color;");
    else
      snprintf(s, 0x200u, "fcolor = 0.0;");
    goto LABEL_42;
  }
  if ( (a1 & 0x800) != 0 )
  {
    snprintf(s, 0x200u, "lowp vec4 diffuseColor = texture2D(Diffuse, Out_Tex0, -2.5);");strcat(byte_617290, s);
	//snprintf(s, 0x200u, "diffuseColor.rg += vec2(0.5, 0.5);");strcat(byte_617290, s);
	//snprintf(s, 0x200u, "diffuseColor = clamp(diffuseColor, 0.0, 1.0);");strcat(byte_617290, s);
  }
  else if ((uintptr_t)(g_libGTASA + 0x617250)){
	  if( *(char *)(g_libGTASA + 0x617250 + 14) ) 
	  {
		snprintf(s, 0x200u, "lowp vec4 diffuseColor = texture2D(Diffuse, Out_Tex0);");strcat(byte_617290, s);
	//	snprintf(s, 0x200u, "diffuseColor.g += 0.5;");strcat(byte_617290, s);
	//	snprintf(s, 0x200u, "diffuseColor = clamp(diffuseColor, 0.0, 1.0);");strcat(byte_617290, s);
	  }
	  else
	  {
		snprintf(s, 0x200u, "lowp vec4 diffuseColor = texture2D(Diffuse, Out_Tex0, -0.5);");strcat(byte_617290, s);
	//	snprintf(s, 0x200u, "diffuseColor.b += 0.5;");strcat(byte_617290, s);
	//	snprintf(s, 0x200u, "diffuseColor = clamp(diffuseColor, 0.0, 1.0);");strcat(byte_617290, s);
	  }
  }
  else
  {
    snprintf(s, 0x200u, "lowp vec4 diffuseColor = texture2D(Diffuse, Out_Tex0, -0.5);");strcat(byte_617290, s);
	//snprintf(s, 0x200u, "diffuseColor.r += 0.5;");strcat(byte_617290, s);
	//snprintf(s, 0x200u, "diffuseColor = clamp(diffuseColor, 0.0, 1.0);");strcat(byte_617290, s);
  }
 


 // strcat(byte_617290, s);
  
  //snprintf(s, 0x200u, "diffuseColor *= 10.0;");
 // strcat(byte_617290, s);
 
	snprintf(s, 0x200u, "lowp vec4 newDiffuseColor = diffuseColor;");
  strcat(byte_617290, s);
  
  snprintf(s, 0x200u, "fcolor = newDiffuseColor;");
  strcat(byte_617290, s);
  
  if ( (a1 & 0x12) == 0 )
  {
    if ( !v2 )
      goto LABEL_43;
    snprintf(s, 0x200u, "fcolor.a += Out_WaterAlphaBlend;");
    goto LABEL_42;
  }
  if ( (a1 & 0x10000) == 0 )
  {
    snprintf(s, 0x200u, "fcolor *= Out_Color;");
    strcat(byte_617290, s);
    
    if ( v2 )
    {
      snprintf(s, 0x200u, "fcolor.a += Out_WaterAlphaBlend;");
      strcat(byte_617290, s);
      
      if ( (a1 & 0x40) == 0 )
        goto LABEL_45;
LABEL_44:
      snprintf(s, 0x200u, "fcolor.xyz = mix(fcolor.xyz, texture2D(EnvMap, Out_Tex1).xyz, EnvMapCoefficient);");
      strcat(byte_617290, s);
      
      goto LABEL_45;
    }
LABEL_43:
    if ( (a1 & 0x40) == 0 )
      goto LABEL_45;
    goto LABEL_44;
  }
  if ( !v2 )
  {
    snprintf(
      s,
      0x200u,
      "fcolor *= vec4(Out_Color.xyz * texture2D(EnvMap, Out_Tex0.xy * DetailTiling, -0.5).xyz * 2.0, Out_Color.w);");
LABEL_42:
    strcat(byte_617290, s);
    
    goto LABEL_43;
  }
  snprintf(
    s,
    0x200u,
    "float waterDetail = texture2D(EnvMap, Out_WaterDetail, -1.0).x + texture2D(EnvMap, Out_WaterDetail2, -1.0).x;");
  strcat(byte_617290, s);
  
  snprintf(s, 0x200u, "fcolor *= vec4(Out_Color.xyz * waterDetail * 1.1, Out_Color.w);");
  strcat(byte_617290, s);
  
  snprintf(s, 0x200u, "fcolor.a += Out_WaterAlphaBlend;");
  strcat(byte_617290, s);
  
  if ( (a1 & 0x40) != 0 )
    goto LABEL_44;
LABEL_45:
  if ( (a1 & 0x1000000) != 0 )
  {
    snprintf(s, 0x200u, "vec2 ReflPos = normalize(Out_Refl.xy) * (Out_Refl.z * 0.5 + 0.5);");
    strcat(byte_617290, s);
    
    snprintf(s, 0x200u, "ReflPos = (ReflPos * vec2(0.5,0.5)) + vec2(0.5,0.5);");
    strcat(byte_617290, s);
    
    snprintf(s, 0x200u, "lowp vec4 ReflTexture =  texture2D(EnvMap, ReflPos);");
    strcat(byte_617290, s);
    
    snprintf(s, 0x200u, "float newEnvMapCoef = EnvMapCoefficient + 0.1;");
    strcat(byte_617290, s);
    
    snprintf(s, 0x200u, "fcolor.xyz = mix(fcolor.xyz, ReflTexture.xyz, newEnvMapCoef);");
    strcat(byte_617290, s);
    
    snprintf(s, 0x200u, "fcolor.w += ReflTexture.b * 0.125;");
    strcat(byte_617290, s);
    
  }
  if((uintptr_t)(g_libGTASA + 0x617250)){
  if ( !*(uintptr_t *)(g_libGTASA + 0x617250 + 8) )
  {
    v3 = (a1 & 0x2000) == 0;
    if ( (a1 & 0x2000) != 0 )
      v3 = (a1 & 0x1000040) == 0;
    if ( !v3 )
    {
      snprintf(s, 0x200u, "fcolor.xyz += Out_Spec;");
      strcat(byte_617290, s);
      
    }
    if ( (a1 & 0x400) != 0 )
    {
      snprintf(s, 0x200u, "fcolor.xyz = mix(fcolor.xyz, FogColor.xyz, Out_FogAmt);");
      strcat(byte_617290, s);
      
    }
  }}
  if ( (a1 & 0x4000000) != 0 )
  {
    snprintf(s, 0x200u, "fcolor.xyz += fcolor.xyz * 0.5;");
    strcat(byte_617290, s);
    
  }
  snprintf(s, 0x200u, "gl_FragColor = fcolor;");
  strcat(byte_617290, s);
  
  if ( a1 << 0x1F )
  {
    snprintf(s, 0x200u, "/*ATBEGIN*/");
    strcat(byte_617290, s);
    
    if ( *(uintptr_t *)(0x5DDD30 + g_libGTASA) == 9 && (a1 & 0x20) != 0 )
    {
      if ( (a1 & 0x800) != 0 )
      {
        snprintf(s, 0x200u, "if (diffuseColor.a < 0.8) { discard; }");
        goto LABEL_70;
      }
      if ( (a1 & 0x200) == 0 )
      {
        snprintf(s, 0x200u, "if (diffuseColor.a < 0.2) { discard; }");
LABEL_70:
        strcat(byte_617290, s);
        
        snprintf(s, 0x200u, "/*ATEND*/");
        strcat(byte_617290, s);
        
        goto LABEL_71;
      }
      snprintf(s, 0x200u, "gl_FragColor.a = Out_Color.a;");
      strcat(byte_617290, s);
      v4 = strlen(byte_617290);
      v5 = "if (diffuseColor.a < 0.5) { discard; }";
    }
    else
    {
      if ( (a1 & 0x800) != 0 )
      {
        snprintf(s, 0x200u, "if (gl_FragColor.a < 0.8) { discard; }");
        goto LABEL_70;
      }
      if ( (a1 & 0x200) == 0 )
      {
        snprintf(s, 0x200u, "if (gl_FragColor.a < 0.2) { discard; }");
        goto LABEL_70;
      }
      snprintf(s, 0x200u, "if (gl_FragColor.a < 0.5) { discard; }");
      strcat(byte_617290, s);
      v4 = strlen(byte_617290);
      v5 = "gl_FragColor.a = Out_Color.a;";
    }
    
    snprintf(s, 0x200u, "%s", v5);
    goto LABEL_70;
  }
LABEL_71:
  if ( (a1 & 4) != 0 )
  {
    snprintf(s, 0x200u, "gl_FragColor.a *= AlphaModulate;");
    strcat(byte_617290, s);
    
  }

     if ( (a1 & 0x20) != 0 )
  {
	  
	  snprintf(s, 0x200u, "if(isSky(vec4(95.0/255.0,144.0/255.0,188.0/255.0,1.0))||isSky(vec4(125.0/255.0,152.0/255.0,212.0/255.0,1.0))||isSky(vec4(164.0/255.0,110.0/255.0,91.0/255.0,1.0))||isSky(vec4(20.0/255.0,21.0/255.0,28.0/255.0,1.0))){gl_FragColor.rgb = diffuseColor.rgb;}");

strcat(byte_617290, s);
  }//skybox
  
snprintf(s, 0x200u, (const char *)"}");
//Log("build pixel shader 11");
  strcat(byte_617290, s);
 // Log("build pixel shader 2");
 // Log("build pixel shader 3");
 // Log("build pixel shader");
 // Log("pixel %s", byte_617290);
 // Log("end build pixel shader");
 //Log("test gtasa pix: %s", (char*)(g_libGTASA + 0x617290));
}
float getBrightnessCoefficient(int hour) {
    if (hour < 6 || hour >= 21) {
        return 4.5;
    } else if (hour >= 6 && hour < 8) {
        float t = (hour - 6.0) / 2.0;
        return 4.5 + (8.0 - 4.5) * t;
    } else if (hour >= 8 && hour < 17) {
        return 10.0;
    } else {
        float t = (hour - 18.0) / 2.0; 
        return 8.0 + (4.5 - 8.0) * t;
    }
}
void (*BuildVertexSource)(unsigned int a1);
void BuildVertexSource_hook(unsigned int a1) {
	int v2; // r8
  bool v3; // r9
  int v4; // r0
  int v5; // r11
  size_t v6; // r0
  const char *v7; // r2
  const char *v8; // r3
  const char *v9; // r8
  int v10; // r0
  int v12; // [sp+18h] [bp-238h]
  char s[4096]; // [sp+30h] [bp-220h] BYREF
  int v14; // [sp+230h] [bp-20h]

  snprintf(s, 0x200u, "#version 100\n");
  strcat(byte_619294, s);
  
  snprintf(s, 0x200u, "precision highp float;");
  strcat(byte_619294, s);
  
  snprintf(s, 0x200u, "uniform mat4 ProjMatrix;");
  strcat(byte_619294, s);
  
  snprintf(s, 0x200u, "uniform mat4 ViewMatrix;");
  strcat(byte_619294, s);
  
  snprintf(s, 0x200u, "uniform mat4 ObjMatrix;");
  strcat(byte_619294, s);
  
  if ( (a1 & 2) != 0 )
  {
    snprintf(s, 0x200u, "uniform lowp vec3 AmbientLightColor;");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "uniform lowp vec4 MaterialEmissive;");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "uniform lowp vec4 MaterialAmbient;");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "uniform lowp vec4 MaterialDiffuse;");
    strcat(byte_619294, s);
    
    if ( (a1 & 0x2000) != 0 )
    {
      snprintf(s, 0x200u, "uniform lowp vec3 DirLightDiffuseColor;");
      strcat(byte_619294, s);
      
      snprintf(s, 0x200u, "uniform vec3 DirLightDirection;");
      strcat(byte_619294, s);
      
      if ( (a1 & 0x1180) != 0 && *(uintptr_t *)(g_libGTASA + 0x8ED870) >= 1 )
      {
        snprintf(s, 0x200u, "uniform vec3 DirBackLightDirection;");
        strcat(byte_619294, s);
        
      }
    }
    if ( (a1 & 0x4000) != 0 )
    {
      snprintf(s, 0x200u, "uniform lowp vec3 DirLight2DiffuseColor;");
      strcat(byte_619294, s);
      
      snprintf(s, 0x200u, "uniform vec3 DirLight2Direction;");
      strcat(byte_619294, s);
      
    }
    if ( (a1 & 0x8000) != 0 )
    {
      snprintf(s, 0x200u, "uniform lowp vec3 DirLight3DiffuseColor;");
      strcat(byte_619294, s);
      
      snprintf(s, 0x200u, "uniform vec3 DirLight3Direction;");
      strcat(byte_619294, s);
      
    }
  }
  snprintf(s, 0x200u, "attribute vec3 Position;");
  strcat(byte_619294, s);
  
  snprintf(s, 0x200u, "attribute vec3 Normal;");
  strcat(byte_619294, s);
  
  if ( (a1 & 0x20) != 0 )
  {
    if ( (a1 & 0x40000) != 0 )
      snprintf(s, 0x200u, "attribute vec4 TexCoord0;");
    else
      snprintf(s, 0x200u, "attribute vec2 TexCoord0;");
    strcat(byte_619294, s);
    
  }
  snprintf(s, 0x200u, "attribute vec4 GlobalColor;");
  strcat(byte_619294, s);
  
  if ( (a1 & 0x180) != 0 )
  {
    snprintf(s, 0x200u, "attribute vec4 BoneWeight;");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "attribute vec4 BoneIndices;");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "uniform highp vec4 Bones[%d];", 3 * *(uintptr_t *)((char *)0x617248 + g_libGTASA));
    strcat(byte_619294, s);
    
  }
  if ( (a1 & 0x20) != 0 )
  {
    snprintf(s, 0x200u, "varying mediump vec2 Out_Tex0;");
    strcat(byte_619294, s);
    
  }
  if ( (a1 & 0x2000000) != 0 )
  {
    snprintf(s, 0x200u, "uniform mat3 NormalMatrix;");
    strcat(byte_619294, s);
    
  }
  v2 = a1 & 0x1000040;
  if ( (a1 & 0x1000040) != 0 )
  {
    snprintf(s, 0x200u, "uniform lowp float EnvMapCoefficient;");
    strcat(byte_619294, s);
    
    if ( (a1 & 0x40) != 0 )
      snprintf(s, 0x200u, "varying mediump vec2 Out_Tex1;");
    else
      snprintf(s, 0x200u, "varying mediump vec3 Out_Refl;");
    strcat(byte_619294, s);
    
  }
  if ( (a1 & 0x9880640) != 0 )
  {
    snprintf(s, 0x200u, "uniform vec3 CameraPosition;");
    strcat(byte_619294, s);
    
  }
  if ( (a1 & 0x400) != 0 )
  {
    snprintf(s, 0x200u, "varying mediump float Out_FogAmt;");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "uniform vec3 FogDistances;");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "varying mediump vec4 Out_ClipSpace;");
    strcat(byte_619294, s);
    
  }
  if ( (a1 & 0x80000) != 0 )
  {
    snprintf(s, 0x200u, "uniform vec3 WaterSpecs;");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "varying mediump vec2 Out_WaterDetail;");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "varying mediump vec2 Out_WaterDetail2;");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "varying mediump float Out_WaterAlphaBlend;");
    strcat(byte_619294, s);
    
  }
  if ( (a1 & 0x100000) != 0 )
  {
    snprintf(s, 0x200u, "attribute vec4 Color2;");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "uniform lowp float ColorInterp;");
    strcat(byte_619294, s);
    
  }
  if ( (a1 & 0x12) != 0 )
  {
    snprintf(s, 0x200u, "varying lowp vec4 Out_Color;");
    strcat(byte_619294, s);
    
  }
  v3 = v2 == 0;
  v4 = 0;
  v5 = a1 & 0x1000040;
  if ( (a1 & 0x2000) == 0 )
    v4 = 1;
  v12 = v4;
  if ( (a1 & 0x2000) != 0 && v2 )
  {
    snprintf(s, 0x200u, "varying lowp vec3 Out_Spec;");
    strcat(byte_619294, s);
    
  }
  snprintf(s, 0x200u, "void main() {");
  strcat(byte_619294, s);
  
  if ( (a1 & 0x180) != 0 )
  {
    snprintf(s, 0x200u, "\tivec4 BlendIndexArray = ivec4(BoneIndices);");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "\tmat4 BoneToLocal;");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "\tBoneToLocal[0] = Bones[BlendIndexArray.x*3] * BoneWeight.x;");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "\tBoneToLocal[1] = Bones[BlendIndexArray.x*3+1] * BoneWeight.x;");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "\tBoneToLocal[2] = Bones[BlendIndexArray.x*3+2] * BoneWeight.x;");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "\tBoneToLocal[3] = vec4(0.0,0.0,0.0,1.0);");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "\tBoneToLocal[0] += Bones[BlendIndexArray.y*3] * BoneWeight.y;");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "\tBoneToLocal[1] += Bones[BlendIndexArray.y*3+1] * BoneWeight.y;");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "\tBoneToLocal[2] += Bones[BlendIndexArray.y*3+2] * BoneWeight.y;");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "\tBoneToLocal[0] += Bones[BlendIndexArray.z*3] * BoneWeight.z;");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "\tBoneToLocal[1] += Bones[BlendIndexArray.z*3+1] * BoneWeight.z;");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "\tBoneToLocal[2] += Bones[BlendIndexArray.z*3+2] * BoneWeight.z;");
    strcat(byte_619294, s);
    
    if ( (a1 & 0x100) != 0 )
    {
      snprintf(s, 0x200u, "\tBoneToLocal[0] += Bones[BlendIndexArray.w*3] * BoneWeight.w;");
      strcat(byte_619294, s);
      
      snprintf(s, 0x200u, "\tBoneToLocal[1] += Bones[BlendIndexArray.w*3+1] * BoneWeight.w;");
      strcat(byte_619294, s);
      
      snprintf(s, 0x200u, "\tBoneToLocal[2] += Bones[BlendIndexArray.w*3+2] * BoneWeight.w;");
      strcat(byte_619294, s);
      
    }
    snprintf(s, 0x200u, "\tvec4 WorldPos = ObjMatrix * (vec4(Position,1.0) * BoneToLocal);");
  }
  else
  {
    snprintf(s, 0x200u, "\tvec4 WorldPos = ObjMatrix * vec4(Position,1.0);");
  }
  strcat(byte_619294, s);
  
  if ( (a1 & 0x8000000) != 0 )
	  
  
  {
	  Log("skybox yes");
    snprintf(s, 0x200u, "    vec3 ReflVector = WorldPos.xyz - CameraPosition.xyz;");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "    float coefOffset = 1.0 - clamp(length(ReflVector.xy) / 45.0, 0.0, 1.0);");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "    coefOffset = -1000.0 * coefOffset;");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "\tvec3 ReflPos = normalize(ReflVector);");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "    ReflPos.xy = normalize(ReflPos.xy) * (ReflPos.z * 0.5 + 0.5);");
    strcat(byte_619294, s);
    v6 = strlen(byte_619294);
    v7 = "\tgl_Position = vec4(ReflPos.xy, coefOffset + length(ReflVector) * 0.002, 1.0);";
LABEL_52:
    snprintf(s, 0x200u, "%s", v7);
    goto LABEL_53;
  }
  if ( (a1 & 0x800000) != 0 )
  {
    snprintf(s, 0x200u, "    vec3 ReflVector = WorldPos.xyz - CameraPosition.xyz;");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "\tvec3 ReflPos = normalize(ReflVector);");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "    ReflPos.xy = normalize(ReflPos.xy) * (ReflPos.z * 0.5 + 0.5);");
    strcat(byte_619294, s);
    v6 = strlen(byte_619294);
    v7 = "\tgl_Position = vec4(ReflPos.xy, length(ReflVector) * 0.002, 1.0);";
    goto LABEL_52;
  }
  snprintf(s, 0x200u, "\tvec4 ViewPos = ViewMatrix * WorldPos;");
  strcat(byte_619294, s);
  
  snprintf(s, 0x200u, "\tgl_Position = ProjMatrix * ViewPos;");
  strcat(byte_619294, s);
  
  if ( (a1 & 0x400) == 0 )
    goto LABEL_54;
  snprintf(s, 0x200u, "\tOut_ClipSpace = gl_Position;");
LABEL_53:
  strcat(byte_619294, s);
  
LABEL_54:
  if ( (a1 & 2) != 0 )
  {
    if ( (a1 & 0x201) == 0x201 && (a1 & 0xE000) != 0 )
    {
      snprintf(s, 0x200u, "vec3 WorldNormal = normalize(vec3(WorldPos.xy - CameraPosition.xy, 0.0001)) * 0.85;");
    }
    else if ( (a1 & 0x180) != 0 )
    {
      snprintf(s, 0x200u, "vec3 WorldNormal = mat3(ObjMatrix) * (Normal * mat3(BoneToLocal));");
    }
    else
    {
      snprintf(s, 0x200u, "vec3 WorldNormal = (ObjMatrix * vec4(Normal,0.0)).xyz;");
    }
    goto LABEL_63;
  }
  if ( v5 )
  {
    snprintf(s, 0x200u, "vec3 WorldNormal = vec3(0.0, 0.0, 0.0);");
LABEL_63:
    strcat(byte_619294, s);
    
  }
  if((uintptr_t)(g_libGTASA + 0x617250)){
  if ( !*(uintptr_t *)(g_libGTASA + 0x617250 + 8) && (a1 & 0x400) != 0 )
  {
    snprintf(
      s,
      0x200u,
      "Out_FogAmt = clamp((length(WorldPos.xyz - CameraPosition.xyz) - FogDistances.x) * FogDistances.z, 0.0, 1.0);");
    strcat(byte_619294, s);
    
  }}
  if ( (a1 & 0x20) != 0 )
  {
    if ( (a1 & 0x40000) != 0 )
    {
      v8 = "TexCoord0.xy / TexCoord0.w";
    }
    else
    {
      v8 = "TexCoord0";
      if ( (a1 & 0x20000) != 0 )
        v8 = "TexCoord0 / 512.0";
    }
    if ( (a1 & 0x2000000) != 0 )
      snprintf(s, 0x200u, "Out_Tex0 = (NormalMatrix * vec3(%s, 1.0)).xy;", v8);
    else
      snprintf(s, 0x200u, "Out_Tex0 = %s;", v8);
    strcat(byte_619294, s);
    
  }
  if ( v5 )
  {
    snprintf(s, 0x200u, "vec3 reflVector = normalize(WorldPos.xyz - CameraPosition.xyz);");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "reflVector = reflVector - 2.0 * dot(reflVector, WorldNormal) * WorldNormal;");
    strcat(byte_619294, s);
    
    if ( (a1 & 0x1000000) != 0 )
      snprintf(s, 0x200u, "Out_Refl = reflVector;");
    else
      snprintf(s, 0x200u, "Out_Tex1 = vec2(length(reflVector.xy), (reflVector.z * 0.5) + 0.25);");
    strcat(byte_619294, s);
    
  }
  if ( (a1 & 0x100000) != 0 )
  {
    snprintf(s, 0x200u, "lowp vec4 InterpColor = mix(GlobalColor, Color2, ColorInterp);");
    strcat(byte_619294, s);
    
    v9 = "InterpColor";
  }
  else
  {
    v9 = "GlobalColor";
  }
  if ( (a1 & 2) != 0 )
  {
    snprintf(s, 0x200u, "vec3 Out_LightingColor;");
    strcat(byte_619294, s);
	
	//snprintf(s, 0x200u, "MaterialAmbient.xyz *= 100000.0;");
   // strcat(byte_619294, s);
    
    if ( (a1 & 8) != 0 )
    {
      if ( (a1 & 0x200) != 0 )
        snprintf(s, 0x200u, "Out_LightingColor = AmbientLightColor * MaterialAmbient.xyz * 1.5;");
      else
        snprintf(s, 0x200u, "Out_LightingColor = AmbientLightColor * MaterialAmbient.xyz * 1.5 + %s.xyz;", v9);
    }
    else
    {
      snprintf(s, 0x200u, "Out_LightingColor = AmbientLightColor * MaterialAmbient.xyz * 1.5 + MaterialEmissive.xyz;");
    }
    strcat(byte_619294, s);
	
	//Log("Out_LightingColor *= %f;", getBrightnessCoefficient(pNetGame->m_byteWorldTime));
	//if(pNetGame){
		//if ( (a1 & 0x1000000) == 0 ){
			
			
			//snprintf(
   //         s,
   //         0x200u,
   //         "Out_LightingColor.rgb += vec3(0.19, 0.25, 0.0);"); //warning light
     //   strcat(byte_619294, s);
			/*if ( (a1 & 0x1000000) != 0 ) {
	snprintf(
            s,
            0x200u,
            "Out_LightingColor.rgb = vec3(0.0, 0.0, 0.0);"); //warning light
        strcat(byte_619294, s);
			}**/
	
    if ( (a1 & 0xE000) != 0 ) 
    {
      if ( (a1 & 0x2000) != 0 )
      {
        if ( (a1 & 0x1180) != 0 && *(uintptr_t *)(g_libGTASA + 0x8ED870) >= 2 )
          snprintf(
            s,
            0x200u,
            "Out_LightingColor += (max(dot(DirLightDirection, WorldNormal), 0.0) + max(dot(DirBackLightDirection, WorldNo"
            "rmal), 0.0)) * DirLightDiffuseColor;");
        else
          snprintf(
            s,
            0x200u,
            "Out_LightingColor += max(dot(DirLightDirection, WorldNormal), 0.0) * DirLightDiffuseColor;");
        strcat(byte_619294, s);
        
        if ( (a1 & 0x4000) == 0 )
        {
LABEL_96:
          if ( (a1 & 0x8000) == 0 )
            goto LABEL_98;
          goto LABEL_97;
        }
      }
      else if ( (a1 & 0x4000) == 0 )
      {
        goto LABEL_96;
      }
      snprintf(
        s,
        0x200u,
        "Out_LightingColor += max(dot(DirLight2Direction, WorldNormal), 0.0) * DirLight2DiffuseColor;");
      strcat(byte_619294, s);
      
      if ( (a1 & 0x8000) != 0 )
      {
LABEL_97:
        snprintf(
          s,
          0x200u,
          "Out_LightingColor += max(dot(DirLight3Direction, WorldNormal), 0.0) * DirLight3DiffuseColor;");
        strcat(byte_619294, s);
        
        goto LABEL_98;
      }
    }

LABEL_98:
    if ( (a1 & 0x12) != 0 )
    {
		
      if ( (a1 & 0x10) != 0 )
			snprintf(
			  s,
			  0x200u,
			  "Out_Color = vec4((Out_LightingColor.xyz + %s.xyz * 1.5) * MaterialDiffuse.xyz, (MaterialAmbient.w) * %s.w);",
			  v9,
			  v9);
		  else
			snprintf(s, 0x200u, "Out_Color = vec4(Out_LightingColor * MaterialDiffuse.xyz, MaterialAmbient.w * %s.w);", v9);
		  strcat(byte_619294, s);
		//  if ( (a1 & 0x1000000) == 0 ){
		//	snprintf(s, 0x200u, "Out_Color.rgb += vec3(0.1, 0.1, 0.1);");
	  
		//	strcat(byte_619294, s);
		 // }
		//	  snprintf(s, 0x200u, "Out_Color.rgb += vec3(0.02, 0.02, 0.02);");
	  
		//	strcat(byte_619294, s);
		//  }
		  snprintf(s, 0x200u, "Out_Color = clamp(Out_Color, 0.0, 1.0);");
		  strcat(byte_619294, s);
		  
    }
	
    goto LABEL_106;
  }
  if ( (a1 & 0x12) != 0 )
  {
    snprintf(s, 0x200u, "Out_Color = %s;", v9);
    strcat(byte_619294, s);
	
    
  }
LABEL_106:
  v10 = *((uint8_t *)6386264 + g_libGTASA/* + 0x10*/);
  if ( *((uint8_t *)6386264 + g_libGTASA /*+ 0x10*/) )
	  
    v10 = 1;
  if ( !(v10 | v12 | v3) )
  {
    snprintf(
      s,
      0x200u,
      "float specAmt = max(pow(dot(reflVector, DirLightDirection), %.1f), 0.0) * EnvMapCoefficient * 0.5;");// * 1.3
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "Out_Spec = specAmt * DirLightDiffuseColor;");
    strcat(byte_619294, s);
    
  }
  if ( (a1 & 0x80000) != 0 )
  {
    snprintf(s, 0x200u, "Out_WaterDetail = (Out_Tex0 * 4.0) + vec2(WaterSpecs.x * -0.3, WaterSpecs.x * 0.21);");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "Out_WaterDetail2 = (Out_Tex0 * -8.0) + vec2(WaterSpecs.x * 0.12, WaterSpecs.x * -0.05);");
    strcat(byte_619294, s);
    
    snprintf(s, 0x200u, "Out_WaterAlphaBlend = distance(WorldPos.xy, CameraPosition.xy) * WaterSpecs.y;");
    strcat(byte_619294, s);
    
  }
  snprintf(s, 0x200u, (const char *)"}");
  strcat(byte_619294, s);
  
 // Log("build vertex shader");
//  Log("vertex %s", byte_619294);
 // Log("end build vertex shader");
 // Log("test gtasa vert: %s", (char*)(g_libGTASA + 0x619294));
}

int (*RQShader__BuildSource)(unsigned int a1, char **a2, char **a3);
int RQShader__BuildSource_hook(unsigned int a1, char **a2, char **a3)
{
  int v5; // r5
  unsigned int v6; // r6

  v5 = g_libGTASA;
  byte_619294 = 0;
  byte_617290 = 0;
  v6 = a1 & 0xFE7FFFBF;
  ((void ( *)(unsigned int))(g_libGTASA + 0x1A40B5))(a1 & 0xFE7FFFBF);
  ((void ( *)(unsigned int))(g_libGTASA + 0x1A4AB5))(v6);
  return 1;
}
void sub_484C0()
{
  
}
int (*CheckCompile)(uintptr_t *thiz, unsigned int a2, unsigned int a3, const char *a4);
int CheckCompile_hook(uintptr_t *thiz, unsigned int a2, unsigned int a3, const char *a4){
	return 1;
	
}
void (*InitAfterCompile)(uintptr_t* thiz);
void InitAfterCompile_hook(uintptr_t* thiz){
	//UniformLocationWaveness = glGetUniformLocation((int)(this + 0x3E8), "Waveness");
	//Log("init after compile %d", (int)(thiz + 0xFA));
	InitAfterCompile(thiz);
}
void gooo();
#include "../util/patch.h"
void SetUpGLHooks()
{
    RQCaps = (RQCapabilities *)g_libGTASA + 0x617250;

    byte_619294 = (char*)(g_libGTASA + 0x619294);
    byte_619294[0] = 0;
    byte_617290 = (char*)(g_libGTASA + 0x617290);
	byte_617290[0] = 0;
	CHook::RET(0x3456C8 + g_libGTASA);
	NOP(g_libGTASA + 0x3A01A4, 2);
	dword_207F54 = (int)RQCaps;
//	CHook::Redirect(0x1A7AE4, g_libGTASA + 2, sub_484C0);
	//NOP(0x1A7AE4 + g_libGTASA + 0x16, 1);
	//NOP(g_libGTASA + 0x3B1AF0, 2);
	//1A348E  sub_1B6BC((char *)&loc_1A348E + dword_207F0C, sub_48028);
	//SetUpHook(g_libGTASA + 0x1A34B0, (uintptr_t)CheckCompile_hook, (uintptr_t*)&CheckCompile);
    SetUpHook(g_libGTASA + 0x1A40B4, (uintptr_t)BuildPixelSource_hook, (uintptr_t*)&BuildPixelSource);
	
    SetUpHook(g_libGTASA + 0x1A4AB4, (uintptr_t)BuildVertexSource_hook, (uintptr_t*)&BuildVertexSource);
	
	//CHook::SetUpHook(g_libGTASA + 0x1A348E + 0x82, (uintptr_t)InitAfterCompile_hook, (uintptr_t*)&InitAfterCompile);
	//SetUpHook(g_libGTASA + 0x1A5EB0, (uintptr_t)RQShader__BuildSource_hook, (uintptr_t*)&RQShader__BuildSource);
}
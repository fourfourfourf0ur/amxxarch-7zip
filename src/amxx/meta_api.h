#pragma once

#include "dllapi.h"
#include "h_export.h"
#include "engine_api.h"
#include "enginecallbacks.h"
#include "plinfo.h"
#include "mutil.h"

#define META_INTERFACE_VERSION "5:13"

enum META_RES {
  MRES_UNSET = 0,
  MRES_IGNORED,
  MRES_HANDLED,
  MRES_OVERRIDE,
  MRES_SUPERCEDE,
};

struct meta_globals_t {
  META_RES mres;
  META_RES prev_mres;
  META_RES status;
  void* orig_ret;
  void* override_ret;

#ifdef METAMOD_CORE
  uint32* esp_save;
#endif
};

extern meta_globals_t* gpMetaGlobals;
#define SET_META_RESULT(result) gpMetaGlobals->mres = result
#define RETURN_META(result)       \
  do {                            \
    gpMetaGlobals->mres = result; \
    return;                       \
  } while (0)
#define RETURN_META_VALUE(result, value) \
  do {                                   \
    gpMetaGlobals->mres = result;        \
    return value;                        \
  } while (0)
#define META_RESULT_STATUS gpMetaGlobals->status
#define META_RESULT_PREVIOUS gpMetaGlobals->prev_mres
#define META_RESULT_ORIG_RET(type) *(type*)gpMetaGlobals->orig_ret
#define META_RESULT_OVERRIDE_RET(type) *(type*)gpMetaGlobals->override_ret

struct META_FUNCTIONS {
  GETENTITYAPI_FN pfnGetEntityAPI;
  GETENTITYAPI_FN pfnGetEntityAPI_Post;
  GETENTITYAPI2_FN pfnGetEntityAPI2;
  GETENTITYAPI2_FN pfnGetEntityAPI2_Post;
  GETNEWDLLFUNCTIONS_FN pfnGetNewDLLFunctions;
  GETNEWDLLFUNCTIONS_FN pfnGetNewDLLFunctions_Post;
  GET_ENGINE_FUNCTIONS_FN pfnGetEngineFunctions;
  GET_ENGINE_FUNCTIONS_FN pfnGetEngineFunctions_Post;
};

struct gamedll_funcs_t {
  DLL_FUNCTIONS* dllapi_table;
  NEW_DLL_FUNCTIONS* newapi_table;
};

extern gamedll_funcs_t* gpGamedllFuncs;
extern mutil_funcs_t* gpMetaUtilFuncs;

C_DLLEXPORT void Meta_Init();
typedef void (*META_INIT_FN)();

C_DLLEXPORT int Meta_Query(const char* interfaceVersion, plugin_info_t** plinfo,
                           mutil_funcs_t* pMetaUtilFuncs);
typedef int (*META_QUERY_FN)(const char* interfaceVersion,
                             plugin_info_t** plinfo,
                             mutil_funcs_t* pMetaUtilFuncs);

C_DLLEXPORT int Meta_Attach(PLUG_LOADTIME now, META_FUNCTIONS* pFunctionTable,
                            meta_globals_t* pMGlobals,
                            gamedll_funcs_t* pGamedllFuncs);
typedef int (*META_ATTACH_FN)(PLUG_LOADTIME now, META_FUNCTIONS* pFunctionTable,
                              meta_globals_t* pMGlobals,
                              gamedll_funcs_t* pGamedllFuncs);

C_DLLEXPORT int Meta_Detach(PLUG_LOADTIME now, PL_UNLOAD_REASON reason);
typedef int (*META_DETACH_FN)(PLUG_LOADTIME now, PL_UNLOAD_REASON reason);

C_DLLEXPORT int GetEntityAPI_Post(DLL_FUNCTIONS* pFunctionTable,
                                  int interfaceVersion);
C_DLLEXPORT int GetEntityAPI2_Post(DLL_FUNCTIONS* pFunctionTable,
                                   int* interfaceVersion);
C_DLLEXPORT int GetNewDLLFunctions_Post(NEW_DLL_FUNCTIONS* pNewFunctionTable,
                                        int* interfaceVersion);
C_DLLEXPORT int GetEngineFunctions(enginefuncs_t* pengfuncsFromEngine,
                                   int* interfaceVersion);
C_DLLEXPORT int GetEngineFunctions_Post(enginefuncs_t* pengfuncsFromEngine,
                                        int* interfaceVersion);

#define MDLL_FUNC gpGamedllFuncs->dllapi_table
#define MDLL_GameDLLInit MDLL_FUNC->pfnGameInit
#define MDLL_Spawn MDLL_FUNC->pfnSpawn
#define MDLL_Think MDLL_FUNC->pfnThink
#define MDLL_Use MDLL_FUNC->pfnUse
#define MDLL_Touch MDLL_FUNC->pfnTouch
#define MDLL_Blocked MDLL_FUNC->pfnBlocked
#define MDLL_KeyValue MDLL_FUNC->pfnKeyValue
#define MDLL_Save MDLL_FUNC->pfnSave
#define MDLL_Restore MDLL_FUNC->pfnRestore
#define MDLL_ObjectCollsionBox MDLL_FUNC->pfnAbsBox
#define MDLL_SaveWriteFields MDLL_FUNC->pfnSaveWriteFields
#define MDLL_SaveReadFields MDLL_FUNC->pfnSaveReadFields
#define MDLL_SaveGlobalState MDLL_FUNC->pfnSaveGlobalState
#define MDLL_RestoreGlobalState MDLL_FUNC->pfnRestoreGlobalState
#define MDLL_ResetGlobalState MDLL_FUNC->pfnResetGlobalState
#define MDLL_ClientConnect MDLL_FUNC->pfnClientConnect
#define MDLL_ClientDisconnect MDLL_FUNC->pfnClientDisconnect
#define MDLL_ClientKill MDLL_FUNC->pfnClientKill
#define MDLL_ClientPutInServer MDLL_FUNC->pfnClientPutInServer
#define MDLL_ClientCommand MDLL_FUNC->pfnClientCommand
#define MDLL_ClientUserInfoChanged MDLL_FUNC->pfnClientUserInfoChanged
#define MDLL_ServerActivate MDLL_FUNC->pfnServerActivate
#define MDLL_ServerDeactivate MDLL_FUNC->pfnServerDeactivate
#define MDLL_PlayerPreThink MDLL_FUNC->pfnPlayerPreThink
#define MDLL_PlayerPostThink MDLL_FUNC->pfnPlayerPostThink
#define MDLL_StartFrame MDLL_FUNC->pfnStartFrame
#define MDLL_ParmsNewLevel MDLL_FUNC->pfnParmsNewLevel
#define MDLL_ParmsChangeLevel MDLL_FUNC->pfnParmsChangeLevel
#define MDLL_GetGameDescription MDLL_FUNC->pfnGetGameDescription
#define MDLL_PlayerCustomization MDLL_FUNC->pfnPlayerCustomization
#define MDLL_SpectatorConnect MDLL_FUNC->pfnSpectatorConnect
#define MDLL_SpectatorDisconnect MDLL_FUNC->pfnSpectatorDisconnect
#define MDLL_SpectatorThink MDLL_FUNC->pfnSpectatorThink
#define MDLL_Sys_Error MDLL_FUNC->pfnSys_Error
#define MDLL_PM_Move MDLL_FUNC->pfnPM_Move
#define MDLL_PM_Init MDLL_FUNC->pfnPM_Init
#define MDLL_PM_FindTextureType MDLL_FUNC->pfnPM_FindTextureType
#define MDLL_SetupVisibility MDLL_FUNC->pfnSetupVisibility
#define MDLL_UpdateClientData MDLL_FUNC->pfnUpdateClientData
#define MDLL_AddToFullPack MDLL_FUNC->pfnAddToFullPack
#define MDLL_CreateBaseline MDLL_FUNC->pfnCreateBaseline
#define MDLL_RegisterEncoders MDLL_FUNC->pfnRegisterEncoders
#define MDLL_GetWeaponData MDLL_FUNC->pfnGetWeaponData
#define MDLL_CmdStart MDLL_FUNC->pfnCmdStart
#define MDLL_CmdEnd MDLL_FUNC->pfnCmdEnd
#define MDLL_ConnectionlessPacket MDLL_FUNC->pfnConnectionlessPacket
#define MDLL_GetHullBounds MDLL_FUNC->pfnGetHullBounds
#define MDLL_CreateInstancedBaselines MDLL_FUNC->pfnCreateInstancedBaselines
#define MDLL_InconsistentFile MDLL_FUNC->pfnInconsistentFile
#define MDLL_AllowLagCompensation MDLL_FUNC->pfnAllowLagCompensation

#define MNEW_FUNC gpGamedllFuncs->newapi_table
#define MNEW_OnFreeEntPrivateData MNEW_FUNC->pfnOnFreeEntPrivateData
#define MNEW_GameShutdown MNEW_FUNC->pfnGameShutdown
#define MNEW_ShouldCollide MNEW_FUNC->pfnShouldCollide
#define MNEW_CvarValue MNEW_FUNC->pfnCvarValue
#define MNEW_CvarValue2 MNEW_FUNC->pfnCvarValue2

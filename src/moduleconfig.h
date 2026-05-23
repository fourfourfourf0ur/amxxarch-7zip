#ifndef AMXXARCH_MODULECONFIG_H
#define AMXXARCH_MODULECONFIG_H

#define MODULE_NAME "AmxxArch"
#define MODULE_VERSION "3.0.0"
#define MODULE_DATE __DATE__
#define MODULE_AUTHOR "AmxxArch"
#define MODULE_URL "https://github.com/Garey27/amxxarch"
#define MODULE_LOGTAG "AmxxArch"
#define MODULE_LIBRARY "amxxarch"
#define MODULE_LIBCLASS "amxxarch"
#define MODULE_RELOAD_ON_MAPCHANGE 0

#define NO_MSVC8_AUTO_COMPAT 1
#define HAVE_STDINT_H 1
#define AMX_NATIVE_CALL
#define AMXAPI
#define AMXEXPORT

#if defined(__linux__)
#define _vsnprintf vsnprintf
#define stricmp strcasecmp
#endif

class CBaseEntity;

#define USE_METAMOD
#define FN_AMXX_ATTACH on_amxx_attach
#define FN_AMXX_DETACH on_amxx_detach
#define FN_StartFrame on_start_frame

#endif

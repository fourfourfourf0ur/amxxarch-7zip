#ifndef AMXXARCH_AMXX_COMPAT_H
#define AMXXARCH_AMXX_COMPAT_H

#define NO_MSVC8_AUTO_COMPAT 1
#define HAVE_STDINT_H 1
#define AMX_NATIVE_CALL
#define AMXAPI
#define AMXEXPORT

#include <stdint.h>

#if defined(__linux__)
#define _vsnprintf vsnprintf
#endif

class CBaseEntity;

#endif

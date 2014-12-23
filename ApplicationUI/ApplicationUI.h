#pragma once

#include "Platform/System.h"

#if HELIUM_SHARED
# ifdef HELIUM_APPLICATION_UI_EXPORTS
#  define HELIUM_APPLICATION_UI_API HELIUM_API_EXPORT
# else
#  define HELIUM_APPLICATION_UI_API HELIUM_API_IMPORT
# endif
#else
# define HELIUM_APPLICATION_UI_API
#endif

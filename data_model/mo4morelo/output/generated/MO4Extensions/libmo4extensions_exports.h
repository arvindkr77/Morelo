//@<COPYRIGHT>@
//==================================================
//Copyright $2024.
//Siemens Product Lifecycle Management Software Inc.
//All Rights Reserved.
//==================================================
//@<COPYRIGHT>@

/** 
    @file 

    This file contains the declaration for the Dispatch Library  MO4Extensions

*/

#include <common/library_indicators.h>

#ifdef EXPORTLIBRARY
#define EXPORTLIBRARY something else
#error ExportLibrary was already defined
#endif

#define EXPORTLIBRARY            libMO4Extensions

#if !defined(LIBMO4EXTENSIONS) && !defined(IPLIB)
#   error IPLIB or LIBMO4EXTENSIONS is not defined
#endif

/* Handwritten code should use MO4EXTENSIONS_API, not MO4EXTENSIONSEXPORT */

#define MO4EXTENSIONS_API MO4EXTENSIONSEXPORT

#if IPLIB==libMO4Extensions || defined(LIBMO4EXTENSIONS)
#   if defined(__lint)
#       define MO4EXTENSIONSEXPORT       __export(MO4Extensions)
#       define MO4EXTENSIONSGLOBAL       extern __global(MO4Extensions)
#       define MO4EXTENSIONSPRIVATE      extern __private(MO4Extensions)
#   elif defined(_WIN32)
#       define MO4EXTENSIONSEXPORT       __declspec(dllexport)
#       define MO4EXTENSIONSGLOBAL       extern __declspec(dllexport)
#       define MO4EXTENSIONSPRIVATE      extern
#   else
#       define MO4EXTENSIONSEXPORT
#       define MO4EXTENSIONSGLOBAL       extern
#       define MO4EXTENSIONSPRIVATE      extern
#   endif
#else
#   if defined(__lint)
#       define MO4EXTENSIONSEXPORT       __export(MO4Extensions)
#       define MO4EXTENSIONSGLOBAL       extern __global(MO4Extensions)
#   elif defined(_WIN32) && !defined(WNT_STATIC_LINK)
#       define MO4EXTENSIONSEXPORT      __declspec(dllimport)
#       define MO4EXTENSIONSGLOBAL       extern __declspec(dllimport)
#   else
#       define MO4EXTENSIONSEXPORT
#       define MO4EXTENSIONSGLOBAL       extern
#   endif
#endif

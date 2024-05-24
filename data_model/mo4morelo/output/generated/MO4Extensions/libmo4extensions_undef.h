//@<COPYRIGHT>@
//==================================================
//Copyright $2024.
//Siemens Product Lifecycle Management Software Inc.
//All Rights Reserved.
//==================================================
//@<COPYRIGHT>@


#include <common/library_indicators.h>

#if !defined(EXPORTLIBRARY)
#   error EXPORTLIBRARY is not defined
#endif

#undef EXPORTLIBRARY

#if !defined(LIBMO4EXTENSIONS) && !defined(IPLIB)
#   error IPLIB or LIBMO4EXTENSIONS is not defined
#endif

#undef MO4EXTENSIONS_API
#undef MO4EXTENSIONSEXPORT
#undef MO4EXTENSIONSGLOBAL
#undef MO4EXTENSIONSPRIVATE

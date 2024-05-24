//@<COPYRIGHT>@
//==================================================
//Copyright $2023.
//Siemens Product Lifecycle Management Software Inc.
//All Rights Reserved.
//==================================================
//@<COPYRIGHT>@

/* 
 * @file 
 *
 *   This file contains the declaration for the Extension MO4_init_name_extension
 *
 */
 
#ifndef MO4_INIT_NAME_EXTENSION_HXX
#define MO4_INIT_NAME_EXTENSION_HXX
#include <tccore/method.h>
#include <MO4Extensions/libmo4extensions_exports.h>
#ifdef __cplusplus
         extern "C"{
#endif
                 
extern MO4EXTENSIONS_API int MO4_init_name_extension(METHOD_message_t* msg, va_list args);
                 
#ifdef __cplusplus
                   }
#endif

#include <MO4Extensions/libmo4extensions_undef.h>
                
#endif  // MO4_INIT_NAME_EXTENSION_HXX

//@<COPYRIGHT>@
//==================================================
//Copyright $2024.
//Siemens Product Lifecycle Management Software Inc.
//All Rights Reserved.
//==================================================
//@<COPYRIGHT>@

/* 
 * @file 
 *
 *   This file contains the declaration for the Extension MO4_register_epm_handlers
 *
 */
 
#ifndef MO4_REGISTER_EPM_HANDLERS_HXX
#define MO4_REGISTER_EPM_HANDLERS_HXX
#include <tccore/method.h>
#include <epm/epm.h>
#include <MO4Extensions/libmo4extensions_exports.h>
#ifdef __cplusplus
         extern "C"{
#endif
                 
extern MO4EXTENSIONS_API int MO4_register_epm_handlers(METHOD_message_t* msg, va_list args);
                 
#ifdef __cplusplus
                   }
#endif
                
#include <MO4Extensions/libmo4extensions_undef.h>
                
#endif  // MO4_REGISTER_EPM_HANDLERS_HXX

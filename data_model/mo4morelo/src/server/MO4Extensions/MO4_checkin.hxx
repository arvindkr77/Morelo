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
 *   This file contains the declaration for the Extension MO4_checkin
 *
 */
 
#ifndef MO4_CHECKIN_HXX
#define MO4_CHECKIN_HXX

#include <string>
#include <base_utils/IFail.hxx>
#include <bom/bom.h>
#include <cfm/cfm.h>
#include <fclasses/tc_stdarg.h>
#include <fclasses/tc_string.h>
#include <mld/logging/Logger.hxx>
#include <pom/enq/enq.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <tccore/grm.h>
#include <tccore/grmtype.h>
#include <tccore/method.h>
#include <tccore/item.h>
#include <tccore/tctype.h>
#include <tc/tc_errors.h>
#include <user_exits/epm_toolkit_utils.h>
#include <ug_va_copy.h>
#include "MO4_errors.hxx"
#include "MO4_utils.hxx"

#include <MO4Extensions/libmo4extensions_exports.h>
#ifdef __cplusplus
         extern "C"{
#endif
                 
extern MO4EXTENSIONS_API int MO4_checkin(METHOD_message_t* msg, va_list args);
extern int getEngPartRevForBOM(tag_t rimObject, tag_t* engPartObject);
                 
#ifdef __cplusplus
                   }
#endif
                
#include <MO4Extensions/libmo4extensions_undef.h>
                
#endif  // MO4_CHECKIN_HXX

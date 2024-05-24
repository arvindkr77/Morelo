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

inline Teamcenter::Logging::Logger* getLogger()
{
	return Teamcenter::Logging::Logger::getLogger("Teamcenter.MO4");
}

#define logDebug(message, ...) \
{\
	Teamcenter::Logging::Logger* logger = getLogger();\
	logger->printf(Teamcenter::Logging::Logger::LOG_DEBUG, "Debug:",message, __FUNCTION__, __LINE__, __VA_ARGS__);\
}
#define logInfo(message, ...) \
{\
	Teamcenter::Logging::Logger* logger = getLogger();\
	logger->printf(Teamcenter::Logging::Logger::LOG_INFO, "Info:", message, __VA_ARGS__);\
}
#define logError(message, ...) \
{\
	Teamcenter::Logging::Logger* logger = getLogger();\
	logger->printf(Teamcenter::Logging::Logger::LOG_ERROR, "Error:", message, __VA_ARGS__);\
}

#include <MO4Extensions/libmo4extensions_exports.h>
#ifdef __cplusplus
         extern "C"{
#endif
                 
extern MO4EXTENSIONS_API int MO4_checkin(METHOD_message_t* msg, va_list args);
                 
#ifdef __cplusplus
                   }
#endif
                
#include <MO4Extensions/libmo4extensions_undef.h>
                
#endif  // MO4_CHECKIN_HXX

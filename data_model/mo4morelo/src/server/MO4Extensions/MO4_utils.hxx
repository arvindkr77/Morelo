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
 *  This file contains macros and functions for
 *  the implementation of checkin and delete
 *
 */


#ifndef SRC_SERVER_MO4EXTENSIONS_MO4_UTILS_HXX_
#define SRC_SERVER_MO4EXTENSIONS_MO4_UTILS_HXX_

#include <base_utils/IFail.hxx>
#include <base_utils/ScopedSmPtr.hxx>
#include <bom/bom.h>
#include <cfm/cfm.h>
#include <mld/logging/Logger.hxx>
#include <pom/enq/enq.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <tccore/method.h>
#include "MO4_errors.hxx"
#include <user_exits/epm_toolkit_utils.h>

// constant definitions
#define MO4_ENG_PART_TYPE_NAME 			"MO4_Eng_Part"
#define MO4_RIM_TYPE_NAME 				"MO4_Rim"
#define MO4_RIM_DATA_TYPE_NAME 			"MO4_rim_data"
#define MO4_COVER_TYPE_NAME				"MO4_Cover"
#define MO4_COVER_DATA_TYPE_NAME 		"MO4_cover_data"
#define MO4_ARTICLE_NO_ATTR 			"mo4_article_number"
#define MO4_BOM_ALIGNED_ATTR 			"mo4_bom_aligned"
#define MO4_FINDNO_ATTR         		"bl_sequence_no"
#define MO4_ID_ATTR						"mo4_id"
#define MO4_MATERIAL_CODE_ATTR 			"mo4_material_code"
#define ITEM_ID_ATTR					"item_id"
#define OBJECT_NAME_ATTR				"object_name"
#define MO4_OBJECT_NAME_SEPARATOR	"-"

// Logging
inline Teamcenter::Logging::Logger* getLogger()
{
	std::string loggerName = "Teamcenter.MO4";
	return Teamcenter::Logging::Logger::getLogger(loggerName);
}

#define MO4_logDebug(message, ...) \
{\
	Teamcenter::Logging::Logger* logger = getLogger();\
	logger->printf(Teamcenter::Logging::Logger::LOG_DEBUG, "Debug:"); \
	logger->printf(Teamcenter::Logging::Logger::LOG_DEBUG, message, __VA_ARGS__); \
	logger->printf(Teamcenter::Logging::Logger::LOG_DEBUG, "\n"); \
}
#define _MO4_logInfo(message, ...) \
{\
	Teamcenter::Logging::Logger* logger = getLogger();\
	logger->printf(Teamcenter::Logging::Logger::LOG_INFO, "Info:"); \
	logger->printf(Teamcenter::Logging::Logger::LOG_INFO, message, __VA_ARGS__);\
	logger->printf(Teamcenter::Logging::Logger::LOG_INFO, "\n"); \
}
#define MO4_logError(message, ...) \
{\
	Teamcenter::Logging::Logger* logger = getLogger();\
	logger->printf(Teamcenter::Logging::Logger::LOG_ERROR, "Error:"); \
	logger->printf(Teamcenter::Logging::Logger::LOG_ERROR, message, __VA_ARGS__);\
	logger->printf(Teamcenter::Logging::Logger::LOG_ERROR, "\n"); \
}


// function declarations

// function finds Part using a given business object type, an attribute name and attribute value
// returns one Part object or error if not found or to many objects found
int MO4_findPartviaAttributeValue(const char *partTypeName, char *targetAttrName, char* attrValue, tag_t* engPartObject);

#endif /* SRC_SERVER_MO4EXTENSIONS_MO4_UTILS_HXX_ */

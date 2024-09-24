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
 *  This file contains error codes for the implementation
 *  of checkin and delete
 *
 */

#ifndef MO4_ERRORS_HXX_
#define MO4_ERRORS_HXX_

#include <common/emh_const.h>


#define MO4_BOM_NOT_FOUND_ERROR 				(EMH_USER_error_base + 100)
#define MO4_BOM_MORE_THAN_ONE_FOUND_ERROR 		(EMH_USER_error_base + 101)
#define MO4_BOMLINE_NOT_FOUND_ERROR 			(EMH_USER_error_base + 102)
#define MO4_ENGPART_NOT_FOUND_ERROR 			(EMH_USER_error_base + 103)
#define MO4_ENGPART_MORE_THAN_ONE_FOUND_ERROR 	(EMH_USER_error_base + 104)
#define MO4_MATERIAL_NOT_FOUND_ERROR 			(EMH_USER_error_base + 105)
#define MO4_MATERIAL_MORE_THAN_ONE_FOUND_ERROR  (EMH_USER_error_base + 106)
#define MO4_INVALID_PARAMETER_ERROR  			(EMH_USER_error_base + 107)
#define MO4_MATERIAL_FOUND_ERROR 			    (EMH_USER_error_base + 108)
#endif /* MO4_ERRORS_HXX_ */

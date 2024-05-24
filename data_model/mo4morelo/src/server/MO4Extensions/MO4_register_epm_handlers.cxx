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
 *   This file contains the implementation for the Extension MO4_register_epm_handlers
 *
 */
#include <MO4Extensions/MO4_register_epm_handlers.hxx>
#include <MO4Extensions/MO4_wf_action_handler_impl.hxx>
#include "MO4_utils.hxx"

int MO4_register_epm_handlers( METHOD_message_t * /*msg*/, va_list /*args*/ )
{
	const char *pcFunctionName = "MO4_register_epm_handlers";
    int iRetCode = ITK_ok;

    TC_write_syslog("==>%s\n", pcFunctionName);

    // iRetCode = EPM_register_rule_handler( "MO4-custom-rule-handler", "This is a custom rule handler", MO4_rh_rule_handler);

    iRetCode = EPM_register_action_handler( "MO4-create-part-for-design", "This is a custom action handler", MO4_ah_create_part_for_design);

    TC_write_syslog("<==%s\n", pcFunctionName);

    return iRetCode;
}

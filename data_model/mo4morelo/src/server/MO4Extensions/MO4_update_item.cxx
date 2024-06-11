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
 *   This file contains the implementation for the Extension MO4_update_item
 *
 */
#include <MO4Extensions/MO4_update_item.hxx>
#include <ug_va_copy.h>
#include <tccore/aom.h>
#include <tccore/aom_prop.h>
#include <tccore/item.h>
#include <fclasses/tc_string.h>
#include <user_exits/epm_toolkit_utils.h>

int MO4_update_item( METHOD_message_t * /*msg*/, va_list args )
{
	const char *pcFunctionName = "MO4_update_item";

    int ifail = ITK_ok;
    tag_t tRev = NULLTAG;
    tag_t tItem = NULLTAG;
    char *itemType = NULL;
    logical lIsChangedName = false;
    logical lIsChangedExt = false;
    logical lIsChangedName2 = false;
    logical lIsDrwType = false;
    logical lModifiable = true;
    va_list largs;

    TC_write_syslog("==>%s\n", pcFunctionName);

    va_copy(largs, args);
    tRev = va_arg(largs, tag_t);
    va_end(largs);

    ifail = ITEM_ask_item_of_rev(tRev, &tItem);
    if (ifail == ITK_ok)
    	ifail = ITEM_ask_type2(tItem, &itemType);

    TC_write_syslog("itemType=%s\n", itemType);

    if(tc_strcmp(itemType,"MO4_Drawing") == 0)
    	lIsDrwType = true;

    if (ifail == ITK_ok && tRev != NULLTAG && tItem != NULLTAG)
    {
        char* valueExtRev = NULL;
        char* valueExtItem = NULL;
        char* valueNameRev = NULL;
        char* valueNameItem = NULL;
        char* valueName2Rev = NULL;
        char* valueName2Item = NULL;

        ifail = AOM_ask_value_string(tRev, "mo4_name_extension", &valueExtRev);
        if (ifail == ITK_ok)
            ifail = AOM_ask_value_string(tItem, "mo4_name_extension", &valueExtItem);
        TC_write_syslog("valueExtItem = %s\n", valueExtItem);
        if (ifail == ITK_ok && valueExtRev != NULL && valueExtItem != NULL && tc_strcmp(valueExtRev, valueExtItem) != 0)
        {
            lIsChangedExt = true;
        }
        TC_write_syslog("lIsChangedExt = %d\n", lIsChangedExt);

        if (ifail == ITK_ok)
            ifail = AOM_ask_value_string(tRev, "object_name", &valueNameRev);
        if (ifail == ITK_ok)
            ifail = AOM_ask_value_string(tItem, "object_name", &valueNameItem);
        if (ifail == ITK_ok && valueNameRev != NULL && valueNameItem != NULL && tc_strcmp(valueNameRev, valueNameItem) != 0)
        {
            lIsChangedName = true;
        }
        TC_write_syslog("lIsChangedName = %d\n", lIsChangedName);

        if (ifail == ITK_ok && lIsDrwType == false)
            ifail = AOM_ask_value_string(tRev, "mo4_object_name", &valueName2Rev);
        if (ifail == ITK_ok && lIsDrwType == false)
            ifail = AOM_ask_value_string(tItem, "mo4_object_name", &valueName2Item);
        if (ifail == ITK_ok && valueName2Rev != NULL && valueName2Item != NULL && tc_strcmp(valueName2Rev, valueName2Item) != 0)
        {
            lIsChangedName2 = true;
        }
        TC_write_syslog("lIsChangedName2 = %d\n", lIsChangedName2);

        if (ifail == ITK_ok && (lIsChangedExt || lIsChangedName || lIsChangedName2))
        {
            ifail = POM_modifiable(tItem, &lModifiable);
            if (ifail == ITK_ok && !lModifiable)
            {
                ifail = AOM_refresh(tItem, POM_modify_lock);
            }
            if (ifail == ITK_ok && lIsChangedExt)
                ifail = AOM_set_value_string(tItem, "mo4_name_extension", valueExtRev);
            if (ifail == ITK_ok && lIsChangedName)
                ifail = AOM_set_value_string(tItem, "object_name", valueNameRev);
            if (ifail == ITK_ok && lIsChangedName2 && lIsDrwType == false)
                ifail = AOM_set_value_string(tItem, "mo4_object_name", valueName2Rev);
            if (ifail == ITK_ok)
                ifail = AOM_save_with_extensions(tItem);
            if (!lModifiable)
            {
                AOM_refresh(tItem, POM_no_lock);
            }
        }
        SAFE_SM_FREE(valueExtRev);
        SAFE_SM_FREE(valueExtItem);
        SAFE_SM_FREE(valueNameRev);
        SAFE_SM_FREE(valueNameItem);
        SAFE_SM_FREE(valueName2Rev);
        SAFE_SM_FREE(valueName2Item);
    }

    TC_write_syslog("<==%s\n", pcFunctionName);

    return ifail;
}

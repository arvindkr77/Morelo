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
    int ifail = ITK_ok;
    tag_t tRev = NULLTAG;
    tag_t tItem = NULLTAG;
    logical lIsChangedName = false;
    logical lIsChangedExt = false;
    logical lModifiable = true;
    va_list largs;

    va_copy(largs, args);
    tRev = va_arg(largs, tag_t);
    va_end(largs);

    ifail = ITEM_ask_item_of_rev(tRev, &tItem);
    if (ifail == ITK_ok && tRev != NULLTAG && tItem != NULLTAG)
    {
        char* valueExtRev = NULL;
        char* valueExtItem = NULL;
        char* valueNameRev = NULL;
        char* valueNameItem = NULL;
        ifail = AOM_ask_value_string(tRev, "mo4_name_extension", &valueExtRev);
        if (ifail == ITK_ok)
            ifail = AOM_ask_value_string(tItem, "mo4_name_extension", &valueExtItem);
        if (ifail == ITK_ok && valueExtRev != NULL && valueExtItem != NULL && tc_strcmp(valueExtRev, valueExtItem) != 0)
        {
            lIsChangedExt = true;
        }
        if (ifail == ITK_ok)
            ifail = AOM_ask_value_string(tRev, "object_name", &valueNameRev);
        if (ifail == ITK_ok)
            ifail = AOM_ask_value_string(tItem, "object_name", &valueNameItem);
        if (ifail == ITK_ok && valueNameRev != NULL && valueNameItem != NULL && tc_strcmp(valueNameRev, valueNameItem) != 0)
        {
            lIsChangedName = true;
        }

        if (ifail == ITK_ok && (lIsChangedExt || lIsChangedName))
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
    }
    return ifail;
}

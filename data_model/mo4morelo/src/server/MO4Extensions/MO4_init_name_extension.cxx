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
 *   This file contains the implementation for the Extension MO4_init_name_extension
 *
 */
#include "MO4_init_name_extension.hxx"

#include <ug_va_copy.h>
#include <tccore/aom_prop.h>
#include <tccore/item.h>
#include <fclasses/tc_string.h>
#include <base_utils/ScopedSmPtr.hxx>
#include <base_utils/IFail.hxx>
#include <base_utils/TcResultStatus.hxx>
#include <metaframework/CreateInput.hxx>

using namespace std;
using namespace Teamcenter;

int MO4_init_name_extension( METHOD_message_t * /*msg*/, va_list args )
{
	const char *pcFunctionName = "MO4_init_name_extension";
    int ifail = ITK_ok;
    ResultStatus stat;

    TC_write_syslog("==>%s\n", pcFunctionName);

    try
    {
        va_list largs;
        va_copy(largs, args);
        CreateInput *createInput = va_arg(largs, CreateInput*);
        va_end( largs );

        bool isNull = true;
        string sNameExtension;
        string sName2;
        tag_t tItem = NULLTAG;
        char *itemType = NULL;

        stat = createInput->getString("mo4_name_extension", sNameExtension, isNull);
        if (isNull)
        {
            stat = createInput->getTag("items_tag", tItem, isNull);
            if (!isNull)
            {
                scoped_smptr<char> spcNameExtension;
                stat = AOM_ask_value_string(tItem, "mo4_name_extension", &spcNameExtension);
                stat = createInput->setString("mo4_name_extension", spcNameExtension.getString(), false);
            }
        }

        stat = createInput->getString("mo4_object_name", sName2, isNull);
        if (isNull)
        {
            stat = createInput->getTag("items_tag", tItem, isNull);
            if (!isNull)
            {
                scoped_smptr<char> spcName2;
                ifail = ITEM_ask_type2(tItem, &itemType);
                if(ifail == ITK_ok && tc_strcmp(itemType,"MO4_Drawing") !=0)
                {
					stat = AOM_ask_value_string(tItem, "mo4_object_name", &spcName2);
					stat = createInput->setString("mo4_object_name", spcName2.getString(), false);
                }
            }
        }
    }
    catch(IFail & ex)
    {
        return ex.ifail();
    }

    TC_write_syslog("<==%s\n", pcFunctionName);

    return ifail;
}

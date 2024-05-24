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
#include <base_utils/ScopedSmPtr.hxx>
#include <base_utils/IFail.hxx>
#include <base_utils/TcResultStatus.hxx>
#include <metaframework/CreateInput.hxx>

using namespace std;
using namespace Teamcenter;

int MO4_init_name_extension( METHOD_message_t * /*msg*/, va_list args )
{
    int ifail = ITK_ok;
    ResultStatus stat;

    try
    {
        va_list largs;
        va_copy(largs, args);
        CreateInput *createInput = va_arg(largs, CreateInput*);
        va_end( largs );

        bool isNull = true;
        string sNameExtension;
        stat = createInput->getString("mo4_name_extension", sNameExtension, isNull);
        if (isNull)
        {
            tag_t tItem = NULLTAG;
            stat = createInput->getTag("items_tag", tItem, isNull);
            if (!isNull)
            {
                scoped_smptr<char> spcNameExtension;
                stat = AOM_ask_value_string(tItem, "mo4_name_extension", &spcNameExtension);
                stat = createInput->setString("mo4_name_extension", spcNameExtension.getString(), false);
            }
        }
    }
    catch(IFail & ex)
    {
        return ex.ifail();
    }
    return ifail;
}

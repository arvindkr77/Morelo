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
#include <qry/qry.h>
#include "MO4_utils.hxx"
#include <base_utils/ScopedSmPtr.hxx>
#include <base_utils/IFail.hxx>
#include <base_utils/TcResultStatus.hxx>
#include <metaframework/CreateInput.hxx>

using namespace std;
using namespace Teamcenter;

int MO4_init_name_extension( METHOD_message_t * /*msg*/, va_list args )
{
    int ifail = ITK_ok;
	int iRowCount = 0;
	int iEPRevObjCount = 0;

    bool isNull = true;
	logical exists;

    ResultStatus stat;

	char* sObjectType = NULL;
	char* sArticleNumber = NULL;

	const char *pcFunctionName = "MO4_init_name_extension";
	char *ppcAttributeNames[1] = { "Article Number" };

	char **ppcAttributeValues  = NULL;

    string sNameExtension;
    string sName2;
    string sName3;

    tag_t tItem = NULLTAG;
	tag_t tQry = NULLTAG;

	tag_t *pptEPRevObjs = NULL;

    TC_write_syslog("==>%s\n", pcFunctionName);

    try
    {
        va_list largs;
        va_copy(largs, args);
        CreateInput *createInput = va_arg(largs, CreateInput*);
        va_end( largs );

		stat = createInput->getTag("items_tag", tItem, isNull);
		if(isNull)
		{
			return ifail;
		}

		stat = AOM_ask_value_string(tItem, "object_type", &sObjectType);

		TC_write_syslog("sObjectType = %s\n", sObjectType);

		if(tc_strcmp(sObjectType, "MO4_Eng_Part") == 0)
		{
			stat = AOM_ask_value_string(tItem, "mo4_article_number", &sArticleNumber);

			TC_write_syslog("sArticleNumber = %s\n", sArticleNumber);

			if(sArticleNumber!=NULL && strlen(sArticleNumber) > 0)
			{
				ppcAttributeValues = (char **) MEM_alloc( sizeof(char *) * 1 );

				tc_strdup(sArticleNumber, (char **) &(ppcAttributeValues[0]));

				if (!ifail) ifail = QRY_find2 ("Find Engineering Part Revision", &tQry);

				if (!ifail) ifail = QRY_execute(tQry, 1, ppcAttributeNames, ppcAttributeValues, &iEPRevObjCount, &pptEPRevObjs);

				if(iEPRevObjCount > 0)
				{
					MO4_logError("Article Number = %s is not unique\n", sArticleNumber);
					EMH_store_error(EMH_severity_error, MO4_MATERIAL_FOUND_ERROR);
					return MO4_MATERIAL_FOUND_ERROR;
				}
			}
		}

//        stat = createInput->getString("mo4_name_extension", sNameExtension, isNull);
//        if (isNull)
//        {
//			scoped_smptr<char> spcNameExtension;
//			stat = AOM_ask_value_string(tItem, "mo4_name_extension", &spcNameExtension);
//			stat = createInput->setString("mo4_name_extension", spcNameExtension.getString(), false);
//        }
//
//        stat = createInput->getString("mo4_object_name", sName2, isNull);
//        if (isNull)
//        {
//			scoped_smptr<char> spcName2;
//			stat = AOM_ask_value_string(tItem, "mo4_object_name", &spcName2);
//			stat = createInput->setString("mo4_object_name", spcName2.getString(), false);
//        }
//
//        stat = createInput->getString("mo4_name3", sName3, isNull);
//        if (isNull)
//        {
//			scoped_smptr<char> spcName3;
//			stat = AOM_ask_value_string(tItem, "mo4_name3", &spcName3);
//			stat = createInput->setString("mo4_name3", spcName3.getString(), false);
//        }
    }
    catch(IFail & ex)
    {
        return ex.ifail();
    }

    TC_write_syslog("<==%s\n", pcFunctionName);

    return ifail;
}

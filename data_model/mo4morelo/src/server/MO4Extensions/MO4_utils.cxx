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
 *   This file contains the implementation of functions
 *   for checkin and delete
 *
 */
#include <MO4Extensions/MO4_utils.hxx>

// function finds Engineering Part using a given attribute name and attribute value
// returns one Engineering Part or error if not found or to many objects found
int MO4_findPartviaAttributeValue(const char* partTypeName, char *targetAttrName, char* attrValue, tag_t* engPartObject)
{
	int ifail = ITK_ok;
	int count = 0;
	int columns = 0;
	const char *qry_id = "findEngPart";
	const char *select_attrs[] = {"puid"};
	void ***results = NULL;


	MO4_logDebug("Start: %s", __FUNCTION__);

	/* check input parameters */
	if (!partTypeName|| !targetAttrName|| !attrValue)
	{
		if (!partTypeName)
			MO4_logError("Bad input parameter <%s> for function <%s>.\n", "Part Type Name", __FUNCTION__);
		if (!targetAttrName)
			MO4_logError("Bad input parameter <%s> for function <%s>.\n", "Target Attribute Name", __FUNCTION__);
		if (!attrValue)
			MO4_logError("Bad input parameter <%s> for function <%s>.\n", "Target Attribute Value", __FUNCTION__);
		return MO4_INVALID_PARAMETER_ERROR;
	}

	*engPartObject = NULLTAG;
	if ((ifail = POM_enquiry_create(qry_id)) != ITK_ok)
	{
		POM_enquiry_delete(qry_id);
		return ifail;
	}
	if (!ifail)
		ifail = POM_enquiry_add_select_attrs(qry_id, MO4_ENG_PART_TYPE_NAME, 1, select_attrs);
	if (!ifail)
		ifail = POM_enquiry_set_string_value(qry_id, "valId", 1,  (const char **) &attrValue, POM_enquiry_bind_value);
	if (!ifail)
		ifail = POM_enquiry_set_attr_expr(qry_id, "exp", partTypeName, targetAttrName, POM_enquiry_equal, "valId");
	if (!ifail)
		ifail = POM_enquiry_set_where_expr(qry_id, "exp");
	if (!ifail)
		ifail = POM_enquiry_execute(qry_id, &count, &columns, &results);

	/* Should always be just one */
	if (count == 1 && results)
	{
		MO4_logDebug("Searched <%s> <%s> found.", partTypeName, attrValue);
		*engPartObject = *(tag_t *)results[0][0];
		AOM_refresh(*engPartObject, POM_no_lock);
	}
	else /* Error */
	{
		if (count > 1)
		{
			if (attrValue) MO4_logError("Searched <%s> <%s> - more than one found!", partTypeName, attrValue);
			if (tc_strcmp(partTypeName,MO4_ENG_PART_TYPE_NAME) == 0)
				ifail = MO4_ENGPART_MORE_THAN_ONE_FOUND_ERROR;
			else
				ifail = MO4_MATERIAL_MORE_THAN_ONE_FOUND_ERROR;
		}
		else
		{
			if (attrValue) MO4_logError("Searched <%s> <%s> not found.", partTypeName, attrValue);
			if (tc_strcmp(partTypeName,MO4_ENG_PART_TYPE_NAME) == 0)
				ifail = MO4_ENGPART_NOT_FOUND_ERROR;
			else
				ifail = MO4_MATERIAL_NOT_FOUND_ERROR;
		}
	}

	POM_enquiry_delete(qry_id);
	SAFE_SM_FREE(results);

	MO4_logDebug("End: %s",__FUNCTION__);
	return ifail;
}


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
 *   This file contains the implementation for the Extension MO4_checkin
 *   HFR
 */

#include <MO4Extensions/MO4_checkin.hxx>

// function adds given material to existing or new created BOM
int MO4_addMaterialToBOM(tag_t engPartRevObject, tag_t matObject, int id)
{
	int ifail = ITK_ok;
	int bvCount = 0;
	tag_t* bvList = NULLTAG;
	tag_t bv = NULLTAG;
	tag_t bvr = NULLTAG;
	tag_t engPartObject = NULLTAG;
	tag_t bomWindow = NULLTAG;
	tag_t topLine = NULLTAG;
	tag_t rule = NULLTAG;
	tag_t childLine = NULLTAG;
	char idStr[100] = "";
	char* itemId = NULL;

	MO4_logDebug("Start: %s",__FUNCTION__);

	/* check input parameters */
	if (!engPartRevObject|| !matObject || id < 0)
	{
		if (!engPartRevObject)
			MO4_logError("No Engineering Part Revision object given for function <%s>.\n", __FUNCTION__);
		if (!matObject)
			MO4_logError("No Material object given for function <%s>.\n", __FUNCTION__);
		if (id < 0)
			MO4_logError("Invalid ID value given for function <%s> (ID < 0).\n", __FUNCTION__);
		return MO4_INVALID_PARAMETER_ERROR;
	}

	ITEM_ask_id2(engPartObject, &itemId);
	if (id > -1)
		sprintf(idStr, "%d", id);

	/* check BOM exists */
	ifail = ITEM_ask_item_of_rev(engPartRevObject, &engPartObject);
	if (engPartObject)
		ifail = ITEM_list_bom_views(engPartObject, &bvCount, &bvList);

	if (bvCount == 0)
	{
		MO4_logDebug("No BOM found - create one %s.", "\n");
		ifail = PS_create_bom_view (NULLTAG, "", "", engPartObject, &bv);
		if (!ifail)
			ifail = AOM_save_without_extensions(bv);
		if (!ifail)
			ifail = PS_create_bvr(bv, "", "",  false, engPartRevObject, &bvr);
		if (!ifail)
			ifail = AOM_save_without_extensions(bvr);
		if (!ifail)
			ifail = AOM_save_without_extensions(engPartObject);
	}
	else /* Add bomline to existing bom */
	{
		MO4_logDebug("BOM found - use it. %s", "\n");
		bv = bvList[0];
	}

	if (!ifail)
		ifail = BOM_create_window(&bomWindow);
	if (!ifail)
		ifail = BOM_set_window_top_line(bomWindow, NULLTAG, engPartRevObject, bv, &topLine);

	/* set Revision Rule */
	if (!ifail)
		ifail = CFM_find("Latest Working", &rule);
	if (!ifail)
		ifail = BOM_set_window_config_rule(bomWindow, rule);

	if (!ifail)
		ifail = BOM_line_add(topLine, matObject, NULLTAG,  NULLTAG, &childLine);
	if (!ifail && id )
	{
		MO4_logDebug("Set Find No. <%s>.", idStr);
		ifail = AOM_UIF_set_value(childLine, MO4_FINDNO_ATTR, idStr);
	}
	if (!ifail)
		ifail = BOM_set_window_pack_all(bomWindow, TRUE);
	if (!ifail)
		ifail = BOM_save_window(bomWindow);

	BOM_close_window(bomWindow);

	SAFE_SM_FREE(itemId);
	SAFE_SM_FREE(bvList);

	MO4_logDebug("End: %s",__FUNCTION__);
	return ifail;
}

// function replaces given material in existing BOM
int MO4_updateMaterialInBOM(tag_t engPartRevObject, tag_t matObject, int id)
{
	int ifail = ITK_ok;
	int bvCount = 0;
	int lineCount = 0;
	int found = 0;
	tag_t* bvList = NULLTAG;
	tag_t* childLines = NULLTAG;
	tag_t topLine = NULLTAG;
	tag_t rule = NULLTAG;
	tag_t bv = NULLTAG;
	tag_t bomWindow = NULLTAG;
	tag_t engPartObject = NULLTAG;
	tag_t matRevObject = NULLTAG;
	char idStr[100] = "";
	char* itemId = NULL;

	MO4_logDebug("Start: %s",__FUNCTION__);

	/* check input parameters */
	if (!engPartRevObject|| !matObject || id < 0)
	{
		if (!engPartRevObject)
			MO4_logError("No Engineering Part object given for function <%s>.\n", __FUNCTION__);
		if (!matObject)
			MO4_logError("No Material object given for function <%s>.\n", __FUNCTION__);
		if (id < 0)
			MO4_logError("Invalid ID value given for function <%s> (ID < 0).\n", __FUNCTION__);
		return MO4_INVALID_PARAMETER_ERROR;
	}

	/* check BOM exists */
	ifail = ITEM_ask_item_of_rev(engPartRevObject, &engPartObject);
	if (engPartObject)
	{
		ITEM_ask_id2(engPartObject, &itemId);
		sprintf(idStr, "%d", id);
	}
	else *itemId = idStr[0];

	if (!ifail)
		ifail = ITEM_ask_latest_rev(matObject, &matRevObject);
	if (engPartObject && matRevObject)
		ifail = ITEM_list_bom_views(engPartObject, &bvCount, &bvList);

	switch (bvCount)
	{
		case 1:
			bv = bvList[0];
			break;
		case 0:
			MO4_logError("Field <mo4_bom_aligned> is <%s> but no BOM found at Engineering Part <%s>.", "true", itemId);
			return MO4_BOM_NOT_FOUND_ERROR;
		default:
			MO4_logError("More than one BOM found at Engineering Part <%s>. This is not expected.", itemId);
			return MO4_BOM_MORE_THAN_ONE_FOUND_ERROR;
	}

	/* find and replace BOMLine */
	if (!ifail)
		ifail = BOM_create_window(&bomWindow);
	if (!ifail)
		ifail = BOM_set_window_top_line(bomWindow, NULLTAG, engPartRevObject, bv, &topLine);

	/* set Revision Rule */
	if (!ifail)
		ifail = CFM_find("Latest Working", &rule);
	if (!ifail)
		ifail = BOM_set_window_config_rule(bomWindow, rule);
	if (!ifail)
		ifail = BOM_line_ask_child_lines (topLine, &lineCount, &childLines);

	if (id >-1)
		sprintf(idStr, "%d", id);

	 for (int i=0; i<lineCount; i++)
	 {
		 char *findNo = NULL;
		 int count = 0;
		 char** values = NULL;

		ifail = AOM_ask_displayable_values(childLines[i], MO4_FINDNO_ATTR, &count, &values);
		if (count == 1)
			 findNo = values[0];
		if (findNo && tc_strcmp(findNo, idStr) == 0)
		{
			ifail = BOM_line_replace_in_context(childLines[i], matRevObject);
			MO4_logDebug("BOMLine at Engineering Part <%s> found and material for Find No. <%s> replaced.", itemId, idStr);
			SAFE_SM_FREE(values);
			found = 1;
			break;
		}
		SAFE_SM_FREE(values);
	}

	if (!ifail)
		ifail = BOM_set_window_pack_all(bomWindow, TRUE);
	if (!ifail && found)
		ifail = BOM_save_window(bomWindow);

	if (!found)
	{
		MO4_logError("BOMLine found for Find No. <%s> !", idStr);
		ifail = MO4_BOMLINE_NOT_FOUND_ERROR;
	}
	BOM_close_window(bomWindow);

	SAFE_SM_FREE(itemId);
	SAFE_SM_FREE(childLines);
	SAFE_SM_FREE(bvList);

	MO4_logDebug("End: %s",__FUNCTION__);
	return ifail;
}

// function gets related Engineering Part Revision for a given Rim or Cover object
int MO4_getEngPartRevForBOM(tag_t targetObject, tag_t* engPartObject)
{
	int ifail = ITK_ok;
	int count = 0;
	char* typeName = NULL;
	char* relationTypeName = NULL;
    char* objName = NULL;
	tag_t type = NULLTAG;
	tag_t relationType = NULLTAG;
	tag_t *primaryObjs = NULLTAG;

	MO4_logDebug("Start: %s",__FUNCTION__);

	/* check input parameters */
	if (!targetObject)
	{
		MO4_logError("Bad input parameter <%s> for function <%s>.\n", "Target Objecte", __FUNCTION__);
		return MO4_INVALID_PARAMETER_ERROR;
	}

	WSOM_ask_name2(targetObject, &objName);

	/* Check object type */
	ifail = TCTYPE_ask_object_type (targetObject, &type);
	if (!ifail)
		 ifail = TCTYPE_ask_name2(type, &typeName);
	MO4_logDebug("Checked typeName: <%s>.", typeName);

	if (typeName && strcmp(typeName, MO4_RIM_TYPE_NAME) == 0)
	{
		relationTypeName = MO4_RIM_DATA_TYPE_NAME;
	}
	else if (typeName &&  tc_strcmp(typeName, MO4_COVER_TYPE_NAME) == 0)
	{
		relationTypeName = MO4_COVER_DATA_TYPE_NAME;
	}
	else
	{
		MO4_logDebug("Object <%s> of type <%s> is not designed to be handled by this function.\n", objName, typeName);
		return ITK_ok;
	}

	ifail = GRM_find_relation_type(relationTypeName, &relationType);
	if (relationType)
		ifail = GRM_list_primary_objects_only(targetObject, relationType, &count, &primaryObjs);

	/* Should always be just one */
	if (count == 1 && primaryObjs)
	{
	    *engPartObject = primaryObjs[0];

	    ifail = WSOM_ask_name2(targetObject, &objName);
		MO4_logDebug("Searched Engineering Part for <%s> object <%s> for BOM found.", typeName, objName);

	}
	else /* Error */
	{
		if (count == 0)
		{
			MO4_logError("Searched Engineering Part for <%s> <%s> for BOM not found.", typeName, objName);
			ifail = MO4_ENGPART_NOT_FOUND_ERROR;
		}
		else
		{
			MO4_logError("Searched Engineering Part for <%s> <%s> for checked out: No or more than one found.", typeName, objName);
			ifail = MO4_ENGPART_MORE_THAN_ONE_FOUND_ERROR;
		}
	}

	SAFE_SM_FREE(objName);
	SAFE_SM_FREE(typeName);
	SAFE_SM_FREE(primaryObjs);

	MO4_logDebug("End: %s",__FUNCTION__);
	return ifail;
}

// function sets "mo4_bom_aligned" at Rim or Cover to true
int MO4_activateBomAligned(tag_t targetObject, tag_t matObject)
{
	int ifail = ITK_ok;
	logical isModifiable = false;
	char* itemId = NULL;

	MO4_logDebug("Start: %s",__FUNCTION__);

	ifail = ITEM_ask_id2(matObject, &itemId);

	if (!ifail)
		ifail = POM_modifiable(targetObject, &isModifiable);

	if (!ifail && !isModifiable)
		ifail = AOM_refresh(targetObject, POM_modify_lock);

	if (!ifail)
		ifail = AOM_set_value_logical(targetObject, MO4_BOM_ALIGNED_ATTR, TRUE);

	if (!ifail)
		ifail = AOM_save_without_extensions(targetObject);

	if (!isModifiable)
		ifail = AOM_refresh(targetObject, POM_no_lock);

	SAFE_SM_FREE(itemId);

	MO4_logDebug("End: %s",__FUNCTION__);
	return ifail;
}

/*
 * When Rim or Cover is checked in, the Material code on Rim/ is used to find Engineering Part where
 * Article Number of Engineering Part is equal to Material Code of Rim/Cover. This Engineering Part is
 * then added as BOM child to Engineering Part which parent of Rim/Cover. If the Engineering Part found
 * is already added as BOM child then it is replaced with new Engineering Part.
 */
int MO4_checkin( METHOD_message_t * /*msg*/, va_list args )
{
	int ifail = ITK_ok;
	tag_t targetObject = NULLTAG;
	tag_t matObject = NULLTAG;
	tag_t engPartObject = NULLTAG;
	tag_t type = NULLTAG;
	char* typeName = NULL;
	char* matCode = NULL;
	int id = -1;
	logical bomAligned = false;
	va_list largs;

	MO4_logDebug("Start: %s",__FUNCTION__);

	/* get RIM object */
	va_copy(largs, args);
	targetObject = va_arg(largs, tag_t);
	va_end(largs);


	 /* Check object type */
	 ifail = TCTYPE_ask_object_type (targetObject, &type);
	 if (!ifail)
		 ifail = TCTYPE_ask_name2(type, &typeName);
	 MO4_logDebug("Checked typeName: <%s>.", typeName);
	 if (typeName && (tc_strcmp(typeName, MO4_RIM_TYPE_NAME) == 0 || tc_strcmp(typeName, MO4_COVER_TYPE_NAME) == 0))
	 {
		 MO4_logDebug("Object type found: <%s>.", typeName);
		 ifail = AOM_ask_value_string(targetObject, MO4_MATERIAL_CODE_ATTR, &matCode);
		 ifail = AOM_ask_value_logical(targetObject, MO4_BOM_ALIGNED_ATTR,  &bomAligned);
		 ifail = AOM_ask_value_int(targetObject, MO4_ID_ATTR, &id);

		 /* Create/Update BOM */
		 if (matCode && tc_strlen(matCode) >1)
		 {
			 MO4_logDebug("Found Material Code: <%s>.", matCode);
			 ifail = MO4_findPartviaAttributeValue(MO4_ENG_PART_TYPE_NAME, MO4_ARTICLE_NO_ATTR, matCode, &matObject);

			 /* Create BOM */
			 if (matObject && bomAligned == false)
			 {
				 MO4_logDebug("Material Object <%s> found && bomAligned == <false>.", matCode);
				 MO4_logDebug("Add Material <%s> to BOM.", matCode);
				 ifail = MO4_getEngPartRevForBOM(targetObject, &engPartObject);
				 if (engPartObject)
				 {
					 ifail = MO4_addMaterialToBOM(engPartObject, matObject, id);
					 /* Set BOM Aligned attribute to True */
					 if (!ifail)
						 ifail = MO4_activateBomAligned(targetObject, matObject);
				 }
			 } /* Update BOM */
			 else if (matObject && bomAligned == true)
			 {
				 MO4_logDebug("Material Object <%s> found && bomAligned == <true>.", matCode);
				 MO4_logDebug("Replace Material <%s> in BOM.", matCode);
				 ifail = MO4_getEngPartRevForBOM(targetObject, &engPartObject);
				 if (engPartObject)
				 {
					 if (!ifail)
						 ifail = MO4_updateMaterialInBOM(engPartObject, matObject, id);
				 }
			 }
			 else /* no material object */
			 {
				 MO4_logError("No Material <%s> found. Nothing done.", matCode);
			 }
		 }
		 else
		 {
			 MO4_logError("Field <%s> not set. <%s> object skipped.", MO4_MATERIAL_CODE_ATTR, typeName);
			 MO4_logError("No Material <%s> found. Nothing done.", matCode);
		 }
	 }

	 SAFE_SM_FREE(typeName);

	 MO4_logDebug("End: %s", __FUNCTION__);
	 return ifail;
}

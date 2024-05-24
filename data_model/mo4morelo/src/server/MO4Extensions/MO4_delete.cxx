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
 *   This file contains the implementation for the Extension MO4_delete
 *
 */
#include <MO4Extensions/MO4_delete.hxx>


// function removes given material from BOM
int MO4_removeMaterialFromBOM(tag_t engPartObject, int id)
{
	int ifail = ITK_ok;
	int bvCount = 0;
	int lineCount = 0;
	int found = 0;
	tag_t bv = NULLTAG;
	tag_t topLine = NULLTAG;
	tag_t rule = NULLTAG;
	tag_t bomWindow = NULLTAG;
	tag_t engPartRevObject = NULLTAG;
	tag_t* bvList = NULLTAG;
	tag_t* childLines = NULLTAG;

	char idStr[100] = "";
	char* itemId = NULL;

	MO4_logDebug("Start: %s",__FUNCTION__);

	/* check input parameters */
	if (!engPartObject|| id < 0)
	{
		if (!engPartObject)
			MO4_logError("No Engineering Part object given for function <%s>.\n", __FUNCTION__);
		if (id < 0)
			MO4_logError("Invalid ID value given for function <%s> (ID < 0).\n", __FUNCTION__);
		return MO4_INVALID_PARAMETER_ERROR;
	}

	ITEM_ask_id2(engPartObject, &itemId);
	sprintf(idStr, "%d", id);

	/* check BOM exists */
	ifail = ITEM_ask_latest_rev(engPartObject, &engPartRevObject);
	if (!ifail)
		ifail = ITEM_list_bom_views(engPartObject, &bvCount, &bvList);
	switch (bvCount)
	{
		case 1:
			bv = bvList[0];
			break;
		case 0:
			MO4_logError("Field <mo4_bom_aligned> is <%s> but no BOM found at Engineering Part <%s>.", "true", itemId);
			SAFE_SM_FREE(itemId);
			return MO4_BOM_NOT_FOUND_ERROR;
		default:
			MO4_logError("More than one BOM found at Engineering Part <%s>. This is not expected.", itemId);
			SAFE_SM_FREE(itemId);
			return MO4_BOM_MORE_THAN_ONE_FOUND_ERROR;
	}


	/* find and delete BOMLine */
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
			ifail = BOM_line_cut(childLines[i]);
			MO4_logDebug("BOMLine at Engineering Part <%s> found and material for Find No. <%s> removed.", itemId, idStr);
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
		MO4_logError("BOMLine at Engineering Part <%s> not found for Find No. <%s> !",itemId , idStr);
		ifail = MO4_BOMLINE_NOT_FOUND_ERROR;
	}
	BOM_close_window(bomWindow);

	SAFE_SM_FREE(itemId);
	SAFE_SM_FREE(childLines);
	SAFE_SM_FREE(bvList);

	MO4_logDebug("End: %s",__FUNCTION__);
	return ifail;
}


// function splits given object name at separator
// if separator not found or object name equals separator
// then returns original name
int MO4_getItemIdFromObjectName(char* objectName, char** id)
{
	int ifail = ITK_ok;
	char buffer[WSO_name_size_c] = "";
	char* ptr = NULL;
	MO4_logDebug("Start: %s", __FUNCTION__);

	/* objectName == MO4_OBJECT_NAME_SEPARATOR -> id = MO4_OBJECT_NAME_SEPARATOR */
	if (tc_strcmp(objectName, MO4_OBJECT_NAME_SEPARATOR) == 0)
	{
		*id = (char *)MEM_alloc(sizeof(char) * (tc_strlen(objectName) + 1));
		tc_strcpy(*id, objectName);
		MO4_logDebug("Object Name split from <%s> to <%s>.\n", objectName, *id);
		return ITK_ok;
	}

	sprintf(buffer,"%s", objectName);
	ptr = strtok(buffer, MO4_OBJECT_NAME_SEPARATOR);
	if (ptr)
	{
		*id = (char *)MEM_alloc(sizeof(char) * (tc_strlen(ptr) + 1));
		tc_strcpy(*id, ptr);
	}
	else
	{
		*id = (char *)MEM_alloc(sizeof(char) * (tc_strlen(objectName) + 1));
		tc_strcpy(*id, objectName);
	}
	MO4_logDebug("Object Name split from <%s> to <%s>.\n", objectName, *id);

	MO4_logDebug("End: %s",__FUNCTION__);
	return ifail;
}

// Post action for Rim or Cover delete
int MO4_delete( METHOD_message_t * msg, va_list /*args*/ )
{
	using std::string;

	int ifail = ITK_ok;
	tag_t targetObject = NULLTAG;
	tag_t engPartObject = NULLTAG;
	tag_t type = NULLTAG;
	char* objName = NULL;
	char* typeName = NULL;
	char* itemId = NULL;
	int id = -1;
	logical bomAligned = false;

	MO4_logDebug("Start: %s",__FUNCTION__);

	/* get RIM or Cover object */
	targetObject = msg->object;

	/* get object_name */
	ifail = WSOM_ask_name2(targetObject, &objName);

	/* Check object type */
	if (!ifail)
		 ifail = TCTYPE_ask_object_type (targetObject, &type);
	if (!ifail)
		 ifail = TCTYPE_ask_name2(type, &typeName);

	if (typeName && (tc_strcmp(typeName, MO4_RIM_TYPE_NAME) == 0 || tc_strcmp(typeName, MO4_COVER_TYPE_NAME) == 0))
	{
		MO4_logDebug("Object <%s> of type <%s> found.\n", objName, typeName);
	}
	else
	{
		MO4_logDebug("Object <%s> of type <%s> is not designed to be handled by this function.\n", objName, typeName);
		return ITK_ok;
	}
	/* check if already transfered to BOM */
	ifail = AOM_ask_value_logical(targetObject, MO4_BOM_ALIGNED_ATTR,  &bomAligned);

	if (bomAligned == true)
	{
		/* Ask Eng Part via first part of "object_name" attribute of Rim*/
		if (!ifail)
			ifail = MO4_getItemIdFromObjectName(objName, &itemId);
		if (!ifail && itemId)
			ifail = MO4_findPartviaAttributeValue(MO4_ENG_PART_TYPE_NAME, ITEM_ID_ATTR, itemId, &engPartObject);

		/* Update BOM remove Rim or Cover */

		if (!ifail)
			ifail = AOM_ask_value_int(targetObject, MO4_ID_ATTR, &id);
		if (!ifail && bomAligned)
			ifail = MO4_removeMaterialFromBOM(engPartObject, id);
	}
	SAFE_SM_FREE(itemId);
	SAFE_SM_FREE(objName);
	SAFE_SM_FREE(typeName);

	MO4_logDebug("End: %s",__FUNCTION__);
 return ifail;

}

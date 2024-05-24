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
 *
 */
#include <MO4Extensions/MO4_checkin.hxx>


#define MO4_RIM_TYPE_NAME 		"MO4_Rim"
#define MO4_RIM_DATA_TYPE_NAME 	"MO4_rim_data"
#define MO4_ENG_PART_TYPE_NAME 	"MO4_Eng_Part"
#define MO4_MATERIAL_CODE_ATTR 	"mo4_material_code"
#define MO4_BOM_ALIGNED_ATTR 	"mo4_bom_aligned"
#define MO4_ID_ATTR				"mo4_id"
#define MO4_FINDNO_ATTR         "bl_sequence_no"
#define OBJECT_NAME_ATTR		"object_name"

// function adds given material to existing or new created BOM
int addMaterialToBOM(tag_t engPartRevObject, tag_t matObject, int id)
{
	int ifail = ITK_ok;
	int bvCount = 0;
	tag_t* bvList = NULLTAG;
	tag_t  bv = NULLTAG;
	tag_t  bvr = NULLTAG;
	tag_t bomWindow = NULLTAG;
	tag_t engPartObject = NULLTAG;


	TC_write_syslog("addMaterialToBOM \n");
	ifail = ITEM_ask_item_of_rev(engPartRevObject, &engPartObject);
	if (engPartObject)
		ifail = ITEM_list_bom_views(engPartObject, &bvCount, &bvList);
	//ifail = ITEM_rev_list_all_bom_view_revs(engPartRevObject, &bvrCount, &bvrList);

	if (bvCount == 0)
	{
		TC_write_syslog("addMaterialToBOM No BOM found - create one\n");
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
	else
	{
		/* add bomline to existing bom */
		TC_write_syslog("addMaterialToBOM BOM found - use it\n");
		bv = bvList[0];

	}
		tag_t topLine = NULLTAG;
		tag_t rule = NULLTAG;
		tag_t childLine = NULLTAG;


		if (!ifail)
			ifail = BOM_create_window(&bomWindow);
		if (!ifail)
			//ifail = BOM_set_window_top_line(bomWindow, engPartObject, engPartRevObject, bv, &topLine);
			ifail = BOM_set_window_top_line(bomWindow, NULLTAG, engPartRevObject, bv, &topLine);
		//LOG_ERROR_AND_THROW_STATUS;

		if (!ifail)
			ifail = CFM_find("Latest Working", &rule);
		if (!ifail)
			ifail = BOM_set_window_config_rule(bomWindow, rule);
		//LOG_ERROR_AND_THROW_STATUS;

		if (!ifail)
			//ifail = BOM_line_add(topLine, matObject, NULLTAG,  bv, &childLine);
			ifail = BOM_line_add(topLine, matObject, NULLTAG,  NULLTAG, &childLine);
		if (!ifail && id )
		{
			TC_write_syslog("addMaterialToBOM BOM set Find No.\n");
			char buffer[100]="";
			if (id > -1)
				sprintf(buffer,"%d",id);
			ifail = AOM_UIF_set_value(childLine, MO4_FINDNO_ATTR, buffer);
		}
		if (!ifail)
			ifail = BOM_set_window_pack_all(bomWindow, TRUE);
		if (!ifail)
			ifail = BOM_save_window(bomWindow);
		//LOG_ERROR_AND_THROW_STATUS;

		/*if (!ifail)
			ifail = AOM_refresh(bv, FALSE);*/
		//LOG_ERROR_AND_THROW_STATUS;

		BOM_close_window(bomWindow);
		//ERROR_AND_THROW_STATUS;

	TC_write_syslog("addMaterialToBOM finished \n");

	SAFE_SM_FREE(bvList);
	return ifail;
}

// function replaces given material in existing BOM
int updateMaterialInBOM(tag_t engPartRevObject, tag_t matObject, int id)
{

	int ifail = ITK_ok;
	int bvCount = 0;
	int lineCount = 0;
	tag_t* bvList = NULLTAG;
	tag_t* childLines = NULLTAG;
	tag_t topLine = NULLTAG;
	tag_t rule = NULLTAG;
	tag_t  bv = NULLTAG;
	//tag_t  bvr = NULLTAG;
	tag_t bomWindow = NULLTAG;
	tag_t engPartObject = NULLTAG;
	tag_t matRevObject = NULLTAG;
	char** values = NULL;

	TC_write_syslog("updateRimTInBOM not fully implemented yet. \n");

	ifail = ITEM_ask_item_of_rev(engPartRevObject, &engPartObject);
	if (!ifail)
		ifail = ITEM_ask_latest_rev(matObject, &matRevObject);
	if (engPartObject && matRevObject )
		ifail = ITEM_list_bom_views(engPartObject, &bvCount, &bvList);

	if (bvCount >0)
		bv = bvList[0];
	else return ifail;

	// find and replace BOMLine
	if (!ifail)
		ifail = BOM_create_window(&bomWindow);
	if (!ifail)
		ifail = BOM_set_window_top_line(bomWindow, engPartObject, engPartRevObject, bv, &topLine);

		//LOG_ERROR_AND_THROW_STATUS;

	if (!ifail)
		ifail = CFM_find("Latest Working", &rule);
	if (!ifail)
		ifail = BOM_set_window_config_rule(bomWindow, rule);
	if (!ifail)
		 ifail = BOM_line_ask_child_lines (topLine, &lineCount, &childLines);
	 for (int i=0; i<lineCount; i++)
	 {
		 TC_write_syslog("updateRimInBOM search BOMLine. \n");
		 char *findNo = NULL;
		 char idStr[100] = "";
		 int count = 0;
		 sprintf(idStr, "%d", id);

		ifail = AOM_ask_displayable_values(childLines[i], MO4_FINDNO_ATTR, &count, &values);
		 if (count == 1)
			 findNo = values[0];
		 if (findNo && tc_strcmp(findNo, idStr)== 0)
		 {
			TC_write_syslog("updateRimInBOM BOMLine found and replaced. \n");
			ifail = BOM_line_replace_in_context(childLines[i], matRevObject);
			break;
		 }
	 }
	if (!ifail)
			ifail = BOM_set_window_pack_all(bomWindow, TRUE);
	if (!ifail)
		ifail = BOM_save_window(bomWindow);
		//LOG_ERROR_AND_THROW_STATUS;

		/*if (!ifail)
			ifail = AOM_refresh(bv, FALSE);*/
		//LOG_ERROR_AND_THROW_STATUS;

	BOM_close_window(bomWindow);
		//ERROR_AND_THROW_STATUS;

	TC_write_syslog("addMaterialToBOM finished \n");

	SAFE_SM_FREE(childLines);
	SAFE_SM_FREE(bvList);
	SAFE_SM_FREE(values);
	return ifail;
}

// function finds TC Engineering Part using a given material code string via matCode == p.object_name
int findMaterial(char* matCode, tag_t* matObject)
{
	int ifail = ITK_ok;
	int count = 0;
	int columns = 0;
	const char *qry_id = "findMaterial";
	const char *select_attrs[] = {"puid"};
	void ***results;

	TC_write_syslog("findMaterial \n");

	try {
			if ((ifail = POM_enquiry_create(qry_id)) != ITK_ok)
			{
				POM_enquiry_delete(qry_id);
				return ifail;
			}
			ifail = POM_enquiry_add_select_attrs(qry_id, MO4_ENG_PART_TYPE_NAME, 1, select_attrs);
			ifail = POM_enquiry_set_string_value(qry_id, "val_id", 1,  (const char **) &matCode, POM_enquiry_bind_value);
			ifail = POM_enquiry_set_attr_expr(qry_id, "exp1", MO4_ENG_PART_TYPE_NAME, OBJECT_NAME_ATTR, POM_enquiry_equal, "val_id");
			ifail = POM_enquiry_set_where_expr(qry_id, "exp1");
			ifail = POM_enquiry_execute(qry_id, &count, &columns, &results);

			if (count == 1) {
				TC_write_syslog("Material found\n");
				*matObject = *(tag_t *)results[0][0];
				AOM_refresh(*matObject, false);
				char  mem[2000] = "";
				//objDumpToMem(*matObject, &mem);
				if (mem)
					TC_write_syslog(mem);

			}
			ifail = POM_enquiry_delete(qry_id);
		} catch (const IFail &e) {
			ifail = e.ifail();
		}
		TC_write_syslog("findMaterial finished \n");
		SAFE_SM_FREE(results);

		return ifail;
	return 0;
}

// function gets related Engineering Part Revision for a given Rim object
int getEngPartRevForBOM(tag_t rimObject, tag_t* engPartObject)
{
	int ifail = ITK_ok;
	int count = 0;
	tag_t relation = NULLTAG;
	tag_t *primaryObjs = NULLTAG;

	TC_write_syslog("getEngPartRevForBOM \n");

	ifail = GRM_find_relation_type(MO4_RIM_DATA_TYPE_NAME, &relation);
	if (relation)
		ifail = GRM_list_primary_objects_only(rimObject, relation, &count, &primaryObjs);

	/* should always be just one */
	if (primaryObjs)
	{
		TC_write_syslog("EngPartRevForBOM found \n");
	    *engPartObject = primaryObjs[0];
	}
	TC_write_syslog("getEngPartRevForBOM finished \n");

	SAFE_SM_FREE(primaryObjs);
	return ifail;
}

// function sets "mo4_bom_aligned" at Rim to true
int activateBomAlignedAttr(tag_t rimObject)
{
	int ifail = ITK_ok;
	logical isModifiable = false;

	ifail = POM_modifiable(rimObject, &isModifiable);
	if (ifail == ITK_ok && !isModifiable)
	{
	     ifail = AOM_refresh(rimObject, POM_modify_lock);
	}
	if (!ifail)
		ifail = AOM_set_value_logical(rimObject, MO4_BOM_ALIGNED_ATTR, TRUE);
	if (!ifail)
		ifail = AOM_save_without_extensions(rimObject);
	if (!isModifiable)
	{
		ifail = AOM_refresh(rimObject,  POM_no_lock);
	}
	return ifail;
}

int MO4_checkin( METHOD_message_t * /*msg*/, va_list args )
{
	int ifail = ITK_ok;
	tag_t rimObject = NULLTAG;
	tag_t matObject = NULLTAG;
	tag_t engPartObject = NULLTAG;
	tag_t type = NULLTAG;
	char* typeName = NULL;
	char* matCode = NULL;
	int id = -1;
	logical bomAligned = false;
	va_list largs;

	 va_copy(largs, args);
	 rimObject = va_arg(largs, tag_t);
	 va_end(largs);

	logDebug("Start: ");

	 /* Check object type */
	 ifail = TCTYPE_ask_object_type (rimObject, &type);
	 ifail = TCTYPE_ask_name2(type, &typeName);
	 if (typeName) TC_write_syslog(typeName);
	 if (tc_strcmp(typeName, MO4_RIM_TYPE_NAME) == 0)
	 {
		 logDebug("Object type found: %s", typeName);
		 ifail = AOM_ask_value_string(rimObject, MO4_MATERIAL_CODE_ATTR, &matCode);
		 ifail = AOM_ask_value_logical(rimObject, MO4_BOM_ALIGNED_ATTR,  &bomAligned);
		 ifail = AOM_ask_value_int(rimObject, MO4_ID_ATTR, &id);
		 //AOM_is_null_empty(object, "mo4_bom_aligned", 0, &isEmpty);

		 /* Create/Update BOM */
		 if (matCode && tc_strlen(matCode) >1)
		 {
			 TC_write_syslog("matCode not empty \n");
			 ifail = findMaterial(matCode, &matObject);
			 if (matObject && bomAligned == false)
			 {

				 ifail = getEngPartRevForBOM(rimObject, &engPartObject);
				 if (engPartObject)
				 {
					 TC_write_syslog("engPartObject found \n");
					 if (id) TC_write_syslog("ID=: %d\n",id);
					 else TC_write_syslog("id not set \n");
					 if (id) TC_write_syslog("ID=: %d\n",id);
					 ifail = addMaterialToBOM(engPartObject, matObject, id);
				 }
				 /* set BOM Aligned attribute */
				if (!ifail)
				{
					ifail = activateBomAlignedAttr(rimObject);
				}
			 }
			 else if (matObject)
			 {
				 TC_write_syslog("bom_aligne == true \n");
				 ifail = getEngPartRevForBOM(rimObject, &engPartObject);
				 if (engPartObject)
					 ifail = updateMaterialInBOM(engPartObject, matObject, id);
			 }
			 else
			 {
				if (matCode)
				{
					 logError("No Material <%s> found.", matCode);
				}
				else
					 logError("Field <mo4_mat_Code> not set.");
				ifail = ITK_invalid_name;
				EMH_store_error_s1(EMH_severity_warning, ifail, "Part not found");


				 TC_write_syslog("No Material found \n");
			 }
		 }
	 }
	 logDebug("End: ");

	 return ifail;
}

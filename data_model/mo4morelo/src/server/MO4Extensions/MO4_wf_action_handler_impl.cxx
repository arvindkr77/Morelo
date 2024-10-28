/*
 * MO4_wf_action_handler_impl.cxx
 *
 *  Created on: 17 May 2024
 *      Author: install
 */

#include "MO4_wf_action_handler_impl.hxx"
#include "MO4_utils.hxx"
#include <tccore/design.h>
#include <tccore/part.h>
#include <tccore/grm.h>


/**
 * This handler is designed to create a Engineering Part for selected Engineering Design Revision
 * and relate with 'Representation For' relation.
 *
 */
int MO4_ah_create_part_for_design
(
	EPM_action_message_t msg
)
{
	const char *pcFunctionName = "MO4_ah_create_part_for_design";

	int				iRetCode = ITK_ok;
	int				i = 0 ;
	int				iAttachmentCount = 0;

	int*			error_codes = NULL;

	char*			pcItemId = NULL;
	char*			pcSearchItemId = NULL;
	char*			pcNewPartItemId = NULL;
	char*			pcNameExt = NULL;
	char*			pcObjectType = NULL ;
	char*			pcItemName = NULL;
	char*           pcName2 = NULL;
	char*           pcName3 = NULL;
	char*           pcRevId = NULL;

	char**			pcErrorMessages  = NULL;
	char**			ppcAttributeNames = NULL;
	char**			ppcAttributeValues  = NULL;

	logical*		part_required_values = NULL;

	tag_t           tRootTask = NULLTAG ;
	tag_t           tNewEngPart = NULLTAG ;
	tag_t           tNewEngPartRev = NULLTAG ;
	tag_t           tEngPart = NULLTAG;
	tag_t           tEngPartRev = NULLTAG;
	tag_t           tRelationType = NULLTAG;
	tag_t           tRelation = NULLTAG;

	tag_t*          ptAttachments = NULL;

	TC_write_syslog("==>%s\n", pcFunctionName);

	// Find all attachments
	iRetCode = EPM_ask_root_task ( msg.task , &tRootTask );

	if (!iRetCode) iRetCode = EPM_ask_attachments ( tRootTask , EPM_target_attachment , &iAttachmentCount , &ptAttachments );

	TC_write_syslog("\niAttachmentCount = %d", iAttachmentCount);

	if(iAttachmentCount > 0)
	{
		if (!iRetCode) iRetCode = DESIGN_ask_part_required(ptAttachments, iAttachmentCount, &part_required_values, &error_codes, &pcErrorMessages);
	}

	ppcAttributeNames = (char **) MEM_alloc( sizeof(char *) * 5 );
	ppcAttributeValues = (char **) MEM_alloc( sizeof(char *) * 5 );

	tc_strdup("item_id", &(ppcAttributeNames[0]));
	tc_strdup("object_name", &(ppcAttributeNames[1]));
	tc_strdup("mo4_name_extension", &(ppcAttributeNames[2]));
	tc_strdup("mo4_object_name", &(ppcAttributeNames[3]));
	tc_strdup("mo4_name3", &(ppcAttributeNames[4]));

	// Get the relation type tag of "TC_Is_Represented_By"
	if (!iRetCode) iRetCode = GRM_find_relation_type("TC_Is_Represented_By", &tRelationType);

	// Loop through all attachments
	for ( i = 0 ; ( i < iAttachmentCount ) && ( iRetCode == ITK_ok ) ; i++ )
	{
		if(part_required_values[i] == true)
		{
			if (!iRetCode) iRetCode = WSOM_ask_object_type2(ptAttachments[i], &pcObjectType );
			if (!iRetCode) iRetCode = AOM_ask_value_string(ptAttachments[i], "item_id", &pcItemId);
			if (!iRetCode) iRetCode = AOM_ask_value_string(ptAttachments[i], "object_name", &pcItemName);
			if (!iRetCode) iRetCode = AOM_ask_value_string(ptAttachments[i], "mo4_name_extension", &pcNameExt);
			if (!iRetCode) iRetCode = AOM_ask_value_string(ptAttachments[i], "mo4_object_name", &pcName2);
			if (!iRetCode) iRetCode = AOM_ask_value_string(ptAttachments[i], "mo4_name3", &pcName3);
			TC_write_syslog("\npcObjectType = %s, pcItemId = %s, part_required_value = %d\n", pcObjectType, pcItemId, part_required_values[i]);

			pcSearchItemId = (char*) MEM_alloc ( (strlen(pcItemId) + 3) * sizeof(char) );
			sprintf(pcSearchItemId, "P_%s",pcItemId);

			if (!iRetCode) iRetCode = ITEM_find_item(pcSearchItemId, &tEngPart);
			if(tEngPart == NULLTAG)
			{
				tc_strdup((const char*) pcSearchItemId, &(ppcAttributeValues[0]));
				tc_strdup((const char*) pcItemName, &(ppcAttributeValues[1]));
				tc_strdup((const char*) pcNameExt, &(ppcAttributeValues[2]));
				tc_strdup((const char*) pcName2, &(ppcAttributeValues[3]));
				tc_strdup((const char*) pcName3, &(ppcAttributeValues[4]));

				TC_write_syslog("\n%s = %s, %s = %s, %s = %s, %s = %s, %s = %s\n", ppcAttributeNames[0], ppcAttributeValues[0], ppcAttributeNames[1], ppcAttributeValues[1],
						ppcAttributeNames[2], ppcAttributeValues[2], ppcAttributeNames[3], ppcAttributeValues[3], ppcAttributeNames[4], ppcAttributeValues[4]);

				// Create Engineering Part and then relate it with Selected Engineering Design Revision
				if (!iRetCode) iRetCode = ITEM_create_item2("MO4_Eng_Part", 5, ppcAttributeNames, ppcAttributeValues, pcRevId, &tNewEngPart, &tNewEngPartRev);
				if (!iRetCode) iRetCode = AOM_save_with_extensions(tNewEngPart);

				if(iRetCode == ITK_ok)
				{
					iRetCode = AOM_ask_value_string(tNewEngPart, "item_id", &pcNewPartItemId);

					TC_write_syslog("\nItemId of New EngPart = %s\n", pcNewPartItemId);

					if (!iRetCode) iRetCode = GRM_create_relation( tNewEngPartRev, ptAttachments[i], tRelationType, NULLTAG, &tRelation );
					if (!iRetCode) iRetCode = GRM_save_relation( tRelation );
					MEM_free(pcNewPartItemId);
				}
			}
			else // If Engineering Part is available then relate it with Selected Engineering Design Revision
			{
				if (!iRetCode) iRetCode = ITEM_ask_latest_rev(tEngPart, &tEngPartRev);
				if (!iRetCode) iRetCode = GRM_find_relation(tEngPartRev, ptAttachments[i], tRelationType, &tRelation);
				if(tRelation == NULLTAG)
				{
					if (!iRetCode) iRetCode = GRM_create_relation( tEngPartRev, ptAttachments[i], tRelationType, NULLTAG, &tRelation );
					if (!iRetCode) iRetCode = GRM_save_relation( tRelation );
				}
			}

			MEM_free(pcObjectType);
			MEM_free(pcItemId);
			MEM_free(pcSearchItemId);
			MEM_free(pcNameExt);
			MEM_free(pcName2);
			MEM_free(pcItemName);
		}
	}

	TC_write_syslog("<==%s\n", pcFunctionName);

	return iRetCode;
}

/**
 * This handler is designed to create a Engineering Design for selected Engineering Part Revision
 * and relate with 'Representation By' relation.
 *
 */
int MO4_ah_create_design_for_part
(
	EPM_action_message_t msg
)
{
	const char *pcFunctionName = "MO4_ah_create_design_for_part";

	int				iRetCode = ITK_ok;
	int				i = 0 ;
	int				iAttachmentCount = 0;

	int*			error_codes = NULL;

	char*			pcItemId = NULL;
	char*			pcSearchItemId = NULL;
	char*			pcNewDesignItemId = NULL;
	char*			pcNameExt = NULL;
	char*           pcName2 = NULL;
	char*           pcName3 = NULL;
	char*			pcObjectType = NULL ;
	char*			pcItemName = NULL;
	char*           pcRevId = NULL;

	char**			pcErrorMessages  = NULL;
	char**			ppcAttributeNames = NULL;
	char**			ppcAttributeValues  = NULL;

	logical*		design_required_values = NULL;

	tag_t           tRootTask = NULLTAG ;
	tag_t           tNewEngDesign = NULLTAG ;
	tag_t           tNewEngDesignRev = NULLTAG ;
	tag_t           tEngDesign = NULLTAG;
	tag_t           tEngDesignRev = NULLTAG;
	tag_t           tRelationType = NULLTAG;
	tag_t           tRelation = NULLTAG;

	tag_t*          ptAttachments = NULL;

	TC_write_syslog("==>%s\n", pcFunctionName);

	// Find all attachments
	iRetCode = EPM_ask_root_task ( msg.task , &tRootTask );

	if (!iRetCode) iRetCode = EPM_ask_attachments ( tRootTask , EPM_target_attachment , &iAttachmentCount , &ptAttachments );

	TC_write_syslog("\niAttachmentCount = %d", iAttachmentCount);

	if(iAttachmentCount > 0)
	{
		if (!iRetCode) iRetCode = PART_ask_design_required(ptAttachments, iAttachmentCount, &design_required_values, &error_codes, &pcErrorMessages);
	}

	ppcAttributeNames = (char **) MEM_alloc( sizeof(char *) * 5 );
	ppcAttributeValues = (char **) MEM_alloc( sizeof(char *) * 5 );

	tc_strdup("item_id", &(ppcAttributeNames[0]));
	tc_strdup("object_name", &(ppcAttributeNames[1]));
	tc_strdup("mo4_name_extension", &(ppcAttributeNames[2]));
	tc_strdup("mo4_object_name", &(ppcAttributeNames[3]));
	tc_strdup("mo4_name3", &(ppcAttributeNames[4]));

	// Get the relation type tag of "TC_Is_Represented_By"
	if (!iRetCode) iRetCode = GRM_find_relation_type("TC_Is_Represented_By", &tRelationType);

	// Loop through all attachments
	for ( i = 0 ; ( i < iAttachmentCount ) && ( iRetCode == ITK_ok ) ; i++ )
	{
		if(design_required_values[i] == true)
		{
			if (!iRetCode) iRetCode = WSOM_ask_object_type2(ptAttachments[i], &pcObjectType );
			if (!iRetCode) iRetCode = AOM_ask_value_string(ptAttachments[i], "item_id", &pcItemId);
			if (!iRetCode) iRetCode = AOM_ask_value_string(ptAttachments[i], "object_name", &pcItemName);
			if (!iRetCode) iRetCode = AOM_ask_value_string(ptAttachments[i], "mo4_name_extension", &pcNameExt);
			if (!iRetCode) iRetCode = AOM_ask_value_string(ptAttachments[i], "mo4_object_name", &pcName2);
			if (!iRetCode) iRetCode = AOM_ask_value_string(ptAttachments[i], "mo4_name3", &pcName3);
			TC_write_syslog("\npcObjectType = %s, pcItemId = %s, design_required_value = %d\n", pcObjectType, pcItemId, design_required_values[i]);

			pcSearchItemId = (char*) MEM_alloc ( (strlen(pcItemId) + 3) * sizeof(char) );
			sprintf(pcSearchItemId, "D_%s",pcItemId);

			if (!iRetCode) iRetCode = ITEM_find_item(pcSearchItemId, &tEngDesign);
			if(tEngDesign == NULLTAG)
			{
				tc_strdup((const char*) pcSearchItemId, &(ppcAttributeValues[0]));
				tc_strdup((const char*) pcItemName, &(ppcAttributeValues[1]));
				tc_strdup((const char*) pcNameExt, &(ppcAttributeValues[2]));
				tc_strdup((const char*) pcName2, &(ppcAttributeValues[3]));
				tc_strdup((const char*) pcName3, &(ppcAttributeValues[4]));

				TC_write_syslog("\n%s = %s, %s = %s, %s = %s, %s = %s, %s = %s\n", ppcAttributeNames[0], ppcAttributeValues[0], ppcAttributeNames[1], ppcAttributeValues[1],
						ppcAttributeNames[2], ppcAttributeValues[2], ppcAttributeNames[3], ppcAttributeValues[3], ppcAttributeNames[4], ppcAttributeValues[4]);

				// Create Engineering Design and then relate it with Selected Engineering Part Revision
				if (!iRetCode) iRetCode = ITEM_create_item2("MO4_Eng_Design", 5, ppcAttributeNames, ppcAttributeValues, pcRevId, &tNewEngDesign, &tNewEngDesignRev);
				if (!iRetCode) iRetCode = AOM_save_with_extensions(tNewEngDesign);

				if(iRetCode == ITK_ok)
				{
					iRetCode = AOM_ask_value_string(tNewEngDesign, "item_id", &pcNewDesignItemId);

					TC_write_syslog("\nItemId of New EngDesign = %s\n", pcNewDesignItemId);

					if (!iRetCode) iRetCode = GRM_create_relation( ptAttachments[i], tNewEngDesignRev, tRelationType, NULLTAG, &tRelation );
					if (!iRetCode) iRetCode = GRM_save_relation( tRelation );
					MEM_free(pcNewDesignItemId);
				}
			}
			else // If Engineering Design is available then relate it with selected Engineering Part Revision
			{
				if (!iRetCode) iRetCode = ITEM_ask_latest_rev(tEngDesign, &tEngDesignRev);
				if (!iRetCode) iRetCode = GRM_find_relation(ptAttachments[i], tEngDesignRev, tRelationType, &tRelation );
				if(tRelation == NULLTAG)
				{
					if (!iRetCode) iRetCode = GRM_create_relation( ptAttachments[i], tEngDesignRev, tRelationType, NULLTAG, &tRelation );
					if (!iRetCode) iRetCode = GRM_save_relation( tRelation );
				}
			}

			MEM_free(pcObjectType);
			MEM_free(pcItemId);
			MEM_free(pcSearchItemId);
			MEM_free(pcNameExt);
			MEM_free(pcItemName);
		}
	}

	TC_write_syslog("<==%s\n", pcFunctionName);

	return iRetCode;
}

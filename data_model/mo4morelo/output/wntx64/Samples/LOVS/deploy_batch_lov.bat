set bmide_project_folder=C:\Siemens\Teamcenter\bmide\workspace\mo4morelo

call bmide_manage_batch_lovs -u=infodba -p=infodba -g=dba -option=update -file=%bmide_project_folder%\output\wntx64\Samples\LOVS\MO4_Name2LOV\MO4_Name2LOV.xml

call bmide_manage_batch_lovs -u=infodba -p=infodba -g=dba -option=update -file=%bmide_project_folder%\output\wntx64\Samples\LOVS\MO4_Name3LOV\MO4_Name3LOV.xml

call bmide_manage_batch_lovs -u=infodba -p=infodba -g=dba -option=update -file=%bmide_project_folder%\output\wntx64\Samples\LOVS\MO4_NamingLOV\MO4_NamingLOV.xml

call bmide_manage_batch_lovs -u=infodba -p=infodba -g=dba -option=update -file=%bmide_project_folder%\output\wntx64\Samples\LOVS\MO4_ColorLOV\MO4_ColorLOV.xml

call bmide_manage_batch_lovs -u=infodba -p=infodba -g=dba -option=update -file=%bmide_project_folder%\output\wntx64\Samples\LOVS\MO4_DrwNameLOV\MO4_DrwNameLOV.xml

call bmide_manage_batch_lovs -u=infodba -p=infodba -g=dba -option=update -file=%bmide_project_folder%\output\wntx64\Samples\LOVS\MO4_DrwTypeLOV\MO4_DrwTypeLOV.xml

call bmide_manage_batch_lovs -u=infodba -p=infodba -g=dba -option=update -file=%bmide_project_folder%\output\wntx64\Samples\LOVS\MO4_GlossLevelLOV\MO4_GlossLevelLOV.xml

call bmide_manage_batch_lovs -u=infodba -p=infodba -g=dba -option=update -file=%bmide_project_folder%\output\wntx64\Samples\LOVS\MO4_PartTypeLOV\MO4_PartTypeLOV.xml

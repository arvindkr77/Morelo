# ==================================================
# Copyright 2015
# Siemens Product Lifecycle Management Software Inc.
# All Rights Reserved.
# ==================================================
# ==================================================
#             Auto-generated source
#                 DO NOT EDIT
# ==================================================



# ==================================================
# Define macros for C++ Server Library
# ==================================================
MO4EXTENSIONS_SRC        = $(BASE_PATH)$(PS)src$(PS)server$(PS)MO4Extensions
MO4EXTENSIONS_DRV        = $(PROJECT_DRV)$(PS)MO4Extensions
MO4EXTENSIONS_OBJ        = $(PROJECT_OBJ)$(PS)libmo4extensions
MO4EXTENSIONS_CXXS       = $(MO4EXTENSIONS_SRC)$(PS)*.c* $(MO4EXTENSIONS_DRV)$(PS)*.cxx
MO4EXTENSIONS_CXX_FLAGS  = $(CXX_FLAGS) -DIPLIB=mo4extensions
MO4EXTENSIONS_LINK_FLAGS = $(LINK_FLAGS)
MO4EXTENSIONS_INCLUDES   = -I$(BASE_PATH)$(PS)src$(PS)server -I$(PROJECT_DRV) \
                                  -I$(TC_ROOT)$(PS)include -I$(TC_ROOT)$(PS)include_cpp -I$(TC_ROOT)$(PS)aw_include_cpp \
                                  
MO4EXTENSIONS_LIB_PATH   = $(LP)$(PROJECT_BIN)  $(LIB_PATH) 
MO4EXTENSIONS_LIBS       = $(LIB_PREFIX)metaframework$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)sa$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)tccore$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)tc$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)ps$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)cfm$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)epm$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)bom$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)cxpom$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)pom$(LIB_SUFFIX)  $(LIB_PREFIX)fclasses$(LIB_SUFFIX)  $(LIB_PREFIX)itk$(LIB_SUFFIX) \
                                  $(LIB_PREFIX)extensionframework$(LIB_SUFFIX) $(LIB_PREFIX)mld$(LIB_SUFFIX)  $(LIB_PREFIX)base_utils$(LIB_SUFFIX) $(SYSLIBS)
                                                                   
MO4EXTENSIONS_LD_FLAGS   = $(MO4EXTENSIONS_LINK_FLAGS) $(MO4EXTENSIONS_LIB_PATH)
MO4EXTENSIONS_TARGET     = $(PROJECT_BIN)$(PS)libMO4Extensions
MO4EXTENSIONS_AUTO_TARG  = $(MO4EXTENSIONS_DRV)$(PS)libmo4extensionsinitializemodule.cxx
MO4EXTENSIONS_AUTO_SRC   = $(PROJECT_HOME)$(PS)extensions$(PS)*.xml

MO4Extensions_all:   $(MO4EXTENSIONS_AUTO_TARG) $(MO4EXTENSIONS_OBJ)  $(MO4EXTENSIONS_TARGET).$(LIB_EXT)

$(MO4EXTENSIONS_AUTO_TARG) : $(MO4EXTENSIONS_AUTO_SRC)
	@echo ________________________________________________________________________________
	@echo Auto Generation of business object classes for MO4Extensions
	@echo $(TC_ROOT)$(PS)bin$(PS)bmide_generatecode$(SCRIPT_EXT) -templateProj=$(PROJECT_HOME) -templateDeps=$(TEMPLATE_HOME) -gensrcDir=$(PROJECT_DRV) -srcDir=$(BASE_PATH)$(PS)src$(PS)server -log=$(BASE_PATH)$(PS)output$(PS)generate.log
	@echo ________________________________________________________________________________

$(MO4EXTENSIONS_OBJ) :
	@echo Creating: $(MO4EXTENSIONS_OBJ)
	-@$(MAKE_DIR) $(MO4EXTENSIONS_OBJ)

$(MO4EXTENSIONS_CXXS) :


# ==================================================
# Windows Build Target
# ==================================================
$(MO4EXTENSIONS_TARGET).dll : $(MO4EXTENSIONS_CXXS)
	@echo ________________________________________________________________________________
	@echo Building C++ Server MO4Extensions
	@echo Compiling Source...
	@$(COMPILE)  $(MO4EXTENSIONS_CXX_FLAGS) $(MO4EXTENSIONS_INCLUDES) -c $(**)   $(CARG_OUT)$(MO4EXTENSIONS_OBJ)$(PS)
	@echo.
	@$(LINK) $(MO4EXTENSIONS_LD_FLAGS) $(MO4EXTENSIONS_OBJ)$(PS)*.$(OBJ_EXT)  $(MO4EXTENSIONS_LIBS)  $(LARG_OUT)$(MO4EXTENSIONS_TARGET).dll
	$(COPY_FILE)  $(PROJECT_BIN)$(PS)libMO4Extensions.dll $(PROJECT_TC)$(PS)bin
	$(COPY_FILE)  $(PROJECT_BIN)$(PS)libMO4Extensions.lib $(PROJECT_TC)$(PS)lib
	@echo ________________________________________________________________________________


	
# ==================================================
# UNIX Build Target
# ==================================================
$(MO4EXTENSIONS_TARGET).so : $(MO4EXTENSIONS_CXXS)
	@echo ________________________________________________________________________________
	@echo Building C++ Server MO4Extensions
	@# The compile output goes to the current directory (project root), so remove any .o files
	@# that may have been left here from previous failed builds. Then after the compile move all .o
	@# files to the the desired obj folder.
	@-rm *.$(OBJ_EXT)
	@echo Compiling $(MO4EXTENSIONS_CXXS)
	@$(COMPILE)  $(MO4EXTENSIONS_CXX_FLAGS) $(MO4EXTENSIONS_INCLUDES) -c $(MO4EXTENSIONS_CXXS) 
	@mv *.$(OBJ_EXT) $(MO4EXTENSIONS_OBJ)
	@echo Linking $(MO4EXTENSIONS_TARGET).so
	@$(LINK) $(MO4EXTENSIONS_LD_FLAGS) $(MO4EXTENSIONS_OBJ)$(PS)*.$(OBJ_EXT)  $(MO4EXTENSIONS_LIBS)  $(LARG_OUT) $(MO4EXTENSIONS_TARGET).so
	$(COPY_FILE)  $(PROJECT_BIN)$(PS)libMO4Extensions.so $(PROJECT_TC)$(PS)lib
	@echo ________________________________________________________________________________


MO4Extensions_clean:
	@echo ________________________________________________________________________________
	@echo Cleaning C++ Server MO4Extensions
	-$(DELETE_DIR)  $(MO4EXTENSIONS_OBJ)
	-$(DELETE_DIR)  $(MO4EXTENSIONS_DRV)
	-$(DELETE_FILE) $(PROJECT_BIN)$(PS)libMO4Extensions.*	
	-$(DELETE_FILE) $(PROJECT_TC)$(PS)bin$(PS)libMO4Extensions.dll
	-$(DELETE_FILE) $(PROJECT_TC)$(PS)lib$(PS)libMO4Extensions.lib	
	-$(DELETE_FILE) $(PROJECT_TC)$(PS)lib$(PS)libMO4Extensions.so	
	@echo ________________________________________________________________________________


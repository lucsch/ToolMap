# SCRIPT FOR BUILDING LSVersion
# INCLUDE THIS FILE FOR INCLUDING A DIALOG CONTAINING
# ALL VERSION INFORMATIONS 

SET(LSVERSION_NAME "lsVersion")

#OPTIONS - PARAMETERS 
SET(USE_VERSION 1 CACHE BOOL "Use component ${LSVERSION_NAME}")
IF(USE_VERSION)
	SET(lsVERSION_ENABLED 1)
	SET(lsVERSION_SOFT_NAME ${CMAKE_PROJECT_NAME})	

	SET(wxWIDGETS_USING_SVN 0 CACHE BOOL "Using latest version of wxWidgets from SVN")
	IF(wxWIDGETS_USING_SVN)
		SET(wxWIDGETS_PATH_SVN 0 CACHE PATH "Path to the SVN version of wxWidgets from")	
	
		# GET WXWIDGETS VERSION
		EXECUTE_PROCESS(COMMAND "svnversion" -n
		WORKING_DIRECTORY ${wxWIDGETS_PATH_SVN}
		TIMEOUT 20
		OUTPUT_VARIABLE lsVERSION_WXWIDGETS_SVN
		ERROR_VARIABLE MY_ERROR2)
		IF(MY_ERROR2)
			MESSAGE(FATAL_ERROR "myError2 : ${MY_ERROR2}")
		ENDIF(MY_ERROR2)
		MESSAGE("wxWidgets version: ${lsVERSION_WXWIDGETS_SVN}")

	ENDIF(wxWIDGETS_USING_SVN)
	
	# IF GEOS WAS SEARCHED
	IF(GEOS_INCLUDE_DIR)
		SET(lsVERSION_HAS_GEOS 1)
	ENDIF(GEOS_INCLUDE_DIR)	

	# IF GDAL WAS SEARCHED
	IF(GDAL_INCLUDE_DIR)
		SET(lsVERSION_HAS_GDAL 1)
	ENDIF(GDAL_INCLUDE_DIR)	

	# IF CURL WAS SEARCHED
	IF(SEARCH_CURL)
	  SET (lsVERSION_HAS_CURL 1)  
	ENDIF(SEARCH_CURL)

	# IF SQLITE WAS SEARCHED
	IF(SQLITE_LIBRARIES)
	  SET (lsVERSION_HAS_SQLITE 1)
	ENDIF(SQLITE_LIBRARIES)
	
	#IF MYSQL WAS SEARCHED
	IF(MYSQL_INCLUDE_DIR)
		SET(lsVERSION_HAS_MYSQL 1)
	ENDIF(MYSQL_INCLUDE_DIR)

	# IF VROOMGIS IS SET
	IF(VROOMGIS_PATH)
	  	# GET VROOMGIS VERSION
		EXECUTE_PROCESS(COMMAND "svnversion" -n
		WORKING_DIRECTORY ${VROOMGIS_PATH}
		TIMEOUT 20
		OUTPUT_VARIABLE lsVERSION_VROOMGIS_SVN
		ERROR_VARIABLE MY_ERROR2)
		IF(MY_ERROR2)
			MESSAGE(FATAL_ERROR "myError2 : ${MY_ERROR2}")
		ENDIF(MY_ERROR2)
		MESSAGE("my vroomgis version ${lsVERSION_VROOMGIS_SVN}")
	ENDIF(VROOMGIS_PATH)

	#FINDING OUR PATH
	FIND_PATH(LSVERSION_PATH "lsversion_param.h.in" 
	"/plugins/lsversion/src/"
	"../plugins/lsversion/src/"
	"../../plugins/lsversion/src/"
	"../../../plugins/lsversion/src/"
	"../../../../plugins/lsversion/src/"
	"/lib/lsversion/src/"
	"../lib/lsversion/src/"
	"../../lib/lsversion/src/"
	"../../../lib/lsversion/src/"
	"../../../../lib/lsversion/src/"
	"/lsversion/src/"
	"../lsversion/src/"
	"../../lsversion/src/"
	"../../../lsversion/src/"
	"../../../../lsversion/src/"
	 NO_DEFAULT_PATH)
	
	# GET PROGRAM SVN VERSION
	EXECUTE_PROCESS(COMMAND "svnversion" -n
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
	TIMEOUT 20
	OUTPUT_VARIABLE lsVERSION_SOFT_VERSION
	ERROR_VARIABLE MY_ERROR1)
	IF(MY_ERROR1)
		MESSAGE(FATAL_ERROR "myError1 : ${MY_ERROR1}")
	ENDIF(MY_ERROR1)
	MESSAGE("${lsVERSION_SOFT_NAME} version: ${lsVERSION_SOFT_VERSION}")



	#
	# CREATE LIBRARY 
	#
	#search all source files
	MESSAGE(${LSVERSION_PATH})
	FILE (GLOB LSVERSION_SRC_FILES "${LSVERSION_PATH}/*.cpp")
	SOURCE_GROUP(source FILES ${LSVERSION_SRC_FILES})

	#COUNTING NUMBER OF SOURCES FOUND
	LIST(LENGTH LSVERSION_SRC_FILES MY_VERSION_LENGTH)
	IF(NOT MY_VERSION_LENGTH)
		MESSAGE(FATAL_ERROR "${MY_VERSION_LENGTH} source found for lsVersion Plugin")
	ENDIF(NOT MY_VERSION_LENGTH)

	# search all headers files
	FILE (GLOB LSVERSION_HEAD_FILES "${LSVERSION_PATH}/*.h")
	SOURCE_GROUP(header FILES ${LSVERSION_HEAD_FILES})
	INCLUDE_DIRECTORIES("${LSVERSION_PATH}")
	ADD_LIBRARY("lsVersion" STATIC ${LSVERSION_SRC_FILES} ${LSVERSION_HEAD_FILES})
	

ELSE(USE_VERSION)
	MESSAGE( "Not using vrVersion Component")
ENDIF(USE_VERSION)


CONFIGURE_FILE("${LSVERSION_PATH}/lsversion_param.h.in" "${PROJECT_BINARY_DIR}/lsversion_param.h")
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})





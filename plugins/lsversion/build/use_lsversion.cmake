# SCRIPT FOR BUILDING LSVersion
# INCLUDE THIS FILE FOR INCLUDING A DIALOG CONTAINING
# ALL VERSION INFORMATIONS 

SET(LSVERSION_NAME "lsVersion")

#OPTIONS - PARAMETERS 
SET(USE_VERSION 1 CACHE BOOL "Use component ${LSVERSION_NAME}")
IF (USE_VERSION)
    SET(lsVERSION_ENABLED 1)
    SET(lsVERSION_SOFT_NAME ${CMAKE_PROJECT_NAME})

    SET(wxWIDGETS_USING_GIT 0 CACHE BOOL "Using latest version of wxWidgets from GIT")
    IF (wxWIDGETS_USING_GIT)
        SET(wxWIDGETS_PATH_GIT 0 CACHE PATH "Path to the GIT version of wxWidgets from")

        # GET WXWIDGETS VERSION
        EXECUTE_PROCESS(COMMAND git rev-list --count head
                WORKING_DIRECTORY ${wxWIDGETS_PATH_GIT}
                TIMEOUT 20
                OUTPUT_VARIABLE lsVERSION_WXWIDGETS_GIT
                ERROR_VARIABLE MY_ERROR2)

        IF (MY_ERROR2)
            MESSAGE(FATAL_ERROR "myError2 : ${MY_ERROR2}")
        ENDIF (MY_ERROR2)
        MESSAGE(STATUS "wxWidgets version: ${lsVERSION_WXWIDGETS_GIT}")

    ENDIF (wxWIDGETS_USING_GIT)

    # IF GEOS WAS SEARCHED
    IF (GEOS_INCLUDE_DIR)
        SET(lsVERSION_HAS_GEOS 1)
    ENDIF (GEOS_INCLUDE_DIR)

    # IF GDAL WAS SEARCHED
    IF (GDAL_INCLUDE_DIR)
        SET(lsVERSION_HAS_GDAL 1)
    ENDIF (GDAL_INCLUDE_DIR)

    # IF CURL WAS SEARCHED
    IF (SEARCH_CURL)
        SET(lsVERSION_HAS_CURL 1)
    ENDIF (SEARCH_CURL)

    # IF SQLITE WAS SEARCHED
    IF (SQLITE_LIBRARIES)
        SET(lsVERSION_HAS_SQLITE 1)
    ENDIF (SQLITE_LIBRARIES)

    #IF MYSQL WAS SEARCHED
    IF (MYSQL_INCLUDE_DIR)
        SET(lsVERSION_HAS_MYSQL 1)
    ENDIF (MYSQL_INCLUDE_DIR)

    # IF VROOMGIS IS SET
    IF (VROOMGIS_PATH)
        # GET VROOMGIS VERSION
        EXECUTE_PROCESS(COMMAND svnversion -n
                WORKING_DIRECTORY ${VROOMGIS_PATH}
                TIMEOUT 20
                OUTPUT_VARIABLE lsVERSION_VROOMGIS_SVN
                ERROR_VARIABLE MY_ERROR2)
        IF (MY_ERROR2)
            MESSAGE(FATAL_ERROR "myError2 : ${MY_ERROR2}")
        ENDIF (MY_ERROR2)
        MESSAGE(STATUS "vroomGis version ${lsVERSION_VROOMGIS_SVN}")
    ENDIF (VROOMGIS_PATH)

    #FINDING OUR PATH
    FIND_PATH(LSVERSION_PATH "lsversion_param.h.in"
            "plugins/lsversion/src/"
            "../plugins/lsversion/src/"
            "../../plugins/lsversion/src/"
            "../../../plugins/lsversion/src/"
            "../../../../plugins/lsversion/src/"
            "lib/lsversion/src/"
            "../lib/lsversion/src/"
            "../../lib/lsversion/src/"
            "../../../lib/lsversion/src/"
            "../../../../lib/lsversion/src/"
            "lsversion/src/"
            "../lsversion/src/"
            "../../lsversion/src/"
            "../../../lsversion/src/"
            "../../../../lsversion/src/"
            NO_DEFAULT_PATH)

    # GET PROGRAM REVISON (NUMBER)
    EXECUTE_PROCESS(COMMAND git rev-list --count HEAD
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            TIMEOUT 20
            OUTPUT_VARIABLE lsVERSION_SOFT_VERSION
            ERROR_VARIABLE MY_ERROR1)
    IF (MY_ERROR1)
        MESSAGE(FATAL_ERROR "myError1 : ${MY_ERROR1}")
    ENDIF (MY_ERROR1)
    STRING(STRIP ${lsVERSION_SOFT_VERSION} lsVERSION_SOFT_VERSION)

    # GET PROGRAM BRANCH
    EXECUTE_PROCESS(COMMAND git rev-parse --abbrev-ref HEAD
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            TIMEOUT 20
            OUTPUT_VARIABLE lsVERSION_SOFT_VERSION_BRANCH
            ERROR_VARIABLE MY_ERROR1)
    IF (MY_ERROR1)
        MESSAGE(FATAL_ERROR "myError1 : ${MY_ERROR1}")
    ENDIF (MY_ERROR1)
    STRING(STRIP ${lsVERSION_SOFT_VERSION_BRANCH} lsVERSION_SOFT_VERSION_BRANCH)

    # GET PROGRAM REVISION (SHORT)
    EXECUTE_PROCESS(COMMAND git rev-parse --short HEAD
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            TIMEOUT 20
            OUTPUT_VARIABLE lsVERSION_SOFT_VERSION_REVISION
            ERROR_VARIABLE MY_ERROR1)
    IF (MY_ERROR1)
        MESSAGE(FATAL_ERROR "myError1 : ${MY_ERROR1}")
    ENDIF (MY_ERROR1)
    STRING(STRIP ${lsVERSION_SOFT_VERSION_REVISION} lsVERSION_SOFT_VERSION_REVISION)

    MESSAGE(STATUS "lsversion path: ${LSVERSION_PATH}")
    MESSAGE(STATUS "${lsVERSION_SOFT_NAME} version: ${VERSION_MAJOR}.${VERSION_MINOR}")
    MESSAGE(STATUS "${lsVERSION_SOFT_NAME} version: ${lsVERSION_SOFT_VERSION}")
    MESSAGE(STATUS "${lsVERSION_SOFT_NAME} branch: ${lsVERSION_SOFT_VERSION_BRANCH}")
    MESSAGE(STATUS "${lsVERSION_SOFT_NAME} revision: ${lsVERSION_SOFT_VERSION_REVISION}")

    #
    # CREATE LIBRARY
    #
    #search all source files
    #MESSAGE(${LSVERSION_PATH})
    FILE(GLOB LSVERSION_SRC_FILES "${LSVERSION_PATH}/*.cpp")
    SOURCE_GROUP(source FILES ${LSVERSION_SRC_FILES})

    #COUNTING NUMBER OF SOURCES FOUND
    LIST(LENGTH LSVERSION_SRC_FILES MY_VERSION_LENGTH)
    IF (NOT MY_VERSION_LENGTH)
        MESSAGE(FATAL_ERROR "${MY_VERSION_LENGTH} source found for lsVersion Plugin")
    ENDIF (NOT MY_VERSION_LENGTH)

    # search all headers files
    FILE(GLOB LSVERSION_HEAD_FILES "${LSVERSION_PATH}/*.h")
    SOURCE_GROUP(header FILES ${LSVERSION_HEAD_FILES})
    INCLUDE_DIRECTORIES("${LSVERSION_PATH}")
    ADD_LIBRARY("lsVersion" STATIC ${LSVERSION_SRC_FILES} ${LSVERSION_HEAD_FILES})


ELSE (USE_VERSION)
    MESSAGE(AUTHOR_WARNING "Not using lsVersion Component")
ENDIF (USE_VERSION)

CONFIGURE_FILE("${LSVERSION_PATH}/lsversion_param.h.in" "${PROJECT_BINARY_DIR}/lsversion_param.h")
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR})





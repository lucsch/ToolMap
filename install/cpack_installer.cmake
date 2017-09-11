# INSTALLER
# use git version for package number
set(VERSION_PATCH ${lsVERSION_SOFT_VERSION})

# install ToolMap (application)
install (TARGETS ${CMAKE_PROJECT_NAME}
        BUNDLE DESTINATION .
        RUNTIME DESTINATION bin)

# install GPL licence file
install ( FILES install/COPYING DESTINATION .)

# search dll to install
if (GDAL_DLL_NAME)
    list(APPEND LIB_TO_INSTALL ${GDAL_DLL_NAME})
endif (GDAL_DLL_NAME)

if (GEOS_DLL_NAME)
    list(APPEND LIB_TO_INSTALL ${GEOS_DLL_NAME})
endif (GEOS_DLL_NAME)

if (GEOS_C_DLL_NAME)
    list(APPEND LIB_TO_INSTALL ${GEOS_C_DLL_NAME})
endif (GEOS_C_DLL_NAME)

if (PROJ_DLL_NAME)
    list(APPEND LIB_TO_INSTALL ${PROJ_DLL_NAME})
endif (PROJ_DLL_NAME)

if (SQLITE_DLL_NAME)
    list(APPEND LIB_TO_INSTALL ${SQLITE_DLL_NAME})
endif (SQLITE_DLL_NAME)

if (MYSQL_DLL_NAME)
    list(APPEND LIB_TO_INSTALL ${MYSQL_DLL_NAME})
endif (MYSQL_DLL_NAME)

if (CURL_DLL_NAME)
    list(APPEND LIB_TO_INSTALL ${CURL_DLL_NAME})
endif (CURL_DLL_NAME)


if (WIN32)
    # install program and dlls
    install (
            PROGRAMS
            ${LIB_TO_INSTALL}
            DESTINATION bin
    )

    # install errmsg.sys
    if(NOT MYSQL_ERRMSG_FILE)
        message(FATAL_ERROR "errmsg.sys not found ! Installer will not work!")
    endif()
    install(FILES
            ${MYSQL_ERRMSG_FILE}
            DESTINATION bin/mysql)

    # install webfiles
    # this part is a partial duplicates of copywebfiles.cmake
    FILE (GLOB WEBFILES "resource/web/*.js" "resource/web/*.html")
    install(FILES
            ${WEBFILES}
            DESTINATION share/toolmap)

    # install Microsoft Visual Studio librairies (MSVCP140.DLL, etc.)
    include(InstallRequiredSystemLibraries)
    INSTALL(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION bin)
endif (WIN32)

 # COMMON PROPERTIES
set(CPACK_PACKAGE_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "ToolMap - open source tool for GIS creation")
set(CPACK_PACKAGE_VENDOR "Terranum")
set(CPACK_STRIP_FILES ON) # tell cpack to strip all debug symbols from all files

# IDENTIFY ARCHITECTURE
set(CPACK_PACKAGE_ARCH "unkown-architecture")
if(${CMAKE_SYSTEM_NAME} MATCHES Windows)
    if(CMAKE_CL_64)
        set(CPACK_PACKAGE_ARCH "win64")
    elseif(MINGW)
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            # 64 bits
            set(CPACK_PACKAGE_ARCH "mingw64")
        elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
            # 32 bits
            set(CPACK_PACKAGE_ARCH "mingw32")
        endif()
    elseif(WIN32)
        set(CPACK_PACKAGE_ARCH "win32")
    endif()
    message(STATUS "${CPACK_PACKAGE_ARCH} architecture")
endif(${CMAKE_SYSTEM_NAME} MATCHES Windows)


if(${CMAKE_SYSTEM_NAME} MATCHES Linux)
    if(${CMAKE_SYSTEM_PROCESSOR} MATCHES i686)
        set(CPACK_PACKAGE_ARCH "linux32")
    elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES x86_64)
        if(${CMAKE_CXX_FLAGS} MATCHES " -m32 ")
            set(CPACK_PACKAGE_ARCH "linux32")
        else()
            set(CPACK_PACKAGE_ARCH "linux64")
        endif(${CMAKE_CXX_FLAGS} MATCHES " -m32 ")
    else()
        set(CPACK_PACKAGE_ARCH "linux")
    endif()
endif(${CMAKE_SYSTEM_NAME} MATCHES Linux)

if(${CMAKE_SYSTEM_NAME} MATCHES Darwin)
    set(CPACK_PACKAGE_ARCH "mac64")
endif(${CMAKE_SYSTEM_NAME} MATCHES Darwin)

# OS SPECIFIC PROPERTIES
if (APPLE)
    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}_${CPACK_PACKAGE_VERSION}")
    set(CPACK_DMG_VOLUME_NAME "${CMAKE_PROJECT_NAME}")
    set(CPACK_DMG_FORMAT "UDBZ")
endif (APPLE)

if (WIN32)
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "ToolMap")
    set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION}-${CPACK_PACKAGE_ARCH}-Setup")
    set(CPACK_PACKAGE_EXECUTABLES "ToolMap;ToolMap")
    set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY "${CMAKE_PROJECT_NAME}-${CPACK_PACKAGE_VERSION}")
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_LIST_DIR}/COPYING")
    set(CPACK_NSIS_DISPLAY_NAME "ToolMap")
    set(CPACK_NSIS_CONTACT "Terranum toolmap@terranum.ch")
    set(CPACK_NSIS_HELP_LINK "www.terranum.ch")
    set(CPACK_NSIS_EXECUTABLES_DIRECTORY "bin")
    set(CPACK_NSIS_URL_INFO_ABOUT "www.terranum.ch")
    set(CPACK_CREATE_DESKTOP_LINKS "ToolMap")
    set(CPACK_NSIS_COMPRESSOR "lzma")
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL "ON")
    set(CPACK_NSIS_MODIFY_PATH "ON")
    # Icon in the add/remove control panel. Must be an .exe file
    set(CPACK_NSIS_INSTALLED_ICON_NAME ToolMap.exe)
    if (CMAKE_CL_64)
        set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
    else()
        set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
    endif()
endif(WIN32)


#
# DEB Files (Linux)
#
# TODO: Replace this with AppImage / Snap / Flatpak
IF (UNIX AND NOT APPLE)
    INSTALL(FILES "art/toolmap.png" DESTINATION "/usr/share/pixmaps")
    INSTALL(FILES "install/linux/toolmap.desktop" DESTINATION "/usr/share/applications")

    SET(CPACK_GENERATOR "DEB")
    #SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Terranum <toolmap@terranum.ch>")
    SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libmysqlclient18 (>= 5.1), mysql-server-core-5.5, libgeos-c1, libcurl3-gnutls, libgdal1-1.7.0, libwrap0")
    # SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "In order to handle geological information in an efficient way and for easily transfer it into geological information system, the research center on alpine environment (CREALP) developed in close collaboration with the Swiss Geological Survey (SGS) the software package ToolMap. ToolMap allows structuring and storing geological data through a robust relational database, vectorising field data and building up multilayer GIS models with high topological integrity. Thus, it permits a much faster production of GIS datasets readily available for a wide variety of applications.")
    SET(CPACK_DEBIAN_PACKAGE_SECTION "science")
    SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
ENDIF(UNIX AND NOT APPLE)

include(CPack)
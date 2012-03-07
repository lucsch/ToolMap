# COPY THE ERRMSG FILE TO THE CORRECT LOCATION
# THIS WILL ONLY WORK IF MYSQL WAS FOUND
# (c) Lucien Schreiber CREALP 2012

SET (MYSQL_ERRSQL_FROM_PATH "")
SET (MYSQL_ERRSQL_TO_PATH "")

IF (MYSQL_INCLUDE_DIR)
	MESSAGE ("MYSQL Include dir defined : ${MYSQL_INCLUDE_DIR}")
	SET (MYSQL_SHARE_DIR "${MYSQL_INCLUDE_DIR}../share/english")
	
	IF(APPLE)
		ADD_CUSTOM_COMMAND(TARGET ${CMAKE_PROJECT_NAME}
			POST_BUILD
			COMMAND mkdir -p
			 ${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${CMAKE_PROJECT_NAME}.app/Contents/mysql
			COMMAND cp ${MYSQL_SHARE_DIR}/errmsg.sys 
			${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${CMAKE_PROJECT_NAME}.app/Contents/mysql/errmsg.sys
			)
	ENDIF(APPLE)
	
	
	IF(WIN32)
		ADD_CUSTOM_COMMAND(TARGET ${CMAKE_PROJECT_NAME}
			POST_BUILD
			COMMAND mkdir -p
			 ${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/mysql
			COMMAND cp ${MYSQL_SHARE_DIR}/errmsg.sys 
			${PROJECT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/mysql/errmsg.sys
		)
	ENDIF(WIN32)
	
	# do nothing for Linux
ENDIF(MYSQL_INCLUDE_DIR)



  
  

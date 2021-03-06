/***************************************************************************
 lsversion_core.cpp
 -------------------
 copyright            : (C) 2010 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "lsversion_core.h"

wxString lsVersion::GetSoftName() {
  wxString myName = wxEmptyString;
#ifdef lsVERSION_SOFT_NAME
  myName = lsVERSION_SOFT_NAME;
#endif
  return myName;
}

wxString lsVersion::GetSoftGIT() {
  wxString myGITVersion = wxEmptyString;
#ifdef lsVERSION_SOFT_VERSION
  myGITVersion = lsVERSION_SOFT_VERSION;
#endif
  return myGITVersion;
}

wxString lsVersion::GetSoftGITBranch() {
  wxString myGITtxt = wxEmptyString;
#ifdef lsVERSION_SOFT_VERSION_BRANCH
  myGITtxt = lsVERSION_SOFT_VERSION_BRANCH;
#endif
  return myGITtxt;
}

wxString lsVersion::GetSoftGITRevision() {
  wxString myGITtxt = wxEmptyString;
#ifdef lsVERSION_SOFT_VERSION_REVISION
  myGITtxt = lsVERSION_SOFT_VERSION_REVISION;
#endif
  return myGITtxt;
}

wxString lsVersion::GetwxWidgetsNumber() {
  wxString mywxVersion = wxString::Format("%d.%d.%d", wxMAJOR_VERSION, wxMINOR_VERSION, wxRELEASE_NUMBER);
  if (wxSUBRELEASE_NUMBER != 0) {
    mywxVersion.Append(wxString::Format(".%d", wxSUBRELEASE_NUMBER));
  }

  return mywxVersion;
}

wxString lsVersion::GetwxWidgetsGIT() {
  wxString myGITVersion = wxEmptyString;
#ifdef lsVERSION_WXWIDGETS_GIT
  myGITVersion = lsVERSION_WXWIDGETS_GIT;
#endif
  return myGITVersion;
}

wxString lsVersion::GetGDALNumber() {
  wxString myGDAL = wxEmptyString;
#ifdef lsVERSION_HAS_GDAL
  myGDAL = GDAL_RELEASE_NAME;
#endif
  return myGDAL;
}

wxString lsVersion::GetGEOSNumber() {
  wxString myGEOS = wxEmptyString;
#ifdef lsVERSION_HAS_GEOS
  myGEOS = GEOS_VERSION;
#endif
  return myGEOS;
}

wxString lsVersion::GetCurlNumber() {
  wxString myCurl = wxEmptyString;
#ifdef lsVERSION_HAS_CURL
  myCurl = wxString(LIBCURL_VERSION);
#endif
  return myCurl;
}

wxString lsVersion::GetSQLiteNumber() {
  wxString mySQlite = wxEmptyString;
#ifdef lsVERSION_HAS_SQLITE
  mySQlite = wxString(sqlite3_libversion());
#endif
  return mySQlite;
}

wxString lsVersion::GetMySQLNumber() {
  wxString myMySQL = wxEmptyString;
#ifdef lsVERSION_HAS_MYSQL
  myMySQL = wxString(mysql_get_client_info(), wxConvUTF8);
#endif
  return myMySQL;
}

wxString lsVersion::GetVroomGISNumber() {
  wxString myVroomGIS = wxEmptyString;
#ifdef lsVERSION_VROOMGIS_SVN
  myVroomGIS = lsVERSION_VROOMGIS_SVN;
#endif
  return myVroomGIS;
}

wxString lsVersion::GetAllModules() {
  wxString myModules = _T("wxWidgets: ") + GetwxWidgetsNumber();

  if (!GetwxWidgetsGIT().IsEmpty()) {
    myModules.Append(wxString::Format(" (%s)", GetwxWidgetsGIT()));
  }
  myModules.Append(_T("\n"));

  if (!GetVroomGISNumber().IsEmpty()) {
    myModules.Append(_T("vroomGIS: ") + GetVroomGISNumber() + _T("\n"));
  }

  if (!GetMySQLNumber().IsEmpty()) {
    myModules.Append(_T("MySQL: ") + GetMySQLNumber() + _T("\n"));
  }

  if (!GetSQLiteNumber().IsEmpty()) {
    myModules.Append(_T("SQLite: ") + GetSQLiteNumber() + _T("\n"));
  }

  if (!GetGDALNumber().IsEmpty()) {
    myModules.Append(_T("GDAL: ") + GetGDALNumber() + _T("\n"));
  }

  if (!GetGEOSNumber().IsEmpty()) {
    myModules.Append(_T("GEOS: ") + GetGEOSNumber() + _T("\n"));
  }

  if (!GetCurlNumber().IsEmpty()) {
    myModules.Append(_T("libCURL: ") + GetCurlNumber() + _T("\n"));
  }

  if (!GetSoftGITRevision().IsEmpty()) {
    myModules.Append(wxString::Format(_T("%s revision: %s \n"), GetSoftName(), GetSoftGITRevision()));
  }

  if (!GetSoftGITBranch().IsEmpty()) {
    myModules.Append(wxString::Format(_T("%s branch: %s \n"), GetSoftName(), GetSoftGITBranch()));
  }

  myModules.Append(wxGetOsDescription());
  return myModules;
}

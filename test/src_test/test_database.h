/***************************************************************************
 test_database.h
 Testing Database class
 -------------------
 copyright : (C) 2009 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _TEST_VR_DRIVER_H_
#define _TEST_VR_DRIVER_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <cxxtest/TestSuite.h>
#include <wx/filename.h>

#include "../../src/database/database.h"
#include "test_param.h"

class TEST_Database : public CxxTest::TestSuite {
 public:
  DataBase* m_DB;

  void setUp() {
    wxApp::SetInstance(new wxAppConsole());
    m_DB = new DataBase();
  }

  void tearDown() {
    delete m_DB;
  }

  void testName() {
    wxLogMessage(_T("------------------------------------"));
    wxLogMessage(_T("-------- TESTING DATABASE ----------"));
    wxLogMessage(_T("------------------------------------"));
  }

  void testOpenDatabase() {
    TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_CombioulaCorrect));
    wxString myFalsePath = g_TestPathPRJ + _T("JJJ");
    TS_ASSERT(!m_DB->DataBaseOpen(myFalsePath, g_TestPrj_CombioulaExercice));
    TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_CombioulaExercice));
  }

  void testResults() {
    TS_ASSERT_EQUALS(m_DB->DataBaseHasResults(), false);
    TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_CombioulaCorrect));
    TS_ASSERT_EQUALS(m_DB->DataBaseQuery(_T("SELECT OBJECT_ID FROM generic_lines")), true);
    TS_ASSERT_EQUALS(m_DB->DataBaseHasResults(), true);
    m_DB->DataBaseClearResults();
    TS_ASSERT(m_DB->DataBaseQuery(_T("SELECT OBJECT_ID FROM generic_lines WHERE OBJECT_ID = 1000000")));
    TS_ASSERT_EQUALS(m_DB->DataBaseHasResults(), false);
  }

  void testQueryNoResults() {
    TS_ASSERT_EQUALS(m_DB->DataBaseQueryNoResults(_T("SELECT OBJECT_ID FROM generic_lines")), false);
    TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_CombioulaCorrect));

    TS_ASSERT(m_DB->DataBaseQueryNoResults(_T("SELECT OBJECT_ID FROM generic_lines")));
    TS_ASSERT_EQUALS(m_DB->DataBaseQueryNoResults(_T("SELECT COUNT(*) FROM generic_linesss")), false);
  }

  void testResultString() {
    TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_CombioulaCorrect));
    wxString myReturnedString = wxEmptyString;
    TS_ASSERT(m_DB->DataBaseQuery(_T("SELECT OBJECT_DESC FROM dmn_layer_object WHERE OBJECT_ID = 16")));
    // second query should fail, must delete results first.
    m_DB->DataBaseClearResults();
    TS_ASSERT(m_DB->DataBaseQuery(_T("SELECT OBJECT_DESC FROM dmn_layer_object WHERE OBJECT_ID = 16")));
    TS_ASSERT(m_DB->DataBaseGetNextResult(myReturnedString));
    TS_ASSERT_EQUALS(myReturnedString, _T("delimitation tassement"));  // oid = 17
    TS_ASSERT(!m_DB->DataBaseGetNextResult(myReturnedString));
    TS_ASSERT(myReturnedString == wxEmptyString);
    m_DB->DataBaseClearResults();

    TS_ASSERT(m_DB->DataBaseQuery(_T("SELECT OBJECT_ID FROM dmn_layer_object WHERE OBJECT_ID = 17777")));
    TS_ASSERT(!m_DB->DataBaseGetNextResult(myReturnedString));
    TS_ASSERT(myReturnedString == wxEmptyString);
  }

  void testResultArrayString() {
    TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_CombioulaCorrect));
    TS_ASSERT(m_DB->DataBaseQuery(_T("SELECT * FROM dmn_layer_object WHERE OBJECT_ID = 16")));
    wxArrayString myResults;

    TS_ASSERT(m_DB->DataBaseGetNextResult(myResults));
    TS_ASSERT_EQUALS(myResults.GetCount(), 9);  // 9 cols in dmn_layer_object
    TS_ASSERT_EQUALS(myResults.Item(4), _T("delimitation tassement"));
    m_DB->DataBaseClearResults();
    // limit tests

    TS_ASSERT(m_DB->DataBaseQuery(_T("SELECT OBJECT_ID FROM dmn_layer_object WHERE OBJECT_ID = 17")));
    TS_ASSERT(m_DB->DataBaseGetNextResult(myResults));
    TS_ASSERT_EQUALS(myResults.GetCount(), 1);
    m_DB->DataBaseClearResults();

    TS_ASSERT(!m_DB->DataBaseQuery(_T("SELECT OBJECT_ID FROM dmn_layer_object WHERE OBJCT_ID = 17777")));
    TS_ASSERT(!m_DB->DataBaseGetNextResult(myResults));
    TS_ASSERT_EQUALS(myResults.GetCount(), 0);
  }

  void testCountResults() {
    TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_CombioulaCorrect));
    unsigned int myCols = 0;
    long myRows = 0;
    TS_ASSERT(!m_DB->DataBaseGetResultSize(&myCols, &myRows));
    TS_ASSERT(m_DB->DataBaseQuery(_T("SELECT * FROM dmn_layer_object WHERE OBJECT_ID = 17")));
    TS_ASSERT(m_DB->DataBaseGetResultSize(&myCols, nullptr));
    TS_ASSERT_EQUALS(myCols, 9);
    TS_ASSERT(m_DB->DataBaseGetResultSize(nullptr, &myRows));
    TS_ASSERT_EQUALS(myRows, 1);
    myRows = 0;
    myCols = 0;
    TS_ASSERT(m_DB->DataBaseGetResultSize(&myCols, &myRows));
    TS_ASSERT_EQUALS(myRows, 1);
    TS_ASSERT_EQUALS(myCols, 9);
  }

  void testResultLong() {
    TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_CombioulaCorrect));
    long myResult = 0;
    TS_ASSERT(m_DB->DataBaseQuery(_T("SELECT OBJECT_ID FROM dmn_layer_object WHERE OBJECT_ID = 17")));
    TS_ASSERT(m_DB->DataBaseGetNextResult(myResult));
    TS_ASSERT_EQUALS(myResult, 17);
  }

  void testResultArrayLong() {
    TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_CombioulaCorrect));
    TS_ASSERT(m_DB->DataBaseQuery(_T("SELECT OBJECT_ID, OBJECT_CD FROM dmn_layer_object WHERE OBJECT_ID <= 17")));

    unsigned int myCols = 0;
    long myRows = 0;

    TS_ASSERT(m_DB->DataBaseGetResultSize(&myCols, &myRows));
    TS_ASSERT_EQUALS(myCols, 2);
    TS_ASSERT_EQUALS(myRows, 17);

    wxArrayLong myResults;
    int i = 0;
    bool bReturn = false;
    while (1) {
      bReturn = m_DB->DataBaseGetNextResult(myResults);
      if (i < 17) {
        TS_ASSERT_EQUALS(bReturn, true);
      } else
        TS_ASSERT_EQUALS(bReturn, false);
      i++;
      if (!bReturn) break;
    }

    m_DB->DataBaseClearResults();
    TS_ASSERT(
        m_DB->DataBaseQuery(_T("SELECT OBJECT_ID FROM dmn_layer_object WHERE OBJECT_ID <= 17 ORDER BY OBJECT_ID")));
    TS_ASSERT(m_DB->DataBaseGetNextResult(myResults));
    TS_ASSERT(m_DB->DataBaseGetResultSize(&myCols, &myRows));
    TS_ASSERT_EQUALS(myCols, 1);
    TS_ASSERT_EQUALS(myRows, 17);
  }

  void testResultDouble() {
    TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Fields));
    TS_ASSERT(m_DB->DataBaseQuery(_T("SELECT TestFloat32 FROM layer_at1 WHERE OBJECT_ID = 1")));
    wxDouble myValue = 0;
    TS_ASSERT(m_DB->DataBaseGetNextResult(myValue));
    TS_ASSERT_EQUALS(myValue, 8.99);
  }

  void testResultArrayDouble() {
    TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Fields));
    TS_ASSERT(m_DB->DataBaseQuery(_T("SELECT TestFloat32 FROM layer_at1 ORDER BY OBJECT_ID")));
    wxArrayDouble values;
    TS_ASSERT(m_DB->DataBaseGetNextResult(values));
    TS_ASSERT_EQUALS(values.Item(0), 8.99);
  }

  void testColResultsString() {
    TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Fields));
    TS_ASSERT(m_DB->DataBaseQuery(_T("SELECT TestText FROM layer_at1 ORDER BY OBJECT_ID")));
    wxArrayString myResults;
    TS_ASSERT(m_DB->DataBaseGetResults(myResults));
    TS_ASSERT_EQUALS(myResults.GetCount(), 2);
    TS_ASSERT_EQUALS(myResults.Item(1), _T("Ceci est un test pour un max de caracteres ke lonp"));
    TS_ASSERT(!m_DB->DataBaseGetResults(myResults));
    TS_ASSERT_EQUALS(myResults.GetCount(), 0);
    TS_ASSERT(m_DB->DataBaseQuery(_T("SELECT TestText FROM layer_at1 ORDER BY OBJECT_ID")));
    m_DB->DataBaseClearResults();
  }

  void testColResultsLong() {
    TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Fields));
    TS_ASSERT(m_DB->DataBaseQuery(_T("SELECT OBJECT_ID FROM layer_at1 ORDER BY OBJECT_ID")));
    wxArrayLong myResults;
    TS_ASSERT(m_DB->DataBaseGetResults(myResults));
    TS_ASSERT_EQUALS(myResults.GetCount(), 2);
    TS_ASSERT_EQUALS(myResults.Item(1), 4);
  }

  void testColResultsDouble() {
    TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Fields));
    TS_ASSERT(m_DB->DataBaseQuery(_T("SELECT TestFloat32 FROM layer_at1 ORDER BY OBJECT_ID")));
    wxArrayDouble myResults;
    TS_ASSERT(m_DB->DataBaseGetResults(myResults));
    TS_ASSERT_EQUALS(myResults.GetCount(), 2);
    TS_ASSERT_EQUALS(myResults.Item(1), 6.00);
  }

  void testPathName() {
    TS_ASSERT(m_DB->DataBaseGetName() == wxEmptyString);
    TS_ASSERT(m_DB->DataBaseGetPath() == wxEmptyString);
    TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Fields));

    TS_ASSERT(m_DB->DataBaseGetName() == g_TestPrj_Fields);
    TS_ASSERT(m_DB->DataBaseGetPath() == g_TestPathPRJ);

    wxString myFalseName = g_TestPrj_Fields + _T("ssss");
    TS_ASSERT(!m_DB->DataBaseOpen(g_TestPathPRJ, myFalseName));
    TS_ASSERT(m_DB->DataBaseGetName() == wxEmptyString);
    TS_ASSERT(m_DB->DataBaseGetPath() == wxEmptyString);
  }

  void testQueriesNumber() {
    wxString myQueries = _T("SELECT * FROM COUCOU; INSERT INTO ..; SELECT ALL");
    TS_ASSERT_EQUALS(m_DB->DataBaseQueriesNumber(myQueries), 3);
    wxString myQueries2 = _T("SELECT * FROM COUCOU; INSERT INTO; SELECT ALL;");
    TS_ASSERT_EQUALS(m_DB->DataBaseQueriesNumber(myQueries), 3);
  }

  void testVersion() {
    TS_ASSERT(DataBase::DataBaseGetVersion() == _T("5.6.36"));
  }

  void testCreateNewDatabase() {
    TS_ASSERT(!m_DB->DataBaseCreateNew(g_TestPathPRJ, g_TestPrj_MyTest));
    TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_MyTest));
    TS_ASSERT(m_DB->DataBaseQuery(_T("SHOW TABLES FROM mytest1")));
  }

  void testGetDataBaseSize() {
    wxString myFailMsg = _("Not available");
    wxString myDBSize = m_DB->DataBaseGetSize(2, myFailMsg);
    TS_ASSERT(myDBSize == myFailMsg);
    TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_MyTest));
    myDBSize = m_DB->DataBaseGetSize(2, myFailMsg);
    wxLogMessage(myDBSize);
    TS_ASSERT(myDBSize != myFailMsg);

    TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_CombioulaCorrect));
    myDBSize = m_DB->DataBaseGetSize(2, myFailMsg);
    wxLogDebug(myDBSize);
    TS_ASSERT(myDBSize != myFailMsg);
  }

  void testGetLastInsertID() {
    TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Fields));
    long myIID = m_DB->DataBaseGetLastInsertedID();
    TS_ASSERT_EQUALS(myIID, wxNOT_FOUND);
    TS_ASSERT(m_DB->DataBaseQueryNoResults(_T("INSERT INTO dmn_layer_object (OBJECT_CD) VALUES (1)")));

    myIID = m_DB->DataBaseGetLastInsertedID();
    TS_ASSERT(myIID != wxNOT_FOUND);
    wxLogDebug(_T("Last inserted ID = %ld"), myIID);

    myIID = m_DB->DataBaseGetLastInsertedID();
    TS_ASSERT(myIID != wxNOT_FOUND);
  }

  void testGetRawRow() {
    TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Fields));
    TS_ASSERT(m_DB->DataBaseQuery(
        _T("SELECT Envelope(OBJECT_GEOMETRY) FROM generic_lines WHERE OBJECT_ID = 1")));  // WHERE OBJECT_ID = 2")));
    MYSQL_ROW myRow;
    tmArrayULong myLength;
    TS_ASSERT(m_DB->DataBaseGetNextRowResult(myRow, myLength));
    TS_ASSERT(myRow != nullptr);
    TS_ASSERT(myLength.GetCount() != 0);
    TS_ASSERT(!m_DB->DataBaseGetNextRowResult(myRow, myLength));
    TS_ASSERT(myRow == nullptr);
    TS_ASSERT_EQUALS(myLength.GetCount(), 0);
  }

  void testDeleteDB() {
    // ensure database testedit_12 didn't exists
    wxLogMessage(_T("Removing : " + g_TestPathPRJ + _T("testedit_12")));
    if (wxFileName::Rmdir(g_TestPathPRJ + _T("testedit_12"), wxPATH_RMDIR_RECURSIVE)) {
      wxLogMessage(_T("Removed testedit_12 allready existing"));
    }

    TS_ASSERT(!m_DB->DataBaseDelete());
    TS_ASSERT(m_DB->DataBaseCreateNew(g_TestPathPRJ, _T("testedit_12")));
    TS_ASSERT(m_DB->DataBaseDelete());
  }

  void testEscapeString() {
    wxLogDebug(_T("Escaping special character"));
    wxString myBefore = _T("SELECT coucou'toi");
    wxString myAfter = wxEmptyString;

    TS_ASSERT(m_DB->DataBaseOpen(g_TestPathPRJ, g_TestPrj_Fields));
    TS_ASSERT(m_DB->DataBaseStringEscapeQuery(myBefore, myAfter));
    wxLogDebug(_T("before : %s - after : %s"), myBefore.c_str(), myAfter.c_str());
    TS_ASSERT_EQUALS(myAfter, _T("SELECT coucou\\'toi"));
  }
};

#endif

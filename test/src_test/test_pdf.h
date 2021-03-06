/***************************************************************************
 test_pdf.h
 UNIT TESTING for pdf exporting
 -------------------
 copyright : (C) 2011 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _TM_TEST_PDF_H_
#define _TM_TEST_PDF_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>

#include "../../src/core/pdfdocument.h"
#include "../../src/database/database_tm.h"
#include "test_param.h"

class TEST_PDF : public CxxTest::TestSuite {
 public:
  DataBaseTM *m_pDB;
  PrjDefMemManage *m_PrjDef;

  TEST_PDF(bool bTest) {
    wxApp::SetInstance(new wxAppConsole());
    m_pDB = new DataBaseTM();
    TS_ASSERT(m_pDB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_221));
    // load project Data
    m_PrjDef = m_pDB->GetProjectDataFromDB();
    TS_ASSERT(m_PrjDef != NULL);
    TS_ASSERT_DIFFERS(m_PrjDef->GetCountLayers(), 0);
  }

  virtual ~TEST_PDF() {
    wxDELETE(m_pDB);
    wxDELETE(m_PrjDef);
  }

  static TEST_PDF *createSuite() {
    return new TEST_PDF(true);
  }
  static void destroySuite(TEST_PDF *suite) {
    delete suite;
  }

  void setUp() {
    // This is needed for the
    // SetFont() function. Otherwise the SetFont function crash...
    wxInitialize();
  }

  void tearDown() {}

  void testName() {
    wxLogMessage(_T("------------------------------------"));
    wxLogMessage(_T("------- TESTING PDF EXPORT   ------"));
    wxLogMessage(_T("------------------------------------"));
  }

  void testGenerate1() {
    PdfDocument myPdf(m_PrjDef);
    myPdf.Generate(wxFileName(g_TestPathEXPORT, "export-A4-portrait.pdf"));
  }

  void testGenerate2() {
    PdfDocument myPdf(m_PrjDef);
    myPdf.SetPaperOrientation(wxLANDSCAPE);
    myPdf.SetTwoColsLayout(true);
    myPdf.Generate(wxFileName(g_TestPathEXPORT, "export-A4-landscape.pdf"));
  }

  void testGenerate3() {
    PdfDocument myPdf(m_PrjDef);
    myPdf.SetPaperOrientation(wxPORTRAIT);
    myPdf.SetTwoColsLayout(true);
    myPdf.SetPaperFormat(wxPAPER_A3);
    myPdf.SetPageBreak(false);
    myPdf.Generate(wxFileName(g_TestPathEXPORT, "export-A3-portraittwo.pdf"));
  }

  void testGenerate4() {
    PdfDocument myPdf(m_PrjDef);
    myPdf.SetTwoColsLayout(true);
    myPdf.SetOnePage(true);
    myPdf.SetPaperSize(296, 717);
    myPdf.Generate(wxFileName(g_TestPathEXPORT, "export-onepage_two.pdf"));
  }

  void testGenerate5() {
    PdfDocument myPdf(m_PrjDef);
    myPdf.SetTwoColsLayout(true);
    myPdf.SetPaperFormat(wxPAPER_A3);
    myPdf.SetPageBreak(false);
    myPdf.SetDecorate(false);
    myPdf.Generate(wxFileName(g_TestPathEXPORT, "export-A3-portraittwo_nodecoration.pdf"));
  }

  void testGenerate6() {
    PdfDocument myPdf(m_PrjDef);
    myPdf.SetTwoColsLayout(true);
    myPdf.SetPaperFormat(wxPAPER_A4);
    myPdf.SetPageBreak(false);
    myPdf.Generate(wxFileName(g_TestPathEXPORT, "export-A4-portraittwo_nobreak.pdf"));
  }

  void testGenerate7() {
    PdfDocument myPdf(m_PrjDef);
    myPdf.SetPageBreak(false);
    myPdf.SetOnePage(true);
    myPdf.SetPaperSize(148, 881);
    myPdf.Generate(wxFileName(g_TestPathEXPORT, "export-onepage.pdf"));
  }
};

#endif

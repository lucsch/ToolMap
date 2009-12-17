/***************************************************************************
							test_tmpointorientattrib.h
					TESTING point orientation & attribution class
                             -------------------
    copyright            : (C) 2009 CREALP Lucien Schreiber 
    email                : lucien.schreiber at crealp dot vs dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _TM_POINT_ORIENT_ATTRIB_TEST_H_
#define _TM_POINT_ORIENT_ATTRIB_TEST_H_



#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <cxxtest/TestSuite.h>
#include "test_param.h"
#include "../../src/gis/tmpointorientattrib.h"

class TEST_tmPointOrientAttrib : public CxxTest::TestSuite
{
public:
	wxPoint myPt1, myPt90, myPt180, myPt270, myPt45, myPt235, myPtd0;
	DataBaseTM * m_DB;
	
	void setUp()
	{
		myPt1 = wxPoint(20,20);
		myPt90 = wxPoint(30,20);
		myPt180 = wxPoint(20,30);
		myPt270 = wxPoint (10,20);
		myPt45 = wxPoint(40,0);
		myPt235 = wxPoint(0,40);
		myPtd0 = wxPoint(20,0);
		
		m_DB = new DataBaseTM();
		m_DB->OpenTMDatabase(g_TestPathPRJ + g_TestPrj_Export); 

	}
	
	void tearDown() 
	{
		delete m_DB;
	}

		
	void testName(){
		wxLogMessage(_T("------------------------------------"));
		wxLogMessage(_T("--- TESTING TMPOINTORIENT ATTRIB ---"));
		wxLogMessage(_T("------------------------------------"));
	}
	
	void testValid()
	{
		tmPointOrientAttrib myPtA;
		TS_ASSERT(myPtA.IsValid()==false);
		myPtA.SetStartPoint(myPt1);
		TS_ASSERT(myPtA.IsValid()==false);
		myPtA.SetEndPoint(myPt90);
		TS_ASSERT(myPtA.IsValid()==true);
		myPtA.Create(m_DB, 2);
		TS_ASSERT(myPtA.IsCorrectType()==false);
		myPtA.Create(m_DB, 5);
		TS_ASSERT(myPtA.IsCorrectType()==true);
		
		// no field pt
		myPtA.Create(m_DB, 6);
		TS_ASSERT(myPtA.IsCorrectType()==false);
		
		// no orient field pt
		myPtA.Create(m_DB, 7);
		TS_ASSERT(myPtA.IsCorrectType()==false);
		
	}
	
	void testUpdateOrient()
	{
		tmPointOrientAttrib myPtA;
		myPtA.Create(m_DB, 5);
		myPtA.SetStartPoint(myPt1);
		myPtA.SetEndPoint(myPt45);
		TS_ASSERT(myPtA.Update()==false);
		TS_ASSERT(myPtA.IsCorrectType()==true);
		TS_ASSERT(myPtA.IsValid()==true);
		
		TS_ASSERT(myPtA.Update()==true);
	}
	
	void testUpdateOrient2()
	{
		tmPointOrientAttrib myPtA;
		myPtA.Create(m_DB, 1);
		myPtA.SetStartPoint(myPt1);
		myPtA.SetEndPoint(myPt235);
		TS_ASSERT(myPtA.IsCorrectType()==true);
		TS_ASSERT(myPtA.Update()==true);
	}
	
	
	
		
};




#endif

/***************************************************************************
								tmgisimport_test.cpp
                    UNIT TESTING for importing gis data
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


#ifndef _TM_GIS_IMPORT_TEST_CPP_
#define _TM_GIS_IMPORT_TEST_CPP_

#include "../../src/database/database.h"
#include "../../src/gis/tmgisimport.h"

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
//#include <iostream>

class tmGISImportTEST : public CppUnit::TestFixture 
{ 
	CPPUNIT_TEST_SUITE( tmGISImportTEST );
	CPPUNIT_TEST( TESTOpenDatabaseImport );
	CPPUNIT_TEST( TESTOpen );
	CPPUNIT_TEST (TESTFileSpatialType );
	CPPUNIT_TEST_SUITE_END();
	
private:
	DataBase * m_DB;
	tmGISImport * m_Import;
	
public: 
	void setUp()
	{
		tmGISData::InitGISDrivers(true,true);
		m_DB = new DataBase();
		CPPUNIT_ASSERT(m_DB->DataBaseOpen(_T("/Users/Lucien/Downloads/"),
										  _T("testfields")) == true);
		m_Import = new tmGISImport;
		
	}
	
	void tearDown() 
	{
		delete m_DB;
		delete m_Import;
	}
	
	void TESTOpenDatabaseImport()
	{
		CPPUNIT_ASSERT(m_DB->DataBaseOpen(_T("/Users/Lucien/Downloads/"),
										  _T("testfields")) == true);
	}
	
	void TESTOpen()
	{
		CPPUNIT_ASSERT (m_Import->IsOpen() == false);
		CPPUNIT_ASSERT (m_Import->Open(_T("/Users/Lucien/DATA/SIG/TOOLMAP/VECT/arcgis_line_test.shp"))==true);
		CPPUNIT_ASSERT (m_Import->IsOpen() == true);
		CPPUNIT_ASSERT (m_Import->Open(_T("/Users/Lucien/DATA/SIG/DATA/CH/cantonsuissesss.shp"))==false);
		CPPUNIT_ASSERT (m_Import->Open(_T("/Users/Lucien/DATA/SIG/SIRS_ERROR/index.html"))==false);
		CPPUNIT_ASSERT (m_Import->IsOpen() == false);

		
	}
	
	
	
	void TESTFileSpatialType ()
	{

		CPPUNIT_ASSERT (m_Import->Open(_T("/Users/Lucien/DATA/SIG/DATA/CH/cantonsuisse.shp"))==true);
		CPPUNIT_ASSERT (m_Import->GetSpatialType () == LAYER_SPATIAL_POLYGON);
		
	}
	
	
};
	


#endif
/***************************************************************************
								tmgisdatavectormysql.cpp
                    For dealing with spatial data from a mysql database
                             -------------------
    copyright            : (C) 2007 CREALP Lucien Schreiber 
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

// comment doxygen

#include "tmgisdatavectormysql.h"


// init static member
DataBaseTM * tmGISDataVectorMYSQL::m_DB = NULL;


tmGISDataVectorMYSQL::tmGISDataVectorMYSQL()
{

}




tmGISDataVectorMYSQL::~tmGISDataVectorMYSQL()
{

	
}



bool tmGISDataVectorMYSQL:: CheckGeometryFields(const wxString & tablename)
{
	wxASSERT(m_DB);
	wxString sSentence = _T("");
	sSentence = wxString::Format(_T("SELECT COLUMN_NAME from INFORMATION_SCHEMA.COLUMNS WHERE ")
								 _T("table_schema=\"%s\" AND table_name='%s' ")
								 _T("AND COLUMN_NAME IN ('%s','%s');"), m_DB->DataBaseGetName().c_str(),
								 tablename.c_str(), tmGISMYSQL_FIELD1.c_str(), tmGISMYSQL_FIELD2.c_str());
	
	if (!m_DB->DataBaseQuery(sSentence))
	{
		wxLogDebug(_T("Error checking geometry fields : %s"), sSentence.c_str());
		return FALSE;
	}
	
	if (m_DB->DatabaseGetCountResults() != 2)
	{
		wxLogDebug(_T("The table : %s dosen't contain needed geometry fields"), tablename.c_str());
		return FALSE;
	}
	
	return TRUE;
}




bool tmGISDataVectorMYSQL::Open (const wxString & filename, bool bReadWrite)
{
	tmGISData::Open(filename, bReadWrite);
	
	// ensure that we have set a valid pointer to the database
	wxASSERT(m_DB);
	if (!m_DB)
	{
		wxLogDebug(_T("Pointer to database invalid, open database first"));
		return FALSE;
	}
	
	// does the table exists ?
	if(!m_DB->DataBaseTableExist(filename))
	{
		wxLogDebug(_T("Table '%s' dosen't exists in the database"), filename.c_str());
		return FALSE;
	}
	
	// does the fields exists ?
	if (!CheckGeometryFields(filename))
		return FALSE;

	return TRUE;
}




tmRealRect tmGISDataVectorMYSQL::GetMinimalBoundingRectangle()
{
	OGREnvelope * psExtent = new OGREnvelope();
	OGREnvelope oEnv;
	MYSQL_ROW row;
	unsigned long *  row_length;
	
	// query for the geometry enveloppe for all lines
	wxString sSentence = wxString::Format(_T("SELECT Envelope(%s) FROM %s"),
										  tmGISMYSQL_FIELD2.c_str(), GetShortFileName().c_str());
	if (m_DB->DataBaseQuery(sSentence))
	{
		// init extend based on the first object
		// to avoid 0 values for Xmin.
		row_length = m_DB->DataBaseGetNextRowResult(row);
		
		if(!row)
		{
			wxLogDebug(_T("No spatial data in the layer : %s"),
					   GetShortFileName().c_str());
			
			return tmRealRect(0,0,0,0);
		}
		
		
		OGRGeometry *poGeometry = CreateDataBaseGeometry(row, row_length);
		
		wxASSERT(poGeometry);
		
		poGeometry->getEnvelope(&oEnv);
		psExtent->MinX = oEnv.MinX;
		psExtent->MinY = oEnv.MinY;
		delete poGeometry;
		
		// loop all lines
		while (1)
		{
			row_length = m_DB->DataBaseGetNextRowResult(row);
			if (row_length == NULL)
				break;
			
			// compute the geometry and get the xmin xmax, ymin, ymax
			OGRGeometry *poGeometry = CreateDataBaseGeometry(row, row_length);
			if ( poGeometry != NULL )
			{
				poGeometry->getEnvelope(&oEnv);
				if (oEnv.MinX < psExtent->MinX) 
					psExtent->MinX = oEnv.MinX;
				if (oEnv.MinY < psExtent->MinY) 
					psExtent->MinY = oEnv.MinY;
				if (oEnv.MaxX > psExtent->MaxX) 
					psExtent->MaxX = oEnv.MaxX;
				if (oEnv.MaxY > psExtent->MaxY) 
					psExtent->MaxY = oEnv.MaxY;
			}
			delete poGeometry;
		}
		return tmRealRect(psExtent->MinX,psExtent->MinY,
						  psExtent->MaxX,psExtent->MaxY);
		
	}
	
	wxLogDebug(_T("Error computing extend : %s : Sentence is %s "),
			   m_DB->DataBaseGetLastError().c_str(),
			   sSentence.c_str());
	
	
	
	return tmRealRect(0,0,0,0);
}



OGRGeometry *  tmGISDataVectorMYSQL::CreateDataBaseGeometry(MYSQL_ROW & row,
															unsigned long * length,
															int geometry_col)
{
	OGRGeometry * geometry = NULL;
	// Geometry columns will have the first 4 bytes contain the SRID.
	OGRGeometryFactory::createFromWkb(((unsigned char *)row[geometry_col]) + 4, 
									  NULL,
									  &geometry,
									  length[geometry_col] - 4 );
	return geometry;
}




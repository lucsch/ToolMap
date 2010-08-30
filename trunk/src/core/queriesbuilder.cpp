/***************************************************************************
								queriesbuilder.cpp
                   
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

#include "queriesbuilder.h"


bool QueriesBuilder::_IsQueryNull() {
	if (m_QueryData == NULL) {
		wxLogError(_T("Query is null !"));
		return true;
	}
	return false;
}



bool QueriesBuilder::_IsCreated() {
	if (m_IsCreated == false){
		wxLogError(_T("Query not created, call Create() first"));
		return false;
	}
	return true;
}



bool QueriesBuilder::_CreateLayersQuery() {
	wxString myBaseQuery(_T("SELECT OBJECT_GEOM_ID FROM %s INNER JOIN %s ON")
						 _T(" OBJECT_VAL_ID = OBJECT_ID WHERE THEMATIC_LAYERS_LAYER_INDEX=%d"));
	wxString myAtribTable = TABLE_NAME_GIS_ATTRIBUTION[m_QueryData->m_QueryLayerType];
	m_QueryData->m_QuerySQL = wxString::Format(myBaseQuery, 
											   myAtribTable.c_str(),
											   TABLE_NAME_OBJECTS.c_str(),
											   m_QueryData->m_QueryLayerID);
	return true;
}

bool QueriesBuilder::_CreateSelectionQuery() {
	wxString myAtribTable = TABLE_NAME_GIS_ATTRIBUTION[m_QueryData->m_QueryLayerType];
	wxString myBaseQuery = wxEmptyString;
	wxString myQueryAdd = wxEmptyString;
	
	switch (m_QueryData->m_QueryFieldsStatus) {
		case AATTRIBUTION_NO:
			myBaseQuery =  _T("SELECT attrib.OBJECT_GEOM_ID from %s")
			_T(" attrib WHERE attrib.OBJECT_VAL_ID = %d");
			
			m_QueryData->m_QuerySQL = wxString::Format(myBaseQuery,
													   myAtribTable.c_str(),
													   m_QueryData->m_QueryObjectID);
			break;
			
			
			
		case AATTRIBUTION_EMPTY:
			myBaseQuery = _T("SELECT DISTINCT attrib.OBJECT_GEOM_ID from %s attrib LEFT JOIN (%s%d layer)")
			_T(" ON (attrib.OBJECT_GEOM_ID = layer.OBJECT_ID) WHERE attrib.OBJECT_VAL_ID = %d AND layer.OBJECT_ID IS NULL");
			m_QueryData->m_QuerySQL = wxString::Format(myBaseQuery,
													   myAtribTable.c_str(),
													   TABLE_NAME_LAYER_AT.c_str(),
													   m_QueryData->m_QueryLayerID,
													   m_QueryData->m_QueryObjectID);
			break;
			
		
			
		case AATTRIBUTION_YES:
			myBaseQuery = wxString::Format(_T("SELECT DISTINCT  type.OBJECT_GEOM_ID  FROM %s type")
												 _T("  LEFT JOIN (%s%d layer) ON (type.OBJECT_GEOM_ID ")
												 _T("= layer.OBJECT_ID) WHERE type.OBJECT_VAL_ID =%d AND"),
												 myAtribTable.c_str(),
												 TABLE_NAME_LAYER_AT.c_str(),
												 m_QueryData->m_QueryLayerID,
												 m_QueryData->m_QueryObjectID);
			myQueryAdd = _T(" layer.%s = %s AND");
			for (unsigned int i = 0; i< m_QueryData->m_QueryFields.GetCount(); i++) {
				switch ( m_QueryData->m_QueryFields.Item(i).m_FieldType) {
					case TM_FIELD_INTEGER:
					case TM_FIELD_FLOAT:
						myBaseQuery.Append(wxString::Format(_T(" layer.%s = %s AND"),
														 m_QueryData->m_QueryFields.Item(i).m_Fieldname.c_str(),
														 m_QueryData->m_QueryFieldsValues.Item(i).c_str()));
						break;
						
						
					default:
						myBaseQuery.Append(wxString::Format(_T(" layer.%s = \"%s\" AND"),
														 m_QueryData->m_QueryFields.Item(i).m_Fieldname.c_str(),
														 m_QueryData->m_QueryFieldsValues.Item(i).c_str()));
						
						break;
				}
				
			}
			myBaseQuery.RemoveLast(3);
			m_QueryData->m_QuerySQL = myBaseQuery;
			break;
			
			
			
			
		default:
			wxLogError(_T("Unsupported case"));
			break;
	}
	return true;
}




bool QueriesBuilder::_CreateObjectQuery() {
	wxString myBaseQuery = wxString::Format(_T("SELECT OBJECT_GEOM_ID from %s WHERE OBJECT_VAL_ID = %d"),
											TABLE_NAME_GIS_ATTRIBUTION[m_QueryData->m_QueryLayerType].c_str(),
											m_QueryData->m_QueryObjectID);
	m_QueryData->m_QuerySQL = myBaseQuery;
	return true;
}



bool QueriesBuilder::_CreateGeomLineQuery() {
	wxString myQuery = wxString::Format(_T("SELECT geom.OBJECT_ID  FROM %s geom WHERE GLength(OBJECT_GEOMETRY) < %d"),
										TABLE_NAME_GIS_GENERIC[0].c_str(), // allways lines
										m_QueryData->m_QueryLineSize);
	m_QueryData->m_QuerySQL = myQuery;
	return true;
}

bool QueriesBuilder::_CreateGeomNodeQuery() {
	wxString myQuery = wxString::Format(_T("SELECT geom.OBJECT_ID  FROM %s geom WHERE NumPoints(OBJECT_GEOMETRY) < %d"),
										TABLE_NAME_GIS_GENERIC[0].c_str(),
										m_QueryData->m_QueryNodeNumber);
	m_QueryData->m_QuerySQL = myQuery;
	return true;
}


bool QueriesBuilder::_CreateDuplicateQuery() {
	wxString myQuery = wxString::Format(_T("SELECT OBJECT_ID FROM %s GROUP by OBJECT_GEOMETRY")
										_T(" having COUNT(OBJECT_GEOMETRY) > 1 ORDER BY OBJECT_ID"),
										TABLE_NAME_GIS_GENERIC[m_QueryData->m_QueryLayerType].c_str());
	m_QueryData->m_QuerySQL = myQuery;
	return true;
}




QueriesBuilder::QueriesBuilder(QueriesData * query) {
	wxASSERT(query);
	m_QueryData = query;
	m_IsCreated = false;
}

QueriesBuilder::~QueriesBuilder() {
}

bool QueriesBuilder::IsOk() {
	if (_IsQueryNull())
		return false;
	
	return m_QueryData->IsOk();
}



bool QueriesBuilder::Save(DataBaseTM * database) {
	wxASSERT(database);
		
	if (_IsCreated()==false){
		return false;
	}
	
	
	return database->EditQueries((int) m_QueryData->m_QueryLayerType,
						  m_QueryData->m_QueryName,
						  m_QueryData->m_QuerySQL,
						  -1);
}


bool QueriesBuilder::Create(DataBaseTM * database) {
	
	if (IsOk()==false)
		return false;
	
	switch (m_QueryData->m_QueryType) {
		case QUERY_GENERIC:
			m_IsCreated = true;
			break;
			
		case QUERY_SQL:
			m_IsCreated = true;
			break;
			
		case QUERY_LAYERS:
			_CreateLayersQuery();
			m_IsCreated = true;
			break;
			
		case QUERY_SELECTED:
			_CreateSelectionQuery();
			m_IsCreated = true;
			break;
			
			
		case QUERY_OBJECTS:
			_CreateObjectQuery();
			m_IsCreated = true;
			break;
			
		case QUERY_LINES:
			_CreateGeomLineQuery();
			m_IsCreated = true;
			break;
			
		case QUERY_NODES:
			_CreateGeomNodeQuery();
			m_IsCreated = true;
			break;
			
		case QUERY_DUPLICATE:
			_CreateDuplicateQuery();
			m_IsCreated = true;
			break;

		default:
			wxLogError(_T("Unsupported query type"));
			break;
	}
	
	return m_IsCreated;
}




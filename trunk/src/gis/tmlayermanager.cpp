/***************************************************************************
						tmlayermanager.cpp
                    Central point for layer management
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

#include "tmlayermanager.h"




BEGIN_EVENT_TABLE(tmLayerManager, wxEvtHandler)
	EVT_COMMAND(wxID_ANY, tmEVT_LM_REMOVE,tmLayerManager::RemoveLayer)
	EVT_COMMAND(wxID_ANY, tmEVT_LM_ADD,  tmLayerManager::AddLayer)
END_EVENT_TABLE()



/***************************************************************************//**
 @brief Constructor
 @details Init values to default and init members.
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 July 2008
 *******************************************************************************/
tmLayerManager::tmLayerManager(wxWindow * parent, tmTOCCtrl * tocctrl)
{
	InitMemberValue();
	
	m_TOCCtrl = tocctrl;
	m_Parent = parent;
	m_Parent->PushEventHandler(this);
}


/***************************************************************************//**
 @brief Destructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 July 2008
 *******************************************************************************/
tmLayerManager::~tmLayerManager()
{
	UnInitLayerManager();
	m_Parent->PopEventHandler(FALSE);
}


/***************************************************************************//**
 @brief Init members to default
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 July 2008
 *******************************************************************************/
void tmLayerManager::InitMemberValue()
{
	m_TOCCtrl = NULL;
	m_Parent = NULL;
	m_DB = NULL;
}



/***************************************************************************//**
 @brief Init the layer manager with a project
 @details Call this function when a project is opened, this allows the needed
 initialization to takes place.
 @param db Pointer to a valid #DataBaseTM object (ASSERT is done in debug mode)
 @return  Always TRUE for the moment
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 July 2008
 *******************************************************************************/
bool tmLayerManager::InitLayerManager(DataBaseTM * db)
{
	// check
	wxASSERT_MSG (m_TOCCtrl != NULL, _T("Toc Ctrl is null, error"));
	wxASSERT_MSG (db != NULL, _T("Database pointer is empty... error"));
	
	// 1) init Database
	m_DB = db;
	
	
	// 2) Init TOCCtrl
	m_TOCCtrl->InsertProjectName(m_DB->DataBaseGetName());
	FillTOCArray();
	
	// 3) Load Data
	if(!LoadProjectLayers())
	{
		wxLogDebug(_T("Loading layers not completed succesfully"));
		return FALSE;
	}	
	
	
	return TRUE;
}



/***************************************************************************//**
 @brief Un-Init the layer manager
 @details Call this function when a project is closed
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 July 2008
 *******************************************************************************/
bool tmLayerManager::UnInitLayerManager()
{
	
	// saving TOC status
	if (m_TOCCtrl->IsTOCReady())
		SaveTOCStatus();
	
	wxLogDebug(_T("Clearing TOC"));
	m_DB = NULL;
	
	
	// clear the ctrl
	m_TOCCtrl->ClearAllLayers();
	
	return TRUE;
}


/***************************************************************************//**
 @brief Fill TOC control with Database TOC values
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 July 2008
 *******************************************************************************/
void tmLayerManager::FillTOCArray()
{
	wxASSERT_MSG (m_DB != NULL, _T("Database pointer is empty... error"));
	
	
	tmLayerProperties * lyrproptemp = NULL;
	int iNumberAdded = 0;
	
	while (1)
	{
		lyrproptemp = m_DB->GetNextTOCEntry();
		
		if(lyrproptemp ==NULL)
		{
			break;
		}
		
		  if(!m_TOCCtrl->InsertLayer(lyrproptemp))
			  break;
	}
	
	wxLogDebug(_T("%d items added to TOC array"), m_TOCCtrl->GetCountLayers());
}



/***************************************************************************//**
 @brief Save TOC status to database
 @details This function called when closing project saves the status of the TOC
 ctrl. Only visibility and rank of layers are saved, other informations are
 saved in real time when adding a new layer or removing one.
 @return  TRUE if data saved to database, FALSE otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 July 2008
 *******************************************************************************/
bool tmLayerManager::SaveTOCStatus()
{
	wxASSERT_MSG(m_TOCCtrl, _T("Error TOC ctrl not defined"));
	
	tmLayerProperties * itemProp = NULL;
	int iRank = 0;
	
	wxString sSentence = _T("");
	
	while (1)
	{
		if (iRank == 0)
		{
			itemProp = m_TOCCtrl->IterateLayers(TRUE);
		}
		else
		{
			itemProp = m_TOCCtrl->IterateLayers(FALSE);
		}
		
		if (!itemProp)
			break;
		
		m_DB->PrepareTOCStatusUpdate(sSentence, itemProp, iRank);
		iRank ++;
	}
	
	// update the database with toc status
	if (!m_DB->DataBaseQueryNoResult(sSentence))
	{
		wxLogDebug(_T("Error updating DB with TOC status : %s"),sSentence.c_str());
		return FALSE;
	}
	
	return TRUE;
	
}



/***************************************************************************//**
 @brief Response to the event sent by the #tmTOCCtrl
 @details Item was alleready delleted from the tmTOCCtrl and is now removed from
 the DB and the display is refreshed.
 @param event Contain the layer database ID into the GetExtraLong() function
 @author Lucien Schreiber (c) CREALP 2008
 @date 11 July 2008
 *******************************************************************************/
void tmLayerManager::RemoveLayer (wxCommandEvent & event)
{
	// checks where allready done by the tmTOCCtrl
	// we can delete item here from the DB
	long litemID = event.GetExtraLong();
	
	if (!m_DB->RemoveTOCLayer(litemID))
		return;
	
	// TODO: Refresh screen display
	
	
	wxLogDebug(_T("tmLayerManager : removing layer %d"), litemID);
}



/***************************************************************************//**
 @brief Response to the event sent by the "Add Gis Data" menu
 @details This function is doing following operations :
 - Checking that #tmTOCCtrl is ready : tmTOCCtrl::IsTOCReady()
 - Importing data 
 - Saving layer to the database and getting back his real ID
 - Adding layer to the tmTOCCtrl
 with his real ID.
 @author Lucien Schreiber (c) CREALP 2008
 @date 11 July 2008
 *******************************************************************************/
void tmLayerManager::AddLayer (wxCommandEvent & event)
{
	// check that the a project was opened !
	if (!m_TOCCtrl->IsTOCReady())
		return;
	
	
	// TODO: Import data using GIS class
	wxFileDialog * m_dlg = new wxFileDialog(m_Parent, _("Add GIS layer to the project"),
											_T(""), _T(""),
											tmGISData::GetAllSupportedGISFormatsWildcards());
	if(m_dlg->ShowModal() == wxID_CANCEL)
	{
		delete m_dlg;
		return;
	}
	
	tmGISData * myData;
	
	myData = tmGISData::CreateGISBasedOnType( m_dlg->GetFilterIndex());
	wxASSERT(myData);
	myData->Open(m_dlg->GetPath());
	tmRealRect myRect = myData->GetMinimalBoundingRectangle();
	wxLogDebug(_T("Minimum rectangle is : %.*f - %.*f, %.*f - %.*f"),
			   2,myRect.x_min, 2, myRect.y_min,
			   2, myRect.x_max, 2, myRect.y_max);
	
	
	// trying some database functions 
	tmGISDataVectorMYSQL::SetDataBaseHandle(m_DB);
	tmGISData * myDBGIS = tmGISData::CreateGISBasedOnType(tmGIS_VECTOR_MYSQL);
	if(myDBGIS->Open(_T("generic_lines")))
	{
		myRect = myDBGIS->GetMinimalBoundingRectangle();
		wxLogDebug(_T("Minimum rectangle is : %.*f - %.*f, %.*f - %.*f"),
				   2,myRect.x_min, 2, myRect.y_min,
				   2, myRect.x_max, 2, myRect.y_max);
	}
	
	
	if (myDBGIS)
		delete myDBGIS;
	
	
	// TEMP: code for trying adding
	wxFileName myFilename (m_dlg->GetPath());
	
	
	tmLayerProperties * item = new tmLayerProperties();
	item->m_LayerNameExt = myFilename.GetFullName();
	item->m_LayerPathOnly = myFilename.GetPath();
	
	
	
	delete m_dlg;
	if(myData)
	{
		wxLogDebug(_T("Pointer not empty, Nice"));
		delete myData;
	
	}
	
	
	
	
	// saving to the database and getting the last ID
	long lastinsertedID = m_DB->AddTOCLayer(item);
	if (lastinsertedID < 0)
		return;
	
	item->m_LayerID = lastinsertedID;
	item->m_LayerNameExt.Append(wxString::Format(_T(" [%d]"),lastinsertedID));
	
	// adding entry to TOC
	if(!m_TOCCtrl->InsertLayer(item))
		return;

	
}



bool tmLayerManager::LoadProjectLayers()
{
	// ensure that TOC ctrl isn't empty
	if (m_TOCCtrl->GetCountLayers() == 0)
	{
		wxLogDebug(_T("No data loaded into the TOC ctrl, load data into the TOC first"));
		return FALSE;
	}
	
	// iterate throught all layers
	// TODO: May need a threaded version here
	int iRank = 0;
	tmLayerProperties * itemProp = NULL;
	
	// prepare loading of MySQL data
	tmGISDataVectorMYSQL::SetDataBaseHandle(m_DB);
	while (1)
	{
		if (iRank == 0)
		{
			itemProp = m_TOCCtrl->IterateLayers(TRUE);
		}
		else
		{
			itemProp = m_TOCCtrl->IterateLayers(FALSE);
		}
		
		if (!itemProp)
			break;
		
		// loading data
		tmGISData * layerData = LoadLayer(itemProp);
		
		// processing data
		
		// deleting data
		if (layerData)
		{
			wxFileName myfilename (itemProp->m_LayerPathOnly, itemProp->m_LayerNameExt);
			wxLogDebug(myfilename.GetFullPath());
			//layerData->Open(<#const wxString filename#>, <#bool bReadWrite#>)
			
			delete layerData;
		}
		
		
		iRank ++;
		
	}
	
	
	return TRUE;
}



tmGISData * tmLayerManager::LoadLayer (tmLayerProperties * layerProp)
{
	wxASSERT(layerProp);
	tmGISData * m_Data = NULL;
	wxString myExtension = _T("");
	
	switch (layerProp->m_LayerIsGeneric)
	{
		case TOC_NAME_LINES:
		case TOC_NAME_POINTS:
		case TOC_NAME_ANNOTATIONS:
		case TOC_NAME_LABELS:
		case TOC_NAME_FRAME:
			m_Data = tmGISData::CreateGISBasedOnType(tmGIS_VECTOR_MYSQL);
			break;
		case TOC_NAME_NOT_GENERIC:
			myExtension = layerProp->GetFileExtension();
			m_Data = tmGISData::CreateGISBasedOnExt(myExtension);
			break;
			
		default:
			wxLogDebug(_T("%s file format not supported yet"),layerProp->m_LayerNameExt.c_str() );
			return NULL;
			break;
	}
	
	
	// here load data
		
	return m_Data;
}

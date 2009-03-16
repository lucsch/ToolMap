/***************************************************************************
							tmattributionmanager.cpp
							Manage the attribution
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

#include "tmattributionmanager.h"


DEFINE_EVENT_TYPE(tmEVT_SHORTCUT_ATTRIBUTION_DONE);
DEFINE_EVENT_TYPE(tmEVT_AM_COPY_ATTRIBUTION);

BEGIN_EVENT_TABLE(tmAttributionManager, wxEvtHandler)
	EVT_COMMAND (wxID_ANY, tmEVT_SELECTION_DONE, tmAttributionManager::OnSelection)
	EVT_COMMAND (wxID_ANY, tmEVT_ATTRIBUTION_BTN_PRESSED, tmAttributionManager::OnAttributeBtn)
	EVT_COMMAND (wxID_ANY, tmEVT_INFO_BTN_PRESSED, tmAttributionManager::OnInfoBtn)
	EVT_COMMAND (wxID_ANY, tmEVT_QUERY_RUN, tmAttributionManager::OnRunQuery)
	EVT_COMMAND (wxID_ANY, tmEVT_SHORTCUT_REFRESH, tmAttributionManager::OnRefreshShortcut)
	EVT_COMMAND (wxID_ANY, tmEVT_AM_SHORTCUT_PRESSED, tmAttributionManager::OnShortcutPressed)
	EVT_COMMAND (wxID_ANY, tmEVT_AM_COPY_ATTRIBUTION, tmAttributionManager::OnCopyAttribution)
END_EVENT_TABLE()


/***************************************************************************//**
 @brief Constructor
 @param parent Adress of the parent, used for event processing
 @param toc Adress of a #tmTOCCtrl
 @param panel Adress of a #AttribObjType_PANEL object
 @param selection Adress of a #tmSelectedDataMemory object
 @author Lucien Schreiber (c) CREALP 2008
 @date 04 November 2008
 *******************************************************************************/
tmAttributionManager::tmAttributionManager(wxWindow * parent,
										   tmTOCCtrl * toc, 
										   AttribObjType_PANEL * panel,
										   tmSelectedDataMemory * selection)
{
	m_Parent = parent;
	m_TOC = toc;
	m_Panel = panel;
	m_SelData = selection;
	m_pPrjMem = NULL;
	
	m_pDB = NULL;
	m_pLayerProperties = NULL;
	m_Parent->PushEventHandler(this);
	m_ShortcutLoaded = false;
	
}


/***************************************************************************//**
 @brief Connect shortcut
 @details This function is called when project is open
 @author Lucien Schreiber (c) CREALP 2008
 @date 18 December 2008
 *******************************************************************************/
void tmAttributionManager::ConnectShortcutEvent()
{
	m_ShortcutLoaded = true;
}


/***************************************************************************//**
 @brief Disconnect shortcut
 @details This function is called when project is closed
 @author Lucien Schreiber (c) CREALP 2008
 @date 18 December 2008
 *******************************************************************************/
void tmAttributionManager::DisconnectShortcutEvent()
{
	m_ShortcutLoaded = false;
}


/***************************************************************************//**
 @brief Called when a key is pressed
 @details This function respond to the F1-F12 shortcut key
 @author Lucien Schreiber (c) CREALP 2008
 @date 18 December 2008
 *******************************************************************************/
void tmAttributionManager::OnShortcutPressed (wxCommandEvent & event)
{
	int iShortcutIndex = 0;
	int myLayerType = -1;
	wxString myDescription = _T("");
	wxArrayLong myValues;

	if (m_ShortcutLoaded == false)
		return;

	int myKeyCode = event.GetInt();
	
	wxLogDebug(_T("Key pressed : value %d"),myKeyCode);
	
	if (myKeyCode >= WXK_F1 && myKeyCode <= WXK_F12)
	{
		// get the key index : 
		iShortcutIndex = myKeyCode - WXK_F1 + 1;
		wxASSERT(iShortcutIndex >= 1 && iShortcutIndex <=12);
		if(m_ShortcutMem.GetShortcut(iShortcutIndex, myLayerType,
									 myDescription, myValues) != wxNOT_FOUND)
		{
			// get selected features
			wxArrayLong  * mySelObjArray = m_SelData->GetSelectedValues();
			
			// verification 			
			if (mySelObjArray && ShortcutAttributionChecking(mySelObjArray->GetCount(),
															 myLayerType))
			{
				wxLogDebug(_T("Shortcut found : %s"), myDescription.c_str());
				
				// create attribution object based on type
				tmAttributionData * myAttrib = CreateAttributionData(myLayerType);
				myAttrib->Create(mySelObjArray, m_pDB);
				if(!myAttrib->SetAttributeBasicValues(&myValues))
				{
					wxLogMessage(_("Unable to attribute those data"));
				}
				
				delete myAttrib;
				
				// send notification to frame
				wxCommandEvent evt(tmEVT_SHORTCUT_ATTRIBUTION_DONE, wxID_ANY);
				evt.SetString(myDescription);
				m_Parent->GetEventHandler()->AddPendingEvent(evt);
				
			}
		}
		
		
	}
	
}



/***************************************************************************//**
 @brief Validate program state before shortcut attribution
 @details We should ensure that :
 - Selected layer in the TOC correspond to the Shortcut layer type
 - There is some object selected
 - Display a validation
 dialog if trying to give values to more than one object to avoid big
 attribution mistakes
 @param iCount Number of selected object
 @param shortcutlayer_type The layer type of the shortcut
 @return  true if all checks listed above are valid, false if we shouldn't
 attribute
 @author Lucien Schreiber (c) CREALP 2008
 @date 18 December 2008
 *******************************************************************************/
bool tmAttributionManager::ShortcutAttributionChecking (int iCount, int shortcutlayer_type)
{
	wxASSERT(m_pLayerProperties);
	if (m_pLayerProperties->m_LayerType != shortcutlayer_type)
	{
		wxLogDebug(_T("Something incorrect, selected shortcut")
				   _T(" doesn't correspond to the selected layer"));
		return false;
	}
	
	
	if (iCount == 0)
	{
		wxLogDebug(_T("No object selected, unable to attribute"));
		return false;
	}
	
	// more than one object selected, display confirmation dialog box
	if (iCount > 1)
	{
		wxString myM = _("More than one object is selected!\n");
		wxString myM2 = _("Are you really sure you want to attributes\n");
		wxString myM3 = wxString::Format(_("the %d selected features ?\n"),
										 iCount);
				
		if(wxMessageBox(myM + myM2 + myM3,
						_("Warning more than one object selected"),
						wxYES_NO | wxNO_DEFAULT, m_Parent) == wxNO)
			return false;
	}
	
	return true;
}



/***************************************************************************//**
 @brief Destructor
 @details Doesn't do anything actually
 @author Lucien Schreiber (c) CREALP 2008
 @date 04 November 2008
 *******************************************************************************/
tmAttributionManager::~tmAttributionManager()
{
	UnInitAttributionManager();
	m_Parent->PopEventHandler(false);
	m_Parent->SetEventHandler(m_Parent);
}



/***************************************************************************//**
 @brief Init attribution manager
 @details This function should be the first called for beeing able to use the
 attibution manager
 @return  true if Database, TOC, selection data, and panel are valid
 @author Lucien Schreiber (c) CREALP 2008
 @date 04 November 2008
 *******************************************************************************/
bool tmAttributionManager::InitAttributionManager(DataBaseTM * pDb, 
												  PrjDefMemManage * memprojdef)
{
	m_pDB = pDb;
	wxASSERT (m_pDB);
	
	m_pPrjMem = memprojdef;
	wxASSERT(m_pPrjMem);
	
	m_pLayerProperties = NULL;
	
	// check validity for all objects
	bool bIsReady = IsAttributionManagerReady();
	if (bIsReady)
		ConnectShortcutEvent();
	
	return bIsReady;
}



/***************************************************************************//**
 @brief Uninit attribution manager
 @details Mark the attribution manager as not valid.
 @author Lucien Schreiber (c) CREALP 2008
 @date 04 November 2008
 *******************************************************************************/
void tmAttributionManager::UnInitAttributionManager()
{
	m_pDB = NULL;
	m_pLayerProperties = NULL;
	
	DisconnectShortcutEvent();
}



/***************************************************************************//**
 @brief Checks all #tmAttributionManager object for validity
 @return  true if all objects are valid, false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 04 November 2008
 *******************************************************************************/
bool tmAttributionManager::IsAttributionManagerReady()
{
	if (!m_Parent)
		return false;
	
	if (!m_Panel)
		return false;
	
	if (!m_SelData)
		return false;
	
	if(!m_TOC)
		return false;
	
	if (!m_pDB)
		return false;
	
	return true;
}



/***************************************************************************//**
 @brief Checks that only one object is selected
 @details Use the IsAttributionManagerReady() before to ensure that all controls
 are well initialised
 @return true if only one item is selected
 @author Lucien Schreiber (c) CREALP 2009
 @date 11 March 2009
 *******************************************************************************/
bool tmAttributionManager::IsOnlyOneObjSelected ()
{
	wxASSERT (m_SelData);
	if (m_SelData->GetCount() == 1)
		return true;
	return false;
}



/***************************************************************************//**
 @brief Called when selection was made
 @details This is called by the #tmRenderer when a selection (either by point or
 rectangle) was made. This event is also used by #tmLayerManager::OnSelection
 @param event Contain data about the status of the shift key and the rectangle
 size. 
 - Use event.GetInt() for getting the status of the shift key
 - Use event.GetClientObject() for getting the wxRect.
 @author Lucien Schreiber (c) CREALP 2008
 @date 29 October 2008
 *******************************************************************************/
void tmAttributionManager::OnSelection (wxCommandEvent & event)
{
	int iSelFeatureCount = m_SelData->GetCount();
	
	// some verifications :
	//	A layer must be selected
	//	A construction layer must be selected (< TOC_NAME_FRAME)
	m_pLayerProperties = m_TOC->GetSelectionLayer();
	if (!m_pLayerProperties)
		return;
	
	if(m_pLayerProperties->m_LayerType >= TOC_NAME_FRAME)
		return;
	
	m_Panel->SetAttributeBtn(iSelFeatureCount);
	m_Panel->SetInfoBtn(iSelFeatureCount);
	TOC_GENERIC_NAME mySelType = static_cast<TOC_GENERIC_NAME> (m_pLayerProperties->m_LayerType);
	m_Panel->SetVisibleNotebook(mySelType);
	
	// if auto display attribute is checked 
	if (m_Panel->IsAutoDisplayAttributeChecked() && iSelFeatureCount == 1)
	{
		wxCommandEvent evt (tmEVT_INFO_BTN_PRESSED, wxID_ANY);
		m_Parent->GetEventHandler()->AddPendingEvent(evt);
	}
	
	// propagate to menu
	event.Skip();
	
	
}



/***************************************************************************//**
 @brief Called when user press attribute
 @details This event function is called by the #AttribObjType_PANEL when user
 press the attribute Button.
 @param event 
 @author Lucien Schreiber (c) CREALP 2008
 @date 05 November 2008
 *******************************************************************************/
void tmAttributionManager::OnAttributeBtn (wxCommandEvent & event)
{
	// verification : 
	//		Selected notebook page is same type as
	//		selected layer in TOC
	wxASSERT(m_pLayerProperties);
	if (m_pLayerProperties->m_LayerType != m_Panel->GetVisibleNotebook())
	{
		wxLogDebug(_("Something incorrect, selected panel doesn't correspond to the selected layer"));
		return;
	}
	
	wxArrayLong  * mySelObjArray = m_SelData->GetSelectedValues();
	//int myLayerIndex = m_SelData->GetSelectedLayer();
	
	// create attribution object based on type
	tmAttributionData * myAttrib = CreateAttributionData(m_pLayerProperties->m_LayerType);
	myAttrib->Create(mySelObjArray, m_pDB);
	if(!myAttrib->SetAttributeBasic(m_Panel))
	{
		wxLogMessage(_("Unable to attribute those data"));
	}
	
	delete myAttrib;
	
	// clear list if needed
	if (m_Panel->IsEmptyListValuesRequired())
		m_Panel->EmptyListValues();
	
}



/***************************************************************************//**
 @brief Called when user press the info button
 @details This function is called by the #AttribObjType_PANEL
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 November 2008
 *******************************************************************************/
void tmAttributionManager::OnInfoBtn (wxCommandEvent & event)
{
	wxLogDebug(_T("Getting info"));
	
	
	wxASSERT(m_pLayerProperties);
	wxASSERT (m_pDB);
	
	// create attribution object based on type
	wxArrayLong  * mySelObjArray = m_SelData->GetSelectedValues();
	tmAttributionData * myAttrib = CreateAttributionData(m_pLayerProperties->m_LayerType);
	
	myAttrib->Create(mySelObjArray, m_pDB);
	if (!myAttrib->GetInfoBasic(m_Panel))
	{
		wxLogError(_("Error getting informations for objects"));
	}
	
	delete myAttrib;
	
	
}


/***************************************************************************//**
 @brief Called when user modifies the shortcut
 @details This function reload the shortcuts into memory when shortcuts are
 changed
 @author Lucien Schreiber (c) CREALP 2008
 @date 18 December 2008
 *******************************************************************************/
void tmAttributionManager::OnRefreshShortcut (wxCommandEvent & event)
{
	
	wxLogDebug(_T("%d : shortcuts loaded into memory"),
			   LoadShortcutIntoMemory());
}



/***************************************************************************//**
 @brief Load shortcuts from database into memory
 @return  the number of loaded shortcuts
 @author Lucien Schreiber (c) CREALP 2008
 @date 18 December 2008
 *******************************************************************************/
int tmAttributionManager::LoadShortcutIntoMemory ()
{
	wxASSERT(m_pDB);
	bool bFirstLoop = true;
	int myLayerType = -1;
	int myKey = 0;
	wxString myDescription = _T("");
	long myShortcutValues = 0;
	
	// clear shortcut from memory
	m_ShortcutMem.Clear();
	
	
	// load shortcuts from memory
	while (1)
	{
		if(!m_pDB->GetNextShortcutFull(bFirstLoop, myLayerType,
								   myKey, myDescription, myShortcutValues))
		{
			break;
		}
		bFirstLoop = false;
		
		//wxLogDebug(_T("Shortcut : %d, %s, %d "),
		//		   myKey, myDescription.c_str(),myShortcutValues);
		
		m_ShortcutMem.AddShortcutMemory(myLayerType, myKey, 
										myDescription, myShortcutValues);
	}
	
	
	return m_ShortcutMem.GetCount();
}



/***************************************************************************//**
 @brief Create an object of #tmAttributionData type
 @details This function may be used for getting a valid pointer on one of the
 #tmAttributionData derived class.
 @param type one of the following : 
 - TOC_NAME_LINES 
 - TOC_NAME_LABELS
 - TOC_NAME_POINTS 
 - TOC_NAME_ANNOTATION
 @return  A valid pointer of type #tmAttributionData class
 @author Lucien Schreiber (c) CREALP 2008
 @date 06 November 2008
 *******************************************************************************/
tmAttributionData * tmAttributionManager::CreateAttributionData (int type)
{
	tmAttributionData * myAttrib = NULL;
	switch (type)
	{
		case TOC_NAME_LINES:
			myAttrib = new tmAttributionDataLine();
			break;
			
		case TOC_NAME_POINTS:
			myAttrib = new tmAttributionDataPoint();
			break;
			
		case TOC_NAME_LABELS:
			myAttrib = new tmAttributionDataLabel();
			break;
			
		default:
			myAttrib = new tmAttributionData();
			break;
	}
	
	if (myAttrib == NULL)
		wxLogDebug(_T("Error creating attribution object"));
	
	return myAttrib;
}


/***************************************************************************//**
 @brief Copy attribution between objects
 @details This event function may be called for copying attributions between
 objects.
 @param event event should contain following informations : 
 - event.GetExtraLong() the ID of the object we copy attribution from
 - event.GetClientData() (wxArrayLong*) the ID of objects we copy attribution to.
 @author Lucien Schreiber (c) CREALP 2009
 @date 16 February 2009
 *******************************************************************************/
void tmAttributionManager::OnCopyAttribution (wxCommandEvent & event)
{
	tmAttributionData * myAttrib = CreateAttributionData(m_TOC->GetEditLayer()->
														 m_LayerType);
	if (myAttrib == NULL)
		return;
	
	// init 
	wxArrayLong * mySelectedValues = (wxArrayLong*) event.GetClientData();
	myAttrib->Create(mySelectedValues, m_pDB);
	myAttrib->SetDataBaseTable(TABLE_NAME_GIS_ATTRIBUTION[m_TOC->GetEditLayer()->
														  m_LayerType]);
	
	// copy attribution
	myAttrib->CopyAttributesBasic(event.GetExtraLong());
	
	delete mySelectedValues;
	delete myAttrib;
}



/***************************************************************************//**
 @brief Received when user select run Query
 @details This function receive the query from the #Queries_PANEL. The query may
 be retreived from GetString() function
 @author Lucien Schreiber (c) CREALP 2008
 @date 10 November 2008
 *******************************************************************************/
void tmAttributionManager::OnRunQuery (wxCommandEvent & event)
{
	wxASSERT(m_pDB);
	wxASSERT(m_TOC);
	wxASSERT(m_SelData);
	
	wxLogDebug(_T("Running query"));
	
	// getting the query
	wxString myQuery = event.GetString();
	
	
	// getting the layer ID
	int myLayerType = event.GetInt();
	int myLayerID = m_pDB->GetLayerID(static_cast<TOC_GENERIC_NAME> (myLayerType));
	if (myLayerID == wxNOT_FOUND)
	{
		wxASSERT_MSG (0, _T("Error getting layer ID"));
		return;
	}
	// Selecting layer in TOC
	m_TOC->SetSelectedLayer(myLayerID);
		
	
	
	// passing the query
	if (!m_pDB->DataBaseQuery(myQuery))
	{
		wxString szError = wxString::Format(_("Error running the query. Bellow is the error rapported by MySQL\n%s"),
											m_pDB->DataBaseGetLastError().c_str());
		wxMessageBox(szError,
					 _("Error running the query"),
					 wxICON_EXCLAMATION | wxOK ,
					 m_Parent);
		return;
	}
	
	// query succeed, getting results
	wxArrayLong myResults;
	long myTempRes = -1;
	while (1)
	{
		myTempRes = m_pDB->DataBaseGetNextResultAsLong();
		if (myTempRes == -1)
			break;
		myResults.Add(myTempRes);
	}
	
	// clear selection
	m_SelData->Clear();
	m_SelData->SetLayerID(myLayerID);
	
	m_SelData->AddSelected(&myResults);
	
	// update display
	wxCommandEvent evt(tmEVT_LM_UPDATE, wxID_ANY);
	m_Parent->GetEventHandler()->AddPendingEvent(evt);
	
	// update attribution panel
	wxCommandEvent tmpevt;
	OnSelection(tmpevt);
	
}


//TODO: Add comment here
int tmAttributionManager::DisplayAAttributionWindow (wxArrayString * values,
													 PrjMemLayersArray * layers)
{
	/*TODO: Remove temp code
	PrjMemLayersArray myLayersArray;
	
	PrjMemFieldArray myFieldArray1;
	ProjectDefMemoryFields myField1;
	myField1.m_Fieldname = _T("Etat");
	myField1.m_FieldType = TM_FIELD_TEXT;
	myField1.m_FieldPrecision = 20;
	myFieldArray1.Add(myField1);
	
	ProjectDefMemoryFields myField2;
	myField2.m_Fieldname = _T("Status");
	myField2.m_FieldType = TM_FIELD_ENUMERATION;
	ProjectDefMemoryFieldsCodedVal myCodedVal1;
	myCodedVal1.m_ValueName = _T("Actif");
	PrjMemFieldCodedValArray myVals;
	myVals.Add(myCodedVal1);
	myCodedVal1.m_ValueName = _T("Inactif");
	myVals.Add(myCodedVal1);
	myField2.m_pCodedValueArray = &myVals;
	myFieldArray1.Add(myField2);
	
	
	ProjectDefMemoryLayers myLayer1;
	myLayer1.m_LayerName = _T("Faille");
	myLayer1.m_pLayerFieldArray = & myFieldArray1;
	myLayersArray.Add(myLayer1);
	//
	
	wxArrayString myValues;
	myValues.Add(_T("Remarques"));
	myValues.Add(_T("Inactif"));*/
	
	tmAAttribWindow myAADlg (m_Parent, layers, values, wxID_ANY);
	return myAADlg.ShowModal();
							 
}



/***************************************************************************//**
 @brief Advanced Attribution button is pressed
 @return  true if the advanced attribution succeed, false if cancelled or if an
 error occur
 @author Lucien Schreiber (c) CREALP 2009
 @date 11 March 2009
 *******************************************************************************/
bool tmAttributionManager::AAttributionButtonShow ()
{
	// validating
	if (IsAttributionManagerReady() == false)
	{
		wxLogDebug(_T("Project isn't ready, open a project or define a Edit Layer"));
		return false;
	}
	if (IsOnlyOneObjSelected() == false)
	{
		wxLogDebug(_T("This works only if one object is selected, select only one object"));
		return false;
	}
	tmLayerProperties * myEditLayer = m_TOC->GetEditLayer();
	if (myEditLayer == NULL)
	{
		wxLogDebug(_T("You aren't in edit mode, please start editing one layer first"));
		return false;
	} 
	
	
	tmAttributionData * myAttribObj = CreateAttributionData(myEditLayer->m_LayerType);
	if (myAttribObj == NULL)
		return false;
	myAttribObj->Create(m_SelData->GetSelectedValues(), m_pDB);
	
	// attributed layers
	wxArrayLong myLayersID;
	if (myAttribObj->GetAttributionLayersID(m_SelData->GetSelectedUnique(),
											myLayersID)==false)
		return false;
	


	
	// get layers info
	wxASSERT (m_pPrjMem);
	PrjMemLayersArray myLayersInfoArray;
	for (unsigned int i = 0; i<myLayersID.GetCount();i++)
	{
		wxLogDebug(_T("layer %d searched"), myLayersID.Item(i));
		
		ProjectDefMemoryLayers * myActualLayer = m_pPrjMem->FindLayerByRealID(myLayersID.Item(i));
		if (!myActualLayer)
		{
			wxLogDebug(_T("Layers %d not found, error"), myLayersID.Item(i));
			return false;
		}
		myLayersInfoArray.Add(myActualLayer);
	}
	
	
	wxArrayString myValues;
	DisplayAAttributionWindow(&myValues, &myLayersInfoArray);
	
	// cleaning without deleting item
	for (unsigned int j = 0; j<myLayersInfoArray.GetCount();j++)
		myLayersInfoArray.Detach(j);

	return true;
}


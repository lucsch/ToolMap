/***************************************************************************
								snapping_panel.cpp
				Deals with the snapping panel, used for setting the
							snapping for layers
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

#include "snapping_panel.h"

// EVENT TABLE
BEGIN_EVENT_TABLE( Snapping_PANEL, ManagedAuiWnd )
	EVT_TEXT( ID_SNAP_TOLERENCE_TXT, Snapping_PANEL::OnUpdateTolerence )
	EVT_FLATBUTTON( ID_SNAP_ADD, Snapping_PANEL::OnAddSnapping )
	EVT_FLATBUTTON( ID_SNAP_REMOVE, Snapping_PANEL::OnRemoveSnapping )
	EVT_FLATBUTTON( ID_SNAP_CLEAR, Snapping_PANEL::OnClearSnapping )
END_EVENT_TABLE()



/***************************************************************************//**
 @brief Snapping panel constructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 19 January 2009
 *******************************************************************************/
Snapping_PANEL::Snapping_PANEL( wxWindow* parent, wxWindowID id, wxAuiManager * auimanager)
: ManagedAuiWnd(auimanager)
{
	InitMembersValue();
	m_ParentEvt = parent;
	m_ParentEvt->PushEventHandler(this);
	
	wxPanel *  ContentFrame = new wxPanel (parent, wxID_ANY);
	CreateControls(ContentFrame);	
	
	// define properties of Panel.
	m_PaneInfo.Name(SNAPPING_PANEL_TITLE);
	m_PaneInfo.Caption(SNAPPING_PANEL_TITLE);
	m_PaneInfo.Float();
	m_PaneInfo.Right();
	m_PaneInfo.Layer(4);
	m_PaneInfo.Position(5);
	m_PaneInfo.MinSize(SNAPPING_PANEL_SIZE);
	m_PaneInfo.CloseButton(true);
	m_PaneInfo.Hide();
	m_PaneInfo.FloatingPosition(100, 150);
	m_PaneInfo.FloatingSize(SNAPPING_PANEL_SIZE + wxSize(100,100));
	
	m_AuiPanelName = SNAPPING_PANEL_TITLE;
	
	AddManagedPane(ContentFrame, m_PaneInfo);

}



/***************************************************************************//**
 @brief Set default values to member
 @author Lucien Schreiber (c) CREALP 2008
 @date 19 January 2009
 *******************************************************************************/
void Snapping_PANEL::InitMembersValue()
{
	m_Tolerence = NULL;
	m_SnappingList = NULL;
	m_ParentEvt = NULL;
	m_pDB = NULL;
}



/***************************************************************************//**
 @brief Create the Panel controls
 @author Lucien Schreiber (c) CREALP 2008
 @date 19 January 2009
 *******************************************************************************/
wxSizer * Snapping_PANEL::CreateControls(wxWindow * parent,
										 bool call_fit, bool set_sizer)
{
	
	wxBoxSizer* bSizer13;
	bSizer13 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer14;
	bSizer14 = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* m_staticText5;
	m_staticText5 = new wxStaticText( parent, wxID_ANY, _("Tolerence (map unit) :"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	bSizer14->Add( m_staticText5, 0, wxALL, 5 );
	
	m_Tolerence = new wxTextCtrl( parent, ID_SNAP_TOLERENCE_TXT, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer14->Add( m_Tolerence, 1, wxALL|wxEXPAND, 5 );
	
	bSizer13->Add( bSizer14, 0, wxEXPAND, 5 );
	
	wxArrayString colname;
	colname.Add(_("Layer"));
	colname.Add(_("Vertex"));
	colname.Add(_("Begin/End"));
	
	wxArrayInt colsize;
	colsize.Add(150);
	colsize.Add(80);
	colsize.Add(80);
	
    m_SnappingList = new SnappingList( parent, ID_SNAP_LIST,
									&colname, & colsize,
									wxSize(100, 100));
	
	
	bSizer13->Add( m_SnappingList, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer( wxHORIZONTAL );
	
	wxFlatButton* m_button10;
	m_button10 = new wxFlatButton( parent, ID_SNAP_ADD, wxFLATBUTTON_TEXT_ADD);
	bSizer15->Add( m_button10, 0, wxLEFT|wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	wxFlatButton* m_button11;
	m_button11 = new wxFlatButton( parent, ID_SNAP_REMOVE, wxFLATBUTTON_TEXT_REMOVE);
	bSizer15->Add( m_button11, 0, wxTOP|wxBOTTOM|wxRIGHT, 5 );
	
	wxFlatButton* m_button12;
	m_button12 = new wxFlatButton( parent, ID_SNAP_CLEAR, _("Clear snapping"), wxDefaultSize);
	bSizer15->Add( m_button12, 0, wxTOP|wxBOTTOM, 5 );
	
	bSizer13->Add( bSizer15, 0, wxEXPAND, 5 );
	
	
	if (set_sizer)
    {
        parent->SetSizer( bSizer13 );
        if (call_fit)
		{
            bSizer13->SetSizeHints( parent );
		}
	}
    
    return bSizer13;
	
	
}


Snapping_PANEL::~Snapping_PANEL()
{
}



/***************************************************************************//**
 @brief Load the snapping from database
 @details Call the Snapping_PANEL::SetDataBase() function before
 @return  true if snapping status was loaded successfully
 @author Lucien Schreiber (c) CREALP 2009
 @date 19 January 2009
 *******************************************************************************/
bool Snapping_PANEL::LoadSnappingStatus ()
{
	wxASSERT(m_pDB);
	
	long mylid = 0;
	wxString mylName = _T("");
	int mySnapStatus = 0;
	bool iFirstLoop = true;
	int iLoop = 0;
	
	while (1)
	{
		if (!m_pDB->GetNextSnapping(mylid, mylName, mySnapStatus, iFirstLoop))
			break;
		iFirstLoop = false;
		m_SnappingList->AddItemToList(mylName, iLoop);
		m_SnappingList->SetSnappingStatus(mySnapStatus, iLoop, false);
		m_SnappingList->SetItemData(iLoop, mylid);
		iLoop++;
	}
	
	
	return true;
}


/***************************************************************************//**
 @brief Called when user press add snapping
 @author Lucien Schreiber (c) CREALP 2009
 @date 20 January 2009
 *******************************************************************************/
void Snapping_PANEL::OnAddSnapping( wxCommandEvent& event )
{
	wxASSERT (m_pDB);
	if (m_pDB)
	{
		m_SnappingList->AddItem();
	}
}
















/************************ SNAPPING LIST *****************************************/
/***************************************************************************//**
 @brief Constructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 19 January 2009
 *******************************************************************************/
SnappingList::SnappingList (wxWindow * parent,
						  wxWindowID id,
						  wxArrayString * pColsName, 
						  wxArrayInt * pColsSize,
						  wxSize size) :
ListGenReportWithDialog(parent, id, pColsName, pColsSize, size)
{
	
}



/***************************************************************************//**
 @brief Destructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 19 January 2009
 *******************************************************************************/
SnappingList::~SnappingList()
{
	
}



/***************************************************************************//**
 @brief Set the snapping status
 @details Put yes or empty in snapping status column
 @param snapStatus The snapping status as integer :
  - 0 = No snapping 
  - 1 = Snapping vertex 
  - 2 = snapping Begin/End
 @author Lucien Schreiber (c) CREALP 2009
 @date 19 January 2009
 *******************************************************************************/
void SnappingList::SetSnappingStatus (int snapStatus, int iRow, bool clearbefore)
{
	// clear text
	if (clearbefore)
	{
		SetItemText(iRow, 1, _T(""));
		SetItemText(iRow, 2, _T(""));
	}
	
	if ((snapStatus & 1) == 1)
		SetItemText(iRow, 1, tmSNAPPING_TEXT_YES);
	
	if ((snapStatus & 2) == 2)
		SetItemText(iRow, 2, tmSNAPPING_TEXT_YES);
		
}



/***************************************************************************//**
 @brief Get the snapping status from the list
 @details This function gets the snapping status from one lists row into an
 integer
 @param iRow the zero based index of the row we want to get the snapping status
 @return  one of the following values :
 - 0 = no snapping 
 - 1 = snapping vertex
 - 2 = snapping begin/end 
 - 3 = snapping both
 @author Lucien Schreiber (c) CREALP 2009
 @date 20 January 2009
 *******************************************************************************/
int SnappingList::GetSnappingStatus (int iRow)
{
	// ensure that the row exists
	wxASSERT (iRow <= (GetItemCount() -1));
	
	int iReturnSnap = 0;
	if (GetItemColText(iRow, 1) == tmSNAPPING_TEXT_YES)
		iReturnSnap = iReturnSnap | 1;
	
	if (GetItemColText(iRow, 2) == tmSNAPPING_TEXT_YES)
		iReturnSnap = iReturnSnap | 2;
		
	
	return iReturnSnap;
}


/***************************************************************************//**
 @brief Called just before displaying the add dialog
 @author Lucien Schreiber (c) CREALP 2009
 @date 20 January 2009
 *******************************************************************************/
void SnappingList::BeforeAdding()
{
	wxArrayLong myLayersId;
	wxArrayString myLayersName;
	wxString myDlgMessage = _("Select one or more layer to add to the snapping list");
	wxString myDlgCaption = _("Add one or more snapping layer(s)");
	
	m_pDB->GetValidLayersForSnapping(myLayersId, myLayersName);
	wxMultiChoiceDialog * myDlg = new wxMultiChoiceDialog(this,
														  myDlgMessage,
														  myDlgCaption,
														  myLayersName);
	SetDialog(myDlg);
}


/***************************************************************************//**
 @brief Called after the adding dialog was closed
 @author Lucien Schreiber (c) CREALP 2009
 @date 20 January 2009
 *******************************************************************************/
void SnappingList::AfterAdding (bool bRealyAddItem)
{
	delete m_pDialog;
}


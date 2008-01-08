/***************************************************************************
							project_def.h
						Project definition dialog
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

#include "project_def_dlg.h"


IMPLEMENT_DYNAMIC_CLASS( ProjectDefDLG, wxDialog )



BEGIN_EVENT_TABLE( ProjectDefDLG, wxDialog )
	EVT_FLATBUTTON(ID_DLGPD_SPAT_MDL_ADD, ProjectDefDLG::OnAddLayer)
	EVT_BUTTON(ID_DLGPD_PROJ_PATH_BTN, ProjectDefDLG::OnSelectProjectPath)
    EVT_FLATBUTTON(ID_DLGPD_SPAT_MDL_DEL,  ProjectDefDLG::OnRemoveLayer)
END_EVENT_TABLE()


void ProjectDefDLG::OnAddLayer(wxCommandEvent & event)
{
	m_DlgPd_Stat_Model_List->AddItem();
}

void ProjectDefDLG::OnRemoveLayer (wxCommandEvent & event)
{
	// remove the object from the array
	m_DlgPd_Stat_Model_List->DeleteItem();
}


void ProjectDefDLG::OnSelectProjectPath (wxCommandEvent & event)
{
	wxDirDialog myDirDialog (this);
	if( myDirDialog.ShowModal() == wxID_OK)
	{
		m_DlgPD_Proj_Path->SetValue(myDirDialog.GetPath());
	}
}


ProjectDefDLG::ProjectDefDLG()
{
    Init();
}

ProjectDefDLG::ProjectDefDLG( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}



bool ProjectDefDLG::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();

    return true;
}




ProjectDefDLG::~ProjectDefDLG()
{
}



void ProjectDefDLG::Init()
{
    m_DlgPD_Proj_Path = NULL;
    m_DlgPd_Proj_Name = NULL;
    m_DlgPd_Proj_Unit = NULL;
    m_DlgPd_Proj_Projection = NULL;
    m_DlgPd_Stat_Model_List = NULL;

}



void ProjectDefDLG::CreateControls()
{    
    ProjectDefDLG* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Project properties"));
    wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxVERTICAL);
    itemBoxSizer2->Add(itemStaticBoxSizer3, 0, wxGROW|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer4 = new wxFlexGridSizer(4, 3, 0, 0);
    itemFlexGridSizer4->AddGrowableCol(1);
    itemStaticBoxSizer3->Add(itemFlexGridSizer4, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Project path :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_DlgPD_Proj_Path = new wxTextCtrl( itemDialog1, ID_DLGPD_PROJ_PATH, _T(""), wxDefaultPosition, wxSize(200, -1), 0 );
    itemFlexGridSizer4->Add(m_DlgPD_Proj_Path, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton * myDlgPd_Proj_Path_Btn = new wxButton( itemDialog1, ID_DLGPD_PROJ_PATH_BTN, _("..."), wxDefaultPosition, wxSize(50, -1), 0 );
    itemFlexGridSizer4->Add(myDlgPd_Proj_Path_Btn, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText8 = new wxStaticText( itemDialog1, wxID_STATIC, _("Project name :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText8, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_DlgPd_Proj_Name = new wxTextCtrl( itemDialog1, ID_DLGPD_PROJ_NAME, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(m_DlgPd_Proj_Name, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemFlexGridSizer4->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText11 = new wxStaticText( itemDialog1, wxID_STATIC, _("Units"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText11, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_DlgPd_Proj_UnitStrings;
    m_DlgPd_Proj_UnitStrings.Add(_("Meters"));
    m_DlgPd_Proj_UnitStrings.Add(_("Kilometers"));
    m_DlgPd_Proj_UnitStrings.Add(_("Lat / Long"));
    m_DlgPd_Proj_Unit = new wxChoice( itemDialog1, ID_DLGPD_PROJ_UNIT, wxDefaultPosition, wxDefaultSize, m_DlgPd_Proj_UnitStrings, 0 );
    m_DlgPd_Proj_Unit->SetStringSelection(_("Meters"));
    itemFlexGridSizer4->Add(m_DlgPd_Proj_Unit, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemFlexGridSizer4->Add(5, 5, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText14 = new wxStaticText( itemDialog1, wxID_STATIC, _("Projection :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText14, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_DlgPd_Proj_ProjectionStrings;
    m_DlgPd_Proj_ProjectionStrings.Add(_("No projection"));
    m_DlgPd_Proj_ProjectionStrings.Add(_("Swiss projection"));
    m_DlgPd_Proj_Projection = new wxChoice( itemDialog1, ID_DLGPD_PROJ_PROJECTION, wxDefaultPosition, wxDefaultSize, m_DlgPd_Proj_ProjectionStrings, 0 );
    m_DlgPd_Proj_Projection->SetStringSelection(_("No projection"));
    itemFlexGridSizer4->Add(m_DlgPd_Proj_Projection, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer16Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Define your spatial model"));
    wxStaticBoxSizer* itemStaticBoxSizer16 = new wxStaticBoxSizer(itemStaticBoxSizer16Static, wxVERTICAL);
    itemBoxSizer2->Add(itemStaticBoxSizer16, 1, wxGROW|wxALL, 5);

    m_DlgPd_Stat_Model_List = new ProjectDefList( itemDialog1, ID_DLGPD_SPAT_MODEL_LIST, wxSize(-1,150),this);
    itemStaticBoxSizer16->Add(m_DlgPd_Stat_Model_List, 1, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer18 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer16->Add(itemBoxSizer18, 0, wxALIGN_LEFT|wxALL, 5);

    wxFlatButton * myFlatButton = new wxFlatButton(itemDialog1, ID_DLGPD_SPAT_MDL_ADD, wxFLATBUTTON_TEXT_ADD);
	itemBoxSizer18->Add(myFlatButton, 0,wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

	
	wxFlatButton * myFlatButton2 = new wxFlatButton(itemDialog1, ID_DLGPD_SPAT_MDL_DEL, wxFLATBUTTON_TEXT_REMOVE);
    itemBoxSizer18->Add(myFlatButton2, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer21 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer21, 0, wxALIGN_RIGHT|wxALL, 5);
    wxButton* itemButton22 = new wxButton( itemDialog1, wxID_OK, _("&Create new project"), wxDefaultPosition, wxDefaultSize, 0 );
 	itemStdDialogButtonSizer21->AddButton(itemButton22);

    wxButton* itemButton23 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer21->AddButton(itemButton23);

    itemStdDialogButtonSizer21->Realize();
}




/************************ PROJECT DEF LIST **********************************/
ProjectDefList::ProjectDefList(wxWindow * parent, wxWindowID  id, wxSize size, ProjectDefDLG * myParentDlg) 
	: ListGenReportWithDialog(parent,id,size)
{
	
	// Create columns
	wxArrayString myColNames;
	wxArrayInt myColsWidths;
	
	myColNames.Add(_("Name"));
	myColNames.Add(_("Type"));
	
	myColsWidths.Add(250);
	myColsWidths.Add(80);
	
	CreateColumns(&myColNames, &myColsWidths);
	
	m_ChoiceToChange = NULL;
	
	// create an array for storing spatial thems
	m_LayersArray = new PrjMemLayersArray();

	m_LayersDialog = NULL;

}


ProjectDefList::~ProjectDefList()
{
	if (!m_LayersArray->IsEmpty())
		m_LayersArray->Clear();
	delete m_LayersArray;
}



void ProjectDefList::BeforeAdding()
{
	// create and set the dialog here
	ProjectDefLayersDlg * myLayerDialog = new ProjectDefLayersDlg (this);
	wxLogDebug(_T("Creating Thematic Layer Dialog"));
	SetDialog(myLayerDialog);
	
	m_LayersObj = new ProjectDefMemoryLayers;
	// pass container to the dialog for using Transfert data to and from
	// dialog.
	((ProjectDefLayersDlg*)m_pDialog)->SetMemoryLayersObject(m_LayersObj);
	
}


void ProjectDefList::AfterAdding(bool bRealyAddItem)
{
	wxArrayString myListValues;
	
	if (bRealyAddItem)
	{
		// add data to the array
		m_LayersArray->Add(m_LayersObj);
		wxLogDebug(_T("Size of the Spatial array : %d"), m_LayersArray->GetCount());
		
		
		// add data to the list
		// prepare data for list representation
		myListValues.Add(m_LayersObj->m_LayerName);
		myListValues.Add(PRJDEF_LAYERS_TYPE_STRING[m_LayersObj->m_LayerType]);
		EditDataToList(myListValues);
		
	}
	else
		delete m_LayersObj;	
	
	// delete the dialog here
    wxLogDebug(_T("Destroying Thematic Layer Dialog"));
	delete m_pDialog;
	
}


void ProjectDefList::BeforeEditing ()
{
	// create the dialog
	m_LayersDialog = new ProjectDefLayersDlg (this);
	wxLogDebug(_T("Creating Thematic Layer Dialog"));
	SetDialog(m_LayersDialog);

	// find item selected and then call a new Dialog
	// for editing the existing Field
	int iItemIndex = FindObjInLayersArray(this, m_LayersArray);
	if (iItemIndex != -1)
	{
		// get the object from the array
		m_LayersObj = &(m_LayersArray->Item(iItemIndex));

		// transfert the data obj to the dialog, data will be 
		// filled during DataTransfer...
		((ProjectDefLayersDlg*)m_pDialog)->SetMemoryLayersObject(m_LayersObj);	
	}
	
}

void ProjectDefList::AfterEditing (bool bRealyEdited)
{
	wxArrayString myListValues;
	
	if (bRealyEdited)
	{
		// modify data to the array is automatically done using DataTransfert
		wxLogDebug(_T("Size of the Spatial array : %d"), m_LayersArray->GetCount());
		
		// modify data to the list
		// prepare data for list representation
		myListValues.Add(m_LayersObj->m_LayerName);
		myListValues.Add(PRJDEF_LAYERS_TYPE_STRING[m_LayersObj->m_LayerType]);
		EditDataToList(myListValues, GetSelectedItem());
		
	}

	// delete dialog
    wxLogDebug(_T("Destroying Thematic Layer Dialog"));
	delete m_LayersDialog;
	
}



void ProjectDefList::BeforeDeleting()
{
	// remove item from array before removing it from the list
	// because of the unknown position of item (may have been moved)
	// if a corresponding item was found, remove it from the array
	int iItemIndex = FindObjInLayersArray(this, m_LayersArray);
	if ( iItemIndex != -1)
	{
		m_LayersArray->RemoveAt(iItemIndex);
	}	
}




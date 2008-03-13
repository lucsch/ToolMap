/***************************************************************************
						objectdefinitionlistdlg.cpp
	Contain description of class used for ProjectEditObjectDefinitionDLG
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

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "objectdefinitionlistdlg.h"


/******************************** EDIT OBJECT DEFINITION DLG ************************/
IMPLEMENT_DYNAMIC_CLASS( ObjectDefinitionListDlg, wxDialog )


BEGIN_EVENT_TABLE( ObjectDefinitionListDlg, wxDialog )
END_EVENT_TABLE()


ObjectDefinitionListDlg::ObjectDefinitionListDlg()
{
    Init();
}


ObjectDefinitionListDlg::ObjectDefinitionListDlg( wxWindow* parent,
												 int iListlayerType,
												 wxWindowID id, const wxString& caption,
												 const wxPoint& pos, const wxSize& size, long style )
{
    Init();
	// should we use the frequency control
	m_ParentListType = iListlayerType;
	
    Create(parent, id, caption, pos, size, style);
}


bool ObjectDefinitionListDlg::Create( wxWindow* parent,
									 wxWindowID id, const wxString& caption, 
									 const wxPoint& pos, const wxSize& size, long style )
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


ObjectDefinitionListDlg::~ObjectDefinitionListDlg()
{
}


void ObjectDefinitionListDlg::Init()
{
    m_DLGODD_Code = NULL;
    m_DLGODD_Description = NULL;
    m_DLGODD_List_Lyr_Name = NULL;
    m_DLGODD_Frequency = NULL;
}


void ObjectDefinitionListDlg::CreateControls()
{    
    ObjectDefinitionListDlg* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Object properties"));
    wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxVERTICAL);
    itemBoxSizer2->Add(itemStaticBoxSizer3, 0, wxGROW|wxALL, 5);

    wxFlexGridSizer* itemFlexGridSizer4 = new wxFlexGridSizer(3, 2, 0, 0);
    itemFlexGridSizer4->AddGrowableCol(1);
    itemStaticBoxSizer3->Add(itemFlexGridSizer4, 0, wxGROW, 5);

    wxStaticText* itemStaticText5 = new wxStaticText( itemDialog1, wxID_STATIC, _("Code :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText5, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_DLGODD_Code = new wxTextCtrl( itemDialog1, wxID_ANY, _T(""), wxDefaultPosition, wxSize(250, -1), 0 );
    itemFlexGridSizer4->Add(m_DLGODD_Code, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText7 = new wxStaticText( itemDialog1, wxID_STATIC, _("Description :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText7, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    m_DLGODD_Description = new wxTextCtrl( itemDialog1, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    m_DLGODD_Description->SetMaxLength(255);
    itemFlexGridSizer4->Add(m_DLGODD_Description, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticText* itemStaticText9 = new wxStaticText( itemDialog1, wxID_STATIC, _("Layer name :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer4->Add(itemStaticText9, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString m_DLGODD_List_Lyr_NameStrings;
    m_DLGODD_List_Lyr_Name = new wxChoice( itemDialog1, ID_DLGODD_LIST_LYR_NAME, wxDefaultPosition, wxDefaultSize, m_DLGODD_List_Lyr_NameStrings, 0 );
    itemFlexGridSizer4->Add(m_DLGODD_List_Lyr_Name, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

	if (m_ParentListType == LAYER_LINE)
	{
		wxArrayString m_DLGODD_FrequencyStrings;
		m_DLGODD_FrequencyStrings.Add(_("&Frequent"));
		m_DLGODD_FrequencyStrings.Add(_("&Not Frequent"));
		m_DLGODD_Frequency = new wxRadioBox( itemDialog1, ID_DLGODD_FREQUENCY, _("Frequency"), wxDefaultPosition, wxDefaultSize, m_DLGODD_FrequencyStrings, 1, wxRA_SPECIFY_ROWS );
		m_DLGODD_Frequency->SetSelection(0);
		itemStaticBoxSizer3->Add(m_DLGODD_Frequency, 0, wxGROW|wxALL, 5);
		
		itemBoxSizer2->Add(5, 5, 1, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	}

    wxStaticLine* itemStaticLine13 = new wxStaticLine( itemDialog1, wxID_STATIC, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
    itemBoxSizer2->Add(itemStaticLine13, 0, wxGROW|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer14 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer14, 0, wxALIGN_RIGHT|wxALL, 5);
    wxButton* itemButton15 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer14->AddButton(itemButton15);

    wxButton* itemButton16 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton16->SetDefault();
    itemStdDialogButtonSizer14->AddButton(itemButton16);

    itemStdDialogButtonSizer14->Realize();

    // Set validators
    m_DLGODD_Description->SetValidator( wxTextValidator(wxFILTER_NONE, & m_Description) );
}



bool ObjectDefinitionListDlg::TransferDataToWindow()
{
	// called automaticaly before displaying window
	//m_DLGODD_List_Lyr_Name->Append(<#const wxArrayString strings#>)
	
}




/******************************** LIST OF OBJECT DEFINITION  ************************/
ObjectDefinitionList::ObjectDefinitionList(wxWindow * parent,
										   wxWindowID id,
										   PRJDEF_LAYERS_TYPE paneltype,
										   wxArrayString * pColsName, 
										   wxArrayInt * pColsSize,
										   wxSize size) : ListGenReportWithDialog(parent, id, pColsName, pColsSize, size)
{
	m_layertype = paneltype;
}

ObjectDefinitionList::~ObjectDefinitionList()
{
	
}


void ObjectDefinitionList::BeforeAdding()
{
	// create the dialog
	ObjectDefinitionListDlg * myDlg = new ObjectDefinitionListDlg(this, m_layertype);
	SetDialog(myDlg);
}



void ObjectDefinitionList::AfterAdding (bool bRealyAddItem)
{
	
	delete m_pDialog;
}



void ObjectDefinitionList::BeforeEditing ()
{
	// create the dialog
	ObjectDefinitionListDlg * myDlg = new ObjectDefinitionListDlg(this, m_layertype);
	SetDialog(myDlg);
	
}

void ObjectDefinitionList::AfterEditing (bool bRealyEdited)
{
	delete m_pDialog;
}





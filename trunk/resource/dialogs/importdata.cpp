/////////////////////////////////////////////////////////////////////////////
// Name:        importdata.cpp
// Purpose:     
// Author:      Lucien Schreiber
// Modified by: 
// Created:     Fri 24 Aug 14:50:18 2007
// RCS-ID:      
// Copyright:   (c) CREALP 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), Fri 24 Aug 14:50:18 2007

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "importdata.h"

////@begin XPM images
////@end XPM images


/*!
 * Importdata type definition
 */

IMPLEMENT_DYNAMIC_CLASS( Importdata, wxDialog )


/*!
 * Importdata event table definition
 */

BEGIN_EVENT_TABLE( Importdata, wxDialog )

////@begin Importdata event table entries
////@end Importdata event table entries

END_EVENT_TABLE()


/*!
 * Importdata constructors
 */

Importdata::Importdata()
{
    Init();
}

Importdata::Importdata( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * Importdata creator
 */

bool Importdata::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin Importdata creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end Importdata creation
    return true;
}


/*!
 * Importdata destructor
 */

Importdata::~Importdata()
{
////@begin Importdata destruction
////@end Importdata destruction
}


/*!
 * Member initialisation
 */

void Importdata::Init()
{
////@begin Importdata member initialisation
////@end Importdata member initialisation
}


/*!
 * Control creation for Importdata
 */

void Importdata::CreateControls()
{    
////@begin Importdata content construction
    // Generated by DialogBlocks, Thu 18 Oct 10:39:03 2007 (Personal Edition)

    Importdata* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxNotebook* itemNotebook3 = new wxNotebook( itemDialog1, ID_NOTEBOOK6, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );

    wxPanel* itemPanel4 = new wxPanel( itemNotebook3, ID_PANEL14, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    itemPanel4->SetSizer(itemBoxSizer5);

    wxFlexGridSizer* itemFlexGridSizer6 = new wxFlexGridSizer(1, 3, 0, 0);
    itemFlexGridSizer6->AddGrowableCol(1);
    itemBoxSizer5->Add(itemFlexGridSizer6, 0, wxGROW, 5);
    wxStaticText* itemStaticText7 = new wxStaticText( itemPanel4, wxID_STATIC, _("Shapefile :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer6->Add(itemStaticText7, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl8 = new wxTextCtrl( itemPanel4, ID_TEXTCTRL, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer6->Add(itemTextCtrl8, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton9 = new wxButton( itemPanel4, ID_BUTTON, _("..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer6->Add(itemButton9, 0, wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer5->Add(itemBoxSizer10, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText11 = new wxStaticText( itemPanel4, wxID_STATIC, _("Shapefile type :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(itemStaticText11, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT|wxBOTTOM, 5);

    wxStaticText* itemStaticText12 = new wxStaticText( itemPanel4, wxID_STATIC, _("Lines"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer10->Add(itemStaticText12, 0, wxALIGN_CENTER_VERTICAL|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    wxStaticBox* itemStaticBoxSizer13Static = new wxStaticBox(itemPanel4, wxID_ANY, _("Import attributes"));
    wxStaticBoxSizer* itemStaticBoxSizer13 = new wxStaticBoxSizer(itemStaticBoxSizer13Static, wxVERTICAL);
    itemBoxSizer5->Add(itemStaticBoxSizer13, 1, wxGROW|wxALL, 5);
    wxCheckBox* itemCheckBox14 = new wxCheckBox( itemPanel4, ID_CHECKBOX3, _("Import attributes"), wxDefaultPosition, wxDefaultSize, 0 );
    itemCheckBox14->SetValue(false);
    itemStaticBoxSizer13->Add(itemCheckBox14, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    wxArrayString itemListBox15Strings;
    wxListBox* itemListBox15 = new wxListBox( itemPanel4, ID_LISTBOX1, wxDefaultPosition, wxDefaultSize, itemListBox15Strings, wxLB_SINGLE );
    itemStaticBoxSizer13->Add(itemListBox15, 1, wxGROW, 5);

    wxBoxSizer* itemBoxSizer16 = new wxBoxSizer(wxHORIZONTAL);
    itemStaticBoxSizer13->Add(itemBoxSizer16, 0, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText17 = new wxStaticText( itemPanel4, wxID_STATIC, _("Project field :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer16->Add(itemStaticText17, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString itemChoice18Strings;
    wxChoice* itemChoice18 = new wxChoice( itemPanel4, ID_CHOICE18, wxDefaultPosition, wxDefaultSize, itemChoice18Strings, 0 );
    itemBoxSizer16->Add(itemChoice18, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemNotebook3->AddPage(itemPanel4, _("Shapefile"));

    wxPanel* itemPanel19 = new wxPanel( itemNotebook3, ID_PANEL15, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer20 = new wxBoxSizer(wxVERTICAL);
    itemPanel19->SetSizer(itemBoxSizer20);

    wxStaticText* itemStaticText21 = new wxStaticText( itemPanel19, wxID_STATIC, _("Warning : the imported project \nmust contain the exact same \nstructure as the actual one"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer20->Add(itemStaticText21, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer22Static = new wxStaticBox(itemPanel19, wxID_ANY, _("Project path"));
    wxStaticBoxSizer* itemStaticBoxSizer22 = new wxStaticBoxSizer(itemStaticBoxSizer22Static, wxHORIZONTAL);
    itemBoxSizer20->Add(itemStaticBoxSizer22, 1, wxGROW|wxALL, 5);
    wxStaticText* itemStaticText23 = new wxStaticText( itemPanel19, wxID_STATIC, _("Path :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer22->Add(itemStaticText23, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxTextCtrl* itemTextCtrl24 = new wxTextCtrl( itemPanel19, ID_TEXTCTRL11, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer22->Add(itemTextCtrl24, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* itemButton25 = new wxButton( itemPanel19, ID_BUTTON18, _("..."), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer22->Add(itemButton25, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    itemNotebook3->AddPage(itemPanel19, _("ToolMap Project"));

    itemBoxSizer2->Add(itemNotebook3, 1, wxGROW|wxALL, 5);

    wxStdDialogButtonSizer* itemStdDialogButtonSizer26 = new wxStdDialogButtonSizer;

    itemBoxSizer2->Add(itemStdDialogButtonSizer26, 0, wxALIGN_RIGHT|wxALL, 5);
    wxButton* itemButton27 = new wxButton( itemDialog1, wxID_OK, _("&Import"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer26->AddButton(itemButton27);

    wxButton* itemButton28 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer26->AddButton(itemButton28);

    itemStdDialogButtonSizer26->Realize();

////@end Importdata content construction
}


/*!
 * Should we show tooltips?
 */

bool Importdata::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap Importdata::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin Importdata bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end Importdata bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon Importdata::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin Importdata icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end Importdata icon retrieval
}

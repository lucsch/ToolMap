/////////////////////////////////////////////////////////////////////////////
// Name:        batchattribution.cpp
// Purpose:     
// Author:      Lucien Schreiber
// Modified by: 
// Created:     Fri 31 Aug 15:08:30 2007
// RCS-ID:      
// Copyright:   (c) CREALP 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), Fri 31 Aug 15:08:30 2007

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

#include "batchattribution.h"

////@begin XPM images
////@end XPM images


/*!
 * Batchattribution type definition
 */

IMPLEMENT_DYNAMIC_CLASS( Batchattribution, wxDialog )


/*!
 * Batchattribution event table definition
 */

BEGIN_EVENT_TABLE( Batchattribution, wxDialog )

////@begin Batchattribution event table entries
////@end Batchattribution event table entries

END_EVENT_TABLE()


/*!
 * Batchattribution constructors
 */

Batchattribution::Batchattribution()
{
    Init();
}

Batchattribution::Batchattribution( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}


/*!
 * Batchattribution creator
 */

bool Batchattribution::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
////@begin Batchattribution creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end Batchattribution creation
    return true;
}


/*!
 * Batchattribution destructor
 */

Batchattribution::~Batchattribution()
{
////@begin Batchattribution destruction
////@end Batchattribution destruction
}


/*!
 * Member initialisation
 */

void Batchattribution::Init()
{
////@begin Batchattribution member initialisation
////@end Batchattribution member initialisation
}


/*!
 * Control creation for Batchattribution
 */

void Batchattribution::CreateControls()
{    
////@begin Batchattribution content construction
    // Generated by DialogBlocks, Mon 12 Nov 13:59:52 2007 (Personal Edition)

    Batchattribution* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText4 = new wxStaticText( itemDialog1, wxID_STATIC, _("Objects type :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer3->Add(itemStaticText4, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxArrayString itemChoice5Strings;
    itemChoice5Strings.Add(_("Faille"));
    wxChoice* itemChoice5 = new wxChoice( itemDialog1, ID_CHOICE11, wxDefaultPosition, wxDefaultSize, itemChoice5Strings, 0 );
    itemChoice5->SetStringSelection(_("Faille"));
    itemBoxSizer3->Add(itemChoice5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer6Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Fields"));
    wxStaticBoxSizer* itemStaticBoxSizer6 = new wxStaticBoxSizer(itemStaticBoxSizer6Static, wxVERTICAL);
    itemBoxSizer2->Add(itemStaticBoxSizer6, 0, wxGROW|wxALL, 5);

    wxArrayString itemChoice7Strings;
    itemChoice7Strings.Add(_("Statut"));
    wxChoice* itemChoice7 = new wxChoice( itemDialog1, ID_CHOICE12, wxDefaultPosition, wxDefaultSize, itemChoice7Strings, 0 );
    itemChoice7->SetStringSelection(_("Statut"));
    itemStaticBoxSizer6->Add(itemChoice7, 0, wxGROW|wxALL, 5);

    wxStaticBox* itemStaticBoxSizer8Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Values"));
    wxStaticBoxSizer* itemStaticBoxSizer8 = new wxStaticBoxSizer(itemStaticBoxSizer8Static, wxVERTICAL);
    itemBoxSizer2->Add(itemStaticBoxSizer8, 1, wxGROW|wxALL, 5);

    wxRadioButton* itemRadioButton9 = new wxRadioButton( itemDialog1, ID_RADIOBUTTON1, _("Actif"), wxDefaultPosition, wxDefaultSize, 0 );
    itemRadioButton9->SetValue(false);
    itemStaticBoxSizer8->Add(itemRadioButton9, 0, wxGROW|wxALL, 5);

    wxRadioButton* itemRadioButton10 = new wxRadioButton( itemDialog1, ID_RADIOBUTTON2, _("Inactif"), wxDefaultPosition, wxDefaultSize, 0 );
    itemRadioButton10->SetValue(false);
    itemStaticBoxSizer8->Add(itemRadioButton10, 0, wxGROW|wxALL, 5);

    wxRadioButton* itemRadioButton11 = new wxRadioButton( itemDialog1, ID_RADIOBUTTON3, wxGetTranslation(wxString(wxT("Indetermin")) + (wxChar) 0x00E9), wxDefaultPosition, wxDefaultSize, 0 );
    itemRadioButton11->SetValue(false);
    itemStaticBoxSizer8->Add(itemRadioButton11, 0, wxGROW|wxALL, 5);

    wxButton* itemButton12 = new wxButton( itemDialog1, ID_BUTTON25, _("Attribute"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer8->Add(itemButton12, 0, wxALIGN_RIGHT|wxALL, 5);

    wxButton* itemButton13 = new wxButton( itemDialog1, ID_BUTTON26, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer2->Add(itemButton13, 0, wxALIGN_RIGHT|wxALL, 5);

////@end Batchattribution content construction
}


/*!
 * Should we show tooltips?
 */

bool Batchattribution::ShowToolTips()
{
    return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap Batchattribution::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin Batchattribution bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end Batchattribution bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon Batchattribution::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin Batchattribution icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end Batchattribution icon retrieval
}

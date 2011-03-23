/***************************************************************************
 preference_dlg.h
 Display Preference dialog
 -------------------
 copyright            : (C) 2011 CREALP Lucien Schreiber 
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

#include "preference_dlg.h"


PreferenceDLG::PreferenceDLG( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	_CreateControls();
}



PreferenceDLG::~PreferenceDLG()
{
}



void PreferenceDLG::_CreateControls(){
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	wxNotebook* m_notebook2;
	m_notebook2 = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	wxPanel* m_panel3;
	m_panel3 = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	m_UpdateCheckCtrl = new wxCheckBox( m_panel3, wxID_ANY, _("Check for updates at startup"), wxDefaultPosition, wxDefaultSize, 0 );
	m_UpdateCheckCtrl->SetValue(true); 
	bSizer6->Add( m_UpdateCheckCtrl, 0, wxALL, 5 );
	
	wxStaticText* m_staticText7;
	m_staticText7 = new wxStaticText( m_panel3, wxID_ANY, _("Proxy informations:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer6->Add( m_staticText7, 0, wxALL, 5 );
	
	m_ProxyInfoCtrl = new wxTextCtrl( m_panel3, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer6->Add( m_ProxyInfoCtrl, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticText* m_staticText8;
	m_staticText8 = new wxStaticText( m_panel3, wxID_ANY, _("Please use the following form: myproxy.com:8080"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer6->Add( m_staticText8, 0, wxALL, 5 );
	
	m_panel3->SetSizer( bSizer6 );
	m_panel3->Layout();
	bSizer6->Fit( m_panel3 );
	m_notebook2->AddPage( m_panel3, _("Updates"), false );
	
	bSizer5->Add( m_notebook2, 1, wxEXPAND | wxALL, 5 );
	
	wxStdDialogButtonSizer* m_sdbSizer2;
	wxButton* m_sdbSizer2OK;
	wxButton* m_sdbSizer2Cancel;
	m_sdbSizer2 = new wxStdDialogButtonSizer();
	m_sdbSizer2OK = new wxButton( this, wxID_OK );
	m_sdbSizer2->AddButton( m_sdbSizer2OK );
	m_sdbSizer2Cancel = new wxButton( this, wxID_CANCEL );
	m_sdbSizer2->AddButton( m_sdbSizer2Cancel );
	m_sdbSizer2->Realize();
	bSizer5->Add( m_sdbSizer2, 0, wxEXPAND|wxALL, 5 );
	
	this->SetSizer( bSizer5 );
	this->Layout();
	bSizer5->Fit( this );
	
	this->Centre( wxBOTH );
}


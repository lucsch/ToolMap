/***************************************************************************
								tmsymboldlg.cpp
						Display Symbology dialog
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

#include "tmsymboldlg.h"


IMPLEMENT_DYNAMIC_CLASS( tmSymbolDLG, wxDialog )



BEGIN_EVENT_TABLE( tmSymbolDLG, wxDialog )
END_EVENT_TABLE()


tmSymbolDLG::tmSymbolDLG()
{
    Init();
}



tmSymbolDLG::tmSymbolDLG( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}



bool tmSymbolDLG::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );
	
    CreateControlsBasic();
	
	SetSizeHint();
    return true;
}



void tmSymbolDLG::SetSizeHint (){
	GetSizer()->SetSizeHints(this);
}



tmSymbolDLG::~tmSymbolDLG()
{

}


void tmSymbolDLG::Init()
{
	m_SymbolPanel = NULL;
}



void tmSymbolDLG::CreateControlsBasic()
{    
	tmSymbolDLG* itemDialog1 = this;
	
    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);
	
	m_SymbolPanel = new wxPanel(itemDialog1, ID_SYMDLG_PANEL); 
	//m_NoteBook = new wxNotebook( itemDialog1, ID_SYMDLG_NOTEBOOK, 
	//							wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );
	
    //wxPanel* itemPanel4 = new wxPanel( m_NoteBook, ID_SYMDLG_PANEL, 
	//								  wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    //wxBoxSizer* itemBoxSizer5 = new wxBoxSizer(wxVERTICAL);
    //itemPanel4->SetSizer(itemBoxSizer5);
	
    /*m_MetaDataWnd = new wxHtmlWindow( itemPanel4, ID_SYMDLG_INFOTXT,
									 wxDefaultPosition, wxDefaultSize, wxBORDER_SUNKEN  | wxHW_DEFAULT_STYLE);
    itemBoxSizer5->Add(m_MetaDataWnd, 1, wxGROW|wxALL, 5);
	
    m_NoteBook->AddPage(itemPanel4, _("Informations"));*/
	

    //itemBoxSizer2->Add(m_NoteBook, 1, wxGROW|wxALL, 5);
	itemBoxSizer2->Add(m_SymbolPanel, 1, wxGROW|wxALL, 5);
								
    wxStdDialogButtonSizer* itemStdDialogButtonSizer27 = new wxStdDialogButtonSizer;
	
    itemBoxSizer2->Add(itemStdDialogButtonSizer27, 0, wxALIGN_RIGHT|wxALL, 5);
    wxButton* itemButton28 = new wxButton( itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer27->AddButton(itemButton28);
	
    wxButton* itemButton29 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemStdDialogButtonSizer27->AddButton(itemButton29);
	
    itemStdDialogButtonSizer27->Realize();

}



/*void tmSymbolDLG::SetMetaData (const wxString & metadata)
{
	m_MetaDataWnd->SetPage(metadata);
}*/




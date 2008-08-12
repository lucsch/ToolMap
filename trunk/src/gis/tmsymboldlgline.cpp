/***************************************************************************
								tmsymboldlgline.cpp
						Display Line Symbology dialog
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

#include "tmsymboldlgline.h"


IMPLEMENT_DYNAMIC_CLASS( tmSymbolDLGLine, tmSymbolDLG )



BEGIN_EVENT_TABLE( tmSymbolDLGLine, tmSymbolDLG )
END_EVENT_TABLE()


tmSymbolDLGLine::tmSymbolDLGLine()
{
    Init();
}



tmSymbolDLGLine::tmSymbolDLGLine( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    Init();
    Create(parent, id, caption, pos, size, style);
}



bool tmSymbolDLGLine::Create( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    tmSymbolDLG::Create( parent, id, caption, pos, size, style );
	    
	CreateControlsLine();
	
	return true;
}




tmSymbolDLGLine::~tmSymbolDLGLine()
{

}


void tmSymbolDLGLine::Init()
{
}


void tmSymbolDLGLine::CreateControlsLine()
{
	wxString sFunction = wxString::FromAscii(__FUNCTION__);
	wxString sFunctionLineError = wxString::Format( _T("%s line %d : "),
												   sFunction.c_str(), __LINE__); 
	wxString sErrMsg = wxString::Format(_T("%s Undefined m_Notebook"), sFunctionLineError.c_str());
	wxASSERT_MSG(m_NoteBook,sErrMsg);
	
	
	wxPanel* itemPanel7 = new wxPanel( m_NoteBook, ID_SYMDLGL_PANEL, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer8 = new wxBoxSizer(wxVERTICAL);
    itemPanel7->SetSizer(itemBoxSizer8);
	
    wxNotebook* itemNotebook9 = new wxNotebook( itemPanel7, ID_SYMDLGL_NOTEBOOK, wxDefaultPosition, wxDefaultSize, wxBK_DEFAULT );
	
    wxPanel* itemPanel10 = new wxPanel( itemNotebook9, ID_SYMDLGL_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxFlexGridSizer* itemFlexGridSizer11 = new wxFlexGridSizer(3, 2, 0, 0);
    itemFlexGridSizer11->AddGrowableCol(1);
    itemPanel10->SetSizer(itemFlexGridSizer11);
	
    wxStaticText* itemStaticText12 = new wxStaticText( itemPanel10, wxID_STATIC, _("Color :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText12, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    tmColourPickerCtrl * itemButton13 = new tmColourPickerCtrl(itemPanel10, ID_SYMDLGL_COLOR,
															   *wxBLUE);
	itemFlexGridSizer11->Add(itemButton13, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);													   
	
	/*wxColourPickerCtrl * itemButton13 = new wxColourPickerCtrl( itemPanel10, ID_SYMDLGL_COLOR, *wxBLACK, 
	wxDefaultPosition, wxDefaultSize, wxCLRP_USE_TEXTCTRL | wxCLRP_SHOW_LABEL);
    itemFlexGridSizer11->Add(itemButton13, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);*/
	
/*	wxBitmap myBitmap (10,10);
	wxMemoryDC dc;
	dc.SelectObject(myBitmap);
	dc.SetBrush(*wxBLUE_BRUSH);
	dc.DrawRectangle(0, 0, 10, 10);
	dc.SelectObject(wxNullBitmap);
	
	wxBitmapButton * itemButton13 = new wxBitmapButton(itemPanel10, ID_SYMDLGL_COLOR, myBitmap);
	
	
/*	wxButton* itemButton13 = new wxButton( itemPanel10, ID_SYMDLGL_COLOR, _("Button"), wxDefaultPosition,
										  wxDefaultSize, 0 );
	itemButton13->SetForegroundColour(*wxBLUE);
	itemButton13->SetBackgroundColour(*wxBLUE);
	itemButton13->SetOwnForegroundColour(*wxBLUE);*/
	//itemFlexGridSizer11->Add(itemButton13, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText14 = new wxStaticText( itemPanel10, wxID_STATIC, _("Shape :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText14, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxArrayString itemChoice15Strings;
    itemChoice15Strings.Add(_("----------"));
    itemChoice15Strings.Add(_("..............."));
    wxChoice* itemChoice15 = new wxChoice( itemPanel10, ID_CHOICE4, wxDefaultPosition, wxDefaultSize, itemChoice15Strings, 0 );
    itemFlexGridSizer11->Add(itemChoice15, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxStaticText* itemStaticText16 = new wxStaticText( itemPanel10, wxID_STATIC, _("Width :"), wxDefaultPosition, wxDefaultSize, 0 );
    itemFlexGridSizer11->Add(itemStaticText16, 0, wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    wxSpinCtrl* itemSpinCtrl17 = new wxSpinCtrl( itemPanel10, ID_SPINCTRL3, _T("0"), wxDefaultPosition, wxDefaultSize, wxSP_ARROW_KEYS, 0, 100, 0 );
    itemFlexGridSizer11->Add(itemSpinCtrl17, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5);
	
    itemNotebook9->AddPage(itemPanel10, _("Unique"));
	
    wxPanel* itemPanel18 = new wxPanel( itemNotebook9, ID_PANEL12, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer19 = new wxBoxSizer(wxVERTICAL);
    itemPanel18->SetSizer(itemBoxSizer19);
	
    wxStaticText* itemStaticText20 = new wxStaticText( itemPanel18, wxID_STATIC, _("NOT IMPLEMENTED NOW"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer19->Add(itemStaticText20, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    itemNotebook9->AddPage(itemPanel18, _("Discrete"));
	
    wxPanel* itemPanel21 = new wxPanel( itemNotebook9, ID_PANEL13, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
    wxBoxSizer* itemBoxSizer22 = new wxBoxSizer(wxVERTICAL);
    itemPanel21->SetSizer(itemBoxSizer22);
	
    wxStaticText* itemStaticText23 = new wxStaticText( itemPanel21, wxID_STATIC, _("NOT IMPLEMENTED NOW"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer22->Add(itemStaticText23, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);
	
    itemNotebook9->AddPage(itemPanel21, _("Continuous"));
	
    itemBoxSizer8->Add(itemNotebook9, 1, wxGROW|wxALL, 5);
	
    wxStaticBox* itemStaticBoxSizer24Static = new wxStaticBox(itemPanel7, wxID_ANY, _("Transparency"));
    wxStaticBoxSizer* itemStaticBoxSizer24 = new wxStaticBoxSizer(itemStaticBoxSizer24Static, wxHORIZONTAL);
    itemBoxSizer8->Add(itemStaticBoxSizer24, 0, wxGROW|wxALL, 5);
    wxSlider* itemSlider25 = new wxSlider( itemPanel7, ID_SLIDER1, 0, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL );
    itemStaticBoxSizer24->Add(itemSlider25, 1, wxGROW|wxALL, 5);
	
    wxTextCtrl* itemTextCtrl26 = new wxTextCtrl( itemPanel7, ID_TEXTCTRL10, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemStaticBoxSizer24->Add(itemTextCtrl26, 0, wxGROW|wxALL, 5);
	
    m_NoteBook->AddPage(itemPanel7, _("Line Symbology"));
	
	// adapt dialog size to new controls added
	SetSizeHint();
}




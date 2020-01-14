/////////////////////////////////////////////////////////////////////////////
// Name:        editquery.cpp
// Purpose:
// Author:      Lucien Schreiber
// Modified by:
// Created:     Tue 21 Aug 14:08:12 2007
// RCS-ID:
// Copyright:   (c) CREALP 2007
// Licence:
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), Tue 21 Aug 14:08:12 2007

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

#include "editquery.h"

////@begin XPM images
////@end XPM images

/*!
 * EditQuery type definition
 */

IMPLEMENT_DYNAMIC_CLASS(EditQuery, wxDialog)

/*!
 * EditQuery event table definition
 */

BEGIN_EVENT_TABLE(EditQuery, wxDialog)

////@begin EditQuery event table entries
////@end EditQuery event table entries

END_EVENT_TABLE()

/*!
 * EditQuery constructors
 */

EditQuery::EditQuery() {
  Init();
}

EditQuery::EditQuery(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size,
                     long style) {
  Init();
  Create(parent, id, caption, pos, size, style);
}

/*!
 * EditQuery creator
 */

bool EditQuery::Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size,
                       long style) {
  ////@begin EditQuery creation
  SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
  wxDialog::Create(parent, id, caption, pos, size, style);

  CreateControls();
  if (GetSizer()) {
    GetSizer()->SetSizeHints(this);
  }
  Centre();
  ////@end EditQuery creation
  return true;
}

/*!
 * EditQuery destructor
 */

EditQuery::~EditQuery() {
  ////@begin EditQuery destruction
  ////@end EditQuery destruction
}

/*!
 * Member initialisation
 */

void EditQuery::Init() {
  ////@begin EditQuery member initialisation
  ////@end EditQuery member initialisation
}

/*!
 * Control creation for EditQuery
 */

void EditQuery::CreateControls() {
  ////@begin EditQuery content construction
  // Generated by DialogBlocks, Mon 12 Nov 13:55:13 2007 (Personal Edition)

  EditQuery* itemDialog1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer2);

  wxStaticBox* itemStaticBoxSizer3Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Query informations"));
  wxStaticBoxSizer* itemStaticBoxSizer3 = new wxStaticBoxSizer(itemStaticBoxSizer3Static, wxVERTICAL);
  itemBoxSizer2->Add(itemStaticBoxSizer3, 0, wxGROW | wxALL, 5);

  wxFlexGridSizer* itemFlexGridSizer4 = new wxFlexGridSizer(3, 2, 0, 0);
  itemFlexGridSizer4->AddGrowableCol(1);
  itemStaticBoxSizer3->Add(itemFlexGridSizer4, 0, wxGROW, 5);

  wxStaticText* itemStaticText5 =
      new wxStaticText(itemDialog1, wxID_STATIC, _("Name :"), wxDefaultPosition, wxDefaultSize, 0);
  itemFlexGridSizer4->Add(itemStaticText5, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL, 5);

  wxTextCtrl* itemTextCtrl6 = new wxTextCtrl(itemDialog1, ID_TEXTCTRL7, _T(""), wxDefaultPosition, wxDefaultSize, 0);
  itemFlexGridSizer4->Add(itemTextCtrl6, 0, wxGROW | wxALIGN_CENTER_VERTICAL | wxALL, 5);

  wxStaticText* itemStaticText7 =
      new wxStaticText(itemDialog1, wxID_STATIC, _("Remarks :"), wxDefaultPosition, wxDefaultSize, 0);
  itemFlexGridSizer4->Add(itemStaticText7, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL, 5);

  wxTextCtrl* itemTextCtrl8 = new wxTextCtrl(itemDialog1, ID_TEXTCTRL8, _T(""), wxDefaultPosition, wxDefaultSize, 0);
  itemFlexGridSizer4->Add(itemTextCtrl8, 0, wxGROW | wxALIGN_CENTER_VERTICAL | wxALL, 5);

  wxStaticText* itemStaticText9 =
      new wxStaticText(itemDialog1, wxID_STATIC, _("Target :"), wxDefaultPosition, wxDefaultSize, 0);
  itemFlexGridSizer4->Add(itemStaticText9, 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL | wxALL, 5);

  wxArrayString itemChoice10Strings;
  itemChoice10Strings.Add(_("Lines"));
  itemChoice10Strings.Add(_("Point"));
  itemChoice10Strings.Add(_("Polygons"));
  itemChoice10Strings.Add(_("Notes"));
  wxChoice* itemChoice10 =
      new wxChoice(itemDialog1, ID_CHOICE3, wxDefaultPosition, wxDefaultSize, itemChoice10Strings, 0);
  itemChoice10->SetStringSelection(_("Lines"));
  itemFlexGridSizer4->Add(itemChoice10, 0, wxGROW | wxALIGN_CENTER_VERTICAL | wxALL, 5);

  wxStaticBox* itemStaticBoxSizer11Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Query"));
  wxStaticBoxSizer* itemStaticBoxSizer11 = new wxStaticBoxSizer(itemStaticBoxSizer11Static, wxVERTICAL);
  itemBoxSizer2->Add(itemStaticBoxSizer11, 1, wxGROW | wxALL, 5);

  wxBoxSizer* itemBoxSizer12 = new wxBoxSizer(wxHORIZONTAL);
  itemStaticBoxSizer11->Add(itemBoxSizer12, 0, wxGROW | wxALL, 5);

  wxStaticText* itemStaticText13 =
      new wxStaticText(itemDialog1, wxID_STATIC, _("Field :"), wxDefaultPosition, wxDefaultSize, 0);
  itemBoxSizer12->Add(itemStaticText13, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

  wxArrayString itemChoice14Strings;
  itemChoice14Strings.Add(wxGetTranslation(wxString(wxT("d")) + (wxChar)0x00E9 + wxT("pendent de Target...")));
  wxChoice* itemChoice14 =
      new wxChoice(itemDialog1, ID_CHOICE, wxDefaultPosition, wxDefaultSize, itemChoice14Strings, 0);
  itemChoice14->SetStringSelection(wxGetTranslation(wxString(wxT("d")) + (wxChar)0x00E9 + wxT("pendent de Target...")));
  itemBoxSizer12->Add(itemChoice14, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

  wxStaticBox* itemStaticBoxSizer15Static = new wxStaticBox(itemDialog1, wxID_ANY, _("Value(s)"));
  wxStaticBoxSizer* itemStaticBoxSizer15 = new wxStaticBoxSizer(itemStaticBoxSizer15Static, wxVERTICAL);
  itemStaticBoxSizer11->Add(itemStaticBoxSizer15, 1, wxGROW | wxALL, 5);

  wxListCtrl* itemListCtrl16 =
      new wxListCtrl(itemDialog1, ID_LISTCTRL3, wxDefaultPosition, wxSize(100, 100), wxLC_REPORT);
  itemStaticBoxSizer15->Add(itemListCtrl16, 1, wxGROW | wxALL, 5);

  wxStdDialogButtonSizer* itemStdDialogButtonSizer17 = new wxStdDialogButtonSizer;

  itemBoxSizer2->Add(itemStdDialogButtonSizer17, 0, wxALIGN_RIGHT | wxALL, 5);
  wxButton* itemButton18 = new wxButton(itemDialog1, wxID_OK, _("&OK"), wxDefaultPosition, wxDefaultSize, 0);
  itemStdDialogButtonSizer17->AddButton(itemButton18);

  wxButton* itemButton19 = new wxButton(itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0);
  itemStdDialogButtonSizer17->AddButton(itemButton19);

  itemStdDialogButtonSizer17->Realize();

  ////@end EditQuery content construction
}

/*!
 * Should we show tooltips?
 */

bool EditQuery::ShowToolTips() {
  return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap EditQuery::GetBitmapResource(const wxString& name) {
  // Bitmap retrieval
  ////@begin EditQuery bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
  ////@end EditQuery bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon EditQuery::GetIconResource(const wxString& name) {
  // Icon retrieval
  ////@begin EditQuery icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
  ////@end EditQuery icon retrieval
}

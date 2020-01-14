/////////////////////////////////////////////////////////////////////////////
// Name:        complexattribution.cpp
// Purpose:
// Author:      Lucien Schreiber
// Modified by:
// Created:     Tue 28 Aug 17:19:19 2007
// RCS-ID:
// Copyright:   (c) CREALP 2007
// Licence:
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), Tue 28 Aug 17:19:19 2007

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

#include "complexattribution.h"

////@begin XPM images
////@end XPM images

/*!
 * Complexattribution type definition
 */

IMPLEMENT_DYNAMIC_CLASS(Complexattribution, wxDialog)

/*!
 * Complexattribution event table definition
 */

BEGIN_EVENT_TABLE(Complexattribution, wxDialog)

////@begin Complexattribution event table entries
////@end Complexattribution event table entries

END_EVENT_TABLE()

/*!
 * Complexattribution constructors
 */

Complexattribution::Complexattribution() {
  Init();
}

Complexattribution::Complexattribution(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos,
                                       const wxSize& size, long style) {
  Init();
  Create(parent, id, caption, pos, size, style);
}

/*!
 * Complexattribution creator
 */

bool Complexattribution::Create(wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos,
                                const wxSize& size, long style) {
  ////@begin Complexattribution creation
  SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
  wxDialog::Create(parent, id, caption, pos, size, style);

  CreateControls();
  if (GetSizer()) {
    GetSizer()->SetSizeHints(this);
  }
  Centre();
  ////@end Complexattribution creation
  return true;
}

/*!
 * Complexattribution destructor
 */

Complexattribution::~Complexattribution() {
  ////@begin Complexattribution destruction
  ////@end Complexattribution destruction
}

/*!
 * Member initialisation
 */

void Complexattribution::Init() {
  ////@begin Complexattribution member initialisation
  ////@end Complexattribution member initialisation
}

/*!
 * Control creation for Complexattribution
 */

void Complexattribution::CreateControls() {
  ////@begin Complexattribution content construction
  // Generated by DialogBlocks, Mon 12 Nov 14:02:38 2007 (Personal Edition)

  Complexattribution* itemDialog1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer2);

  wxArrayString itemListBox3Strings;
  wxListBox* itemListBox3 =
      new wxListBox(itemDialog1, ID_LISTBOX3, wxDefaultPosition, wxDefaultSize, itemListBox3Strings, wxLB_SINGLE);
  itemBoxSizer2->Add(itemListBox3, 1, wxGROW | wxALL, 5);

  wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer4, 0, wxGROW | wxALL, 5);

  wxButton* itemButton5 = new wxButton(itemDialog1, wxID_CLOSE, _("&Close"), wxDefaultPosition, wxDefaultSize, 0);
  itemBoxSizer4->Add(itemButton5, 0, wxALIGN_CENTER_VERTICAL | wxALL, 5);

  wxButton* itemButton6 = new wxButton(itemDialog1, ID_BUTTON21, _("Attribute"), wxDefaultPosition, wxDefaultSize, 0);
  itemBoxSizer4->Add(itemButton6, 1, wxALIGN_CENTER_VERTICAL | wxALL, 5);

  ////@end Complexattribution content construction
}

/*!
 * Should we show tooltips?
 */

bool Complexattribution::ShowToolTips() {
  return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap Complexattribution::GetBitmapResource(const wxString& name) {
  // Bitmap retrieval
  ////@begin Complexattribution bitmap retrieval
  wxUnusedVar(name);
  return wxNullBitmap;
  ////@end Complexattribution bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon Complexattribution::GetIconResource(const wxString& name) {
  // Icon retrieval
  ////@begin Complexattribution icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
  ////@end Complexattribution icon retrieval
}

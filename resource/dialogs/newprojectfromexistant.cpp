/////////////////////////////////////////////////////////////////////////////
// Name:        newprojectfromexistant.cpp
// Purpose:
// Author:      Lucien Schreiber
// Modified by:
// Created:     Wed 29 Aug 08:44:12 2007
// RCS-ID:
// Copyright:   (c) CREALP 2007
// Licence:
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), Wed 29 Aug 08:44:12 2007

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

#include "newprojectfromexistant.h"

////@begin XPM images
////@end XPM images

/*!
 * Newprojectfromexistant type definition
 */

IMPLEMENT_DYNAMIC_CLASS(Newprojectfromexistant, wxWizard)

/*!
 * Newprojectfromexistant event table definition
 */

BEGIN_EVENT_TABLE(Newprojectfromexistant, wxWizard)

////@begin Newprojectfromexistant event table entries
////@end Newprojectfromexistant event table entries

END_EVENT_TABLE()

/*!
 * Newprojectfromexistant constructors
 */

Newprojectfromexistant::Newprojectfromexistant() {
  Init();
}

Newprojectfromexistant::Newprojectfromexistant(wxWindow* parent, wxWindowID id, const wxPoint& pos) {
  Init();
  Create(parent, id, pos);
}

/*!
 * Newprojectfromexistant creator
 */

bool Newprojectfromexistant::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos) {
  ////@begin Newprojectfromexistant creation
  SetExtraStyle(wxWIZARD_EX_HELPBUTTON);
  wxBitmap wizardBitmap(GetBitmapResource(wxT("wizard.png")));
  wxWizard::Create(parent, id, _("New project from existant"), wizardBitmap, pos,
                   wxDEFAULT_DIALOG_STYLE | wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCLOSE_BOX);

  CreateControls();
  ////@end Newprojectfromexistant creation
  return true;
}

/*!
 * Newprojectfromexistant destructor
 */

Newprojectfromexistant::~Newprojectfromexistant() {
  ////@begin Newprojectfromexistant destruction
  ////@end Newprojectfromexistant destruction
}

/*!
 * Member initialisation
 */

void Newprojectfromexistant::Init() {
  ////@begin Newprojectfromexistant member initialisation
  ////@end Newprojectfromexistant member initialisation
}

/*!
 * Control creation for Newprojectfromexistant
 */

void Newprojectfromexistant::CreateControls() {
  ////@begin Newprojectfromexistant content construction
  // Generated by DialogBlocks, Wed 29 Aug 08:44:39 2007 (Personal Edition)

  wxWizard* itemWizard1 = this;

  wxWizardPageSimple* lastPage = NULL;
  ////@end Newprojectfromexistant content construction
}

/*!
 * Runs the wizard.
 */

bool Newprojectfromexistant::Run() {
  wxWindowList::compatibility_iterator node = GetChildren().GetFirst();
  while (node) {
    wxWizardPage* startPage = wxDynamicCast(node->GetData(), wxWizardPage);
    if (startPage) return RunWizard(startPage);
    node = node->GetNext();
  }
  return false;
}

/*!
 * Should we show tooltips?
 */

bool Newprojectfromexistant::ShowToolTips() {
  return true;
}

/*!
 * Get bitmap resources
 */

wxBitmap Newprojectfromexistant::GetBitmapResource(const wxString& name) {
  // Bitmap retrieval
  ////@begin Newprojectfromexistant bitmap retrieval
  wxUnusedVar(name);
  if (name == _T("wizard.png")) {
    wxBitmap bitmap(_T("wizard.png"), wxBITMAP_TYPE_PNG);
    return bitmap;
  }
  return wxNullBitmap;
  ////@end Newprojectfromexistant bitmap retrieval
}

/*!
 * Get icon resources
 */

wxIcon Newprojectfromexistant::GetIconResource(const wxString& name) {
  // Icon retrieval
  ////@begin Newprojectfromexistant icon retrieval
  wxUnusedVar(name);
  return wxNullIcon;
  ////@end Newprojectfromexistant icon retrieval
}

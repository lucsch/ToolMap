/////////////////////////////////////////////////////////////////////////////
// Name:        editquery.h
// Purpose:
// Author:      Lucien Schreiber
// Modified by:
// Created:     Tue 21 Aug 14:08:12 2007
// RCS-ID:
// Copyright:   (c) CREALP 2007
// Licence:
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), Tue 21 Aug 14:08:12 2007

#ifndef _EDITQUERY_H_
#define _EDITQUERY_H_

/*!
 * Includes
 */

////@begin includes
#include "wx/listctrl.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_EDITQUERY 10066
#define ID_TEXTCTRL7 10067
#define ID_TEXTCTRL8 10068
#define ID_CHOICE3 10069
#define ID_CHOICE 10000
#define ID_LISTCTRL3 10070
#define SYMBOL_EDITQUERY_STYLE wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCLOSE_BOX
#define SYMBOL_EDITQUERY_TITLE _("Edit Query")
#define SYMBOL_EDITQUERY_IDNAME ID_EDITQUERY
#define SYMBOL_EDITQUERY_SIZE wxSize(400, 300)
#define SYMBOL_EDITQUERY_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * EditQuery class declaration
 */

class EditQuery : public wxDialog {
  DECLARE_DYNAMIC_CLASS(EditQuery)
  DECLARE_EVENT_TABLE()

 public:
  /// Constructors
  EditQuery();
  EditQuery(wxWindow* parent, wxWindowID id = SYMBOL_EDITQUERY_IDNAME, const wxString& caption = SYMBOL_EDITQUERY_TITLE,
            const wxPoint& pos = SYMBOL_EDITQUERY_POSITION, const wxSize& size = SYMBOL_EDITQUERY_SIZE,
            long style = SYMBOL_EDITQUERY_STYLE);

  /// Creation
  bool Create(wxWindow* parent, wxWindowID id = SYMBOL_EDITQUERY_IDNAME,
              const wxString& caption = SYMBOL_EDITQUERY_TITLE, const wxPoint& pos = SYMBOL_EDITQUERY_POSITION,
              const wxSize& size = SYMBOL_EDITQUERY_SIZE, long style = SYMBOL_EDITQUERY_STYLE);

  /// Destructor
  ~EditQuery();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  ////@begin EditQuery event handler declarations

  ////@end EditQuery event handler declarations

  ////@begin EditQuery member function declarations

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource(const wxString& name);

  /// Retrieves icon resources
  wxIcon GetIconResource(const wxString& name);
  ////@end EditQuery member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

  ////@begin EditQuery member variables
  ////@end EditQuery member variables
};

#endif
// _EDITQUERY_H_

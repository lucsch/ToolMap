/////////////////////////////////////////////////////////////////////////////
// Name:        selectadvancedshortcutvalues.h
// Purpose:
// Author:      Lucien Schreiber
// Modified by:
// Created:     Fri 28 Sep 14:54:04 2007
// RCS-ID:
// Copyright:   (c) CREALP 2007
// Licence:
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), Fri 28 Sep 14:54:04 2007

#ifndef _SELECTADVANCEDSHORTCUTVALUES_H_
#define _SELECTADVANCEDSHORTCUTVALUES_H_

/*!
 * Includes
 */

////@begin includes
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
#define ID_SELECTADVANCEDSHORTCUTVALUES 10199
#define ID_LISTBOX5 10200
#define ID_BUTTON31 10201
#define ID_BUTTON32 10202
#define SYMBOL_SELECTADVANCEDSHORTCUTVALUES_STYLE wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCLOSE_BOX
#define SYMBOL_SELECTADVANCEDSHORTCUTVALUES_TITLE _("Object attributes shortcut")
#define SYMBOL_SELECTADVANCEDSHORTCUTVALUES_IDNAME ID_SELECTADVANCEDSHORTCUTVALUES
#define SYMBOL_SELECTADVANCEDSHORTCUTVALUES_SIZE wxSize(400, 300)
#define SYMBOL_SELECTADVANCEDSHORTCUTVALUES_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Selectadvancedshortcutvalues class declaration
 */

class Selectadvancedshortcutvalues : public wxDialog {
  DECLARE_DYNAMIC_CLASS(Selectadvancedshortcutvalues)
  DECLARE_EVENT_TABLE()

 public:
  /// Constructors
  Selectadvancedshortcutvalues();
  Selectadvancedshortcutvalues(wxWindow* parent, wxWindowID id = SYMBOL_SELECTADVANCEDSHORTCUTVALUES_IDNAME,
                               const wxString& caption = SYMBOL_SELECTADVANCEDSHORTCUTVALUES_TITLE,
                               const wxPoint& pos = SYMBOL_SELECTADVANCEDSHORTCUTVALUES_POSITION,
                               const wxSize& size = SYMBOL_SELECTADVANCEDSHORTCUTVALUES_SIZE,
                               long style = SYMBOL_SELECTADVANCEDSHORTCUTVALUES_STYLE);

  /// Creation
  bool Create(wxWindow* parent, wxWindowID id = SYMBOL_SELECTADVANCEDSHORTCUTVALUES_IDNAME,
              const wxString& caption = SYMBOL_SELECTADVANCEDSHORTCUTVALUES_TITLE,
              const wxPoint& pos = SYMBOL_SELECTADVANCEDSHORTCUTVALUES_POSITION,
              const wxSize& size = SYMBOL_SELECTADVANCEDSHORTCUTVALUES_SIZE,
              long style = SYMBOL_SELECTADVANCEDSHORTCUTVALUES_STYLE);

  /// Destructor
  ~Selectadvancedshortcutvalues();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  ////@begin Selectadvancedshortcutvalues event handler declarations

  ////@end Selectadvancedshortcutvalues event handler declarations

  ////@begin Selectadvancedshortcutvalues member function declarations

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource(const wxString& name);

  /// Retrieves icon resources
  wxIcon GetIconResource(const wxString& name);
  ////@end Selectadvancedshortcutvalues member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

  ////@begin Selectadvancedshortcutvalues member variables
  ////@end Selectadvancedshortcutvalues member variables
};

#endif
// _SELECTADVANCEDSHORTCUTVALUES_H_

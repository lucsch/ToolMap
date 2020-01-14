/////////////////////////////////////////////////////////////////////////////
// Name:        batchattribution.h
// Purpose:
// Author:      Lucien Schreiber
// Modified by:
// Created:     Fri 31 Aug 15:08:30 2007
// RCS-ID:
// Copyright:   (c) CREALP 2007
// Licence:
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), Fri 31 Aug 15:08:30 2007

#ifndef _BATCHATTRIBUTION_H_
#define _BATCHATTRIBUTION_H_

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
#define ID_BATCHATTRIBUTION 10137
#define ID_CHOICE11 10138
#define ID_CHOICE12 10168
#define ID_RADIOBUTTON1 10169
#define ID_RADIOBUTTON2 10170
#define ID_RADIOBUTTON3 10171
#define ID_BUTTON25 10172
#define ID_BUTTON26 10173
#define SYMBOL_BATCHATTRIBUTION_STYLE wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCLOSE_BOX
#define SYMBOL_BATCHATTRIBUTION_TITLE _("Object attribute (batch)")
#define SYMBOL_BATCHATTRIBUTION_IDNAME ID_BATCHATTRIBUTION
#define SYMBOL_BATCHATTRIBUTION_SIZE wxSize(400, 300)
#define SYMBOL_BATCHATTRIBUTION_POSITION wxDefaultPosition
////@end control identifiers

/*!
 * Batchattribution class declaration
 */

class Batchattribution : public wxDialog {
  DECLARE_DYNAMIC_CLASS(Batchattribution)
  DECLARE_EVENT_TABLE()

 public:
  /// Constructors
  Batchattribution();
  Batchattribution(wxWindow* parent, wxWindowID id = SYMBOL_BATCHATTRIBUTION_IDNAME,
                   const wxString& caption = SYMBOL_BATCHATTRIBUTION_TITLE,
                   const wxPoint& pos = SYMBOL_BATCHATTRIBUTION_POSITION,
                   const wxSize& size = SYMBOL_BATCHATTRIBUTION_SIZE, long style = SYMBOL_BATCHATTRIBUTION_STYLE);

  /// Creation
  bool Create(wxWindow* parent, wxWindowID id = SYMBOL_BATCHATTRIBUTION_IDNAME,
              const wxString& caption = SYMBOL_BATCHATTRIBUTION_TITLE,
              const wxPoint& pos = SYMBOL_BATCHATTRIBUTION_POSITION, const wxSize& size = SYMBOL_BATCHATTRIBUTION_SIZE,
              long style = SYMBOL_BATCHATTRIBUTION_STYLE);

  /// Destructor
  ~Batchattribution();

  /// Initialises member variables
  void Init();

  /// Creates the controls and sizers
  void CreateControls();

  ////@begin Batchattribution event handler declarations

  ////@end Batchattribution event handler declarations

  ////@begin Batchattribution member function declarations

  /// Retrieves bitmap resources
  wxBitmap GetBitmapResource(const wxString& name);

  /// Retrieves icon resources
  wxIcon GetIconResource(const wxString& name);
  ////@end Batchattribution member function declarations

  /// Should we show tooltips?
  static bool ShowToolTips();

  ////@begin Batchattribution member variables
  ////@end Batchattribution member variables
};

#endif
// _BATCHATTRIBUTION_H_

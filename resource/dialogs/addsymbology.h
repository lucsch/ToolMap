/////////////////////////////////////////////////////////////////////////////
// Name:        addsymbology.h
// Purpose:     
// Author:      Lucien Schreiber
// Modified by: 
// Created:     Fri 24 Aug 11:02:21 2007
// RCS-ID:      
// Copyright:   (c) CREALP 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), Fri 24 Aug 11:02:21 2007

#ifndef _ADDSYMBOLOGY_H_
#define _ADDSYMBOLOGY_H_

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
#define ID_ADDSYMBOLOGY 10098
#define ID_COMBOBOX1 10099
#define ID_BUTTON17 10100
#define ID_CHECKBOX1 10101
#define SYMBOL_ADDSYMBOLOGY_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_ADDSYMBOLOGY_TITLE _("Add Symbology")
#define SYMBOL_ADDSYMBOLOGY_IDNAME ID_ADDSYMBOLOGY
#define SYMBOL_ADDSYMBOLOGY_SIZE wxSize(400, 300)
#define SYMBOL_ADDSYMBOLOGY_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * AddSymbology class declaration
 */

class AddSymbology: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( AddSymbology )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    AddSymbology();
    AddSymbology( wxWindow* parent, wxWindowID id = SYMBOL_ADDSYMBOLOGY_IDNAME, const wxString& caption = SYMBOL_ADDSYMBOLOGY_TITLE, const wxPoint& pos = SYMBOL_ADDSYMBOLOGY_POSITION, const wxSize& size = SYMBOL_ADDSYMBOLOGY_SIZE, long style = SYMBOL_ADDSYMBOLOGY_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_ADDSYMBOLOGY_IDNAME, const wxString& caption = SYMBOL_ADDSYMBOLOGY_TITLE, const wxPoint& pos = SYMBOL_ADDSYMBOLOGY_POSITION, const wxSize& size = SYMBOL_ADDSYMBOLOGY_SIZE, long style = SYMBOL_ADDSYMBOLOGY_STYLE );

    /// Destructor
    ~AddSymbology();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin AddSymbology event handler declarations

////@end AddSymbology event handler declarations

////@begin AddSymbology member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end AddSymbology member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin AddSymbology member variables
////@end AddSymbology member variables
};

#endif
    // _ADDSYMBOLOGY_H_
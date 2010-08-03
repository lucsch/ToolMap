/////////////////////////////////////////////////////////////////////////////
// Name:        importpartialdatabase.h
// Purpose:     
// Author:      Lucien Schreiber
// Modified by: 
// Created:     Tue  9 Oct 09:11:47 2007
// RCS-ID:      
// Copyright:   (c) CREALP 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), Tue 9 Oct 09:11:47 2007

#ifndef _IMPORTPARTIALDATABASE_H_
#define _IMPORTPARTIALDATABASE_H_

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
#define ID_IMPORTPARTIALDATABASE 10227
#define ID_CHOICE15 10228
#define ID_TEXTCTRL22 10229
#define ID_BUTTON35 10230
#define ID_CHECKBOX8 10231
#define SYMBOL_IMPORTPARTIALDATABASE_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_IMPORTPARTIALDATABASE_TITLE _("Import partial database")
#define SYMBOL_IMPORTPARTIALDATABASE_IDNAME ID_IMPORTPARTIALDATABASE
#define SYMBOL_IMPORTPARTIALDATABASE_SIZE wxSize(400, 300)
#define SYMBOL_IMPORTPARTIALDATABASE_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * Importpartialdatabase class declaration
 */

class Importpartialdatabase: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( Importpartialdatabase )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    Importpartialdatabase();
    Importpartialdatabase( wxWindow* parent, wxWindowID id = SYMBOL_IMPORTPARTIALDATABASE_IDNAME, const wxString& caption = SYMBOL_IMPORTPARTIALDATABASE_TITLE, const wxPoint& pos = SYMBOL_IMPORTPARTIALDATABASE_POSITION, const wxSize& size = SYMBOL_IMPORTPARTIALDATABASE_SIZE, long style = SYMBOL_IMPORTPARTIALDATABASE_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_IMPORTPARTIALDATABASE_IDNAME, const wxString& caption = SYMBOL_IMPORTPARTIALDATABASE_TITLE, const wxPoint& pos = SYMBOL_IMPORTPARTIALDATABASE_POSITION, const wxSize& size = SYMBOL_IMPORTPARTIALDATABASE_SIZE, long style = SYMBOL_IMPORTPARTIALDATABASE_STYLE );

    /// Destructor
    ~Importpartialdatabase();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin Importpartialdatabase event handler declarations

////@end Importpartialdatabase event handler declarations

////@begin Importpartialdatabase member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end Importpartialdatabase member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin Importpartialdatabase member variables
////@end Importpartialdatabase member variables
};

#endif
    // _IMPORTPARTIALDATABASE_H_
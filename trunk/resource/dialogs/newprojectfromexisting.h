/////////////////////////////////////////////////////////////////////////////
// Name:        newprojectfromexisting.h
// Purpose:     
// Author:      Lucien Schreiber
// Modified by: 
// Created:     Wed 29 Aug 08:42:46 2007
// RCS-ID:      
// Copyright:   (c) CREALP 2007
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (Personal Edition), Wed 29 Aug 08:42:46 2007

#ifndef _NEWPROJECTFROMEXISTING_H_
#define _NEWPROJECTFROMEXISTING_H_

/*!
 * Includes
 */

////@begin includes
#include "wx/statline.h"
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
#define ID_NEWPROJECTFROMEXISTING 10036
#define ID_TEXTCTRL15 10149
#define ID_BUTTON22 10150
#define ID_TEXTCTRL16 10151
#define ID_STATICLINE1 10152
#define ID_TEXTCTRL17 10153
#define ID_BUTTON23 10154
#define ID_RADIOBOX1 10155
#define SYMBOL_NEWPROJECTFROMEXISTING_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_NEWPROJECTFROMEXISTING_TITLE _("New project from existing")
#define SYMBOL_NEWPROJECTFROMEXISTING_IDNAME ID_NEWPROJECTFROMEXISTING
#define SYMBOL_NEWPROJECTFROMEXISTING_SIZE wxSize(400, 300)
#define SYMBOL_NEWPROJECTFROMEXISTING_POSITION wxDefaultPosition
////@end control identifiers


/*!
 * Newprojectfromexisting class declaration
 */

class Newprojectfromexisting: public wxDialog
{    
    DECLARE_DYNAMIC_CLASS( Newprojectfromexisting )
    DECLARE_EVENT_TABLE()

public:
    /// Constructors
    Newprojectfromexisting();
    Newprojectfromexisting( wxWindow* parent, wxWindowID id = SYMBOL_NEWPROJECTFROMEXISTING_IDNAME, const wxString& caption = SYMBOL_NEWPROJECTFROMEXISTING_TITLE, const wxPoint& pos = SYMBOL_NEWPROJECTFROMEXISTING_POSITION, const wxSize& size = SYMBOL_NEWPROJECTFROMEXISTING_SIZE, long style = SYMBOL_NEWPROJECTFROMEXISTING_STYLE );

    /// Creation
    bool Create( wxWindow* parent, wxWindowID id = SYMBOL_NEWPROJECTFROMEXISTING_IDNAME, const wxString& caption = SYMBOL_NEWPROJECTFROMEXISTING_TITLE, const wxPoint& pos = SYMBOL_NEWPROJECTFROMEXISTING_POSITION, const wxSize& size = SYMBOL_NEWPROJECTFROMEXISTING_SIZE, long style = SYMBOL_NEWPROJECTFROMEXISTING_STYLE );

    /// Destructor
    ~Newprojectfromexisting();

    /// Initialises member variables
    void Init();

    /// Creates the controls and sizers
    void CreateControls();

////@begin Newprojectfromexisting event handler declarations

////@end Newprojectfromexisting event handler declarations

////@begin Newprojectfromexisting member function declarations

    /// Retrieves bitmap resources
    wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    wxIcon GetIconResource( const wxString& name );
////@end Newprojectfromexisting member function declarations

    /// Should we show tooltips?
    static bool ShowToolTips();

////@begin Newprojectfromexisting member variables
////@end Newprojectfromexisting member variables
};

#endif
    // _NEWPROJECTFROMEXISTING_H_

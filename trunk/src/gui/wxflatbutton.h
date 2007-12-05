/***************************************************************************
								wxflatbutton.h
                    Creation of a flat button on every plateform
					uses wxButton flat or wxToggleButton (on mac)
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


#ifndef WXFLATBUTTON_H
#define WXFLATBUTTON_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/tglbtn.h>


#define wxFLATBUTTON_STYLE wxBORDER_NONE 
#define wxFLATBUTTON_SIZE wxSize(40,-1)
#define wxFLATBUTTON_TEXT_ADD _T("+")
#define wxFLATBUTTON_TEXT_REMOVE _T("-")


#if (__WXMAC__)
class wxFlatButton : public wxToggleButton
	{
		private:
		DECLARE_DYNAMIC_CLASS(wxFlatButton)
		void InitwxFlatButton();
	
		public:
			wxFlatButton();
			wxFlatButton(wxWindow * parent, wxWindowID id, const wxString & label = wxEmptyString);
			~wxFlatButton();
	};
#else
class wxFlatButton : public wxButton
	{
		private:
		DECLARE_DYNAMIC_CLASS(wxFlatButton)
		void InitwxFlatButton();
	
		public:
			wxFlatButton();
			wxFlatButton(wxWindow * parent, wxWindowID id, const wxString & label = wxEmptyString);
			~wxFlatButton();
	};
#endif






#endif

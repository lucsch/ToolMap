/***************************************************************************
								aattribbatch_dlg.h
						Display dialog for batch advanced attribution
                             -------------------
    copyright            : (C) 2010 CREALP Lucien Schreiber 
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

#ifndef _AATTRIB_BATCH_DLG_H_
#define _AATTRIB_BATCH_DLG_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include <wx/statline.h>


///////////////////////////////////////////////////////////////////////////

#define ID_DLGAATB_LIST_TYPE 22450
#define ID_DLGAATB_LIST_FIELDS 22451

///////////////////////////////////////////////////////////////////////////////
/// Class AAttribBatch_DLG
///////////////////////////////////////////////////////////////////////////////
class AAttribBatch_DLG : public wxDialog 
{
	DECLARE_EVENT_TABLE()
private:
	wxListBox* m_ListType;
	wxListBox* m_ListFields;
	wxPanel* m_PanelValue;
	wxStdDialogButtonSizer* m_Btn;
	wxButton* m_BtnSave;
	wxButton* m_BtnApply;
	wxButton* m_BtnCancel;

	void OnTypeChange( wxCommandEvent& event ) { event.Skip(); }
	void OnFieldsChange( wxCommandEvent& event ) { event.Skip(); }
	void OnApply( wxCommandEvent& event ) { event.Skip(); }
	void OnSave( wxCommandEvent& event ) { event.Skip(); }


public:

	AAttribBatch_DLG( wxWindow* parent, wxWindowID id = wxID_ANY, 
		const wxString& title = wxT("Advanced batch attribution"), 
		const wxPoint& pos = wxDefaultPosition, 
		const wxSize& size = wxSize( 400,260 ),
		long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
	~AAttribBatch_DLG();

};

#endif 
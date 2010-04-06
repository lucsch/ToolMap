/***************************************************************************
 information_dlg.h
 Display informations dialog for layers and features
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

#include "information_dlg.h"
#include "../gis/tmtocctrl.h"
#include "../gis/tmselecteddatamemory.h"
#include "../gis/tmmanagerevent.h"

#include "../gis/tmgisdata.h"
#include <wx/grid.h>



BEGIN_EVENT_TABLE(InformationDLG, wxDialog)
	EVT_CLOSE(InformationDLG::OnCloseDlg)
END_EVENT_TABLE();


void InformationDLG::_CreateControls() {
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer22;
	bSizer22 = new wxBoxSizer( wxVERTICAL );
	
	wxNotebook * m_notebook = new wxNotebook( this, wxID_ANY, 
											 wxDefaultPosition, wxDefaultSize, 0 );
	
	wxPanel* m_panel5;
	m_panel5 = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition,
						   wxDefaultSize, wxTAB_TRAVERSAL );
	
	wxBoxSizer* bSizer26;
	bSizer26 = new wxBoxSizer( wxVERTICAL );
	
	m_SelCtrl = new tmSelectionInfoCtrl(m_panel5, wxID_ANY, m_Selected);
	bSizer26->Add( m_SelCtrl, 1, wxEXPAND, 5 );
	
	m_panel5->SetSizer( bSizer26 );
	m_panel5->Layout();
	bSizer26->Fit( m_panel5 );

	
	m_notebook->AddPage( m_panel5, _("Feature"), false );
	
	
	
	wxPanel* m_panel4;
	m_panel4 = new wxPanel( m_notebook, wxID_ANY, 
						   wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer23;
	bSizer23 = new wxBoxSizer( wxVERTICAL );
	
	m_LayerCtrl = new tmLayerInfoCtrl( m_panel4, wxID_ANY, wxDefaultPosition, wxSize( 350,250 ));
	bSizer23->Add( m_LayerCtrl, 1, wxEXPAND, 5 );
	
	m_panel4->SetSizer( bSizer23 );
	m_panel4->Layout();
	bSizer23->Fit( m_panel4 );
	m_notebook->AddPage( m_panel4, _("Layer"), false );
	
	
	bSizer22->Add( m_notebook, 1, wxEXPAND | wxALL, 5 );
	
	this->SetSizer( bSizer22 );
	this->Layout();
	bSizer22->Fit( this );
	
	this->Center(wxBOTH);
	
	// Otherwise ESC is hiding the window
	SetEscapeId(wxID_NONE);
}



void InformationDLG::OnCloseDlg(wxCloseEvent & event) {
	Destroy();	
}




InformationDLG::InformationDLG(wxWindow * parent, tmTOCCtrl * toc,
							   tmSelectedDataMemory * sel,
							   wxWindowID id, const wxString & title,
							   const wxPoint & pos, const wxSize & size,
							   long style) : 
wxDialog(parent, id, title, pos, size, style) {
	m_TOC = toc,
	m_Selected = sel;
	
	_CreateControls();
	
	
}



InformationDLG::~InformationDLG() {
}



bool InformationDLG::TransferDataToWindow() {
	UpdateLayer();
	UpdateSelection();
	
	return true;
}



void InformationDLG::UpdateLayer() {
	wxASSERT(m_TOC);
	m_LayerCtrl->Update(m_TOC);
}



void InformationDLG::UpdateSelection() {
	m_SelCtrl->Update();
	
}






/***************************************************************************//**
Layer informations (wxHtmlWindow)
author Lucien Schreiber (c) CREALP 2010
date 01 mars 2010
*******************************************************************************/
tmLayerInfoCtrl::tmLayerInfoCtrl(wxWindow * parent, wxWindowID id,
								 const wxPoint & pos, const wxSize & size):
wxHtmlWindow(parent, id, pos, size){
	
}


tmLayerInfoCtrl::~tmLayerInfoCtrl() {
}



void tmLayerInfoCtrl::Update(tmTOCCtrl * toc) {
	tmLayerProperties * itemProp = toc->GetSelectionLayer();
	if (!itemProp)
	{
		wxLogDebug(_T("No layer selected, select a layer"));
		return;
	}
	
	
	tmGISData * myData = tmGISData::LoadLayer(itemProp);
	if (!myData)
	{
		wxLogError(_("Error loading GIS data for metadata"));
		return;
	}
	wxString myMetaData = myData->GetMetaDataAsHtml();
	SetPage(myMetaData);
}



/***************************************************************************//**
Selection Control
author Lucien Schreiber (c) CREALP 2010
date 02 mars 2010
*******************************************************************************/
BEGIN_EVENT_TABLE(tmSelectionInfoCtrl, wxTreeMultiCtrl)
	EVT_LEFT_UP(tmSelectionInfoCtrl::OnItemLeftClick)
	EVT_RIGHT_UP(tmSelectionInfoCtrl::OnItemRightClick)
	EVT_MENU(ID_POPUP_ONLY_THIS_OBJ, tmSelectionInfoCtrl::OnPopupSelectionThis)
	EVT_MENU(ID_POPUP_REMOVE_FROM_SEL, tmSelectionInfoCtrl::OnPopupSelectionRemove)
	EVT_MENU(ID_POPUP_MOVE_TO, tmSelectionInfoCtrl::OnPopupMove)
	EVT_MENU(ID_POPUP_ZOOM_TO, tmSelectionInfoCtrl::OnPopupZoom)
	EVT_MENU(wxID_COPY, tmSelectionInfoCtrl::OnPopupCopy)
END_EVENT_TABLE()


tmSelectionInfoCtrl::tmSelectionInfoCtrl(wxWindow * window, wxWindowID id,
										 tmSelectedDataMemory * sel,
										 const wxPoint & pos, const wxSize & size, long style) : 
wxTreeMultiCtrl(window, id, pos, size, style){
	wxASSERT(sel);
	m_Selected = sel;
	m_ClickedItemID = wxNOT_FOUND;
	SetBackgroundColour(*wxWHITE);
	m_ParentItem = AddRoot(_("Selected features"), _("Selected features"));
}



tmSelectionInfoCtrl::~tmSelectionInfoCtrl() {
	
}



void tmSelectionInfoCtrl::_DeleteAllInfos(const wxTreeMultiItem & dontdelete) {
	Freeze();
	int myFirstChildCookie = 0;
	int myIterateChildCookie = 0;
	wxTreeMultiItem myParent = GetFirstChild(m_ParentItem, myFirstChildCookie);
	wxTreeMultiItem myChild;
	
	while (1) {
		if (myParent.IsOk() == false) {
			break;
		}
		
		if (myParent != dontdelete) {
			myChild = GetFirstChild(myParent, myIterateChildCookie);
			if (myChild.IsOk() == true){
				DeleteChildren(myParent);
			}
			
			Collapse(myParent, false);
		}
		myParent = GetNextChild(m_ParentItem, myFirstChildCookie);
			
	}
	Thaw();
}


wxMenu * tmSelectionInfoCtrl::_CreatePopupMenu() {

	wxMenu* myMenu = new wxMenu();
	wxMenuItem* m_menuItem61;
	m_menuItem61 = new wxMenuItem( myMenu, ID_POPUP_MOVE_TO, wxString( wxT("Move to") ) , wxEmptyString, wxITEM_NORMAL );
	myMenu->Append( m_menuItem61 );
	
	wxMenuItem* m_menuItem7;
	m_menuItem7 = new wxMenuItem( myMenu, ID_POPUP_ZOOM_TO, wxString( wxT("Zoom to") ) , wxEmptyString, wxITEM_NORMAL );
	myMenu->Append( m_menuItem7 );
	
	myMenu->AppendSeparator();
	
	wxMenuItem* m_menuItem9;
	m_menuItem9 = new wxMenuItem( myMenu, ID_POPUP_REMOVE_FROM_SEL, wxString( wxT("Remove from selection") ) , wxEmptyString, wxITEM_NORMAL );
	myMenu->Append( m_menuItem9 );
	
	wxMenuItem* m_menuItem10;
	m_menuItem10 = new wxMenuItem( myMenu, ID_POPUP_ONLY_THIS_OBJ, wxString( wxT("Select this object only") ) , wxEmptyString, wxITEM_NORMAL );
	myMenu->Append( m_menuItem10 );
	
	myMenu->AppendSeparator();
	
	wxMenuItem* m_menuItem11;
	m_menuItem11 = new wxMenuItem( myMenu, wxID_COPY, wxString( wxT("Copy data to clipboard") ) , wxEmptyString, wxITEM_NORMAL );
	myMenu->Append( m_menuItem11 );
	
	return myMenu;
}


void tmSelectionInfoCtrl::_UpdateSelection() {
	wxWindow * myWindow = wxTheApp->GetTopWindow();
	wxASSERT(myWindow);
	
	// update display
	wxCommandEvent evt(tmEVT_LM_UPDATE, wxID_ANY);
	myWindow->GetEventHandler()->AddPendingEvent(evt);
	
	// update selecction / attribution panel
	wxCommandEvent evt2(tmEVT_SELECTION_DONE, wxID_ANY);
	myWindow->GetEventHandler()->AddPendingEvent(evt2);
}


bool tmSelectionInfoCtrl::_GetItemByMousePos(wxTreeMultiItem & item, const wxPoint & position) {
	int xViewScrolled = 0, yViewScrolled = 0;
	GetViewStart(&xViewScrolled, &yViewScrolled);
	
	int xUnitScroll = 0, yUnitScroll = 0;
	GetScrollPixelsPerUnit(&xUnitScroll, &yUnitScroll);
	
	wxPoint myPosition;	
	myPosition.x = position.x + xViewScrolled * xUnitScroll;
	myPosition.y = position.y + yViewScrolled * yUnitScroll;
	
	
	int myFlags = 0;
	item = HitTest(myPosition, myFlags);
	
	if (item.IsOk() == false) {
		return false;
	}
	
	if (myFlags == wxTMC_HITTEST_WINDOW) {
		return false;
	}
	
	if (item == m_ParentItem) {
		return false;
	}
	
	return true;
}



void tmSelectionInfoCtrl::OnItemLeftClick(wxMouseEvent & event) {
	wxPoint myRealPosition = event.GetPosition();
	m_ClickedItemID = wxNOT_FOUND;
	
	wxTreeMultiItem clickeditem;
	if (_GetItemByMousePos(clickeditem, myRealPosition) == false) {
		return;
	}
	
	int myCookie = 0;
	wxTreeMultiItem myCtrl = GetFirstChild(clickeditem, myCookie);
	if (myCtrl.IsOk() == true) {
		wxLogMessage(_T("Contain a child"));
		
		return;
	}
	
	// TODO: this is temp code.
	wxGrid * myGrid  = new wxGrid(this, -1);
	myGrid->CreateGrid(1,10);
	myGrid->SetRowLabelValue(0, _T(""));
	myGrid->SetRowLabelSize(1);
	myGrid->AutoSize();
	
	
	AppendWindow(clickeditem, myGrid);
	// remove all control with windows
	_DeleteAllInfos(clickeditem);
	
	event.Skip();
}


void tmSelectionInfoCtrl::OnItemRightClick(wxMouseEvent & event) {
	m_ClickedItemID = wxNOT_FOUND;
	
	wxTreeMultiItem myClickedItem;
	if (_GetItemByMousePos(myClickedItem, event.GetPosition()) == false) {
		return;
	}
	
	
	wxLogMessage(myClickedItem.GetName());
	if (myClickedItem.GetName().ToLong(&m_ClickedItemID) == false) {
		wxFAIL;
		return;
	}
	
	PopupMenu(_CreatePopupMenu(), event.GetPosition().x, event.GetPosition().y);
	event.Skip();
}




void tmSelectionInfoCtrl::OnPopupSelectionThis(wxCommandEvent & event) {
	wxASSERT(m_Selected);
	wxASSERT(m_ClickedItemID != wxNOT_FOUND);
	wxLogMessage(_("Selecting item %d"), m_ClickedItemID);
	
	m_Selected->SetSelected(m_ClickedItemID);
	
	_UpdateSelection();

}



void tmSelectionInfoCtrl::OnPopupSelectionRemove(wxCommandEvent & event) {
	wxASSERT(m_Selected);
	wxASSERT(m_ClickedItemID != wxNOT_FOUND);
	wxLogMessage(_("Removing item %d from selection"), m_ClickedItemID);
	
	wxArrayLong myRemoveArray;
	myRemoveArray.Add(m_ClickedItemID);
	m_Selected->Remove(&myRemoveArray);
	
	_UpdateSelection();
}


void tmSelectionInfoCtrl::OnPopupMove(wxCommandEvent & event) {
	wxASSERT(m_Selected);
	wxASSERT(m_ClickedItemID != wxNOT_FOUND);
	
	// Send event to layermanager
	wxWindow * myWindow = wxTheApp->GetTopWindow();
	wxASSERT(myWindow);
	
	wxCommandEvent evt(tmEVT_LM_MOVE_TO_FEATURE, wxID_ANY);
	evt.SetExtraLong(m_ClickedItemID);
	myWindow->GetEventHandler()->AddPendingEvent(evt);
}


void tmSelectionInfoCtrl::OnPopupZoom(wxCommandEvent & event) {
	wxASSERT(m_Selected);
	wxASSERT(m_ClickedItemID != wxNOT_FOUND);
	
	// Send event to layermanager
	wxWindow * myWindow = wxTheApp->GetTopWindow();
	wxASSERT(myWindow);
	
	wxCommandEvent evt(tmEVT_LM_ZOOM_TO_FEATURE, wxID_ANY);
	evt.SetExtraLong(m_ClickedItemID);
	myWindow->GetEventHandler()->AddPendingEvent(evt);
}


void tmSelectionInfoCtrl::OnPopupCopy(wxCommandEvent & event) {
	event.Skip();
}


	


void tmSelectionInfoCtrl::Update() {
	wxASSERT(m_Selected);
	Freeze();
	DeleteChildren(m_ParentItem);
	
	wxArrayLong * mySelVal = m_Selected->GetSelectedValues();
	if (mySelVal == NULL){
		Thaw();
		return;
	}
	
	for (unsigned int i = 0; i<m_Selected->GetCount(); i++) {
		Collapse(AppendNode(m_ParentItem, wxString() << mySelVal->Item(i),
							wxString() << mySelVal->Item(i)), false);
	}
	Thaw();
	
}

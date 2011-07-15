/***************************************************************************
 tmstats.cpp
 -------------------
 copyright            : (C) 2011 CREALP Lucien Schreiber 
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

#include "tmstats.h"
#include "tmstatsevent.h"
#include "../database/database_tm.h"

// event declared in tmstatsevent.h
DEFINE_EVENT_TYPE(tmEVT_STAT_CLICK);
DEFINE_EVENT_TYPE(tmEVT_STAT_ATTRIB);
DEFINE_EVENT_TYPE(tmEVT_STAT_INTERSECTION);


tmStatsData::tmStatsData() {
	Reset();
}



tmStatsData::~tmStatsData() {
}



void tmStatsData::Reset() {
	m_Id = wxNOT_FOUND;
	m_TimeStart = wxInvalidDateTime;
	ResetPartial();
}


void tmStatsData::ResetPartial(){
	// don't clean Id and start time!
	m_NbClick = 0;
	m_NbAttribution = 0;
	m_NbIntersection = 0;
	m_TimeElapsed = wxTimeSpan();
}



bool tmStatsData::IsOk() {
	if (m_TimeStart.IsValid() == false) {
		return false;
	}
	
	if (m_TimeElapsed.IsNull() == true) {
		return false;
	}
	return true;
}










void tmStatsManager::AppendToBuffer(long click, long attrib, long intersection) {
	// TODO: Check that statistics is started here!
	
	m_StatBufferData.m_NbClick = m_StatBufferData.m_NbClick + click;
	m_StatBufferData.m_NbAttribution = m_StatBufferData.m_NbAttribution + attrib;
	m_StatBufferData.m_NbIntersection = m_StatBufferData.m_NbIntersection + intersection;
	
	if (m_StatBufferData.m_NbClick >= m_BufferSize ||
		m_StatBufferData.m_NbAttribution >= m_BufferSize ||
		m_StatBufferData.m_NbIntersection >= m_BufferSize) {
		_FlushBuffer();
	}
}



void tmStatsManager::_FlushBuffer() {
	// TODO: Append actual data to the actual record (record created during StartRecord)
	
	m_StatBufferData.ResetPartial();
}



void tmStatsManager::_StartRecord() {
}



void tmStatsManager::_StopRecord() {
}


/*
void tmStatsManager::OnGetStatsMessage(wxCommandEvent & event) {
	
	int myClick = 0;
	int myAttrib = 0;
	int myIntersection = 0;
	
	wxLogMessage("Event received!!!");
	wxEventType myEvtType = event.GetEventType();
	
	// switch isn't working here because EVT aren't const!
	if(myEvtType == tmEVT_STAT_CLICK){
		myClick = 1;
	} 
	else if (myEvtType == tmEVT_STAT_ATTRIB) {
		myAttrib = 1;
	} 
	else if (myEvtType == tmEVT_STAT_INTERSECTION) {
		myIntersection = 1;
	}
	else {
		wxFAIL;
	}
	
	_AppendToBuffer(myClick, myAttrib, myIntersection);
}
*/


tmStatsManager::tmStatsManager() {
	Create(NULL);
}



tmStatsManager::~tmStatsManager() {
}



void tmStatsManager::Create(DataBaseTM * database) {
	m_Database = database;
	m_StatBufferData.Reset();
	wxASSERT(m_StatBufferData.IsOk() == false);
	m_IsStarted = false;
}



void tmStatsManager::ShowStatsDialog() {
}



















void tmStats_DLG::OnRecordStart(wxCommandEvent & event) {
}



void tmStats_DLG::OnRecordStop(wxCommandEvent & event) {
}



void tmStats_DLG::OnExport(wxCommandEvent & event) {
}



void tmStats_DLG::_CreateControls() {
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );
	
	m_SessionSizerCtrl = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Last session (date)") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 4, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticText* m_staticText1;
	m_staticText1 = new wxStaticText( this, wxID_ANY, _("Mouse clicks:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer1->Add( m_staticText1, 0, wxALL|wxEXPAND, 5 );
	
	m_SessionClickCtrl = new wxStaticText( this, wxID_ANY, _T("-"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SessionClickCtrl->Wrap( -1 );
	fgSizer1->Add( m_SessionClickCtrl, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticText* m_staticText3;
	m_staticText3 = new wxStaticText( this, wxID_ANY, _("Attributions:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer1->Add( m_staticText3, 0, wxALL|wxEXPAND, 5 );
	
	m_SessionAttribCtrl = new wxStaticText( this, wxID_ANY, _T("-"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SessionAttribCtrl->Wrap( -1 );
	fgSizer1->Add( m_SessionAttribCtrl, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticText* m_staticText9;
	m_staticText9 = new wxStaticText( this, wxID_ANY, _("Intersections:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizer1->Add( m_staticText9, 0, wxALL, 5 );
	
	m_SessionIntersectCtrl = new wxStaticText( this, wxID_ANY, _T("-"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SessionIntersectCtrl->Wrap( -1 );
	fgSizer1->Add( m_SessionIntersectCtrl, 0, wxALL, 5 );
	
	wxStaticText* m_staticText11;
	m_staticText11 = new wxStaticText( this, wxID_ANY, _("Elapsed time:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText11->Wrap( -1 );
	fgSizer1->Add( m_staticText11, 0, wxALL, 5 );
	
	m_SessionElapsedTimeCtrl = new wxStaticText( this, wxID_ANY, _T("-"), wxDefaultPosition, wxDefaultSize, 0 );
	m_SessionElapsedTimeCtrl->Wrap( -1 );
	fgSizer1->Add( m_SessionElapsedTimeCtrl, 0, wxALL, 5 );
	
	m_SessionSizerCtrl->Add( fgSizer1, 1, wxEXPAND, 5 );
	
	bSizer2->Add( m_SessionSizerCtrl, 1, wxALL, 5 );
	
	m_TotalSizerCtrl = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Total (2 sessions)") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer11;
	fgSizer11 = new wxFlexGridSizer( 4, 2, 0, 0 );
	fgSizer11->SetFlexibleDirection( wxBOTH );
	fgSizer11->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	wxStaticText* m_staticText12;
	m_staticText12 = new wxStaticText( this, wxID_ANY, _("Mouse clicks:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText12->Wrap( -1 );
	fgSizer11->Add( m_staticText12, 0, wxALL|wxEXPAND, 5 );
	
	m_TotalClickCtrl = new wxStaticText( this, wxID_ANY, _T("-"), wxDefaultPosition, wxDefaultSize, 0 );
	m_TotalClickCtrl->Wrap( -1 );
	fgSizer11->Add( m_TotalClickCtrl, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticText* m_staticText31;
	m_staticText31 = new wxStaticText( this, wxID_ANY, _("Attributions:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText31->Wrap( -1 );
	fgSizer11->Add( m_staticText31, 0, wxALL|wxEXPAND, 5 );
	
	m_TotalAttribCtrl = new wxStaticText( this, wxID_ANY, _T("-"), wxDefaultPosition, wxDefaultSize, 0 );
	m_TotalAttribCtrl->Wrap( -1 );
	fgSizer11->Add( m_TotalAttribCtrl, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticText* m_staticText91;
	m_staticText91 = new wxStaticText( this, wxID_ANY, _("Intersections:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText91->Wrap( -1 );
	fgSizer11->Add( m_staticText91, 0, wxALL, 5 );
	
	m_TotalIntersectCtrl = new wxStaticText( this, wxID_ANY, _T("-"), wxDefaultPosition, wxDefaultSize, 0 );
	m_TotalIntersectCtrl->Wrap( -1 );
	fgSizer11->Add( m_TotalIntersectCtrl, 0, wxALL, 5 );
	
	wxStaticText* m_staticText111;
	m_staticText111 = new wxStaticText( this, wxID_ANY, _("Elapsed time:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText111->Wrap( -1 );
	fgSizer11->Add( m_staticText111, 0, wxALL, 5 );
	
	m_TotalElapsedTimeCtrl = new wxStaticText( this, wxID_ANY, _T("-"), wxDefaultPosition, wxDefaultSize, 0 );
	m_TotalElapsedTimeCtrl->Wrap( -1 );
	fgSizer11->Add( m_TotalElapsedTimeCtrl, 0, wxALL, 5 );
	
	m_TotalSizerCtrl->Add( fgSizer11, 1, wxEXPAND, 5 );
	
	bSizer2->Add( m_TotalSizerCtrl, 1, wxALL, 5 );
	
	bSizer1->Add( bSizer2, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer6;
	sbSizer6 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Recording") ), wxHORIZONTAL );
	
	m_BtnStartCtrl = new wxButton( this, wxID_EXECUTE, _("Start"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer6->Add( m_BtnStartCtrl, 0, wxALL, 5 );
	
	m_BtnStopCtrl = new wxButton( this, wxID_STOP, _("Stop"), wxDefaultPosition, wxDefaultSize, 0 );
	sbSizer6->Add( m_BtnStopCtrl, 0, wxALL, 5 );
	
	bSizer1->Add( sbSizer6, 0, wxEXPAND|wxALL, 5 );
	
	
	bSizer1->Add( 0, 0, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	
	bSizer3->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_BtnExportCtrl = new wxButton( this, wxID_SAVE, _("Export..."), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_BtnExportCtrl, 0, wxALL|wxALIGN_RIGHT, 5 );
	
	m_BtnCloseCtrl = new wxButton( this, wxID_CANCEL, _("Close"), wxDefaultPosition, wxDefaultSize, 0 );
	m_BtnCloseCtrl->SetDefault(); 
	bSizer3->Add( m_BtnCloseCtrl, 0, wxALL, 5 );
	
	bSizer1->Add( bSizer3, 0, wxALL|wxEXPAND|wxALIGN_RIGHT, 5 );
	
	this->SetSizer( bSizer1 );
	this->Layout();
	bSizer1->Fit( this );
	
	this->Centre( wxBOTH );
}




tmStats_DLG::tmStats_DLG(wxWindow * parent, wxWindowID id, const wxString & title,
						 const wxPoint & pos, const wxSize & size, long style) :
wxDialog(parent, id, title, pos, size,style) {
	_CreateControls();

}



tmStats_DLG::~tmStats_DLG(){
}



bool tmStats_DLG::TransferDataToWindow() {
	return true;
}



bool tmStats_DLG::TransferDataFromWindow() {
	return true;
}


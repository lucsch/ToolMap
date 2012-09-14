/***************************************************************************
								toolmap.cpp
				main file for toolmap 2, contain app and main frame
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



#include "toolmap.h"
#include "tmlog.h"
#include "../gui/tmimportwiz.h"
#include "../gis/tmimport.h"
#include "../gui/backupmanager_dlg.h"
#include "backupmanager.h"
#include <wx/textdlg.h>

#if wxUSE_CRASHREPORT
	#include <wx/msw/crashrpt.h>
#endif
#include <wx/stdpaths.h>

#include "../img/misc_bmp.h"
#include "../img/toolbar_bmp.h"
#include "../gui/information_dlg.h"
#include "../gui/newtemplateprjwizard.h"
#include "../gui/preference_dlg.h"
#include "../components/tmupdate/update.h"
#include "lsversion_dlg.h"
#include "lsversion_param.h"
#include "tmstats.h"
#include "tmstatsevent.h"
#include "../gui/attribution_obj_type.h"

#include "pdfdocument.h"
#include "pdfexportwizard.h"
#include "lscrashreport.h"


IMPLEMENT_APP(ToolMapApp);

ToolMapApp::ToolMapApp(){
    wxHandleFatalExceptions();
}


bool ToolMapApp::OnInit(){
#ifdef __WXDEBUG__
    // debugging string for OSX
    // this is needed for viewing string content with Xcode !!
    wxString myTest = _T("Test debugging");
    myTest.ToUTF8().data();
#endif
    
	// clear log if exists
	_RemoveLogFile();

	// add handler for PNG embedded images (toolbar)
	wxImage::AddHandler(new wxPNGHandler);
	ToolMapFrame* frame = new ToolMapFrame(NULL, g_ProgName, wxDefaultPosition, wxSize(900,500),_T("MAIN_WINDOW"));
	tmWindowPosition myPos;
	wxRect myWndPos;
	if (myPos.LoadPosition(frame->GetName(), myWndPos)==true){
		frame->SetPosition(wxPoint(myWndPos.GetX(), myWndPos.GetY()));
		frame->SetSize(wxSize(myWndPos.GetWidth(), myWndPos.GetHeight()));
	}

	frame->Show(true);
	return true;
}


void ToolMapApp::OnFatalException(){
    lsCrashReport myCrashReport (g_ProgName);
    // add mysql report
    wxFileName mySQLReport (wxStandardPaths::Get().GetAppDocumentsDir(),_T("toolmap_mysql_log.sql"));
	if (wxFileExists(mySQLReport.GetFullPath())==true){
        myCrashReport.AddFileToReport(mySQLReport.GetFullPath());
    }
    
    if(myCrashReport.PrepareReport(wxDebugReport::Context_Exception)==false){
        return;
    }
    
    wxConfigBase * myConfig =  wxConfigBase::Get(false);
    wxASSERT(myConfig);
    myConfig->SetPath("UPDATE");
    wxString myProxyInfo = myConfig->Read("proxy_info", wxEmptyString);
    myConfig->SetPath("..");
    
    if (myCrashReport.SendReportWeb(_T("http://www.crealp.ch/crashreport/upload_file.php"), myProxyInfo)==false){
        wxString myDocPath = wxStandardPaths::Get().GetDocumentsDir();
        if(myCrashReport.SaveReportFile(myDocPath)==false){
            wxLogError(_("Unable to save the crashreport!"));
            return;
        }
        wxLogWarning(_("Connection problem! crashreport wasn't sent. crashreport was saved into '%s'\nplease send it manually to lucien.schreiber@crealp.vs.ch"), myDocPath);
    }
}


void ToolMapApp::_RemoveLogFile(){
	wxFileName flog (wxStandardPaths::Get().GetAppDocumentsDir(),_T("toolmap_mysql_log.sql"));
	if (wxFileExists(flog.GetFullPath())==true){
		wxLogDebug(_T("Removing MySQL Log file"));
		wxRemoveFile(flog.GetFullPath());
	}
}

IMPLEMENT_DYNAMIC_CLASS(ToolMapFrame, wxFrame)

BEGIN_EVENT_TABLE (ToolMapFrame, wxFrame)
	EVT_MENU (ID_MENU_NEW_PRJ_EMPTY, ToolMapFrame::OnNewProject)
	EVT_MENU (ID_MENU_NEW_PRJ_EXISTING, ToolMapFrame::OnNewProjectExisting )
	EVT_MENU (ID_MENU_LOG_WINDOW, ToolMapFrame::OnLogWindow)
	EVT_MENU (ID_MENU_TOC_WINDOW, ToolMapFrame::OnTocWindow)
	EVT_MENU (ID_MENU_OPEN_PRJ, ToolMapFrame::OnOpenProject)
	EVT_MENU (ID_MENU_OBJ_DEF, ToolMapFrame::OnEditProjectObjects)
	EVT_MENU (ID_MENU_OBJ_ATTRIB_DEF, ToolMapFrame::OnEditObjectAttributes)
	EVT_MENU (ID_MENU_PRJ_SETTINGS, ToolMapFrame::OnEditProjectSettings )
	EVT_MENU (ID_MENU_PRJ_DEF, ToolMapFrame::OnEditProject)
	EVT_MENU (ID_MENU_PRJ_BACKUP, ToolMapFrame::OnProjectBackup)
    EVT_MENU (ID_MENU_PRJ_BACKUP_MANAGER, ToolMapFrame::OnProjectBackupManage)
	EVT_MENU (ID_MENU_PRJ_SAVE_TEMPLATE, ToolMapFrame::OnProjectSaveTemplate)
	EVT_MENU_RANGE (wxID_FILE1, wxID_FILE5, ToolMapFrame::OnOpenRecentProject)
	EVT_MENU (ID_MENU_ADD_SPATIAL_DATA, ToolMapFrame::OnAddGisData)
	EVT_MENU (ID_MENU_IMPORT_GIS_DATA, ToolMapFrame::OnImportGISData)
	EVT_MENU (ID_MENU_SHORTCUTS, ToolMapFrame::OnShowShortcutWindow)
	EVT_MENU (ID_MENU_ADJUST_SNAPPING, ToolMapFrame::OnShowSnappingWindow)
	EVT_MENU(wxID_EXIT, ToolMapFrame::OnQuit)
	EVT_MENU (wxID_PREFERENCES, ToolMapFrame::OnPreferences)
	EVT_MENU (ID_MENU_EXPORT_MODEL, ToolMapFrame::OnExportProjectModel)
    EVT_MENU(wxID_REFRESH, ToolMapFrame::OnRefreshView)


	// TOOL EVENT
	EVT_MENU (wxID_BACKWARD, ToolMapFrame::OnZoomPrevious)
	EVT_MENU (ID_MENU_ZOOM_FIT, ToolMapFrame::OnToolChanged)
	EVT_MENU (ID_MENU_ZOOM, ToolMapFrame::OnToolChanged)
	EVT_MENU (ID_MENU_PAN, ToolMapFrame::OnToolChanged)
	EVT_MENU (ID_MENU_ZOOM_SELECTED_LAYER, ToolMapFrame::OnZoomToSelectedLayer)
	EVT_MENU (ID_MENU_SELECT, ToolMapFrame::OnToolChanged)
	EVT_MENU (ID_MENU_DRAW, ToolMapFrame::OnToolChanged)
	EVT_MENU (ID_MENU_MODIFY, ToolMapFrame::OnToolChanged)
	EVT_MENU (ID_MENU_CUT_LINES, ToolMapFrame::OnToolChanged)
	EVT_MENU (ID_MENU_ORIENT_POINT, ToolMapFrame::OnToolChanged)
	EVT_MENU (ID_MENU_MODIFY_SHARED, ToolMapFrame::OnToolChanged)


	//EDIT MENU
	EVT_MENU (ID_MENU_DELETE_OBJ, ToolMapFrame::OnEditDeleteSelected)
	EVT_MENU (ID_MENU_UNDO, ToolMapFrame::OnEditUndo)
	EVT_MENU (ID_MENU_CREATE_INTERSECTIONS, ToolMapFrame::OnCreateIntersections)
	EVT_MENU (ID_MENU_EDIT_VERTEX_POS, ToolMapFrame::OnEditVertexPosition)
	EVT_MENU (ID_MENU_MERGE_LINES, ToolMapFrame::OnMergeSelectedLines)
	EVT_MENU (ID_MENU_FLIP_LINE,ToolMapFrame::OnFlipLine)

	//ATTRIBUTION MENU
	EVT_MENU (ID_MENU_ATTRIB_TYPES,ToolMapFrame::OnShowObjectAttributionWindow)
	EVT_MENU (ID_MENU_ATTRIB_ATTRIBUTES, ToolMapFrame::OnShowAAttributionWindow)
	EVT_MENU (ID_MENU_ATTRIB_BATCH, ToolMapFrame::OnAAttributionBatchWindow)

	// EXPORT MENU
	EVT_MENU (ID_MENU_EXPORT_LAYER, ToolMapFrame::OnExportSelected)
	//EVT_MENU (ID_MENU_EXPORT_FULL, ToolMapFrame::OnExportAll)

	//SELECTION MENU
	EVT_MENU (ID_MENU_SELECT_NONE, ToolMapFrame::OnSelectNone)
	EVT_MENU (ID_MENU_SELECT_BY_OID, ToolMapFrame::OnSelectByOid )
	EVT_MENU (ID_MENU_SELECT_INVERSE, ToolMapFrame::OnSelectInvert)

	EVT_MENU (ID_MENU_INFO_WINDOW, ToolMapFrame::OnShowInformationDialog)

	//TOOL MENU
	EVT_MENU (ID_MENU_TOOL_DANGLING, ToolMapFrame::OnDanglingNodes)
	EVT_MENU (ID_MENU_STATISTICS, ToolMapFrame::OnStatisticsDialog)

	EVT_MENU (ID_MENU_QUERIES,  ToolMapFrame::OnShowQueriesWindow)
	// queries event are binded :-)

	EVT_MENU (ID_MENU_CHECK_UPDATE,ToolMapFrame::OnCheckUpdates)
	EVT_MENU (ID_MENU_REPORT_BUG, ToolMapFrame::OnReportBug)
	EVT_MENU (ID_MENU_ASK_NEW_FEATURE, ToolMapFrame::OnReportBug)
	EVT_MENU (ID_MENU_USER_MANUAL, ToolMapFrame::OnUserManual)

	EVT_MENU (wxID_ABOUT, ToolMapFrame::OnAbout)
	EVT_MENU (ID_MENU_COMPONENTS, ToolMapFrame::OnComponentsInfo)

    EVT_MENU(ID_MENU_LAYOUT_DEFAULT, ToolMapFrame::OnLayoutDefault)
    EVT_MENU(ID_MENU_LAYOUT_VERTICAL, ToolMapFrame::OnLayoutVertical)
    EVT_MENU(ID_MENU_LAYOUT_HORIZONTAL, ToolMapFrame::OnLayoutHorizontal)

	// AUI EVENT WHEN PANE CLOSED
	EVT_AUI_PANE_CLOSE (ToolMapFrame::OnCloseManagedPane)
	EVT_CLOSE(ToolMapFrame::OnClose)

	// NOTIFICATION EVENT
	EVT_COMMAND (wxID_ANY, tmEVT_SHORTCUT_ATTRIBUTION_DONE, ToolMapFrame::OnShortcutAttributionDone)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_EDIT_START, ToolMapFrame::OnEditSwitch)
	EVT_COMMAND (wxID_ANY, tmEVT_EM_EDIT_STOP, ToolMapFrame::OnEditSwitch)
	EVT_COMMAND (wxID_ANY, tmEVT_SELECTION_DONE, ToolMapFrame::OnUpdateSelection)

	// STATISTICS EVENT
	EVT_COMMAND (wxID_ANY, tmEVT_STAT_CLICK, ToolMapFrame::OnStatisticsUpdate)
	EVT_COMMAND (wxID_ANY, tmEVT_STAT_ATTRIB, ToolMapFrame::OnStatisticsUpdate)
	EVT_COMMAND (wxID_ANY, tmEVT_STAT_INTERSECTION, ToolMapFrame::OnStatisticsUpdate)


	// UPDATE UI EVENT
	EVT_UPDATE_UI_RANGE(ID_MENU_PRJ_BACKUP, ID_MENU_PRJ_SETTINGS, ToolMapFrame::OnUpdateMenuProject)
    EVT_UPDATE_UI(ID_MENU_EXPORT_MODEL, ToolMapFrame::OnUpdateMenuProject)

	EVT_UPDATE_UI (ID_MENU_ADD_SPATIAL_DATA, ToolMapFrame::OnUpdateMenuProject)
	EVT_UPDATE_UI (ID_MENU_IMPORT_GIS_DATA, ToolMapFrame::OnUpdateMenuProject)
	EVT_UPDATE_UI (ID_MENU_UNLINK_SPATIAL_DATA, ToolMapFrame::OnUpdateMenuProject)

	EVT_UPDATE_UI_RANGE(ID_MENU_ZOOM, ID_MENU_ZOOM_FIT, ToolMapFrame::OnUpdateMenuProject)
	EVT_UPDATE_UI (ID_MENU_ZOOM_SELECTED_LAYER, ToolMapFrame::OnUpdateMenuProject)
	EVT_UPDATE_UI (wxID_BACKWARD, ToolMapFrame::OnUpdateMenuPreviousZoom)

	EVT_UPDATE_UI (ID_MENU_UNDO, ToolMapFrame::OnUpdateMenuEditUndo)
	EVT_UPDATE_UI (ID_MENU_DRAW, ToolMapFrame::OnUpdateMenuEditDraw)
	EVT_UPDATE_UI (ID_MENU_MODIFY, ToolMapFrame::OnUpdateMenuEditModify)
	EVT_UPDATE_UI (ID_MENU_EDIT_VERTEX_POS, ToolMapFrame::OnUpdateMenuEditModify)
	EVT_UPDATE_UI (ID_MENU_CUT_LINES, ToolMapFrame::OnUpdateMenuEditModify)
	EVT_UPDATE_UI (ID_MENU_CREATE_INTERSECTIONS, ToolMapFrame::OnUpdateMenuEditModify)
	EVT_UPDATE_UI (ID_MENU_DELETE_OBJ, ToolMapFrame::OnUpdateMenuEditDelete)
	EVT_UPDATE_UI (ID_MENU_MERGE_LINES, ToolMapFrame::OnUpdateMenuEditMerge)
	EVT_UPDATE_UI (ID_MENU_SHORTCUTS, ToolMapFrame::OnUpdateMenuShowShortcuts)
	EVT_UPDATE_UI (ID_MENU_FLIP_LINE, ToolMapFrame::OnUpdateMenuFlipLine)
	EVT_UPDATE_UI (ID_MENU_MODIFY_SHARED, ToolMapFrame::OnUpdateMenuEditSharedNode)


	EVT_UPDATE_UI (ID_MENU_ATTRIB_ATTRIBUTES, ToolMapFrame::OnUpdateMenuEditModify)
	EVT_UPDATE_UI (ID_MENU_ATTRIB_BATCH, ToolMapFrame::OnUpdateMenuEditDelete)
	EVT_UPDATE_UI (ID_MENU_ADJUST_SNAPPING, ToolMapFrame::OnUpdateMenuShowSnapping)


	EVT_UPDATE_UI (ID_MENU_TOOL_DANGLING, ToolMapFrame::OnUpdateMenuProject)
	EVT_UPDATE_UI (ID_MENU_ORIENT_POINT, ToolMapFrame::OnUpdateMenuEditPointOrient)
	EVT_UPDATE_UI (ID_MENU_STATISTICS, ToolMapFrame::OnUpdateStatisticsDialog)

	EVT_UPDATE_UI_RANGE (ID_MENU_SELECT_BY_OID, ID_MENU_SELECT,ToolMapFrame::OnUpdateMenuProject)
	EVT_UPDATE_UI_RANGE (ID_MENU_SELECT_NONE, ID_MENU_SELECT_INVERSE, ToolMapFrame::OnUpdateMenuEditClearSelection)

	EVT_UPDATE_UI (ID_MENU_QUERIES, ToolMapFrame::OnUpdateMenuShowQuery)
	EVT_UPDATE_UI (ID_QUERIES_RUN, ToolMapFrame::OnUpdateMenuEditQueryRun)
    EVT_UPDATE_UI (ID_QUERIES_REMOVE, ToolMapFrame::OnUpdateMenuEditQueryRun)
    EVT_UPDATE_UI (ID_QUERIES_ADD, ToolMapFrame::OnUpdateMenuEditQueryAdd)

	EVT_UPDATE_UI (ID_MENU_TOC_WINDOW, ToolMapFrame::OnUpdateMenuShowTOC)
	EVT_UPDATE_UI (ID_MENU_LOG_WINDOW, ToolMapFrame::OnUpdateMenuShowLog)
	EVT_UPDATE_UI (ID_MENU_INFO_WINDOW, ToolMapFrame::OnUpdateMenuShowInfo)
END_EVENT_TABLE()



ToolMapFrame::ToolMapFrame()
{

}


/* Frame initialisation */
ToolMapFrame::ToolMapFrame(wxFrame *frame, const wxString& title,wxPoint pos, wxSize size, const wxString & name)
			: wxFrame(frame, wxID_ANY, title,pos,size, wxDEFAULT_FRAME_STYLE, name)
{
    // Generic list settings for using generic list under Mac
	// otherwise some problem may occur with dnd and inserting
	// item
	wxSystemOptions::SetOption( wxT("mac.listctrl.always_use_generic"), 1 );
    setlocale(LC_NUMERIC, "C");

	// initing bitmap
	images_misc_init();
    images_toolbar_init();

	// Loading icon
	wxIcon icon;
	icon.CopyFromBitmap(*_img_icon_toolmap);
	SetIcon(icon);

    // adding status bar
	CreateStatusBar(4, wxSTB_DEFAULT_STYLE, wxID_ANY);

	_CreateMenu();
	_CreateToolBar();
    _CreateAccelerators();

	wxLog * myDlgLog = new tmLogGuiSeverity(wxLOG_Warning);
	delete wxLog::SetActiveTarget(myDlgLog);
	m_LogWindow = new wxLogWindow(this, g_ProgName + _(" Log"), false);

	wxLogDebug(_("Debug mode enabled"));

	wxConfigBase::Set(new wxFileConfig(g_ProgName));
	wxLogDebug("Config file : %s", wxFileConfig::GetLocalFile(g_ProgName).GetFullPath());


	// create the UI
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	wxPanel * mypanel2 = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxWANTS_CHARS);
	bSizer2->Add( mypanel2, 1, wxEXPAND, 5 );

	m_AuiManager = new wxAuiManager(mypanel2);
	m_TocWindow = new TocWindowDlgGen(m_AuiManager, mypanel2);
	// init object attribution panel
	m_MainPanel = new Main_PANEL(mypanel2, m_AuiManager);
	m_AttribObjPanel = new AttribObjType_PANEL(mypanel2, m_AuiManager);
	m_QueriesPanel = new Queries_PANEL(mypanel2,wxID_ANY, m_AuiManager);
	m_ShortCutPanel = new Shortcuts_PANEL(mypanel2, wxID_ANY, m_AuiManager);
	m_SnappingPanel = new Snapping_PANEL(mypanel2, wxID_ANY, m_AuiManager);

    _CreatePerspectives();

	// loading position
	wxString myPosText = wxEmptyString;
	tmWindowPosition myPos;
	if(myPos.LoadPosition(_T("AUI_PANES"), myPosText)==true)
	{
		if (myPos.HasScreenChanged()==false)
			m_AuiManager->LoadPerspective(myPosText, true);
	}

	m_InfoBar = new WebUpdateInformationBar(this);
	bSizer2->Add(m_InfoBar, wxSizerFlags().Expand());
	this->SetSizer( bSizer2 );
	this->Layout();

	// create layer manager object
	m_LayerManager = new tmLayerManager(this, m_TocWindow->GetTOCCtrl(),
										m_MainPanel->GetGISRenderer(),
										GetStatusBar(),
										m_ScaleCombo);

	m_AttribManager = new tmAttributionManager(this,
											   m_TocWindow->GetTOCCtrl(),
											   m_AttribObjPanel,
											   m_LayerManager->GetSelectedDataMemory());

	m_EditManager = new tmEditManager (this,
									   m_TocWindow->GetTOCCtrl(),
									   m_LayerManager->GetSelectedDataMemory(),
									   m_MainPanel->GetGISRenderer(),
									   m_LayerManager->GetScale());

	m_ToolManager = new tmToolManager(this,
									  m_TocWindow->GetTOCCtrl(),
									  m_LayerManager->GetSelectedDataMemory(),
									  m_MainPanel->GetGISRenderer(),
									  m_LayerManager->GetScale());

	m_StatManager = new tmStatsManager();

	// init the menu manager
	m_MManager = new MenuManager(GetMenuBar());
	m_PManager = new ProjectManager(this);
	m_PManager->SetMenuManager(m_MManager);
	m_PManager->GetObjectManager()->SetPanel(m_AttribObjPanel);
	m_PManager->SetStatusBar(GetStatusBar());
	m_PManager->SetLayerManager(m_LayerManager);
	m_PManager->SetAttributionManager(m_AttribManager);
	m_PManager->SetQueriesPanel(m_QueriesPanel);
	m_PManager->SetShortcutPanel(m_ShortCutPanel);
	m_PManager->SetSnappingPanel(m_SnappingPanel);
	m_PManager->SetEditManager(m_EditManager);
	m_PManager->SetToolManager(m_ToolManager);
	m_PManager->SetStatManager(m_StatManager);

	m_QueriesPanel->SetSelectedData(m_LayerManager->GetSelectedDataMemory());
    m_QueriesPanel->SetTOCCtrl(m_TocWindow->GetTOCCtrl());

	// loading GIS drivers
	tmGISData::InitGISDrivers(TRUE, TRUE);

	_CheckUpdates(false);
	_LoadPreference(false);


	// connecting menu to object kind panel
	wxASSERT(m_AttribObjPanel);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, &AttribObjType_PANEL::OnDisplayAttributesAuto, m_AttribObjPanel, ID_CTXT_AUTODISPLAY_ATTRIB);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, &AttribObjType_PANEL::OnEmptyListAffterAttributes, m_AttribObjPanel, ID_CTXT_EMPTY_LIST_AFTER_ATTRIB);
	this->Bind(wxEVT_COMMAND_MENU_SELECTED, &AttribObjType_PANEL::OnFullAttribution, m_AttribObjPanel, ID_CTXT_FULL_ATTRIB);
    
    wxASSERT(m_QueriesPanel);
    this->Bind(wxEVT_COMMAND_MENU_SELECTED, &Queries_PANEL::OnAddQueries,m_QueriesPanel, ID_QUERIES_ADD);
    this->Bind(wxEVT_COMMAND_MENU_SELECTED, &Queries_PANEL::OnRemoveQueries,m_QueriesPanel, ID_QUERIES_REMOVE);
    this->Bind(wxEVT_COMMAND_MENU_SELECTED, &Queries_PANEL::OnRunQueries,m_QueriesPanel, ID_QUERIES_RUN);


}


/* Frame destruction */
ToolMapFrame::~ToolMapFrame()
{
	this->Unbind(wxEVT_COMMAND_MENU_SELECTED, &AttribObjType_PANEL::OnDisplayAttributesAuto, m_AttribObjPanel, ID_CTXT_AUTODISPLAY_ATTRIB);
	this->Unbind(wxEVT_COMMAND_MENU_SELECTED, &AttribObjType_PANEL::OnEmptyListAffterAttributes, m_AttribObjPanel, ID_CTXT_EMPTY_LIST_AFTER_ATTRIB);
	this->Unbind(wxEVT_COMMAND_MENU_SELECTED, &AttribObjType_PANEL::OnFullAttribution, m_AttribObjPanel, ID_CTXT_FULL_ATTRIB);

    wxASSERT(m_QueriesPanel);
    this->Unbind(wxEVT_COMMAND_MENU_SELECTED, &Queries_PANEL::OnAddQueries,m_QueriesPanel, ID_QUERIES_ADD);
    this->Unbind(wxEVT_COMMAND_MENU_SELECTED, &Queries_PANEL::OnRemoveQueries,m_QueriesPanel, ID_QUERIES_REMOVE);
    this->Unbind(wxEVT_COMMAND_MENU_SELECTED, &Queries_PANEL::OnRunQueries,m_QueriesPanel, ID_QUERIES_RUN);

    

	// close project
	m_PManager->CloseProject();

	m_AuiManager->UnInit();


	wxDELETE(m_ToolManager);
	wxDELETE(m_EditManager);
	wxDELETE(m_AttribManager);
	wxDELETE(m_LayerManager);

	wxDELETE(m_SnappingPanel);
	wxDELETE(m_ShortCutPanel);
	wxDELETE(m_QueriesPanel);
	wxDELETE(m_AttribObjPanel);
	wxDELETE(m_TocWindow);

	delete m_LogWindow;
	delete m_AuiManager;
	delete m_MManager;
	delete m_PManager;
	wxDELETE(m_StatManager);

	images_misc_clean();
    images_toolbar_clean();

	// finish the GEOS library
	wxLogDebug(_T("Clearing GEOS library"));
	finishGEOS();
}



void ToolMapFrame::OnQuit(wxCommandEvent & event)
{
	Close(true);
}


void ToolMapFrame::OnClose(wxCloseEvent & event)
{
	// saving window postion
	tmWindowPosition myPos;
	myPos.SavePosition(GetName(), GetRect());
	myPos.SavePosition(_T("AUI_PANES"), m_AuiManager->SavePerspective());
	myPos.SaveScreenPosition();

	wxLog::SetActiveTarget (NULL);
	event.Skip();
}

void ToolMapFrame::_CreateMenu()
{
    // PROJECT
    wxMenuBar* menuBar = new wxMenuBar;
    wxMenu* itemMenu2 = new wxMenu;
	wxMenu* itemMenu3 = new wxMenu;
    itemMenu3->Append(ID_MENU_NEW_PRJ_EMPTY, _("Empty...\tCtrl+N"), wxEmptyString, wxITEM_NORMAL);
    itemMenu3->Append(ID_MENU_NEW_PRJ_EXISTING, _("From template...\tCtrl+Alt+N"), wxEmptyString, wxITEM_NORMAL);
    itemMenu2->Append(wxID_ANY, _("New Project"), itemMenu3);
	itemMenu2->Append(ID_MENU_OPEN_PRJ, _("&Open...\tCtrl+Alt+O"), wxEmptyString, wxITEM_NORMAL);
    wxMenu* itemMenu7 = new wxMenu;
    itemMenu2->Append(ID_MENU_RECENT, _("Recent"), itemMenu7);
    itemMenu2->AppendSeparator();
    itemMenu2->Append(ID_MENU_PRJ_BACKUP, _("Bac&kup\tCtrl+S"), wxEmptyString, wxITEM_NORMAL);
    itemMenu2->Append(ID_MENU_PRJ_BACKUP_MANAGER, _("Manage backup..."), wxEmptyString, wxITEM_NORMAL);
	itemMenu2->Append(ID_MENU_PRJ_SAVE_TEMPLATE, _("Save as template...\tCtrl+Alt+S"), wxEmptyString, wxITEM_NORMAL);
    itemMenu2->AppendSeparator();
    itemMenu2->Append(ID_MENU_EXPORT_LAYER, _("Export Layer...\tCtrl+Alt+E"), wxEmptyString, wxITEM_NORMAL);
	itemMenu2->Append(ID_MENU_EXPORT_MODEL, _("Export Model as PDF..."), _T(""), wxITEM_NORMAL);
    itemMenu2->AppendSeparator();
    wxMenu* itemMenu16 = new wxMenu;
    itemMenu16->Append(ID_MENU_PRJ_DEF, _("Project definition..."), wxEmptyString, wxITEM_NORMAL);
    itemMenu16->Append(ID_MENU_OBJ_DEF, _("Object Kind..."), wxEmptyString, wxITEM_NORMAL);
	itemMenu16->Append(ID_MENU_OBJ_ATTRIB_DEF, _("Object Attribute..."), wxEmptyString, wxITEM_NORMAL);
    itemMenu16->Append(ID_MENU_PRJ_SETTINGS, _("Settings..."), wxEmptyString, wxITEM_NORMAL);
    itemMenu2->Append(ID_MENU_PRJ_EDIT, _("Edit"), itemMenu16);
    //itemMenu2->AppendSeparator();
    itemMenu2->AppendSeparator();
    itemMenu2->Append(wxID_EXIT, _("Exit"), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(itemMenu2, _("&Project"));


    // DATA
    wxMenu* itemMenu24 = new wxMenu;
    itemMenu24->Append(ID_MENU_ADD_SPATIAL_DATA, _("Link data...\tCtrl+O"), wxEmptyString, wxITEM_NORMAL);
    itemMenu24->Append(ID_MENU_UNLINK_SPATIAL_DATA, _("Unlink data...\tCtrl+W"), wxEmptyString, wxITEM_NORMAL);
    itemMenu24->AppendSeparator();
    itemMenu24->Append(ID_MENU_IMPORT_GIS_DATA, _("Import data..."), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(itemMenu24, _("Data"));


    // VIEW
    wxMenu* itemMenu28 = new wxMenu;
    itemMenu28->Append(wxID_BACKWARD, _("Previous Zoom\t<"), wxEmptyString, wxITEM_NORMAL);
	//itemMenu28->Enable(wxID_BACKWARD, false);
	itemMenu28->AppendSeparator();
    itemMenu28->Append(ID_MENU_ZOOM, _("Zoom by rectangle\tZ"), wxEmptyString, wxITEM_NORMAL);
    itemMenu28->Append(ID_MENU_PAN, _("Pan\tH"), _T(""), wxITEM_NORMAL);
    itemMenu28->Append(ID_MENU_ZOOM_FIT, _("Zoom to full extent\tCtrl+0"), wxEmptyString, wxITEM_NORMAL);
    //itemMenu28->Append(ID_MENU_ZOOM_GIVEN_SCALE, _("Zoom to a given scale..."), _T(""), wxITEM_NORMAL);
    //itemMenu28->Append(ID_MENU_ZOOM_COORDINATE, _("Zoom to coordinates..."), _T(""), wxITEM_NORMAL);
    //itemMenu28->Append(ID_MENU_ZOOM_NEXT_SCALE, _("Zoom to next defined scale\t+"), _T(""), wxITEM_NORMAL);
    //itemMenu28->Append(ID_MENU_PREVIOUS_SCALE, _("Zoom to previous defined scale\t-"), _T(""), wxITEM_NORMAL);
    itemMenu28->AppendSeparator();
    //itemMenu28->Append(ID_MENU_SELECTION, _("Zoom to selection"), _T(""), wxITEM_NORMAL);
    //itemMenu28->Enable(ID_MENU_SELECTION, false);
    itemMenu28->Append(ID_MENU_ZOOM_SELECTED_LAYER, _("Zoom to layer"), wxEmptyString, wxITEM_NORMAL);
    itemMenu28->AppendSeparator();
    itemMenu28->Append(wxID_REFRESH, _("Refresh\tCtrl+R"), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(itemMenu28, _("View"));

    // SELECTION
    wxMenu* itemMenu66 = new wxMenu;
    itemMenu66->Append(ID_MENU_SELECT, _("Select tool\tV"), _T(""), wxITEM_NORMAL);
	itemMenu66->Append(ID_MENU_SELECT_BY_OID, _("Select by Feature ID..."), wxEmptyString, wxITEM_NORMAL);
    //itemMenu66->Append(ID_MENU_SELECT_ALL, _("Select all"), _T(""), wxITEM_NORMAL);
    itemMenu66->Append(ID_MENU_SELECT_NONE, _("Clear Selection\tCtrl+D"), wxEmptyString, wxITEM_NORMAL);
    itemMenu66->AppendSeparator();
    itemMenu66->Append(ID_MENU_SELECT_INVERSE, _("Invert Selection"), wxEmptyString, wxITEM_NORMAL);
    //itemMenu66->Enable(ID_MENU_SELECT_INVERSE, false);
    menuBar->Append(itemMenu66, _("Selection"));


    // EDITION
    wxMenu* itemMenu41 = new wxMenu;
    itemMenu41->Append(ID_MENU_UNDO, _("Remove last vertex\tCtrl+Z"), wxEmptyString, wxITEM_NORMAL);
    //itemMenu41->Append(ID_MENU_REDO, _("Redo\tCtrl+R"), _T(""), wxITEM_NORMAL);
    itemMenu41->AppendSeparator();
    itemMenu41->Append(ID_MENU_DRAW, _("Draw feature\tD"), wxEmptyString, wxITEM_NORMAL);
	//itemMenu41->Enable(ID_MENU_DRAW, false);
    itemMenu41->Append(ID_MENU_MODIFY, _("Modify feature\tM"), wxEmptyString, wxITEM_NORMAL);
	//itemMenu41->Enable(ID_MENU_MODIFY, false);
	itemMenu41->Append(ID_MENU_MODIFY_SHARED, _("Move shared Node\tCtrl+T"));
    itemMenu41->Append(ID_MENU_EDIT_VERTEX_POS, _("Edit vertex\tCtrl+V"), wxEmptyString, wxITEM_NORMAL);
	wxString myDeleteText = _("Delete selected feature");
#ifdef __WXMAC__
    myDeleteText.Append(_T("\tBack"));
#else
     myDeleteText.Append(_T("\tDel"));
#endif
    itemMenu41->Append(ID_MENU_DELETE_OBJ, myDeleteText, wxEmptyString, wxITEM_NORMAL);
    itemMenu41->AppendSeparator();
    itemMenu41->Append(ID_MENU_CUT_LINES, _("Cut line\tCtrl+X"), wxEmptyString, wxITEM_NORMAL);
	itemMenu41->Append(ID_MENU_MERGE_LINES, _("Merge line\tCtrl+F"), wxEmptyString, wxITEM_NORMAL);
    itemMenu41->Append(ID_MENU_CREATE_INTERSECTIONS, _("Create intersection\tCtrl+I"), wxEmptyString, wxITEM_NORMAL);
    itemMenu41->Append(ID_MENU_FLIP_LINE, _("Flip line\tCtrl+Alt+F"), _T(""), wxITEM_NORMAL);
	itemMenu41->AppendSeparator();
    itemMenu41->Append(ID_MENU_ADJUST_SNAPPING, _("Snapping...\tCtrl+G"), wxEmptyString, wxITEM_CHECK);
    menuBar->Append(itemMenu41, _("Edition"));

    // ATTRIBUTION
    wxMenu* itemMenu55 = new wxMenu;
    itemMenu55->Append(ID_MENU_ATTRIB_TYPES, _("Object Kind..."), _T(""), wxITEM_CHECK);
    itemMenu55->Append(ID_MENU_ATTRIB_ATTRIBUTES, _("Object Attribute (single feature)...\tCtrl+A"), wxEmptyString, wxITEM_NORMAL);
    itemMenu55->Append(ID_MENU_ATTRIB_BATCH, _("Object Attribute (multiple features)...\tCtrl+Alt+A"), wxEmptyString, wxITEM_NORMAL);
    //itemMenu55->Append(ID_MENU_COPY_PASTE_ATTRIB, _("Copy-Paste Attribution\tCtrl+B"), _T(""), wxITEM_NORMAL);
    //itemMenu55->Enable(ID_MENU_COPY_PASTE_ATTRIB, false);
	itemMenu55->AppendSeparator();
	wxMenu* itemMenu56 = new wxMenu();

	itemMenu56->Append(ID_CTXT_FULL_ATTRIB,  _("Full attribution") , wxEmptyString, wxITEM_CHECK);
	itemMenu56->Append(ID_CTXT_EMPTY_LIST_AFTER_ATTRIB, _("Empty list after attribution") , wxEmptyString, wxITEM_CHECK );
	itemMenu56->Check(ID_CTXT_EMPTY_LIST_AFTER_ATTRIB, true);
	itemMenu56->Append(ID_CTXT_AUTODISPLAY_ATTRIB,  _("Auto display attributes") , wxEmptyString, wxITEM_CHECK);

	itemMenu55->Append(wxID_ANY, _("Object Kind Panel"), itemMenu56);
	itemMenu55->AppendSeparator();
    itemMenu55->Append(ID_MENU_ORIENT_POINT, _("Orientation (interactive mode)\tCtrl+Y"), wxEmptyString, wxITEM_NORMAL);
	itemMenu55->AppendSeparator();
    itemMenu55->Append(ID_MENU_SHORTCUTS, _("Shortcut..."), wxEmptyString, wxITEM_CHECK);
    menuBar->Append(itemMenu55, _("Attribution"));


    // VALIDATION
    wxMenu* itemMenu63 = new wxMenu;
    itemMenu63->Append(ID_MENU_QUERIES, _("Queries Panel..."), wxEmptyString, wxITEM_CHECK);
    itemMenu63->AppendSeparator();
    itemMenu63->Append(ID_QUERIES_ADD, _("New query..."));
    itemMenu63->Append(ID_QUERIES_REMOVE, _("Remove selected query..."));
    itemMenu63->AppendSeparator();
	itemMenu63->Append(ID_QUERIES_RUN, _("Run selected query\tCtrl+Alt+R"), wxEmptyString, wxITEM_NORMAL);
    itemMenu63->AppendSeparator();
    itemMenu63->Append(ID_MENU_TOOL_DANGLING, _("Dangling Nodes..."), _T(""), wxITEM_NORMAL);
	menuBar->Append(itemMenu63, _("Validation"));

    // WINDOW
    wxMenu* itemMenu77 = new wxMenu;
    itemMenu77->Append(ID_MENU_TOC_WINDOW, _("Table of content"), wxEmptyString, wxITEM_CHECK);
    itemMenu77->Check(ID_MENU_TOC_WINDOW, true);
    itemMenu77->Append(ID_MENU_INFO_WINDOW, _("Information Window\tCtrl+Alt+I"), wxEmptyString, wxITEM_CHECK);
    itemMenu77->Append(ID_MENU_LOG_WINDOW, _("Log Window\tCtrl+L"), wxEmptyString, wxITEM_CHECK);
#ifndef __WXMAC__
	itemMenu77->AppendSeparator();
#endif
	itemMenu77->Append(wxID_PREFERENCES);
    itemMenu77->AppendSeparator();
    wxMenu * myLayoutMenu = new wxMenu;
    myLayoutMenu->Append(ID_MENU_LAYOUT_DEFAULT, _("Default"));
    myLayoutMenu->Append(ID_MENU_LAYOUT_VERTICAL, _("Vertical"));
    myLayoutMenu->Append(ID_MENU_LAYOUT_HORIZONTAL, _("Horizontal"));
    itemMenu77->AppendSubMenu(myLayoutMenu, _("Workspace"));
	itemMenu77->AppendSeparator();
	itemMenu77->Append(ID_MENU_STATISTICS, _("Statistics..."), _T(""));
    menuBar->Append(itemMenu77, _("Window"));

    // HELP
    wxMenu* itemMenu81 = new wxMenu;
    itemMenu81->Append(wxID_ABOUT, _("About..."), wxEmptyString, wxITEM_NORMAL);
	itemMenu81->Append(ID_MENU_COMPONENTS, _("Components informations..."), wxEmptyString, wxITEM_NORMAL);
	itemMenu81->AppendSeparator();
    itemMenu81->Append(ID_MENU_CHECK_UPDATE, _("Check for updates..."), wxEmptyString, wxITEM_NORMAL);
	itemMenu81->AppendSeparator();
	itemMenu81->Append(ID_MENU_USER_MANUAL, _("User Manual..."), wxEmptyString, wxITEM_NORMAL);
	itemMenu81->AppendSeparator();
	itemMenu81->Append(ID_MENU_REPORT_BUG, _("Report a bug..."), wxEmptyString, wxITEM_NORMAL);
	itemMenu81->Append(ID_MENU_ASK_NEW_FEATURE, _("Ask for a new feature..."), wxEmptyString, wxITEM_NORMAL);
    menuBar->Append(itemMenu81, _("&Help"));
    this->SetMenuBar(menuBar);
        
}


void ToolMapFrame::_CreateToolBar()
{
	long style = wxTB_FLAT | wxTB_HORIZONTAL;
	// conditionnal compilation for better look under win32
#ifndef __WXMSW__
	style += wxTB_TEXT;
#endif

	wxToolBar* itemToolBar3 = this->CreateToolBar(style, wxID_ANY);
    itemToolBar3->SetToolBitmapSize(wxSize(32, 32));
    itemToolBar3->AddTool(ID_MENU_SELECT, _("Select"), *_img_toolbar_select, wxNullBitmap, wxITEM_NORMAL, _("Select"), wxEmptyString);
    itemToolBar3->AddTool(ID_MENU_ZOOM_FIT, _("Fit"), *_img_toolbar_zoom_fit, wxNullBitmap, wxITEM_NORMAL, _("Zoom to full extent"), wxEmptyString);
    itemToolBar3->AddTool(ID_MENU_ZOOM, _("Zoom"),*_img_toolbar_zoom, wxNullBitmap, wxITEM_NORMAL, _("Zoom by rectangle"), wxEmptyString);
    itemToolBar3->AddTool(ID_MENU_PAN, _("Pan"), *_img_toolbar_pan, wxNullBitmap, wxITEM_NORMAL, _("Pan"), wxEmptyString);
    itemToolBar3->AddTool(wxID_BACKWARD, _("Previous Zoom"), *_img_toolbar_previous, wxNullBitmap, wxITEM_NORMAL, _("Previous Zoom"), wxEmptyString);

	wxArrayString itemComboBox8Strings;
    m_ScaleCombo = new tmScaleCtrlCombo ( itemToolBar3, ID_TOOLBAR_ZOOM, wxDefaultPosition, wxDefaultSize, itemComboBox8Strings );
	itemToolBar3->AddControl(m_ScaleCombo);
    itemToolBar3->AddTool(ID_MENU_DRAW, _("Draw"), *_img_toolbar_edit, wxNullBitmap, wxITEM_NORMAL, _("Draw"), wxEmptyString);
    itemToolBar3->AddTool(ID_MENU_MODIFY, _("Modify"), *_img_toolbar_modify, wxNullBitmap, wxITEM_NORMAL, _("Modify"), wxEmptyString);
	itemToolBar3->AddTool(ID_MENU_MODIFY_SHARED, _("Move shared Node"), *_img_toolbar_vertex_move, wxNullBitmap, wxITEM_NORMAL, _("Move shared Node"));

	itemToolBar3->AddSeparator();
    itemToolBar3->AddTool(ID_MENU_ATTRIB_TYPES, _("Object Kind"), *_img_toolbar_attribute, wxNullBitmap, wxITEM_NORMAL, _("Object Kind"), wxEmptyString);
    itemToolBar3->AddTool(ID_MENU_ATTRIB_ATTRIBUTES, _("Object Attribute"), *_img_toolbar_attribute_extend, wxNullBitmap, wxITEM_NORMAL, _("Object Attribute"), wxEmptyString);
    itemToolBar3->AddSeparator();
    itemToolBar3->AddTool(ID_MENU_INFO_WINDOW, _("Information"), *_img_toolbar_info, wxNullBitmap, wxITEM_NORMAL, _("Information"), wxEmptyString);
    itemToolBar3->Realize();
}



void ToolMapFrame::_CreateAccelerators() {
    wxAcceleratorEntry entries[8];
    entries[0].Set(wxACCEL_NORMAL, (int) 'V', ID_MENU_SELECT);
    entries[1].Set(wxACCEL_NORMAL, (int) 'Z', ID_MENU_ZOOM);
    entries[2].Set(wxACCEL_NORMAL, (int) 'H', ID_MENU_PAN);
    entries[3].Set(wxACCEL_NORMAL, (int) 'D', ID_MENU_DRAW);
    entries[4].Set(wxACCEL_NORMAL, (int) 'M', ID_MENU_MODIFY);
    entries[5].Set(wxACCEL_NORMAL, (int) '<', wxID_BACKWARD);
    entries[6].Set(wxACCEL_NORMAL, WXK_BACK, ID_MENU_DELETE_OBJ);
    entries[7].Set(wxACCEL_NORMAL, WXK_DELETE, ID_MENU_DELETE_OBJ);

    wxAcceleratorTable accel(8, entries);
    SetAcceleratorTable(accel);
}



void ToolMapFrame::OnNewProject(wxCommandEvent & event)
{
	// call the project manager and ask for
	// creating a new project.
	if (m_PManager->CreateNewProject())
	{
	// add name to the program bar
	wxString myProgName = g_ProgName + _T(" - ") + m_PManager->GetProjectName();
	SetTitle(myProgName);


	}

}




void ToolMapFrame::OnOpenProject (wxCommandEvent & event)
{
	// display a dir dialog for selecting the project to open
	wxDirDialog * myDirDLG = new wxDirDialog(this, _("Choose a ToolMap project"),
											 _T(""), wxRESIZE_BORDER | wxDD_DIR_MUST_EXIST);
	if (myDirDLG->ShowModal()==wxID_OK)
	{
		// call the project manager and ask to open an
		// existing project.
		int iActError = m_PManager->OpenProject(myDirDLG->GetPath());
		if (iActError != tmDB_OPEN_OK){
			// If we can open the project,set the name in the program bar.
			wxString myProgName = g_ProgName;
			SetTitle(myProgName);
		}
	}
	wxDELETE(myDirDLG);
}



/***************************************************************************//**
 @brief Open file based on recent
 @details Event function called when user try to open a file from the recent
 menu.
 @author Lucien Schreiber (c) CREALP 2007
 @date 07 April 2008
 *******************************************************************************/
void ToolMapFrame::OnOpenRecentProject(wxCommandEvent & event){
	wxString myPath = _T("");

	// get the file to open (the one clicked...)
	if (m_MManager->GetRecentFile(myPath, event.GetId() - wxID_FILE1))
	{
		int iActError = m_PManager->OpenProject(myPath);
		if (iActError == tmDB_OPEN_OK) {
			wxString myProgName = g_ProgName +  _T(" - ") + m_PManager->GetProjectName();
			SetTitle(myProgName);
			return;
		}

		if (iActError == tmDB_OPEN_ERR_NOT_FOUND ) {
			wxFileName myName (myPath, _T(""));
			wxArrayString myPathNames = myName.GetDirs();
			tmOpenRecentError_DLG myDlg(this, wxID_ANY, _("Open failed"), myPathNames.Item(myPathNames.GetCount() -1));
			myDlg.ShowModal();

			if (myDlg.GetRemoveFromRecent() == true) {
				m_MManager->RemoveFileFromRecent(event.GetId() - wxID_FILE1);
			}
		}
		// opening failed ! Correct Programe name
		wxString myProgName = g_ProgName;
		SetTitle(myProgName);
	}
}



void ToolMapFrame::OnEditProjectObjects (wxCommandEvent & event)
{
	// call the project manager
	m_PManager->EditProjectObjectDefinition();
}



void ToolMapFrame::OnEditProjectSettings (wxCommandEvent & event)
{
	// call the project manager.
	m_PManager->EditProjectSettings();
}


void ToolMapFrame::OnEditProject (wxCommandEvent & event)
{
	// call the project manager
	m_PManager->EditProject(0);

}

void ToolMapFrame::OnEditObjectAttributes (wxCommandEvent & event)
{
	// call the project manager.
	m_PManager->EditProject(1);
}



void ToolMapFrame::OnNewProjectExisting (wxCommandEvent & event)
{
	NewTemplatePrjWizard myWizard (this, wxID_ANY, _("New project from template file"));
	if(myWizard.ShowWizard() != wxID_OK){
		return;
	}

	wxBeginBusyCursor();
	wxASSERT(myWizard.GetBackupFileData()->IsValid());
	BackupManager myBckManager(NULL);

	// close project now
	m_PManager->CloseProject();

	// restore
	if(myBckManager.Restore(*(myWizard.GetBackupFileData()))==false){
		wxLogError(_("Error Creating project from template"));
		return;
	}
	wxEndBusyCursor();

	m_PManager->OpenProject(myWizard.GetBackupFileData()->GetInputDirectory().GetFullPath());
}


void ToolMapFrame::OnLogWindow(wxCommandEvent & event)
{
	if (m_LogWindow->GetFrame()->IsShown()) {
		m_LogWindow->GetFrame()->Hide();
	}
	else {
		m_LogWindow->Show();
		m_LogWindow->GetFrame()->Raise();
	}
}

void ToolMapFrame::OnTocWindow (wxCommandEvent & event)
{
	if (m_TocWindow->IsShown())
	{
		m_TocWindow->Hide();
	}
	else
	{
		m_TocWindow->Show();
	}

}



void ToolMapFrame::OnShowObjectAttributionWindow (wxCommandEvent & event)
{
	if(m_AttribObjPanel->IsPanelShown())
	{
		m_AttribObjPanel->HidePanel();
		GetMenuBar()->Check(ID_MENU_ATTRIB_TYPES, FALSE);
	}
	else
	{
		m_AttribObjPanel->ShowPanel();
		GetMenuBar()->Check(ID_MENU_ATTRIB_TYPES, TRUE);
	}

}


/***************************************************************************//**
 @brief Display or hide the #Queries_PANEL
 @author Lucien Schreiber (c) CREALP 2008
 @date 08 November 2008
 *******************************************************************************/
void ToolMapFrame::OnShowQueriesWindow (wxCommandEvent & event)
{
	if (m_QueriesPanel->IsPanelShown()){
		m_QueriesPanel->HidePanel();
	}
	else{
		m_QueriesPanel->ShowPanel();
	}
    wxCommandEvent evt;
    OnRefreshView(evt);
}


/*
void ToolMapFrame::OnQueriesAction (wxCommandEvent & event){
	// sent event to the queries manager
	if (m_QueriesPanel == NULL) {
        return;
    }
    
    wxASSERT(m_QueriesPanel);
    m_QueriesPanel->ProcessEvent(event);
}
*/


/***************************************************************************//**
 @brief Display or hide the #Shortcuts_PANEL
 @author Lucien Schreiber (c) CREALP 2008
 @date 04 December 2008
 *******************************************************************************/
void ToolMapFrame::OnShowShortcutWindow (wxCommandEvent & event)
{
	if (m_ShortCutPanel->IsPanelShown()){
		m_ShortCutPanel->HidePanel();
	}
	else{
		m_ShortCutPanel->ShowPanel();
	}
    wxCommandEvent evt;
    OnRefreshView(evt);
}



/***************************************************************************//**
 @brief Display or hide the #Snapping_PANEL
 @author Lucien Schreiber (c) CREALP 2009
 @date 19 January 2009
 *******************************************************************************/
void ToolMapFrame::OnShowSnappingWindow (wxCommandEvent & event)
{
	if (m_SnappingPanel->IsPanelShown())
	{
		m_SnappingPanel->HidePanel();
	}
	else
	{
		m_SnappingPanel->ShowPanel();
	}
	wxCommandEvent evt;
    OnRefreshView(evt);
}




void ToolMapFrame::OnEditSwitch (wxCommandEvent & event)
{
	bool bEditStart = false;
	if (event.GetEventType() == tmEVT_EM_EDIT_START)
		bEditStart = true;
	else
		m_LayerManager->OnSelect(); // set the select cursor
}


/***************************************************************************//**
 @brief User press the "Delete selected object" menu
 @image html editdeletemenu.png
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 February 2009
 *******************************************************************************/
void ToolMapFrame::OnEditDeleteSelected (wxCommandEvent & event)
{

	if (m_EditManager->IsDrawingAllowed()==false)
	{
		return;
	}

	int iSelNum = m_EditManager->GetSelectionCount();
	if (iSelNum > 1)

		if (wxMessageBox(wxString::Format(_("%d Objects selected ! Confirm deleting ?"), iSelNum),
							 wxString::Format(_("Deleting %d objects"), iSelNum),
							 wxICON_WARNING | wxYES_NO, this) ==wxNO)
			return;


	m_EditManager->DeleteSelected(true);
}



/***************************************************************************//**
 @brief Called when user press "Undo"
 @author Lucien Schreiber (c) CREALP 2009
 @date 05 February 2009
 *******************************************************************************/
void ToolMapFrame::OnEditUndo (wxCommandEvent & event)
{
	m_EditManager->UndoLastVertex();
}



/***************************************************************************//**
 @brief Called when user try to create intersections
 @author Lucien Schreiber (c) CREALP 2009
 @date 09 February 2009
 *******************************************************************************/
void ToolMapFrame::OnCreateIntersections (wxCommandEvent & event)
{
	m_EditManager->CreateIntersections();
}



/***************************************************************************//**
 @brief Display the edit vertex position dialog
 @author Lucien Schreiber (c) CREALP 2009
 @date 23 February 2009
 *******************************************************************************/
void ToolMapFrame::OnEditVertexPosition (wxCommandEvent & event)
{
	m_EditManager->EditVertexPosition();
}



/***************************************************************************//**
 @brief Called when line merge menu is pressed
 @author Lucien Schreiber (c) CREALP 2009
 @date 27 February 2009
 *******************************************************************************/
void ToolMapFrame::OnMergeSelectedLines (wxCommandEvent & event)
{
	m_EditManager->MergeSelectedLines();
}


/***************************************************************************//**
 @details Called when Display Advanced attribution is pressed
 @author Lucien Schreiber (c) CREALP 2009
 @date 06 March 2009
 *******************************************************************************/
void ToolMapFrame::OnShowAAttributionWindow (wxCommandEvent & event)
{
	m_AttribManager->AAttributionButtonShow();
}


void ToolMapFrame::OnAAttributionBatchWindow (wxCommandEvent & event){
	m_AttribManager->AAttributionBatchShow();
}


/***************************************************************************//**
 @brief Show the informations dialog
 @author Lucien Schreiber (c) CREALP 2009
 @date 07 April 2009
 *******************************************************************************/
void ToolMapFrame::OnShowInformationDialog (wxCommandEvent & event)
{
	/*int i = 0;
	int * pi = NULL;
	*pi = 12;*/

	m_AttribManager->DisplayInformationsWnd();
}



void ToolMapFrame::OnCheckUpdates (wxCommandEvent & event)
{
	_CheckUpdates(true);
}



void ToolMapFrame::OnReportBug (wxCommandEvent & event)
{
	wxLaunchDefaultBrowser(_T("http://www.crealp.ch/index.php?option=com_mad4joomla&jid=3&Itemid=320"));
}



void ToolMapFrame::OnComponentsInfo (wxCommandEvent & event){
	lsVersionDlg myDlg (this, wxID_ANY, _("Components"));
	myDlg.SetBitmapLogo(*_img_icon_toolmap);
	myDlg.SetCopyright(wxString::Format("\u00A9 CREALP, \u00A9 SWISSTOPO, %d",wxDateTime::Now().GetYear()));
	myDlg.ShowModal();
}



void ToolMapFrame::OnUserManual(wxCommandEvent & event){
	wxLaunchDefaultBrowser(_T("http://www.crealp.ch/toolmap/documentation/doku.php"));
}


void ToolMapFrame::OnFlipLine (wxCommandEvent & event){
	m_EditManager->FlipLine();
}



void ToolMapFrame::OnPreferences(wxCommandEvent & event){
	PreferenceDLG myDlg(this, wxID_ANY);
	if (myDlg.ShowModal() != wxID_OK) {
		return;
	}

	_LoadPreference(true);
}


void ToolMapFrame::OnRefreshView(wxCommandEvent & event){
    wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
    GetEventHandler()->AddPendingEvent(evt2);
}


void ToolMapFrame::OnExportProjectModel (wxCommandEvent & event){
    // wizard
    PdfExportWizard myWizard(this);
    if (myWizard.ShowWizard() == wxID_CANCEL){
        return;
    }

    // select filename
    wxASSERT(m_PManager->GetMemoryProjectDefinition());
    wxString myFileTxtName = wxFileSelector(_("Export Model as PDF"), wxEmptyString,
											m_PManager->GetMemoryProjectDefinition()->m_PrjName,
                                            "pdf", _("PDF files (*.pdf)|*.pdf"),
											wxFD_SAVE | wxFD_OVERWRITE_PROMPT, this);
	if (myFileTxtName.IsEmpty()) {
		return;
	}

	PdfDocument myPdf (m_PManager->GetMemoryProjectDefinition());
    myWizard.GetWizardChoices(myPdf);
    myPdf.Generate(wxFileName(myFileTxtName));

	wxFileName myExportName (myFileTxtName);
	if (wxMessageBox(wxString::Format(_("Open generated: '%s' file ?"), myExportName.GetFullName()),
					 _("Open generated file"),
					 wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION,
					 this) == wxYES){
		wxLaunchDefaultApplication(myFileTxtName);
	}

}




void ToolMapFrame::_LoadPreference(bool reload){
	wxConfigBase * myConfig =  wxConfigBase::Get(false);
    wxASSERT(myConfig);

	myConfig->SetPath("GENERAL");
	wxString mySelColorText = myConfig->Read("selection_color", wxEmptyString);
	bool mySelHalo = myConfig->ReadBool("selection_halo", false);
    myConfig->SetPath("..");

	wxColour mySelColor = *wxRED;
	if (mySelColorText != wxEmptyString) {
		mySelColor.Set(mySelColorText);
	}

	wxASSERT(m_LayerManager);
	m_LayerManager->SetSelectionColour(mySelColor);
	m_LayerManager->SetSelectionHalo(mySelHalo);

	if (reload == true) {
		//m_LayerManager->ReloadProjectLayersThreadStart(false);
		wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
		GetEventHandler()->AddPendingEvent(evt2);
	}
}



void ToolMapFrame::_CreatePerspectives(){
    wxASSERT(m_AuiManager);
    m_Perspectives.Clear();

    // default perspective
    wxAuiPaneInfoArray myPanels = m_AuiManager->GetAllPanes();
    for (unsigned int i = 0; i< myPanels.GetCount(); i++) {
        myPanels.Item(i).Hide();
    }
    m_AuiManager->GetPane(SYMBOL_MAIN_PANEL_TITLE).Show();
    m_Perspectives.Add(m_AuiManager->SavePerspective());

    // vertical perspective
    m_AuiManager->GetPane(SNAPPING_PANEL_TITLE).Show().Left().Dock().Layer(1);
    m_AuiManager->GetPane(SHORTCUT_PANEL_TITLE).Show().Left().Dock().Layer(1);
    m_AuiManager->GetPane(SYMBOL_ATTRIBOBJTYPE_PANEL_TITLE).Show().Right().Dock().Layer(1);
    m_AuiManager->GetPane(SYMBOL_QUERIES_PANEL_TITLE).Show().Right().Dock().Layer(1);
    m_Perspectives.Add(m_AuiManager->SavePerspective());

    // horizontal perspective
    m_AuiManager->GetPane(SYMBOL_ATTRIBOBJTYPE_PANEL_TITLE).Show().Left().Dock().Layer(1);
    m_AuiManager->GetPane(SYMBOL_QUERIES_PANEL_TITLE).Show().Bottom().Dock().Layer(2);
    m_AuiManager->GetPane(SHORTCUT_PANEL_TITLE).Show().Bottom().Dock().Layer(2).Position(1);
    m_AuiManager->GetPane(SNAPPING_PANEL_TITLE).Show().Bottom().Dock().Layer(2).Position(0);
    m_Perspectives.Add(m_AuiManager->SavePerspective());
    
    m_AuiManager->LoadPerspective(m_Perspectives.Item(tmPERSPECTIVE_MODE_DEFAULT));
    m_AuiManager->Update();
}


void ToolMapFrame::OnLayoutDefault (wxCommandEvent & event){
    wxASSERT(m_AuiManager);
    m_AuiManager->LoadPerspective(m_Perspectives.Item(tmPERSPECTIVE_MODE_DEFAULT));
}



void ToolMapFrame::OnLayoutVertical (wxCommandEvent & event){
    wxASSERT(m_AuiManager);
    m_AuiManager->LoadPerspective(m_Perspectives.Item(tmPERSPECTIVE_MODE_VERTICAL));
}



void ToolMapFrame::OnLayoutHorizontal (wxCommandEvent & event){
    wxASSERT(m_AuiManager);
    m_AuiManager->LoadPerspective(m_Perspectives.Item(tmPERSPECTIVE_MODE_HORIZONTAL));
}



void ToolMapFrame::_CheckUpdates(bool ismanual){
	wxConfigBase * myConfig =  wxConfigBase::Get(false);
    wxASSERT(myConfig);
    myConfig->SetPath("UPDATE");
	bool bCheckStartup = myConfig->ReadBool("check_on_start", true);
    wxString myProxyInfo = myConfig->Read("proxy_info", wxEmptyString);
    myConfig->SetPath("..");

	if (bCheckStartup == false && ismanual == false) {
        return;
    }

	// clean svn number. may be
	// 1234 or 1234:1245 or 1234M or 1234S or event 1234:1245MS
	long mySvnVersion = 0;
	wxString mySvnText(lsVERSION_SOFT_VERSION);
	while (wxStrpbrk(mySvnText, _T("MS")) != NULL) {
		mySvnText.RemoveLast();
	}

	int mySeparatorPos = mySvnText.Find(":");
	if (mySeparatorPos != wxNOT_FOUND) {
		mySvnText = mySvnText.Mid(mySeparatorPos);
	}

	if(mySvnText.ToLong(&mySvnVersion)==false){
		wxFAIL;
	}

    WebUpdateThread * myUpdate = new WebUpdateThread(m_InfoBar, myProxyInfo);
    myUpdate->CheckNewVersion(mySvnVersion, true, ismanual, true);
}



void ToolMapFrame::OnImportGISData (wxCommandEvent & event)
{
	ImportWizard myWizard (this, wxID_ANY);
	if(myWizard.ShowWizard() != wxID_OK){
		return;
	}

	tmImport * myImport = myWizard.GetImport();
	wxASSERT(myImport);

	wxProgressDialog myProgress(_("Importing data progress"),
								_T("Importing data in progress, please wait"),
								100,
								this,
								wxPD_CAN_ABORT|wxPD_AUTO_HIDE|wxPD_APP_MODAL);
	wxASSERT(m_PManager->GetDatabase());
	myImport->Import(m_PManager->GetDatabase(), &myProgress);

	wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
	GetEventHandler()->AddPendingEvent(evt2);


	/*tmGISImport myImport;

	tmGISImport_DLG myDlg (this, &myImport);
	if(myDlg.ShowModal() != wxID_OK)
		return;

	wxProgressDialog myProgress(_("Importing GIS data progress"),
								_T("Importing data in progress, please wait"),
								100,
								this,
								wxPD_CAN_ABORT|wxPD_AUTO_HIDE|wxPD_APP_MODAL);

	// importing
	wxASSERT(m_PManager->GetDatabase());
	myImport.Import(m_PManager->GetDatabase(), myDlg.GetImportLayer(), &myProgress);
	wxLogDebug(_("GIS data imported in %u [ms]"), myImport.GetElapsedTime());

	wxCommandEvent evt2(tmEVT_LM_UPDATE, wxID_ANY);
	GetEventHandler()->AddPendingEvent(evt2);*/
}




/***************************************************************************//**
 @brief Clear active selection
 @author Lucien Schreiber (c) CREALP 2009
 @date 31 March 2009
 *******************************************************************************/
void ToolMapFrame::OnSelectNone (wxCommandEvent & event)
{
	m_LayerManager->SelectedClear();
}



void ToolMapFrame::OnSelectByOid (wxCommandEvent & event){
	m_LayerManager->SelectByOid();
}


/***************************************************************************//**
 @brief Invert actual selection
 @author Lucien Schreiber (c) CREALP 2009
 @date 31 March 2009
 *******************************************************************************/
void ToolMapFrame::OnSelectInvert (wxCommandEvent & event)
{
	m_LayerManager->SelectedInvert();
}



/***************************************************************************//**
 @brief Called earch time the selection is updated
 @details Modifies menu according to number of selected item
 @author Lucien Schreiber (c) CREALP 2009
 @date 18 February 2009
 *******************************************************************************/
void ToolMapFrame::OnUpdateSelection (wxCommandEvent & event)
{
	SetStatusText(wxString::Format(_T("%d Selected features"),
                                   m_LayerManager->GetSelectedDataMemory()->GetCount()),2);
	event.Skip();
}


void ToolMapFrame::OnProjectBackup (wxCommandEvent & event)
{
    wxASSERT(m_PManager);
    wxASSERT(m_PManager->GetDatabase());

    // backup path exists ?
    wxString myBackupPath = wxEmptyString;
    if(m_PManager->GetDatabase()->GetProjectBackupPath(myBackupPath) != PATH_OK){
        wxString sErrMsg =	_("No path specified or path invalid \n");
        sErrMsg.Append(		_("for backups or restore operations,\n\n"));
        sErrMsg.Append(		_("Please go to Project->Edit Project->Settings...\n"));
        sErrMsg.Append(		_("and specify a valid path."));
        wxMessageBox(sErrMsg,_("No valid path found"),wxICON_ERROR | wxOK);
        return;
    }

    // create backup file
    BackupFile myBckFile;
    myBckFile.SetInputDirectory(wxFileName(m_PManager->GetDatabase()->DataBaseGetPath(),
                                           m_PManager->GetDatabase()->DataBaseGetName()));
    myBckFile.SetDate(wxDateTime::Now());
    myBckFile.SetOutputName(wxFileName(myBackupPath,
									   m_PManager->GetDatabase()->DataBaseGetName(),
									   "tmbk"));

    // ask for comment
    wxTextEntryDialog myDlg (this, _("Backup comment:"), _("Backup"), wxEmptyString, wxOK | wxCENTRE);
    if (myDlg.ShowModal() == wxID_OK) {
        myBckFile.SetComment(myDlg.GetValue());
    }

	wxBeginBusyCursor();
    wxLogMessage("filename for backup will be : " + myBckFile.GetOutputName().GetFullPath());
    BackupManager myBckManager (m_PManager->GetDatabase());

	// Don't display progress dialog under Mac... Toooo slow!
	wxWindow * myWnd = NULL;
#ifndef __WXMAC__
	myWnd = this;
#endif

    if (myBckManager.Backup(myBckFile,myWnd) == false) {
        wxLogError(_("Backup : '%s' Failed !"), myBckFile.GetOutputName().GetFullName());
    }
	wxEndBusyCursor();
}



void ToolMapFrame::OnProjectBackupManage (wxCommandEvent & event){
    wxASSERT(m_PManager);

    // backup path exists ?
    wxString myBackupPath = wxEmptyString;
    if(m_PManager->GetDatabase()->GetProjectBackupPath(myBackupPath) != PATH_OK){
        wxString sErrMsg =	_("No path specified or path invalid \n");
        sErrMsg.Append(		_("for backups or restore operations,\n\n"));
        sErrMsg.Append(		_("Please go to Project->Edit Project->Settings...\n"));
        sErrMsg.Append(		_("and specify a valid path."));
        wxMessageBox(sErrMsg,_("No valid path found"),wxICON_ERROR | wxOK);
        return;
    }

    BackupManager myBckManager(m_PManager->GetDatabase());
    BackupManagerDLG myDlg (this, wxID_ANY, _("Manage Backup"), &myBckManager);
    if(myDlg.ShowModal() != wxID_OK){
		return;
	}

	wxBeginBusyCursor();
	wxString myRestoreName = myDlg.GetRestoreFileName();
	wxASSERT(myRestoreName != wxEmptyString);

	BackupFile myRestoreInfo;
	myRestoreInfo.SetOutputName(wxFileName(myBackupPath, myRestoreName));
	myRestoreInfo.SetInputDirectory(wxFileName(m_PManager->GetDatabase()->DataBaseGetPath(),
											   m_PManager->GetDatabase()->DataBaseGetName()));
	// unused but needed for structure validity!
	myRestoreInfo.SetDate(wxDateTime::Now());
	wxASSERT(myRestoreInfo.IsValid());

	// close project now
	m_PManager->CloseProject();

	// restore
	if(myBckManager.Restore(myRestoreInfo)==false){
		wxLogError(_("Error restoring project: '%s'"), myRestoreName);
		return;
	}
	wxEndBusyCursor();
	m_PManager->OpenProject(myRestoreInfo.GetInputDirectory().GetFullPath());
}



void ToolMapFrame::OnProjectSaveTemplate (wxCommandEvent & event){
	wxASSERT(m_PManager);
    wxASSERT(m_PManager->GetDatabase());

	// get template directory
	wxFileDialog myTemplateFileDlg (this, _("Save template"), "","", _("template files (*.tmtp)|*.tmtp"),
									wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (myTemplateFileDlg.ShowModal() == wxID_CANCEL) {
		return;
	}

	wxFileName myTemplateFileName = wxFileName(myTemplateFileDlg.GetPath());
	if (myTemplateFileName.IsOk() == false) {
		wxLogError(_("Template name is Incorrect!"));
		return;
	}

	if (myTemplateFileName.GetExt() != _T("tmtp")) {
		myTemplateFileName.SetExt(_T("tmtp"));
	}

    // create backup file
    BackupFile myBckFile;
    myBckFile.SetInputDirectory(wxFileName(m_PManager->GetDatabase()->DataBaseGetPath(),
                                           m_PManager->GetDatabase()->DataBaseGetName()));
    myBckFile.SetDate(wxDateTime::Now());
    myBckFile.SetOutputName(myTemplateFileName);
	myBckFile.SetUseDate(false);

    // ask for comment
    wxTextEntryDialog myDlg (this, _("Template comment:"), _("Save Template"), wxEmptyString, wxOK | wxCENTRE);
    if (myDlg.ShowModal() == wxID_OK) {
        myBckFile.SetComment(myDlg.GetValue());
    }

	wxBeginBusyCursor();
    wxLogMessage("filename for template will be : " + myBckFile.GetOutputName().GetFullPath());
    BackupManager myBckManager (m_PManager->GetDatabase());

	// Don't display progress dialog under Mac... Toooo slow!
	wxWindow * myWnd = NULL;
#ifndef __WXMAC__
	myWnd = this;
#endif

    if (myBckManager.Backup(myBckFile,myWnd) == false) {
        wxLogError(_("Template : '%s' Failed !"), myBckFile.GetOutputName().GetFullName());
    }
	wxEndBusyCursor();
}



void ToolMapFrame::OnAddGisData (wxCommandEvent & event)
{
	m_LayerManager->AddLayer(event);
}


void ToolMapFrame::OnShortcutAttributionDone (wxCommandEvent & event)
{
	wxString myStatusInfo =  event.GetString();
	myStatusInfo.Append(_(" : Attribution DONE"));

	GetStatusBar()->SetStatusText(myStatusInfo, 0);
}



/***************************************************************************//**
 @brief This is called when a pane was closed using the close button
 @details In this case, we should remove the tick near the window name in the
 menu
 @author Lucien Schreiber (c) CREALP 2009
 @date 23 January 2009
 *******************************************************************************/
void ToolMapFrame::OnCloseManagedPane(wxAuiManagerEvent & event)
{
	int iId = wxNOT_FOUND;
	wxAuiPaneInfo * myPaneInfo = event.GetPane();

	if (myPaneInfo->name == SHORTCUT_PANEL_TITLE)
		iId = ID_MENU_SHORTCUTS;

	if (myPaneInfo->name == SNAPPING_PANEL_TITLE)
		iId = ID_MENU_ADJUST_SNAPPING;

	if (iId != wxNOT_FOUND)
		GetMenuBar()->Check(iId, false);
}




void ToolMapFrame::OnStatisticsUpdate(wxCommandEvent & event){
	wxASSERT(m_StatManager);
	if (m_StatManager->IsReady() == false) {
		return;
	}

	if (event.GetEventType() == tmEVT_STAT_CLICK) {
		m_StatManager->AppendToBuffer(1, 0, 0);
	}
	else if (event.GetEventType() == tmEVT_STAT_ATTRIB) {
		m_StatManager->AppendToBuffer(0, 1, 0);
	}
	else if (event.GetEventType() == tmEVT_STAT_INTERSECTION) {
		m_StatManager->AppendToBuffer(0, 0, 1);
	}
}




void ToolMapFrame::OnStatisticsDialog(wxCommandEvent & event){
	wxASSERT(m_StatManager);
	m_StatManager->ShowStatsDialog(this);
}



/*void ToolMapFrame::OnMenuZoomPreviousChange(wxCommandEvent & event)
{
	bool bStatus = static_cast<bool>(event.GetInt());
	m_MManager->ZoomStatus(bStatus);
	m_TManager->ZoomStatus(bStatus);
}*/



void ToolMapFrame::OnToolChanged (wxCommandEvent & event)
{
	// check that a project is open
	if (!m_PManager->IsProjectOpen())
		return;

	switch (event.GetId())
	{
		case ID_MENU_SELECT:
			m_LayerManager->OnSelect();
			break;

		case ID_MENU_ZOOM_FIT:
			// execute command imediatly
			m_LayerManager->OnZoomToFit();
			break;

		case ID_MENU_ZOOM:
			m_LayerManager->OnZoomRectangle();
			break;

		case ID_MENU_PAN:
			m_LayerManager->OnPan();
			break;

		case ID_MENU_DRAW:
			m_EditManager->OnToolEdit();
			break;

		case ID_MENU_MODIFY:
			m_EditManager->OnToolModify();
			break;

		case ID_MENU_CUT_LINES:
			m_EditManager->OnToolCutLines();
			break;

		case ID_MENU_ORIENT_POINT:
			m_EditManager->OnToolOrientedPoint();
			break;

		case ID_MENU_MODIFY_SHARED:
			m_EditManager->OnToolEditShared();
			break;


		default:
			m_LayerManager->OnSelect();
			wxLogDebug(_T("Tool Not supported now : %d"), event.GetId());
			break;
	}


}



void ToolMapFrame::OnZoomPrevious (wxCommandEvent & event)
{
	m_LayerManager->ZoomPrevious();
}


void ToolMapFrame::OnZoomToSelectedLayer(wxCommandEvent & event){
	wxLogMessage(_T("Zoom to selected layer"));
	m_LayerManager->ZoomToSelectedLayer();

}


/***************************************************************************//**
 @brief Called when export selected is pressed
 @details This function create an object of type #tmExportManager
 @author Lucien Schreiber (c) CREALP 2008
 @date 13 November 2008
 *******************************************************************************/
void ToolMapFrame::OnExportSelected (wxCommandEvent & event){
	if (m_PManager->IsProjectOpen() == false){
        return;
    }
    
    // get project def from memory
    PrjDefMemManage * memProj = m_PManager->GetMemoryProjectDefinition();
    PrjDefMemManage myCopyProj;
    myCopyProj = *memProj;
    
    tmExportManager myExport(this, m_PManager->GetDatabase());
    if (myExport.ExportSelected(&myCopyProj, m_LayerManager)==true){
        wxLogDebug(_T("Exporting layer(s) success"));
    }
    else {
        wxLogDebug(_T("Exporting layer(s) failed"));
    }
	
}



/***************************************************************************//**
 @brief Called when export all is pressed
 @details Export all layers without any further user intervention
 @author Lucien Schreiber (c) CREALP 2008
 @date 13 November 2008
 *******************************************************************************/
void ToolMapFrame::OnExportAll (wxCommandEvent & event)
{
	if (m_PManager->IsProjectOpen() == false){
        return;
    }
	
    // get project def from memory
    PrjDefMemManage * memProj = m_PManager->GetMemoryProjectDefinition();
    PrjDefMemManage myCopyProj;
    myCopyProj = *memProj;
    
    tmExportManager myExport(this, m_PManager->GetDatabase());
    if (myExport.ExportAll(&myCopyProj)){
        wxLogDebug(_T("Exporting all project success"));
    }
    else {
        wxLogDebug(_T("Exporting all project failed"));
    }
	
    
}



/***************************************************************************//**
 @brief Called when user press "Search dangling node" menu
 @author Lucien Schreiber (c) CREALP 2009
 @date 13 May 2009
 *******************************************************************************/
void ToolMapFrame::OnDanglingNodes(wxCommandEvent & event)
{
	m_ToolManager->FindDanglingNodes();
}



void ToolMapFrame::OnAbout(wxCommandEvent& event)
{
	tmAboutDLG myDlg(this,wxID_ANY, _("About"));
	myDlg.ShowModal();
}



void ToolMapFrame::OnUpdateMenuProject(wxUpdateUIEvent & event){
	wxASSERT(m_PManager);
	event.Enable(m_PManager->IsProjectOpen());
}



void ToolMapFrame::OnUpdateMenuPreviousZoom(wxUpdateUIEvent & event){
	wxASSERT(m_LayerManager);
	event.Enable(m_LayerManager->HasZoomPrevious());

}


void ToolMapFrame::OnUpdateMenuEditUndo (wxUpdateUIEvent & event){
	wxASSERT(m_EditManager);
	event.Enable(m_EditManager->HasLastVertex());
}

void ToolMapFrame::OnUpdateMenuEditDraw (wxUpdateUIEvent & event){
	wxASSERT(m_EditManager);
	event.Enable( m_EditManager->IsDrawingAllowed());
}

void ToolMapFrame::OnUpdateMenuEditModify (wxUpdateUIEvent & event){
	wxASSERT(m_EditManager);
	tmRenderer * myRenderer = m_MainPanel->GetGISRenderer();
	wxASSERT(myRenderer);
	wxASSERT(m_LayerManager);
	bool bEnabled = m_EditManager->IsModifictionAllowed();
	if (bEnabled == false && myRenderer->GetTool() == tmTOOL_MODIFY) {
		m_LayerManager->OnSelect();
	}
	event.Enable(bEnabled);
}


void ToolMapFrame::OnUpdateMenuEditDelete (wxUpdateUIEvent & event){
	wxASSERT(m_EditManager);
	bool bEnable = false;
	if (m_EditManager->IsDrawingAllowed()==true) {
		if (m_EditManager->GetSelectionCount() >= 1) {
			bEnable = true;
		}
	}
	event.Enable(bEnable);
}


void ToolMapFrame::OnUpdateMenuEditMerge (wxUpdateUIEvent & event){
	wxASSERT(m_EditManager);
	bool bEnable = false;
	if (m_EditManager->IsDrawingAllowed()==true) {
		if (m_EditManager->GetSelectionCount() > 1) {
			bEnable = true;
		}
	}
	event.Enable(bEnable);
}


void ToolMapFrame::OnUpdateMenuEditPointOrient (wxUpdateUIEvent & event){
	wxASSERT(m_EditManager);
	bool bEnable = false;
	if (m_EditManager->IsModifictionAllowed() == true) {
		if (m_EditManager->IsLayerType(LAYER_SPATIAL_POINT)) {
			bEnable = true;
		}
	}
	event.Enable(bEnable);
}


void ToolMapFrame::OnUpdateMenuEditClearSelection (wxUpdateUIEvent & event){
	wxASSERT(m_EditManager);
	bool bReturn = false;
	if (m_EditManager->GetSelectionCount() >= 1) {
		bReturn = true;
	}
	event.Enable(bReturn);
}

void ToolMapFrame::OnUpdateMenuEditQueryRun (wxUpdateUIEvent & event){
	wxASSERT(m_QueriesPanel);
	event.Enable(m_QueriesPanel->IsQuerySelected());
}


void ToolMapFrame::OnUpdateMenuEditQueryAdd (wxUpdateUIEvent & event){
    wxASSERT(m_PManager);
	event.Enable(m_PManager->IsProjectOpen());
}


void ToolMapFrame::OnUpdateMenuShowSnapping (wxUpdateUIEvent & event){
	wxASSERT(m_SnappingPanel);
	event.Check(m_SnappingPanel->IsPanelShown());
}


void ToolMapFrame::OnUpdateMenuShowShortcuts (wxUpdateUIEvent & event){
	wxASSERT(m_ShortCutPanel);
	event.Check(m_ShortCutPanel->IsPanelShown());
}


void ToolMapFrame::OnUpdateMenuShowQuery (wxUpdateUIEvent & event){
	wxASSERT(m_QueriesPanel);
	event.Check(m_QueriesPanel->IsPanelShown());
}

void ToolMapFrame::OnUpdateMenuShowTOC (wxUpdateUIEvent & event){
	wxASSERT(m_TocWindow);
	event.Check(m_TocWindow->IsShown());
}


void ToolMapFrame::OnUpdateMenuShowLog (wxUpdateUIEvent & event){
	wxASSERT(m_LogWindow);
	event.Check(m_LogWindow->GetFrame()->IsShown());
}


void ToolMapFrame::OnUpdateMenuShowInfo (wxUpdateUIEvent & event){
	wxWindow * myInfoWnd = wxWindow::FindWindowById(ID_INFORMATION_DLG);
	bool bCheck = false;
	if (myInfoWnd != NULL && myInfoWnd->IsShown()) {
		bCheck = true;
	}
	event.Check(bCheck);
}


void ToolMapFrame::OnUpdateMenuFlipLine (wxUpdateUIEvent & event){
	wxASSERT(m_EditManager);
	bool bEnable = false;
	if (m_EditManager->IsModifictionAllowed() == true) {
		if (m_EditManager->IsLayerType(LAYER_SPATIAL_LINE)) {
			bEnable = true;
		}
	}
	event.Enable(bEnable);
}


void ToolMapFrame::OnUpdateMenuEditSharedNode (wxUpdateUIEvent & event){
	wxASSERT(m_EditManager);
	bool bEnable = false;
	if (m_TocWindow->GetTOCCtrl()->GetEditLayer() &&
		m_TocWindow->GetTOCCtrl()->GetEditLayer()->GetType() == TOC_NAME_LINES) {
		bEnable = true;
	}
	event.Enable(bEnable);
}


void ToolMapFrame::OnUpdateStatisticsDialog (wxUpdateUIEvent & event){
	wxASSERT(m_StatManager);
	event.Enable(m_StatManager->IsReady());
}






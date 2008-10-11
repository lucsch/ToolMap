/***************************************************************************
								tmlayermanager.h
                    Central point for layer management 
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


#ifndef _TM_LAYERMANAGER_H_
#define _TM_LAYERMANAGER_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "tmtocctrl.h"					// class TOC ctrl
#include "../database/database_tm.h"	// class database
#include "tmgisdata.h"					// for GISdata
#include "tmgisdatavectormysql.h"		// for direct access to GIS mysql
#include "tmrenderer.h"					// for GIS rendering
#include "tmdrawer.h"					// for drawing into bitmaps
#include "tmprogress.h"					// for displaying progress during load
#include "tmscalectrl.h"				// for combo box scale control

// forward declaration
class tmGISLoadingDataThread;
class tmScaleCtrlCombo;

DECLARE_EVENT_TYPE(tmEVT_THREAD_GISDATALOADED, -1)

enum tmTHREAD_STATUS 
{
	tmTHREAD_STOP = 0,
	tmTHREAD_RUN
};

static wxCriticalSection s_SharedDataCritical;

/***************************************************************************//**
 @brief GIS class for dealing with layers
 @details This is the super class for dealing with GIS. This class is the
 central point dealing with : 
 - Database (#DataBaseTM)
 - TOC controls (#tmTOCCtrl)
 - GIS progress indicator (#tmProgressIndicator)
 - ...
 @author Lucien Schreiber (c) CREALP 2008
 @date 07 July 2008
 *******************************************************************************/
class tmLayerManager : public wxEvtHandler 
	{
	private:
		tmTOCCtrl * m_TOCCtrl;
		tmRenderer * m_GISRenderer;
		wxWindow * m_Parent;
		DataBaseTM * m_DB;
		tmGISScale m_Scale;
		tmDrawer m_Drawer;
		tmGISLoadingDataThread * m_Thread;
		tmProgressIndicator * m_Progress;
		wxBitmap * m_Bitmap;
		wxStatusBar * m_StatusBar;
		tmScaleCtrlCombo * m_ScaleCtrl;
		
		// shared member data with thread
		 
		tmTHREAD_STATUS m_Shared_ThreadStatus;
		
		// values for thread
		bool m_computeFullExtent;
		
		// init member values to default
		void InitMemberValue();
		
		// TOC specific functions
		void FillTOCArray();
		bool SaveTOCStatus();
		bool IsOK ();
		
		// scale ctrl functions
		void InitScaleCtrlList ();
		
		// event functions
		void OnZoomRectangleIn (wxCommandEvent & event);
		void OnZoomRectangleOut (wxCommandEvent & event);
		void OnPanFinished (wxCommandEvent & event);
		void OnScrolled (wxCommandEvent & event);
		
		// scrollbars
		void UpdateScrollBars ();
	
		// bitmap specific functions
		void CreateEmptyBitmap (const wxSize & size);
		
		// Reading all layers for extent or drawing
		int ReadLayerExtent(bool loginfo = false);
		int ReadLayerDraw ();
		
		
		DECLARE_EVENT_TABLE()
		
	public:
		// ctor / dtor
		tmLayerManager(wxWindow * parent, tmTOCCtrl * tocctrl, tmRenderer * renderer,
					   wxStatusBar * status, tmScaleCtrlCombo * scalectrl);
		~tmLayerManager();
		
			
		// Init layermanager
		bool InitLayerManager(DataBaseTM * db);
		bool UnInitLayerManager();
		
		
		// layers operations
		void RemoveLayer (wxCommandEvent & event);
		void AddLayer (wxCommandEvent & event);
		static tmGISData * LoadLayer (tmLayerProperties * layerProp);
		
		// event operations
		void OnSizeChange (wxCommandEvent & event);
		void OnUpdateCoordinates (wxCommandEvent &event);
		void OnShowLayer (wxCommandEvent & event);
		void OnScaleChanged (wxCommandEvent & event);
		
		
		// tool operations
		void OnZoomToFit ();
		void OnZoomRectangle();
		void OnPan ();
		void OnSelect();
		
		
		
		bool LoadProjectLayers();
		bool ReloadProjectLayersThreadStart(bool bFullExtent = TRUE, bool bInvalidateFullExt = TRUE);
		void OnReloadProjectLayersDone (wxCommandEvent & event);
		
		
	};




/***************************************************************************//**
 @brief Thread Class for loading GIS data
 @note Don't call this class directly, use the tmLayerManager class
 instead
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 July 2008
 *******************************************************************************/
class tmGISLoadingDataThread : public wxThread
	{
	private:
		bool m_Stop;
		tmTHREAD_STATUS * m_ThreadStatus;
		
		bool ReadLayerExtentThread(); 
		
	protected:
		wxWindow * m_Parent;
		tmTOCCtrl * m_TOC;
		tmGISScale * m_Scale;
		DataBaseTM * m_DB;
		tmDrawer * m_Drawer;
		
	public:
		tmGISLoadingDataThread(wxWindow * parent, tmTOCCtrl * toc,
							   tmGISScale * scale,
							   DataBaseTM * database,
							   tmDrawer * drawer,
							   tmTHREAD_STATUS * threadstatus);
		~tmGISLoadingDataThread();
		virtual void * Entry();
		void StopThread (){m_Stop = TRUE;}
		bool TestDestroyThread();
	};




#endif

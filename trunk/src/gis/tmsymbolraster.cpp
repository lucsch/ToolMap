/***************************************************************************
								tmsymbolraster.cpp
				Deals with GIS raster symbology and associed dialog
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

#include "tmsymbolraster.h"

//TODO: remove this code
#include <wx/mstream.h>
#include <wx/txtstrm.h>
#include <wx/datstrm.h>


tmSymbolRaster::tmSymbolRaster()
{
	m_RasterData.m_bUseColorTransparency = FALSE;
	m_RasterData.m_TransparencyColour = wxColour(*wxBLACK);
	m_RasterData.m_GlobalTransparency = 0;

}

tmSymbolRaster::tmSymbolRaster(const  tmSymbolRaster & origin)
{
	m_RasterData.m_bUseColorTransparency = origin.m_RasterData.m_bUseColorTransparency;
	m_RasterData.m_TransparencyColour = origin.m_RasterData.m_TransparencyColour;
	m_RasterData.m_GlobalTransparency = origin.m_RasterData.m_GlobalTransparency;
}


tmSymbolRaster::~tmSymbolRaster()
{
	
	
}


tmSymbolDLG * tmSymbolRaster::GetSymbolDialog (wxWindow * parent, const wxPoint & dlgpos)
{
	tmSymbolDLGRaster * myDlg = new tmSymbolDLGRaster(parent,SYMBOL_TMSYMBOLDLG_IDNAME,
													  SYMBOL_TMSYMBOLDLG_TITLE,
													  dlgpos);
	myDlg->SetDialogData(m_RasterData);
	
	return myDlg;
}


bool tmSymbolRaster::GetDialogData(tmSymbolDLG * dlg)
{
	m_RasterData = ((tmSymbolDLGRaster *) dlg)->GetDialogData();
	return TRUE;
}



bool tmSymbolRaster::Serialize(tmSerialize &s)
{
	s.EnterObject();
	if(s.IsStoring())
	{
		s << m_RasterData.m_bUseColorTransparency;
		s << m_RasterData.m_TransparencyColour;
		s << m_RasterData.m_GlobalTransparency;
		
	}
	else
	{
		s >> m_RasterData.m_bUseColorTransparency;
		s >> m_RasterData.m_TransparencyColour;
		s >> m_RasterData.m_GlobalTransparency;
	}
	s.LeaveObject();
	
	// return false when the archive encountered an error
	return s.IsOk(); 
}



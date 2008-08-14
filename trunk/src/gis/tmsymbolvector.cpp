/***************************************************************************
								tmsymbolvector.cpp
				Deals with GIS vector symbology and associed dialog
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

#include "tmsymbolvector.h"
#include "tmsymbolvectorline.h"
#include "tmsymbolvectorpoint.h"
#include "tmsymbolvectorpolygon.h"


tmSymbolVector::tmSymbolVector()
{
	
	
}



tmSymbolVector::~tmSymbolVector()
{
	
}


tmSymbolDLG * tmSymbolVector::GetSymbolDialog (wxWindow * parent, const wxPoint & dlgpos)
{
	return new tmSymbolDLG(parent,SYMBOL_TMSYMBOLDLG_IDNAME,
							   SYMBOL_TMSYMBOLDLG_TITLE,
							   dlgpos);
}

 

tmSymbolVector * tmSymbolVector::CreateSymbolVectorBasedOnType (tmLayerProperties * item)
{
	switch (item->m_LayerSpatialType)
	{
		case LAYER_SPATIAL_LINE:
			return new tmSymbolVectorLine();
			break;
		case LAYER_SPATIAL_POINT:
			return new tmSymbolVectorPoint();
			break;
		case LAYER_SPATIAL_POLYGON:
			return new tmSymbolVectorPolygon();
			break;
			
		default:
			wxLogDebug(_T("Symbology for this format not implemented now"));
			return NULL;
			break;
	}

}


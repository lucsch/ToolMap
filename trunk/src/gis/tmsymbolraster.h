/***************************************************************************
								tmsymbolraster.h
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


#ifndef _TM_SYMBOLRASTER_H_
#define _TM_SYMBOLRASTER_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "tmsymbol.h"	// tmSymbol class definition
#include "tmsymboldlgraster.h"	// raster dialog definition

class tmSymbolRaster : public tmSymbol
	{
	private:
		virtual tmSymbolDLG * GetSymbolDialog (wxWindow * parent, const wxPoint & dlgpos);
	protected:
	public:
		tmSymbolRaster();
		~tmSymbolRaster();
	};







#endif

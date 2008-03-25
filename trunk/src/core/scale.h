/***************************************************************************
								scale.h
                   Deals with the scale properties
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


#ifndef _SCALE_H
#define _SCALE_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


class ScaleTM : public wxObject
	{
	private:
	public:
		ScaleTM(){;}
		~ScaleTM(){;}
		
		// static function for converting string
		static bool GetScaleFromLong(const long & scale, wxString & stringscale);
		static long GetScaleFromString (const wxString & scale);
	};




#endif

/***************************************************************************
 tmgisdatarasterweb
 -------------------
 copyright            : (C) 2013 CREALP Lucien Schreiber
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

#ifndef _TM_GISDATARASTER_WEB_H_
#define _TM_GISDATARASTER_WEB_H_

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "tmgisdataraster.h"


class tmGISDataRasterWeb : public tmGISDataRaster
{
private:
    
protected:
public:
    tmGISDataRasterWeb();
    ~tmGISDataRasterWeb();
    
    virtual wxString GetDataSizeAsHtml (int iPrecision = 2);
	virtual bool Open (const wxString & filename, bool bReadWrite = TRUE);
};

#endif

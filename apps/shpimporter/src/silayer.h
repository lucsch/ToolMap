/***************************************************************************
 silayer.h
 -------------------
 copyright            : (C) 2012 CREALP Lucien Schreiber
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
#ifndef _SILAYER_H_
#define _SILAYER_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/filename.h>
#include <wx/textfile.h>


#include "sikind.h"
#include "siattribut.h"
#include "database.h"
#include "databaseresult.h"

enum SILAYER_TYPE {
    SILAYER_TYPE_POINT = 0,
    SILAYER_TYPE_LINE,
    SILAYER_TYPE_POLYGON,
    SILAYER_TYPE_UNKNOWN = -1
};


class siLayer {
private:
    wxFileName m_LayerNameIn;
    long m_LayerIndexOut;
    DataBase * m_Database;
    siKind m_Kind;
    siAttribut m_Attributs;
    SILAYER_TYPE m_LayerType;
    
public:
    siLayer(const wxString & layerpath, DataBase * database);
    virtual ~siLayer();
    
    bool LoadFromFile(const wxString & filename);
    bool Process();
    
    const siKind GetKindRef();
    inline const SILAYER_TYPE GetLayerType() const;
    
};


inline const SILAYER_TYPE siLayer::GetLayerType() const {
    return m_LayerType;
}

#endif
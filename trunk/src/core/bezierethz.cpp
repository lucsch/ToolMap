/***************************************************************************
 bezierethz
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


#include "bezierethz.h"



BezierEthz::BezierEthz (double width, int maxpoints){
    m_Width = width;
    m_MaxPoints = maxpoints;
    
    
}


BezierEthz::~BezierEthz(){
    m_Points.DeleteContents(true);
    m_Points.Clear();
}



void BezierEthz::_BezierPoint (const wxRealPoint & A, const wxRealPoint & B, wxRealPoint & result, double t){
    result.y = A.y + (double) (B.y - A.y) * t;
    result.x = A.x + (double) (B.x - A.x) * t;
}


bool BezierEthz::_Segmentation (const wxRealPoint & P1, const wxRealPoint & P2, const wxRealPoint & P3, const wxRealPoint & P4, double t, wxRealPoint & result){
    wxRealPoint L2;
    wxRealPoint L3;
    wxRealPoint R2;
    wxRealPoint R3;
    wxRealPoint H;
    
    if (t <= 0 || t >= 1.0) {
        return false;
    }
    _BezierPoint(P1, P2, L2, t);
    _BezierPoint(P2, P3, H, t);
    _BezierPoint(L2, H, L3, t);
    _BezierPoint(P3, P4, R3, t);
    _BezierPoint(H, R3, R2, t);
    _BezierPoint(L3, R2, result, t);
    return true;
}


void BezierEthz::Init (wxRealPoint P1, wxRealPoint P2, wxRealPoint P3, wxRealPoint P4){
    double myLeftVal = (double) 1.0 / m_MaxPoints;
    m_Points.DeleteContents(true);
    m_Points.Clear();
    
    // segmentation
    double myInterval = myLeftVal;
    while (1) {
        wxRealPoint myResult;
        if (_Segmentation(P1, P2, P3, P4, myInterval, myResult) == false) {
            break;
        }
        myInterval += myLeftVal;
        m_Points.push_back(new wxRealPoint(myResult));
    }
    
    // simplification
    
}



#include <wx/listimpl.cpp>
WX_DEFINE_LIST(wxRealPointList);



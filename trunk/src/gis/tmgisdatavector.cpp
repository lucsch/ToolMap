/***************************************************************************
								tmgisdatavector.cpp
                   Super class for dealing with vector GIS data
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

#include "tmgisdatavector.h"
#include "tmgisdatavectorshp.h"
#include "tmgisdatavectormysql.h"


tmGISDataVector::tmGISDataVector()
{

}



tmGISDataVector::~tmGISDataVector()
{

}


wxString tmGISDataVector::GetAllVectorGISFormatsWildcards()
{
	// Don't return mysql wildcard, used only internally
	wxString myWildcards = _T("");
	
	myWildcards.Append(tmGISDATA_VECTOR_TYPE_WILDCARDS[tmGIS_VECTOR_SHAPEFILE - tmGISVECTOR_OFFSET]);
	return myWildcards;
}


tmGISDataVector * tmGISDataVector::CreateGISVectorBasedOnType(const int & gis_format_index)
{
	switch (gis_format_index)
	{
		case tmGIS_VECTOR_SHAPEFILE:
			return new tmGISDataVectorSHP();
			break;
		case tmGIS_VECTOR_MYSQL:
			return new tmGISDataVectorMYSQL();
			break;
	}
	return NULL;
}



tmGISDataVector * tmGISDataVector::CreateGISVectorBasedOnExt (const wxString & extension)
{
	int iLoop = sizeof(tmGISDATA_VECTOR_TYPE_EXTENSION) / sizeof(wxString);
	for (int i = 0; i< iLoop; i++)
	{
		if (tmGISDATA_VECTOR_TYPE_EXTENSION[i].Contains(extension))
			return CreateGISVectorBasedOnType(i+tmGISVECTOR_OFFSET);
	}
	return NULL;
}




void tmGISDataVector::InitGISDriversVector()
{
	OGRRegisterAll();
}



/***************************************************************************//**
 @brief Formating fields info into html string
 @return  The html compliant string
 @author Lucien Schreiber (c) CREALP 2008
 @date 23 October 2008
 *******************************************************************************/
wxString tmGISDataVector::GetFieldsMetadata ()
{
	wxString myResult = _("<B><U>Fields info</U></B><BR>");
	int iNbFields = GetFieldsCount();
	if (iNbFields < 0)
	{
		myResult.Append(_("Error getting fields info<BR>"));
		return myResult;
	}
	
	myResult.Append(wxString::Format(_("Number of field(s) : %d<BR>"), iNbFields));
	
	wxArrayString myFieldName;
	if (GetFieldsName(myFieldName)==false)
	{
		myResult.Append(_("Error getting fields name<BR>"));
		return myResult;
	}
	
	myResult.Append(_T("<OL>"));
	
	for (unsigned int i =0; i<myFieldName.GetCount();i++)
		myResult.Append(_T("<LI>") + myFieldName.Item(i) + _T("</LI>"));
	
	myResult.Append(_T("</OL><BR>"));

	
	return myResult;
}



/***************************************************************************//**
 @brief Compute real GEOS intersection (not bounding box)
 @details This function may be used for ensuring that a geometry really
 intersects another one
 @note Needs GEOS library
 @param rect GEOS geometry of intersecting rectangle
 @param object GEOS geometry of object
 @return  true if object really intersects rect.
 @author Lucien Schreiber (c) CREALP 2008
 @date 31 October 2008
 *******************************************************************************/
bool tmGISDataVector::CheckGEOSIntersection(GEOSGeom * rect, GEOSGeom * object)
{
	char result = GEOSIntersects(*object, *rect);
	if (result == 1)
		return true;
	return false;
}


/***************************************************************************//**
 @brief Create a GEOS geometry
 @details This function may be used for creating a GEOS compatible geometry from
 an OGR object
 @note Needs GEOS library
 @warning Don't forget to destroy Object
 returned with : GEOSGeom_destroy(GEOSGeometry* g)
 @param geom A valid OGR geometry
 @return Valid GEOS object or NULL if an error occur. Don't forget
 to destroy the returned object with GEOSGeom_destroy(GEOSGeometry* g);
 @author Lucien Schreiber (c) CREALP 2008
 @date 31 October 2008
 *******************************************************************************/
GEOSGeom tmGISDataVector::CreateGEOSGeometry (OGRGeometry * geom)
{
	GEOSGeom geosgeom =  geom->exportToGEOS();

	return geosgeom;
}



/***************************************************************************//**
 @brief Create a GEOS geometry
 @details This function may be used for creating a GEOS compatible geometry from
 an real rectangle
 @note Needs GEOS library
 @warning Don't forget to destroy Object returned with :
 GEOSGeom_destroy(GEOSGeometry* g)
 @param rect A #tmRealRect object
 @return Valid GEOS object or NULL if an error occur. Don't forget
 to destroy the returned object with GEOSGeom_destroy(GEOSGeometry* g);
 @author Lucien Schreiber (c) CREALP 2008
 @date 31 October 2008
 *******************************************************************************/
GEOSGeom  tmGISDataVector::CreateGEOSGeometry (const tmRealRect & rect)
{
	// creating using wkt
	wxString sRect = wxString::Format(_T("POLYGON ((%f %f,%f %f,%f %f,%f %f,%f %f))"),
									  rect.x_min, rect.y_min,
									  rect.x_max, rect.y_min,
									  rect.x_max, rect.y_max,
									  rect.x_min, rect.y_max,
									  rect.x_min, rect.y_min);
	// conversion Unicode wxString -> const char *
	char * buffer = new char [sRect.Length()+2];
	strcpy(buffer, (const char*)sRect.mb_str(wxConvUTF8));
	GEOSGeom  grect = GEOSGeomFromWKT(buffer);
		
	return grect;
}



/***************************************************************************//**
 @brief Create a OGR geometry
 @param rect object of type : #tmRealRect
 @return  return an OGRgeometry or null
 @author Lucien Schreiber (c) CREALP 2008
 @date 31 October 2008
 *******************************************************************************/
OGRGeometry * tmGISDataVector::CreateOGRGeometry (const tmRealRect & rect)
{
	
	wxString sRect = wxString::Format(_T("POLYGON ((%f %f,%f %f,%f %f,%f %f,%f %f))"),
									  rect.x_min, rect.y_min,
									  rect.x_max, rect.y_min,
									  rect.x_max, rect.y_max,
									  rect.x_min, rect.y_max,
									  rect.x_min, rect.y_min);
	// conversion Unicode wxString -> const char *
	char * buffer = new char [sRect.Length()+2];
	strcpy(buffer, (const char*)sRect.mb_str(wxConvUTF8));
	
	
	OGRGeometry * geom = NULL;
	OGRGeometryFactory::createFromWkt(&buffer, NULL, &geom);
	
	return geom;
}


/***************************************************************************//**
 @brief Search if a vertex intersect the passed geometry
 @details Check for all geometry (point, lines, polygon) if a vertex intersect
 the passed geometry. If true this vertex is returned
 @param geometry A valid geometry (point, line, polygon)
 @param buffer The buffer for intersection
 @return  the vertex intersecting the geometry (dont forget to delete) or null
 if nothing found
 @author Lucien Schreiber (c) CREALP 2009
 @date 29 January 2009
 *******************************************************************************/
wxRealPoint * tmGISDataVector::GetVertexIntersection(OGRGeometry * geometry, 
													 OGRGeometry * buffer)
{
	OGRPoint * myPointLine = (OGRPoint*) OGRGeometryFactory::createGeometry(wkbPoint);
	
	// should not be deleted, belong to OGR
	OGRLineString * myLineLine = NULL;
	OGRPolygon * myPoly = NULL;
	OGRPoint * myPoint = NULL;
	
	int i = 0;
	wxRealPoint * ptReturn = NULL;
	//bool bBreak = false;
	
	switch (wkbFlatten(geometry->getGeometryType()))
	{
		case wkbLineString:
			myLineLine = (OGRLineString*) geometry;
			for (i=0; i<myLineLine->getNumPoints();i++)
			{
				myLineLine->getPoint(i, myPointLine);
				if (myPointLine && myPointLine->Intersect(buffer))
				{
					ptReturn = new wxRealPoint(myPointLine->getX(),
											   myPointLine->getY());
					break;
				}
				
			}
			OGRGeometryFactory::destroyGeometry(myPointLine);
			break;
		
			
			
			
		case wkbPoint:
			myPoint = (OGRPoint*) geometry;
			if (myPoint && myPoint->Intersect(buffer))
			{
				ptReturn = new wxRealPoint(myPoint->getX(),
										   myPoint->getY());
			}
			break;

			
			
			
		case wkbPolygon:
			myPoly = (OGRPolygon*) geometry;
			// exterior ring
			myLineLine = (OGRLineString*) myPoly->getExteriorRing();
			ptReturn = GetVertexIntersection(myLineLine, buffer);
			
			// interior ring if needed
			if (!ptReturn)
			{
				for (i = 0; i<myPoly->getNumInteriorRings();i++)
				{
					myLineLine = (OGRLineString*) myPoly->getInteriorRing(i);
					if (!myLineLine)
						break;
					
					ptReturn = GetVertexIntersection(myLineLine, buffer);
					if (ptReturn)
						break;
				}
			}
			break;
		
		
		default:
			break;
	}
	
	return ptReturn;
}



/***************************************************************************//**
 @brief Search if a Begin/End vertex intersect the passed geometry
 @details This function is very close to the 
 tmGISDataVector::GetVertexIntersection but search only for begin / end vertex.
 So it works only for lines
 @param geometry A valid geometry (line ONLY)
 @param buffer The buffer for intersection
 @return  the vertex intersecting the geometry (dont forget to delete) or null 
 if nothing found
 @author Lucien Schreiber (c) CREALP 2009
 @date 29 January 2009
 *******************************************************************************/
wxRealPoint * tmGISDataVector::GetBeginEndInterseciton (OGRGeometry * geometry, 
														OGRGeometry * buffer)
{
	wxRealPoint * ptReturn = NULL;
	
	if (wkbFlatten(geometry->getGeometryType()) == wkbLineString)
	{
		OGRLineString *	myLine = (OGRLineString*) geometry;
		int iPointLine = myLine->getNumPoints();
		
		
		OGRPoint * myPoint = (OGRPoint*) OGRGeometryFactory::createGeometry(wkbPoint);
		OGRPoint * myPoint2 = (OGRPoint*) OGRGeometryFactory::createGeometry(wkbPoint);
		
		myLine->getPoint(0, myPoint);
		myLine->getPoint(iPointLine-1, myPoint2);
		
		if (myPoint->Intersect(buffer))
		{
			ptReturn = new wxRealPoint(myPoint->getX(),
									   myPoint->getY());
			
		}
		else if (myPoint2->Intersect(buffer))
		{
			ptReturn = new wxRealPoint(myPoint2->getX(),
									   myPoint2->getY());
		}
		
		OGRGeometryFactory::destroyGeometry(myPoint);
		OGRGeometryFactory::destroyGeometry(myPoint2);
		
	}
	
	return ptReturn;
}


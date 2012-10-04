/***************************************************************************
								tmexportdatashp.cpp
                    Class for the process of exporting data as SHP
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

#include "tmexportdatashp.h"
#include "../database/database_tm.h"



/***************************************************************************//**
 @brief Init class member values
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 November 2008
 *******************************************************************************/
void tmExportDataSHP::InitMemberValues()
{
	m_Extension = _T(".shp");
	m_Frame = NULL;
    m_Shp = NULL;
}



/***************************************************************************//**
 @brief Constructor
 @details Use tmExportDataSHP::Create() for two step initialization
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 November 2008
 *******************************************************************************/
tmExportDataSHP::tmExportDataSHP()
{
	InitMemberValues();
}



/***************************************************************************//**
 @brief Constructor
 @param database A valid #DataBaseTM object (an assert is done in debug mode)
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 November 2008
 *******************************************************************************/
tmExportDataSHP::tmExportDataSHP (DataBaseTM * database)
{
	InitMemberValues();
	Create(database);
}



/***************************************************************************//**
 @brief Creator
 @details For two step initialization. In case of use of the default constructor
 @param database A valid #DataBaseTM object (an assert is done in debug mode)
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 November 2008
 *******************************************************************************/
void tmExportDataSHP::Create (DataBaseTM * database)
{
	tmExportData::Create(database);
}


/***************************************************************************//**
 @brief destructor
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 November 2008
 *******************************************************************************/
tmExportDataSHP::~tmExportDataSHP()
{
	if (m_Frame){
		OGRGeometryFactory::destroyGeometry(m_Frame);
    }
    wxDELETE(m_Shp);
}



/***************************************************************************//**
 @brief Create empty file
 @details Create a file  ProjectDefMemoryLayers informations, path and extension
 specified in class
 @param myLayer contain the name of the file (without extension) to create
 @param path path where we want the file to be created
 @return  true if file created with sucess, false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 November 2008
 *******************************************************************************/
bool tmExportDataSHP::CreateEmptyExportFile (ProjectDefMemoryLayers * myLayer,
											 const wxString & path)
{
	bool bReturn = true;
	wxASSERT(myLayer);
    wxASSERT(m_Shp == NULL);
	wxFileName * myShpFileName = GetFileName(myLayer, path);
	if (!myShpFileName)
	{
		wxLogError(_("Unable to get the file name !"));
		return false;
	}

    if (myLayer->m_LayerType == LAYER_POLYGON) {
        m_Shp = new tmGISDataVectorSHPMemory();
    }
    else{
        m_Shp = new tmGISDataVectorSHP();
    }

	if(m_Shp->CreateFile(myShpFileName->GetFullPath(), (int) myLayer->m_LayerType) == false){
		bReturn =  false;
	}

	wxDELETE(myShpFileName);
	return bReturn;

}



/***************************************************************************//**
 @brief Add optional fields to the Shp
 @param myfields array of fields for this shapefile
 @return  true if fields were added successfully, false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 15 November 2008
 *******************************************************************************/
bool tmExportDataSHP::AddOptFields (const PrjMemFieldArray & myfields)
{

	bool bReturn = true; // if adding fields failed, stop
	for (unsigned int i = 0; i< myfields.GetCount(); i++)
	{
		ProjectDefMemoryFields * field = myfields.Item(i);
		wxASSERT(field);
		int iSize = 0;

		switch (field->m_FieldType)
		{
			case TM_FIELD_TEXT:
				bReturn = m_Shp->AddFieldText(field->m_Fieldname, field->m_FieldPrecision);
				break;

			case TM_FIELD_INTEGER:
				bReturn = m_Shp->AddFieldNumeric(field->m_Fieldname, false);
				break;

			case TM_FIELD_FLOAT:
				bReturn = m_Shp->AddFieldNumeric(field->m_Fieldname, true);
				break;

			case TM_FIELD_ENUMERATION:
				// compute max size for enum
				iSize = GetSizeOfEnum(field->m_pCodedValueArray);
				bReturn = m_Shp->AddFieldText(field->m_Fieldname, iSize);
				break;

			case TM_FIELD_DATE:
				bReturn = m_Shp->AddFieldDate(field->m_Fieldname);
				break;

			default:
				bReturn = false;
				break;
		}

		if (!bReturn)
			break;

	}

	return bReturn;
}



/***************************************************************************//**
 @brief Adding generic fields
 @details Adding field :
 - OBJECT_CD
 - OBJECT_DESC
 @return  true if fields added successfully
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 November 2008
 *******************************************************************************/
bool tmExportDataSHP::AddGenericFields (int iObjeDescSize){
	wxASSERT (iObjeDescSize);

	if (m_Shp->AddFieldNumeric(_T("OBJ_CD")) &&
		m_Shp->AddFieldText(_T("OBJ_DESC"), iObjeDescSize)){
		return true;
	}

	return false;

}


/***************************************************************************//**
 @brief Adding Field for storing ToolMap FID
 @details Add a first field containing ToolMap's FID. This is needed for adding
 advanced attribution because setting FID doesn't work with Shapefile (GDAL Bug
 ???). Added field name is TM_OID
 @return  true if field added successfully
 @author Lucien Schreiber (c) CREALP 2009
 @date 26 March 2009
 *******************************************************************************/
bool tmExportDataSHP::AddFIDField ()
{
	if(m_Shp->AddFieldNumeric(_T("TM_OID"), false)==false){
		wxLogError(_("Adding OID field failed"));
		return false;
	}

	return true;
}



/***************************************************************************//**
 @brief Write all geometrics lines to the shp
 @param myLayer object containing info on the actual layer, such as layer_ID or
 layer_Name
 @return  false if write failled, true otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 November 2008
 *******************************************************************************/
bool tmExportDataSHP::WriteLines (ProjectDefMemoryLayers * myLayer)
{
	wxASSERT(m_Frame);
	wxASSERT(m_pDB);
	wxASSERT(m_pDB->DataBaseHasResults() == true);

	// get row of data
	DataBaseResult myResult;
	m_pDB->DataBaseGetResults(&myResult);
	wxASSERT(myResult.HasResults()==true);

	for (long i = 0; i < myResult.GetRowCount(); i++) {
		myResult.NextRow();

		//
		// Is geometry inside the frame ?
		//
		OGRGeometry * myGeom = NULL;
		if ( myResult.GetValue(1, &myGeom) == false){
			wxASSERT(myGeom == NULL);
			wxLogError(_T("No geometry returned for '%s' in loop %d"),
					   myLayer->m_LayerName.c_str(), i);
			continue;
		}

		long myOid = wxNOT_FOUND;
		myResult.GetValue(0, myOid);
		wxASSERT(myOid != wxNOT_FOUND);


		if (myGeom == NULL) {
			wxLogError(_("No geometry found for OID : %ld - Layer : '%s'"),
					   myOid, myLayer->m_LayerName.c_str());
			continue;
		}


		OGRGeometry * myCropLine = SafeIntersection(myGeom, m_Frame);
		OGRGeometryFactory::destroyGeometry(myGeom);

		if (myCropLine == NULL) {
			continue;
		}

		if (myCropLine->IsEmpty() == true) {
			OGRGeometryFactory::destroyGeometry(myCropLine);
			continue;
		}

		//
		// Add geometry first and then values
		//
		m_Shp->AddGeometry(myCropLine, myOid);
		OGRGeometryFactory::destroyGeometry(myCropLine);

		// basic attribution
		if(SetAttributsBasic(myResult)==false){
			m_Shp->CloseGeometry();
			wxLogError(_("Unable to set basic attribution for OID : %ld"), myOid);
			continue;
		}


		// advanced attribution
		if (SetAttributsAdvanced(myResult, myLayer)==false) {
			m_Shp->CloseGeometry();
			wxLogError(_("Unable to set advanced attribution for OID : %ld"), myOid);
			continue;
		}

		m_Shp->CloseGeometry();
	}
	return true;
}




/***************************************************************************//**
 @brief Write all geometrics points to the shp
 @param myLayer object containing info on the actual layer, such as layer_ID or
 layer_Name
 @return  false if write failled, true otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 November 2008
 *******************************************************************************/
bool tmExportDataSHP::WritePoints (ProjectDefMemoryLayers * myLayer)
{
	wxASSERT(m_Frame);
	wxASSERT(m_pDB);
	wxASSERT(m_pDB->DataBaseHasResults() == true);

	// get row of data
	DataBaseResult myResult;
	m_pDB->DataBaseGetResults(&myResult);
	wxASSERT(myResult.HasResults()==true);

	for (long i = 0; i < myResult.GetRowCount(); i++) {
		myResult.NextRow();

		//
		// Is geometry inside the frame ?
		//
		OGRGeometry * myGeom = NULL;
		if ( myResult.GetValue(1, &myGeom) == false){
			wxASSERT(myGeom == NULL);
			wxLogError(_T("No geometry returned for '%s' in loop %ld"),
					   myLayer->m_LayerName.c_str(), i);
			continue;
		}

		long myOid = wxNOT_FOUND;
		myResult.GetValue(0, myOid);
		wxASSERT(myOid != wxNOT_FOUND);


		if (myGeom == NULL) {
			wxLogError(_("No geometry found for OID : %ld - Layer : '%s'"),
					   myOid, myLayer->m_LayerName.c_str());
			continue;
		}

		if (myGeom->Intersect(m_Frame)==false) {
			OGRGeometryFactory::destroyGeometry(myGeom);
			continue;
		}

		//
		// Add geometry first and then values
		//
		m_Shp->AddGeometry(myGeom, myOid);
		OGRGeometryFactory::destroyGeometry(myGeom);

		// basic attribution
		if(SetAttributsBasic(myResult)==false){
			m_Shp->CloseGeometry();
			wxLogError(_("Unable to set basic attribution for OID : %ld"), myOid);
			continue;
		}


		// advanced attribution
		if (SetAttributsAdvanced(myResult, myLayer)==false) {
			m_Shp->CloseGeometry();
			wxLogError(_("Unable to set advanced attribution for OID : %ld"), myOid);
			continue;
		}

		m_Shp->CloseGeometry();
	}
	return true;
}



bool tmExportDataSHP::WriteLabels (ProjectDefMemoryLayers * myLayer){
	wxASSERT(m_Frame);
	wxASSERT(m_pDB);
	wxASSERT(m_pDB->DataBaseHasResults() == true);

    /* create spatial index
    m_Shp.CreateSpatialIndex();
    wxFileName myName (m_Shp.GetFullFileName());
    m_Shp.Close();
    if (m_Shp.Open(myName.GetFullPath(), true)==false){
        m_pDB->DataBaseClearResults();
        return false;
    }
    */
    
    // rasterize polygons
    double myRasterizeFactor = 1;
    if (m_Shp->Rasterize(myRasterizeFactor) == false) {
        m_pDB->DataBaseClearResults();
        return false;
    }
    
    // create spatial join field
    wxString mySpatialJoinFieldName = _T("NB_LABELS");
    m_Shp->AddFieldNumeric(mySpatialJoinFieldName);
    
	// get row of data
	DataBaseResult myResult;
	m_pDB->DataBaseGetResults(&myResult);
	wxASSERT(myResult.HasResults()==true);
   
    long mySkippedPoly = 0;
	for (long i = 0; i < myResult.GetRowCount(); i++) {
		myResult.NextRow();

		//
		// Is geometry inside the frame ?
		//
		OGRGeometry * myGeom = NULL;
		if ( myResult.GetValue(1, &myGeom) == false){
			wxASSERT(myGeom == NULL);
			wxLogError(_T("No geometry returned for '%s' in loop %ld"),
					   myLayer->m_LayerName.c_str(), i);
			continue;
		}

		long myOid = wxNOT_FOUND;
		myResult.GetValue(0, myOid);
		wxASSERT(myOid != wxNOT_FOUND);


		if (myGeom == NULL) {
			wxLogError(_("No geometry found for OID : %ld - Layer : '%s'"),
					   myOid, myLayer->m_LayerName.c_str());
			continue;
		}

		if (myGeom->Intersect(m_Frame)==false) {
			OGRGeometryFactory::destroyGeometry(myGeom);
			continue;
		}
        
		// Search intersection with polygons, using rasterization and fallback to rigorous method!
        long myFid = wxNOT_FOUND;
        if (myRasterizeFactor != 0) {
            myFid = m_Shp->GetFeatureIDIntersectedOnRaster((OGRPoint*) myGeom);

        }
        if (myFid == wxNOT_FOUND) {
            mySkippedPoly++;
            wxLogDebug(_("Skipped label with OID: %ld"), myOid);
            // search using polygon.
            myFid = m_Shp->GetFeatureIDIntersectedBy(myGeom);
            if (myFid == wxNOT_FOUND) {
                wxLogError(_("Label %ld is inside the frame but doesn't belong to any polygon: Ignored!"), myOid);
                OGRGeometryFactory::destroyGeometry(myGeom);
                continue;
            }
        }
        OGRGeometryFactory::destroyGeometry(myGeom);
        wxASSERT(myFid != wxNOT_FOUND);
        m_Shp->SelectFeatureByOID(myFid);
        
        // update spatial join (labels - polygons)
        int mySpatialJoinValue = 1;
        if (m_Shp->GetFieldNumeric(mySpatialJoinFieldName, mySpatialJoinValue) == true) {
            mySpatialJoinValue++;
        }
        m_Shp->SetFieldNumeric(mySpatialJoinFieldName, mySpatialJoinValue);
        
		// basic attribution
		if(SetAttributsBasic(myResult)==false){
			m_Shp->CloseGeometry();
			wxLogError(_("Unable to set basic attribution for OID : %ld"), myOid);
			continue;
		}

		// advanced attribution
		if (SetAttributsAdvanced(myResult, myLayer)==false) {
			m_Shp->CloseGeometry();
			wxLogError(_("Unable to set advanced attribution for OID : %ld"), myOid);
			continue;
		}
		m_Shp->CloseGeometry();
	}
    
    wxLogMessage(_("%ld / %ld polygons not found using raster (%.2f\%)"), mySkippedPoly, myResult.GetRowCount(), (mySkippedPoly * 1.0 / myResult.GetRowCount()) * 100);
    // copy from memory to SHP
    m_Shp->CopyToFile(m_Shp->GetFullFileName(), _T("ESRI Shapefile"));
    
    // remove rasterized file
    //m_Shp->RemoveRasterizeFile();
	return true;
}



void tmExportDataSHP::_AppendValidToCollection(OGRGeometry * geometry, OGRGeometryCollection * collection){
    if (geometry == NULL) {
        return;
    }
    
    if (geometry->IsEmpty() == true) {
        OGRGeometryFactory::destroyGeometry(geometry);
        return;
    }
    
    collection->addGeometry(geometry);
    OGRGeometryFactory::destroyGeometry(geometry);
}



/***************************************************************************//**
 @brief Compute polygons from lines
 @details This function uses GEOS for computing polygon from lines
 @param myLayer Informations about the current layer
 @return  true if polygons were created successfully
 @author Lucien Schreiber (c) CREALP 2008
 @date 17 November 2008
 *******************************************************************************/
bool tmExportDataSHP::WritePolygons (ProjectDefMemoryLayers * myLayer)
{
	wxASSERT(m_Frame);
	wxASSERT(m_pDB);
	wxASSERT(m_pDB->DataBaseHasResults() == true);

	// get row of data
	DataBaseResult myResult;
	m_pDB->DataBaseGetResults(&myResult);
	wxASSERT(myResult.HasResults()==true);


	//
	// Is geometry inside the frame ?
	//
	
	// Crop with a bigger frame to ensure all intersections will be created
	// Bug #140
	OGRGeometry * myBigFrame =  SafeBuffer(m_Frame, 1);
	wxASSERT(myBigFrame);
	
	OGRMultiLineString * myNodedLines = (OGRMultiLineString*) OGRGeometryFactory::createGeometry(wkbMultiLineString);
	for (long i = 0; i < myResult.GetRowCount(); i++) {
		myResult.NextRow();

		OGRGeometry * myGeom = NULL;
		if ( myResult.GetValue(1, &myGeom) == false){
			wxASSERT(myGeom == NULL);
			wxLogError(_T("No geometry returned for '%s' in loop %ld"),
					   myLayer->m_LayerName.c_str(),i);
			continue;
		}

		long myOid = wxNOT_FOUND;
		myResult.GetValue(0, myOid);
		wxASSERT(myOid != wxNOT_FOUND);


		if (myGeom == NULL) {
			wxLogError(_("No geometry found for OID : %ld - Layer : '%s'"),
					   myOid,myLayer->m_LayerName.c_str());
			continue;
		}

        OGRGeometry * myCropLine = SafeIntersection(myGeom, myBigFrame);
		//OGRGeometry * myCropLine = myGeom->Intersection(myBigFrame);
        OGRwkbGeometryType myType = wkbFlatten( myCropLine->getGeometryType());
        if (myType == wkbMultiLineString) {
            wxLogDebug(_("Multi lines encountered for OID : %ld"), myOid);
            OGRMultiLineString * myCropedLines = (OGRMultiLineString*) myCropLine;
            for (int f = 0; f< myCropedLines->getNumGeometries(); f++) {
                OGRGeometry * myfLine = myCropedLines->getGeometryRef(f)->clone();
                _AppendValidToCollection(myfLine, myNodedLines);
            }
            
        }
        else {
            _AppendValidToCollection(myCropLine, myNodedLines);
        }
		OGRGeometryFactory::destroyGeometry(myGeom);

		
	}

	OGRGeometryFactory::destroyGeometry(myBigFrame);

	//
	// Union frame with cropped lines
	//

	// transform non standard OGRLinearRing -> OGRLineString
	// needed, otherwise union wont work !!!
	OGRLinearRing * myFrame = m_Frame->getExteriorRing();
	OGRLineString * myLineString = (OGRLineString*) OGRGeometryFactory::createGeometry(wkbLineString);
	myLineString->setCoordinateDimension(2);
	for (int p= 0; p<myFrame->getNumPoints();p++)
	{
		OGRPoint * myPoint = (OGRPoint*) OGRGeometryFactory::createGeometry(wkbPoint);
		myFrame->getPoint(p,myPoint);
		myLineString->addPoint(myPoint);
		OGRGeometryFactory::destroyGeometry(myPoint);
	}

	OGRGeometry * myLines = myNodedLines->Union(myLineString);
	wxASSERT(myLines);
	OGRGeometryFactory::destroyGeometry(myNodedLines);
	OGRGeometryFactory::destroyGeometry(myLineString);

	int iTotalLines = ((OGRMultiLineString *) myLines)->getNumGeometries();
	wxLogMessage(_("%d lines for creating polygons"), iTotalLines);

    
    /* temp code for exporting lines
    not working !!! 
    tmGISDataVectorSHP myLineShp;
    wxFileName myLineFileName (m_Shp.GetFullFileName());
    myLineFileName.SetName(myLineFileName.GetName() + _T("-line"));
    wxString myLineFileNameTxt = myLineFileName.GetFullPath();
    myLineShp.CreateFile(myLineFileName, wkbLineString);
    for (int l = 0; l< iTotalLines; l++) {
        OGRMultiLineString * myLinesCollection = (OGRMultiLineString*) myLines;
        myLineShp.AddGeometry(myLinesCollection->getGeometryRef(l), l);
    }*/
    
	
	//
	// Create polygons
	//
	GEOSGeom * ahInGeoms = (GEOSGeom *) CPLCalloc(sizeof(void*),iTotalLines);
	for(int i = 0; i < iTotalLines; i++ ){
		ahInGeoms[i] = ((OGRMultiLineString*) myLines)->getGeometryRef(i)->exportToGEOS();
	}

    GEOSGeom hResultGeom = GEOSPolygonize( ahInGeoms, iTotalLines );

	// cleaning
	for (int h = 0; h< iTotalLines; h++)
	{
		GEOSGeom_destroy(ahInGeoms[h]);
	}
	CPLFree(ahInGeoms);
	OGRGeometryFactory::destroyGeometry(myLines);


	int myNbPoly = GEOSGetNumGeometries(hResultGeom);
	// save polygon to shp
	for (int j = 0; j < myNbPoly;j++)
	{
		const GEOSGeometry * myActualPoly = GEOSGetGeometryN(hResultGeom, j);
		GEOSGeom myActualPolyClone = GEOSGeom_clone(myActualPoly);
		OGRGeometry * myPoly = SafeCreateFromGEOS(myActualPolyClone);
		GEOSGeom_destroy(myActualPolyClone);
		m_Shp->AddGeometry(myPoly, -1);
		m_Shp->CloseGeometry();
		OGRGeometryFactory::destroyGeometry(myPoly);
	}
	GEOSGeom_destroy(hResultGeom);
	return true;
}





bool tmExportDataSHP::SetAttributsBasic(DataBaseResult & results){

	if (results.HasResults() == false || results.IsRowOk() == false) {
		return false;
	}

	// get results
	wxString myObjectCD = wxEmptyString;
	wxString myObjectDesc = wxEmptyString;

	if (results.GetValue(2, myObjectCD)==false) {
		wxLogError(_("Unable to get the Object Code"));
		return false;
	}

	if (results.GetValue(3, myObjectDesc)==false) {
		wxLogError(_("Unable to get the object Description"));
		return false;
	}

	m_Shp->SetFieldValue(myObjectCD, TM_FIELD_INTEGER, 1);
	m_Shp->SetFieldValue(myObjectDesc, TM_FIELD_TEXT, 2);
	return true;
}


bool tmExportDataSHP::SetAttributsAdvanced(DataBaseResult & results,
										   ProjectDefMemoryLayers * layer){
	wxASSERT(layer);
	if (layer->m_pLayerFieldArray.GetCount() == 0) {
		// no advanced attribution
		return true;
	}


	if (results.HasResults() == false || results.IsRowOk() == false) {
		return false;
	}


	for (unsigned int i = 0; i<layer->m_pLayerFieldArray.GetCount(); i++) {
		wxString myValue = wxEmptyString;
		if (results.GetValue(i + 4, myValue) == false) {
			continue;
		}
        
        // special case if field is enumeration
        if (layer->m_pLayerFieldArray[i]->m_FieldType == TM_FIELD_ENUMERATION) {
            long myValueID = wxNOT_FOUND;
            myValue.ToLong(&myValueID);
            for (unsigned int c = 0; c < layer->m_pLayerFieldArray[i]->m_pCodedValueArray.GetCount(); c++) {
                ProjectDefMemoryFieldsCodedVal * myCVal = layer->m_pLayerFieldArray[i]->m_pCodedValueArray[c];
                if (myCVal->m_ValueID == myValueID) {
                    myValue = myCVal->m_ValueName;
                    break;
                }
            }
        }

		m_Shp->SetFieldValue(myValue ,
							layer->m_pLayerFieldArray.Item(i)->m_FieldType,
							i+3);
	}
	return true;
}



/***************************************************************************//**
 @brief Compute intersection
 @details This function try to bypass the Intersection() bug of GDAL by using
 GEOS directly
 @param line The line to intersect
 @param frame the frame for intersecting
 @return  A valid OGRLineString or NULL
 @author Lucien Schreiber (c) CREALP 2008
 @date 18 November 2008
 *******************************************************************************/
OGRGeometry * tmExportDataSHP::SafeIntersection(OGRGeometry * line, OGRGeometry * frame)
{
	wxASSERT(line);
	wxASSERT(frame);

	GEOSGeom  geosline = NULL;
	GEOSGeom  geosframe = NULL;
	GEOSGeom  geosintersect = NULL;
	OGRGeometry * returncrop = NULL;

	geosline = line->exportToGEOS();
	geosframe = frame->exportToGEOS();

	wxASSERT(geosline);
	wxASSERT(geosframe);
	if (geosline != NULL && geosframe != NULL)
	{
		geosintersect = GEOSIntersection(geosline, geosframe);
		GEOSGeom_destroy(geosline);
		GEOSGeom_destroy(geosframe);

		if (geosintersect != NULL)
		{
			returncrop = SafeCreateFromGEOS(geosintersect);
			GEOSGeom_destroy(geosintersect);
		}


	}

	return returncrop;
}


/***************************************************************************//**
 @brief Compute Union
 @details This function try to bypass the Union() bug of GDAL by using
 GEOS directly
 @param union1 The multi-line string
 @param line The line to intersect
 @return  A valid OGRLineString or NULL
 @author Lucien Schreiber (c) CREALP 2008
 @date 18 November 2008
 *******************************************************************************/
OGRGeometry * tmExportDataSHP::SafeUnion (OGRGeometry * union1, OGRGeometry * line)
{
	wxASSERT(union1);
	wxASSERT(line);

	GEOSGeom  geosline = NULL;
	GEOSGeom  geosunion = NULL;
	GEOSGeom  geosresult = NULL;
	OGRGeometry * returnunion = NULL;

	geosline = line->exportToGEOS();
	geosunion = union1->exportToGEOS();
	if (geosline != NULL && union1 != NULL)
	{
		geosresult = GEOSUnion(geosunion, geosline);
		GEOSGeom_destroy(geosline);
		GEOSGeom_destroy(geosunion);

		if (geosresult != NULL)
		{
			returnunion = SafeCreateFromGEOS(geosresult);
			GEOSGeom_destroy(geosresult);
		}

	}

	return returnunion;
}


/***************************************************************************//**
 @brief Safe conversion function
 @param geosGeom The GEOSGeom to convert
 @return  A valid OGRGeometry or NULL if an error occur
 @author Lucien Schreiber (c) CREALP 2008
 @date 15 January 2009
 *******************************************************************************/
OGRGeometry * tmExportDataSHP::SafeCreateFromGEOS (GEOSGeom geosGeom)
{
	size_t nSize = 0;
    unsigned char *pabyBuf = NULL;
    OGRGeometry *poGeometry = NULL;

	GEOSWKBWriter * myWKBWriter =  GEOSWKBWriter_create();
	pabyBuf = GEOSWKBWriter_write(myWKBWriter, geosGeom, &nSize);

    if( pabyBuf == NULL || nSize == 0 )
    {
        GEOSWKBWriter_destroy(myWKBWriter);
		return NULL;
    }

    if( OGRGeometryFactory::createFromWkb( (unsigned char *) pabyBuf,
										  NULL, &poGeometry, (int) nSize )
	   != OGRERR_NONE )
    {
		GEOSWKBWriter_destroy(myWKBWriter);
        poGeometry = NULL;
    }

	//delete pabyBuf;
#if GEOS_CAPI_VERSION_MAJOR >= 2 || (GEOS_CAPI_VERSION_MAJOR == 1 && GEOS_CAPI_VERSION_MINOR >= 6) 
		GEOSFree( pabyBuf ); 
#else
	free(pabyBuf);
#endif
	GEOSWKBWriter_destroy(myWKBWriter);
    return poGeometry;
}



OGRGeometry * tmExportDataSHP::SafeBuffer (OGRGeometry * ogrgeom, int size)
{
	wxASSERT (ogrgeom);
	GEOSGeom geom = ogrgeom->exportToGEOS();
	GEOSGeom geombuffer;
	OGRGeometry * returnbuffer = NULL;

	if (geom != NULL)
	{
		geombuffer = GEOSBuffer(geom, size, 30);
		GEOSGeom_destroy(geom);

		if (geombuffer != NULL)
		{
			returnbuffer = SafeCreateFromGEOS(geombuffer);
			GEOSGeom_destroy(geombuffer);
		}
	}

	return returnbuffer;
}






/***************************************************************************//**
 @brief Set the frame
 @details If the frame allready exists, it is destroyed and this new frame is
 used
 @param points array of points creating a polygon
 @param nbvertex number of vertex
 @author Lucien Schreiber (c) CREALP 2008
 @date 16 November 2008
 *******************************************************************************/
void tmExportDataSHP::SetFrame (wxRealPoint * points, int nbvertex)
{
	wxASSERT (points);
	if (m_Frame)
		OGRGeometryFactory::destroyGeometry(m_Frame);

	m_Frame =(OGRPolygon*) OGRGeometryFactory::createGeometry(wkbPolygon);
	OGRLineString * myLine;
	myLine = (OGRLineString*) OGRGeometryFactory::createGeometry(wkbLineString);
	for (int i = 0; i<nbvertex;i++)
		myLine->addPoint(points[i].x, points[i].y);

	m_Frame->addRing((OGRLinearRing*) myLine);

	OGRGeometryFactory::destroyGeometry(myLine);

}


/***************************************************************************//**
 @brief Set value for multiple fields
 @details Loop in all layer's field and set value for all of them
 @param layer valid object containing layer (and layer's fields) definition
 @param values values to set to fields (a check is done that the numbers of
 values is egal to the number of fields)
 @return  true if all fields were set correctly
 @author Lucien Schreiber (c) CREALP 2009
 @date 27 March 2009
 *******************************************************************************/
bool tmExportDataSHP::SetMultipleFields (ProjectDefMemoryLayers * layer,
										 const wxArrayString & values)
{
	wxASSERT (values.GetCount()-1 == layer->m_pLayerFieldArray.GetCount());

	for (unsigned int i = 0; i<layer->m_pLayerFieldArray.GetCount();i++)
	{
		if(!m_Shp->SetFieldValue(values.Item(i+1), layer->m_pLayerFieldArray.Item(i)->m_FieldType, i+3))
			return false;
		else
			m_Shp->UpdateFeature();
	}

	return true;
}







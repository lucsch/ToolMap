/***************************************************************************
								tmgisdataraster.cpp
                    Super class for dealing with raster GIS data
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

#include "tmgisdataraster.h"
#include "tmgisdatarastertiff.h"
#include "tmgisdatarasteregrid.h"


tmGISDataRaster::tmGISDataRaster()
{
	m_DataSet = NULL;
	m_RasterBand = NULL;
	m_FileType = _T("Generic GDAL Raster");
	m_PxImgFilter = wxRect(0,0,-1,-1);
	m_ClippedCoord = tmRealRect (0,0,0,0);
}


tmGISDataRaster::~tmGISDataRaster()
{
	// closing GDAL raster dataset
	if (m_DataSet)
	{
		GDALClose(m_DataSet);
		m_DataSet = NULL;
	}
	m_RasterBand = NULL;
}



bool tmGISDataRaster::Open (const wxString & filename, bool bReadWrite)
{
	// init parent member values
	tmGISData::Open(filename, bReadWrite);
	
	// convert utf wxString into char *
	//const char* ascii_str = 
	char * buffer = new char [filename.Length()+2];
	strcpy(buffer, (const char*)filename.mb_str(wxConvUTF8));
	
	// open the raster and return true if success
	m_DataSet = (GDALDataset*) GDALOpen(buffer, (GDALAccess) bReadWrite);
	if (m_DataSet == NULL)
	{
		if (IsLoggingEnabled())
			wxLogDebug(_T("Unable to open %s : %s"), m_FileType.c_str(), filename.c_str());
		return FALSE;
	}
	
	return TRUE;
}



tmRealRect tmGISDataRaster::GetMinimalBoundingRectangle()
{
	// ASSERT m_DataSet
	wxString sFunction = wxString::FromAscii(__FUNCTION__);
	wxString sFunctionLineError = wxString::Format( _T("%s line %d : "),
												   sFunction.c_str(), __LINE__); 
	wxString sErrMsg = wxString::Format(_T("%s Error, m_DataSet not defined"),
										sFunctionLineError.c_str());
	wxASSERT_MSG(m_DataSet,sErrMsg);
	
	// getting bounding box
	double dCoord[6];
	if (m_DataSet->GetGeoTransform(dCoord) != CE_None)
		return tmRealRect(0,0,0,0);
	
	// selecting band 1
	m_RasterBand = m_DataSet->GetRasterBand(1);
	
	// computing bounding box
	
	tmRealRect myCoord (0,0,0,0);
	
	myCoord.x_min = dCoord[0];
	myCoord.y_max = dCoord[3];
	myCoord.y_min = myCoord.y_max + (m_DataSet->GetRasterYSize() * dCoord[5]);
	myCoord.x_max = myCoord.x_min + (m_DataSet->GetRasterXSize() * dCoord[1]);
	
	return myCoord;
	
}





wxString tmGISDataRaster::GetAllRasterGISFormatsWildcards()
{
	wxString myWildCards = _T("");
	for (unsigned int i = 0; i < (sizeof(tmGISDATA_RASTER_TYPE_WILDCARDS) / sizeof(wxString)); i++)
	{
		myWildCards.Append(tmGISDATA_RASTER_TYPE_WILDCARDS[i]);
		
		if (i+1 < (sizeof(tmGISDATA_RASTER_TYPE_WILDCARDS) / sizeof (wxString)))
			myWildCards.Append(_T("|"));
	}
	
	return myWildCards;
}



tmGISDataRaster * tmGISDataRaster::CreateGISRasterBasedOnType (const int & gis_format_index)
{
	switch (gis_format_index)
	{
		case tmGIS_RASTER_TIFF:
			return new tmGISDataRasterTIFF();
			break;
		case tmGIS_RASTER_BINGRID:
			return new tmGISDataRasterEGRID();
			break;
	}
	return NULL;
}



tmGISDataRaster * tmGISDataRaster::CreateGISRasterBasedOnExt (const wxString & extension)
{
	int iLoop = sizeof( tmGISDATA_RASTER_TYPE_EXTENSION) / sizeof(wxString);
	for (int i = 0; i< iLoop; i++)
	{
		if (tmGISDATA_RASTER_TYPE_EXTENSION[i].Contains(extension))
			return CreateGISRasterBasedOnType(i);
	}
	return NULL;
}



void tmGISDataRaster::InitGISDriversRaster()
{
	GDALAllRegister();
}



/***************************************************************************//**
 @brief Get Image dimensions in Pixels
 @return  The image dimensions in pixels or -1,-1 if image wasen't open
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 September 2008
 *******************************************************************************/
wxSize tmGISDataRaster::GetImagePxDim ()
{
	if (m_DataSet)
	{
		return wxSize (m_DataSet->GetRasterXSize(), m_DataSet->GetRasterYSize());
	}
	
	fprintf(stderr, "%s line %d : Error getting image dimension \n ", __FUNCTION__, __LINE__);
	return wxSize (-1,-1);
}


/***************************************************************************//**
 @brief Return width and height of pixels in real units
 @details Divide the width and height of image by the width and height of image
 in pixels
 @param pxsizeX value of one pixels width in real units
 @param pxsizeY value of one pixels height in real units
 @param imgrealcoord If specified, image coordinates aren't calculated again. 
 But if empty, function will calculate image coordinate.
 @return  True if values are valid
 @author Lucien Schreiber (c) CREALP 2008
 @date 25 September 2008
 *******************************************************************************/
bool tmGISDataRaster::GetImagePxSize (double & pxsizeX, double & pxsizeY,const tmRealRect & imgrealcoord)
{
	tmRealRect myImageCoord = imgrealcoord;
	if (imgrealcoord == tmRealRect(0,0,0,0))
		myImageCoord = GetMinimalBoundingRectangle();
	
	wxSize pxdim = GetImagePxDim();
	
	pxsizeX = myImageCoord.GetWidth() / pxdim.GetWidth();
	pxsizeY = myImageCoord.GetHeight() / pxdim.GetHeight();
	
	if (pxsizeX > 0 && pxsizeY > 0)
		return true;
	
	return false;
}



/***************************************************************************//**
 @brief Convert clipped Real size to image pixels size
 @param origin Non clipped coordinates (Real)
 @param clipped Clipped coordinates (Real)
 @return  Clipped coordinates (Pixels), may be used for loading part of image
 @author Lucien Schreiber (c) CREALP 2008
 @date 25 September 2008
 *******************************************************************************/
wxRect tmGISDataRaster::ConvertClipedImage (const tmRealRect & origin, 
											const tmRealRect & clipped)
{
	// getting informations : image width in pixels and sizeof one pixels width
	// height.
	wxSize originPX = GetImagePxDim();
	if (originPX == wxSize (-1,-1))
	{
		fprintf(stderr, "%s line %d : Error getting image width and height in px \n ",
				__FUNCTION__, __LINE__);
		return wxRect (0,0,-1,-1);
	}
	
	double dPxWidthX = 0, dPxWidthY = 0;
	if (!GetImagePxSize(dPxWidthX, dPxWidthY, origin))
	{
		fprintf(stderr, "%s line %d : Error getting pixel width and height \n ",
				__FUNCTION__, __LINE__);
		return wxRect (0,0,-1,-1);
	}
	
	// converting Real size to image pixels size 
	wxRect clippedPX = wxRect (0,0,originPX.GetWidth(), originPX.GetHeight());
	
	// simple case, origin and clipped are similar, image is fully displayed
	if (origin == clipped)
		return clippedPX;
	
	if (origin.x_min < clipped.x_min) // left clipped
	{
		clippedPX.x = (clipped.x_min - origin.x_min) / dPxWidthX;
		clippedPX.width = clippedPX.GetWidth() - clippedPX.x;
	}
	
	if (clipped.x_max < origin.x_max) // right clipped
	{
		clippedPX.width = clippedPX.width - ((origin.x_max - clipped.x_max) / dPxWidthX);
	}
	
	if (clipped.y_max < origin.y_max) // top clipped
	{
		clippedPX.y = (origin.y_max - clipped.y_max) / dPxWidthY;
		clippedPX.height = clippedPX.GetHeight() - clippedPX.y;
	}
	
	if (origin.y_min < clipped.y_min) // bottom clipped
	{
		clippedPX.height = clippedPX.GetHeight() - ((clipped.y_min - origin.y_min) / dPxWidthY);
	}
	return clippedPX;
}



/***************************************************************************//**
 @brief Define the area of interest
 @details This function define the area we are interessed in. Reading may be
 done after with GetRasterData function.
 @note Real coordinates passed here are
 converted into raster coordinates
 @param filter Filter real coordinates
 @param type one of value defined in #TOC_GENERIC_NAME such as :  
  - TOC_NAME_TIFF
  - TOC_NAME_EGRID
 @return bool true if all works, false otherwise
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 September 2008
 *******************************************************************************/
bool tmGISDataRaster::SetSpatialFilter (tmRealRect filter, int type)
{
	wxASSERT (m_DataSet);
	
	tmRealRect myImgCoord = GetMinimalBoundingRectangle();
	if (myImgCoord == tmRealRect(0,0,0,0))
		return FALSE;
	
	
	if(myImgCoord.Clip(filter, m_ClippedCoord))
	{
		// image visible,
		// clip image with spatial filter
		m_PxImgFilter = ConvertClipedImage(myImgCoord, m_ClippedCoord);
		
	}
	
	return TRUE;
}


/***************************************************************************//**
 @brief Should we load image data
 @details This function works only if tmGISDataRaster::SetSpatialFilter was
 called before. This function return true if m_PxImgFilter contain valid
 coordinates
 @return  true if the image is visible (inside drawing area), false otherwise.
 If false is returned, no need to call the drawing process
 @author Lucien Schreiber (c) CREALP 2008
 @date 25 September 2008
 *******************************************************************************/
bool tmGISDataRaster::IsImageInsideVisibleArea ()
{
	if (m_PxImgFilter != wxRect (0,0,-1,-1))
		return TRUE;
	return FALSE;
}



/***************************************************************************//**
 @brief Extract image data
 @details Extract the image data from the GDALDataset and pack it into a single
 array of RGB triples. Use the ColorTable to determine the RGB values. We
 extract the data from the GDALDataset rather than create our own driver
 because the data needs to be translated from 4 byte pixel information into 3
 byte RGB information. This could be done as the data is written to the data
 set or afterwards, as it is done here. Any minor savings from our own driver
 are outweighed by the high development/maintenance costs.
 @warning You must call tmGISDataRaster::SetSpatialFilter() before this function
 for initialization to take place.
 @note Comes from
 THUBAN (http://thuban.intevation.org/) and modified for ToolMap2. Extracted
 from version 1.2.0
 @param imgbuf ???
 @param imglen ???
 @param maskbuf ???
 @param masklen ???
 @param imgSize ???
 @return CE_None if data was read succesfully
 @author Thuban Team & modfied by Lucien Schreiber
 @date 24 September 2008
 *******************************************************************************/
CPLErr tmGISDataRaster::GetImageData(unsigned char **imgbuf,
									 unsigned int   *imglen,
									 unsigned char **maskbuf,
									 unsigned int   *masklen,
									 wxSize imgSize)
{
    // my definitions 
	int             bEnablem_DataSettAlpha = FALSE, bEnableSrcAlpha = FALSE;
	int             bMakeMask = false, bMakeAlpha = false, bInvertMask = false;
	wxRect imgfilter = m_PxImgFilter;
	
	double dmin = 0;
	double dmax = 0;
	double dnodata = 0;
	
	
	CPLErr ret = CE_None;
	
    GDALColorTable *pal = NULL;
	
	
    m_DataSet->FlushCache();
	
    int rasterCount  = m_DataSet->GetRasterCount();
    int nRasterXSize = imgfilter.GetWidth();		//m_DataSet->GetRasterXSize();
    int nRasterYSize = imgfilter.GetHeight();	//m_DataSet->GetRasterYSize();
    if ( ! (nRasterXSize > 0 && nRasterYSize > 0 ))
    {
		if (IsLoggingEnabled())
			wxLogMessage(_("The dimensions (%ix%i) are invalid"), 
						 nRasterXSize, nRasterYSize);
        return CE_Failure;
    }
	
    //
    // create the new image array for RGBRGB... values
    //
    *imglen = 3 * imgSize.GetWidth() * imgSize.GetHeight();
    *imgbuf = (unsigned char*)CPLMalloc(*imglen);
    if ( *imgbuf == NULL)
    {
		if (IsLoggingEnabled())
			wxLogMessage(_("The system does not have enough memory to project"));
        return CE_Failure;
    }
	
    //
    // if there are three or more banm_DataSet assume that the first three
    // are for RGB, unless told otherwise
    //
    if (rasterCount >= 3)
    {
        for (int i=1; i <= 3; i++)
        {
            int offs = 0;
            GDALRasterBand *band = m_DataSet->GetRasterBand(i);
			
            switch (band->GetColorInterpretation())
            {
                case GCI_Undefined: offs = i-1; break;
				case GCI_RedBand:   offs = 0; break;
				case GCI_GreenBand: offs = 1; break;
				case GCI_BlueBand:  offs = 2; break;
                default:            offs = -1; break;
            }
			
            //
            // copy the image into the buffer using the proper offset
            // so we first copy over all Red values, then all Green
            // values, and then all Blue values
            //
			
            if (0 <= offs && offs < 3)
            {
                ret = band->RasterIO(GF_Read, imgfilter.GetX(), imgfilter.GetY(),
                                     nRasterXSize, nRasterYSize,
                                     *imgbuf+offs, imgSize.GetWidth(), imgSize.GetHeight(),
                                     GDT_Byte, 3, 0);
                if (ret == CE_Failure)
                {
					if (IsLoggingEnabled())
						wxLogError(_T( "An unknown error occured while reading band %i"),i);
                    break;
                }
            }
        }
    }
    else if (rasterCount >= 1)
    {
        //
        // one band is either a palette based image, or greyscale
        //
		
		// definitions for grayscale
        GDALRasterBand *band = m_DataSet->GetRasterBand(1);
		int iLoop = 0;
		double dRange = 0;
		int iBuffSize = 0;
        void * myGdalScanData = NULL;
		unsigned char *data = NULL;
		char Resultat = '\n';
		GDALDataType myDataType;
		
        switch (band->GetColorInterpretation())
        {
            case GCI_PaletteIndex:
				
                pal = band->GetColorTable();
				
                if (pal == NULL)
                {
					if (IsLoggingEnabled())
						wxLogError(_T( "Couldn't find a palette for palette-based image") );                  
					ret = CE_Failure;
                }
                else
                {
                    GDALPaletteInterp pal_interp
					= pal->GetPaletteInterpretation();
					
                    //
                    // copy over all the palette indices and then
                    // loop through the buffer replacing the values
                    // with the correct RGB triples.
                    //
                    ret = band->RasterIO(GF_Read, imgfilter.GetX(), imgfilter.GetY(),
                                         nRasterXSize, nRasterYSize,
                                         *imgbuf, imgSize.GetWidth(), imgSize.GetHeight(),
                                         GDT_UInt16, 3, 0);
					
                    if (ret == CE_Failure)
                    {
						if (IsLoggingEnabled())
							wxLogError(_T("An unknown error occured while reading band 1"));
                        break;
                    }
					
                    for (unsigned char *data = *imgbuf;
                         data != (*imgbuf+*imglen);
                         data += 3)
                    {
						
                        unsigned short int val = *((unsigned short int *)data);
						
                        const GDALColorEntry *color = pal->GetColorEntry(val);
						
                        if (pal_interp == GPI_Gray)
                        {
                            *(data + 0) = color->c1;
                            *(data + 1) = color->c1;
                            *(data + 2) = color->c1;
                        }
                        else
                        {
                            *(data + 0) = color->c1;
                            *(data + 1) = color->c2;
                            *(data + 2) = color->c3;
                        }
                    }
                }
                break;
				
            case GCI_Undefined: // can we try to make a greyscale image?
            case GCI_GrayIndex:
				
				// compute stats,
				// TODO: move stat computation in another place
				GetStatMinMaxNoDataValue(dmin, dmax, dnodata);
				dRange = dmax - dmin;
				
				myDataType = band->GetRasterDataType();
                myGdalScanData = ReadImageData(band, imgfilter, imgSize, iBuffSize);
					data = *imgbuf;
				for (unsigned int i = 0; i<*imglen; i += 3)
                {
					
					double myGrayValDouble = ReadGDALValueToDouble(myGdalScanData,
																   myDataType, iLoop);
					iLoop ++;
					
					int myGrayValInt = static_cast < int >((myGrayValDouble - dmin) * (255 / dRange));
					if (myGrayValInt < 0) myGrayValInt = 0;
					if (myGrayValInt > 255) myGrayValInt = 255;
					
					//TODO: Dealing with no-data here
					
					*(data + i) = myGrayValInt;
					*(data + i + 1) = myGrayValInt;
					*(data + i + 2) = myGrayValInt;
					
				}
					
				CPLFree (myGdalScanData);
		        break;
				
            default:
				if (IsLoggingEnabled())
					wxLogError(_T("Unsupported color interpretation '%s'"),
							   GDALGetColorInterpretationName(band->GetColorInterpretation()));
				
                ret = CE_Failure;
                break;
        }
    }
    else
    {
		if (IsLoggingEnabled())
			wxLogError(_T("Unsupported number of raster banm_DataSet (%i) in image"),
				   rasterCount);
		
        ret = CE_Failure;
    }
	
    if (ret == CE_None && bEnablem_DataSettAlpha && rasterCount > 1)
    {
        if (bMakeMask)
        {
            //
            // The mask is really an XBM image. In other worm_DataSet, each
            // pixel is represented by one bit in a byte array.
            //
            // First read the alpha band, and then convert it to
            // a bit array by thresholding each pixel value at 128.
            //
			
            *masklen = ((imgSize.GetWidth() + 7) / 8) * imgSize.GetHeight();
            *maskbuf = (unsigned char *)CPLMalloc(*masklen);
			
            if ( *maskbuf != NULL )
            {
                unsigned char *tmp
				= (unsigned char *)CPLMalloc(imgSize.GetWidth() * imgSize.GetHeight());
				
                if ( tmp == NULL )
                {
                    CPLFree(*maskbuf);
                    *maskbuf = NULL;
                }
                else
                {
                    GDALRasterBand *band = m_DataSet->GetRasterBand(rasterCount);
					
                    ret = band->RasterIO(GF_Read, imgfilter.GetX(), imgfilter.GetY(),
                                         nRasterXSize, nRasterYSize,
                                         tmp, imgSize.GetWidth(), imgSize.GetHeight(),
                                         GDT_Byte, 0, 0);
					
                    if (ret != CE_Failure)
                    {
                        int i, j, b=1, c=0;
                        unsigned char *ptr = *maskbuf;
                        unsigned char *tptr = tmp;
						
                        //unsigned int empty_count=0;
						
                        for (i=0; i < nRasterYSize; i++)
                        {
                            for (j=nRasterXSize; j >= 8; j -= 8)
                            {
                                c=0; b=1;
                                if (*tptr++ >= 128) {c|=b;} b<<=1;
                                if (*tptr++ >= 128) {c|=b;} b<<=1;
                                if (*tptr++ >= 128) {c|=b;} b<<=1;
                                if (*tptr++ >= 128) {c|=b;} b<<=1;
                                if (*tptr++ >= 128) {c|=b;} b<<=1;
                                if (*tptr++ >= 128) {c|=b;} b<<=1;
                                if (*tptr++ >= 128) {c|=b;} b<<=1;
                                if (*tptr++ >= 128) {c|=b;} b<<=1;
								
                                if (bInvertMask)
                                    *(ptr++) = ~c;
                                else
                                    *(ptr++) = c;
                            }
							
                            c=0; b=1;
                            switch (nRasterXSize & 7)
                            {
                                case 7: if (*tptr++ >= 128) {c|=b;} b<<=1;
                                case 6: if (*tptr++ >= 128) {c|=b;} b<<=1;
                                case 5: if (*tptr++ >= 128) {c|=b;} b<<=1;
                                case 4: if (*tptr++ >= 128) {c|=b;} b<<=1;
                                case 3: if (*tptr++ >= 128) {c|=b;} b<<=1;
                                case 2: if (*tptr++ >= 128) {c|=b;} b<<=1;
                                case 1:
                                    if (*tptr++ >= 128) {c|=b;}
                                    b<<=1;
									
                                    //
                                    // byte should be padded with 0's so
                                    // it's not a simple inversion
                                    //
                                    if (bInvertMask)
                                        *(ptr++) = ~c & (b-1);
                                    else
                                        *(ptr++) = c;
									
                                default: break;
                            }
                        }
						
#if 0
                        if (empty_count == *masklen)
                        {
                            fprintf(stderr, "mask not used\n");
							
                            CPLFree(*maskbuf);
                            *maskbuf = NULL;
                        }
#endif
                    }
					
                    CPLFree(tmp);
                    tmp = NULL;
                }
				
            }
        }
        else if (bMakeAlpha)
        {
            //
            // This is the simple case. The array we get back from RasterIO
            // is already in the correct format.
            //
			
            *masklen = imgSize.GetWidth() * imgSize.GetHeight();
            *maskbuf = (unsigned char *)CPLMalloc(*masklen);
			
            if ( *maskbuf != NULL )
            {
                GDALRasterBand *band = m_DataSet->GetRasterBand(rasterCount);
				
                ret = band->RasterIO(GF_Read, imgfilter.GetX(), imgfilter.GetY(),
                                     nRasterXSize, nRasterYSize,
                                     *maskbuf, imgSize.GetWidth(), imgSize.GetHeight(),
                                     GDT_Byte, 0, 0);
				
#if 0
                if (ret == CE_Failure)
                {
                    CPLFree(*maskbuf);
                    *maskbuf = NULL;
                }
#endif
            }
        }
    }
	
    if (ret != CE_None)
    {
        if (*imgbuf  != NULL) { CPLFree(*imgbuf);  *imgbuf  = NULL; }
        if (*maskbuf != NULL) { CPLFree(*maskbuf); *maskbuf = NULL; }
    }
	
    return ret;
}



/***************************************************************************//**
 @brief Reading image data based on GDAL type
 @details This function is used actually for DTM but may be extended for all
 raster.
 @note inspired by QGIS code
 @param gdalBand Pointer to a valid GDAL band
 @param imgfilter Position and width of the image in real px. For reading all
 image pass 0,0, width, height
 @param imgSize Size of the bitmap we will use for displaying data
 @param buffsize 
 @return  pointer to data of type GDALDataType, data must be freed with CPLFree
 @author Lucien Schreiber (c) CREALP 2008
 @date 03 October 2008
 *******************************************************************************/
void *tmGISDataRaster::ReadImageData ( GDALRasterBand *gdalBand, const wxRect & imgfilter,
								 const wxSize & imgSize, int & buffsize)
{
	GDALDataType type = gdalBand->GetRasterDataType();
	int size = GDALGetDataTypeSize ( type ) / 8;
	
	buffsize = size * imgSize.GetWidth() * imgSize.GetHeight();
	void *data = CPLMalloc (buffsize);
		
	CPLErr myErr = gdalBand->RasterIO ( GF_Read,
									   imgfilter.GetX(),
									   imgfilter.GetY(),
									   imgfilter.GetWidth(),
									   imgfilter.GetHeight(),
									   data,
									   imgSize.GetWidth(),
									   imgSize.GetHeight(),
									   type, 0, 0 );
	
	return data;
}



/***************************************************************************//**
 @brief Getting double value from data returned by GDAL
 @details This function allows to deals with image with different data type such
 as those described in GDALDataType (see gdal doc)
 @note inspired by QGIS code
 @param data Pointer to data returned by tmGISDataRaster::ReadImageData
 @param type type of data as returned by gdalBand->GetRasterDataType()
 @param index index of the pixel
 @return  Pixel value at position index
 @author Lucien Schreiber (c) CREALP 2008
 @date 03 October 2008
 *******************************************************************************/
double tmGISDataRaster::ReadGDALValueToDouble ( void *data, GDALDataType type, int index )
{
	double val;
	
	switch ( type )
	{
		case GDT_Byte:
			return (double) ((GByte *)data)[index];
			break;
		case GDT_UInt16:
			return (double) ((GUInt16 *)data)[index];
			break;
		case GDT_Int16:
			return (double) ((GInt16 *)data)[index];
			break;
		case GDT_UInt32:
			return (double) ((GUInt32 *)data)[index];
			break;
		case GDT_Int32:
			return (double) ((GInt32 *)data)[index];
			break;
		case GDT_Float32:
			return (double) ((float *)data)[index];
			break;
		case GDT_Float64:
			val = ((double *)data)[index];
			return (double) ((double *)data)[index];
			break;
		default:
			if (IsLoggingEnabled())
				wxLogDebug(_T("GDAL data type is not supported"));
	}
	return 0.0;
}



/***************************************************************************//**
 @brief Returning min - max - nodata stat
 @details This function return the following statistics :
 - Minimum band values (altitudes in case of DTM)
 - Maximum band values
 - Nodata values.
 If a value wasen't computed, 0 is returned.
 @param dmin will contain the minimum value
 @param dmax will contain the maximum value
 @param dnodata will contain the value used for nodata
 @return  true if at least one value was computed succesfully
 @author Lucien Schreiber (c) CREALP 2008
 @date 03 October 2008
 *******************************************************************************/
bool tmGISDataRaster::GetStatMinMaxNoDataValue (double & dmin, double & dmax,
												double & dnodata)
{
	int bResult = false;
	bool bReturn = false;
	dmin = 0;
	dmax = 0;
	dnodata = 0;
	
	double dtempmin = m_RasterBand->GetMinimum(&bResult);
	if (bResult)
	{
		dmin = dtempmin;
		bReturn = true;
	}
	
	double dtempmax = m_RasterBand->GetMaximum(&bResult);
	if (bResult)
	{
		dmax = dtempmax;
		bReturn = true;
	}
	
	double dtempnodata = m_RasterBand->GetNoDataValue(&bResult);
	if (bResult)
	{
		dnodata = dtempnodata;
		bReturn = true;
	}
	
	return bReturn;
}



/***************************************************************************//**
 @brief Get Metadata information well formated
 @return  An html string to be displayed in the properties dialog
 @author Lucien Schreiber (c) CREALP 2008
 @date 22 October 2008
 *******************************************************************************/
wxString tmGISDataRaster::GetMetaDataAsHtml ()
{
	wxString myResult = _T("");
	myResult.Append(_("<B><U>Name</B></U><BR>"));
	myResult.Append(GetFullFileName() + _T("<BR><BR>"));
	
	myResult.Append(_("<B><U>General informations</B></U><BR>"));
	myResult.Append(GetBandMetaData() + _T("<BR>"));
	myResult.Append(GetUnitMetaData() + _T("<BR>"));
	myResult.Append(GetImagePxSizeMetadata() + _T("<BR><BR>"));
	
	myResult.Append(GetMinimalBoundingRectangleAsHtml(2) + _T("<BR>"));
	
	myResult.Append(GetPyramidsMetadata() + _T("<BR><BR>"));
	
	myResult.Append(GetDataSizeAsHtml(2));
	
	return myResult;
}



/***************************************************************************//**
 @brief Getting number of band
 @return  The number of band or -1 if an error occur
 @author Lucien Schreiber (c) CREALP 2008
 @date 23 October 2008
 *******************************************************************************/
int tmGISDataRaster::GetBandCount ()
{

	if (!m_DataSet)
	{
		wxLogDebug(_T("m_Dataset not defined, please define it first"));
		return -1;
	}
	return m_DataSet->GetRasterCount();
}



/***************************************************************************//**
 @brief Getting number of band formatted as html
 @return  An html compliant string indicating the number of bands
 @author Lucien Schreiber (c) CREALP 2008
 @date 23 October 2008
 *******************************************************************************/
wxString tmGISDataRaster::GetBandMetaData ()
{
	int iNbBand = GetBandCount();
	if (iNbBand == -1)
	{
		return _("Unable to compute the number of band");
	}
	
	return wxString::Format(_("Number of band(s) : %d"), iNbBand);
}



/***************************************************************************//**
 @brief Get Informations about Unit formatted for html
 @return An html compliant string containing the unit used for the raster
 @author Lucien Schreiber (c) CREALP 2008
 @date 23 October 2008
 *******************************************************************************/
wxString tmGISDataRaster::GetUnitMetaData ()
{
	GDALDataType myDataType = m_DataSet->GetRasterBand(1)->GetRasterDataType();
	wxString myRetVal = _("Raster units are : ");
	myRetVal.Append(tmRASTER_DATATYPES[(int) myDataType]);
	return myRetVal;
}



/***************************************************************************//**
 @brief Get info about pyramids
 @details Pyramids are small image subsets usefull for displaying raster faster
 @param pyramids String containing all pyramids informations if pyramids isn't
 NULL. 
 @return  Number of pyramids in the raster
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 October 2008
 *******************************************************************************/
int tmGISDataRaster::GetPyramidsInfo (wxArrayString * pyramids)
{
	int iNbPyramids = 0;
	GDALRasterBand * myOverview = NULL;
	
	if (!m_RasterBand)
	{
		m_RasterBand = m_DataSet->GetRasterBand(1);
	}
	
	// counting pyramids
	iNbPyramids = m_RasterBand->GetOverviewCount();
	
	// getting all pyramids if required
	if (iNbPyramids > 0 && pyramids != NULL)
	{
		for (int i = 0; i< iNbPyramids; i++)
		{
			myOverview = m_RasterBand->GetOverview(i);
			if (myOverview)
			{
				pyramids->Add(wxString::Format(_T("%d x %d"),
											   myOverview->GetXSize(),
											   myOverview->GetYSize()));
			}
		}
		
	}
	
	
	return iNbPyramids;
}



/***************************************************************************//**
 @brief Getting pyramids info as html string
 @details Pyramids are small image subsets usefull for displaying raster faster
 @return  html compliant string containing pyramids info
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 October 2008
 *******************************************************************************/
wxString tmGISDataRaster::GetPyramidsMetadata()
{
	wxString myRetString = _("<U><B>Raster Overviews</B></U><BR>");
	wxArrayString myPyramidsInfoString;
	int iNbPyramids = GetPyramidsInfo(&myPyramidsInfoString);
	
	myRetString.Append(wxString::Format(_("Number of raster overview : %d<BR>"),
										iNbPyramids));
	
	if (iNbPyramids == 0)
	{
		
		myRetString.Append(_("<I>Build image overviews for faster image display</I>"));
		return myRetString;
	}
	
	// list of overviews
	myRetString.Append(_T("<UL>"));
	for (unsigned int i = 0; i<myPyramidsInfoString.GetCount(); i++)
	{
		myRetString.Append(_T("<LI>") + myPyramidsInfoString.Item(i) + _T("</LI>"));
	}
	myRetString.Append(_T("</UL>"));
	
	
	return myRetString;
}



/***************************************************************************//**
 @brief Get Pixels width and height
 @return  html compliant string containing width and height of the raster in
 pixels
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 October 2008
 *******************************************************************************/
wxString tmGISDataRaster::GetImagePxSizeMetadata ()
{
	wxSize myRasterSize = GetImagePxDim();
	return wxString::Format(_("Raster width : %d (pixels)<BR>Raster height : %d (pixels)"),
							myRasterSize.GetWidth(), myRasterSize.GetHeight());
}


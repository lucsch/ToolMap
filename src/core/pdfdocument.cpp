/***************************************************************************
 pdfdocument.cpp
 -------------------
 copyright            : (C) 2011 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "pdfdocument.h"

#include "prjdefmemmanage.h"

bool PdfDocument::_GenerateTitle() {
  m_pdf->SetFont("Helvetica", "B", m_FontSize + 4);
  m_pdf->Cell(0, m_LineSpacing, wxString::Format(_("'%s' Data Model"), m_prjName.Upper()), wxPDF_BORDER_BOTTOM, 1);
  m_pdf->SetFont("Helvetica", "I", m_FontSize);

  m_pdf->Cell(m_pdf->GetPageWidth() / 2.0 - m_pdf->GetLeftMargin(), m_LineSpacing,
              wxString::Format(_("Generated by ToolMap on: %s"), wxDateTime::Now().FormatISOCombined(' ')),
              wxPDF_BORDER_NONE, 0);

  m_pdf->Cell(m_pdf->GetPageWidth() / 2.0 - m_pdf->GetRightMargin(), m_LineSpacing,
              wxString::Format(_("Page : %d / {nb}"), m_pdf->PageNo()), wxPDF_BORDER_NONE, 0, wxPDF_ALIGN_RIGHT);
  m_pdf->Ln();
  return true;
}

void PdfDocument::_ComputeOnePageSize(double &width, double &height) {
  width = wxNOT_FOUND;
  height = wxNOT_FOUND;

  wxPdfDocument myTempDoc(m_PaperOrientation, "mm", m_PaperFormat);
  myTempDoc.SetFont("Helvetica", "", m_FontSize);

  // width
  double maxobjectswidth = 0;
  double maxattribswidth = 0;
  for (unsigned int i = 0; i < m_pdfLayers.GetCount(); i++) {
    double objectwidth = m_pdfLayers.Item(i)->GetObjectsWidth(&myTempDoc);
    maxobjectswidth = MAX(maxobjectswidth, objectwidth);

    double attribswidth = m_pdfLayers.Item(i)->GetAttributsWidth(&myTempDoc);
    maxattribswidth = MAX(maxattribswidth, attribswidth);
  }

  if (wxIsSameDouble(maxattribswidth, 0) && wxIsSameDouble(maxobjectswidth, 0)) {
    return;
  }

  width = MAX(maxattribswidth, maxobjectswidth);
  if (IsTwoColsLayout()) {
    width = width * 2.0;
  }

  // height
  // title
  height += 10;  // margins
  height += 10;  // two lines bottom for polygons notes
  height += m_FontSize + 4;
  height += m_FontSize * 2.0;

  // layers
  for (unsigned int i = 0; i < m_pdfLayers.GetCount(); i++) {
    height += (m_FontSize + 2) * 2.0;

    double objectheight = m_pdfLayers.Item(i)->GetObjectsHeight(&myTempDoc);
    double attribheight = m_pdfLayers.Item(i)->GetAttributsHeight(&myTempDoc);

    if (IsTwoColsLayout()) {
      height += MAX(objectheight, attribheight);
    } else {
      height = height + objectheight + attribheight;
    }
  }
}

bool PdfDocument::_OrderLayers() {
  // bubble sort optimized
  unsigned int myLayerNumber = m_pdfLayers.GetCount();
  long myCount = 0;
  while (1) {
    bool swaped = false;
    for (unsigned int i = 1; i <= myLayerNumber - 1; i++) {
      wxString myItemText0 = m_pdfLayers.Item(i - 1)->GetName();
      wxString myItemText1 = m_pdfLayers.Item(i)->GetName();
      int myOrder0 = m_pdfLayers.Item(i - 1)->GetLayerTypeOrder();
      int myOrder1 = m_pdfLayers.Item(i)->GetLayerTypeOrder();

      if (myOrder1 < myOrder0) {
        // swap 0 - 1
        PdfLayer *myLayer1 = m_pdfLayers.Item(i);
        wxASSERT(myLayer1);
        m_pdfLayers.RemoveAt(i);
        m_pdfLayers.Insert(myLayer1, i - 1);
        swaped = true;
      }

      if (myOrder0 == myOrder1 && myItemText0.Cmp(myItemText1) > 0) {
        // swap 0 - 1
        PdfLayer *myLayer1 = m_pdfLayers.Item(i);
        wxASSERT(myLayer1);
        m_pdfLayers.RemoveAt(i);
        m_pdfLayers.Insert(myLayer1, i - 1);
        swaped = true;
      }
    }
    myLayerNumber = myLayerNumber - 1;
    if (!swaped) {
      break;
    }
    myCount++;
  }
  wxLogDebug(_("%ld iteration needed to class layers"), myCount);
  return true;
}

PdfDocument::PdfDocument(PrjDefMemManage *project) {
  wxASSERT(project);
  m_prjName = project->m_PrjName;
  m_PaperFormat = wxPAPER_A4;
  m_PaperWidth = wxNOT_FOUND;
  m_PaperHeight = wxNOT_FOUND;

  m_PaperOrientation = wxPORTRAIT;
  m_FontSize = 10;
  m_LineSpacing = 6;
  m_Decorate = true;
  m_PageBreak = true;
  m_TwoCols = false;
  m_OnePage = false;
  _UpdatePageWidth();
  m_pdf = nullptr;

  // adding layers // ordering by points, lines, polygons
  for (int i = 0; i < project->GetCountLayers(); i++) {
    PdfLayer *myLayer = new PdfLayer(this, project->m_PrjLayerArray.Item(i));
    wxASSERT(myLayer);
    m_pdfLayers.Add(myLayer);
  }
  wxLogDebug("%zu PDF Layers added", m_pdfLayers.GetCount());
}

PdfDocument::~PdfDocument() {
  // manually clearing array of layers, Clear() or Empty() didn't work
  unsigned int iCount = m_pdfLayers.GetCount();
  for (unsigned int i = 0; i < iCount; i++) {
    PdfLayer *myLayer = m_pdfLayers.Item(0);
    wxDELETE(myLayer);
    m_pdfLayers.RemoveAt(0);
  }
  wxASSERT(m_pdfLayers.GetCount() == 0);
  wxDELETE(m_pdf);
}

bool PdfDocument::Generate(const wxFileName &filename) {
  // order layers
  _OrderLayers();

  // one page layout
  if (m_OnePage) {
    double myWidth = wxNOT_FOUND;
    double myHeight = wxNOT_FOUND;
    _ComputeOnePageSize(myWidth, myHeight);
    wxLogDebug("Paper width is: %f, height: %f", myWidth, myHeight);
    SetPaperSize(myWidth, myHeight);
  }

  // create pdf object
  if (m_PaperWidth == wxNOT_FOUND || m_PaperHeight == wxNOT_FOUND) {
    m_pdf = new wxPdfDocument(m_PaperOrientation, "mm", m_PaperFormat);
  } else {
    m_pdf = new wxPdfDocument(m_PaperOrientation, m_PaperWidth, m_PaperHeight, "mm");
  }
  wxASSERT(m_pdf);

  m_pdf->AliasNbPages();
  m_pdf->AddPage(m_PaperOrientation);
  _GenerateTitle();
  m_pdf->Ln();

  for (unsigned int i = 0; i < m_pdfLayers.GetCount(); i++) {
    if (HasPageBreak()) {
      if (i != 0) {
        m_pdf->AddPage(m_PaperOrientation);
        _GenerateTitle();
        m_pdf->Ln();
      }
    } else {
      m_pdf->Ln(GetLineSpacing());
    }
    m_pdfLayers.Item(i)->Generate();
  }

  // add polygon note
  m_pdf->SetFont("Helvetica", "", m_FontSize);
  m_pdf->MultiCell(0, 5,
                   _("* These objects have not to be declared by users, they are automatically"
                     " added to the object kinds list each time a polygon layer is created into"
                     " the spatial model. These geometric features are used to delineate real"
                     " world objects occurring as polygons. "),
                   wxPDF_BORDER_TOP, wxPDF_ALIGN_JUSTIFY);

  m_pdf->SaveAsFile(filename.GetFullPath());
  wxLogMessage(_("Data model exported to: '%s'"), filename.GetFullName());
  wxDELETE(m_pdf);
  return true;
}

void PdfDocument::_UpdatePageWidth() {
  wxPdfDocument *myTempDoc = nullptr;
  if (m_PaperWidth == wxNOT_FOUND || m_PaperHeight == wxNOT_FOUND) {
    myTempDoc = new wxPdfDocument(m_PaperOrientation, "mm", m_PaperFormat);
  } else {
    myTempDoc = new wxPdfDocument(m_PaperOrientation, m_PaperWidth, m_PaperHeight, "mm");
  }
  wxASSERT(myTempDoc);
  m_UsablePageWidth = myTempDoc->GetPageWidth() - myTempDoc->GetLeftMargin() - myTempDoc->GetRightMargin();
  wxDELETE(myTempDoc);
}

void PdfDocument::SetFontSize(int value) {
  m_FontSize = value;
}

void PdfDocument::SetLineSpacing(double value) {
  m_LineSpacing = value;
}

void PdfDocument::SetPaperFormat(wxPaperSize value) {
  m_PaperFormat = value;
  m_PaperHeight = wxNOT_FOUND;
  m_PaperWidth = wxNOT_FOUND;
  _UpdatePageWidth();
}

void PdfDocument::SetDecorate(bool value) {
  m_Decorate = value;
}

void PdfDocument::SetPaperOrientation(wxPrintOrientation value) {
  m_PaperOrientation = value;
  _UpdatePageWidth();
}

double PdfDocument::GetUsablePageWidth() {
  return m_UsablePageWidth;
}

bool PdfDocument::IsLandscape() {
  if (m_PaperOrientation == wxLANDSCAPE) {
    return true;
  }
  return false;
}

wxPdfDocument *PdfDocument::GetPdfRef() {
  return m_pdf;
}

void PdfDocument::SetTwoColsLayout(bool value) {
  m_TwoCols = value;
}

void PdfDocument::SetPageBreak(bool value) {
  m_PageBreak = value;
}

void PdfDocument::SetOnePage(bool value) {
  m_OnePage = value;
  SetPageBreak(false);
}

void PdfDocument::SetPaperSize(double width, double height) {
  m_PaperWidth = width;
  m_PaperHeight = height;
  _UpdatePageWidth();
}

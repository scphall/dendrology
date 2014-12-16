#define SCPH_EASEL_EASEL1D_CLASS_H_cxx
#include "Easel1D.hpp"

#include <iostream>

#include "TStyle.h"
#include "TROOT.h"

#include "Oi.hpp"


bool Easel1D::set_style_sw = false;


Easel1D::Easel1D(TH1* histogram, std::string legname, double width, double height) :
    EaselRoot(histogram, legname, width, height) {
  LHCbStyle();
  if (!set_style_sw) {
    LHCbStyle();
    set_style_sw = true;
  }
}


Easel1D::Easel1D(std::string name, std::string legname, double width, double height) :
    EaselRoot(name, legname, width, height) {
  if (!set_style_sw) {
    LHCbStyle();
    set_style_sw = true;
  }
}


Easel1D::Easel1D() :
    EaselRoot() {
  if (!set_style_sw) {
    LHCbStyle();
    set_style_sw = true;
  }
}


Easel1D::~Easel1D() {}


double Easel1D::getGlobalMax() {
  if (set_global_max_) {
    return global_max_;
  }
  double global_max = -1e5;
  double max;
  for (unsigned int hist_no=0; hist_no<histograms_.size(); ++hist_no) {
    max = getHistogram(hist_no)->GetBinContent(getHistogram(hist_no)->GetMaximumBin());
    if (max > global_max) {
      global_max = max;
    }
  }
  return global_max;
}


double Easel1D::getGlobalMin() {
  if (set_global_min_) {
    return global_min_;
  }
  double global_min = 1e5;
  double min;
  for (unsigned int hist_no=0; hist_no<histograms_.size(); ++hist_no) {
    //min = getHistogram(hist_no)->GetBinContent(getHistogram(hist_no)->GetMaximumBin());
    min = getHistogram(hist_no)->GetBinContent(getHistogram(hist_no)->GetMinimumBin());
    if (min < global_min) {
      global_min = min;
    }
  }
  return global_min;
}


void Easel1D::setMinimum(const double& val) {
  set_global_min_ = true;
  global_min_ = val;
  return;
}


void Easel1D::setMaximum(const double& val) {
  set_global_max_ = true;
  global_max_ = val;
  return;
}


void Easel1D::setHistogramStyle(TH1* histogram) {
  //histogram->SetTitle("");
  histogram->SetLabelColor(kBlack);
  gStyle->SetPadRightMargin(0.05); // increase for colz plots
  gStyle->SetLabelFont(132);
  gStyle->SetTitleFont(132, "x");
  gStyle->SetTitleFont(132, "y");
  gStyle->SetTitleFont(132, "z");
  histogram->GetXaxis()->SetTitleSize(size_axistitle_);
  histogram->GetYaxis()->SetTitleSize(size_axistitle_);
  histogram->GetXaxis()->SetLabelSize(size_axislabel_);
  histogram->GetYaxis()->SetLabelSize(size_axislabel_);
  if (axisname_x_ == "") {
    axisname_x_ = histogram->GetXaxis()->GetTitle();
    axisname_y_ = histogram->GetYaxis()->GetTitle();
  }
  histogram->GetXaxis()->SetTitle(axisname_x_.c_str());
  histogram->GetYaxis()->SetTitle(axisname_y_.c_str());
  //histogram->SetTitleOffset(0.8);
  histogram->SetMaximum(getGlobalMax());
  return;
}


void Easel1D::addHorizontalLine(double y, int colour, double width) {
  double x1 = getHistogram(0)->GetXaxis()->GetBinLowEdge(1);
  double x2 = getHistogram(0)->GetXaxis()->GetBinUpEdge(getHistogram(0)->GetXaxis()->GetNbins());
  addLine(x1, y, x2, y, colour, width);
  return;
}


void Easel1D::addVerticalLine(double x, int colour, double width) {
  //addLine(x, getGlobalMin(), x, 1.05 * getGlobalMax());
  addLine(x, getHistogram(0)->GetMinimum(), x, 1.05 * getGlobalMax(), colour, width);
  return;
}


void Easel1D::normalize() {
  //getHistogram(0)->Sumw2();
  for (unsigned int h=0; h<histograms_.size(); ++h) {
    if (!getHistogram(h)->InheritsFrom("TH1I")) {
      getHistogram(h)->Scale(1.0 / getHistogram(h)->Integral("width"));
    } else {
      getHistogram(h)->Scale(1.0 / getHistogram(h)->Integral("width"));
      oiwarning << "TH1I scaling... only integers allowed.";
    }
  }
  return;
}


void Easel1D::normalize(double scale) {
  for (unsigned int h=0; h<histograms_.size(); ++h) {
    getHistogram(h)->Scale(scale / getHistogram(h)->Integral("width"));
  }
  return;
}


void Easel1D::setMargins(TVirtualPad* pad) {
  //pad->SetTopMargin(0.08);
  //pad->SetRightMargin(0.05);
  //pad->SetBottomMargin(0.16);
  //pad->SetLeftMargin(0.14);
  if (!strcmp(getHistogram(0)->GetTitle(), "")) {
    pad->SetTopMargin(0.05);
    pad->SetBottomMargin(0.10);
  } else {
    pad->SetTopMargin(0.08);
    pad->SetBottomMargin(0.12);
  }
  pad->SetRightMargin(0.04);
  pad->SetLeftMargin(0.07);
  return;
}


void Easel1D::LHCbStyle() {
  Int_t lhcbFont = 132;
  Double_t lhcbWidth = 1.50;
  Double_t lhcbTSize = 0.06;

  gROOT->SetStyle("Plain");
  TStyle *lhcbStyle= new TStyle("lhcbStyle","LHCb plots style");

  lhcbStyle->SetFillColor(1);
  lhcbStyle->SetFillStyle(1001);   // solid
  lhcbStyle->SetFrameFillColor(0);
  lhcbStyle->SetFrameBorderMode(0);
  lhcbStyle->SetPadBorderMode(0);
  lhcbStyle->SetPadColor(0);
  lhcbStyle->SetCanvasBorderMode(0);
  lhcbStyle->SetCanvasColor(0);
  lhcbStyle->SetStatColor(0);
  lhcbStyle->SetLegendBorderSize(0);
  lhcbStyle->SetLegendFont(lhcbFont);

  lhcbStyle->SetPalette(1);

  lhcbStyle->SetPaperSize(20,26);
  //lhcbStyle->SetPadTopMargin(0.05);
  lhcbStyle->SetPadTopMargin(0.08);
  lhcbStyle->SetPadRightMargin(0.05); // increase for colz plots
  lhcbStyle->SetPadBottomMargin(0.16);
  lhcbStyle->SetPadLeftMargin(0.14);

  // use large fonts
  lhcbStyle->SetTextFont(lhcbFont);
  lhcbStyle->SetTextSize(lhcbTSize);
  lhcbStyle->SetLabelFont(lhcbFont,"x");
  lhcbStyle->SetLabelFont(lhcbFont,"y");
  lhcbStyle->SetLabelFont(lhcbFont,"z");
  lhcbStyle->SetLabelSize(lhcbTSize,"x");
  lhcbStyle->SetLabelSize(lhcbTSize,"y");
  lhcbStyle->SetLabelSize(lhcbTSize,"z");
  lhcbStyle->SetTitleFont(lhcbFont);
  lhcbStyle->SetTitleFont(lhcbFont,"x");
  lhcbStyle->SetTitleFont(lhcbFont,"y");
  lhcbStyle->SetTitleFont(lhcbFont,"z");
  lhcbStyle->SetTitleSize(1.2*lhcbTSize,"x");
  lhcbStyle->SetTitleSize(1.2*lhcbTSize,"y");
  lhcbStyle->SetTitleSize(1.2*lhcbTSize,"z");

  // use medium bold lines and thick markers
  lhcbStyle->SetLineWidth(lhcbWidth);
  lhcbStyle->SetFrameLineWidth(lhcbWidth);
  lhcbStyle->SetHistLineWidth(lhcbWidth);
  lhcbStyle->SetFuncWidth(lhcbWidth);
  lhcbStyle->SetGridWidth(lhcbWidth);
  lhcbStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes
  lhcbStyle->SetMarkerStyle(20);
  lhcbStyle->SetMarkerSize(0.5);

  // label offsets
  //lhcbStyle->SetLabelOffset(0.010,"X");
  //lhcbStyle->SetLabelOffset(0.010,"Y");

  // by default, do not display histogram decorations:
  lhcbStyle->SetOptStat(0);
  // full opts at http://root.cern.ch/root/html/TStyle.html#TStyle:SetOptStat
  lhcbStyle->SetStatFormat("6.3g"); // specified as c printf options
  lhcbStyle->SetOptTitle(1);  // Sohould be 0
  lhcbStyle->SetOptFit(0);
  //lhcbStyle->SetOptFit(1011); // order is probability, Chi2, errors, parameters
  //titles
  lhcbStyle->SetTitleOffset(0.95,"X");
  lhcbStyle->SetTitleOffset(0.95,"Y");
  lhcbStyle->SetTitleOffset(1.2,"Z");
  lhcbStyle->SetTitleFillColor(0);
  lhcbStyle->SetTitleStyle(0);
  lhcbStyle->SetTitleBorderSize(0);
  lhcbStyle->SetTitleFont(lhcbFont,"title");
  lhcbStyle->SetTitleX(0.0);
  lhcbStyle->SetTitleY(1.0);
  lhcbStyle->SetTitleW(1.0);
  lhcbStyle->SetTitleH(0.05);

  // look of the statistics box:
  lhcbStyle->SetStatBorderSize(0);
  lhcbStyle->SetStatFont(lhcbFont);
  lhcbStyle->SetStatFontSize(0.05);
  lhcbStyle->SetStatX(0.9);
  lhcbStyle->SetStatY(0.9);
  lhcbStyle->SetStatW(0.25);
  lhcbStyle->SetStatH(0.15);

  // put tick marks on top and RHS of plots
  lhcbStyle->SetPadTickX(1);
  lhcbStyle->SetPadTickY(1);

  // histogram divisions: only 5 in x to avoid label overlaps
  lhcbStyle->SetNdivisions(505,"x");
  lhcbStyle->SetNdivisions(510,"y");

  gROOT->SetStyle("lhcbStyle");
  gROOT->ForceStyle();
  return;
}


#include "Main_Window.H"

#include <iostream>

#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Native_File_Chooser.H>

#include <Fl_FITS_Image.H>

Main_Window::Main_Window() : Fl_Window(400, 200, "StarShootView")
{
  Fl::scheme("gtk+");
  menuBar = new Fl_Menu_Bar(0, 0, 400, 30);
  menuBar->add("&File/&Open", "^o", OpenCallback, (void*)this);
  menuBar->add("&File/&Quit", "^q", QuitCallback);

  imgBox = new Fl_Box(_boundary, _boundary+30, 400-_boundary, 200-_boundary);
}

Main_Window::~Main_Window ()
{}

void Main_Window::OpenCallback(Fl_Widget* w, void* data)
{
  Fl_Native_File_Chooser fnfc;
  fnfc.title("Pick a FITS file");
  fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);
  fnfc.filter("FITS\t*.{fits,fit,fts}");
  // show native chooser
  switch( fnfc.show() ) {
    case -1: fl_alert("ERROR: %s", fnfc.errmsg()); break;
    case  1: break;
    default: Load_Image(fnfc.filename(), data);
  }
}

void Main_Window::QuitCallback(Fl_Widget* w, void* data)
{
  exit(0);
}
void Main_Window::Load_Image(const char* filename, void* data)
{
  Main_Window *win = (Main_Window*)data;
  Fl_FITS_Image fits(filename);
  int img_w = fits.w(), img_h = fits.h();
  int src_w = Fl::w(), src_h = Fl::h();
  int x_pos = win->x_root();
  int y_pos = win->y_root();
  int boundary = win->Get_Boundary();
  suggest_size(src_w, src_h, img_w, img_h, boundary);

  Fl_Image* new_fits = fits.copy(img_w, img_h);

  win->resize(x_pos, y_pos, img_w + boundary, img_h + boundary + 30);
  win->menuBar->resize(0, 0, img_w + boundary, 30);
  win->imgBox->resize(boundary/2, boundary/2 + 30, img_w, img_h);
  win->imgBox->image(new_fits);
  win->redraw();
}
void Main_Window::suggest_size(int src_w, int src_h, int& img_w, int& img_h, int boundary)
{
  // Amount to Scale
  float scale_amount = 1.0;

  if (img_w > src_w)
    scale_amount = src_w * 1.0f / img_w;
  if (img_h > src_h)
  {
    float sc = src_h * 1.0f / img_h;
    if (sc < scale_amount) scale_amount = sc;
  }
  img_w = img_w * scale_amount - boundary;
  img_h = img_h * scale_amount - boundary - 40;
}

#include "Main_Window.H"
#include "Fl_Levels_Chooser.H"
#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Native_File_Chooser.H>

Main_Window::Main_Window() : Fl_Window(400, 200, "StarShootView")
{
  /* Choose the FLTK Theme */
  Fl::scheme("gtk+");

  /* Create the menu bar */
  menuBar = new Fl_Menu_Bar(0, 0, 400, 30);

  /* Create the quit/open button and attach the callback functions */
  menuBar->add("&File/&Open", "^o", OpenCallback, (void*)this);
  menuBar->add("&File/&Quit", "^q", QuitCallback);

  menuBar->add("&Tools/&Levels", "^l", LevelsCallback, (void*)this);

  /* Create the Box which will contain our images */
  imgBox = new Fl_Box(_boundary, _boundary+30, 400-_boundary, 200-_boundary);
}

void Main_Window::OpenCallback(Fl_Widget* w, void* data)
{
  Fl_Native_File_Chooser fnfc;

  /* set the title */
  fnfc.title("Pick a FITS file");
  fnfc.type(Fl_Native_File_Chooser::BROWSE_FILE);

  /* This will restrict the kinds of file that show up */
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
  // If Quit menu option is called exit application
  exit(0);
}

void Main_Window::LevelsCallback(Fl_Widget* w, void* data)
{
  // void pointer to Main_Window pointer
  Main_Window* win = (Main_Window*)data;
  // Gets Fl_FITS_Image pointer from main window
  Fl_FITS_Image* ft = win->GetImgPtr();
  // Creates Level Chooser class
  Fl_Levels_Chooser* lc = new Fl_Levels_Chooser(ft->GetData(), 
                                                ft->w(), ft->h(), ft->d());
  // Shows Level Chooser Window
  lc->show();

  // Check status
  if(lc->Status()==1)
  {
    double lo = lc->GetLowLevel();
    double hi = lc->GetHighLevel();
    Fl_FITS_Image* imgptr = win->GetImgPtr();
    imgptr->Update_Levels(hi, lo);

    /* Calculate Current Image Size */
    int boundary = win->Get_Boundary();
    int img_w = win->w() - boundary;
    int img_h = win->h() - (boundary + 30);

    /* Resize FITS Image */
    Fl_Image* new_fits = imgptr->copy(img_w, img_h);

    // Update image
    win->imgBox->image(new_fits);
    win->redraw();
  }
}

void Main_Window::Load_Image(const char* filename, void* data)
{
  /* Set void pointer as Main_Window pointer */
  Main_Window *win = (Main_Window*)data;

  /* Create FITS object and get FITS image */
  Fl_FITS_Image* imgptr = new Fl_FITS_Image(filename);
  win->SetImgPtr(imgptr);

  /* Get screen size, image size, position, and window position */
  int img_w = imgptr->w(), img_h = imgptr->h();           // Image Size
  int src_w = Fl::w(), src_h = Fl::h();             // Screen Size
  int x_pos = win->x_root(), y_pos = win->y_root(); // Window Position
  int boundary = win->Get_Boundary();               // Boundary Size

  /* Calculate a good image size to scale to */
  suggest_size(src_w, src_h, img_w, img_h, boundary);

  /* Resize image */
  Fl_Image* new_fits = imgptr->copy(img_w, img_h);

  /* Update image, box size, window size, menu bar size, and redraw */
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

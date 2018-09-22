#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_Box.H>

#include <Fl_FITS_Image.H>

int main() {
  fl_register_images();
  Fl_FITS_Image fltk("leo.fit");
  Fl_Window win(fltk.w() + 20, fltk.h() + 20);
  Fl_Box box(10, 10, fltk.w(), fltk.h());
  box.image(fltk);
  win.show();
  return(Fl::run());
}

#include <errno.h>
#include <algorithm>
#include <FL/fl_ask.H>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl_Box.H>

#include <Fl_FITS_Image.H>

void suggest_size(int src_w, int src_h, int& img_w, int& img_h, int boundary)
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
  img_h = img_h * scale_amount - boundary;
}

int main() {
  char filename[] = "leo_triplet_8bit_rgb.fit";
  
  // Get Window Size
  int src_w = Fl::w();
  int src_h = Fl::h();
  int img_w, img_h;
  int bd_size = 20; // Boundary Size

  fl_register_images();           // Initialize image lib
  Fl_FITS_Image fits(filename);   // Grab image
  img_w = fits.w();               // Get Image Size
  img_h = fits.h();

  // Get suggested image size
  suggest_size(src_w, src_h, img_w, img_h, bd_size);

  Fl_Image* new_fits = fits.copy(img_w, img_h);

  Fl_Window     win(img_w + bd_size, img_h + bd_size );  // make the window
  Fl_Box        box(bd_size/2, bd_size/2, img_w, img_h); // widget that will contain image

  // Error checking
  switch ( fits.fail() ) {
    case Fl_Image::ERR_NO_IMAGE:
    case Fl_Image::ERR_FILE_ACCESS:
      fl_alert("%s: %s", filename, strerror(errno)); // show error to user
      return 1;
    case Fl_Image::ERR_FORMAT:
      fl_alert("%s: couldn't decode image", filename);
      return 1;
  }

  box.image(new_fits); // attach fits image to box
  win.show();
  return(Fl::run());
}

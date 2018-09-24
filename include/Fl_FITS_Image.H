//
// FITS image header file for the Fast Light Tool Kit (FLTK)
//

#ifndef Fl_FITS_Image_H
#define Fl_FITS_Image_H
#include "FL/Fl_Image.H"

class FL_EXPORT Fl_FITS_Image : public Fl_RGB_Image {

public:
  Fl_FITS_Image(const char* filename);

private:
  void copy_double_to_uchar(uchar* output, double* input, long long size);
  void planar_to_rgb(uchar* ar, long long size);
};

#endif // Fl_FITS_Image_h

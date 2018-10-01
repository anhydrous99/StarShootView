//
// FITS image header file for the Fast Light Tool Kit (FLTK)
//

#ifndef Fl_FITS_Image_H
#define Fl_FITS_Image_H
#include "FL/Fl_Image.H"

class FL_EXPORT Fl_FITS_Image : public Fl_RGB_Image {

public:
  Fl_FITS_Image(const char* filename);
  ~Fl_FITS_Image() { }
  double* GetData() { return _dat; }
private:
  void copy_double_to_uchar(uchar* output, double* input, long long size);
  void planar_to_rgb(double* ar, long long size);
  double* _dat;
};

#endif // Fl_FITS_Image_h

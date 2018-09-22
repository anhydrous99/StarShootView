//
// Include necessary header files...
//

#include <Fl_FITS_Image.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl.H>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

extern "C"
{
#include <fitsio.h>
}

Fl_FITS_Image::Fl_FITS_Image(const char* filename): Fl_RGB_Image(0,0,0)
{
  fitsfile *fptr; /* FITS file pointer, defined in fitsio.h */
  int status = 0; /* CFITSIO status value MUST be initialized to zero! */
  int bitpix, naxis, ii, anynul;
  long naxes[3] = {1, 1, 1}, fpixel[3] = {1, 1, 1};
  char format[20], hdformat[20];

  /* Clear data ... */
  alloc_array = 0;
  array = (uchar*) 0;

  /* Open Fits File */
  if (!fits_open_file(&fptr, filename, READONLY, &status))
  {
    /* Get Fits parameters */
    if (!fits_get_img_param(fptr, 3, &bitpix, &naxis, naxes, &status))
    {
      if (naxis != 2 && naxis != 3)
        Fl::warning("Error: only 2D or 3D images are supported, it is %dD\n", naxis);
      else
      {
        w(naxes[0]);
	h(naxes[1]);
	d((naxis == 2) ? 1 : 3 );

	/* Check if Size is too Big! */
	if (((size_t)w()) * h() * d() > max_size() ) {
          Fl::warning("JPEG file \"%s\" is too large or contains errors!\n", filename);
	  return;
	}
	long long pn = w() * h() * d();
	array = new uchar[pn];
	alloc_array = 1;

	/* Read Image */
	if (bitpix == BYTE_IMG)
	  fits_read_pix(fptr, TBYTE, fpixel, pn, NULL, const_cast<uchar*>(array), NULL, &status);
	else
	{
          double* tmp = new double[pn];
	  fits_read_pix(fptr, TDOUBLE, fpixel, pn, NULL, tmp, NULL, &status);
	  copy_double_to_uchar(const_cast<uchar*>(array), tmp, pn);
	  delete [] tmp;
	}

	if (d() == 3)
          planar_to_rgb(const_cast<uchar*>(array), pn);
      }
    }
    /* Close Fits file */
    fits_close_file(fptr, &status);
  }
}

void Fl_FITS_Image::copy_double_to_uchar(uchar* output, double* input, long long size)
{
  auto ele = std::minmax_element(input, input+size);
  double max_ele = ele.first - input;
  double min_ele = ele.second - input;
  for (long long i = 0; i < size; i++)
    output[i] = static_cast<uchar>( (input[i] - min_ele) * 255.0 / (max_ele - min_ele) );
}
void Fl_FITS_Image::planar_to_rgb(uchar* ar, long long size)
{
  long long j;
  long long trd = size / 3;

  uchar f[trd];
  uchar s[trd];
  uchar t[trd];

  /* split */
  for (j = 0; j < trd; j++)
  {
    f[j] = *(ar+j);
    s[j] = *(ar+trd+j);
    t[j] = *(ar+2*trd+j);
  }

  /* Perfect Shuffle */
  for (j = 0; j < trd; j++)
  {
    *(ar+(j*3)) = f[j];
    *(ar+(j*3)+1) = s[j];
    *(ar+(j*3)+2) = t[j];
  }
}

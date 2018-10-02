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
  int bitpix, naxis;
  long naxes[3] = {1, 1, 1}, fpixel[3] = {1, 1, 1};

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
        /* Set dimensions */
        w(naxes[0]);
	      h(naxes[1]);
	      d((naxis == 2) ? 1 : 3 );

	      /* Check if Size is too Big! */
	      if (((size_t)w()) * h() * d() > max_size() ) {
          Fl::warning("JPEG file \"%s\" is too large or contains errors!\n", filename);

	        /* close fits file if File too big */
	        fits_close_file(fptr, &status);

	        /* set dimensions to 0 */
	        w(0);
	        h(0);
	        d(0);

	        /* delete array */
	        if (array) {
	          delete[] const_cast<uchar*>(array);
	          array = 0;
	          alloc_array = 0;
	        }

	        /* throw error */
	        ld(ERR_FORMAT);
	        return;
	      }

	      /* allocate array */
	      long long pn = w() * h() * d();
	      array = new uchar[pn];
	      alloc_array = 1;

	      /* Read Image */
        _dat = (double*)malloc(pn * sizeof(double));

	      /* Read image into double array */
	      fits_read_pix(fptr, TDOUBLE, fpixel, pn, NULL, _dat, NULL, &status);

	      /* FITS Images are store in a planar format but FLTK requires an interleaved format */
	      /* RRRRGGGGBBBB -> RGBRGBRGBRGB */
	      if (d() == 3)
          planar_to_rgb(_dat, pn);

	      /* Compress image into uchar array */
	      copy_double_to_uchar(const_cast<uchar*>(array), _dat, pn);
      }
    }
    /* Close Fits file */
    fits_close_file(fptr, &status);

    /* if error occurred, print out error message */
    if (status)
    {
      char toshow[30]; /* max error char of fits_get_errstatus is 30 */
      fits_get_errstatus(status, toshow);
      Fl::warning("Error: %s\n", toshow);
    }
  }
}

void Fl_FITS_Image::copy_double_to_uchar(uchar* output, double* input, long long size)
{
  /* Find largest and smallest number in array */
  double min_ele = 0;
  double max_ele = 0;
  for (int i = 0; i < size; i++)
  {
    if (min_ele > input[i]) min_ele = input[i];
    if (max_ele < input[i]) max_ele = input[i];
  }

  /* Scale Image to uchar based on the largest and smallest number in array */
  for (long long i = 0; i < size; i++)
    output[i] = static_cast<uchar>( (input[i] - min_ele) * 255.0 / (max_ele - min_ele) );
}
void Fl_FITS_Image::planar_to_rgb(double* ar, long long size)
{
  long long j;
  long long trd = size / 3;

  // Allocate temporary arrays
  double* f = (double*)malloc(trd * sizeof(double));
  double* s = (double*)malloc(trd * sizeof(double));
  double* t = (double*)malloc(trd * sizeof(double));

  /* split */
  for (j = 0; j < trd; j++)
  {
    f[j] = ar[j];
    s[j] = ar[trd+j];
    t[j] = ar[2*trd+j];
  }

  /* Perfect Shuffle */
  for (j = 0; j < trd; j++)
  {
    ar[j*3] = f[j];
    ar[(j*3)+1] = s[j];
    ar[(j*3)+2] = t[j];
  }

  delete [] f;
  delete [] s;
  delete [] t;
}
void Fl_FITS_Image::Update_Levels(double hi, double lo)
{
  /* Cast constant array to regular array for modification */
  uchar* output = const_cast<uchar*>(array);
  /* Calculate size of arrays */
  long long size = d() * w() * h();
  /* Find largest and smallest number in array */
  double min_ele = 0;
  double max_ele = 0;
  for (int i = 0; i < size; i++)
  {
    if (min_ele > _dat[i]) min_ele = _dat[i];
    if (max_ele < _dat[i]) max_ele = _dat[i];
  }
  /* Scale according to the cutoffs */
  for (int i = 0; i < size; i++)
  {
    if (_dat[i] < lo)
      output[i] = 0;
    else if (_dat[i] > hi)
      output[i] = 255;
    else
      output[i] = static_cast<uchar>(255*((_dat[i]-lo)/(hi-lo)-(min_ele)/(max_ele-min_ele)));
  }
}

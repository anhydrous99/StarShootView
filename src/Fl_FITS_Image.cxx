//
// Include necessary header files...
//

#include <Fl_FITS_Image.H>
#include <FL/Fl_Shared_Image.H>
#include <FL/Fl.H>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
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
	if (bitpix == BYTE_IMG)
	  fits_read_pix(fptr, TBYTE, fpixel, pn, NULL, const_cast<uchar*>(array), NULL, &status);
	else
	{
          /* Create a double array, to compress any kind of image into 8bit space */
          double* tmp = new double[pn];

	  /* Read image into double array */
	  fits_read_pix(fptr, TDOUBLE, fpixel, pn, NULL, tmp, NULL, &status);

	  /* Compress image into uchar array */
	  copy_double_to_uchar(const_cast<uchar*>(array), tmp, pn);

	  /* delete temporary array */
	  delete [] tmp;
	}

	/* FITS Images are store in a planar format but FLTK requires an interleaved format */
	/* RRRRGGGGBBBB -> RGBRGBRGBRGB */
	if (d() == 3)
          planar_to_rgb(const_cast<uchar*>(array), pn);
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
  auto ele = std::minmax_element(input, input+size);
  double max_ele = ele.first - input;
  double min_ele = ele.second - input;

  /* Scale Image to uchar based on the largest and smallest number in array */
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

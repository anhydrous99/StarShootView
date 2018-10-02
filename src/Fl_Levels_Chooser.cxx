#include "Fl_Levels_Chooser.H"

#include <FL/Fl.H>

#include <map>
#include <cmath>
#include <algorithm>

// Constructor
Fl_Levels_Chooser::Fl_Levels_Chooser(double* image, int width, int height, int d)
{
  // Fl_Window
  window = new Fl_Window(490, 430, "Levels");
  window->user_data((void*)this);

  // Create Chart object
  histogramChart = new Fl_Chart(10, 10, 470, 320, 0);
  histogramChart->type(FL_FILL_CHART);

  // Create sliders
  highlevel = new Fl_Hor_Nice_Slider(10, 340, 470, 20, 0);
  lowlevel = new Fl_Hor_Nice_Slider(10, 370, 470, 20, 0);

  // Create Buttons
  setButton = new Fl_Return_Button(440, 395, 40, 25, "Set");
  cancelButton = new Fl_Button(370, 395, 65, 25, "Cancel");

  // Sets callback functions for buttons
  setButton->callback((Fl_Callback*)cb_setButton);
  cancelButton->callback((Fl_Callback*)cb_cancelButton);

  // Zeros this' callback function and stored data pointer
  callback_ = 0;
  data_ = 0;

  // Number of doubles in image
  int npixel = width * height * d;

  // Number of points in histogram to display
  int nviz_points = 255;

  /* Find largest and smallest number in array */
  double min_ele = 0;
  double max_ele = 0;
  for (int i = 0; i < npixel; i++)
  {
    if (min_ele > image[i]) min_ele = image[i];
    if (max_ele < image[i]) max_ele = image[i];
  }

  // Size per point
  double ps = (max_ele - min_ele) / nviz_points;

  // Create histogram Data
  std::map<double, uint64_t> dic;
  for(int i = 0; i < npixel; i++)
  {
    double cur = roundclose(image[i], ps);
    if (!dic.count(cur))
      dic.insert(std::pair<double, uint64_t>(cur, 1));
    else
      dic.at(cur)++;
  }

  // Set Chart bounds
  histogramChart->bounds(min_ele, max_ele);

  // Add data to histogramChart
  for(auto& item: dic)
  {
    histogramChart->add(item.second, 0, FL_RED);
  }
  histogramChart->redraw();

  // Add bounds for sliders
  highlevel->bounds(min_ele, max_ele);
  lowlevel->bounds(min_ele, max_ele);

  // Set Values for sliders
  highlevel->value(max_ele);
  lowlevel->value(min_ele);

  // Set high and low values
  _high = max_ele;
  _low = min_ele;

  // Set Callbacks
  highlevel->callback(OnHighLevelChanged, (void*)this);
  lowlevel->callback(OnLowLevelChanged, (void*)this);
}

/* Calculates the closest multiple of s to x */
double Fl_Levels_Chooser::roundclose(double x, double s)
{
  double m = floor(x / s);
  double k = round((x - s * m) / s);
  if (k == 0)
    return s * m;
  else if (k == 1)
    return s * (m + 1);
}

// Destructor
Fl_Levels_Chooser::~Fl_Levels_Chooser() {}

// Setup callback function
void Fl_Levels_Chooser::callback(void (*cb)(Fl_Levels_Chooser *, void *), void *d)
{
  callback_ = cb;
  data_ = d;
}

void Fl_Levels_Chooser::show()
{
  // Show the Chooser
  window->show();

  // BLOCK WHILE CHOOSER SHOWN
  while( window->shown() ) {
    Fl::wait();
  }

  // Flush
  Fl::flush();
}

void Fl_Levels_Chooser::OnHighLevelChanged(Fl_Widget* w, void* ti)
{
  // Cast Widget to Slider to have access to the value function
  Fl_Hor_Nice_Slider* sl = (Fl_Hor_Nice_Slider*)w;

  // Cast void pointer to Fl_Levels_Chooser pointer
  Fl_Levels_Chooser* ptr = (Fl_Levels_Chooser*)ti;

  // Grab value from widget
  double nw = sl->value();

  // Change stored value in class
  ptr->SetHighLevel(nw);

  // Get Low Level
  double lo = ptr->GetLowLevel();

  // Check if low level is higher or equal to high level
  if (lo >= nw)
  {
    ptr->SetLowLevel(nw - 0.1);
    ptr->lowlevel->value(nw - 0.1);
  }
}

void Fl_Levels_Chooser::OnLowLevelChanged(Fl_Widget* w, void* ti)
{
  // Cast Widget to Slider to have access to the value function
  Fl_Hor_Nice_Slider* sl = (Fl_Hor_Nice_Slider*)w;

  // Cast void pointer to Fl_Levels_Chooser pointer
  Fl_Levels_Chooser* ptr = (Fl_Levels_Chooser*)ti;

  // Grab value from widget
  double nw = sl->value();

  // Change stored value in class
  ptr->SetLowLevel(nw);

  // Get High Level
  double hi = ptr->GetHighLevel();

  // Check if high level is lower or equal to low level
  if (hi <= nw)
  {
    ptr->SetHighLevel(nw + 0.1);
    ptr->highlevel->value(nw + 0.1);
  }
}

void Fl_Levels_Chooser::cb_setButton_i(Fl_Return_Button*, void*)
{
  // Hides Window
  window->hide();
  // Sets status
  _status = 1;
  // Do any callback that is registered...
  if (callback_)
    (*callback_)(this, data_);
}

// Calls Non-static function
void Fl_Levels_Chooser::cb_setButton(Fl_Return_Button* o, void* v) {
  ((Fl_Levels_Chooser*)(o->parent()->user_data()))->cb_setButton_i(o,v);
}

void Fl_Levels_Chooser::cb_cancelButton_i(Fl_Button*, void*)
{
  // Hides Window
  window->hide();
  // Sets status as canceled
  _status = -1;
}

// Calls Non-static function
void Fl_Levels_Chooser::cb_cancelButton(Fl_Button* o, void* v) {
  ((Fl_Levels_Chooser*)(o->parent()->user_data()))->cb_cancelButton_i(o,v);
}

// Gets data pointer
void * Fl_Levels_Chooser::user_data() const {
  return (data_);
}

// Sets data pointer
void Fl_Levels_Chooser::user_data(void *d) {
  data_ = d;
}

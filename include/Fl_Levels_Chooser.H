#ifndef Fl_Levels_Chooser_H
#define Fl_Levels_Chooser_H

#include <FL/Fl_Window.H>
#include <FL/Fl_Chart.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Return_Button.H>
#include <FL/Fl_Hor_Nice_Slider.H>

class FL_EXPORT Fl_Levels_Chooser {
private:
  Fl_Hor_Nice_Slider* highlevel;
  Fl_Hor_Nice_Slider* lowlevel;
  Fl_Return_Button* setButton;
  Fl_Button* cancelButton;
  Fl_Chart* histogramChart;
  Fl_Window* window;

  double _high, _low;
  int _status = 0;

  void (*callback_)(Fl_Levels_Chooser*, void*);
  void *data_;

  double roundclose(double num, double multiple);

  static void OnHighLevelChanged(Fl_Widget* w, void* d);
  static void OnLowLevelChanged(Fl_Widget* w, void* d);

  inline void cb_setButton_i(Fl_Return_Button*, void*);
  static void cb_setButton(Fl_Return_Button*, void*);

  inline void cb_cancelButton_i(Fl_Button*, void*);
  static void cb_cancelButton(Fl_Button*, void*);
public:
  Fl_Levels_Chooser(double* image, int width, int height, int d);
  ~Fl_Levels_Chooser();

  int Status() { return _status; }

  double GetHighLevel() { return _high; }
  void SetHighLevel(double high) { _high = high; }

  double GetLowLevel() { return _low; }
  void SetLowLevel(double low) { _low = low; }

  void callback(void (*cb)(Fl_Levels_Chooser *, void *), void *d = 0);

  void show();

  void * user_data() const;
  void user_data(void* d);
};

#endif // Fl_Levels_Chooser_H

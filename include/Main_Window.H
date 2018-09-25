#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Menu_Bar.H>

class Main_Window : public Fl_Window
{
public:
  /* Constructor and Destructor */
  Main_Window();
  ~Main_Window() {}

  /* FLTK Widgets */
  Fl_Menu_Bar *menuBar;
  Fl_Box *imgBox;

  /* fucs to access _boundary */
  int Get_Boundary() { return _boundary; }
  void Set_Boundary(int boundary) { _boundary = boundary; }

private:
  /* boundary around image */
  int _boundary = 10;
  /* Called by Open button in menu bar */
  static void OpenCallback(Fl_Widget* w, void* data);
  /* Called by Quit button in menu bar */
  static void QuitCallback(Fl_Widget* w, void* data);
  /* Loads the image into Fl_Box */
  static void Load_Image(const char* filename, void* data);
  /* Figures out perfered size for image to be resized */
  static void suggest_size(int src_w, int src_h, int& img_w, int& img_h, int boundary);
};

#endif // MAINWINDOW_H

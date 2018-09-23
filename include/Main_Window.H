#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Menu_Bar.H>

class Main_Window : public Fl_Window
{
public:
  Main_Window();
  ~Main_Window();

  Fl_Menu_Bar *menuBar;
  Fl_Box *imgBox;

  int Get_Boundary() { return _boundary; }
  void Set_Boundary(int boundary) { _boundary = boundary; }

  bool HasImage() { return _hasimg; }

private:
  int _boundary = 10;
  bool _hasimg = false;
  static void OpenCallback(Fl_Widget* w, void* data);
  static void QuitCallback(Fl_Widget* w, void* data);
  static void Load_Image(const char* filename, void* data);
  static void suggest_size(int src_w, int src_h, int& img_w, int& img_h, int boundary);
};

#endif // MAINWINDOW_H

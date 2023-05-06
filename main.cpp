// This code example is created for educational purpose
// by Thorsten Thormaehlen (contact: www.thormae.de).
// It is distributed without any warranty.

#include <QApplication>
#include "MyWidget.h"
#include "parameters.h"

int main (int argc, char* argv[]) {
  // create a QApplication object that handles initialization,
  // finalization, and the main event loop
  QApplication appl(argc, argv);
  ParaFrame widget;  // create a widget
  widget.show(); //show the widget and its children
  return appl.exec(); // execute the application
}

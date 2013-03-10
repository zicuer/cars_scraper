#include "stdafx.h"
#include "gui/main_window.h"

int main ( int argc, char ** argv )
{
   QApplication app (argc, argv);
   app.addLibraryPath(utils::to_qt(L"./plugins/"));

   gui::main_window_t main_window;
   main_window.show();

   return app.exec();
}
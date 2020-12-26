#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]){
  int id = 0;
  if( argc > 1 ){
    sscanf(argv[1],"%d",&id);
    printf("main id = %d\n",id);
  }
  
  QApplication app(argc, argv);
  MainWindow window(id);

  window.show();

  return app.exec();
}

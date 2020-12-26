#ifndef LGLWIDGET_H
#define LGLWIDGET_H

#include <kqt.h>
#include <kqtgl.h>


class LGLWidget : public KQt::GLWidget
{
Q_OBJECT

 
 public:
 LGLWidget();
 ~LGLWidget();

 void create();
 void draw();


};


#endif

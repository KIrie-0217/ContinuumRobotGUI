#include <QtWidgets>
#include <ktl.h>
#include "../modules/controller.h"
#include "gl.h"

extern Controller *ctrl;

#define LIGHT_NUM 2
KQt::GL::Light light2[LIGHT_NUM];

KQt::GL::SerialMechanismDrawer drawer;

LGLWidget::~LGLWidget(){

}

LGLWidget::LGLWidget(){
  eye.setupView( 1200.0, -400.0,   400.0,
		 100.0,    0.0, 000.0,
		 0, 0, 1 );

  
  projection.setOrtho(700);
  /*
  light[0].setPosition( 0.0, 0.0, 1000.0 );
  light[0].setWhite( 0.3 );
  light[0].setIndex( GL_LIGHT1 );
  
  light[1].setPosition( 500.0, -100.0, 1200.0 );
  light[1].setWhite( 0.0 );
  light[1].setIndex( GL_LIGHT2 );
  */
  //flag_axis = true;
  //DEBUG_PRINT("VRArea cstr done\n");
}


void LGLWidget::create(){
  drawer.setMechanism( &ctrl->robotFK );
  drawer.setParallelOffset( 1, 40.0, 30.0/DEG );
  drawer.setParallelOffset( 2, 38.0, 30.0/DEG ); //pcd1の角度も含む
  drawer.create();

}

void LGLWidget::draw(){
  /*
  for(int i=0;i<LIGHT_NUM;i++){
    light[i].apply();
    light[i].draw();
  }
  */
  glPushMatrix();
  glTranslatef( 0.0, 0.0, -200.0 );
  KQt::GL::rotateMatrix( ~ctrl->T ); //共通座標で表示
  KQt::GL::drawAxis(4.0, 100.0); //共通座標
  KQt::GL::drawGridPlane( 400.0, 20.0, KQt::GL::BLUE);
  glPopMatrix();
  
  
  //KQt::GL::rotateMatrix( ctrl->T ); //共通座標で表示
  //if(flag_axis)
  KQt::GL::drawAxis(2.0, 150.0); //ローカル座標

  
  drawer.draw();
  KQt::GL::drawFrame( ctrl->robotFK.P(),
		      ctrl->robotFK.R(),
		      4.0, 50.0);

  glPushMatrix();
  glTranslatef( KTLV3(ctrl->robotFK.P()) );

  KQt::GL::drawSphere( 20.0, 8, 8, KQt::GL::GREEN );
  //KQt::GL::drawVector( 5*ctrl->Pdref, 10.0, KQt::GL::RED );
  KQt::GL::drawVector( ctrl->Pd   , 10.0, KQt::GL::GREEN );

  glPopMatrix();

}

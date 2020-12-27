#ifndef LMAINWINDOW_H
#define LMAINWINDOW_H

#include <kqt.h>
#include <mss.h>
#include "../modules/controller.h"
#include "gl.h"


class MainWindow : public KQt::MainWindow, public KQt::LogWriter
{
Q_OBJECT

  FILE *MNP_LOG_FILE;
  FILE *AXIS_LOG_FILE[DOF];
  std::string log_filename;
  std::string axis_log_filename[DOF];
  std::string logdir;

  int id = 0;
  
 public:
  MainWindow(int id=0);
  ~ MainWindow();
  
  QDialog* parameterDialog;

  KQt::ParameterManager pm;
  KQt::StartStopManager* startstop;  
  KQt::ModeSelector* modeSelector;
  KQt::ModeSelector* tasksSelector;
  KQt::DataLogger* dataLogger;
  KQt::Plotter* plotter;
  KQt::SignalMonitor* monitor;
  KQt::ReferenceDialog* referenceDialog;
  KQt::JointOperator* jointOperation;
  KQt::CartesianOperator* cartesianOperation;
  LGLWidget* glwidget;

  QLabel* statusLabel;

  KQt::ToggleLabel* servoOnLabel;
  KQt::ToggleLabel* lockLabel;
  
  Ktl::ModuleHandler ctrl_module;
  Ktl::ModuleHandler sim_module;
  Ktl::FileReader fr;
  
  void getParam();
  void getTasks();

  //-- override from MainWindow ------------
  void loadFile(const QString &fileName);
  bool saveFile(const QString &fileName);

  private slots:
  bool start();
  void stop();
  void periodic();
  void execParameterDialog();
  void execReferenceDialog();
  void applyReferences();
  void sendCommand(int id);
  void sendTasks(int id);

  //void operateJointMotion(int id,int trigger,int direction);
  void operateCartesianMotion(int id,int trigger,int direction);

  bool eventFilter(QObject* obj, QEvent* event);
  //------------------------------------------

  //-- override form LogWriter -----------------
  virtual bool write_logfile(void* data,size_t size) override;
  virtual int open_logfile() override;
  virtual void close_logfile() override; 
  //----------------------------------------------

 private:
   void layoutWidgets();
};


#endif

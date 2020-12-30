#include <unistd.h>
#include <ktl.h>
#include <QtUiTools>
#include <string>
#include "../modules/controller.h"
#include "mainwindow.h"

Controller* ctrl;

MainWindow::~MainWindow(){

  Controller::operator delete( ctrl, SHM_NAME[id] ); 
}

MainWindow::MainWindow(int id2){
  id = id2;
  ctrl = new (SHM_NAME[id]) Controller(id);

  setTitle(tr("sarm"));
  setFileExtension("dlt");
  
  QWidget* centralWidget = new QWidget;
  setCentralWidget(centralWidget);

  QHBoxLayout* main_hlayout = new QHBoxLayout;
  QVBoxLayout* main_vlayout = new QVBoxLayout;
  QHBoxLayout* upper_hlayout = new QHBoxLayout;
  QHBoxLayout* lower_hlayout = new QHBoxLayout;

  startstop = new KQt::StartStopManager;
  monitor = new KQt::SignalMonitor;
  plotter = new KQt::Plotter;
  dataLogger = new KQt::DataLogger;
  referenceDialog = new KQt::ReferenceDialog(this);
  modeSelector = new KQt::ModeSelector;
  
  statusLabel = new QLabel("unloaded");
  statusBar()->addPermanentWidget(statusLabel);
    
  QPushButton* paramButton = new QPushButton("Parameter");
  connect(paramButton, SIGNAL(clicked()),
	  this, SLOT(execParameterDialog()) );

  QPushButton* refButton = new QPushButton("Reference");
  connect(refButton, SIGNAL(clicked()),
	  this, SLOT(execReferenceDialog()) );
  connect(referenceDialog, SIGNAL(accepted()),
	  this, SLOT(applyReferences()) );

  QGridLayout* gridLayout = new QGridLayout;
  gridLayout->addWidget( paramButton, 0,1 );
  gridLayout->addWidget( refButton, 1,1 );

  startstop->setStartStopAction(this,
				&MainWindow::start, &MainWindow::stop);
  //startstop->setPeriodicAction(this, SLOT(periodic()) );
  QTimer* ptimer = new QTimer( this );
  connect( ptimer, SIGNAL(timeout()), this, SLOT( periodic() ));
  ptimer->start( 100 );
  
  glwidget = new LGLWidget;
  glwidget->start();

  QTabWidget* tab = new QTabWidget;

  cartesianOperation = new KQt::CartesianOperator;
  connect(cartesianOperation, SIGNAL(stateChanged(int,int,int)),
	  this, SLOT(operateCartesianMotion(int,int,int)));
  

  //------------- Module Seting ---------------------------------
  ctrl_module.rgst( "sarm_module"+std::to_string(id),
		    "../modules/ctrl_module "+std::to_string(id));
  
  //if( ctrl_module.exists() ) ctrl_module.open_handle();
  KQt::ModuleButton* ctrlButton = new KQt::ModuleButton("ctrl",&ctrl_module);

  sim_module.rgst( "sarm_sim", 
		   "../modules/sim_module "+std::to_string(id));
  //if( sim_module.exists() ) sim_module.open_handle();
  KQt::ModuleButton* simButton = new KQt::ModuleButton("sim",&sim_module);
  simButton->update();

  QHBoxLayout* module_layout = new QHBoxLayout;

  module_layout->addWidget(ctrlButton);
  module_layout->addWidget(simButton);

  QGroupBox* module_box = new QGroupBox("Modules");
  module_box->setLayout(module_layout);
  module_box->setStyle( QStyleFactory::create("gtk"));//QGroupBox frame
  gridLayout->addWidget(module_box,3,1);
  //----------------------------------------------------------------

  centralWidget->setLayout(main_vlayout);
  main_vlayout->addLayout( upper_hlayout , 1 );
  main_vlayout->addLayout( lower_hlayout );
  setContentsMargins(0,0,0,0); //0にする
  centralWidget->setContentsMargins(0,0,0,0); //0にする
  main_hlayout->setContentsMargins(0,0,0,0); //0にする
  upper_hlayout->setContentsMargins(0,0,0,0); //0にする
  upper_hlayout->setSpacing(0); //0にする
  
  upper_hlayout->addWidget(monitor);    
  upper_hlayout->addWidget(glwidget);
  lower_hlayout->addWidget(tab);


  //---- Command Buttons ----------------------------------
  QGroupBox* commandBox = new QGroupBox("Command to Arm");
  QVBoxLayout* commandLayout = new QVBoxLayout;
  commandBox->setLayout(commandLayout);
  QButtonGroup* buttonGroup = new QButtonGroup;
  buttonGroup->setExclusive(false);


  for(int i=0;i<COMMAND_LAST-1;i++){
    if(i==COMMAND_START || i==COMMAND_STOP )continue;
    QPushButton* button = new QPushButton(command_text(i));
    button->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    buttonGroup->addButton(button,i);
    commandLayout->addWidget(button,1);
  }

  connect(buttonGroup, SIGNAL(buttonClicked(int)),
	  this, SLOT(sendCommand(int)) );
  
  
  lower_hlayout->addWidget(commandBox);	  
  lower_hlayout->addWidget(startstop);

  QWidget* tab1 = new QWidget;
  tab->addTab(tab1,"config");
  tab1->setLayout(main_hlayout);
  main_hlayout->addWidget(plotter);    
  main_hlayout->addWidget(dataLogger);    
  main_hlayout->addLayout(gridLayout);
  paramButton->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );  
  gridLayout->addWidget( paramButton, 0,1 );
  refButton->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );  
  gridLayout->addWidget( refButton, 1,1 );
  main_hlayout->addWidget(modeSelector); 


  QWidget* tab2 = new QWidget;
  tab->addTab(tab2,"operation");
  QHBoxLayout* layout = new QHBoxLayout;


  

  //layout->addWidget(cartesianOperation);
  //layout->addWidget(jointOperation);

  //--------- Setting Monitor --------------------------------------
  monitor->setAxes("1-1","1-2","1-3","2-1","2-2","2-3","3-1","3-2","3-3",NULL);
#define DATA(name) (&ctrl->name)

  monitor->addType( "q");

  for(int j=0;j<9;j++)
    monitor->registerData( -360.0, 360.0, 0, DEG,
			   DATA(joint[j].qref),DATA(joint[j].q) );
  monitor->registerData( 0.0, 50.0, 0, DEG,
			 DATA(joint[6].qref),DATA(joint[6].q) );
  
  monitor->addType( "qd");
  for(int j=0;j<DOF;j++)
    monitor->registerData(-100.0,100.0, 0, DEG,
			  DATA(joint[j].qdref),DATA(joint[j].qd) );
  
  monitor->addType( "qdd");
  for(int j=0;j<DOF;j++)
    monitor->registerData(-1000.0,1000.0, 0, DEG, DATA(joint[j].qddref),
			  DATA(joint[j].qdd) );


  monitor->addType( "magne");
  for(int j=0;j<DOF;j++)
    monitor->registerData(0,10.0, 0, 1, DATA(magne[j]) );

    
 
  /*
  monitor->addType( "tau_ref");
  for(int j=0;j<DOF;j++)
    monitor->registerData( -200.0, 200.0, 0, 1.0,
			   DATA(joint[j].tau_ref), DATA(joint[j].tau_ff) );
  
  */

 /*
  monitor->addType( "u");
  for(int j=0;j<DOF;j++)
    monitor->registerData( -10.0, 10.0, 0, 1.0, DATA(u[j]) );
  */

 /*
  monitor->addType( "P IK");
  monitor->registerData( -100.0, 100.0, 0, 1.0,
			 DATA(robotIK.P()[0]),
			 DATA(robotIK.P()[1]),
			 DATA(robotIK.P()[2]));
  */
  
  /*
  monitor->addType( "Pd");
  monitor->registerData( -100.0, 100.0, 0, 1.0, DATA(Pd[0]),DATA(Pd[1]),DATA(Pd[2]) );

  monitor->addType( "w");
  monitor->registerData( -100.0, 100.0, 0, 1.0, DATA(w[0]),DATA(w[1]),DATA(w[2]) );
  */


  monitor->addType( "torque");
  for(int j =0; j<DOF;j++)
    monitor->registerData( -1000.0, 1000.0, 0, 1.0, DATA(torque[j]) );


  //---------------------------------------------------------------
  //monitor->addType( "gr");
  //monitor->registerData( 0.0, 1.0, -1, 1.0, DATA(grip) );


  //--- setting logger ------------------------------------
  log_filename = "log.dat";
  logdir = "log_latest";
  for(int j=0;j<DOF;j++)
    axis_log_filename[j] = "axis" + std::to_string(j) + ".dat";

  dataLogger->rgstReceiver( "sarm_log"+std::to_string(id),
			    sizeof(LogData)*BUF_TIME_SIZE, this);
  dataLogger->setLogName(logdir);

  plotter->setDir( logdir );

  for(int j=0;j<DOF;j++)
    plotter->addFile(axis_log_filename[j]);
  
  plotter->addButton("q","[rad]");
  plotter->addLine(LOG_Q_REF+2,"reference");
  plotter->addLine(LOG_Q+2,"position");
  plotter->addButton("qd","[rad/s]");
  plotter->addLine(LOG_Q_REF_D+2,"qdref");
  plotter->addLine(LOG_Q_D+2,"qd");

  plotter->addButton("tau","[mNm]");
  plotter->addLine(LOG_TAU+2,    "tau");
  plotter->addLine(LOG_ZFF+2,"Zff");

  plotter->incrementGroup();
  plotter->addFile(log_filename);
  const char* axis[]={"x","y","z"};
  plotter->addButton("Pr","[mm]");
  for(int j=0;j<3;j++)
    plotter->addLine(j+2,axis[j]);
  
  plotter->addButton("Pd","[mm]");
  for(int j=0;j<3;j++)
    plotter->addLine(j+5,axis[j]);

  
  //---------------------------------------------------------
  modeSelector->addMode("Task",MODE_TASK);     
  modeSelector->addMode("IK",MODE_TEST_IK_MOTION);
  modeSelector->addMode("FK",MODE_TEST_FK_MOTION);
  modeSelector->setMode(2);


  //const QString IMG_PATH("/usr/local/lib/kqt/img/");
  const QString IMG_PATH("./img/");

  lockLabel = new KQt::ToggleLabel(IMG_PATH+"lock.png",
			      IMG_PATH+"unlock.png");
  statusBar()->addPermanentWidget(lockLabel);

  servoOnLabel = new KQt::ToggleLabel();
  statusBar()->addPermanentWidget(servoOnLabel);
  
  statusBar()->addPermanentWidget(startstop->timeDisplay);
  
  QFile file("parameter.ui");
  //file.open(QIOArm::ReadOnly);
  
  QUiLoader loader;   //qdesigner を利用
  parameterDialog = (QDialog*) loader.load(&file, this);

  referenceDialog->appendWaveList("Pref", 6);
  referenceDialog->appendWaveList("Qref", DOF);

  KQt::JointControlEditor* jointControlEditor = new KQt::JointControlEditor;
  for(int j=0;j<DOF;j++){
    jointControlEditor->appendRotaryJoint();
  }
  QVBoxLayout* vl = parameterDialog->findChild<QVBoxLayout*>("verticalLayout");
  vl->insertWidget(0, jointControlEditor);

  pm.setList( ctrl->parameterList() ); //関連付けを取得
  
  //GUI表示のスケーリングを設定・ファイルには適用されない
  for(int j=0;j<DOF;j++){
    pm.setWidgetMagnification("Kpp" , j , 1e-3); // mNm/rad -> Nm/rad
    pm.setWidgetMagnification("Kpd" , j , 1e-3);
    pm.setWidgetMagnification("Kpdd" ,j , 1e-3);
    pm.setWidgetMagnification("Qref", j , DEG); // rad -> deg
  }
  for(int j=3;j<6;j++)
    pm.setWidgetMagnification("Pref", j , DEG); // rad -> deg

  pm.excludeWidgetAssociation("mode");

  //登録されている変数アドレスをその名前に対応するwidgetに関連付ける
  pm.associateWidgets(this);

  pm.setupFileReading(fr); //frに伝達

  readSettings();

  if( !currentFile().isEmpty() )
    loadFile(currentFile());//

  qApp->installEventFilter(this); //こうしないとspaceとか取れない
  //layoutWidgets();
}

void MainWindow::layoutWidgets(){
 
}


void MainWindow::operateCartesianMotion(int id,int t,int d){
  printf("%d %2d %d\n",id,t,d);
}


void MainWindow::sendCommand(int cmd){
  if( !ctrl_module.exists() ){
    printf("CtrlKit:: control module is not loaded\n ");
    return;
  }
  statusBar()->showMessage(tr(command_text(cmd)), 2000);
  ctrl_module.send_command(cmd);
}


void MainWindow::execParameterDialog(){
  pm.setValuesToWidgets(parameterDialog);
  int ret = parameterDialog->exec();

  if( ret == 1 ){
    pm.getValuesFromWidgets(parameterDialog);
  }
}

void MainWindow::execReferenceDialog(){
  pm.setValuesToWidgets(referenceDialog);
  referenceDialog->show();
}

void MainWindow::applyReferences(){
  pm.getValuesFromWidgets(referenceDialog);
}

void MainWindow::getParam(){ //GUI値を読んで変数に格納する
  pm.setValue("mode", modeSelector->currentMode() );
  pm.getValuesFromWidgets();
}



bool MainWindow::start(){

  if( ctrl->state != STATE_STANDBY ){
    printf("state is not standby\n");
    return false;
  }

  getParam();
  
  statusBar()->showMessage(tr("running control now"), 2000);

  sendCommand(COMMAND_START);

  if( dataLogger->isSyncChecked() )
    dataLogger->reserveStart();

  if( monitor->isAvailable() ) 
    monitor->start();

  return true;
}

/*******************************************************
 *        stop button
 *******************************************************/
void MainWindow::stop(){
  if( dataLogger->isSyncChecked() )
    dataLogger->stop();

  sendCommand(COMMAND_STOP);

  statusBar()->showMessage(tr("stoop"), 2000);
}


void MainWindow::periodic(){
  lockLabel->set( ctrl->state == MSS::STATE_READY ); //lock
  servoOnLabel->set( ctrl->flag_servo_on );
  //forceFeedbackLabel->set( ctrl->flag_force_feedback );
  
  statusLabel->setText(state_text(ctrl->state));
}


void MainWindow::loadFile(const QString &fileName){
  fr.read_file(fileName.toStdString() );//,true);

  pm.setValuesToWidgets();
  modeSelector->setMode(ctrl->mode);

  setCurrentFile(fileName);
  statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::saveFile(const QString &fileName){
  getParam();

  fr.write_file(fileName.toStdString(),true);
  
  setCurrentFile(fileName);
  statusBar()->showMessage(tr("File saved"), 2000);
  return true;
}

bool MainWindow::eventFilter(QObject* obj, QEvent* event){

  if( event->type() == QEvent::KeyPress){
    //printf("Basewindow eventFilter key\n");

    QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
    switch( keyEvent->key() ){
    case Qt::Key_C:

      break;
    case Qt::Key_L:
      //ctrl->flag_lock_trigger = true;
      break;
    }
    //return false;
  }
  else if( event->type() == QEvent::KeyRelease){
    QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
    switch( keyEvent->key() ){
    case Qt::Key_C:

      break;
    }

  }

  return QObject::eventFilter(obj,event);
}

/************************************************************/
/*          Data Logging Routine                            */
/************************************************************/
int MainWindow::open_logfile(){
  //--- open log file ------
  MNP_LOG_FILE = fopen(log_filename.c_str(),"w");

  for(int dof=0;dof<DOF;dof++ ){ // open axis data file
    AXIS_LOG_FILE[dof] =
      fopen( axis_log_filename[dof].c_str(),"w");

  }
  printf("opened\n");
  return 0;
}

bool MainWindow::write_logfile(void* data,size_t size){
  LogData* buf = (LogData*)data;

  if( is_first_step() )
    set_time_offset(buf[0].time);  

  for(unsigned int i=0; i<size/sizeof(LogData); i++){
    //-- MNP DATA ------------------------------------------------
    fprintf(MNP_LOG_FILE, "%6.3f ",buf[i].time - time_offset() );

    for(int j=0; j<MNP_LOG_SIZE; j++)
      fprintf(MNP_LOG_FILE,  " %f", buf[i].mnp_data[j] );
    fprintf(MNP_LOG_FILE, "\n");

    //-- AXIS DATA -------------------
    for(int dof=0;dof<DOF;dof++ ){
      fprintf(AXIS_LOG_FILE[dof], "%6.3f ",buf[i].time - time_offset() );
      for(int j=0; j<AXIS_LOG_SIZE; j++)
	fprintf(AXIS_LOG_FILE[dof], " %f",buf[i].axis_data[dof][j]);
      fprintf(AXIS_LOG_FILE[dof], "\n");
    }// dof
    
  }// tempfile
  return true;
}

void MainWindow::close_logfile(){
  printf("closing\n");
  fclose(MNP_LOG_FILE);
  
  for(int dof=0;dof<DOF;dof++ )
    fclose(AXIS_LOG_FILE[dof]);

  Ktl::shell("mkdir -p %s",logdir.c_str() );
  Ktl::shell("mv log.dat axis*.dat %s",logdir.c_str() );
}

#ifndef _DELTA
#define _DELTA

#include <mss.h>
#include "arm.h"
static const int DOF = 9;

static const char* SHM_NAME[2] = {"csh0","csh1"};


enum ControlMode{
  MODE_TASK,
  MODE_TEST_IK_MOTION,
  MODE_TEST_FK_MOTION,
};

enum Command{
  COMMAND_SERVO_ON, //motor on
  COMMAND_SERVO_OFF,//motor off
  COMMAND_START, //位置制御を開始し司令を待つ
  COMMAND_STOP,  //位置制御を停止する
  COMMAND_LOCK, //lock when mode is task 
  COMMAND_UNLOCK, //unlock when mode is task
  COMMAND_INIT, //Init
  COMMAND_DATA,
  COMMAND_DEMO,
  COMMAND_CONTROL,
  COMMAND_RESET_ORIGIN,
  COMMAND_INTERLOCK_RELEACE,
  COMMAND_ABORT,//SERVO_OFFと同じ.
  COMMAND_LAST
};


static const char* command_text(int com){
  static const char* command_text[]={
  "SERVO_ON",
  "SERVO_OFF",
  "START",
  "STOP",
  "LOCK",
  "UNLOCK",
  "1:INIT",
  "2:DATA",
  "3:DEMO",
  "4:Control_mode",
  "RESET_ORIGIN",
  "interlock_releace",
  "ABORT",
  };
  
  if( com < 0 || com >= COMMAND_LAST ){
    return "INVALID_COMMAND";
  }
  return command_text[com];
}



enum State{
  STATE_SERVO_OFF,
  //モータドライバサーボオフの状態

  STATE_STANDBY,
  //サーボオンとなりトルク制御された状態

  STATE_PREPARING,
  //befor unlock

  STATE_READY,
  //司令を受け付ける状態

  STATE_TASK_RUNNING,
  //司令されたタスクを実行している状態・タスクが正常終了するとREADY状態に戻る

  STATE_TEST_RUNNING,
  //テスト動作中．開発用

  STATE_INTERLOCK_RELEACE,
  //INTERLOCK

  STATE_LAST
};
 
static const char* state_text(int state){
  static const char* state_text[]={
    "SERVO_OFF",
    "STANDBY",
    "PREPARING",
    "READY",
    "TASK_RUNNING",
    "TEST_RUNNING",
    "INTERLOCK"
  };
  if( state < 0 || state >= STATE_LAST ){
    return "INVALID_STATE";
  }
  return state_text[state];
};

enum tasks_mode{
  TASKS_FIRST,
  TASKS_DATA,
  TASKS_DEMO,
  TASKS_I,
  TASKS_CONTROL,
};



class Controller : public Ktl::Dynamics, public Ktl::RTShm{
 public:
  const int id;
  int state;
  bool flag_emergency;
  int mode;
  int tasks;
  double qref_tmp_ctrl[DOF];
  double qref_tmp_ctrl_[DOF];
  double qref_[DOF];
  int click_signal[DOF];
  std::vector<std::vector<double>> radians;
  int radians_selector;

  double t; // for only start_PTP

  bool flag_feedforward;

  bool flag_servo_on;
  
  Ktl::Wave Qwave[DOF];
  Ktl::Wave Pwave[DOF];

  double I[DOF];
  double fc_qd;
  double fc_qdd;
  double fc_fext;

  double fth;
  double mth;

  double grip_force;

  Ktl::Vector<3> Poffset;
  Ktl::Vector<3> P0s;

  //---------------------------------
  Ktl::JointController joint[DOF];

  Ktl::Vector<DOF> u; //�����Ű� [V]

  Ktl::Vector<DOF> magne; //0 or 5 [V]

  double qdabs[DOF];
  int sat[DOF];  
  Ktl::Vector<3> fext;
  Ktl::Vector<3> mext;

  Ktl::Vector<3> Pd;
  Ktl::Vector<3> w;

 public:

  //------------------------------------------
  //private:
  Ktl::Vector<3> fref;
  Ktl::Vector<3> mref;


  Arm robotIK;
  Arm robotFK;

  Ktl::Vector<3> P0; //���̺�ɸ�Ǹ����Ȥʤ���֡ʥ������ɸ��
  Ktl::Shell shell; // working range
  Ktl::Vector<3> fshell;

  //-------------------

  bool flag_tatch;//=false;
  bool flag_pose_lock;

  int sw[4];//={0,0,0,0};
  int swpre[4];//={0,0,0,0};

  double qg;
  double qgd;


  Ktl::Differentiator< double > gdiff;
  //-------------

  Ktl::Matrix<3,3> T; //共通座標への変換行列
  Ktl::Matrix<3,3> Tt;//逆（転置）行列

 
  double torque[9];

  
 Controller(int num): id(num), flag_emergency(false),
    fc_qd(100.0), fc_qdd(100.0),
    fth( 0.1 )    {
    printf("created controller %d\n",id);
    setup_configuration();
    //3D create_lists の前に実行されている必要がある
  }

  void setup_configuration(){
    T = Ktl::Matrix<3,3>(Ktl::Z,-PI/2);
    /*
    T = id == 0 ? 
      Ktl::Matrix<3,3>(Ktl::Y,-PI/2)* Ktl::Matrix<3,3>(Ktl::Z, -PI)* Ktl::Matrix<3,3>(Ktl::X, -PI/4) :// R
      Ktl::Matrix<3,3>(Ktl::Y,-PI/2);  // L

    */
    Tt = ~T;
    robotFK.init();
    robotIK.init();
    robotFK.inverse_kinematics();
    P0 = robotFK.P();
    printf("DeltaController::configured as %d\n",id);
  }
  //printf("DeltaController::cstr\n");
  //}

  void* thread(void*);
  void setup();
  void init();
  //void init_force();

  void lock_process();

  //void impedance_control(Ktl::Vector<3> f, Ktl::Vector<3> m);

  //MSS::DeviceInfo packup_status();

  Ktl::TaggedDataList parameterList(){
    Ktl::TaggedDataList list;
    
    list.push_data( "feedforward", &flag_feedforward);
    list.push_data( "mode" , &mode );
    
    list.push_data("fc_qd"  ,&fc_qd);
    list.push_data("fc_qdd" ,&fc_qdd);
    
    
    for(int j=0;j<6;j++){
      list.push_data("Pref",j,&Pwave[j]);
    }
    
    for(int j=0;j<DOF;j++){
      list.push_data("Qref",j,&Qwave[j]);
    }

    for(int j=0;j<DOF;j++){
      list.push_data("Kpp", j ,&joint[j].Kpp);
      list.push_data("Kpd", j ,&joint[j].Kpd);
      list.push_data("Kpdd",j ,&joint[j].Kpdd);
      
      list.push_data("M",    j ,&joint[j].M);
      list.push_data("C",    j ,&joint[j].fr.C);
      list.push_data("Fd",   j ,&joint[j].fr.Fd);
      list.push_data("vlim", j ,&joint[j].fr.vlimit);
      list.push_data("K",    j ,&joint[j].K);
      list.push_data("Zo",   j ,&joint[j].Zo);
    }


    list.push_data("fth" ,&fth);
    list.push_data("mth" ,&mth);

    list.push_data("fc_fext" ,&fc_fext);
    //list.push_data("fc_qdd" ,&fc_qdd, parameterDialog);

    return list;
  }
  
};
//#endif



enum AXIS_LOG{  
  LOG_Q_REF, // 2  ( file column )
  LOG_Q_REF_D, 
  LOG_Q,      
  LOG_Q_D,    
  LOG_TAU,
  LOG_ZFF,
  LOG_VOLTAGE,
  LOG_LAST   
};

#define AXIS_LOG_SIZE LOG_LAST
#define MNP_LOG_SIZE 32
#define LOG_SKIP     1
#define BUF_TIME_SIZE 50

typedef struct{
  float time;
  float mnp_data[MNP_LOG_SIZE];
  float axis_data[DOF][AXIS_LOG_SIZE];
} LogData;


#endif


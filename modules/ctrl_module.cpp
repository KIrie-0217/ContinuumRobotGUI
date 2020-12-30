/**********************************************************************
 *   RTLinux module for the control of pneumatic cylinders
 *                                written by Kotaro Tadano  since 2003
 *********************************************************************/
#include <ktl.h>
#include <libts01.h>
//#include <mss.h>
//#include "arm.h"
#include "controller.h"

#define DEBUG
#ifdef DEBUG
# define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
# define DEBUG_PRINT(...) 
#endif


int magneflag[DOF] = {1,1,1,1,1,1,1,1,1};

unsigned int res(int n)
	{ return 0xffffffff >> (32 - n);}

static const double RR[DOF] =
  { 987.0, 987.0, 987.0, 987.0, 987.0, 987.0, 987.0 ,987.0,987.0};

static const double Rt[DOF] = //関節角度とエンコーダ角度との伝達比
  { 987.0, 987.0, 987.0, 987.0, 987.0, 987.0, 987.0 ,987.0,987.0};

//static const int BIT[DOF]={ 21, 19, 19, 18, 18, 18, 18};

static const double RPP[DOF] = { //１パルス(1LBR)あたりの角度 abs *not gear but motor
  2 * PI / 1024 /4,
 2 * PI / 1024,
 2 * PI / 1024,
 2 * PI / 1024,
 2 * PI / 1024,
 2 * PI / 1024,
 2 * PI / 1024,
 2 * PI / 1024,

 
};
static const double Ktu[DOF] = { //モータトルクから指令電圧への変換
  
  1.0/0.22/6.58, // 1/(800mA/V)/(231mNm/A)
  1.0/0.8/231,
  -1.0/0.6/217,
  10.0/15.3, 10.0/57.8, 10.0/15.3,  10.0/15.3  ,10.0/15.3  ,10.0/15.3  
};

static const double Q0[DOF] = 
  { 0.0/DEG, 0.0/DEG, 0.0/DEG, 0.0/DEG, 0.0/DEG, 0.0/DEG, 0.0, 0.0/DEG, 0.0/DEG};

// 0~2PI -> -PI~PI

double shift_range(double val){
  if (val < PI)
    return val;
  else
    return val - 2*PI;
}

static RTTask ctrl_task;

Controller* mctrl;


//Ktl::FIFO fifoInput[2];
TS01InputData input2[2];

TS01 ts01[2];

/*******************************************************************
 *    thread
*******************************************************************/
void* Controller::thread(void *p_arg){
ctrl_task.init();
printf("ctrl id = %d \n", id );

long count = 0;
const int period = 1000;
const double dt = 1e-6 * period;
Ktl::Dynamics::setdt(dt);

LogData buf;

int command;
int ret;


TS01OutputData output2[2];

Ktl::PPTrajectory<Ktl::Vector<3> > ptp;

Ktl::FIFO fifoLog("sarm_log"+std::to_string(id));
fifoLog.open(Ktl::FIFO::NON_BLOCKING); 

Ktl::FIFO torqueInput("torque");
torqueInput.open(Ktl::FIFO::NON_BLOCKING);


//Ktl::Timer timer;

flag_servo_on = false;

for(int j=0;j<DOF;j++){
  joint[j].setup();
  joint[j].init();
  joint[j].setup_parameters();
}

flag_emergency = false;


//----------------------------------------

state = STATE_SERVO_OFF;

ctrl_task.start(period);
ctrl_task.wait_message(false);


std::cout << "before while" << std::endl;

while(1) {
  bool flag_command_received = false;
  if( ctrl_task.receive(&command) ){
    flag_command_received = true;
    printf("received a command of %s from GUI at %ld\n",
      command_text(command),count);
  } // commandをGUIから受けたら表示


  // COMMANDの類
  if( flag_command_received ){
    //printf("ctrl_thread : received command %d\n",command);
    switch (command) {

      case COMMAND_SERVO_OFF:
        state = STATE_SERVO_OFF;
        flag_servo_on = false;
        tasks = TASKS_FIRST;
        for(int j=0;j<DOF;j++)
          joint[j].tau_ref = 0.0;
      break;

      case COMMAND_SERVO_ON:
        if( state != STATE_SERVO_OFF ) break;
        state = STATE_STANDBY;
        tasks = TASKS_FIRST;
        flag_servo_on = true;
      break;
    
      case COMMAND_STOP: 
        if( state == STATE_SERVO_OFF ) break;
        DEBUG_PRINT("delta thread : stopped\n");
        state = STATE_STANDBY;
      break;
    
      case COMMAND_START: //------------------------
        if( state != STATE_STANDBY ) break;

        for(int j=0;j<DOF;j++){
            joint[j].fc_qd = fc_qd;
            joint[j].setup_parameters();
        }
        //init();
        gdiff.set_cutoff_frequency(10.0);
        //printf("fc_fext=%f\n",fc_fext);

        DEBUG_PRINT("delta thread : period %d[us]\n",   period);

        switch(mode){
          case MODE_TASK:
            state = STATE_PREPARING;
          break;

          case MODE_TEST_FK_MOTION:
            state = STATE_TEST_RUNNING;

            for(int j=0;j<DOF;j++){
              joint[j].ptp.create( Qwave[j].generate(t),
                joint[j].q, 2.0, t);
            }
            rt_print("%ld : ptp start\n",count);
          break;
  
        }   
        fifoLog.flush();

      break;
    
      case COMMAND_C:
        if( state != STATE_PREPARING ) break;
        tasks = TASKS_C;
        state = STATE_PREPARING;
      break;


      case COMMAND_L:
        if(state != STATE_PREPARING  ) break;
        tasks = TASKS_L;
        state = STATE_PREPARING;
      break;


      case COMMAND_G:
        if(state != STATE_PREPARING ) break;
        tasks = TASKS_G;
        state = STATE_PREPARING;
      break;


      case COMMAND_INIT:
        if( state != STATE_PREPARING  ) break;
        tasks = TASKS_I;
        state = STATE_PREPARING;

      break;

      case COMMAND_RESET_ORIGIN:
        if( state < STATE_READY ){
          for(int i=0;i<2;i++)
            ts01[i].set_count(0);
          rt_print("ctrl thread: position initializing.\n");
          DEBUG_PRINT("delta thread: position initialized.\n");
        }
      break;
      //continue;


      case COMMAND_LOCK:
        if( state != STATE_TASK_RUNNING ) break;
        state = STATE_READY;
      break;

      case COMMAND_UNLOCK:
        if( state != STATE_READY ) break;
        state = STATE_TASK_RUNNING;
      break;

    }
  }

  
  //=== UPDATE_STATE ==========================================
  t = count * dt ;

  /*
  double ziki[9];
  for(int i=0; i<9;i++){
    ziki[i] = input2[0].v[i];
  }  
  printf("%d : %f %f %f  %f %f %f  %f %f %f\n",
  count,
    ziki[0],ziki[1], ziki[2],
    ziki[3], ziki[4], ziki[5],
    ziki[6], ziki[7], ziki[8]);
  */

  
  
  torqueInput.get(torque,sizeof(torque));
  printf("%d : %.3f %.3f %.3f  %.3f %.3f %.3f  %.3f %.3f %.3f\n",
  count,
    torque[0], torque[1], torque[2],
    torque[3], torque[4], torque[5],
    torque[6], torque[7], torque[8]);
  

  joint[0].update( RPP[0]*input2[0].count[0] / Rt[0] + Q0[0] );


  for(int j=0; j<DOF; j++){
    try{magne_[j]=magne[j];}
    catch(char *){magne_[j]= 0;}
    magne[j]= input2[0].v[j] ;

    if(abs(magne[j]-magne_[j]) >2 and magne[j] < 3){
      magneflag[j] = -1 * magneflag[j];
    }
    
  }

  std::cout <<"state:" <<state << std::endl;
  std::cout<< "mode:" << mode << std::endl;
  std::cout<< "tasks:" << tasks << std::endl;


  /*
  for(int i=0;i<2;i++){
    std::cout <<joint[0].q<<std::endl;
  }
  */
  
  if( flag_emergency ){
    for(int j=0;j<DOF;j++)
      joint[j].tau_ref = 0.0;
    u.zero();
    flag_servo_on = false;
  }
  
  
  //=== main process ==========================================
  switch(state){
    case STATE_SERVO_OFF:
    case STATE_STANDBY:

      for(int j=0;j<DOF;j++){
        joint[j].qref = joint[j].q;
        joint[j].Qref.update();
        joint[j].tau_ref = 0.0;
      }

    break;

    case STATE_PREPARING: //COMMAND_STARTにより遷移

      if( ptp.is_progressing(t) ){

      for(int j=0;j<DOF;j++){
        //joint[j].qref = robotIK.q(j);
        joint[j].Qref.update();
        joint[j].feedback_control();
        joint[j].tau_ref = joint[j].tau_fb;
      }
      }
      else{
        printf("getting READY\n");
        state = STATE_READY; //(locked)
      }
    break;
        
    case STATE_READY: //(locked)
      for(int j=0;j<DOF;j++){
        //joint[j].qref = robotIK.q(j);
        joint[j].Qref.update();
        joint[j].feedback_control();
        joint[j].tau_ref = joint[j].tau_fb;
      }
    break;

    case STATE_TASK_RUNNING:
      switch(tasks){
        case TASKS_C:
        break;

        case TASKS_G:
        break;

        case TASKS_L:
        break;

        case TASKS_I:
        break;

      }

      /*
      mref.zero();
      
      if( flag_pose_lock ){ //
        //wref.zero();
        for(int j=3;j<DOF;j++){ //
          joint[j].qdref = 0.2 *( 0.0- joint[j].qd );
        }
      }

      for(int j=0;j<DOF;j++){
        joint[j].qref = joint[j].q;
        joint[j].tau_ref = 0.0;
      }

        
      for(int j=0;j<DOF;j++){
        joint[j].feedforward_control(0,1,1);
      }

      */
      
    break;

    case STATE_TEST_RUNNING:
      if( mode == MODE_TEST_FK_MOTION ){
        for(int j=0;j<DOF;j++){
          joint[j].qref = joint[j].ptp.track( Qwave[j].generate(t), t );
        }
      }
      else if( mode == MODE_TEST_IK_MOTION ){

        Ktl::Vector<6> qref;// = robotIK.q();
        for(int j=3;j<DOF;j++)
          joint[j].qref = qref[j];
        for(int j=3;j<DOF;j++)
          joint[j].qref = Pwave[j].generate(t);      
      }
      
      for(int j=0;j<DOF;j++){
        joint[j].Qref.update();
        joint[j].Qdref.update();
        joint[j].feedback_control();
        joint[j].tau_ref = joint[j].tau_fb;
      }

    break; //END of TEST_RUNNING

  } //switch(state)


  // interlock----------------------------

  for(int j=0; j<DOF; j++){
    if(torque[j] > 500){
      state = STATE_SERVO_OFF;
    }
    
  }

  
  //==== gravity compensation (common process) =================
  if( flag_feedforward ){
    for(int j=0;j<DOF;j++){
      joint[j].tau_ref += joint[j].tau_ff;
      //printf("tau_ref %d %f\n",j,joint[j].tau_ref);
    }
  }

   
  //==== output =================================================
  output2:
  for(int j=0;j<DOF;j++){
    u[j] = Ktu[j] * joint[j].tau_ref / RR[j];
    Ktl::limitRange(&u[j], -4.0, 4.0 );
  }
  
  
  for(int j=0; j<DOF; j++){
    output2[0].dout[j] = flag_servo_on;
    output2[0].u[j] = u[j];
  }



  for(int i=0;i<2;i++)
    ts01[i].write_data(&output2[i]);


  if(  count % LOG_SKIP == 0 ){ //-- save data -----------------
    buf.time = t;
    int k=0;
    for(int j=0;j<3;j++)
buf.mnp_data[k++] = 0;
    
    for(int j=0;j<DOF;j++){
buf.axis_data[j][LOG_Q_REF    ] = joint[j].qref;
buf.axis_data[j][LOG_Q_REF_D  ] = joint[j].qdref;
buf.axis_data[j][LOG_Q        ] = joint[j].q;
buf.axis_data[j][LOG_Q_D      ] = joint[j].qd;
buf.axis_data[j][LOG_TAU      ] = joint[j].tau_ref;
buf.axis_data[j][LOG_ZFF      ] = joint[j].tau_ff;
buf.axis_data[j][LOG_VOLTAGE  ] = u[j];
    }
    fifoLog.put(&buf,sizeof(buf));
  }
  
  do_nothing:
  count++;

  ctrl_task.wait();
  
} //while

  return 0;
}


static void *ctrl_thread(void *p_arg){
const int id   = *((int*) p_arg);
printf("starting thread %d\n",id);
return mctrl->thread(p_arg);
}


int fifo_handler(void* p_msg){
int* msg = (int*) p_msg;

ctrl_task.send( *msg );

DEBUG_PRINT("sarm fifo_handler send %d\n",*msg);
return 0;
}



void* sampling_thread(void *p_arg){
int id = * (int*)p_arg;

while(1) {
  //
  ssize_t rsize = ts01[id].read_autosampling_data(&input2[id]);
  //input2[1].print();
}
//printf("thread done\n");
return 0;
}

/*******************************************************************
 *     init_module
 *******************************************************************/
int main(int ac,char* av[]){
  std::string name = "sarm_module"+std::to_string(0);
  //Ktl::ManagedProcess mp(name);
  Ktl::FIFOHandler handler(name);
  
  Ktl::ProcessManager sim_module("sarm_sim");
  if( sim_module.exists() ){
    ts01[0].open("localhost",0);
    ts01[1].open("localhost",1);
  }
  else{
	ts01[0].open("192.168.1.100",0);
	ts01[1].open("192.168.1.101",1);
  }

  Ktl::Thread as_thread[2];
  int id[2] = {0, 1};
  for(int i=0;i<2;i++){
    ts01[i].start_count();
    ts01[i].start_sampling(1000);
    as_thread[i].create(sampling_thread, (void*)&id[i]);
  }
  
  mctrl = new (SHM_NAME[0]) Controller(0);

  Ktl::FileReader fr;
  fr.set_list( mctrl->parameterList() );
  //fr.read_file("../modules/default.dlt",true);
  fr.read_file("/home/kotaro/workspace/serial_master/modules/default.dlt",true);
  
  
  ctrl_task.create(ctrl_thread, 2, &id);

  DEBUG_PRINT("create sarm thread\n");
  handler.handle(fifo_handler);


  /*************************************************
   *     cleanup_module
   ************************************************/
  ctrl_task.cancel();
  ctrl_task.join();
  
  Controller::operator delete( mctrl, SHM_NAME[0] );

  for(int i=0;i<2;i++){
    as_thread[i].cancel();
    
    ts01[i].stop_sampling(); //この順番
    ts01[i].close();
  }
  printf("control module has been removed.\n");

}




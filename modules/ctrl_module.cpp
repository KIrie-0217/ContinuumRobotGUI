/**********************************************************************
 *   RTLinux module for the control of pneumatic cylinders
 *                                written by Kotaro Tadano  since 2003
 *********************************************************************/
#include <ktl.h>
#include <libts01.h>
//#include <mss.h>
//#include "arm.h"
#include "controller.h"

#include "./wire_ref/readCSV.h"
#include <unistd.h>

#define DEBUG
#ifdef DEBUG
# define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
# define DEBUG_PRINT(...) 
#endif


unsigned int res(int n)
	{ return 0xffffffff >> (32 - n);}

static const double RR[DOF] =
  { 987.0, 987.0, 987.0, 987.0, 987.0, 987.0, 987.0 ,987.0,987.0};

static const double Rt[DOF] = //関節角度とエンコーダ角度との伝達比
  { 987.0, 987.0, 987.0, 987.0, 987.0, 987.0, 987.0 ,987.0,987.0};

//static const int BIT[DOF]={ 21, 19, 19, 18, 18, 18, 18};

static const double RPP[DOF] = { //１パルス(1LBR)あたりの角度 abs *not gear but motor
  2 * PI / 1024 /4,
 2 * PI / 1024 /4,
 2 * PI / 1024 /4,
 2 * PI / 1024 /4,
 2 * PI / 1024 /4,
 2 * PI / 1024 /4,
 2 * PI / 1024 /4,
 2 * PI / 1024 /4,
 2 * PI / 1024 /4,
 
};
static const double Ktu[DOF] = { //モータトルクから指令電圧への変換
  
  1.0/0.22/6.58, // 1/(800mA/V)/(231mNm/A)
  1.0/0.22/6.58,
  1.0/0.22/6.58,
  1.0/0.22/6.58,
  1.0/0.22/6.58,
  1.0/0.22/6.58,
  1.0/0.22/6.58,
  1.0/0.22/6.58,
  1.0/0.22/6.58,
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

int Init_flag[DOF] =  {0,0,0,0,0,0,0,0,0};
int Init_flag_total = 0;
int Init_timer = 0;
double qInit[DOF] = {0,0,0,0,0,0,0,0,0};
int magneflag[DOF] = {1,1,1,1,1,1,1,1,1};
int interlock_flag = 0;
int interlock_clear =0;
int interlocked_q[DOF] = {0,0,0,0,0,0,0,0,0};
int task_time = 0;

double qref_tmp[DOF] = {0,0,0,0,0,0,0,0,0};
double qref_wire[DOF] = {0,0,0,0,0,0,0,0,0};
double wire_length[DOF] ={650,770,890,650,770,890,650,770,890};
double Tmax = 20;
double Wiretension[DOF] = {0,0,0,0,0,0,0,0,0};
double Wiretension_mean[DOF] = {0,0,0,0,0,0,0,0,0};

double qref_testrun1[DOF] = {0.35634534,0.24313856,-0.13216739,-0.28775869,-0.47768922,-0.58844919,-0.0632473,0.25142291,0.74329144};
double qref_testrun2[DOF] = {0.52001276,-0.1049952,-0.15513929,-0.1150935,0.0014325,-0.34859202,-0.03706264,-0.01921527,0.5406009};
double qref_testrun3[DOF] = {0.55001276,-0.1049952,-0.15513929,-0.1150935,0.3614325,-0.34859202,-0.03706264,-0.01921527,0.5606009};
double qref_testrun4[DOF] = {0.19150216,0.05161616,-0.02303718,-0.15888016,-0.09013535,-0.1178806,-0.03426495,0.05768769,0.16217832};
static RTTask ctrl_task;

Controller* mctrl;

readCSV CSVreader;
std::vector<std::vector<double>> C_csv;
std::vector<std::vector<double>> L_csv;
std::vector<std::vector<double>> G_csv;
int taskcount;

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
  qref_tmp_ctrl[j]=0;
}

flag_emergency = false;

radians.resize(200);
for(int i =0;i<200;i++){
  radians[i].resize(DOF);
}
std::ifstream ifs;
ifs.open("./radians11.dat");
if(!ifs.is_open()){
  std::cout << "cant open dat file" << std::endl;
}

std::string str;
int I = 0;
int J = 0;

while(getline(ifs,str)){
    std::string tmp;
    double tmp_;
    std::istringstream stream(str);
    J=0;
    while(getline(stream,tmp,',')){

        tmp_ = stod(tmp);
        radians.at(I).at(J) = tmp_;
        J+=1;
    }
    I += 1;
}
radians_selector = 0;

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
        for(int j =0;j <DOF;j++){
          Init_flag[j] = 0;
        }
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
    
      case COMMAND_DATA:
        if( state != STATE_STANDBY ) break;
        tasks = TASKS_DATA;
        task_time =0;
        for(int j=0;j<DOF;j++){
          qref_tmp[j] =0;
        }
        state = STATE_STANDBY;
      break;


      case COMMAND_DEMO:
        if(state != STATE_STANDBY  ) break;
        tasks = TASKS_DEMO;
        state = STATE_STANDBY;

        task_time =0;
        for(int j=0;j<DOF;j++){
          qref_tmp[j] =0;
        }
      break;



      case COMMAND_INIT:
        if( state != STATE_STANDBY  ) break;
        tasks = TASKS_I;
        state = STATE_STANDBY;

      break;

      case COMMAND_CONTROL:
        if( state != STATE_STANDBY  ) break;
        for(int j=0;j<DOF;j++){
          joint[j].qref = joint[j].q;
          qref_[j] = joint[j].qref;
          qref_tmp_ctrl[j] = 0;
          qref_tmp_ctrl_[j] = 0;
        }
        tasks = TASKS_CONTROL;
        state = STATE_STANDBY;

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
        taskcount = 0;
      break;

      case COMMAND_INTERLOCK_RELEACE:
        if(state != STATE_SERVO_OFF) break;
        if(interlock_flag != 1) break;
        state = STATE_INTERLOCK_RELEACE;
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

  /*
  printf("%d : %.3f %.3f %.3f  %.3f %.3f %.3f  %.3f %.3f %.3f\n",
  count,
    joint[0].q, joint[1].q, joint[2].q,
    joint[3].q,joint[4].q, joint[5].q,
    joint[6].q, joint[7].q, joint[8].q);
  */
  /*
  printf("%d : %.3f %.3f %.3f  %.3f %.3f %.3f  %.3f %.3f %.3f\n",
  count,
    qref_tmp_ctrl[0],qref_tmp_ctrl[1],qref_tmp_ctrl[2],
    qref_tmp_ctrl[3],qref_tmp_ctrl[4],qref_tmp_ctrl[5],
    qref_tmp_ctrl[6],qref_tmp_ctrl[7],qref_tmp_ctrl[8]);
    */
   
   std::cout << radians_selector << std::endl;

   std::cout << radians[radians_selector][0] << ":::"<<radians[radians_selector][1]<<":::"<<radians[radians_selector][2]<<":::"<<radians[radians_selector][3]<<
   ":::"<<radians[radians_selector][4]<<":::"<<radians[radians_selector][5]<<":::"<<radians[radians_selector][6]<<":::"<<radians[radians_selector][7]<<":::"<<radians[radians_selector][8]<<std::endl;
  
for(int j=0;j<DOF;j++){
  if(j != DOF-1){
    joint[j].update( RPP[j]*input2[0].count[j] / Rt[j] + Q0[j] );
  }
  else if(j == 8){
    joint[j].update( RPP[j]*input2[1].count[0] / Rt[j] + Q0[j] );
  } 
}

  for(int j=0; j<DOF; j++){
    if(j< DOF-1){
    magne[j]= input2[0].v[j] ;
    }
    else{
    magne[j]=input2[1].v[0];
    }
  }
  /*
  std::cout <<"state:" <<state << std::endl;
  std::cout<< "mode:" << mode << std::endl;
  std::cout<< "tasks:" << tasks << std::endl;
  */

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
        case TASKS_DEMO:
        if(task_time < 3000){
          for(int j= 0; j<DOF ; j++){
            qref_tmp[j] += qref_testrun1[j]/3000;
          }
        }
        else if(task_time > 7000 and task_time < 11000){
          for(int j= 0; j<DOF ; j++){
            qref_tmp[j] += (qref_testrun2[j]-qref_testrun1[j])/4000;
          }
        }
        else if(task_time > 11000 and task_time < 14000){
          for(int j= 0; j<DOF ; j++){
            qref_tmp[j] += (qref_testrun3[j]-qref_testrun2[j])/3000;
          }
        }
        else if(task_time > 17000 and task_time < 20000){
          for(int j= 0; j<DOF ; j++){
            qref_tmp[j] += (qref_testrun4[j]-qref_testrun3[j])/3000;
          }
        } 

        for(int j= 0; j<DOF ; j++){
          joint[j].qref = qref_tmp[j];
        }
        task_time += 1;
        break;

        case TASKS_DATA:
          if(task_time < 3000){
            for(int j=0 ;j<DOF;j++ ){
              qref_tmp[j] += radians[radians_selector][j]/3000;
              joint[j].qref = qref_tmp[j];
            }

            task_time += 1;
          }

        break;

        case TASKS_I:
            printf("%d : [0] %d [1] %d [2] %d  [3] %d [4] %d [5] %d [6] %d [7] %d [8] %d\n",count,
                  Init_flag[0],Init_flag[1], Init_flag[2],
                    Init_flag[3], Init_flag[4], Init_flag[5],
                    Init_flag[6], Init_flag[7], Init_flag[8]);

          for(int j=0;j<DOF;j++){
            if(Init_flag[j] == 2){break;}
            if(j == 0 or j == 1 or j == 8){
              if(torque[j] < 500 *sqrt(2)){
                joint[j].qref += 0.01 / 1000;
                Init_flag[j] = 0;
              }else if(torque[j] >= 500 *sqrt(2) and torque[j] <= 700 *sqrt(2) ){
                joint[j].qref += 0;
                Init_flag[j] = 1;
              }else if(torque[j] > 700 *sqrt(2) ){
                joint[j].qref -= 0.01 / 1000;
                Init_flag[j] = 0;
              }
            }
            else{
              if(torque[j] < 500*sqrt(2) ){
                joint[j].qref += 0.01 / 1000;
                Init_flag[j] = 0;
              }else if(torque[j] >= 500 *sqrt(2) and torque[j] <= 700 *sqrt(2)){
                joint[j].qref += 0;
                Init_flag[j] = 1;
              }else if(torque[j] > 700 *sqrt(2)){
                joint[j].qref -= 0.01 / 1000;
                Init_flag[j] = 0;
              }
            }
          }
          Init_flag_total = 0;
          for(int j= 0;j<DOF;j++){
            Init_flag_total += Init_flag[j];    
          }
          if(Init_flag_total == DOF){
            Init_timer += 1;
          }else{Init_timer =0;}

          if(Init_timer >2000){
            ts01[0].set_count(0);
            ts01[1].set_count(0);
            
            for(int j= 0;j<DOF;j++){
              Init_flag[j] =2;    
              joint[j].qref = 0;
            }
          }
      

        break;

        case TASKS_CONTROL:
          for(int j=0;j<DOF;j++){
            if(click_signal[j] != 0){
              joint[j].qref = qref_[j] + qref_tmp_ctrl[j]/12.5;
              click_signal[j] = 0;
              qref_[j] = joint[j].qref;
            }
          }
        break;
      }



      
      for(int j=0;j<DOF;j++){

        joint[j].Qref.update();
        joint[j].Qdref.update();
        joint[j].feedback_control();
        joint[j].tau_ref = joint[j].tau_fb;
      }

      
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

    case STATE_INTERLOCK_RELEACE:
      interlock_clear = 0;
      for(int j=0;j<DOF;j++){
        joint[j].qref = interlocked_q[j] -0.1;

        if(torque[j] > 5000){
          interlock_clear = 1;
        }  
      }
      if(interlock_clear = 0){
        interlock_flag = 0;
      }

      for(int j=0;j<DOF;j++){
        joint[j].Qref.update();
        joint[j].Qdref.update();
        joint[j].feedback_control();
        joint[j].tau_ref = joint[j].tau_fb;
      }

    
    break;

  } //switch(state)


  // interlock----------------------------

  for(int j=0; j<DOF; j++){
    if(torque[j] > 25000 and interlock_flag == 0){
      interlock_flag = 1;
      for(int j=0;j<DOF;j++){
        interlocked_q[j]  = joint[j].q;
      }
      std::cout << "interlocked" << std::endl;
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
buf.axis_data[j][LOG_TAU      ] = torque[j] /sqrt(2);
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
    /*
    try{
      std::cout << "csv_reading" << std::endl;
      C_csv = CSVreader.read_out("./wire_ref/wire_C.csv");
      std::cout << "csv_readed" << std::endl;
      L_csv = CSVreader.read_out("./wire_ref/wire_L.csv");
      std::cout << "csv_readed2:" <<std::endl;
      //G_csv = CSVreader.read_out("wire_G.csv");
      std::string errormessage ;
      errormessage = "cant";
      throw errormessage;
    }catch(std::string errormessage){
      std::cout << errormessage << std::endl;
    }*/
    

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




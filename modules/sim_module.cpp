#include <ktl.h>
#include <libts01.h>
#include "arm.h"

#define DOF 7
#define BOARD_NUM 2

unsigned int res(int n){ return 0xffffffff >> (32 - n);}

static const int BIT[DOF]={ 21, 19, 19, 18, 18, 18, 18};

static const double RPP[DOF] = { //１パルス(1LBR)あたりの角度 abs
  2 * PI / res(BIT[0]),
  2 * PI / res(BIT[1]),
  2 * PI / res(BIT[2]),
  2 * PI / res(BIT[3]),
  2 * PI / res(BIT[4]),
  2 * PI / res(BIT[5]),
  2 * PI / res(BIT[6])
};

static const double Rt[DOF] = //関節角度とエンコーダ角度との伝達比
  { -1.0, 1.0, -1.0,  1.0, 1.2, 1.0,  2.0 };

static const double RR[DOF] = //減速比
  { -18.46154, 16.92308, 13.84615, 1.0, 1.0, 1.0, 1.0 };

static const double Ktu[DOF] = { //モータトルクから指令電圧への変換
  1.0/0.8/231, // 1/(800mA/V)/(231mNm/A)
  1.0/0.8/231,
 -1.0/0.6/217,
  10.0/15.3, 10.0/57.8, 10.0/15.3,  10.0/15.3  
};

static const double Q0[DOF] = 
  { 0.0/DEG, 150.0/DEG, 18.0/DEG, 0.0/DEG, 0.0/DEG, 0.0/DEG, 0.0};

// 0~2PI -> -PI~PI
double shift_range(double val){
  if (val < PI)
    return val;
  else
    return val - 2*PI;
}

static RTTask sim_task[BOARD_NUM];


/**********************************************************************
 *    シミュレーション用 スレッド
 *********************************************************************/
static void *thread_sim(void *p_arg){
  const int id = *((int*)p_arg);
  sim_task[id].init();

  TS01Simulator ts01(id);
  ts01.create_command_server();
  ts01.create_autosampling_server();

  const int period = 1000;
  unsigned long count=0;

  double t=0;

  int command;

  Ktl::MotionDynamics motor[DOF];//関節のダイナミクス、モータでない

  int ret;

  Ktl::Vector<6> tau;
  Ktl::Vector<6> qd;

  motor[0].setFriction( 46.0, 120.0, 100.0, 0.1 );
  motor[0].setInertia(4.0); //[gm^2]

  motor[1].setFriction( 30.0, 150.0, 150.0, 0.1 );
  motor[1].setInertia(3.0); //[gm^2]
  motor[1].setWorkingRange( 0.0, 90.0/DEG );
  
  motor[2].setFriction( 20.0, 120.0, 120.0, 0.1 );
  motor[2].setInertia(2.0); //[gm^2]
  motor[2].setWorkingRange( 0.0, 90.0/DEG );

  
  for(int i=3;i<DOF;i++){
    motor[i].setFriction( 2.0, 3.0, 3.0, 0.1 );
    motor[i].setInertia(0.5); //[gm^2]
  }

  for(int i=0;i<DOF;i++){
    motor[i].init();
  }

  
  double dt = 1e-6*period; // [sec]
  Ktl::Dynamics::setdt( dt );
  sim_task[id].start(period);	

  printf("sim%d started\n",id);
  
  while (1) {
    if( sim_task[id].receive(&command) ){
      rt_print("sim receive\n");
    }

    t = count * dt;

    for(int j=0;j<6;j++){
      //arm.q(j) = motor[j].position;
      qd[j]    = motor[j].velocity;
    }

    for(int j=0;j<DOF;j++){
      double tau_dr = (ts01.output.u[j]/Ktu[j])*RR[j];
      if( ts01.output.dout[j] == false )
	tau_dr = 0.0;

      motor[j].applyForce( tau_dr );

      motor[j].updateMotion();
    }


    for(int j=0;j<DOF;j++){
      double angle = (motor[j].position - Q0[j]) * Rt[j];//エンコーダ角度
      if( angle < 0.0 ) angle += 2*PI;
      ts01.input.count[j] = angle / RPP[j];
      //ts01.input.ssi[j] = angle / RPP[j];
      //ts01.input.ssi[j] = ts01.input.ssi[j] << 1; //MTL
      //printf("%d: %f %d\n", j,motor[j].position,ts01.input.ssi[j-3]);
    }
    ts01.input.sequence = count;
    count++;
    sim_task[id].wait();    
  }
  
  ts01.join_command_server();  
  return 0;
}



int fifo_handler(void* p){
  int* msg = (int*) p;
  /*
  for(int i=0;i<1;i++){
    sim_task[i].send( *msg );
    rt_print("sim fifo_handler send %d\n",i);
  }
  */
}

int main(int argc, char* argv[]){
  //Ktl::ManagedProcess mp("delta_sim"); //インスタンスを作る
  Ktl::FIFOHandler handler("sarm_sim");

  int id[] = {0,1};

  for(int i=0;i<BOARD_NUM;i++){
    sim_task[i].create(thread_sim, 2, &id[i]);
    rt_print("create sim %d\n",i);
  }

  handler.handle(fifo_handler);

  for(int i=0;i<BOARD_NUM;i++){
    sim_task[i].cancel();
    sim_task[i].join();
  }
  
  rt_print("simulation module removed \n");
  return 0;
}


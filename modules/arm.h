#ifndef __ARM
#define __ARM

//#define DOF 6
class Arm : public Ktl::SerialMechanism<6>{
 public:
  Arm();
  
  Ktl::Matrix<3, 3> Tw; //ジンバルオフセット角
  
};





#endif

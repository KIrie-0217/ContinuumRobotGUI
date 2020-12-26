#include<ktl.h>
#include"arm.h"

Arm::Arm()  
  : Tw(Ktl::Y, 60.0 / DEG) //ジンバルオフセット角
{
  //Tw = Ktl::Matrix<3,3>(Ktl::Y, 45.0 / DEG); //ジンバルオフセット角  

  set( Ktl::Vector<3>(0.0, 0.0, 85.0),
       Ktl::Vector<3>(0.0, 0.0, 1.0),
       Ktl::ROTARY ,
       Ktl::Vector<3>(109.0, 0.0, 15.0) );

  set( Ktl::Vector<3>(290.0, 0.0, 0.0),
       Ktl::Vector<3>(0.0, 1.0, 0.0),
       Ktl::PARALLEL,
       Ktl::Vector<3>(48.19, 0.0, -22.5)
      );
  
  set( Ktl::Vector<3>(0.0, 0.0, 270.0),
       Ktl::Vector<3>(0.0, 1.0, 0.0),
       Ktl::PARALLEL,
       Ktl::Vector<3>(120, 0.0, 120.0) );
       //Ktl::Matrix<3,3>(Ktl::Y, -30.0/DEG) *
       //Ktl::Vector<3>(80, 0.0, 0.0) );

  set( Tw*Ktl::Vector<3>(0.0, -100.0, 0.0 ),
       Tw*Ktl::Vector<3>(1.0, 0.0, 0.0),
       Ktl::ROTARY,
       Tw*Ktl::Vector<3>(120.0, 0.0, 0.0)
       );

  set( Tw*Ktl::Vector<3>(0.0, 0.0, -80.0),  
       Tw*Ktl::Vector<3>(0.0, 1.0, 0.0),
       Ktl::ROTARY,
       Tw*Ktl::Vector<3>(0.0, 100.0, 0.0));
  
  set( Tw*Ktl::Vector<3>(0.0, 0.0, 80.0), 
       Tw*Ktl::Vector<3>(0.0, 0.0, 1.0) );

  setStandardPose( Tw*Ktl::Matrix<3,3>(Ktl::Z,0.5*PI) );
  
  init();
  forward_kinematics();
}


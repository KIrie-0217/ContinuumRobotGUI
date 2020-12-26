#include "share_rt.h"
#ifndef SIM_H
#define SIM_H

#define AXIS_SDATA_SIZE 3
#define MNP_SDATA_SIZE 3
typedef struct{
  float time;
  float mnp_data[MNP_SDATA_SIZE];
  float axis_data[AXIS_SDATA_SIZE][DOF];
} SDataBuf;


#endif

#include <math.h>

#ifndef _LBDEFINITIONS_H_
#define _LBDEFINITIONS_H_


  static const int LATTICEVELOCITIES[19][3] = { { 0 ,-1 ,-1 } ,
                                                {-1 , 0 ,-1 } ,
                                                { 0 , 0 ,-1 } ,
                                                { 1 , 0 ,-1 } ,
                                                { 0 , 1 ,-1 } ,
                                                {-1 ,-1 , 0 } ,
                                                { 0 ,-1 , 0 } ,
                                                { 1 ,-1 , 0 } ,
                                                {-1 , 0 , 0 } ,
                                                { 0 , 0 , 0 } ,
                                                { 1 , 0 , 0 } ,
                                                {-1 , 1 , 0 } ,
                                                { 0 , 1 , 0 } ,
                                                { 1 , 1 , 0 } ,
                                                { 0 ,-1 , 1 } ,
                                                {-1 , 0 , 1 } ,
                                                { 0 , 0 , 1 } ,
                                                { 1 , 0 , 1 } ,
                                                { 0 , 1 , 1 }
                                              };


  static const double LATTICEWEIGHTS[19] = {  1.0 / 36.0,
                                              1.0 / 36.0,
                                              2.0 / 36.0,
                                              1.0 / 36.0,
                                              1.0 / 36.0,
                                              1.0 / 36.0,
                                              2.0 / 36.0,
                                              1.0 / 36.0,
                                              2.0 / 36.0,
                                              12.0 / 36.0,
                                              2.0 / 36.0,
                                              1.0 / 36.0,
                                              2.0 / 36.0,
                                              1.0 / 36.0,
                                              1.0 / 36.0,
                                              1.0 / 36.0,
                                              2.0 / 36.0,
                                              1.0 / 36.0,
                                              1.0 / 36.0,
                                           };


  enum Flags{ FLUID, NO_SLIP, MOVING_WALL, FREE_SLIP, INFLOW, PRESSURE_IN, OUTFLOW };


  // C_S - is the speed of sound which is equal to 1.0 / sqrt ( 3.0 ) = 0.577350269
  static const double C_S = 0.577350269;
  static const int Dimensions = 3;
  static const int Vel_DOF = 19;
  static const double InverseCS_Square = 3.0;
  static const double InverseCS_Four = 9.0;
  static const double Density_Reference = 1.0;
  static const int SELF_INDEX = 9; //for D3Q19
  static const char FILE_DELIMITER = '#';
  static const int MASTER_CPU = 0;


#endif

#pragma once

/*#ifndef OS_X
#undef SEEK_SET
#undef SEEK_END
#undef SEEK_CUR
#endif
*/

//#include <mpi.h>
/*
#include <vector>
#include <CRenderingParams.h>
#include <C2DImage.h>
#include <CGalaxyComponent.h>

using namespace std;


class CReturnValue {
 public:
 
  double* buffer;
  CReturnValue() {
    buffer = 0;
  }
  void Init(int n) {
    buffer = new double[n*n];
  }
};


class CMPI {
 public:

  int current;
  int size;
  int global_tag;
  vector<CReturnValue> returnVals;
  vector<MPI::Request> requests;
  // vector<CGalaxyComponent*>* components;
  C2DImage* image;

  vector<CRenderingParams> paramsSend;
  vector<int> tags;
  //  CMPI(int procCount, vector<CGalaxyComponent*>* components);
  CMPI(int procCount, C2DImage* img);
  // Find next free slave
  void NextFree();
  void SendToSlave(CRenderingParams porg, int proc);

  int procs;
 
  bool Test();


};
*/

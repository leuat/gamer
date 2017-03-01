#include "source/util/cmpi.h"

/*CMPI::CMPI(int procCount, C2DImage* img) {
    image = img;
    procs = procCount;
    paramsSend.resize(procs);
    requests.resize(procs);
    tags.resize(procs);

    size = image->Width;
    for (int i=0;i<procs;i++) {
        tags[i] = -10;
    }


}
void CMPI::SendToSlave(CRenderingParams porg, int current) {
    
    paramsSend[current] = porg;
    paramsSend[current].dataBuffer = (double*)&paramsSend[current];
    MPI::COMM_WORLD.Isend( paramsSend[current].dataBuffer, porg.bufferSize, MPI::DOUBLE_PRECISION , current, 0);

}

bool CMPI::Test() {
    double* data = new double[size*size];

    for (int i=1; i<procs;i++)
    {
        MPI::COMM_WORLD.Recv(data, paramsSend[i].idxCount, MPI::DOUBLE_PRECISION , i, MPI::ANY_TAG);
        for (int j=0;j<paramsSend[i].idxCount;j++) {
            image->Data[j*(int)procs + (int)paramsSend[i].idx].height += data[j];
        }
    }
    delete[] data;

    return true;
}
*/

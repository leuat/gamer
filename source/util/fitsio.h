#pragma once

#include <QString>
#include <QVector>
#include <QFile>
#include <QDebug>
#include <QDataStream>
#include "source/util/gmessages.h"
#include "source/util/buffer2d.h"
#include "source/util/fitsparam.h"



    class FitsIO {
    public:
        FitsIO();

        static void SaveFloat(QString filename, int no, Buffer2D* buffer) {
            FitsHeader header;

            header.Add("SIMPLE  = ","T");
            header.Add("BITPIX  = ","-" + QString::number(sizeof(float)*8));
            header.Add("NAXIS   = ","2");
            header.Add("NAXIS1  = ","" + QString::number(buffer->size()));
            header.Add("NAXIS2  = ","" + QString::number(buffer->size()));
            header.Add("END","");

           int bufferSize = buffer->size()*buffer->size();

            QByteArray*  ba = buffer->toQByteArray(no);
            char* d = new char[ba->size()];
            // FLIP bytes for fuck's sake
            for (int i=0;i<bufferSize;i++) {
                d[i*4+0] = ba->data()[i*4+3];
                d[i*4+1] = ba->data()[i*4+2];
                d[i*4+2] = ba->data()[i*4+1];
                d[i*4+3] = ba->data()[i*4+0];
            }
            QFile file(filename);
            if(!file.open(QIODevice::WriteOnly)) {
                qDebug() << "Could not open " << filename;
                return;
            }
            QDataStream out(&file);

            out.writeRawData((char*)header.data, header.Length);
            out.writeRawData(d, ba->size());

            file.flush();
            file.close();
            delete[] d;
            delete ba;

            GMessages::Message("Saved fits to '" +filename +"'.");

        }
/*            public void ReadFloat(Buffer2D buffer, BinaryReader reader) {
                byte[] data = new byte[buffer.buffer.Length * sizeof(float)];
                reader.Read (data, 0, data.Length);
                byte[] b = new byte[4];
                for (int i=0;i<buffer.buffer.Length;i++) {
                // Endian
                b[ 0 ] = data[4* i +3];
                b[ 1 ] = data[4* i +2];
                b[ 2 ] = data[4* i +1];
                b[ 3 ] = data[4* i +0];
                buffer.buffer[i] = BitConverter.ToSingle(b,0);
            }
            }
        public void ReadDouble(Buffer2D buffer, BinaryReader reader) {
            byte[] data = new byte[buffer.buffer.Length * sizeof(double)];
            reader.Read (data, 0, data.Length);
            byte[] b = new byte[8];
            for (int i=0;i<buffer.buffer.Length;i++) {
                // Endian
                for (int j=0;j<8;j++)
                    b[j] = data[8*i + 7-j];

                buffer.buffer[i] = (float)BitConverter.ToDouble(b,0);
            }
        }

            public void Load(string fname) {

                Buffer2D buffer;
                using (BinaryReader reader = new BinaryReader(File.Open(fname, FileMode.Open))) {
                    reader.Read(header.data, 0, header.data.Length);
                    header.ExtractParams();
                    buffer = new Buffer2D((int)header.getParamFloat("naxis1"), (int)header.getParamFloat("naxis2"));
                    int bpx = (int)header.getParamFloat("bitpix");
                    if (bpx==-32)
                        ReadFloat(buffer, reader);
                    else
                    if (bpx==-64)
                        ReadDouble(buffer, reader);
                    else
                        Debug.Log ("ERROR unsupported fits bitpix: " + bpx);

                }
            buffer.Normalize();
            colorBuffer = new ColorBuffer2D(buffer._width, buffer._height);
            for (int i=0;i<3;i++)
                colorBuffer.buffers[i].Copy(buffer);
            colorBuffer.CreateColorBuffer(1,1, Vector3.one, 1, null);


        }


    }
    */
};


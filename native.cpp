#include "native.h"
#include <QDebug>
//#include <QDebug>



void filter (std::vector<float> &v,bool strong,std::vector<double> &prev) {
    double y0,yy0,xx0,xxx0,yyy0;

    for (int i=0; i<v.size();i++){
        if (strong==true){

            y0=v[i]-1.97955*prev[0]+0.979787*prev[1]
                    +1.9998774679*prev[2]-0.99987748*prev[3];

            prev[1]=prev[0];
            prev[0]=v[i];
            prev[3]=prev[2];
            prev[2]=y0;
            xx0=y0;

            yy0= xx0-1.999911155758623*prev[4]+0.999911159706289*prev[5]
                    +1.999111557536820*prev[6]-0.999111952145663*prev[7];
            prev[5]=prev[4];
            prev[4]=xx0;
            prev[7]=prev[6];
            prev[6]=yy0;
            xxx0=yy0;

            yyy0= xxx0-1.999911155758623*prev[8]+0.999911159706289*prev[9]
                    +1.999111557536820*prev[10]-0.999111952145663*prev[11];
            prev[9]=prev[8];
            prev[8]=xxx0;
            prev[11]=prev[10];
            prev[10]=yyy0;


            }
        else {
            y0=v[i]-1.914500*prev[0]+0.918283*prev[1]
                    +1.99808*prev[2]-0.998084*prev[3];

            prev[1]=prev[0];
            prev[0]=v[i];
            prev[3]=prev[2];
            prev[2]=y0;
            xx0=y0;

            yy0= xx0-2*prev[4]+1*prev[5]
                    +1.995478*prev[6]-0.99549*prev[7];
            prev[5]=prev[4];
            prev[4]=xx0;
            prev[7]=prev[6];
            prev[6]=yy0;
            xxx0=yy0;

            yyy0= xxx0-2*prev[8]+1*prev[9]
                    +1.995478*prev[10]-0.99549*prev[11];
            prev[9]=prev[8];
            prev[8]=xxx0;
            prev[11]=prev[10];
            prev[10]=yyy0;
            }

    v[i]=yyy0;
    }
}


void findDisplacement (std::vector<float> &v,bool strong,std::vector<double> &prev) {
    double py0,pxx0,y0,xx0,yy0,xxx0,yyy0;


        if (strong==true){
          for (auto i=0; i<v.size();i++)v[i]=v[i]/1000.0/2147483648.0*2.5/21.0;
//          qDebug()<<QString::number(prev[16])<<QString::number(v[0]);
          for (auto i=0; i<v.size();i++){
            py0=v[i]-2*prev[0]+prev[1]+1.999912035*prev[2]-0.99991204*prev[3];

            prev[1]=prev[0];
            prev[0]=v[i];
            prev[3]=prev[2];
            prev[2]=py0;

            pxx0=py0;
            y0=pxx0-1.97955*prev[4]+0.979787*prev[5]
                    +1.9998774679*prev[6]-0.99987748*prev[7];

            prev[5]=prev[4];
            prev[4]=pxx0;
            prev[7]=prev[6];
            prev[6]=y0;
            if (i>0) v[i]=v[i-1]+y0; else v[i]=y0+prev[16];

               }

          prev[16]=v.back();
          for (auto i=0; i<v.size();i++){
            yy0= v[i]-2*prev[8]+prev[9]
                    +1.9991203*prev[10]-0.9991207*prev[11];
            prev[9]=prev[8];
            prev[8]=v[i];
            prev[11]=prev[10];
            prev[10]=yy0;

            xxx0=yy0;
            yyy0= xxx0-2*prev[12]+prev[13]
                    +1.9991203*prev[14]-0.9991207*prev[15];

            prev[13]=prev[12];
            prev[12]=xxx0;
            prev[15]=prev[14];
            prev[14]=yyy0;

            v[i]=yyy0;
            }


        }
        else {
            for (auto i=0; i<v.size();i++){
            y0=v[i]-1.914500*prev[0]+0.918283*prev[1]
                    +1.99808*prev[2]-0.998084*prev[3];

            prev[1]=prev[0];
            prev[0]=v[i];
            prev[3]=prev[2];
            prev[2]=y0;
            xx0=y0;

            yy0= xx0-2*prev[4]+1*prev[5]
                    +1.995478*prev[6]-0.99549*prev[7];
            prev[5]=prev[4];
            prev[4]=xx0;
            prev[7]=prev[6];
            prev[6]=yy0;
            xxx0=yy0;

            yyy0= xxx0-2*prev[8]+1*prev[9]
                    +1.995478*prev[10]-0.99549*prev[11];
            prev[9]=prev[8];
            prev[8]=xxx0;
            prev[11]=prev[10];
            prev[10]=yyy0;
            }


    }

}

void findMaxDisplacement (std::vector<float> &v,bool strong,std::vector<double> &prev,
                        float &maxDis, float &minDis) {
    double py0,pxx0,y0,xx0,yy0,xxx0,yyy0;


        if (strong==true){
            //Displacenmet in mm
          for (auto i=0; i<v.size();i++)v[i]=v[i]/2147483648.0*2.5/21.0;
//          qDebug()<<QString::number(prev[16])<<QString::number(v[0]);
          for (auto i=0; i<v.size();i++){
            py0=v[i]-2*prev[0]+prev[1]+1.999912035*prev[2]-0.99991204*prev[3];

            prev[1]=prev[0];
            prev[0]=v[i];
            prev[3]=prev[2];
            prev[2]=py0;

            pxx0=py0;
            y0=pxx0-1.914500*prev[4]+0.918283*prev[5]
                    +1.99808*prev[6]-0.998084*prev[7];

            prev[5]=prev[4];
            prev[4]=pxx0;
            prev[7]=prev[6];
            prev[6]=y0;
            if (i>0) v[i]=v[i-1]+y0; else v[i]=y0+prev[16];

               }

          prev[16]=v.back();
          for (auto i=0; i<v.size();i++){
            yy0= v[i]-2*prev[8]+prev[9]
                    +1.9991203*prev[10]-0.9991207*prev[11];
            prev[9]=prev[8];
            prev[8]=v[i];
            prev[11]=prev[10];
            prev[10]=yy0;

            xxx0=yy0;
            yyy0= xxx0-2*prev[12]+prev[13]
                    +1.9991203*prev[14]-0.9991207*prev[15];

            prev[13]=prev[12];
            prev[12]=xxx0;
            prev[15]=prev[14];
            prev[14]=yyy0;

            v[i]=yyy0;
            if (yyy0>maxDis) maxDis=(float)yyy0;
            if (yyy0<minDis) minDis=(float)yyy0;
            }


        }
        else {
            for (auto i=0; i<v.size();i++){
            y0=v[i]-1.914500*prev[0]+0.918283*prev[1]
                    +1.99808*prev[2]-0.998084*prev[3];

            prev[1]=prev[0];
            prev[0]=v[i];
            prev[3]=prev[2];
            prev[2]=y0;
            xx0=y0;

            yy0= xx0-2*prev[4]+1*prev[5]
                    +1.995478*prev[6]-0.99549*prev[7];
            prev[5]=prev[4];
            prev[4]=xx0;
            prev[7]=prev[6];
            prev[6]=yy0;
            xxx0=yy0;

            yyy0= xxx0-2*prev[8]+1*prev[9]
                    +1.995478*prev[10]-0.99549*prev[11];
            prev[9]=prev[8];
            prev[8]=xxx0;
            prev[11]=prev[10];
            prev[10]=yyy0;
            }


    }

}
void applyfilter (std::vector<std::vector<float>> &v,bool strong, std::vector<std::vector<double>> &prev) {
     std::vector<std::thread> threads(3);
//    for (int i=0; i<v.size();i++)
//    {
//        filter(v[i],strong,prev[i]);
//    }
    for (auto i=0;i<v.size();i++)
    {
            threads[i]= std::thread (filter,std::ref(v[i]),strong,std::ref(prev[i]));
    }
    for (auto i=0;i<v.size();i++)
    {
            threads[i].join();
    }

}


void applyDisplacement (std::vector<std::vector<float>> &v,bool strong, std::vector<std::vector<double>> &prev) {
     std::vector<std::thread> threads(3);

    for (auto i=0;i<v.size();i++)
    {
            threads[i]= std::thread (findDisplacement,std::ref(v[i]),strong,std::ref(prev[i]));
    }
    for (auto i=0;i<v.size();i++)
    {
            threads[i].join();
    }

}



void applyMaxDisplacement (std::vector<std::vector<float>> &v,bool strong,
                        std::vector<std::vector<double>> &prev,
                        std::vector<float> &maxDis,
                        std::vector<float> &minDis) {
     std::vector<std::thread> threads(3);

    for (auto i=0;i<v.size();i++)
    {
            threads[i]= std::thread (findMaxDisplacement,
                                     std::ref(v[i]),strong,std::ref(prev[i]),
                                     std::ref(maxDis[i]),std::ref(minDis[i]));
    }
    for (auto i=0;i<v.size();i++)
    {
            threads[i].join();
    }

}


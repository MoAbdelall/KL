#include <iostream>
#include "header4.h"
using namespace std;

int main()
{
    ifstream srcfile;
    srcfile.open("pinOriented.txt");
    vector <node> nodes;
    readNet(nodes,srcfile);
    vector<vector<int>>conMat;
    buildMat(conMat,nodes);
    //dispMat(conMat,nodes);
    vector <node> half1,half2;
    split(nodes,half1,half2,conMat);
    vector <node> half1OG=half1;vector <node> half2OG=half2;
    //dispMat(conMat,nodes);
    vector <int> maxGainIDs;
    int maxGain;
    vector <Gm> passGms;
    int GmMax;
    do{
        while(!allFixed(half1,half2)){
            //cout<<"NOT FIXED";
            calcCost(half1,half2,conMat);
            //cout<<endl;
            calcGain(half1,half2,maxGain,maxGainIDs,conMat);
            pushGain(maxGain,maxGainIDs,passGms);
            swapNfix(maxGainIDs,half1,half2);
        }
        //cout<<"OUT"<<endl;
        int maxPassGmID=findMaxGmID(passGms);
        GmMax=passGms[maxPassGmID].maxGain;
        if (GmMax>0){
            //function
            finishPass(half1OG,half2OG,passGms,maxPassGmID);
        }
        half1=half1OG;
        half2=half2OG;
        passGms.clear();
    }while(GmMax>0);
    cout<<"done!"<<endl;
    cout<<"Group1: ";
    for(int i=0;i<half1.size();i++){
        cout<<half1[i].name<<" ";
    }
    cout<<"\nGroup2: ";
    for(int i=0;i<half2.size();i++){
        cout<<half2[i].name<<" ";
    }
    return 0;
}

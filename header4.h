#ifndef HEADER3_H_INCLUDED
#define HEADER3_H_INCLUDED
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <vector>
using namespace std;
struct node {
    string name;
    vector <string> innet;
    vector <string> outnet;
    int ID;
    bool fixed=0;
    int cost;
};
struct Gm{
    int maxGain;
    vector <int> maxGainIDs;
};
void readNet(vector<node> &nodes,ifstream &srcfile){
    string line;
    getline(srcfile,line);
    int i=0;
    while (srcfile){
        node tempNode;
        tempNode.name=line.substr(line.find("(")+1,line.find(":")-1-line.find("("));
        //cout<<line.substr(line.find("(")+1,line.find(":")-1)<<" ";
        ////cout<<line.find(",");
        ////cout<<line.find(":");
        if(line.find(",")==string::npos){
            //cout<<line.substr(line.find(" ")+1,line.find(")")-1-line.find(" "))<<endl;
            tempNode.outnet.push_back(line.substr(line.find(" ")+1,line.find(")")-1-line.find(" ")));
            ////cout<<tempNode.outnet[0];
        }else{
            line.erase(0,line.find(" ")+1);
            ////cout<<"\n"<<line<<endl;
            string nodeType=line.substr(0,2);
            while (nodeType.compare("IN")==0){
                tempNode.innet.push_back(line.substr(line.find(" ")+1,line.find(",")-1-line.find(" ")));
                //cout<<line.substr(line.find(" ")+1,line.find(",")-1-line.find(" "))<<" ";
                line.erase(0,line.find(",")+2);
                ////cout<<line<<endl;
                nodeType=line.substr(0,2);
                ////cout<<nodeType;
            }
            ////cout<<"..."<<line<<endl;
            tempNode.outnet.push_back(line.substr(line.find(" ")+1,line.find(")")-1-line.find(" ")));
            //cout<<line.substr(line.find(" ")+1,line.find(")")-1-line.find(" "))<<endl;

        }
        tempNode.ID=i;
        tempNode.fixed=0;
        nodes.push_back(tempNode);
        getline(srcfile,line);
        i++;
    }
//    return 0;
}
void buildMat(vector<vector<int>>&conMat,vector<node> &nodes){
        for (int i = 0; i <nodes.size(); i++){
            vector<int> tempVec;
            for (int j = 0; j <nodes.size(); j++){
                    tempVec.push_back(0);
            }
            conMat.push_back(tempVec);
        }


        for (int i = 0; i <nodes.size(); i++){
            for (int j = 0; j <nodes[i].innet.size(); j++){
                if(i!=nodes.size()-1){
                    for (int k=i+1;k<nodes.size();k++){
                        for (int l=0; l <nodes[k].innet.size(); l++){
                            if(nodes[i].innet[j].compare(nodes[k].innet[l])==0){
                                //cout<<"found in with in "<<nodes[i].name<<" and "<<nodes[k].name<<endl;
                                conMat[i][k]++;
                                conMat[k][i]++;
                            }
                        }
                    }
                }
                for (int k=0;k<nodes.size();k++){
                    if(nodes[i].innet[j].compare(nodes[k].outnet[0])==0){
                        //cout<<"found in with out "<<nodes[i].name<<" and "<<nodes[k].name<<endl;
                        conMat[i][k]++;
                        conMat[k][i]++;
                    }
                }
            }
            for (int k=i+1;k<nodes.size();k++){
                if(nodes[i].outnet[0].compare(nodes[k].outnet[0])==0){
                    //cout<<"found out with out "<<nodes[i].name<<" and "<<nodes[k].name<<endl;
                    conMat[i][k]++;
                    conMat[k][i]++;
                }
            }
        }
//return 0;
}
void dispMat(vector<vector<int>>&conMat,vector<node> &nodes){
    cout<<"  ";
    for (int i = 0; i < conMat.size(); i++){
        cout<<nodes[i].name<<" ";
    }
    cout<<endl;
    for (int i = 0; i < conMat.size(); i++)
        {
            cout<<nodes[i].name<< " ";
            for (int j = 0; j < conMat[i].size(); j++)
            {
                cout << conMat[i][j] << " ";
            }
            cout << endl;
        }
}
void split(vector <node> &nodes,vector <node> &half1,vector <node> &half2,vector<vector<int>>&conMat){
    //cout<<"halfs: ";
    for (int i=0;i<nodes.size();i++){
        if (i<(nodes.size()+1)/2){
            half1.push_back(nodes[i]);
            //cout<<half1[i].name<<" ";
        }else{
            //cout<<"|";
            half2.push_back(nodes[i]);
            //cout<<nodes[i].name<<" ";
        }
    }
    if (nodes.size()%2==1){
        node tempNode;
        tempNode.name="Fake";tempNode.fixed=0;tempNode.ID=nodes.size();
        half2.push_back(tempNode);
        for (int i = 0; i <nodes.size(); i++){
            conMat[i].push_back(0);
        }
        vector<int> tempVec;
        for (int j = 0; j <nodes.size()+1; j++){
                tempVec.push_back(0);
        }
        conMat.push_back(tempVec);
    }
}
void calcCost(vector <node> &half1,vector <node> &half2,vector<vector<int>>&conMat){
    int tempCost=0;
    for (int i=0;i<half1.size();i++){
        for (int j=0;j<half2.size();j++){
            tempCost+=conMat[half1[i].ID][half2[j].ID];
        }
        for(int j=0;j<half1.size();j++){
            tempCost-=conMat[half1[i].ID][half1[j].ID];
        }
        half1[i].cost=tempCost;
        //cout<< tempCost<<" ";
        tempCost=0;
    }
    tempCost=0;
    for (int i=0;i<half2.size();i++){
        for (int j=0;j<half1.size();j++){
            tempCost+=conMat[half2[i].ID][half1[j].ID];
        }
        for(int j=0;j<half2.size();j++){
            tempCost-=conMat[half2[i].ID][half2[j].ID];
        }
        half2[i].cost=tempCost;
        //cout<< tempCost<<" ";
        tempCost=0;

    }
}
void calcGain(vector <node> &half1,vector <node> &half2,int &maxGain, vector <int> &maxGainIDs,vector<vector<int>>&conMat){
    int gain;
    bool flag=0;
    //IDs are their places in halfs
    //maxGainIDs.push_back(0);maxGainIDs.push_back(0);
    for (int i=0;i<half1.size();i++){
        for (int j=0;j<half2.size();j++){
            //cout<<"checking:"<<half1[i].name<<"&"<<half2[j].name<<":"<<endl;
            if(!half1[i].fixed && !half2[j].fixed){
                //cout<<"not fixed"<<endl;
                gain=half1[i].cost+half2[j].cost-2*conMat[half1[i].ID][half2[j].ID];
                if(flag==0){
                    maxGainIDs.clear();
                    maxGainIDs.push_back(i);maxGainIDs.push_back(j);
                    maxGain=gain;
                    flag=1;
                }
                if(gain>maxGain){
                    maxGain=gain;
                    maxGainIDs[0]=i;
                    maxGainIDs[1]=j;
                }
            }
        }
    }
    //cout<<maxGain<<" "<<maxGainIDs[0]<<","<<maxGainIDs[1]<<endl;
}
void swapNfix( vector <int> & maxGainIDs,vector <node> &half1,vector <node> &half2){
    node tempNode=half1[maxGainIDs[0]];
    half1[maxGainIDs[0]]=half2[maxGainIDs[1]];
    half2[maxGainIDs[1]]=tempNode;
    half1[maxGainIDs[0]].fixed=1;
    half2[maxGainIDs[1]].fixed=1;
    //cout<<"fixed:"<<maxGainIDs[0]<<","<<maxGainIDs[1]<<endl;
}
bool allFixed(vector <node> half1,vector <node> half2){
    bool flag=1;
    for (int i=0;i<half1.size();i++){
        if (!half1[i].fixed){
            flag=0;
        }
    }
    for (int i=0;i<half2.size();i++){
        if (!half2[i].fixed){
            flag=0;
        }
    }
    return flag;
}
void pushGain(int &maxGain, vector <int> &maxGainIDs, vector <Gm> &passGms){
    Gm tempGm;
    if(passGms.empty()){
        tempGm.maxGain=maxGain;
        tempGm.maxGainIDs=maxGainIDs;
        passGms.push_back(tempGm);
    }else{
        tempGm.maxGain=maxGain+passGms.back().maxGain;
        //cout<<" Gm="<<tempGm.maxGain<<endl;
        tempGm.maxGainIDs=maxGainIDs;
        passGms.push_back(tempGm);
    }
}
int findMaxGmID(vector <Gm> &passGms){
    int maxGm=passGms[0].maxGain;
    int ID=0;
    for (int i=0;i<passGms.size();i++){
        if (passGms[i].maxGain>maxGm){
            maxGm=passGms[i].maxGain;
            ID=i;
        }
    }
    return ID;
}
void finishPass(vector <node> &half1OG,vector <node> &half2OG, vector <Gm> &passGms,int maxPassGmID){
    node tempNode;
    for(int i=0;i<=maxPassGmID;i++){
        tempNode=half1OG[passGms[i].maxGainIDs[0]];
        half1OG[passGms[i].maxGainIDs[0]]=half2OG[passGms[i].maxGainIDs[1]];
        half2OG[passGms[i].maxGainIDs[1]]=tempNode;
    }
}
#endif // HEADER3_H_INCLUDED

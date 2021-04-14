#include<stdlib.h>
#include<stdio.h>
#include<iostream>
using namespace std;

int main(){
    printf("\033c");
    //===========board and cell size===============
    int bHeight,bWidth,cHeight,cWidth;
    bHeight=8;
    bWidth=8;
    cHeight=2;
    cWidth=3;
    //==========printing board======================
    cout<<"chess tapi dibikin sama faza v0 anjay mabar\n";
    char wBox='#',bBox=' ';
    for(int h=0;h<bHeight;h++){
        for(int k=0;k<cHeight;k++){
            for(int w=0;w<bWidth;w++){
                if((h+w)%2)for(int l=0;l<cWidth;l++)cout<<wBox;
                else for(int l=0;l<cWidth;l++)cout<<bBox;
            }
            cout<<endl;
        }
    }
    string a;
    cin>>a;
    printf("\033c");
}
//g++ consolePreview.cpp -o conPre
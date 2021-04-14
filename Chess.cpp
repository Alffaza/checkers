#include <iostream>
using namespace std;

//=========================le board===============================================
class playingBoard{
    public:
    void init(){
        char *newCords=(char*)malloc(totArea*sizeof(char));  
        cords=newCords;
        bool turnWhite=0;
        for(int i=0;i<totArea;i++){
            if(!(i%cWidth)&& (i%(cHeight*width)))turnWhite=!turnWhite;
            *(cords+i)=(turnWhite)?wBox:bBox;
        }
    }
    playingBoard(int h,int w,int ch,int cw){
        height=h;
        width=w;
        cHeight=ch;
        cWidth=cw;
        totArea=h*w*ch*cw;
        init();
    }
    void show(){
        for(int i=0;i<totArea;i++){
            if(!(i%(cWidth*width)))cout<<endl;
            cout<<*(cords+i);
        }
        cout<<endl;
    }
    private:
    int height,width,cHeight,cWidth,totArea;
    char *cords;
    char wBox='#',bBox=' ';
};
//=====================pieces for chess hehe====================================

class chessPiece{
    public:
    void putPiece(int x,int y){

    }
    protected:
    int xPos,yPos;

};
class pawn:public chessPiece{
    public:
    pawn(int x,int y,playingBoard board){
        xPos=x;
        yPos=y;
    }
    private:
    int xMove,yMove,moveType=1;
};
class horse:public chessPiece{
    
};
class rook:public chessPiece{
    
};
class bishop:public chessPiece{
    
};
class queen:public chessPiece{
    
};
class king:public chessPiece{
    
};

//=======================main func=========================================
int main(){
    playingBoard bord(8,8,2,3); //board height, board width, cell height, cell width
    bord.show();
}
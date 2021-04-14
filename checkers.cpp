#include <iostream>
using namespace std;
#define right 1;
#define left 0;
class checker;
//=========================le board===============================================
class playingBoard{
    public:
        void init(){
            char *newCords=(char*)malloc(totArea*sizeof(char));  
            cords=newCords;
            bool turnWhite=0;
            for(int i=0;i<totArea;i++){
                if(!(i%cWidth)&& (i%(cHeight*width*cWidth)))turnWhite=!turnWhite;
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
        void setPiece(int x,int y, char sym){
            *(lookAt(x,y))=sym;
        }
        void remPiece(int x,int y){
            *(lookAt(x,y))=((x+y)%2)?wBox:bBox;
        }
        bool isOccupied(int x,int y){
            if(*(lookAt(x,y))==bBox||*(lookAt(x,y))==wBox)return 0;
            return 1;
        }
    private:
        char *lookAt(int x,int y){
            int id=(cWidth/2)+(cWidth)*x+(cWidth*width*cHeight)*y+(cWidth*width)*(cHeight/2);
            return (cords+id);
        }
        int height,width,cHeight,cWidth,totArea;
        char *cords;
        checker *chekCords;
        char wBox='#',bBox=' ';
};
//=====================pieces for checkers hehe====================================
class checker{
    public:
        checker(char t,int x,int y, playingBoard *board){
            posX=x;
            posY=y;
            team=t;
            onBoard=board;
            onBoard->setPiece(posX,posY,team);
        }
        void move(bool dir){
            onBoard->remPiece(posX,posY);
            onBoard->setPiece(posX,posY,team);
        }
    private:
        playingBoard *onBoard;
        int posX,posY;
        char team;
};


//=======================main func=========================================
int main(){
    playingBoard bord(8,8,1,3); //board height, board width, cell height, cell width
    checker a('O',0,0,&bord);
    bord.show();
}
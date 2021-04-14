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
        void setPiece(int x,int y,char type, bool isWhite){
            if(isWhite)type-=32;
            int id=1+(cWidth)*x+(cWidth*width*cHeight)*y;
            *(cords+id)=type;
            *(cords+id+(cWidth*width))='I';
        }
    private:
        int height,width,cHeight,cWidth,totArea;
        char *cords;
        char wBox='#',bBox=' ';
};
//=====================pieces for chess hehe====================================
class chessPiece{
    public:
        void place(int x,int y,playingBoard *board){
            xPos=x;
            yPos=y;
            atBoard=board;
            board->setPiece(x,y,pieceType,isWhite);
        }
    protected:
        char pieceType;
        bool isWhite;
    private:
        int xPos,yPos;
        playingBoard *atBoard;
};
class pawn:public chessPiece{
    public:
        pawn(int x,int y,bool w, playingBoard *board){
            pieceType='p';
            isWhite=w;
            place(x,y,board);
        }
};
class horse:public chessPiece{
    public:
        horse(int x,int y,bool w, playingBoard *board){
            pieceType='h';
            isWhite=w;
            place(x,y,board);
        }
};
class rook:public chessPiece{
    public:
        rook(int x,int y,bool w, playingBoard *board){
            pieceType='r';
            isWhite=w;
            place(x,y,board);
        }
};
class bishop:public chessPiece{
    public:
        bishop(int x,int y,bool w, playingBoard *board){
            pieceType='b';
            isWhite=w;
            place(x,y,board);
        }
};
class queen:public chessPiece{
    public:
        queen(int x,int y,bool w, playingBoard *board){
            pieceType='q';
            isWhite=w;
            place(x,y,board);
        }
};
class king:public chessPiece{
    public:
        king(int x,int y,bool w, playingBoard *board){
            pieceType='k';
            isWhite=w;
            place(x,y,board);
        }
};

//=======================main func=========================================
int main(){
    playingBoard bord(8,8,2,3); //board height, board width, cell height, cell width

    bord.show();
}
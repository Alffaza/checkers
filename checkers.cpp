#include <iostream>
using namespace std;
class playingBoard;
class checkersBoard;
//===================== pieces for checkers hehe====================================
class checker{
    public:
        checker(char t,int x,int y, checkersBoard *board);
        void make(char t,int x,int y, checkersBoard *board);
        int move(char T);
        void moving(int x,int y);
        char team;
        checker *ad;
    private:
        checkersBoard *onBoard;
        bool isQueen=0;
        int posX,posY;
};
//=========================le board ===============================================
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
        void show(){
            cout<<"\033c";
            for(int i=0;i<cWidth/2;i++)cout<<" ";
            for(int i=0;i<width;i++){
                cout<<i+1;
                for(int j=0;j<cWidth-1;j++)cout<<" ";
            }
            cout<<endl;
            for(int i=0;i<width*cWidth;i++)cout<<"-";
            cout<<"+";
            for(int i=0;i<totArea;i++){
                if(i!=0&&!(i%(cWidth*width)))cout<<"|"<<char('A'+i/(cWidth*width)-1);
                if(!(i%(cWidth*width)))cout<<endl;
                cout<<*(cords+i);
            }
            cout<<"|"<<char('A'+height-1)<<endl;
        }
        void place(int x,int y, char sym){
            *(lookAt(x,y))=sym;
        }
        void rem(int x,int y){
            *(lookAt(x,y))=((x+y)%2)?wBox:bBox;
        }
        bool isOccupied(int x,int y){
            if(x<0||x>=width||y<0||y>=height)return 1;
            return (*(lookAt(x,y))!=bBox&&*(lookAt(x,y))!=wBox);
        }
        char *lookAt(int x,int y){
            int id=(cWidth/2)+(cWidth)*x+(cWidth*width*cHeight)*y+(cWidth*width)*(cHeight/2);
            return (cords+id);
        }
        int height,width,cHeight,cWidth,totArea;
    protected:
        char *cords;
        char wBox='#',bBox=' ';
};
class checkersBoard:public playingBoard{
    public:
        void checkerInit(){
            checker **newCheckers=(checker**)malloc(height*width*sizeof(checker));
            checkerCords=newCheckers;
            init();
        }
        void setPiece(int x,int y,checker *piece){
            if(!isOccupied(x,y)){
                place(x,y,piece->team);
                int id=y*width+x;
                *(checkerCords+id)=piece;
            }
        }
        void remPiece(int x,int y){
            if(x>=0 && x<width && y>= 0 && y<height && isOccupied(x,y)){
                rem(x,y);
                *(checkerCords+x+y*width)=NULL;
            }
        }
        checkersBoard(int h,int w,int ch,int cw){
            height=h;
            width=w;
            cHeight=ch;
            cWidth=cw;
            totArea=h*w*ch*cw;
            checkerInit();
        }
        void doMove(int x,int y){
            if(!isOccupied(x,y)) cout<<"no piece found there!\n";
            else{
                pieceAt(x,y)->move(pieceAt(x,y)->team);
            }
        }
        checker *pieceAt(int x, int y){
            int id=y*height+x;
            return *(checkerCords+id);
        }
        void cleanMoveMarkers(int x,int y){
            if(isOccupied(x+1,y+1)&&*lookAt(x+1,y+1)=='O')rem(x+1,y+1);
            if(isOccupied(x+1,y-1)&&*lookAt(x+1,y-1)=='O')rem(x+1,y-1);
            if(isOccupied(x-1,y+1)&&*lookAt(x-1,y+1)=='O')rem(x-1,y+1);
            if(isOccupied(x-1,y-1)&&*lookAt(x-1,y-1)=='O')rem(x-1,y-1);
        }
        void playerTurn(char t){
            int inpx;
            char inpy;
            do{
                show();
                cout<<"which piece do you want to move?[number][uppercase letter]\n";
                cin>>inpx>>inpy;
                inpx--;
                inpy-='A';
            }while(*lookAt(inpx,inpy)==bBox||*lookAt(inpx,inpy)==wBox||!(pieceAt(inpx,inpy)->move(t)));
        }
    private:
        checker **checkerCords;
};
//==================== checker method ============================
checker::checker(char t,int x,int y, checkersBoard *board){
    posX=x;
    posY=y;
    team=t;
    onBoard=board;
    onBoard->setPiece(posX,posY,ad);
}
void checker::make(char t,int x,int y, checkersBoard *board){
    posX=x;
    posY=y;
    team=t;
    onBoard=board;
    onBoard->setPiece(posX,posY,ad);
}
int checker::move(char t){
    int inpx;
    char inpy;
    if(team!=t){
        cout<<"it is not "<<team<<"'s turn\n";
    }
    int futY= posY + ((team=='b')? 1:-1);
    int numOfChoices=0;
    if(!onBoard->isOccupied(posX-1,futY)){
        onBoard->place(posX-1,futY,'O');
        numOfChoices++;
    }
    if(!onBoard->isOccupied(posX+1,futY)){
        onBoard->place(posX+1,futY,'O');
        numOfChoices++;
    }
    if(numOfChoices){
        onBoard->show();
        cout<<"choose where to move[number][uppercase letter]\n";
        cin>>inpx>>inpy;
        inpx--;
        inpy-='A';
        if(inpx<0||inpx>=onBoard->width||inpy<0||inpy>=onBoard->height||*(onBoard->lookAt(inpx,inpy))!='O'){
            cout<<"cant move there\n";
            return 0;
        }
    }
    else {
        cout<<"no moves for that piece\n";
        return 0;
    }
    moving(inpx,inpy);
    return 1;
}
void checker::moving(int x,int y){
    onBoard->rem(x,y);
    onBoard->setPiece(x,y,ad);
    onBoard->remPiece(posX,posY);
    onBoard->cleanMoveMarkers(posX,posY);
    posX=x;
    posY=y;
}

//=======================main func=========================================
int main(){
    checkersBoard bord(8,8,1,3); //board height, board width, cell height, cell width
    checker* newone=(checker*)malloc(sizeof(checker));
    newone->ad=newone;
    newone->make('b',0,0,&bord);
    bord.playerTurn('b');
    bord.show();
}
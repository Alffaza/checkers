#include <iostream>
using namespace std;
class playingBoard;
class checkersBoard;
//===================== pieces for checkers =========================================
class checker{
    public:
        checker(char t,int x,int y, checkersBoard *board);
        void make(char t,int x,int y, checkersBoard *board);
        int move(char T);
        void moving(int x,int y);
        int canEat(char t);
        int eat();
        char team;
        checker *ad;
        int posX,posY;
    private:
        checkersBoard *onBoard;
        bool isQueen=0;
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
        void show(int clrScr){
            if(clrScr)cout<<"\033c";
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
        void makeApiece(char t,int x,int y){
            checker* newPiece=(checker*)malloc(sizeof(checker));
            newPiece->ad=newPiece;
            newPiece->make(t,x,y,selfAdress);
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
        checkersBoard(int h,int w,int ch,int cw,checkersBoard *ad){
            selfAdress=ad;
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
            bool f=1;
            do{
                show(f);
                f=0;
                cout<<"which piece do you want to move?[number][uppercase letter]\n";
                cin>>inpx>>inpy;
                inpx--;
                inpy-='A';
            }while(*lookAt(inpx,inpy)==bBox||*lookAt(inpx,inpy)==wBox||!(pieceAt(inpx,inpy)->move(t)));
        }
        void chainEat(int x,int y,char t){
            if(canEatAt(x,y,t)){
                int moveY=((t=='b')? 1:-1);
                int numOfEats=0;
                if(!canEatAt(x+2,y+2*moveY,t)&&!canEatAt(x-2,y+2*moveY,t)){
                    if(pieceAt(x+1,y+moveY)&&pieceAt(x+1,y+moveY)->team!=x)place(x+2,y+2*moveY,'O');
                    if(pieceAt(x-1,y+moveY)&&pieceAt(x-1,y+moveY)->team!=x)place(x-2,y+2*moveY,'O');
                }
                else if(canEatAt(x+2,y+2*moveY,t)==canEatAt(x-2,y+2*moveY,t)){
                    place(x+2,y+2*moveY,'O');
                    place(x-2,y+2*moveY,'O');
                    chainEat(x+2,y+2*moveY,t);
                    chainEat(x-2,y+2*moveY,t);
                }
                else if(canEatAt(x+2,y+2*moveY,t)>canEatAt(x-2,y+2*moveY,t)){
                    place(x+2,y+2*moveY,'O');
                    chainEat(x+2,y+2*moveY,t);
                }
                else if(canEatAt(x+2,y+2*moveY,t)<canEatAt(x-2,y+2*moveY,t)){
                    place(x-2,y+2*moveY,'O');
                    chainEat(x-2,y+2*moveY,'O');
                }
            }
        }
        int canEatAt(int x,int y, char t){
            int moveY=((t=='b')? 1:-1);
            int futY= y+moveY;
            int numOfEats=0;
            if(pieceAt(x+1,futY)&& pieceAt(x+1,futY)->team!=x &&!isOccupied(x+2,futY+moveY)){
                numOfEats= 1+canEatAt(x+2,futY+moveY,t);  
            }
            if(pieceAt(x-1,futY)&& pieceAt(x-1,futY)->team!=x &&!isOccupied(x-2,futY+moveY)){
                numOfEats= max(numOfEats,1+canEatAt(x-2,futY+moveY,t));
            }
            return numOfEats;
        }
    private:
        checker **checkerCords;
        checkersBoard *selfAdress;
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
        return 0;
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
        onBoard->show(0);
        cout<<"choose where to move[number][uppercase letter]\n";
        cin>>inpx>>inpy;
        inpx--;
        inpy-='A';
        if((inpx<0||inpx>=onBoard->width||inpy<0||inpy>=onBoard->height)||*(onBoard->lookAt(inpx,inpy))!='O'){
            onBoard->cleanMoveMarkers(posX,posY);
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
int checker::canEat(char t){
    if(t!=team)return 0;
    int ret=0;
    int moveY=((team=='b')? 1:-1);
    int futY= posY+moveY;
    int tempX=posX;
    int tempY=posY;
    int numOfChoices=0;
    if(onBoard->pieceAt(posX+1,futY)&& onBoard->pieceAt(posX+1,futY)->team!=team &&!onBoard->isOccupied(posX+2,futY+moveY)){
        numOfChoices++;  
    }
    if(onBoard->pieceAt(posX-1,futY)&& onBoard->pieceAt(posX-1,futY)->team!=team &&!onBoard->isOccupied(posX-2,futY+moveY)){
        onBoard->place(posX-2,futY+moveY,'O');  
    }
}
int checker::eat(){

}

//======================= main func =========================================
int main(){
    checkersBoard bord(8,8,1,3,&bord); //board height, board width, cell height, cell width
    cout<<"tes\n";
    bord.makeApiece('b',0,0);
    bord.makeApiece('w',1,1);
    bord.makeApiece('w',3,3);
    bord.makeApiece('w',5,5);
    bord.chainEat(0,0,'b');
    cout<<endl;
    bord.show(0);
}
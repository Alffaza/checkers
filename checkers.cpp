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
        int forcedEat(int x,int y);
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
                if(i!=0&&!(i%(cWidth*width)))cout<<"|";
                if(i!=0&&!(i%(cWidth*width*cHeight)))cout<<char('A'+i/(cWidth*width*cHeight)-1);
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
            if(x<0||x>=width||y<0||y>=height)return " ";
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
            if(isOccupied(x+1,y+1)&&*lookAt(x+1,y+1)==moveMarker)rem(x+1,y+1);
            if(isOccupied(x+1,y-1)&&*lookAt(x+1,y-1)==moveMarker)rem(x+1,y-1);
            if(isOccupied(x-1,y+1)&&*lookAt(x-1,y+1)==moveMarker)rem(x-1,y+1);
            if(isOccupied(x-1,y-1)&&*lookAt(x-1,y-1)==moveMarker)rem(x-1,y-1);
        }

        void cleanAllMarkers(){
            for(int i=0;i<height;i++){
                for(int j=0;j<width;j++)
                    if(isOccupied(j,i)&&(*lookAt(j,i)==moveMarker||*lookAt(j,i)==eatMarker))rem(j,i);
            }
        }

        void playerTurn(char t){
            int inpx;
            char inpy;
            bool f=1,multiChoice;
            int maxEating = checkMandatoryEat(t);

            checker *maxPiece=NULL;
            if(!maxEating)
                do{
                    show(f);
                    f=0;
                    cout<<"which piece do you want to move "<<t<<"?[number][uppercase letter]\n";
                    cin>>inpx>>inpy;
                    inpx--;
                    inpy-='A';
                }
                while(*lookAt(inpx,inpy)==bBox||*lookAt(inpx,inpy)==wBox||!(pieceAt(inpx,inpy)->move(t)));
            else{
                for(int i=0;i<height;i++){
                    for(int j=0;j<width;j++){
                        if(pieceAt(j,i) && pieceAt(j,i)->team==t && canEatAt(j,i,t)==maxEating){
                            if(!maxPiece)maxPiece=pieceAt(j,i);
                            else{
                                chainEat(j,i,t);
                                multiChoice=1;
                            }
                        }
                    }
                }
                if(!multiChoice)while(eatChain(maxPiece));
                else{
                    chainEat(maxPiece->posX,maxPiece->posY,maxPiece->team);
                    int inpx;
                    char inpy;
                    do{
                        show(1);
                        cout<<"multiple forced eats, "<<t<<" choose piece!\n";
                        cin>>inpx>>inpy;
                        inpx--;
                        inpy-='A';
                    } while(canEatAt(inpx,inpy,t)==maxEating);
                    maxPiece=pieceAt(inpx,inpy);
                    while(eatChain(maxPiece));
                }   
            }
        }

        int checkMandatoryEat(char t){
            int ret=0;
            for(int i=0;i<height;i++){
                for(int j=0;j<width;j++){
                    if(pieceAt(j,i) && pieceAt(j,i)->team==t){
                        ret=max(ret,canEatAt(j,i,t));
                        if(canEatAt(j,i,t))cout<<j<<" "<<i<<endl;
                    }
                }
            }
            return ret;
        }

        bool eatChain(checker *piece){
            int x=piece->posX,y=piece->posY;
            char t=piece->team;
            cleanAllMarkers();
            chainEat(x,y,t);
            switch((*lookAt(x+2,y+2)==moveMarker)+(*lookAt(x+2,y-2)==moveMarker)\
            +(*lookAt(x-2,y+2)==moveMarker)+(*lookAt(x-2,y-2)==moveMarker)==1){
                case 1:
                    if((*lookAt(x+2,y+2)==moveMarker))piece->forcedEat(x+2,y+2);
                    else if((*lookAt(x+2,y-2)==moveMarker))piece->forcedEat(x+2,y-2);
                    else if((*lookAt(x-2,y+2)==moveMarker))piece->forcedEat(x-2,y+2);
                    else if((*lookAt(x-2,y-2)==moveMarker))piece->forcedEat(x-2,y-2);
                    return 1;
                break;

                case 0:
                    return 0;
                break;

                default:
                    cout<<"Forced eat! "<<t<<" choose where to land!\n";
                    int inpx;
                    char inpy;
                    cin>>inpx>>inpy;
                    inpx--;
                    inpy-='A';
                    if(*lookAt(inpx,inpy)!=moveMarker){
                        return 0;
                    }
                    piece->forcedEat(inpx,inpy);
                    return 1;
                break;    
            }
        }

        void chainEat(int x,int y,char t){
            if(canEatAt(x,y,t)){
                int moveY=((t=='b')? 1:-1);
                int numOfEats=0;
                if(!canEatAt(x+2,y+2*moveY,t)||!canEatAt(x-2,y+2*moveY,t)){
                    if(pieceAt(x+1,y+moveY)&&pieceAt(x+1,y+moveY)->team!=t)place(x+2,y+2*moveY,moveMarker);
                    if(pieceAt(x-1,y+moveY)&&pieceAt(x-1,y+moveY)->team!=t)place(x-2,y+2*moveY,moveMarker);
                }
                else if(isOccupied(x+1,y+moveY)&&(!isOccupied(x-1,y+moveY)||canEatAt(x+2,y+2*moveY,t)>canEatAt(x-2,y+2*moveY,t))){
                    place(x+2,y+2*moveY,moveMarker);
                    chainEat(x+2,y+2*moveY,t);
                }
                else if(isOccupied(x-1,y+moveY)&&(!isOccupied(x+1,y+moveY)||canEatAt(x+2,y+2*moveY,t)<canEatAt(x-2,y+2*moveY,t))){
                    place(x-2,y+2*moveY,moveMarker);
                    chainEat(x-2,y+2*moveY,t);
                }
            }
        }

        int canEatAt(int x,int y, char t){
            int moveY=((t=='b')? 1:-1);
            int futY= y+moveY;
            int numOfEats=0;
            if(pieceAt(x+1,futY)&& pieceAt(x+1,futY)->team!=t &&!isOccupied(x+2,futY+moveY)){
                numOfEats= max(numOfEats,1+canEatAt(x+2,futY+moveY,t));  
            }
            if(pieceAt(x-1,futY)&& pieceAt(x-1,futY)->team!=t &&!isOccupied(x-2,futY+moveY)){
                numOfEats= max(numOfEats,1+canEatAt(x-2,futY+moveY,t));
            }
            return numOfEats;
        }

        char moveMarker='O',eatMarker='0';

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
        onBoard->place(posX-1,futY,onBoard->moveMarker);
        numOfChoices++;
    }
    if(!onBoard->isOccupied(posX+1,futY)){
        onBoard->place(posX+1,futY,onBoard->moveMarker);
        numOfChoices++;
    }
    if(numOfChoices){
        onBoard->show(1);
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
        onBoard->place(posX-2,futY+moveY,onBoard->moveMarker);  
    }
}

int checker::forcedEat(int x,int y){
    onBoard->show(1);
    cout<<"mandatory eat, press enter\n";
    getchar();
    getchar();
    int difX=(x-posX)/2;
    int difY=(y-posY)/2;
    onBoard->remPiece(posX+difX,posY+difY);
    moving(x,y);
}

//======================= main func =========================================
int main(){
    char p1='w',p2='b';
    char currPlayer=p1,changePlayer=p1^p2;
    //board height, board width, cell height, cell width
    checkersBoard bord(8,8,1,3,&bord);

    for(int i=0;i<bord.height;i+=2){
        for(int j=0;j<3;j++){
            bord.makeApiece(p2,i+(j%2),j);
        }
    }

    for(int i=0;i<bord.height;i+=2){
        for(int j=0;j<3;j++){
            bord.makeApiece(p1,i+!(j%2),bord.height-1-j);
        }
    }
    while(1){
        bord.playerTurn(currPlayer);
        currPlayer=currPlayer^changePlayer;
    }
    bord.show(1);
    // cout<<endl;
    // bord.show(0);
}
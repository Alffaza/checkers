#include <iostream>
using namespace std;
class playingBoard;
class checkersBoard;
//===================== pieces for checkers =========================================
class checker{
    public:
        checker(char t,int x,int y, checkersBoard *board);
        void make(char t,int x,int y, checkersBoard *board);
        int move(char t);
        void moving(int x,int y);
        int canEat(char t);
        int forcedEat(int x,int y);
        void promote();
        void checkPromote();
        char team,sym;
        checker *ad;
        int posX,posY;
        bool isQueen;

    private:
        checkersBoard *onBoard;
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
            return (*(lookAt(x,y))!=bBox&&*(lookAt(x,y))!=wBox&&*(lookAt(x,y))!=mark);
        }

        char *lookAt(int x,int y){
            if(x<0||x>=width||y<0||y>=height)return " ";
            int id=(cWidth/2)+(cWidth)*x+(cWidth*width*cHeight)*y+(cWidth*width)*(cHeight/2);
            return (cords+id);
        }

        int height,width,cHeight,cWidth,totArea;

    protected:
        char *cords;
        char wBox='#',bBox=' ',mark='O';
};
class checkersBoard:public playingBoard{
    public:
        void checkerInit(){
            checker **newCheckers=(checker**)malloc(height*width*sizeof(checker));
            checkerCords=newCheckers;
            init();
        }

        checker * makeApiece(char t,int x,int y){
            checker* newPiece=(checker*)malloc(sizeof(checker));
            newPiece->ad=newPiece;
            newPiece->make(t,x,y,selfAdress);
            return newPiece;
        }

        void setPiece(int x,int y,checker *piece){
            if(!isOccupied(x,y)){
                place(x,y,piece->sym);
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

        checker *pieceAt(int x, int y){
            int id=y*height+x;
            return *(checkerCords+id);
        }

        void cleanAllMarkers(){
            for(int i=0;i<height;i++){
                for(int j=0;j<width;j++)
                    if(*lookAt(j,i)==moveMarker)rem(j,i);
            }
        }

        void playerTurn(char t){
            int inpx;
            char inpy;
            bool f=1,multiChoice=0;
            int maxEating = checkMandatoryEat(t);

            checker *maxPiece=NULL;
            if(!maxEating)
                do{
                    cleanAllMarkers();
                    if(!f)cout<<"wrong input!\n";
                    show(f);
                    f=0;
                    cout<<"which piece do you want to move "<<t<<"?[number][uppercase letter]\n";
                    cin>>inpx>>inpy;
                    getchar();
                    inpx--;
                    inpy-='A';
                }
                while(*lookAt(inpx,inpy)==bBox||*lookAt(inpx,inpy)==wBox||!(pieceAt(inpx,inpy)->move(t)));
            else{
                for(int i=0;i<height;i++){
                    for(int j=0;j<width;j++){
                        if(pieceAt(j,i) && pieceAt(j,i)->team==t && canEatAt(j,i,t,pieceAt(j,i)->isQueen)==maxEating){
                            if(!maxPiece)maxPiece=pieceAt(j,i);
                            else{
                                chainEat(j,i,t,pieceAt(j,i)->isQueen);
                                multiChoice=1;
                            }
                        }
                    }
                }
                if(!multiChoice)while(eatChain(maxPiece));
                else{
                    chainEat(maxPiece->posX,maxPiece->posY,maxPiece->team,maxPiece->isQueen);
                    int inpx;
                    char inpy;
                    do{
                        show(1);
                        cout<<"multiple forced eats, "<<t<<" choose piece![number][uppercase letter]\n";
                        cin>>inpx>>inpy;
                        getchar();
                        inpx--;
                        inpy-='A';
                    }while(pieceAt(inpx,inpy)==NULL||canEatAt(inpx,inpy,t,pieceAt(inpx,inpy)->isQueen)!=maxEating);
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
                        cout<<j<<" "<<i<<endl;
                        ret=max(ret,canEatAt(j,i,t,pieceAt(j,i)->isQueen));
                    }
                }
            }
            return ret;
        }

        bool eatChain(checker *piece){
            int x=piece->posX,y=piece->posY;
            char t=piece->team;
            bool queen=piece->isQueen;
            cleanAllMarkers();
            chainEat(x,y,t,queen);
            switch((*lookAt(x+2,y+2)==moveMarker)+(*lookAt(x+2,y-2)==moveMarker)\
            +(*lookAt(x-2,y+2)==moveMarker)+(*lookAt(x-2,y-2)==moveMarker)){
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
                    show(1);
                    cout<<"Forced eat! "<<t<<" choose where to land![number][uppercase letter]\n";
                    int inpx;
                    char inpy;
                    cin>>inpx>>inpy;
                    getchar();
                    inpx--;
                    inpy-='A';
                    if(*lookAt(inpx,inpy)!=moveMarker){
                        return 0;
                    }
                    piece->forcedEat(inpx,inpy);
                    return 1;
                break;    
            }
            return 0;
        }

        void chainEat(int x,int y,char t,bool queen){
            int maks = canEatAt(x,y,t,queen);
            if(maks){
                int moveY=((t=='b')? 1:-1);
                int numOfEats=0;
                bool wasMarked=0;
                if(maks==1){
                    if(!isOccupied(x+2,y+2*moveY)&&pieceAt(x+1,y+moveY)&&pieceAt(x+1,y+moveY)->team!=t)place(x+2,y+2*moveY,moveMarker);
                    if(!isOccupied(x-2,y+2*moveY)&&pieceAt(x-1,y+moveY)&&pieceAt(x-1,y+moveY)->team!=t)place(x-2,y+2*moveY,moveMarker);
                    if(queen){
                        if(!isOccupied(x+2,y-2*moveY)&&pieceAt(x+1,y-moveY)&&pieceAt(x+1,y-moveY)->team!=t)place(x+2,y-2*moveY,moveMarker);
                        if(!isOccupied(x-2,y-2*moveY)&&pieceAt(x-1,y-moveY)&&pieceAt(x-1,y-moveY)->team!=t)place(x-2,y-2*moveY,moveMarker);
                    }
                }
                else{
                    place(x+2,y+2*moveY,'X');
                    if(isOccupied(x+1,y+moveY)&&(canEatAt(x+2,y+2*moveY,t,queen)==maks-1)){
                        chainEat(x+2,y+2*moveY,t,queen);
                        place(x+2,y+2*moveY,moveMarker);
                    }
                    else rem(x+2,y+2*moveY);
                    place(x-2,y+2*moveY,'X');
                    if(isOccupied(x-1,y+moveY)&&canEatAt(x-2,y+2*moveY,t,queen)==maks-1){
                        place(x-2,y+2*moveY,'X');
                        chainEat(x-2,y+2*moveY,t,queen);    
                        place(x-2,y+2*moveY,moveMarker);   
                    }
                    else rem(x-2,y+2*moveY);
                    if(queen){
                        place(x+2,y-2*moveY,'X');
                        if(isOccupied(x+1,y-moveY)&&canEatAt(x+2,y-2*moveY,t,queen)==maks-1){
                            place(x+2,y-2*moveY,'X');
                            chainEat(x+2,y-2*moveY,t,queen);
                            place(x+2,y-2*moveY,moveMarker);
                        }
                        else rem(x+2,y-2*moveY);
                        place(x-2,y-2*moveY,'X');
                        if(isOccupied(x-1,y-moveY)&&canEatAt(x-2,y-2*moveY,t,queen)==maks-1){
                            place(x-2,y-2*moveY,'X');
                            chainEat(x-2,y-2*moveY,t,queen); 
                            place(x-2,y-2*moveY,moveMarker); 
                        }
                        else rem(x-2,y-2*moveY);
                    }
                }
            }
        }

        int canEatAt(int x,int y, char t,bool queen){
            int moveY=((t=='b')? 1:-1);
            int numOfEats=0;
            bool wasMarked=0;
            if(pieceAt(x+1,y+moveY)&& pieceAt(x+1,y+moveY)->team!=t &&!isOccupied(x+2,y+2*moveY)){
                if(*lookAt(x+2,y+2*moveY)==moveMarker)wasMarked=1;
                place(x+2,y+2*moveY,'X');
                numOfEats= max(numOfEats,1+canEatAt(x+2,y+2*moveY,t,queen));
                rem(x+2,y+2*moveY);
                if(wasMarked){
                    place(x+2,y+2*moveY,moveMarker);
                    wasMarked=0;
                }
            }
            if(pieceAt(x-1,y+moveY)&& pieceAt(x-1,y+moveY)->team!=t &&!isOccupied(x-2,y+2*moveY)){
                if(*lookAt(x-2,y+2*moveY)==moveMarker)wasMarked=1;
                place(x-2,y+2*moveY,'X');
                numOfEats= max(numOfEats,1+canEatAt(x-2,y+2*moveY,t,queen));
                rem(x-2,y+2*moveY);
                if(wasMarked){
                    place(x-2,y+2*moveY,moveMarker);
                    wasMarked=0;
                }
            }
            if(queen){
                if(pieceAt(x+1,y-moveY) && pieceAt(x+1,y-moveY)->team!=t && !isOccupied(x+2,y-2*moveY)){
                    if(*lookAt(x+2,y-2*moveY)==moveMarker)wasMarked=1;
                    place(x+2,y-2*moveY,'X');
                    numOfEats= max(numOfEats,1+canEatAt(x+2,y-2*moveY,t,queen));
                    rem(x+2,y-2*moveY);
                    if(wasMarked){
                        place(x+2,y-2*moveY,moveMarker);
                        wasMarked=0;
                    }
                }
                if(pieceAt(x-1,y-moveY) && pieceAt(x-1,y-moveY)->team!=t && !isOccupied(x-2,y-2*moveY)){
                    if(*lookAt(x-2,y-2*moveY)==moveMarker)wasMarked=1;
                    place(x-2,y-2*moveY,'X');
                    numOfEats= max(numOfEats,1+canEatAt(x-2,y-2*moveY,t,queen));
                    rem(x-2,y-2*moveY);
                    if(wasMarked){
                        place(x-2,y-2*moveY,moveMarker);
                        wasMarked=0;
                    }
                } 
            }
            return numOfEats;
        }

        char moveMarker='O';

    private:
        checker **checkerCords;
        checkersBoard *selfAdress;
};
//==================== checker method ============================
checker::checker(char t,int x,int y, checkersBoard *board){
    make(t,x,y,board);
}

void checker::make(char t,int x,int y, checkersBoard *board){
    posX=x;
    posY=y;
    team=sym=t;
    isQueen=0;
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
    int moveY= ((team=='b')? 1:-1);
    int numOfChoices=0;
    if(!onBoard->isOccupied(posX-1,posY+moveY)){
        onBoard->place(posX-1,posY+moveY,onBoard->moveMarker);
        numOfChoices++;
    }
    if(!onBoard->isOccupied(posX+1,posY+moveY)){
        onBoard->place(posX+1,posY+moveY,onBoard->moveMarker);
        numOfChoices++;
    }
    if(isQueen){
        if(!onBoard->isOccupied(posX-1,posY-moveY)){
            onBoard->place(posX-1,posY-moveY,onBoard->moveMarker);
            numOfChoices++;
        }
        if(!onBoard->isOccupied(posX+1,posY-moveY)){
            onBoard->place(posX+1,posY-moveY,onBoard->moveMarker);
            numOfChoices++;
        }   
    }
    if(numOfChoices){
        onBoard->show(1);
        cout<<"choose where to move[number][uppercase letter]\n";
        cin>>inpx>>inpy;
        getchar();
        inpx--;
        inpy-='A';
        if((inpx<0||inpx>=onBoard->width||inpy<0||inpy>=onBoard->height)||*(onBoard->lookAt(inpx,inpy))!=onBoard->moveMarker){
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
    onBoard->cleanAllMarkers();
    posX=x;
    posY=y;
    checkPromote();
}

int checker::forcedEat(int x,int y){
    onBoard->show(1);
    cout<<"mandatory eat, press enter\n";
    getchar();
    int difX=(x-posX)/2;
    int difY=(y-posY)/2;
    onBoard->remPiece(posX+difX,posY+difY);
    moving(x,y);
    return 1;
}
void checker::checkPromote(){
    if(!isQueen){
        if(team=='b'){
            if(posY==onBoard->height-1) promote();
        }
        else if(posY==0)promote();
    }
}
void checker::promote(){
    isQueen=1;
    sym+='A'-'a';
    onBoard->place(posX,posY,sym);
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


    bord.show(0);
}
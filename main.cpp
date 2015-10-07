 /*
 *  TicTacToe
 *
 *  A game by Thodoris "darkheart" Kouleris
 *  email: darkheart@teilam.gr
 *         darkheart1982@hotmail.com
 *  Home Page: http://darkheartworld.dyndns.org
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
** TicTacToe - a game by darkheart
*/


#include<stdio.h>
#include<string>
#include<SDL/SDL.h>
#include<SDL/SDL_image.h>

const int WIDTH=300;
const int HEIGHT=300;
const int BPP=32;
const int PLR=1;
const int CPU=0;

SDL_Surface *screen=NULL;
SDL_Surface *grid=NULL;
SDL_Surface *x=NULL;
SDL_Surface *o=NULL;
SDL_Surface *w=NULL;
SDL_Surface *l=NULL;

SDL_Rect offset,gridd[3][3];
SDL_Event event;

bool quit=false;
int turn=PLR,PLRpass=0,check[3][3];
//This is the structure for the coordinates of the vector which will hold data
struct coords{
        int x;
        int y;
};

struct coords crs;
struct coords cords(SDL_Event mevent){
        struct coords temp;
        if(mevent.button.x<=99 && mevent.button.y<=99){
                temp.x=0;
                temp.y=0;
        }
        if(mevent.button.x>=100 && mevent.button.x<=199 && mevent.button.y<=99){
                temp.x=0;
                temp.y=1;
        }
        if(mevent.button.x>=200 && mevent.button.x<=300 && mevent.button.y<=99){
                temp.x=0;
                temp.y=2;
        }
        if(mevent.button.x<=99 && mevent.button.y<=199 && mevent.button.y>=100){
                temp.x=1;
                temp.y=0;
        }
        if(mevent.button.x<=199 && mevent.button.x>=100 && mevent.button.y>=100 && mevent.button.y<=199){
                temp.x=1;
                temp.y=1;
        }
        if(mevent.button.x>=200 && mevent.button.x<=300 && mevent.button.y>=100 && mevent.button.y<=199){
                temp.x=1;
                temp.y=2;
        }
        if(mevent.button.x<=99 && mevent.button.y>=200 && mevent.button.y<=300){
                temp.x=2;
                temp.y=0;
        }
        if(mevent.button.x<=199 && mevent.button.x>=100 && mevent.button.y>=200 && mevent.button.y<=300){
                temp.x=2;
                temp.y=1;
        }
        if(mevent.button.x>=200 && mevent.button.x<=300 && mevent.button.y>=200 && mevent.button.y<=300){
                temp.x=2;
                temp.y=2;
        }
        return temp;
};

//The AI function has as an output a random number
int AI(void){
        int lucky_num,x;

        srand(time(NULL));
        lucky_num=rand();
        x=(lucky_num%2)+1;

        return x;
}


//The function which initialize the gridd vallues
int Init_Grid(void){

        gridd[0][0].x=0;
        gridd[0][0].y=0;

        gridd[0][1].x=100;
        gridd[0][1].y=0;

        gridd[0][2].x=200;
        gridd[0][2].y=0;

        gridd[1][0].x=0;
        gridd[1][0].y=100;

        gridd[1][1].x=100;
        gridd[1][1].y=100;

        gridd[1][2].x=200;
        gridd[1][2].y=100;

        gridd[2][0].x=0;
        gridd[2][0].y=200;

        gridd[2][1].x=100;
        gridd[2][1].y=200;

        gridd[2][2].x=200;
        gridd[2][2].y=200;

        return 0;
}

//The function which checks if anyone wins. One is for X player and 2 for O player
int Check_Win(int vec[][3]){

        int win=0;

        //win player X
        if(vec[0][0]==1 && vec[0][1]==1 && vec[0][2]==1)
                win=1;
        if(vec[1][0]==1 && vec[1][1]==1 && vec[1][2]==1)
                win=1;
        if(vec[2][0]==1 && vec[2][1]==1 && vec[2][2]==1)
                win=1;
        if(vec[0][0]==1 && vec[1][0]==1 && vec[2][0]==1)
                win=1;
        if(vec[0][1]==1 && vec[1][1]==1 && vec[2][1]==1)
                win=1;
        if(vec[0][2]==1 && vec[1][2]==1 && vec[2][2]==1)
                win=1;
        if(vec[0][0]==1 && vec[1][1]==1 && vec[2][2]==1)
                win=1;
        if(vec[0][2]==1 && vec[1][1]==1 && vec[2][0]==1)
                win=1;
        //win player O
        if(vec[0][0]==2 && vec[0][1]==2 && vec[0][2]==2)
                win=2;
        if(vec[1][0]==2 && vec[1][1]==2 && vec[1][2]==2)
                win=2;
        if(vec[2][0]==2 && vec[2][1]==2 && vec[2][2]==2)
                win=2;
        if(vec[0][0]==2 && vec[1][0]==2 && vec[2][0]==2)
                win=2;
        if(vec[0][1]==2 && vec[1][1]==2 && vec[1][2]==2)
                win=2;
        if(vec[0][2]==2 && vec[1][2]==2 && vec[2][2]==2)
                win=2;
        if(vec[0][0]==2 && vec[1][1]==2 && vec[2][2]==2)
                win=2;
        if(vec[0][2]==2 && vec[1][1]==2 && vec[2][0]==2)
                win=2;
        return win;
}

//This functions fills vectors with numbers
int Fill_Check(int nmb){
        int i,j;

        for(i=0;i<3;i++)
                for(j=0;j<3;j++)
                        check[i][j]=nmb;
        return 0;
}


//This function puts X or O where you have clicked
int put_xo(SDL_Event mevent,SDL_Surface *xo,SDL_Rect vect[][3]){
        if(mevent.button.x<=99 && mevent.button.y<=99){
                SDL_BlitSurface(xo,NULL,screen,&vect[0][0]);
        }
        if(mevent.button.x>=100 && mevent.button.x<=199 && mevent.button.y<=99){
                SDL_BlitSurface(xo,NULL,screen,&vect[0][1]);
        }
        if(mevent.button.x>=200 && mevent.button.x<=300 && mevent.button.y<=99){
                SDL_BlitSurface(xo,NULL,screen,&vect[0][2]);
        }
        if(mevent.button.x<=99 && mevent.button.y<=199 && mevent.button.y>=100){
                SDL_BlitSurface(xo,NULL,screen,&vect[1][0]);
        }
        if(mevent.button.x<=199 && mevent.button.x>=100 && mevent.button.y>=100 && mevent.button.y<=199){
                SDL_BlitSurface(xo,NULL,screen,&vect[1][1]);
        }
        if(mevent.button.x>=200 && mevent.button.x<=300 && mevent.button.y>=100 && mevent.button.y<=199){
                SDL_BlitSurface(xo,NULL,screen,&vect[1][2]);
        }
        if(mevent.button.x<=99 && mevent.button.y>=200 && mevent.button.y<=300){
                SDL_BlitSurface(xo,NULL,screen,&vect[2][0]);
        }
        if(mevent.button.x<=199 && mevent.button.x>=100 && mevent.button.y>=200 && mevent.button.y<=300){
                SDL_BlitSurface(xo,NULL,screen,&vect[2][1]);
        }
        if(mevent.button.x>=200 && mevent.button.x<=300 && mevent.button.y>=200 && mevent.button.y<=300){
                SDL_BlitSurface(xo,NULL,screen,&vect[2][2]);
        }
        return 0;
}


int main(int argc,char *argv[]){
        if(SDL_Init(SDL_INIT_EVERYTHING)==-1)
                printf("SDL Cannot Initialize");
        screen=SDL_SetVideoMode(WIDTH,HEIGHT,BPP,SDL_SWSURFACE);
        SDL_WM_SetCaption("Tic-Tac-Toe",NULL);
        Init_Grid();
        Fill_Check(0);
        grid=IMG_Load("grid.png");
        x=IMG_Load("x.png");
        o=IMG_Load("o.png");
        w=IMG_Load("win.png");
        l=IMG_Load("lose.png");
        Uint32 colorkeyx = SDL_MapRGB( x->format, 0, 0xFF, 0xFF );
        SDL_SetColorKey( x, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkeyx );
        Uint32 colorkeyo = SDL_MapRGB( o->format, 0, 0xFF, 0xFF );
        SDL_SetColorKey( o, SDL_RLEACCEL | SDL_SRCCOLORKEY, colorkeyo );
        offset.x=0;
        offset.y=0;
        SDL_BlitSurface(grid,NULL,screen,&offset);
        SDL_Flip(screen);
        while(quit==false){
                while(SDL_PollEvent(&event)){
                        if(event.type==SDL_QUIT)
                                quit=true;
                        if(turn==PLR && PLRpass==1)
                                turn=CPU;
                        //Here is the Human Player
                        if(event.type==SDL_MOUSEBUTTONDOWN){
                                crs=cords(event);
                                if(check[crs.x][crs.y]==0){
                                        if(turn==PLR){
                                                check[crs.x][crs.y]=1;
                                                put_xo(event,x,gridd);
                                                SDL_Flip(screen);
                                                PLRpass=1;

                                        }
                                }

                        }
                        //Here stops the Human player
                        //----------------------------------
                        //Checks if Human player wins
                        if(Check_Win(check)==1){
                                SDL_BlitSurface(w,NULL,screen,&offset);
                                SDL_Flip(screen);
                                Fill_Check(1);
                                break;
                        }
                        //----------------------------------
                        //Here starts the CPU player
                        switch(AI()){
                                case 1:{
                                        for(int i=0;i<=2;i++){
                                                for(int j=0;j<=2;j++){
                                                        if(check[i][j]==0){
                                                                crs.x=i;
                                                                crs.y=j;
                                                                break;
                                                        }
                                                }
                                                if(check[crs.x][crs.y]==0)
                                                        break;
                                        }
                                        break;
                                }
                                case 2:{
                                        for(int i=0;i<=2;i++){
                                                for(int j=0;j<=2;j++){
                                                        if(check[j][i]==0){
                                                                crs.x=j;
                                                                crs.y=i;
                                                                break;
                                                        }
                                                }
                                        }
                                        break;
                                }
                        }
                        if(turn==CPU){
                               check[crs.x][crs.y]=2;
                               SDL_BlitSurface(o,NULL,screen,&gridd[crs.x][crs.y]);
                               SDL_Flip(screen);
                               PLRpass=0;
                               turn=PLR;
                       }
                       //CPU player stops here
                       //Checks if CPU wins
                       if(Check_Win(check)==2){
                                SDL_BlitSurface(l,NULL,screen,&offset);
                                SDL_Flip(screen);
                                Fill_Check(2);
                       }


                }
        }
        //Ending the programm
        SDL_FreeSurface(screen);
        SDL_FreeSurface(grid);
        SDL_FreeSurface(x);
        SDL_FreeSurface(o);
        SDL_FreeSurface(l);
        SDL_FreeSurface(w);
        SDL_Quit();
        printf("Thank You For Playing TicTacToe\n");
        return 0;
}




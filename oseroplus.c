#include "oseroplus.h"
//楽なので今回はグローバル変数で管理
int board[HEIGHT][WIDTH];
int turn;//0->1->2->0
//CPUのレベルを変更できるように
int cpu1_level=2;
int cpu2_level=2;
int key;
int your_stone_color;//1:BLACK 2:WHITE 3:RED
int cpu_stone_color1;
int cpu_stone_color2;
int cpu_priority_board[HEIGHT][WIDTH]={
    {10,2,5,4,4,5,2,6,9},
    {2,1,3,3,3,3,1,2,2},
    {5,3,3,3,3,3,3,5,2},
    {4,3,3,0,0,0,3,4,4},
    {4,3,3,0,0,0,3,4,5},
    {5,3,3,0,0,0,3,5,4},
    {2,1,3,3,3,3,1,2,6},
    {1,2,5,4,4,5,2,6,10},
    {10,2,1,2,1,2,5,6,10}
};

void menu(){
    mvprintw(5,5,"REVERSE_PLUS+");
    your_stone_color = 1;
    mvprintw(8,5,"COLOR CHOICE");
    mvprintw(10,5,"BLACK");
    mvprintw(11,5,"WHITE");
    mvprintw(12,5,"RED");
    display_board();
    int y = 10;
    while(1){
        mvprintw(y,12,"<---");
        int key = getch();
        if(key=='\n') break;
        if(key==KEY_UP && y>=11){
            mvprintw(y,12,"    "); 
            y--;
        }else if(key==KEY_DOWN && y<=11){
            mvprintw(y,12,"    "); 
            y++;
        }
        refresh();
    }
    //1:BLACK 2:WHITE 3:RED
    if(y==10){
        your_stone_color = 1;
        cpu_stone_color1 = 2;
        cpu_stone_color2 = 3;
        mvprintw(3,60,"COM1 WHITE");
        mvprintw(4,60,"COM2 RED");
    }else if(y==11){
        cpu_stone_color1 = 1;
        your_stone_color = 2;
        cpu_stone_color2 = 3;
        mvprintw(3,60,"COM1 BLACK");
        mvprintw(4,60,"COM2 RED");
    }else if(y==12){
        cpu_stone_color1 = 1;
        cpu_stone_color2 = 2;
        your_stone_color = 3;
        mvprintw(3,60,"COM1 BLACK");
        mvprintw(4,60,"COM2 WHITE");
    }

    mvprintw(1,55,"...->YOU->COM1->COM2->...");

    mvprintw(14,1,"YOUR ORDER CHOICE");
    mvprintw(16,1,"FIRST");
    mvprintw(17,1,"SECOND");
    mvprintw(18,1,"THIRD");
    y = 16;
    while(1){
        mvprintw(y,12,"<---");
        int key = getch();
        if(key=='\n') break;
        if(key==KEY_UP && y>=17){
            mvprintw(y,12,"    "); 
            y--;
        }else if(key==KEY_DOWN && y<= 17){
            mvprintw(y,12,"    "); 
            y++;
        }
        refresh();
    }
    if(y==16){;
        turn = 0;
    }else if(y==17){
        turn = 2;//順番に注意
    }else if(y==18){
        turn = 1;
    }
    mvprintw(11,33,"B");
    mvprintw(13,37,"B");
    mvprintw(15,41,"B");
    mvprintw(11,37,"W");
    mvprintw(13,41,"W");
    mvprintw(15,33,"W");
    mvprintw(11,41,"R");
    mvprintw(13,33,"R");
    mvprintw(15,37,"R");
    refresh();
    int w_num = 0;
    int b_num = 0;
    int r_num = 0;
    b_num = total_stone_num(BLACK);
    w_num = total_stone_num(WHITE);
    r_num = total_stone_num(RED);
    mvprintw(10,58,"         ");
    mvprintw(11,58,"         ");
    mvprintw(12,58,"         ");
    refresh();
    mvprintw(10,58,"BLACK:%d",b_num);
    mvprintw(11,58,"WHITE:%d",w_num);
    mvprintw(12,58,"RED:%d",r_num);
    sleep(2);
}

int x_convert(int now_x){
    return (now_x - 21)/4;
}

int y_convert(int now_y){
    return (now_y - 5)/2;
}

int x_convert2(int board_x){
    return 21 + 4 * board_x;
}

int y_convert2(int board_y){
    return 5 + 2 * board_y;
}

int max(int a,int b,int c)
{
    int tmpmax;
    tmpmax = a;
    if(tmpmax < b ) {
        tmpmax = b;
    }
    if(tmpmax < c ) {
        tmpmax = c;
    }
    return tmpmax;
}

void game(){
    int proc = 1;
    while(1){
        refresh();
        if(proc==1){
            refresh();
            if(turn==0){
                mvprintw(2,30,"         ");
                mvprintw(2,30,"YOUR TURN");
            }else if(turn==1){
                mvprintw(2,30,"         ");
                mvprintw(2,30,"COM1 TURN");
            }else{
                mvprintw(2,30,"         ");
                mvprintw(2,30,"COM2 TURN");
            }
            proc = 2;
        }else if(proc==2){
            refresh();
            if(turn==0){
                if(can_put_count(your_stone_color)==0){
                    mvprintw(3,30,"NO PLACE");
                    sleep(1);
                    mvprintw(3,30,"        ");
                }else{
                    int casol_y=5,casol_x=21;//改良の余地あり,現時点では左上固定
                    while(1){
                        refresh();
                        mvprintw(casol_y,casol_x,"o");
                        int key = getch();
                        if(key=='\n'){
                            if(num_return(x_convert(casol_x),y_convert(casol_y),your_stone_color)>0){
                                put_stone(x_convert(casol_x),y_convert(casol_y),your_stone_color);
                                break;
                            }else{
                                mvprintw(3,30,"YOU CANT PUT IN THIS PLACE");
                                refresh();
                                sleep(1);
                                mvprintw(3,30,"                           ");
                            }   
                        }else if(key==KEY_UP && casol_y > 5){
                            mvprintw(casol_y,casol_x," ");
                            if(board[y_convert(casol_y)][x_convert(casol_x)]==BLACK){
                                mvprintw(casol_y,casol_x,"B");
                            }else if(board[y_convert(casol_y)][x_convert(casol_x)]==WHITE){
                                mvprintw(casol_y,casol_x,"W");
                            }else if(board[y_convert(casol_y)][x_convert(casol_x)]==RED){
                                mvprintw(casol_y,casol_x,"R");
                            }
                            casol_y -= 2;
                        }else if(key==KEY_DOWN && casol_y < 21){
                            mvprintw(casol_y,casol_x," ");
                            if(board[y_convert(casol_y)][x_convert(casol_x)]==BLACK){
                                mvprintw(casol_y,casol_x,"B");
                            }else if(board[y_convert(casol_y)][x_convert(casol_x)]==WHITE){
                                mvprintw(casol_y,casol_x,"W");
                            }else if(board[y_convert(casol_y)][x_convert(casol_x)]==RED){
                                mvprintw(casol_y,casol_x,"R");
                            }
                            casol_y += 2;
                        }else if(key==KEY_LEFT && casol_x > 21){
                            mvprintw(casol_y,casol_x," ");
                            if(board[y_convert(casol_y)][x_convert(casol_x)]==BLACK){
                                mvprintw(casol_y,casol_x,"B");
                            }else if(board[y_convert(casol_y)][x_convert(casol_x)]==WHITE){
                                mvprintw(casol_y,casol_x,"W");
                            }else if(board[y_convert(casol_y)][x_convert(casol_x)]==RED){
                                mvprintw(casol_y,casol_x,"R");
                            }
                            casol_x -= 4;
                        }else if(key==KEY_RIGHT && casol_x < 53){
                            mvprintw(casol_y,casol_x," ");
                            if(board[y_convert(casol_y)][x_convert(casol_x)]==BLACK){
                                mvprintw(casol_y,casol_x,"B");
                            }else if(board[y_convert(casol_y)][x_convert(casol_x)]==WHITE){
                                mvprintw(casol_y,casol_x,"W");
                            }else if(board[y_convert(casol_y)][x_convert(casol_x)]==RED){
                                mvprintw(casol_y,casol_x,"R");
                            }
                            casol_x += 4;
                        }
                    }
                }
            }else if(turn==1){
                CPU(cpu_stone_color1,cpu1_level);
                sleep(1);
                mvprintw(3,30,"             ");
            }else if(turn==2){
                CPU(cpu_stone_color2,cpu2_level);
                sleep(1);
                mvprintw(3,30,"             ");
            }
            proc = 3;
        }else if(proc==3){
            refresh();
            int b_num = total_stone_num(BLACK);
            int w_num = total_stone_num(WHITE);
            int r_num = total_stone_num(RED);
            mvprintw(10,58,"         ");
            mvprintw(11,58,"         ");
            mvprintw(12,58,"         ");
            mvprintw(10,58,"BLACK:%d",b_num);
            mvprintw(11,58,"WHITE:%d",w_num);
            mvprintw(12,58,"RED:%d",r_num);
            refresh();

            //ターンチェンジ
            if(turn==0){
                turn = 1;
            }else if(turn==1){
                turn = 2;
            }else{
                turn = 0;
            }
            proc = 4;

        }else if(proc==4){
            refresh();
            if(can_put_count(your_stone_color)==0 && can_put_count(cpu_stone_color1)==0 && can_put_count(cpu_stone_color2)==0){
                proc = 5;
            }else{
                proc = 1;
            }
        }else if(proc==5){
            int b_num = total_stone_num(BLACK);
            int w_num = total_stone_num(WHITE);
            int r_num = total_stone_num(RED);
            int bwrMax = max(b_num,w_num,r_num);
            mvprintw(2,30,"           ");
            mvprintw(2,30,"Finish Game");
            if(b_num==w_num && b_num==r_num){
                mvprintw(3,30,"DRAW :|");
            }else if(b_num==w_num && b_num==bwrMax){
                mvprintw(3,30,"BLACK and WHITE win :)");
            }else if(b_num==w_num && r_num==bwrMax){
                mvprintw(3,30,"RED win :)");
            }else if(b_num==r_num && b_num==bwrMax){
                mvprintw(3,30,"BLACK and RED win :)");
            }else if(b_num==r_num && w_num==bwrMax){
                mvprintw(3,30,"WHITE win :)");
            }else if(w_num==r_num && w_num==bwrMax){
                mvprintw(3,30,"WHITE and RED win :)");
            }else if(w_num==r_num && b_num==bwrMax){
                mvprintw(3,30,"BLACK win :)");
            }else if(w_num==bwrMax){
                mvprintw(3,30,"WHITE win :)");
            }else if(b_num==bwrMax){
                mvprintw(3,30,"BLACK win :)");
            }else if(r_num==bwrMax){
                mvprintw(3,30,"RED win :)");
            }
            refresh();
            sleep(2);
            break;
        }
    }
}

int total_stone_num(int color){
    int cnt = 0;
    int x,y;
    for(y=0;y<HEIGHT;y++){
        for(x=0;x<HEIGHT;x++){
            if(board[y][x]==color)
                cnt++;
        }
    }
    return cnt;
}

void put_stone(int x,int y,int my_color){
    mvprintw(y_convert2(y),x_convert2(x)," ");
    board[y][x] = my_color;
    if(my_color==BLACK){
        mvprintw(y_convert2(y),x_convert2(x),"B");
    }else if(my_color==WHITE){
        mvprintw(y_convert2(y),x_convert2(x),"W");
    }else{
        mvprintw(y_convert2(y),x_convert2(x),"R");
    }
    int dx,dy;
    for(dy=-1;dy<=1;dy++){
        for(dx=-1;dx<=1;dx++){
            if(dx==0 && dy==0)
                continue;
            int sx,sy;
            int k=0;
            sx = x;
            sy = y;
            while(1){
                sx += dx;
                sy += dy;
                if(sx<0 || sy<0 || sx>8 || sy>8)
                    break;
                if(board[sy][sx]==BLANK)
                    break;
                if(board[sy][sx]!=my_color)
                    k += 1;
                if(board[sy][sx]==my_color){
                    int i;
                    for(i=0;i<k;i++){
                        sx -= dx;
                        sy -= dy;
                        board[sy][sx]=my_color;
                        mvprintw(y_convert2(sy),x_convert2(sx)," ");
                        if(my_color==BLACK){
                            mvprintw(y_convert2(sy),x_convert2(sx),"B");
                        }else if(my_color==WHITE){
                            mvprintw(y_convert2(sy),x_convert2(sx),"W");
                        }else{
                            mvprintw(y_convert2(sy),x_convert2(sx),"R");
                        }
                    }
                    break;
                }
            }
        }
    }
}

void put_stone_cpu(int x,int y,int my_color){
    board[y][x] = my_color;
    int dx,dy;
    for(dy=-1;dy<=1;dy++){
        for(dx=-1;dx<=1;dx++){
            if(dx==0 && dy==0)
                continue;
            int sx,sy;
            int k=0;
            sx = x;
            sy = y;
            while(1){
                sx += dx;
                sy += dy;
                if(sx<0 || sy<0 || sx>8 || sy>8)
                    break;
                if(board[sy][sx]==BLANK)
                    break;
                if(board[sy][sx]!=my_color)
                    k += 1;
                if(board[sy][sx]==my_color){
                    int i;
                    for(i=0;i<k;i++){
                        sx -= dx;
                        sy -= dy;
                        board[sy][sx]=my_color;
                    }
                    break;
                }
            }
        }
    }
}

int num_return(int x,int y,int my_color){
    int count = 0;
    if(board[y][x]!=BLANK){
        return 0;
    }
    int dx,dy;
    for(dy=-1;dy<=1;dy++){
        for(dx=-1;dx<=1;dx++){
            if(dy==0 && dx==0)
                continue;
            int k=0;
            int nxtx = x;
            int nxty = y;
            while(1){
                nxtx += dx;
                nxty += dy;
                if(nxty<0 || nxty>8|| nxtx<0 || nxtx>8)
                    break;
                if(board[nxty][nxtx]==BLANK)
                    break;
                if(board[nxty][nxtx]!=my_color)
                    k++;
                if(board[nxty][nxtx]==my_color){
                    count += k;
                    break;
                }
            }
        }
    }
    return count;
}

void display_board(){
    int i,j;
    for(i=4;i<=4+2*HEIGHT;i+=2){
        mvprintw(i,20,"-----------------------------------");
    }
    for(i=4;i<=4+2*HEIGHT;i++){
        for(j=19;j<=19+4*WIDTH;j+=4){
            mvprintw(i,j,"|");
        }
    }
}

void reset_board(){
    int x,y;
    for(y=0;y<HEIGHT;y++){
        for(x=0;x<WIDTH;x++){
            if(x==3 && y==3 || x==4 && y==4 || x==5 && y==5){
                board[y][x] = BLACK;
            }else if(x==4 && y==3 || x==5 && y==4 || x==3 && y==5){
                board[y][x] = WHITE;
            }else if(x==5 && y==3 || x==3 && y==4 || x==4 && y==5){
                board[y][x] = RED;
            }else{    
                board[y][x] = BLANK;
            }
        }
    }
}

int can_put_count(int my_color){
    int cnt = 0;
    int x,y;
    for(y=0;y<HEIGHT;y++){
        for(x=0;x<WIDTH;x++){
            if(num_return(x,y,my_color)>0){
                cnt++;
            }
        }
    }
    if(cnt>0){
        return 1;
    }else{
        return 0;
    }
}

//ここからCPU作成
void CPU(int cpu_color,int cpu_level){
    if(cpu_level==1){
        cpu_LV1(cpu_color,cpu_level);
    }else if(cpu_level==2){
        cpu_LV2(cpu_color,cpu_level);
    }//else if(cpu_level==3){
        //cpu_LV3(cpu_color);
    //}
}

void cpu_LV1(int cpu_color,int cpu_level){
    if(can_put_count(cpu_color)==0){
        mvprintw(3,30,"CPU%d NO SPACE",cpu_level);
        refresh();
    }else{
        int cx,cy;
        while(1){
            cx = rand()%9;
            cy = rand()%9;
            if(num_return(cx,cy,cpu_color)>0){
                break;
            }
        }
        put_stone(cx,cy,cpu_color);
    }
}

void cpu_LV2(int cpu_color,int cpu_level){//cpu_priority_board
    if(can_put_count(cpu_color)==0){
        mvprintw(3,30,"CPU%d NO SPACE",cpu_level);
        refresh();
    }else{
        int c_x=0,c_y=0;
        int p=0;
        int x,y;
        for(y=0;y<HEIGHT;y++){
            for(x=0;x<WIDTH;x++){
                if(num_return(x,y,cpu_color)>0 && cpu_priority_board[y][x]>p){
                    p = cpu_priority_board[y][x];
                    c_x = x;
                    c_y = y;
                }
            }
        }
        put_stone(c_x,c_y,cpu_color);
    }
}
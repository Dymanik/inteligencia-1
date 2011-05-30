#include "staticboard.h"
#include <time.h>
#include <queue>

using namespace std;
typedef struct pattern{
    char pat[7][2];
    char h;
} Pattern;

State board;
char LU[4][4] = {{0,1,2,3},{4,5,6,7},{8,9,10,11},{12,13,14,15}};
short lookup[2][2]; 

char p11 [16][16][16][16][16][16][8];
char p12 [16][16][16][16][16][16][8];
char p13 [16];
char pm1 [15][2];

char p21 [16][16][16][16][16][16][8];
char p22 [16][16][16][16][16][16][8];
char p23 [16];
char pm2 [15][2];

long nodosVis;

bool operator< (Pattern a, Pattern b){
    if(a.h >= b.h) return true;

    return false;
}

void inipat(State s, char pm[15][2],Pattern p[2]){

    int i,j;
    for(i=0; i<4; i++){
        for(j=0; j<4; j++){
            int num = getPos(s,i,j);
            if(pm[num][0]==0) {
            	p[0].pat[pm[num][1]][0]=i;
            	p[0].pat[pm[num][1]][1]=j;
            }
            if(pm[num][0]==1){
            	p[1].pat[pm[num][1]][0]=i;
            	p[1].pat[pm[num][1]][1]=j;
            }
        }
    }

    p[0].h = 0;
    p[1].h = 0;

}

void Exp(Pattern pi,int n,Pattern p[4]){

    int i,t=1;
    if(p[0].pat[n][0]>0){
        for(i=0; i<n; i++){
            if(p[0].pat[i][0]==p[0].pat[n][0] && p[0].pat[i][1]==p[0].pat[n][1]){
            	t=0;
                break;
            }
        }
        if(t){
            memcpy(p[0].pat,pi.pat,sizeof(char)*16);
            p[0].h = pi.h+1;
            p[0].pat[n][0]--;
        }
    }
    t=1;
    if(p[1].pat[n][0]<3){
        for(i=0; i<n; i++){
            if(p[1].pat[i][0]==p[1].pat[n][0] && p[1].pat[i][1]==p[1].pat[n][1]){
                t=0;
                    break;
            }
        }
        if(t){
            memcpy(p[1].pat,pi.pat,sizeof(char)*16);
            p[1].h = pi.h+1;
            p[1].pat[n][0]++;
        }
    }
    t=1;
    if(p[2].pat[n][1]<3){
        for(i=0; i<n; i++){
            if(p[2].pat[i][0]==p[2].pat[n][0] && p[2].pat[i][1]==p[2].pat[n][1]){
                t=0;
                break;
            }
        }
        if(t){
            memcpy(p[2].pat,pi.pat,sizeof(char)*16);
            p[2].h = pi.h+1;
            p[2].pat[n][1]++;
        }
    }
    t=1;
    if(p[3].pat[n][1]>0){
        for(i=0; i<n; i++){
            if(p[3].pat[i][0]==p[3].pat[n][0] && p[3].pat[i][1]==p[3].pat[n][1]){
                t=0;
                break;
            }
        }
        if(t){
            memcpy(p[3].pat,pi.pat,sizeof(char)*16);
            p[3].pat[n][1]--;
            p[3].h = pi.h+1;
        }
    }

}

short bfs(Pattern v,char* bd){

    queue<Pattern> q;
    q.push(v);
    //meter v en las bd;
    while(!q.empty()){
        Pattern w = q.pop();
        for(int j=0;j<7;j++){
            Pattern vecs[4];
            Exp(w,j,vecs);
            for(int i=0; i<4; i++){
            	Pattern v = vecs[i];
                char value=bd[LU[v.pat[0][0]][v.pat[0][1]]][LU[v.pat[1][0]][v.pat[1][1]]]
                [LU[v.pat[2][0]][v.pat[2][1]]][LU[v.pat[3][0]][v.pat[3][1]]]
                [LU[v.pat[4][0]][v.pat[4][1]]][LU[v.pat[5][0]][v.pat[5][1]]]
                [LU[v.pat[6][0]][v.pat[6][1]]][LU[v.pat[7][0]][v.pat[7][1]]];
                if(value!=0){
                    bd[LU[v.pat[0][0]][v.pat[0][1]]][LU[v.pat[1][0]][v.pat[1][1]]]
                    [LU[v.pat[2][0]][v.pat[2][1]]][LU[v.pat[3][0]][v.pat[3][1]]]
                    [LU[v.pat[4][0]][v.pat[4][1]]][LU[v.pat[5][0]][v.pat[5][1]]]
                    [LU[v.pat[6][0]][v.pat[6][1]]][LU[v.pat[7][0]][v.pat[7][1]]]=v.h;
                    q.push(v);
                }
            }
        }
    }
}

void expU(){
	short wx,wy,h;
	wx=getWhiteX(board);
	wy=getWhiteY(board);
	h=getH(board);
	if(lookup[getPos(board,wx-1,wy)][0] < wx){
		setH(&board,h+1);
	}else{
		setH(&board,h-1);
	}
	moveU(&board);
}

void expD(){
	short wx,wy,h;
	wx=getWhiteX(board);
	wy=getWhiteY(board);
	h=getH(board);
	if(lookup[getPos(board,wx+1,wy)][0] > wx){
		setH(&board,h+1);
	}else{
		setH(&board,h-1);
	}
	moveD(&board);
}
void expL(){
	short wx,wy,h;
	wx=getWhiteX(board);
	wy=getWhiteY(board);
	h=getH(board);
	if(lookup[getPos(board,wx,wy-1)][1] < wy){
		setH(&board,h+1);
	}else{
		setH(&board,h-1);
	}
	moveL(&board);
}
void expR(){
	short wx,wy,h;
	wx=getWhiteX(board);
	wy=getWhiteY(board);
	h=getH(board);
	if(lookup[getPos(board,wx,wy+1)][1] > wy){
		setH(&board,h+1);
	}else{
		setH(&board,h-1);
	}
	moveR(&board);
}

short hU(){
	short wx,wy,h;
	wx=getWhiteX(board);
	wy=getWhiteY(board);
	h=getH(board);
	if(lookup[getPos(board,wx-1,wy)][0] < wx){
	    return h+1;
	}else{
		return h-1;
	}
}

short hD(){
	short wx,wy,h;
	wx=getWhiteX(board);
	wy=getWhiteY(board);
	h=getH(board);
	if(lookup[getPos(board,wx+1,wy)][0] > wx){
	    return h+1;
	}else{
		return h-1;
	}
}

short hL(){
	short wx,wy,h;
	wx=getWhiteX(board);
	wy=getWhiteY(board);
	h=getH(board);
	if(lookup[getPos(board,wx,wy-1)][1] < wy){
        return h+1;
	}else{
		return h-1;
	}
}
short hR(){
	short wx,wy,h;
	wx=getWhiteX(board);
	wy=getWhiteY(board);
	h=getH(board);
	if(lookup[getPos(board,wx,wy+1)][1] > wy){
		return h+1;
	}else{
		return h-1;
	}
}

short heuristica(State s){

    

}

char dirs[4]={'d','l','r','u'};
int dfs(int sc, int cl, char* ok, char dir){
	nodosVis++;
    short curnth = getH(board);
    short cm = sc + curnth;
    if(cm > cl) return cm;
    if(isGoal(board)){
		*ok = 1;
		return sc;
	}

    int ncl = 32767;
    short i, j;
    short ord[4] = {0,1,2,3}; 
    short hord[4] = {200, 200, 200, 200};

    if(wx <= 0) hord[0] = hD();
	if(wy >= 3) hord[1] = hL();
	if(wy <= 0) hord[2] = hR();
	if(wx >= 3) hord[3] = hU();

    for(i=0, j=3; i>=j;){
        if(hord[i]>curnth){
            short temp = hord[i];
            hord[i] = hord[j];
            hord[j] = temp;
            temp = ord[i];
            ord[i] = ord[j];
            ord[j] = temp;
            j--;
        }else{
            i++;
        }
    }

    int ret;
	short wx,wy,d;
	wx = getWhiteX(board);
	wy = getWhiteY(board);
    for(i=0;i<4;i++){
        d = ord[i];
		State succ;
		//moviendo
		switch(d){
			case 0:
				if(wx <= 0 || dir==dirs[d]) continue;
				expU(board);
				break;
			case 1:
				if(wy >= 3 || dir==dirs[d]) continue;
				expR(board);
				break;
			case 2:
				if(wy <=0 || dir==dirs[d]) continue;
				expL(board);
				break;
			case 3:
				if(wx >= 3 || dir==dirs[d]) continue;
				expD(board);
				break;
		}
		//imprimir(succ);
        ret = dfs(sc+1, cl, ok,dirs[3-d]);
        if(*ok) return ret;
        if(ncl > ret) ncl = ret;
		//regresando el movimiento
		switch(d){
			case 0:
				expD(board);
				break;
			case 1:
				expL(board);
				break;
			case 2:
				expR(board);
				break;
			case 3:
				expU(board);
				break;
		}
	} 
	return ncl;
}


int solve(State s){
    
    int cl = getH(board);
    char ok = 0;
    while(1){
		printf("%d ",cl);
        int ret = dfs(0,cl,&ok,'i');
        if(ok) return ret;
		if(ret>3000) return -1;
        cl = ret;
    }
}

int main(){

    memset(p11, 0, 16*16*16*16*16*16*8);
    memset(p12, 0, 16*16*16*16*16*16*8);
    memset(p13, 0, 16);
    memset(p21, 0, 16*16*16*16*16*16*8);
    memset(p22, 0, 16*16*16*16*16*16*8);
    memset(p23, 0, 16);
	int pasos;
	clock_t begin,end;
	while(1){
	    board = sboard();
	    nodosVis=0;
	    imprimir(board);
	    begin=clock();
	    pasos=solve(board);
	    end=clock();
	    printf("%ld %d %f\n\n",nodosVis,pasos,((double)(end-begin))/CLOCKS_PER_SEC);
	}

}
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

const int ninzuu = 20;
const int siaisuu = 3;
const double win = 1;
const double draw =0.5;
const double lose = 0;

typedef struct{
	int 	aite;
	double 	WL;
} int_dbl;

typedef struct{
	int 	sebangou;
	int 	power;
	int_dbl	kekka[siaisuu];
	double	katiten;
	double 	SOS;
	double	SB;
} player;

void syokika_plyr(player arr[],int ninzuu);
void givepower_rand(player arr[],int ninzuu);
void plyr_print(player arr[],int i);
void rank_print(player rank_arr[]);
int compare(const void *n1, const void *n2);
int havedone(int n1,int n2,int cr_round,player arr[]);
void matching(player p_arr[], int ninzuu,match[][2],int cr_round);
void simple_battle(int match[][2],player arr[],int cr_round,ninzuu);
void updt_katiten1(player arr[],int cr_round);
void updt_katiten2(player arr[],int cr_round);
void updt_SOS1(player arr[],int cr_round);
void updt_SOS2(player arr[],int cr_round);
void updt_SB1(player arr[],int cr_round);
	
	
int main(void){
	player basic_order[ninzuu]; 
	player rank_order[ninzuu];
	syokika_plyr(basic_order, ninzuu);
	int matchtable[ninzuu / 2][2];
	int round = 0;
	
	memcpy(rank_order,basic_order,sizeof(basic_order));
	qsort(rank_order,ninzuu,sizeof(player),compare);
	
	for(int i = 0; i < siaisuu; i ++){
		matching(rank_order, ninzuu, matchtable, round);
		simple_battle(matchtable, basic_order, round,ninzuu);
		updt_katiten1(basic_order, round);
		updt_SOS1(basic_order, round);
		updt_SB1(basic_order, round);
		memcpy(rank_order,basic_order,sizeof(basic_order));
		qsort(rank_order,ninzuu,sizeof(player),compare);
		round ++;
	}
	
	rank_print(rank_order);
	plyr_print(basic_order, 7);
	return 0;
}

int compare(const void *n1, const void *n2){
	if( ((player *)n1)->katiten > ((player*)n2)->katiten ){
		return -1;
	}else if( ((player *)n1)->katiten < ((player*)n2)->katiten ){
		return 1;
	}else if( ((player *)n1)->SOS > ((player*)n2)->SOS ){
		return -1;
	}else if( ((player *)n1)->SOS < ((player*)n2)->SOS ){
		return 1;
	}else if( ((player *)n1)->SB > ((player*)n2)->SB){
		return -1;
	}else if( ((player *)n1)->SB < ((player*)n2)->SB){
		return 1;
	}else{
		return 0;
	}
}

void syokika_plyr(player arr[],const int ninzuu){
	for(int i = 0; i < ninzuu; i ++){
		arr[i].sebangou	= i;
		arr[i].power	= i; //とりあえず背番号を戦闘力としておく
		arr[i].katiten	= 0;
		arr[i].SOS		= 0;
		arr[i].SB		= 0;
		for(int j = 0; j < siaisuu; j ++){
			arr[i].kekka[j].aite 	= -99;
			arr[i].kekka[j].WL		= -99;
		}
	}
}

void givepower_rand(player arr[],const int ninzuu){
	for(int i = 0; i < ninzuu; i ++){
		arr[i].power = rand() / 100;
	}
}

void matching(player p_arr[],const int ninzuu,match[][2],int cr_round){
	int temp[ninzuu];
	for(int i = 0; i < ninzuu; i ++){
		temp[i] = p_arr[i].sebangou;
	}
	for(int i = 0; i < ninzuu / 2; i ++){
		match[i][0] = temp[0];
		int j = 1;
		for(j = 1; j < ninzuu ;j ++){
			if(havedone(temp[0],temp[j],cr_round,p_arr) == 0){
				match[i][1] = temp[j];
				break;
			}
		}
		// temp配列押上
		for(int l = 1; l < j; l ++){
			temp[l - 1] = temp[l];
		}
		for(int m = j + 1; m < ninzuu - i * 2; m ++){
			temp[m - 2] = temp[m];
		}
	}
}
	

	
int havedone(int n1,int n2,int cr_round,player arr[]){
	for(int i = 0; i < cr_round; i ++){
		if(arr[n1].kekka[i].aite == n2){
			return 1;
		}
	}
	return 0;
}
	
void plyr_print(player arr[],int i){
	printf("背番号	: %d\n",arr[i].sebangou);
	printf("勝ち点	: %.1f\n",arr[i].katiten);
	printf("SOS		: %.1f\n",arr[i].SOS);
	printf("SB		: %.1f\n",arr[i].SB);
}
	
void simple_battle(int match[][2],player arr[],int cr_round,ninzuu){
	for(int i = 0; i < ninzuu / 2; i ++){
		arr[match[i][0]].kekka[cr_round].aite = match[i][1];
		arr[match[i][1]].kekka[cr_round].aite = match[i][0];
		if(match[i][0] > match[i][1]){
			arr[match[i][0]].kekka[cr_round].WL = win;
			arr[match[i][1]].kekka[cr_round].WL = lose;
		}
		else if(match[i][0] < match[i][1]){
			arr[match[i][0]].kekka[cr_round].WL = lose;
			arr[match[i][1]].kekka[cr_round].WL = win;
		}
		else{
			arr[match[i][0]].kekka[cr_round].WL = draw;
			arr[match[i][1]].kekka[cr_round].WL = draw;
		}
	}
}

void updt_katiten1(player arr[],int cr_round){
	for(int i = 0; i < ninzuu; i ++ ){
		arr[i].katiten = 0;
		for(int j = 0; j <= cr_round; j ++){
			arr[i].katiten += arr[i].kekka[j].WL;
		}
	}
}

void updt_katiten2(player arr[],int cr_round){
	for(int i = 0; i < ninzuu; i ++ ){
		arr[i].katiten += arr[i].kekka[cr_round].WL;
	}
}

void updt_SOS1(player arr[],int cr_round){
	for(int i = 0; i < ninzuu; i ++ ){
		arr[i].SOS = 0;
		for(int j = 0; j <= cr_round; j ++){
			arr[i].SOS += arr[arr[i].kekka[j].aite].katiten;
		}
	}
}

void updt_SOS2(player arr[],int cr_round){
	for(int i = 0; i < ninzuu; i ++){
		arr[arr[i].kekka[cr_round].aite].SOS += arr[i].katiten; //今回の相手に自分の勝ち点全部あげる
		if(arr[i].kekka[cr_round].WL == win){
			for(int j = 0; j < cr_round; j ++){
				arr[arr[i].kekka[j].aite].SOS += win;
			}
		}else if(arr[i].kekka[cr_round].WL == draw){
			for(int j = 0; j < cr_round; j ++){
				arr[arr[i].kekka[j].aite].SOS += draw;
			}
		}else if(arr[i].kekka[cr_round].WL == lose){
			for(int j = 0; j < cr_round; j ++){
				arr[arr[i].kekka[j].aite].SOS += lose;
			}
		}
	}
}

void updt_SB1(player arr[],int cr_round){
	for(int i = 0; i < ninzuu; i ++ ){
		arr[i].SB = 0;
		for(int j = 0; j <= cr_round; j ++){
			if(arr[i].kekka[j].WL == win){
				arr[i].SB += arr[arr[i].kekka[j].aite].katiten;
			}
		}
	}
}

void rank_print(player rank_arr[]){
	for(int i = 0; i < ninzuu; i ++){
		printf("%d位 : 背番号%d : 戦闘力%d : 勝ち点%.1f : SOS%.1f : SB%.1f\n",i + 1,
		rank_arr[i].sebangou,
		rank_arr[i].power,
		rank_arr[i].katiten,
		rank_arr[i].SOS,
		rank_arr[i].SB);
	}
}
		
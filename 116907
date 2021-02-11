#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define N 50//プレイヤー数
#define ROUND 10//試合数
#define WIN 1.0	//勝ち点
#define LOSE 0	//負け点
#define DRAW 0.5	//引き分け点
#define SYOKI -99	//初期化時の仮数字

// 結果配列のための構造体。対戦した相手と勝敗。
typedef struct{
	int 	aite;
	double 	WL;
} int_dbl;

// プレイヤーのステータスの構造体
typedef struct{
	int 	sebangou;
	int 	power;
	int_dbl	kekka[ROUND];
	double	katiten;
	double 	SOS;
	double	SB;
} player;

// プロトタイプ宣言
int compare(const void *n1, const void *n2);
void syokika_player(player array[]);
void givepower_sebangou(player array[]);
void givepower_gyaku(player array[]);
void givepower_random(player array[]);
void simple_battle(player arr[]);
void updt_katiten(player arr[]);
void updt_SOS(player arr[]);
void updt_SB(player arr[]);
int already(int sebangou1,int sebangou2);
int decided(int x);
int matching(int x);
int Turn(void);
void print_ranking(player rank_arr[]);

// 検証用関数
void print_kekka(player arr[]);
void print_rank(void);
void print_matchpair(void);

player sebangou_jun[N]; // プレイヤー構造体を人数分の配列で宣言
player rank_jun[N]; //こっちは順位順

int cr_round;	//今、何回戦めか
int rank[N][2]; //順位順に背番号を並べた配列。右はマッチングペアが決定しているか(肯定:1,否定:0)
int matchpair[N / 2][2]; //(N / 2)組の対戦を組んだ表

//qsortに必要な比較関数
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

void syokika_player(player array[]) //戦闘力以外を初期化
{
	for(int i = 0; i < N; i ++){
		array[i].sebangou	= i;
		array[i].katiten	= 0;
		array[i].SOS		= 0;
		array[i].SB			= 0;
		for(int j = 0; j < ROUND; j ++){
			array[i].kekka[j].aite 	= SYOKI;
			array[i].kekka[j].WL	= SYOKI;
		}
	}
}

// 戦闘力設定その1。背番号を戦闘力とする。
void givepower_sebangou(player array[])
{
	for(int i = 0; i < N; i ++){
		array[i].power = i;
	}
}

// 戦闘力設定その2。背番号と戦闘力を逆にする。
void givepower_gyaku(player array[])
{
	for(int i = 0; i < N; i ++){
		array[i].power = N - i - 1;
	}
}

// 戦闘力設定その3。0~99の値をランダムで与える。
void givepower_random(player array[])
{
	for(int i = 0; i < N; i ++){
		array[i].power = rand() % 100;
	}
}

// 単純なバトル。戦闘力の高いほうが勝ち。
void simple_battle(player arr[])
{
	for(int i = 0; i < N / 2; i ++){
		arr[matchpair[i][0]].kekka[cr_round].aite = matchpair[i][1];
		arr[matchpair[i][1]].kekka[cr_round].aite = matchpair[i][0];
		if(arr[matchpair[i][0]].power > arr[matchpair[i][1]].power){
			arr[matchpair[i][0]].kekka[cr_round].WL = WIN;
			arr[matchpair[i][1]].kekka[cr_round].WL = LOSE;
		}
		else if(arr[matchpair[i][0]].power < arr[matchpair[i][1]].power){
			arr[matchpair[i][0]].kekka[cr_round].WL = LOSE;
			arr[matchpair[i][1]].kekka[cr_round].WL = WIN;
		}
		else{
			arr[matchpair[i][0]].kekka[cr_round].WL = DRAW;
			arr[matchpair[i][1]].kekka[cr_round].WL = DRAW;
		}
	}
}

// 勝ち点の更新
void updt_katiten(player arr[])
{
	for(int i = 0; i < N; i ++ ){
		arr[i].katiten = 0;
		for(int j = 0; j <= cr_round; j ++){
			arr[i].katiten += arr[i].kekka[j].WL;
		}
	}
}

// ソルコフの更新
void updt_SOS(player arr[])
{
	for(int i = 0; i < N; i ++ ){
		arr[i].SOS = 0;
		for(int j = 0; j <= cr_round; j ++){
			arr[i].SOS += arr[arr[i].kekka[j].aite].katiten;
		}
	}
}

// SBの更新
void updt_SB(player arr[])
{
	for(int i = 0; i < N; i ++ ){
		arr[i].SB = 0;
		for(int j = 0; j <= cr_round; j ++){
			if(arr[i].kekka[j].WL == WIN){
				arr[i].SB += arr[arr[i].kekka[j].aite].katiten;
			}
		}
	}
}

//既に対戦済みかをチェックする関数。戻り値(対戦済み→1,未対戦→0)。
int already(int sebangou1,int sebangou2) 
{
	for(int i = 0; i < cr_round; i ++)
	{
		if(sebangou_jun[sebangou1].kekka[i].aite == sebangou2)
			return 1;
	}
	return 0;
}

// xは順位です。
// マッチングが決まった(マッチング配列に入った)かのチェック。
// 戻り値(決まっている→1, 残っている→0)
int decided(int x) 
{
	if(rank[x][1])
		return 1;
	else
		return 0;
}

 // マッチペア配列のx列を決める
 // 戻り値(マッチング成功→1, 失敗→0)
int matching(int x)
 {
	if(x >= N / 2)
		return 1;
	int left;
	int right;
	for(left = x; left < N; left ++) //マッチペア配列の左側を埋める
	{
		if(decided(left) == 0)
		{
			matchpair[x][0] = rank[left][0];
			rank[left][1] = 1;
			break;
		}
	}
	for(right = left + 1; right < N; right ++)//右側を埋める
	{
		if(	(already(rank[left][0],rank[right][0]) == 0)&&(decided(right) == 0)	)
		{
			
			matchpair[x][1] = rank[right][0];
			rank[right][1] = 1;
			if(matching(x + 1)){
				return 1;
			}else{
				rank[right][1] = 0;
			}
		}
	}
	rank[left][1] = 0; //これが足りなかった？
	return 0;
}

/*
①順位順プレイヤー配列からrank[N][2]配列を作成
②対戦カードを決定(matching関数)→(matchpair配列)
③対戦し、結果を
④勝ち点,SOS,SBを計算
⑤qsortで構造体を順位順に並び替え
*/

// 一連の手順を行う
int Turn(void)
{
	// rank配列に背番号と右0をコピー
	for(int i = 0; i < N; i ++)
	{
		rank[i][0] = rank_jun[i].sebangou;
		rank[i][1] = 0;
	}
	if(matching(0))
	// マッチングに成功したなら、バトっていろいろ更新
	{
		simple_battle(sebangou_jun);
		updt_katiten(sebangou_jun);
		updt_SOS(sebangou_jun);
		updt_SB(sebangou_jun);
		// 背番号順構造体配列をを順位順構造体配列にコピー
		memcpy(rank_jun,sebangou_jun,sizeof(sebangou_jun));
		// それをソート
		qsort(rank_jun,N,sizeof(player),compare);
		return 1;
	}else
		return 0;
}

void print_ranking(player rank_arr[]){
	for(int i = 0; i < N; i ++){
		printf("%d位 : 背番号%d : 戦闘力%d : 勝ち点%.1f : SOS%.1f : SB%.1f\n",i + 1,
		rank_arr[i].sebangou,
		rank_arr[i].power,
		rank_arr[i].katiten,
		rank_arr[i].SOS,
		rank_arr[i].SB);
	}
}


int main()
{
	// プレイヤー構造体の初期化、戦闘力付与②
	syokika_player(sebangou_jun);
	givepower_gyaku(sebangou_jun);
	// 背番号順構造体配列を順位順構造体配列にコピー
	memcpy(rank_jun,sebangou_jun,sizeof(sebangou_jun));
	cr_round = 0;
	for(int i = 0; i < ROUND; i ++)
	{
		if(Turn()){
			cr_round ++;
		}else{
			printf("%d回戦目でマッチング失敗\n",cr_round);
			break;
		}
	}
	print_ranking(rank_jun);
	
	return 0;
}


void print_kekka(player arr[]){
	for(int i = 0; i < N; i ++){
		printf("%d位 : 背番号%d:",i + 1,arr[i].sebangou);
		for(int j = 0; j < ROUND; j ++){
			printf("相手%d(%.1f点), ",arr[i].kekka[j].aite,arr[i].kekka[j].WL);
		}
		printf("\n");
	}
}

//rank[N][2]出力
void print_rank(void)
{
	printf("%d回戦のrank配列\n",cr_round);
	for(int i = 0;i < N; i ++)
	{
		printf("%d, %d\n",rank[i][0],rank[i][1]);
	}
}

//matchpair[N / 2][2]出力
void print_matchpair(void)
{
	printf("%d回戦のmatchpair配列\n",cr_round);
	for(int i = 0;i < N / 2; i ++)
	{
		printf("%d, %d\n",matchpair[i][0],matchpair[i][1]);
	}
}

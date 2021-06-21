#include "Game.h"
#include "SceneMgr.h"
#include "DxLib.h"
#include "Input.h"

int Tile[11]; //タイル
int s;	//サイズ
int v;	//
int x;	//x軸
int y;	//y軸
int i;	//添え字
int j;	//添え字

static int gameScore;
int Score_add;
int Score;
int line;

int mX, mY, mA, mWait;
int mT, mNext;
int mKeyL, mKeyR, mKeyX, mKeyZ,mKeySpace;

bool mGameOver;

static int mImageHandle; //画像ハンドル格納用変数

int mBlock[7][4][4] = {
	{
		{0,0,0,0},		//青ブロック
		{1,1,1,1},
		{0,0,0,0},
		{0,0,0,0},
	},{
		{0,0,0,0},		//オレンジブロック
		{0,1,1,1},
		{0,1,0,0},
		{0,0,0,0},
	},{
		{0,0,0,0},		//黄色ブロック
		{0,1,1,0},
		{0,1,1,0},
		{0,0,0,0},
	},{
		{0,0,0,0},		//赤ブロック
		{1,1,0,0},
		{0,1,1,0},
		{0,0,0,0},
	},{
		{0,0,0,0},		//紫ブロック
		{1,1,1,0},
		{0,1,0,0},
		{0,0,0,0},
	},{
		{0,0,0,0},		//青ブロック
		{1,1,1,0},
		{0,0,1,0},
		{0,0,0,0},
	},{
		{0,0,0,0},		//緑ブロック
		{0,1,1,0},
		{1,1,0,0},
		{0,0,0,0},
	},
};

int mField[MAP_HEIGHT][MAP_WIDTH] = {	//フィールド
	{9,9,9,9,7,7,7,7,7,7,9,9,9,9},
	{9,8,7,7,7,7,7,7,7,7,7,7,8,9},
	{9,8,7,7,7,7,7,7,7,7,7,7,8,9},
	{9,8,7,7,7,7,7,7,7,7,7,7,8,9},
	{9,8,7,7,7,7,7,7,7,7,7,7,8,9},
	{9,8,7,7,7,7,7,7,7,7,7,7,8,9},
	{9,8,7,7,7,7,7,7,7,7,7,7,8,9},
	{9,8,7,7,7,7,7,7,7,7,7,7,8,9},
	{9,8,7,7,7,7,7,7,7,7,7,7,8,9},
	{9,8,7,7,7,7,7,7,7,7,7,7,8,9},
	{9,8,7,7,7,7,7,7,7,7,7,7,8,9},
	{9,8,7,7,7,7,7,7,7,7,7,7,8,9},
	{9,8,7,7,7,7,7,7,7,7,7,7,8,9},
	{9,8,7,7,7,7,7,7,7,7,7,7,8,9},
	{9,8,7,7,7,7,7,7,7,7,7,7,8,9},
	{9,8,7,7,7,7,7,7,7,7,7,7,8,9},
	{9,8,7,7,7,7,7,7,7,7,7,7,8,9},
	{9,8,7,7,7,7,7,7,7,7,7,7,8,9},
	{9,8,7,7,7,7,7,7,7,7,7,7,8,9},
	{9,8,7,7,7,7,7,7,7,7,7,7,8,9},
	{9,8,7,7,7,7,7,7,7,7,7,7,8,9},
	{9,8,7,7,7,7,7,7,7,7,7,7,8,9},
	{9,8,7,7,7,7,7,7,7,7,7,7,8,9},
	{9,8,8,8,8,8,8,8,8,8,8,8,8,9},
	{9,9,9,9,9,9,9,9,9,9,9,9,9,9},
};

bool put(int x, int y, int t, int a, bool s, bool test)	//ブロックを出現 (x軸,y軸,タイプ,角度,判定,判定)
{
	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			int p[] = { i,3 - j,3 - i,j };
			int q[] = { j,i,3 - j,3 - i };
			if (mBlock[t][q[a]][p[a]] == 0) {
				continue;
			}

			v = t;
			if (!s) {
				v = 7;
			}
			else if (mField[y + j][x + i] != 7) {
				return(false);
			}
			if (!test) {
				mField[y + j][x + i] = v;
			}
		}
	}
	return(true);
}

//初期化
void Game_Initialize() {
	//mImageHandle = LoadDivGraph("images/tile.png");

}
//終了処理
void Game_Finalize() {
	DeleteGraph(mImageHandle); //画像の解放
}
//更新
void Game_Update() {
	if (CheckHitKey(KEY_INPUT_ESCAPE) != 0) { //Escキーが押されていたら
		SceneMgr_ChangeScene(eScene_Menu);//シーンをメニューに変更

	}

	Timer();
	if(CheckHitKey(KEY_INPUT_DOWN)!=0){//下キーで高速落下
		WaitTimer(16);
	}
	else {//通常速度
		WaitTimer(50);
	}
	//WaitTimer(50);

}

void next() {//次のブロックをセット
	mX = 5;
	mY = 2;
	mT = mNext;
	mWait = WAIT;

	mA = 0;
	/*if (mKeyX > 0)mA = 3;
	if (mKeyZ > 0)mA = 1;*/
	if (mKeySpace > 0)mA = 1;

	if (!put(mX, mY, mT, mA, true, false)) {	//上に詰まったらゲームオーバー
		mGameOver = true;
	}

	put(5, -1, mNext, 0, false, false);	//次のブロックをセット
	mNext = GetRand(6);
	put(5, -1, mNext, 0, true, false);
}


void wait() {
	if (--mWait == 0) {	//0になったら次のブロックを出す
		next();
	}

	if (mWait == WAIT / 2 - 1) {	//接地したら
		for (int y = 22; y > 2; y--) {
			int n = 0;
			for (int x = 2; x < 12; x++) {
				if (mField[y][x] < 7) {
					n++;
				}
			}
			if (n != 10) {
				continue;
			}
			for (int x = 2; x < 12; x++) {
				mField[y][x] = 10;	//揃ってたら消す　10は消えるエフェクトのブロック
				Score_add += 10;
				line += 1;
			}
		}
	}
	if (mWait == 1) {
		for (int y = 22; y > 2; y--) {
			if (mField[y][2] != 10) {	//10じゃなかったら飛ばす
				continue;
			}
			mWait = WAIT / 2 - 2;
			for (int i = y; i > 3; i--) {
				for (int x = 2; x < 12; x++) {
					mField[i][x] = mField[i - 1][x];	//消したら上のブロックが落ちてくる
				}
			}
			for (int x = 2; x < 12; x++) {
				mField[3][x] = 7;			//一番のブロックは必ずなくなるから7にする
			}
			y++;
		}
	}
}

void tick() {
	if (mGameOver) {
		return;
	}
	if (mWait <= WAIT / 2) {	//接地して0.5秒は動かさない
		WaitTimer(1);
		wait();
		return;
	}

	put(mX, mY, mT, mA, false, false);

	int a = mA;
	//if (mKeyX == 1) a--;
	//if (mKeyZ == 1) a++;
	if (mKeySpace == 1) a++;
	a &= 3;
	if (put(mX, mY, mT, a, true, true)) {
		mA = a;
	}
	int x = mX;
	if (mKeyL == 1 || mKeyL > 20)	x--;
	if (mKeyR == 1 || mKeyR > 20)	x++;
	if (put(x, mY, mT, mA, true, true)) {
		mX = x;
	}
	if (put(mX, mY + 1, mT, mA, true, true)) {
		mY++;
		mWait = WAIT;
	}
	else {	//地面についたとき
		mWait--;
	}

	put(mX, mY, mT, mA, true, false);
}

void onPaint() {
	s = TILE_SIZE / 1.375;
	for (/*int*/ y = 0; y < MAP_HEIGHT; y++) {		//フィールド
		for (/*int */x = 0; x < MAP_WIDTH; x++) {
			DrawExtendGraph(x * s, y * s, x * s + s, y * s + s, Tile[mField[y][x]], FALSE);	//// 画像を画面いっぱいに描画

		}
	}

	if (mGameOver) {
		SetFontSize(32);
		DrawString(188, 128, "GAME OVER", GetColor(0xff, 0xff, 0xff));
	}
	DrawFormatString(500, 100, 0xFFFFFF, "SCORE :  %d",Score_add);
	DrawFormatString(500, 200, 0xFFFFFF,"LINE :  %d", line / 10);
}

void Timer() {
	if (CheckHitKey(KEY_INPUT_LEFT))
	{
		mKeyL++;
	}
	else {
		mKeyL = 0;
	}
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		mKeyR++;
	}
	else {
		mKeyR = 0;
	}
	/*if (CheckHitKey(KEY_INPUT_X))
	{
		mKeyX++;
	}
	else {
		mKeyX = 0;
	}
	if (CheckHitKey(KEY_INPUT_Z))
	{
		mKeyZ++;
	}
	else {
		mKeyZ = 0;
	}*/
	if (CheckHitKey(KEY_INPUT_SPACE))
	{
		mKeySpace++;
	}
	else {
		mKeySpace = 0;
	}
	tick();
	onPaint();
}

//描画
void Game_Draw() {

	LoadDivGraph("images/tileHD2.png", 11, 11, 1, TILE_SIZE, TILE_SIZE, Tile);	//画像分割
	mNext = GetRand(6);
	next();

}


int Game_GetScore() {

	//return gameScore;
	return Score_add;
}

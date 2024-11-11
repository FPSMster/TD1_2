#include <Novice.h>
#define _USE_MATH_DEFINES
#include<math.h>
#include<time.h>

const char kWindowTitle[] = "GC1B 03 クドウ　マコト　タイトル";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	
	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	srand(unsigned int(time(nullptr)));

	struct Vector2{
		float x;
		float y;
	};

	struct Player{
		Vector2 pos;
		float speed;
		Vector2 velocity;
		bool isBulletShot;
	};

	Player player{
		{100.0f,500.0f},
		{10.0f},
		{0.0f,0.0f}
	};

	Player attack[6];
	for (int i = 0; i < 6; i++) {
		attack[i].pos.x = 100.0f;
		attack[i].pos.y = 500.0f;
		attack[i].speed = 15.0f;
		attack[i].isBulletShot = false;

	}
	struct Enemy{
		Vector2 pos;
		float speed;
		Vector2 velocity;
		bool isAlive;
		bool isBulletShot;
	};

	int life = 3;
	int enemyLife = 100;
	int level = 0;

	Enemy enemy{
		{1000.0f,360.0f},
		{5.0f},
		{0.0f,0.0f,},
		{true}
	};

	Enemy enemyAttack[20];
	for (int i = 0; i < 20; i++) {
		enemyAttack[i].pos.x = 1000.0f;
		enemyAttack[i].pos.y = 360.0f;
		enemyAttack[i].speed = 20.0f;
		enemyAttack[i].isBulletShot = false;
	}

	enum Shene {
		TITLE,
		GAME,
		CLEAR,
		GAMEOVER
	};
	int gameShene = TITLE;
	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		switch (gameShene){
		case TITLE:
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				gameShene = GAME;
			}
			break;

		case GAME:
			if (enemyLife <= 0) {
				gameShene = CLEAR;
			}
			if (life <= 0) {
				gameShene = GAMEOVER;
			}
			break;
		case CLEAR:
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				gameShene = TITLE;
			}
			break;
		case GAMEOVER:
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				gameShene = TITLE;
			}
			break;
		}


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		


		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}

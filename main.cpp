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



		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		Novice::DrawEllipse(100, 100, 10, 10, 0.0f, WHITE, kFillModeSolid);
		Novice::DrawBox(50, 50, 50, 50, 1.0f, WHITE, kFillModeSolid);
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

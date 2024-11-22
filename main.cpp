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

	struct EnemyFollowers {
		Vector2 pos;
		float speed;
		bool isAlive;
		bool isBulletShot;
	};

	int life = 3;
	/*int enemyLife = 100;*/
	//int level = 0;

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

	EnemyFollowers enemyFollowers[5];
	for (int i = 0; i < 5; i++) {
		enemyFollowers[i].pos.x = 600.0f + i * 64.0f;
		enemyFollowers[i].pos.y = 555.0f-i*5;
		enemyFollowers[i].speed = 5.0f;
		enemyFollowers[i].isAlive = false;
		enemyFollowers[i].isBulletShot = false;
	}

	enum Shene {
		TITLE,
		GAMEdoragon,
		GAMEookami,
		GAMEgoburinn,
		GAMErizadoman,
		GAMEdoragon2,
		CLEAR,
		GAMEOVER
	};
	int gameShene = TITLE;

	//敵の動き
	float theta = float(M_PI) / 30.0f;
	//float amplitude = 300.0f;
	float move = -500.0f;
	int motionCount = 0;

	enemy.velocity.x = 5.0f;
	unsigned int currentTime = static_cast<unsigned int>(time(nullptr));
	srand(currentTime);
	int randX = rand() % 21 - 10;
	//敵のスポーン
	
	/*enum Boss {
		Boss1,
		Boss2,
		Boss3,
		Boss4,
		Boss5
	};
	int appearEnemy = Boss1;*/

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	int doragonhandle = Novice::LoadTexture("./Resources/doragon.png");
	int ookamihandle = Novice::LoadTexture("./Resources/ookami.png");
	int goburinhanndle = Novice::LoadTexture("./Resources/goburinn.png");
	int kodomogoburinnhandle = Novice::LoadTexture("./Resources/kodomogoburinn.png");
	int rizadomanhandle = Novice::LoadTexture("./Resources/reza-doman.png");
	int doragon2handle = Novice::LoadTexture("./Resources/doragon2.png");

	//背景
	int backGroundHandle = Novice::LoadTexture("./Resources/mori.png");

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
				gameShene = GAMEdoragon;
			}
			break;

		case GAMEdoragon:

			///ドラゴンの処理

			if (theta <= 5*float(M_PI)) {
				theta += float(M_PI) / 150.0f;
				enemy.pos.y = (4 / float(M_PI) * sinf(theta) +
					4 / 3 * float(M_PI) * sinf(3 * theta) + 4 / 5 * float(M_PI) * sinf(5 * theta)) * 50 + 360.0f;
			
			} else {

				enemy.pos.x += move;
				move = -move;
				theta = 0.0f;
				
			}


			///
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				gameShene = GAMEookami;
			}


			/*if (enemyLife <= 0) {
				gameShene = CLEAR;
			}*/
			if (life <= 0) {
				gameShene = GAMEOVER;
			}
			break;

		case GAMEookami:
			//狼

			enemy.pos.y = 650.0f;

			motionCount++;
		
			if (motionCount >= 210 && motionCount<=300) {
				randX = rand() % 21 - 10;
			} else {
				randX = 0;
			}
			

			if (motionCount >= 300) {
				enemy.pos.x -= enemy.speed;
				enemy.speed +=enemy.velocity.x;

			}
			if (enemy.pos.x <= 128.0f) {
				enemy.speed = 0.0f;
				enemy.pos.x = 1000.0f;
				motionCount = 0;
			}


			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				gameShene = GAMEgoburinn;
				motionCount = 0;
			}

			break;

		case GAMEgoburinn:
			//ゴブリン
			motionCount++;

			if (motionCount >= 100 ) {
				for (int i = 0; i < 5; i++) {
					if (enemyFollowers[i].isAlive == false) {
						enemyFollowers[i].isAlive = true;
					}
				}
			}
			for (int i = 0; i < 5; i++) {

				if (enemyFollowers[i].pos.y <= 650.0f && enemyFollowers[i].isAlive) {
					enemyFollowers[i].pos.y +=enemyFollowers[i].speed;
				}
			}


			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				gameShene = GAMErizadoman;
			}

			break;
		case GAMErizadoman:
			//リザードマン
			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				gameShene = GAMEdoragon2;
			}

			break;
		case GAMEdoragon2:
			//ドラゴン２

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
		
		
			
		

		switch (gameShene) {
		case TITLE:
			Novice::ScreenPrintf(0, 0, "TITLE");
			break;

		case GAMEdoragon:
			Novice::DrawSprite(0, 0, backGroundHandle, 1.0f, 1.0f, 0.0f, WHITE);

			Novice::ScreenPrintf(0, 0, "DORAGON");

			Novice::DrawSprite(static_cast<int>(enemy.pos.x -64.0f),
				static_cast<int>(enemy.pos.y-64.0f), doragonhandle, 1.0f, 1.0f, 0.0f, WHITE);

			break;
		case GAMEookami:
			Novice::DrawSprite(0, 0, backGroundHandle, 1.0f, 1.0f, 0.0f, WHITE);
			//狼
			Novice::ScreenPrintf(0, 0, "OOKAMI");
			Novice::ScreenPrintf(0, 20, "%d", motionCount);
			Novice::DrawSprite(static_cast<int>(enemy.pos.x - 64.0f+randX),
				static_cast<int>(enemy.pos.y - 64.0f), ookamihandle, 1.0f, 1.0f, 0.0f, WHITE);
			break;
		case GAMEgoburinn:
			Novice::DrawSprite(0, 0, backGroundHandle, 1.0f, 1.0f, 0.0f, WHITE);
			//
			Novice::ScreenPrintf(0, 20, "%d", motionCount);
			//ゴブリン
			Novice::ScreenPrintf(0, 0, "GOBURINN");
			Novice::DrawSprite(static_cast<int>(enemy.pos.x - 64.0f),
				static_cast<int>(enemy.pos.y - 64.0f), goburinhanndle, 1.0f, 1.0f, 0.0f, WHITE);
			//子供
			for (int i = 0; i < 5; i++) {
				if (enemyFollowers[i].isAlive) {
					Novice::DrawSprite(static_cast<int>(enemyFollowers[i].pos.x),
						static_cast<int>(enemyFollowers[i].pos.y), kodomogoburinnhandle, 1.0f, 1.0f, 0.0f, WHITE);
				}
			}
			

			break;
		case GAMErizadoman:
			Novice::DrawSprite(0, 0, backGroundHandle, 1.0f, 1.0f, 0.0f, WHITE);
			//リザードマン
			Novice::ScreenPrintf(0, 0, "RIZA-DOMAN");
			Novice::DrawSprite(static_cast<int>(enemy.pos.x - 64.0f),
				static_cast<int>(enemy.pos.y - 64.0f), rizadomanhandle, 1.0f, 1.0f, 0.0f, WHITE);
			break;
		case GAMEdoragon2:
			Novice::DrawSprite(0, 0, backGroundHandle, 1.0f, 1.0f, 0.0f, WHITE);
			//ドラゴン２
			Novice::ScreenPrintf(0, 0, "DORAGON2");
			Novice::DrawSprite(static_cast<int>(enemy.pos.x - 64.0f),
				static_cast<int>(enemy.pos.y - 64.0f), doragon2handle, 1.0f, 1.0f, 0.0f, WHITE);
			break;
		case CLEAR:
			
			break;
		case GAMEOVER:
			
			break;
		}

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

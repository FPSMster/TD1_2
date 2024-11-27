#include <Novice.h>
#define _USE_MATH_DEFINES
#include<math.h>
#include<time.h>

const char kWindowTitle[] = "GC1B 03 クドウ　マコト　タイトル";

struct Vector2 {
	float x;
	float y;
};


struct Player {
	Vector2 pos;
	float radius;
	int speed;
	int bulletCoolTime;
	float velocity;
	float gravity;
	float jumpPower;
	bool isJumping;
};

struct Enemy {
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
	float distance;
};

float collision(float  a, float b) {
	return sqrtf(a * a + b * b);
}

void Jump(Player& player)
{
	if (!player.isJumping) {
		player.velocity = -player.jumpPower;
		player.isJumping = true;
	}
}


void ApplyGravity(Player& player)
{
	player.velocity += player.gravity;
	player.pos.y += player.velocity;
	if (player.pos.y + player.radius / 2 >= 600.0f) {
		player.pos.y = 600.0f - player.radius / 2;
		player.isJumping = false;
		player.velocity = 0.0f;
	}
}

struct Bullet {
	Vector2 pos;
	float radius;
	float speed;
	int isShoot;
	Vector2 direction;
	float enemyDistance;
};

struct Mouse {
	int posX;
	int posY;
	Vector2 direction;
};

Vector2 Normalize(const Vector2& vector) {
	float length = sqrtf(vector.x * vector.x + vector.y * vector.y);

	if (length != 0) {
		return { vector.x / length, vector.y / length };
	} else {
		return { 0.0f, 0.0f };
	}
}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	
	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	srand(unsigned int(time(nullptr)));


	//プレイヤーの初期化
	Player player = {};

	player.pos.x = 100.0f;
	player.pos.y = 568.0f;
	player.radius = 64.0f;
	player.velocity = 0.0f;
	player.gravity = 0.8f;
	player.jumpPower = 20.0f;
	player.speed = 10;
	player.bulletCoolTime = 0;
	player.isJumping = false;



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
		enemyAttack[i].speed = 5.0f;
		enemyAttack[i].isBulletShot = false;
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
	int enemyRandX = rand() % 21 - 10;
	int randX = rand() % 21 - 10;

	//無敵時間
	int invincible = 60;
	int ivincibleFlag = false;

	//自機の当たり判定
	float playerDistance = collision((enemy.pos.x - player.pos.x), (enemy.pos.y - player.pos.y));

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
	//クロスヘア
	int pointHandle = Novice::LoadTexture("./Resources./pointo.png");
	//プレイヤー
	int playerHidariHandle = Novice::LoadTexture("./Resources./playerHidari.png");
	int playerMigiHandle = Novice::LoadTexture("./Resources./playerMigi.png");

	int titleHandle = Novice::LoadTexture("./Resources./title.png");
	int scoreHandle = Novice::LoadTexture("./Resources./gameover.png");

	int zangekiHandle[6];
	zangekiHandle[0] = Novice::LoadTexture("./Resources./zangeki1.png");
	zangekiHandle[1] = Novice::LoadTexture("./Resources./zangeki2.png");
	zangekiHandle[2] = Novice::LoadTexture("./Resources./zangeki3.png");
	zangekiHandle[3] = Novice::LoadTexture("./Resources./zangeki4.png");
	zangekiHandle[4] = Novice::LoadTexture("./Resources./zangeki5.png");
	zangekiHandle[5] = Novice::LoadTexture("./Resources./zangeki6.png");

	int zangekiCount = 0;

	int mahouHandle[4];
	mahouHandle[0] = Novice::LoadTexture("./Resources./mahou1.png");
	mahouHandle[1] = Novice::LoadTexture("./Resources./mahou2.png");
	mahouHandle[2] = Novice::LoadTexture("./Resources./mahou3.png");
	mahouHandle[3] = Novice::LoadTexture("./Resources./mahou4.png");

	int mahouCount = 0;

	int bressHandle[4];
	bressHandle[0] = Novice::LoadTexture("./Resources./doragonnBress1.png");
	bressHandle[1] = Novice::LoadTexture("./Resources./doragonnBress2.png");
	bressHandle[2] = Novice::LoadTexture("./Resources./doragonnBress3.png");
	bressHandle[3] = Novice::LoadTexture("./Resources./doragonnBress4.png");

	//弾の初期化
	Bullet bullet[3];
	for (int i = 0; i < 3; i++) {
		bullet[i].pos.x = -128;
		bullet[i].pos.y = -128;
		bullet[i].radius = 32;
		bullet[i].speed = 10;
		bullet[i].isShoot = false;
		bullet[i].direction.x = 0;
		bullet[i].direction.y = 0;
		//球と敵の当たり判定
		bullet[i].enemyDistance = collision((enemy.pos.x - bullet[i].pos.x), (enemy.pos.y - bullet[i].pos.y));
	}

	EnemyFollowers enemyFollowers[5];
	for (int i = 0; i < 5; i++) {
		enemyFollowers[i].pos.x = 600.0f + i * 64.0f;
		enemyFollowers[i].pos.y = 555.0f - i * 5;
		enemyFollowers[i].speed = 5.0f;
		enemyFollowers[i].isAlive = false;
		enemyFollowers[i].isBulletShot = false;
		for (int j = 0; j < 3; j++) {
			enemyFollowers[i].distance = collision((bullet[j].pos.x - enemyFollowers[i].pos.x),
				(bullet[j].pos.y - enemyFollowers[i].pos.y));
		}
	}

	//クロスヘアの初期化
	Mouse mouse = {};

	mouse.posX = 0;
	mouse.posY = 0;
	mouse.direction.x = 0;
	mouse.direction.y = 0;

	//BGM
	int bgmHandle= Novice::LoadAudio("./Resources/Mirroring.mp3");
	int playHandle = -1;

	//SCORE
	int isScore = false;
	int score = 0;

	int numberHandle[10];
	numberHandle[0] = Novice::LoadTexture("./Resources/0.png");
	numberHandle[1] = Novice::LoadTexture("./Resources/1.png");
	numberHandle[2] = Novice::LoadTexture("./Resources/2.png");
	numberHandle[3] = Novice::LoadTexture("./Resources/3.png");
	numberHandle[4] = Novice::LoadTexture("./Resources/4.png");
	numberHandle[5] = Novice::LoadTexture("./Resources/5.png");
	numberHandle[6] = Novice::LoadTexture("./Resources/6.png");
	numberHandle[7] = Novice::LoadTexture("./Resources/7.png");
	numberHandle[8] = Novice::LoadTexture("./Resources/8.png");
	numberHandle[9] = Novice::LoadTexture("./Resources/9.png");

	int numberArray[3]{};

	numberArray[0] = score / 100;
	score %= 100;
	numberArray[1] = score / 10;
	score %= 10;
	numberArray[2] = score / 1;
	score %= 1;

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
		
		if (!Novice::IsPlayingAudio(playHandle) || playHandle == -1) {
			playHandle = Novice::PlayAudio(bgmHandle, true, 0.4f);
		}
		
		switch (gameShene){
		case TITLE:
			life = 3;
			level = 0;
			enemy.pos.x = 1000.0f;
			enemy.pos.y = 600.0f;
			theta = float(M_PI) / 15.0f;
			invincible = 0;
			player.pos.x = 100.0f;
			motionCount = 0;
			ivincibleFlag = false;
			for (int i = 0; i < 20; i++) {
				enemyAttack[i].pos.x = enemy.pos.x;
				enemyAttack[i].pos.y = enemy.pos.y;
				enemyAttack[i].speed = 5.0f;
				enemyAttack[i].isBulletShot = false;
			}
			for (int i = 0; i < 8; i++) {
				
			}
			for (int i = 0; i < 5; i++) {
				enemyFollowers[i].isAlive = false;
				enemyFollowers[i].pos.x = 600.0f + i * 64.0f;
				enemyFollowers[i].pos.y = 505.0f - i * 5;

			}

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				gameShene = GAMEdoragon;
			}
			break;

		case GAMEdoragon:

			Novice::GetMousePosition(&mouse.posX, &mouse.posY);
			mouse.direction.x = mouse.posX - player.pos.x;
			mouse.direction.y = mouse.posY - player.pos.y;
			mouse.direction = Normalize(mouse.direction);

			Novice::SetMouseCursorVisibility(0);



			if (Novice::IsTriggerMouse(0)) {
				if (player.bulletCoolTime <= 0) {
					for (int i = 0; i < 3; i++) {
						if (!bullet[i].isShoot) {
							bullet[i].isShoot = true;
							bullet[i].pos.x = player.pos.x;
							bullet[i].pos.y = player.pos.y;
							bullet[i].direction.x = mouse.direction.x;
							bullet[i].direction.y = mouse.direction.y;
							player.bulletCoolTime = 10;
							break;
						}
					}
				}
			}



			if (player.bulletCoolTime > 0) {
				player.bulletCoolTime--;
			}


			for (int i = 0; i < 3; i++) {
				if (bullet[i].isShoot) {
					bullet[i].pos.x += bullet[i].speed * bullet[i].direction.x;
					bullet[i].pos.y += bullet[i].speed * bullet[i].direction.y;
					if (bullet[i].pos.y <= 0 - bullet[i].radius / 2) {
						bullet[i].isShoot = false;
					}
					if (bullet[i].pos.y >= 720 - bullet[i].radius / 2) {
						bullet[i].isShoot = false;
					}
					if (bullet[i].pos.x <= 0 - bullet[i].radius / 2) {
						bullet[i].isShoot = false;
					}
					if (bullet[i].pos.x >= 1280 - bullet[i].radius / 2) {
						bullet[i].isShoot = false;
					}

					//自機の球と敵の当たり判定
					bullet[i].enemyDistance = collision((enemy.pos.x - bullet[i].pos.x), (enemy.pos.y - bullet[i].pos.y));
					if (bullet[i].enemyDistance <= bullet[i].radius + 64) {
						bullet[i].isShoot = false;
						enemyLife -= 10;
					}

					
				}
			}



			if (keys[DIK_W] && !preKeys[DIK_W]) {

				Jump(player);
			}
			if (keys[DIK_D]) {
				player.pos.x = player.pos.x + player.speed;
			}
			if (keys[DIK_A]) {
				player.pos.x = player.pos.x - player.speed;
			}

			ApplyGravity(player);

			///ドラゴンの処理

			motionCount++;

			if (theta <= 5*float(M_PI)) {
				theta += float(M_PI) / 150.0f;
				enemy.pos.y = (4 / float(M_PI) * sinf(theta) +
					4 / 3 * float(M_PI) * sinf(3 * theta) + 4 / 5 * float(M_PI) * sinf(5 * theta)) * 50 + 360.0f;
			
			} else {

				enemy.pos.x += move;
				move = -move;
				theta = 0.0f;
				
			}

			if (motionCount > 360  ) {
				for (int i = 0; i < 20; i++) {
					if (enemyAttack[i].isBulletShot == false) {
						enemyAttack[i].isBulletShot = true;
						enemyAttack[i].pos.x = enemy.pos.x;
						enemyAttack[i].pos.y = enemy.pos.y;
					}
					if (enemyAttack[i].isBulletShot) {
						enemyAttack[i].pos.y+=enemyAttack[i].speed;
						enemyAttack[i].pos.x -= enemyAttack[i].speed + i*5;

					}

					playerDistance = collision((enemyAttack[i].pos.x - player.pos.x), (enemyAttack[i].pos.y - player.pos.y));
					if (playerDistance <= 16 + player.radius) {
						
						enemyAttack[i].isBulletShot = false;
						if (ivincibleFlag == false) {
							life--;
							ivincibleFlag = true;
						}
						
					}
				}
			}

			if (motionCount >= 450 ) {
				for (int i = 0; i < 20; i++) {
					enemyAttack[i].isBulletShot = false;
				}
				motionCount = 0;
			}


			mahouCount = ++mahouCount % 60;

			if (enemyLife<=0) {
				enemy.pos.x = 1000.0f;
				motionCount = 0;
				level++;
				life = 3;
				enemyLife = 100 + level * 10;
				for (int i = 0; i < 20; i++) {
					enemyAttack[i].isBulletShot = false;
				}
				gameShene = GAMEookami;

			}
			playerDistance = collision((enemy.pos.x - player.pos.x), (enemy.pos.y - player.pos.y));
			if (playerDistance <= 64 + player.radius ) {
				if (ivincibleFlag == false) {
					life--;
					ivincibleFlag = true;
				}
			}
			if (ivincibleFlag) {
				randX = rand() % 21 - 10;
				invincible--;
			} else {
				randX = 0;
			}

			if (invincible == 0) {
				invincible = 60;
				ivincibleFlag = false;
			}



			/*if (enemyLife <= 0) {
				gameShene = CLEAR;
			}*/
			if (life <= 0) {
				isScore = true;
				gameShene = GAMEOVER;
				
			}
			break;

			

		case GAMEookami:
			//自機

			Novice::GetMousePosition(&mouse.posX, &mouse.posY);
			mouse.direction.x = mouse.posX - player.pos.x;
			mouse.direction.y = mouse.posY - player.pos.y;
			mouse.direction = Normalize(mouse.direction);

			if (Novice::IsTriggerMouse(0)) {
				if (player.bulletCoolTime <= 0) {
					for (int i = 0; i < 3; i++) {
						if (!bullet[i].isShoot) {
							bullet[i].isShoot = true;
							bullet[i].pos.x = player.pos.x;
							bullet[i].pos.y = player.pos.y;
							bullet[i].direction.x = mouse.direction.x;
							bullet[i].direction.y = mouse.direction.y;
							player.bulletCoolTime = 10;
							break;
						}
					}
				}
			}

			if (player.bulletCoolTime > 0) {
				player.bulletCoolTime--;
			}


			for (int i = 0; i < 3; i++) {
				if (bullet[i].isShoot) {
					bullet[i].pos.x += bullet[i].speed * bullet[i].direction.x;
					bullet[i].pos.y += bullet[i].speed * bullet[i].direction.y;
					if (bullet[i].pos.y <= 0 - bullet[i].radius / 2) {
						bullet[i].isShoot = false;
					}
					if (bullet[i].pos.y >= 720 - bullet[i].radius / 2) {
						bullet[i].isShoot = false;
					}
					if (bullet[i].pos.x <= 0 - bullet[i].radius / 2) {
						bullet[i].isShoot = false;
					}
					if (bullet[i].pos.x >= 1280 - bullet[i].radius / 2) {
						bullet[i].isShoot = false;
					}

					//自機の球と敵の当たり判定
					bullet[i].enemyDistance = collision((enemy.pos.x - bullet[i].pos.x), (enemy.pos.y - bullet[i].pos.y));
					if (bullet[i].enemyDistance <= bullet[i].radius + 64) {
						bullet[i].isShoot = false;
						enemyLife -= 10;
					}
				}
			}

			if (keys[DIK_W] && !preKeys[DIK_W]) {

				Jump(player);
			}
			if (keys[DIK_D]) {
				player.pos.x = player.pos.x + player.speed;
			}
			if (keys[DIK_A]) {
				player.pos.x = player.pos.x - player.speed;
			}
			ApplyGravity(player);



			//狼
			enemy.pos.y = 600.0f;

			motionCount++;
		
			if (motionCount >= 210 && motionCount<=300) {
				enemyRandX = rand() % 21 - 10;
			} else {
				enemyRandX = 0;
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


			if(enemyLife <= 0) {
				enemy.pos.x = 1000.0f;
				motionCount = 0;
				level++;
				enemyLife = 100 + level * 10;
				life = 3;
				theta = float(M_PI) / 30.0f;
				enemy.speed = 5.0f;
				gameShene = GAMEgoburinn;


			}

			playerDistance = collision((enemy.pos.x - player.pos.x), (enemy.pos.y - player.pos.y));
			if (playerDistance <= 64 + player.radius) {
				if (ivincibleFlag == false) {
					life--;
					ivincibleFlag = true;
				}
			}
			if (ivincibleFlag) {
				randX = rand() % 21 - 10;
				invincible--;
			} else {
				randX = 0;
			}

			if (invincible == 0) {
				invincible = 60;
				ivincibleFlag = false;
			}
			if (life <= 0) {
				isScore = true;
				gameShene = GAMEOVER;
			}
		

			/*if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				enemy.pos.x = 1000.0f;
				gameShene = GAMEgoburinn;
				motionCount = 0;
			}*/

			mahouCount = ++mahouCount % 60;

			break;

		case GAMEgoburinn:
			//自機

			Novice::GetMousePosition(&mouse.posX, &mouse.posY);
			mouse.direction.x = mouse.posX - player.pos.x;
			mouse.direction.y = mouse.posY - player.pos.y;
			mouse.direction = Normalize(mouse.direction);

			if (Novice::IsTriggerMouse(0)) {
				if (player.bulletCoolTime <= 0) {
					for (int i = 0; i < 3; i++) {
						if (!bullet[i].isShoot) {
							bullet[i].isShoot = true;
							bullet[i].pos.x = player.pos.x;
							bullet[i].pos.y = player.pos.y;
							bullet[i].direction.x = mouse.direction.x;
							bullet[i].direction.y = mouse.direction.y;
							player.bulletCoolTime = 10;
							break;
						}
					}
				}
			}

			if (player.bulletCoolTime > 0) {
				player.bulletCoolTime--;
			}


			for (int i = 0; i < 3; i++) {
				if (bullet[i].isShoot) {
					bullet[i].pos.x += bullet[i].speed * bullet[i].direction.x;
					bullet[i].pos.y += bullet[i].speed * bullet[i].direction.y;
					if (bullet[i].pos.y <= 0 - bullet[i].radius / 2) {
						bullet[i].isShoot = false;
					}
					if (bullet[i].pos.y >= 720 - bullet[i].radius / 2) {
						bullet[i].isShoot = false;
					}
					if (bullet[i].pos.x <= 0 - bullet[i].radius / 2) {
						bullet[i].isShoot = false;
					}
					if (bullet[i].pos.x >= 1280 - bullet[i].radius / 2) {
						bullet[i].isShoot = false;
					}

					//自機の球と敵の当たり判定
					bullet[i].enemyDistance = collision((enemy.pos.x - bullet[i].pos.x), (enemy.pos.y - bullet[i].pos.y));
					if (bullet[i].enemyDistance <= bullet[i].radius + 64) {
						bullet[i].isShoot = false;
						enemyLife -= 10;
					}
				}
			}

			if (keys[DIK_W] && !preKeys[DIK_W]) {

				Jump(player);
			}
			if (keys[DIK_D]) {
				player.pos.x = player.pos.x + player.speed;
			}
			if (keys[DIK_A]) {
				player.pos.x = player.pos.x - player.speed;
			}
			ApplyGravity(player);


			//ゴブリン
			motionCount++;


			if (motionCount >= 300 ) {
				for (int i = 0; i < 5; i++) {
					if (enemyFollowers[i].isAlive == false) {
						enemyFollowers[i].isAlive = true;
					}
					for (int j = 0; j < 3; j++) {
						
						enemyFollowers[i].distance = collision((bullet[j].pos.x - enemyFollowers[i].pos.x),
							(bullet[j].pos.y - enemyFollowers[i].pos.y));

						if (enemyFollowers[i].distance <= bullet[j].radius + 32) {
							bullet[j].isShoot = false;
							bullet[j].pos.x = player.pos.x;
							enemyFollowers[i].pos.x = -32;
							
						}

						
					}

					
				}
			}



			for (int i = 0; i < 5; i++) {

				if (enemyFollowers[i].pos.y <= 632.0f && enemyFollowers[i].isAlive) {
					enemyFollowers[i].pos.y += enemyFollowers[i].speed;
				}

				if (motionCount>=360){
					if (enemyFollowers[i - 1].pos.x <= 500.0f) {
						enemyFollowers[i].pos.x -= enemyFollowers[i].speed;
					}
				}

				if (enemyFollowers[i].isAlive) {
					playerDistance = collision((enemyFollowers[i].pos.x - player.pos.x), (enemyFollowers[i].pos.y - player.pos.y));
				}
				if (playerDistance <= 64 + player.radius) {
					if (ivincibleFlag == false) {
						life--;
						ivincibleFlag = true;
					}
				}

			}

			if (enemyFollowers[4].pos.x + 32 <= 0) {
				for (int i= 0; i < 5; i++) {
					enemyFollowers[i].isAlive = false;
					enemyFollowers[i].pos.x = 600.0f + i * 64.0f;
					enemyFollowers[i].pos.y = 505.0f - i * 5;
						
				}
				motionCount = 0;
			}


			if (enemyLife <= 0) {
				enemy.pos.x = 1000.0f;
				motionCount = 0;
				level++;
				enemyLife = 100 + level * 10;
				life = 3;
				gameShene = GAMErizadoman;


			}

			playerDistance = collision((enemy.pos.x - player.pos.x), (enemy.pos.y - player.pos.y));
			if (playerDistance <= 64 + player.radius) {
				if (ivincibleFlag == false) {
					life--;
					ivincibleFlag = true;
				}
			}
			if (ivincibleFlag) {
				randX = rand() % 21 - 10;
				invincible--;
			} else {
				randX = 0;
			}

			if (invincible == 0) {
				invincible = 60;
				ivincibleFlag = false;
			}
			if (life <= 0) {
				isScore = true;
				gameShene = GAMEOVER;
			}

			
			mahouCount = ++mahouCount % 60;

			break;
		case GAMErizadoman:
			//リザードマン

			Novice::GetMousePosition(&mouse.posX, &mouse.posY);
			mouse.direction.x = mouse.posX - player.pos.x;
			mouse.direction.y = mouse.posY - player.pos.y;
			mouse.direction = Normalize(mouse.direction);

			if (Novice::IsTriggerMouse(0)) {
				if (player.bulletCoolTime <= 0) {
					for (int i = 0; i < 3; i++) {
						if (!bullet[i].isShoot) {
							bullet[i].isShoot = true;
							bullet[i].pos.x = player.pos.x;
							bullet[i].pos.y = player.pos.y;
							bullet[i].direction.x = mouse.direction.x;
							bullet[i].direction.y = mouse.direction.y;
							player.bulletCoolTime = 10;
							break;
						}
					}
				}
			}

			if (player.bulletCoolTime > 0) {
				player.bulletCoolTime--;
			}


			for (int i = 0; i < 3; i++) {
				if (bullet[i].isShoot) {
					bullet[i].pos.x += bullet[i].speed * bullet[i].direction.x;
					bullet[i].pos.y += bullet[i].speed * bullet[i].direction.y;
					if (bullet[i].pos.y <= 0 - bullet[i].radius / 2) {
						bullet[i].isShoot = false;
					}
					if (bullet[i].pos.y >= 720 - bullet[i].radius / 2) {
						bullet[i].isShoot = false;
					}
					if (bullet[i].pos.x <= 0 - bullet[i].radius / 2) {
						bullet[i].isShoot = false;
					}
					if (bullet[i].pos.x >= 1280 - bullet[i].radius / 2) {
						bullet[i].isShoot = false;
					}

					//自機の球と敵の当たり判定
					bullet[i].enemyDistance = collision((enemy.pos.x - bullet[i].pos.x), (enemy.pos.y - bullet[i].pos.y));
					if (bullet[i].enemyDistance <= bullet[i].radius + 64) {
						bullet[i].isShoot = false;
						enemyLife -= 10;
					}
				}
			}

			if (keys[DIK_W] && !preKeys[DIK_W]) {

				Jump(player);
			}
			if (keys[DIK_D]) {
				player.pos.x = player.pos.x + player.speed;
			}
			if (keys[DIK_A]) {
				player.pos.x = player.pos.x - player.speed;
			}
			ApplyGravity(player);

			motionCount++;

			if (motionCount >= 120) {
				enemy.pos.x -= 5.0f;
			}


			if (motionCount == 150) {
				enemy.velocity.y = -20;
				enemy.speed += enemy.velocity.y;
			}

			if (motionCount >= 150 && motionCount<=160) {
				enemy.pos.y += enemy.speed;
			}

			if (motionCount >= 160 && enemy.pos.y <= 600.0f) {
				enemy.pos.y += enemy.speed;
				enemy.speed += 0.9f;
			}

			
			if (enemy.pos.x <= -64) {
				enemy.pos.x = 1280 + 64;
				enemy.speed = 5.0f;
				motionCount = 0;
			}

			if (enemy.pos.x > 1000) {
				enemy.pos.y = 600.0f;
				enemy.pos.x -= 5.0f;
				motionCount = 0;
			}

			zangekiCount = ++zangekiCount % 60;
			
			mahouCount = ++mahouCount % 60;

			if (enemyLife <= 0) {
				enemy.pos.x = 1000.0f;
				motionCount = 0;
				level++;
				enemyLife = 100 + level * 10;
				life = 3;

				for (int i = 0; i < 20; i++) {
					enemyAttack[i].pos.x = enemy.pos.x;
					enemyAttack[i].pos.y = enemy.pos.y;
				}
				gameShene = GAMEdoragon2;


			}

			playerDistance = collision((enemy.pos.x - player.pos.x), (enemy.pos.y - player.pos.y));
			if (playerDistance <= 64 + player.radius) {
				if (ivincibleFlag == false) {
					life--;
					ivincibleFlag = true;
				}
			}
			if (ivincibleFlag) {
				randX = rand() % 21 - 10;
				invincible--;
			} else {
				randX = 0;
			}

			if (invincible == 0) {
				invincible = 60;
				ivincibleFlag = false;
			}
			if (life <= 0) {
				isScore = true;
				gameShene = GAMEOVER;
			}

			

			break;
		case GAMEdoragon2:
			//ドラゴン２

			Novice::GetMousePosition(&mouse.posX, &mouse.posY);
			mouse.direction.x = mouse.posX - player.pos.x;
			mouse.direction.y = mouse.posY - player.pos.y;
			mouse.direction = Normalize(mouse.direction);

			if (Novice::IsTriggerMouse(0)) {
				if (player.bulletCoolTime <= 0) {
					for (int i = 0; i < 3; i++) {
						if (!bullet[i].isShoot) {
							bullet[i].isShoot = true;
							bullet[i].pos.x = player.pos.x;
							bullet[i].pos.y = player.pos.y;
							bullet[i].direction.x = mouse.direction.x;
							bullet[i].direction.y = mouse.direction.y;
							player.bulletCoolTime = 10;
							break;
						}
					}
				}
			}

			if (player.bulletCoolTime > 0) {
				player.bulletCoolTime--;
			}


			for (int i = 0; i < 3; i++) {
				if (bullet[i].isShoot) {
					bullet[i].pos.x += bullet[i].speed * bullet[i].direction.x;
					bullet[i].pos.y += bullet[i].speed * bullet[i].direction.y;
					if (bullet[i].pos.y <= 0 - bullet[i].radius / 2) {
						bullet[i].isShoot = false;
					}
					if (bullet[i].pos.y >= 720 - bullet[i].radius / 2) {
						bullet[i].isShoot = false;
					}
					if (bullet[i].pos.x <= 0 - bullet[i].radius / 2) {
						bullet[i].isShoot = false;
					}
					if (bullet[i].pos.x >= 1280 - bullet[i].radius / 2) {
						bullet[i].isShoot = false;
					}

					//自機の球と敵の当たり判定
					bullet[i].enemyDistance = collision((enemy.pos.x - bullet[i].pos.x), (enemy.pos.y - bullet[i].pos.y));
					if (bullet[i].enemyDistance <= bullet[i].radius + 64) {
						bullet[i].isShoot = false;
						enemyLife -= 10;
					}
				}
			}

			if (keys[DIK_W] && !preKeys[DIK_W]) {

				Jump(player);
			}
			if (keys[DIK_D]) {
				player.pos.x = player.pos.x + player.speed;
			}
			if (keys[DIK_A]) {
				player.pos.x = player.pos.x - player.speed;
			}
			ApplyGravity(player);

			mahouCount = ++mahouCount % 60;


			//
			motionCount++;
			if (motionCount >= 90 && motionCount<=110 ) {
				enemy.pos.x -= enemy.speed;
				enemy.speed += enemy.velocity.x;
			}

			if (motionCount>=110) {
				enemy.speed = 0.0f;
				enemy.pos.x = 1000.0f;

				theta += float(M_PI) / 150.0f;
				enemy.pos.y = (4 / float(M_PI) * sinf(theta) +
					4 / 3 * float(M_PI) * sinf(3 * theta) + 4 / 5 * float(M_PI) * sinf(5 * theta)) * 50 + 360.0f;
			
			}

			if (motionCount > 360) {
				for (int i = 0; i < 20; i++) {
					if (enemyAttack[i].isBulletShot == false) {
						enemyAttack[i].isBulletShot = true;
						enemyAttack[i].pos.x = enemy.pos.x;
						enemyAttack[i].pos.y = enemy.pos.y;
					}
					if (enemyAttack[i].isBulletShot) {
						enemyAttack[i].pos.y += enemyAttack[i].speed;
						enemyAttack[i].pos.x -= enemyAttack[i].speed + i * 5;

					}

					playerDistance = collision((enemyAttack[i].pos.x - player.pos.x), (enemyAttack[i].pos.y - player.pos.y));
					if (playerDistance <= 16 + player.radius) {

						enemyAttack[i].isBulletShot = false;
						if (ivincibleFlag == false) {
							life--;
							ivincibleFlag = true;
						}
						
					}
				}
			}


			if (motionCount >= 550) {
				
				enemy.pos.y = 600.0f;
					motionCount = 0;
			}

			if (enemyLife <= 0) {
				enemy.pos.x = 1000.0f;
				motionCount = 0;
				level++;
				enemyLife = 100 + level * 10;
				theta = float(M_PI) / 30.0f;
				life = 3;
				move = -500.0f;
				for (int i = 0; i < 20; i++) {
					enemyAttack[i].isBulletShot = false;
					enemyAttack[i].pos.x = enemy.pos.x;
					enemyAttack[i].pos.y = enemy.pos.y;
				
				}
				gameShene = GAMEdoragon;


			}

			playerDistance = collision((enemy.pos.x - player.pos.x), (enemy.pos.y - player.pos.y));
			if (playerDistance <= 64 + player.radius) {
				if (ivincibleFlag == false) {
					life--;
					ivincibleFlag = true;
				}
			}
			if (ivincibleFlag) {
				invincible--;
			}


			if (ivincibleFlag) {
				randX = rand() % 21 - 10;
				invincible--;
			} else {
				randX = 0;
			}

			if (invincible == 0) {
				invincible = 60;
				ivincibleFlag = false;
			}

			if (life <= 0) {
			
				gameShene = GAMEOVER;
				isScore = true;
			}


			break;
		
		case GAMEOVER:
			if (isScore) {
				score = level;
				
				numberArray[0] = score / 100;
				score %= 100;
				numberArray[1] = score / 10;
				score %= 10;
				numberArray[2] = score / 1;
				score %= 1;

			}

			if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
				gameShene = TITLE;
				life = 3;
				score = 0;
				level = 0;
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
			/*Novice::ScreenPrintf(0, 0, "TITLE");*/
			Novice::DrawSprite(0, 0, titleHandle, 1.0f, 1.0f, 0.0f, WHITE);
			break;

		case GAMEdoragon:

			Novice::DrawSprite(0, 0, backGroundHandle, 1.0f, 1.0f, 0.0f, WHITE);

			/*Novice::ScreenPrintf(0, 0, "DORAGON");
			Novice::ScreenPrintf(0, 20, "%d", life);*/
			for (int i = 0; i < 20; i++) {
				if (enemyAttack[i].isBulletShot) {
					Novice::DrawSprite(static_cast<int>(enemyAttack[i].pos.x - 16), static_cast<int>(enemyAttack[i].pos.y - 16),
						bressHandle[mahouCount / 15], 1.0f, 1.0f, 0.0f, WHITE);
				}
			}

			for (int i = 0; i < 3; i++) {
				if (bullet[i].isShoot) {
					Novice::DrawSprite(static_cast<int>(bullet[i].pos.x - bullet[i].radius),
						static_cast<int>(bullet[i].pos.y - bullet[i].radius),
						mahouHandle[mahouCount / 15], 1.0f, 1.0f, 0.0f, WHITE);
				}
			}


			if (player.pos.x >= mouse.posX) {
				Novice::DrawSprite(static_cast<int>(player.pos.x - 16+randX), static_cast<int>(player.pos.y - 16), playerHidariHandle, 1.0f, 1.0f, 0.0f, WHITE);
			} else{
				Novice::DrawSprite(static_cast<int>(player.pos.x - 16+randX), static_cast<int>(player.pos.y - 16), playerMigiHandle, 1.0f, 1.0f, 0.0f, WHITE);
			}

	

			Novice::DrawSprite(static_cast<int>(enemy.pos.x -64.0f+enemyRandX),
				static_cast<int>(enemy.pos.y-64.0f), doragonhandle, 1.0f, 1.0f, 0.0f, WHITE);


			Novice::DrawSprite(mouse.posX - 24, mouse.posY - 24, pointHandle, 1.0f, 1.0f, 0.0f, WHITE);

			break;
		case GAMEookami:
			Novice::DrawSprite(0, 0, backGroundHandle, 1.0f, 1.0f, 0.0f, WHITE);
			//狼
			/*Novice::ScreenPrintf(0, 0, "OOKAMI");
			Novice::ScreenPrintf(0, 20, "%d", life);*/

			for (int i = 0; i < 3; i++) {
				if (bullet[i].isShoot) {
					Novice::DrawSprite(static_cast<int>(bullet[i].pos.x - bullet[i].radius), static_cast<int>(bullet[i].pos.y - bullet[i].radius),
						mahouHandle[mahouCount / 15], 1.0f, 1.0f, 0.0f, WHITE);
				}
			}

			if (player.pos.x >= mouse.posX) {
				Novice::DrawSprite(static_cast<int>(player.pos.x - 16+randX), static_cast<int>(player.pos.y - 16), playerHidariHandle, 1.0f, 1.0f, 0.0f, WHITE);
			} else {
				Novice::DrawSprite(static_cast<int>(player.pos.x - 16+randX), static_cast<int>(player.pos.y - 16), playerMigiHandle, 1.0f, 1.0f, 0.0f, WHITE);
			}

		
			Novice::DrawSprite(static_cast<int>(enemy.pos.x - 64.0f+enemyRandX),
				static_cast<int>(enemy.pos.y - 64.0f), ookamihandle, 1.0f, 1.0f, 0.0f, WHITE);
			Novice::DrawSprite(mouse.posX - 24, mouse.posY - 24, pointHandle, 1.0f, 1.0f, 0.0f, WHITE);

			break;
		case GAMEgoburinn:
			Novice::DrawSprite(0, 0, backGroundHandle, 1.0f, 1.0f, 0.0f, WHITE);
			//
			/*Novice::ScreenPrintf(0, 20, "%d", motionCount);*/

			for (int i = 0; i < 3; i++) {
				if (bullet[i].isShoot) {
					Novice::DrawSprite(static_cast<int>(bullet[i].pos.x - bullet[i].radius), static_cast<int>(bullet[i].pos.y - bullet[i].radius),
						mahouHandle[mahouCount / 15], 1.0f, 1.0f, 0.0f, WHITE);
				}
			}
			
			if (player.pos.x >= mouse.posX) {
				Novice::DrawSprite(static_cast<int>(player.pos.x - 16+randX), static_cast<int>(player.pos.y - 16), playerHidariHandle, 1.0f, 1.0f, 0.0f, WHITE);
			} else {
				Novice::DrawSprite(static_cast<int>(player.pos.x - 16+randX), static_cast<int>(player.pos.y - 16), playerMigiHandle, 1.0f, 1.0f, 0.0f, WHITE);
			}
			

			//ゴブリン
			/*Novice::ScreenPrintf(0, 0, "GOBURINN");*/
			Novice::DrawSprite(static_cast<int>(enemy.pos.x - 64.0f),
				static_cast<int>(enemy.pos.y - 64.0f), goburinhanndle, 1.0f, 1.0f, 0.0f, WHITE);
			//子供
			for (int i = 0; i < 5; i++) {
				if (enemyFollowers[i].isAlive) {
					Novice::DrawSprite(static_cast<int>(enemyFollowers[i].pos.x-32),
						static_cast<int>(enemyFollowers[i].pos.y-32), kodomogoburinnhandle, 1.0f, 1.0f, 0.0f, WHITE);
				}
			}
			
			Novice::DrawSprite(mouse.posX - 24, mouse.posY - 24, pointHandle, 1.0f, 1.0f, 0.0f, WHITE);

			break;
		case GAMErizadoman:

			Novice::DrawSprite(0, 0, backGroundHandle, 1.0f, 1.0f, 0.0f, WHITE);
			//リザードマン
			/*Novice::ScreenPrintf(0, 0, "RIZA-DOMAN");
			Novice::ScreenPrintf(0, 20, "%d", motionCount);*/

			for (int i = 0; i < 3; i++) {
				if (bullet[i].isShoot) {
					Novice::DrawSprite(static_cast<int>(bullet[i].pos.x - bullet[i].radius), static_cast<int>(bullet[i].pos.y - bullet[i].radius),
						mahouHandle[mahouCount/15],1.0f,1.0f, 0.0f, WHITE);
				}
			}

			if (player.pos.x >= mouse.posX) {
				Novice::DrawSprite(static_cast<int>(player.pos.x - 16+randX), static_cast<int>(player.pos.y - 16), playerHidariHandle, 1.0f, 1.0f, 0.0f, WHITE);
			} else {
				Novice::DrawSprite(static_cast<int>(player.pos.x - 16+randX), static_cast<int>(player.pos.y - 16), playerMigiHandle, 1.0f, 1.0f, 0.0f, WHITE);
			}
			

			
			Novice::DrawSprite(static_cast<int>(enemy.pos.x - 128.0f),
				static_cast<int>(enemy.pos.y-64.0f), zangekiHandle[zangekiCount/10], 1.0f, 1.0f, 0.0f, WHITE);
			Novice::DrawSprite(static_cast<int>(enemy.pos.x - 64.0f),
				static_cast<int>(enemy.pos.y - 64.0f), rizadomanhandle, 1.0f, 1.0f, 0.0f, WHITE);

			Novice::DrawSprite(mouse.posX - 24, mouse.posY - 24, pointHandle, 1.0f, 1.0f, 0.0f, WHITE);

			break;
		case GAMEdoragon2:
			Novice::DrawSprite(0, 0, backGroundHandle, 1.0f, 1.0f, 0.0f, WHITE);
			//ドラゴン２
			/*Novice::ScreenPrintf(0, 0, "DORAGON2");
			Novice::ScreenPrintf(0, 20, "%d", motionCount);*/

			for (int i = 0; i < 20; i++) {
				if (enemyAttack[i].isBulletShot) {
					Novice::DrawSprite(static_cast<int>(enemyAttack[i].pos.x - 16), static_cast<int>(enemyAttack[i].pos.y - 16),
						bressHandle[mahouCount / 15], 1.0f, 1.0f, 0.0f, WHITE);
				}
			}

			for (int i = 0; i < 3; i++) {
				if (bullet[i].isShoot) {
					Novice::DrawSprite(static_cast<int>(bullet[i].pos.x - bullet[i].radius), static_cast<int>(bullet[i].pos.y - bullet[i].radius),
						mahouHandle[mahouCount / 15], 1.0f, 1.0f, 0.0f, WHITE);
				}
			}

		
			if (player.pos.x >= mouse.posX) {
				Novice::DrawSprite(static_cast<int>(player.pos.x - 16+randX), static_cast<int>(player.pos.y - 16), playerHidariHandle, 1.0f, 1.0f, 0.0f, WHITE);
			} else {
				Novice::DrawSprite(static_cast<int>(player.pos.x - 16), static_cast<int>(player.pos.y - 16), playerMigiHandle, 1.0f, 1.0f, 0.0f, WHITE);
			}


			Novice::DrawSprite(static_cast<int>(enemy.pos.x - 64.0f),
				static_cast<int>(enemy.pos.y - 64.0f), doragon2handle, 1.0f, 1.0f, 0.0f, WHITE);

			Novice::DrawSprite(mouse.posX - 24, mouse.posY - 24, pointHandle, 1.0f, 1.0f, 0.0f, WHITE);

			
			
			break;
		case GAMEOVER:

			/*Novice::ScreenPrintf(0, 0, "GAMEOVER");*/
			Novice::DrawSprite(0, 0, scoreHandle, 1.0f, 1.0f, 0.0f, WHITE);

			for (int i = 0; i < 3; i++){
				Novice::DrawSprite(250+i*32, 500, numberHandle[numberArray[i]], 1.0f, 1.0f, 0.0f, WHITE);
			}
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

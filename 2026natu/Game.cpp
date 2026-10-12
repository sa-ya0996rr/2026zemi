// DxLibのキー入力や終了処理などを使う
#include "DxLib.h"
// ゲームの関数の宣言を読み込む
#include "Game.h"
// ブロックの情報やマップ定数を使う
#include "Block.h"
// プレイヤー操作の関数・変数を使う
#include "Player.h"
// ゲーム画面を描く関数を使う
#include "Draw.h"
// rand、srand、exitなどを使う
#include <stdlib.h>
// 現在時刻を乱数の種に使う
#include <time.h>
/*
*/

// ゲームの進行段階。0=操作、1=1回目の重力、2=上昇、3=新動物、4=2回目の重力
int gameState = 0;
// プレイヤーが終えたターン数
int turn = 0;
// 現在の得点
int score = 0;
// trueならゲームオーバー
bool gameOver = false;
/*
*/

// 落下アニメーションで1フレームに動く量
const float FALL_SPEED = 4.0f;
// 既存ブロックが上がる速さ
const float RISE_SPEED = 2.0f;
// 新しい動物が下から上がる速さ
const float NEW_BLOCK_SPEED = 2.0f;
// 好物が下から上がる速さ
const float FOOD_SPEED = 2.0f;
/*
*/

// 新しい動物の配列番号。-1は対象がない状態
int newBlockIndex = -1;
/*
*/

// 好物用
// 好物の配列番号。-1は好物がない状態
int foodBlockIndex = -1;
// 好物が上昇アニメーション中かどうか
bool foodRising = false;
/*
*/

// ==================================================
// 関数の宣言
// ==================================================

// 支えのないブロックを落下対象にして、重力処理を開始する
void StartGravity();
// 落下中のブロックを動かし、落下が終わったか確認する
void UpdateFalling();
/*
*/

// 既存ブロックを論理上1段上げ、上昇アニメーションを開始する
void StartRising();
// 既存ブロックを画面上で上昇させる
void UpdateRising();

// 新しい動物を追加して、下から上がる段階に切り替える
void StartNewBlock();
// 新しい動物を画面下から所定のマスまで上げる
void UpdateNewBlock();
// 新しい羊または牛をランダムな長さ・位置で追加する
void AddNewAnimal();

// 好物（WまたはT）をランダムに出す
void MakeFavoriteFood();
// 好物を毎フレーム少しずつ上昇させる
void UpdateFavoriteFood();
// 好物の真下に好きな動物がいるか調べ、得点を加算する
void CheckFavoriteFood();

// ==================================================
// 好物を出す
// ==================================================
void MakeFavoriteFood()
{
    // 3ターン目までは出さない
    // ターン数が3未満なら、ここで関数を終了する
    if (turn < 3)
        return;

    // 5回に1回くらい出る
    // 乱数が0でなければ今回は好物を出さない（およそ5回に1回）
    if (rand() % 5 != 0)
        return;
    /*
    */

    // 動物がいるか調べる
    // 生きている羊・牛がいるかを記録するフラグ
    bool sheepExist = false;
    bool cowExist = false;

    for (int i = 0; i < blockCount; i++)
    {
        if (blockExist[i] == false)
            continue;

        if (blockType[i] == 'S')
            sheepExist = true;

        if (blockType[i] == 'C')
            cowExist = true;
    }
    /*
    */

    // 動物がいなければ出さない
    if (sheepExist == false && cowExist == false)
        return;

    // 好物の種類を決める
    // 今回出す好物の種類（W=小麦、T=とうもろこし）
    char foodType;

    if (sheepExist && cowExist)
    {
        // 羊と牛が両方いる場合はランダム
        if (rand() % 2 == 0)
            foodType = 'W';
        else
            foodType = 'T';
    }
    else if (sheepExist)
    {
        // 羊しかいないなら小麦
        foodType = 'W';
    }
    else
    {
        // 牛しかいないならとうもろこし
        foodType = 'T';
    }
    /*
    */

    // 出す場所を決める
    // 好物を置く横方向のマスをランダムに選ぶ
    int x = rand() % WIDTH;

    AddBlock(
        foodType,
        x,
        HEIGHT - 1, 1);

    /*
    */

    // AddBlockで最後に追加されたブロックが好物
    foodBlockIndex = blockCount - 1;

    // 画面の下から出す
    blockDrawY[foodBlockIndex] =
        (float)(BOARD_Y + HEIGHT * CELL);

    // 好物の上昇アニメーションを開始する
    foodRising = true;
    /*
    */
}

// ==================================================
// 好物を上に動かす
// ==================================================
void UpdateFavoriteFood()
/*
*/
{
    if (foodBlockIndex < 0)
        return;

    if (blockExist[foodBlockIndex] == false)
    {
        foodRising = false;
        foodBlockIndex = -1;
        return;
    }

    // 好物を少しずつ上げる
    blockDrawY[foodBlockIndex] -= FOOD_SPEED;

    // 画面内に入ったら止める
    float targetY = (float)(BOARD_Y + (HEIGHT - 1) * CELL);

    if (blockDrawY[foodBlockIndex] <= targetY)
    {
        blockDrawY[foodBlockIndex] = targetY;
        foodRising = false;
    }
}

// ==================================================
// 好物と動物をチェック
// ==================================================
void CheckFavoriteFood()
{
    for (int i = 0; i < blockCount; i++)
    {
        if (blockExist[i] == false)
            continue;

        // --------------------------------------------------
        // 小麦 → 羊
        // --------------------------------------------------
        if (blockType[i] == 'W')
        {
            for (int j = 0; j < blockCount; j++)
            {
                if (blockExist[j] == false)
                    continue;

                if (blockType[j] != 'S')
                    continue;
                /*
                */

                // 小麦が羊の上にあるか
                if (blockY[i] == blockY[j] - 1)
                {
                    if (IsOverlapping(
                        blockX[i],
                        blockLength[i],
                        blockX[j],
                        blockLength[j]))
                    {
                        // 好物と好きな動物が合ったので100点加算
                        score += 100;

                        // 食べられた好物を消す
                        blockExist[i] = false;

                        if (i == foodBlockIndex)
                        {
                            foodBlockIndex = -1;
                            foodRising = false;
                        }

                        break;
                    }
                    /*
                    */
                }
            }
        }


        // --------------------------------------------------
        // とうもろこし → 牛
        // --------------------------------------------------

        if (blockType[i] == 'T')
        {
            for (int j = 0; j < blockCount; j++)
            {
                if (blockExist[j] == false)
                    continue;

                if (blockType[j] != 'C')
                    continue;


                // とうもろこしが牛の上にあるか
                if (blockY[i] == blockY[j] - 1)
                {
                    if (IsOverlapping(
                        blockX[i],
                        blockLength[i],
                        blockX[j],
                        blockLength[j]))
                    {
                        score += 100;

                        blockExist[i] = false;

                        if (i == foodBlockIndex)
                        {
                            foodBlockIndex = -1;
                            foodRising = false;
                        }

                        break;
                    }
                }
            }
        }
    }
}

// ==================================================
// 新しい動物を追加
// ==================================================
void AddNewAnimal()
{
    // 新しい動物の種類を入れる変数（S=羊、C=牛）
    char type;

    if (rand() % 2 == 0)
        type = 'S';
    else
        type = 'C';
    /*
    */

    // 動物ブロックの長さを1～4マスから選ぶ
    int length = rand() % 4 + 1;
    /*
    */

    // ブロック全体がマップ内に収まる横位置を選ぶ
    int x = rand() % (WIDTH - length + 1);

    AddBlock(
        type,
        x,
        HEIGHT - 1,
        length
    );


    newBlockIndex =
        blockCount - 1;


    // 画面の下から出す
    blockDrawY[newBlockIndex] =
        (float)(BOARD_Y + HEIGHT * CELL);


    newBlockRising = true;
}


// ==================================================
// 重力開始
// ==================================================
void StartGravity()
{
    // 落下するブロックが1つでもあるかを記録する
    bool someoneFalls = false;


    for (int i = 0; i < blockCount; i++)
        falling[i] = false;


    for (int i = 0; i < blockCount; i++)
    {
        if (blockExist[i] == false)
            continue;

        // 真下に支えがあるブロックは落ちない
        if (HasSupport(i))
            continue;

        // 一番下の段より下には落とさない
        if (blockY[i] >= HEIGHT - 1)
            continue;

        // このブロックを落下対象にする
        falling[i] = true;
        /*
        */

        someoneFalls = true;
    }


    if (someoneFalls)
    {
        if (gameState == 4)
            gameState = 4;
        else
            gameState = 1;
    }
    else
    {
        if (gameState == 4)
        {
            gameState = 0;

            playerMoved = false;
            playerTurnFinished = false;
        }
        else
        {
            StartRising();
        }
    }
}


// ==================================================
// ブロックを落とす
// ==================================================
void UpdateFalling()
{
    for (int i = 0; i < blockCount; i++)
    {
        if (blockExist[i] == false)
            continue;

        if (falling[i] == false)
            continue;

        int targetY =
            blockY[i] + 1;

        if (targetY >= HEIGHT)
        {
            falling[i] = false;
            continue;
        }

        bool canFall = true;

        for (int j = 0; j < blockCount; j++)
        {
            if (i == j)
                continue;

            if (blockExist[j] == false)
                continue;

            if (blockY[j] != targetY)
                continue;

            if (IsOverlapping(
                blockX[i],
                blockLength[i],
                blockX[j],
                blockLength[j]))
            {
                canFall = false;
                break;
            }
        }

        if (canFall == false)
        {
            falling[i] = false;
            continue;
        }

        // 次のマスに到着したときの画面上のY座標
        float targetDrawY =
            (float)(
                BOARD_Y +
                targetY * CELL
                );

        // 描画位置を少しずつ下へ動かす
        blockDrawY[i] += FALL_SPEED;


        if (blockDrawY[i] >= targetDrawY)
        {
            blockDrawY[i] = targetDrawY;
            blockY[i] = targetY;

            falling[i] = false;
        }
    }
    /*
    */

    // まだ落下中のブロックがあるかを調べる
    bool stillFalling = false;

    for (int i = 0; i < blockCount; i++)
    {
        if (blockExist[i] == false)
            continue;

        if (falling[i])
        {
            stillFalling = true;
            break;
        }
    }
    /*
    */

    if (stillFalling)
        return;

    // さらに落下できるブロックが残っているかを調べる
    bool needMoreGravity = false;

    for (int i = 0; i < blockCount; i++)
    {
        if (blockExist[i] == false)
            continue;

        if (HasSupport(i))
            continue;


        if (blockY[i] < HEIGHT - 1)
        {
            needMoreGravity = true;
            break;
        }
    }

    if (needMoreGravity)
    {
        StartGravity();
    }
    else
    {
        if (gameState == 1)
        {
            StartRising();
        }
        else if (gameState == 4)
        {
            gameState = 0;

            playerMoved = false;
            playerTurnFinished = false;
        }
    }
}

// ==================================================
// 既存ブロックを1段上げる
// ==================================================
void StartRising()
{
    for (int i = 0; i < blockCount; i++)
    {
        if (blockExist[i] == false)
            continue;

        // 一番上の段にブロックがあると、さらに上げられない
        if (blockY[i] <= 0)
        {
            gameOver = true;
            return;
        }
        /*
        */
    }

    bool hasBlock = false;

    for (int i = 0; i < blockCount; i++)
    {
        if (blockExist[i] == false)
            continue;


        hasBlock = true;

        // マス上の位置を1段上にし、描画アニメーションを開始する
        blockY[i]--;

        rising[i] = true;
    }
    /*
    */

    if (hasBlock)
        gameState = 2;
    else
        StartNewBlock();
    /*
    */
}

// ==================================================
// ブロックを上げるアニメーション
// ==================================================
void UpdateRising()
{
    bool stillRising = false;

    for (int i = 0; i < blockCount; i++)
    {
        if (blockExist[i] == false)
            continue;

        if (rising[i] == false)
            continue;

        float targetY = (float)(BOARD_Y + blockY[i] * CELL);

        blockDrawY[i] -= RISE_SPEED;

        if (blockDrawY[i] <= targetY)
        {
            blockDrawY[i] = targetY;

            rising[i] = false;
        }
        else
        {
            stillRising = true;
        }
    }

    if (!stillRising)
        StartNewBlock();
}

// ==================================================
// 新しいブロックを出す
// ==================================================
void StartNewBlock()
{
    AddNewAnimal();

    gameState = 3;
}

// ==================================================
// 新しい動物を上げる
// ==================================================
void UpdateNewBlock()
{
    if (newBlockIndex < 0)
    {
        gameState = 4;
        StartGravity();
        return;
    }


    if (blockExist[newBlockIndex] == false)
    {
        gameState = 4;
        StartGravity();
        return;
    }

    float targetY = (float)(BOARD_Y + blockY[newBlockIndex] * CELL);

    blockDrawY[newBlockIndex] -= NEW_BLOCK_SPEED;

    if (blockDrawY[newBlockIndex] <= targetY)
    {
        blockDrawY[newBlockIndex] = targetY;

        newBlockRising = false;

        gameState = 4;

        StartGravity();
    }
    /*
    */
}

// ==================================================
// ゲーム初期化
// ==================================================
// ゲーム開始時に変数を初期化して最初のブロックを配置する
void InitGame()
{
    // 起動時刻を乱数の種にして、毎回違う乱数になりやすくする
    srand((unsigned int)time(NULL));

    blockCount = 0;

    turn = 0;

    score = 0;

    gameOver = false;

    gameState = 0;

    newBlockIndex = -1;

    foodBlockIndex = -1;

    foodRising = false;

    dragging = false;

    dragBlock = -1;

    playerMoved = false;

    playerTurnFinished = false;

    newBlockRising = false;

    // Block.cppにある初期配置のブロックを作る
    MakeInitialBlocks();
}
/*
*/

// ==================================================
// ゲーム更新
// ==================================================
// 毎フレーム呼ばれ、現在のゲーム段階に応じて処理を進める
void UpdateGame()
{
    // Escキーで終了
    // Escキーが押されたらゲームを終了する
    if (CheckHitKey(KEY_INPUT_ESCAPE))
    {
        DxLib_End();
        exit(0);
    }

    // ゲームオーバー後はゲームの更新を行わない
    if (gameOver)
        return;

    // ==================================================
    // プレイヤーのターン
    // ==================================================
    if (gameState == 0)
    {
        UpdatePlayer();

        if (playerTurnFinished)
        {
            // プレイヤーの操作が終わったのでターン数を1増やす
            turn++;

            playerTurnFinished = false;

            // 好物をチェック
            // 動物と好物の位置を調べ、食べられていれば得点を加算
            CheckFavoriteFood();

            // 重力開始
            // プレイヤー操作後の1回目の重力を開始する
            StartGravity();
        }
    }

    // ==================================================
    // 1回目の重力
    // ==================================================
    else if (gameState == 1)
    {
        UpdateFalling();
    }

    // ==================================================
    // 既存ブロックを上げる
    // ==================================================
    else if (gameState == 2)
    {
        UpdateRising();
    }

    // ==================================================
    // 新しい動物を上げる
    // ==================================================
    else if (gameState == 3)
    {
        UpdateNewBlock();
    }

    // ==================================================
    // 2回目の重力
    // ==================================================
    else if (gameState == 4)
    {
        UpdateFalling();
    }

    // ==================================================
    // 好物の処理
    // ==================================================
    // 好物が上昇中なら、毎フレーム位置を更新する
    if (foodRising)
    {
        UpdateFavoriteFood();
    }

    // 3ターン目以降、好物を出す
    // すでに好物が出ている場合は出さない
    if (turn >= 3 &&
        foodBlockIndex == -1)
    {
        MakeFavoriteFood();
    }
    /*
    */
}

// ==================================================
// ゲーム描画
// ==================================================
// 現在のターン・スコア・状態をDraw.cppへ渡して画面を描く
void DrawGame()
{
    // 描画に必要な情報をDraw.cppの関数へ渡す
    DrawGameScreen(
        turn,
        score,
        gameOver
    );
    /*
    */
}

/*
Game.cppを一言で説明すると

発表や授業で説明するなら、これでかなり分かりやすいと思う。

Game.cppは、ゲーム全体の進行を管理する役割です。プレイヤーの操作後に重力を発生させ、既存のブロックを上げ、新しい動物を追加し、再び重力を発生させるというゲームの流れを管理しています。また、好物の出現やスコア、ゲームオーバーなども管理しています。

そして4つのファイルの関係は、

Player.cpp
  ↓
「プレイヤーがどう動かすか」

Block.cpp
  ↓
「ブロックが何なのか・支えがあるか」

Game.cpp
  ↓
「ゲーム全体をどう進めるか」

Draw.cpp
  ↓
「それを画面にどう表示するか」

という分担になっているよ。
*/
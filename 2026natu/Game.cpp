/*
Game.cppはこのゲームの中でかなり重要で、

「ゲーム全体の進行を管理するファイル」

になっているよ。

Game.cppの全体の役割

大きく分けると、Game.cppはこの仕事をしている。

Game.cpp
│
├─ ゲームの状態を管理
│
├─ ターンを管理
│
├─ スコアを管理
│
├─ 重力でブロックを落とす
│
├─ ブロックを1段上げる
│
├─ 新しい動物を出す
│
├─ 好物を出す
│
├─ 好物を食べたか判定
│
├─ ゲームオーバー判定
│
├─ ゲームの初期化
│
├─ 毎フレームのゲーム更新
│
└─ Draw.cppに画面描画を依頼

つまり、

Player.cpp → プレイヤーの操作

Block.cpp → ブロックそのものの情報・判定

Draw.cpp → 画面に描く

Game.cpp → それらをまとめてゲームを進める

という役割分担になっている。
*/

#include "DxLib.h"
#include "Game.h"
#include "Block.h"
#include "Player.h"
#include "Draw.h"
#include <stdlib.h>
#include <time.h>
/*
① ヘッダーファイルを読み込む

最初の部分。

#include "DxLib.h"
#include "Game.h"
#include "Block.h"
#include "Player.h"
#include "Draw.h"
#include <stdlib.h>
#include <time.h>

この部分は、Game.cppで必要な機能を読み込んでいる。

DxLib.h
#include "DxLib.h"

DxLibの機能を使うため。

例えば、

CheckHitKey()
GetMousePoint()
DxLib_End()

など。

Game.h
#include "Game.h"

Game.cppのゲーム関係の関数を他のファイルとつなぐため。

Block.h
#include "Block.h"

ブロックの情報を使うため。

例えば、

blockCount
blockType
blockX
blockY
blockLength
blockExist

など。

Player.h
#include "Player.h"

プレイヤー操作の関数や変数を使うため。

今回だと、

UpdatePlayer();
playerMoved;
playerTurnFinished;

など。

Draw.h
#include "Draw.h"

最後にゲーム画面を描く、

DrawGameScreen();

を使うため。

<stdlib.h>
#include <stdlib.h>

rand()やexit()などを使うため。

<time.h>
#include <time.h>

乱数を毎回変えるために、

time(NULL)

を使う。
*/

int gameState = 0;
int turn = 0;
int score = 0;
bool gameOver = false;
/*
② ゲームの基本的な変数
int gameState = 0;
int turn = 0;
int score = 0;
bool gameOver = false;

それぞれ、

変数	    意味
gameState	今ゲームのどの処理をしているか
turn	    現在のターン数
score	    現在のスコア
gameOver	ゲームオーバーかどうか

③ gameStateが特に重要

このゲームでは、

gameState

を使ってゲームの進行を切り替えている。

現在は、

0 → プレイヤー操作
1 → 1回目の重力
2 → 既存ブロックを上げる
3 → 新しい動物を出す
4 → 2回目の重力

という流れ。

つまり、

プレイヤーが動かす
      ↓
    重力
      ↓
既存ブロックを上げる
      ↓
新しい動物
      ↓
    重力
      ↓
次のターン

となっている。
*/

const float FALL_SPEED = 4.0f;
const float RISE_SPEED = 2.0f;
const float NEW_BLOCK_SPEED = 2.0f;
const float FOOD_SPEED = 2.0f;
/*
④ アニメーション速度
const float FALL_SPEED = 4.0f;
const float RISE_SPEED = 2.0f;
const float NEW_BLOCK_SPEED = 2.0f;
const float FOOD_SPEED = 2.0f;

これはブロックを動かす速度。

変数	意味
FALL_SPEED	ブロックが落ちる速度
RISE_SPEED	既存ブロックが上がる速度
NEW_BLOCK_SPEED	新しい動物が上がる速度
FOOD_SPEED	好物が上がる速度

floatなのは、

4.0
2.0

のような小数を使って滑らかに動かすため。
*/

int newBlockIndex = -1;
/*
⑤ 新しいブロックを管理
int newBlockIndex = -1;

これは、

今出てくる新しい動物が、ブロック配列の何番目なのか

を覚えておく変数。

-1は、

「今は新しいブロックがない」

という意味として使っている。
*/

// 好物用
int foodBlockIndex = -1;
bool foodRising = false;
/*
⑥ 好物用の変数
int foodBlockIndex = -1;
bool foodRising = false;

foodBlockIndex

好物がブロック配列の何番目なのかを覚える。

foodRising

好物が現在、

下から上に上がっている

かどうか。
*/

// ==================================================
// 関数の宣言
// ==================================================

void StartGravity();
void UpdateFalling();
/*
⑦ 関数の宣言

例えば、

void StartGravity();
void UpdateFalling();

など。

これは、

Game.cppの後ろの方にこういう関数がありますよ

と先に知らせている。

今回のGame.cppにはたくさんの関数があるから、先に宣言しておくことで、後ろに書いた関数を前の関数から使えるようにしている。
*/

void StartRising();
void UpdateRising();

void StartNewBlock();
void UpdateNewBlock();
void AddNewAnimal();

void MakeFavoriteFood();
void UpdateFavoriteFood();
void CheckFavoriteFood();
/*
⑧ 好物を出す MakeFavoriteFood()
void MakeFavoriteFood()

この関数は、

小麦やとうもろこしを出す

ための関数。

3ターン目までは出さない
if (turn < 3)
    return;

つまり、

TURN 0
TURN 1
TURN 2

では好物を出さない。

turnが3以上になったら先に進む。

5回に1回
if (rand() % 5 != 0)
    return;

乱数を使って、

0～4

のどれかを作る。

そのうち0のときだけ進む。

なので、おおよそ

5回に1回

好物が出る仕組み。
*/

// ==================================================
// 好物を出す
// ==================================================
void MakeFavoriteFood()
{
    // 3ターン目までは出さない
    if (turn < 3)
        return;

    // 5回に1回くらい出る
    if (rand() % 5 != 0)
        return;
    /*
    ⑧ 好物を出す MakeFavoriteFood()
    void MakeFavoriteFood()

    この関数は、

    小麦やとうもろこしを出す

    ための関数。

    3ターン目までは出さない
    if (turn < 3)
        return;

    つまり、

    TURN 0
    TURN 1
    TURN 2

    では好物を出さない。

    turnが3以上になったら先に進む。

    5回に1回
    if (rand() % 5 != 0)
        return;

    乱数を使って、

    0～4

    のどれかを作る。

    そのうち0のときだけ進む。

    なので、おおよそ

    5回に1回

    好物が出る仕組み。
    */

    // 動物がいるか調べる
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
    ⑨ 羊と牛がいるか調べる
    bool sheepExist = false;
    bool cowExist = false;

    最初は、

    羊はいない
    牛はいない

    としておく。

    そのあと、

    for (int i = 0; i < blockCount; i++)

    ですべてのブロックを調べる。

    if (blockType[i] == 'S')
        sheepExist = true;

    if (blockType[i] == 'C')
        cowExist = true;

    Sがあれば羊がいる。

    Cがあれば牛がいる。
    */

    // 動物がいなければ出さない
    if (sheepExist == false && cowExist == false)
        return;

    // 好物の種類を決める
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
    ⑩ 好物の種類を決める

    羊と牛の両方がいる場合、

    if (rand() % 2 == 0)
        foodType = 'W';
    else
        foodType = 'T';

    となっている。

    つまり、

    羊＋牛
     ↓
    ランダム
     ↓
    W または T

    羊しかいなければ、

    foodType = 'W';

    なので小麦。

    牛しかいなければ、

    foodType = 'T';

    なのでとうもろこし。
    */

    // 出す場所を決める
    int x = rand() % WIDTH;

    AddBlock(
        foodType,
        x,
        HEIGHT - 1, 1);

    /*
    ⑪ 好物をブロックとして追加
    AddBlock(
        foodType,
        x,
        HEIGHT - 1,
        1
    );

    ここでBlock.cppの、

    AddBlock()

    を呼んでいる。

    好物は、

    長さ1

    のブロックとして追加している。
    */

    foodBlockIndex = blockCount - 1;

    // 画面の下から出す
    blockDrawY[foodBlockIndex] =
        (float)(BOARD_Y + HEIGHT * CELL);

    foodRising = true;
    /*
    ⑫ 好物を画面の下から出す
    blockDrawY[foodBlockIndex] =
        (float)(BOARD_Y + HEIGHT * CELL);

    論理上は一番下の段にいるけど、描画位置はさらに下にしている。

    そのため、

    画面外
     ↓
    ゆっくり上がる
     ↓
    一番下のマス

    という演出ができる。
    */
}

// ==================================================
// 好物を上に動かす
// ==================================================
void UpdateFavoriteFood()
/*
⑬ UpdateFavoriteFood()

これは好物を少しずつ上に動かす関数。

blockDrawY[foodBlockIndex] -= FOOD_SPEED;

Y座標を小さくすると画面上では上に移動する。

そして、

if (blockDrawY[foodBlockIndex] <= targetY)

になったら目的地に到着。
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
                ⑭ 好物を食べたか調べる
                void CheckFavoriteFood()

                これは、

                好物の真下に対応する動物がいるか

                を調べる。

                例えば小麦なら、

                 W
                 S

                という状態。

                if (blockType[i] == 'W')

                で小麦を探して、

                if (blockType[j] != 'S')
                    continue;

                で羊を探す。
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
                        score += 100;

                        blockExist[i] = false;

                        if (i == foodBlockIndex)
                        {
                            foodBlockIndex = -1;
                            foodRising = false;
                        }

                        break;
                    }
                    /*
                    ⑮ 小麦＋羊なら100点
                    score += 100;

                    小麦と羊が正しい位置関係なら、

                    +100点

                    になる。

                    とうもろこし＋牛の場合も同じ。
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
    char type;

    if (rand() % 2 == 0)
        type = 'S';
    else
        type = 'C';
    /*
    ⑯ 新しい動物を作る
    void AddNewAnimal()

    この関数では新しい動物を作る。

    if (rand() % 2 == 0)
        type = 'S';
    else
        type = 'C';

    なので、

    S 羊
    C 牛

    をランダムで選ぶ。
    */

    int length = rand() % 4 + 1;
    /*
    ⑰ 長さもランダム
    int length =
        rand() % 4 + 1;

    これは、

    1～4

    の数字をランダムに作っている。

    つまり、

    S
    SS
    SSS
    SSSS

    や、

    C
    CC
    CCC
    CCCC

    のようなブロックが出る。
    */

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
    bool someoneFalls = false;


    for (int i = 0; i < blockCount; i++)
        falling[i] = false;


    for (int i = 0; i < blockCount; i++)
    {
        if (blockExist[i] == false)
            continue;

        if (HasSupport(i))
            continue;

        if (blockY[i] >= HEIGHT - 1)
            continue;


        falling[i] = true;
        /*
        ⑱ 重力開始 StartGravity()
        void StartGravity()

        ここはゲームの重要部分。

        まず、

        falling[i] = false;

        ですべてのブロックを「落ちていない」にする。

        そのあと、

        if (HasSupport(i))
            continue;

        で支えがあるか調べる。

        支えがなければ、

        falling[i] = true;

        にして落下対象にする。
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


        float targetDrawY =
            (float)(
                BOARD_Y +
                targetY * CELL
                );


        blockDrawY[i] += FALL_SPEED;


        if (blockDrawY[i] >= targetDrawY)
        {
            blockDrawY[i] = targetDrawY;
            blockY[i] = targetY;

            falling[i] = false;
        }
    }
    /*
    ⑲ UpdateFalling()

    実際にブロックを落とす関数。

    blockDrawY[i] += FALL_SPEED;

    Y座標を増やして、

    ↓
    ↓
    ↓

    と落とす。

    目的のマスまで来たら、

    blockY[i] = targetY;
    falling[i] = false;

    にする。
    */

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
    ⑳ 重力が終わったか確認
    bool stillFalling = false;

    として、

    if (falling[i])
    {
        stillFalling = true;
        break;
    }

    でまだ落ちているブロックがあるか確認する。

    全部止まったら、次の処理へ進む。
    */

    if (stillFalling)
        return;

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


        if (blockY[i] <= 0)
        {
            gameOver = true;
            return;
        }
        /*
        ㉒ ゲームオーバー
        if (blockY[i] <= 0)
        {
            gameOver = true;
            return;
        }

        一番上にブロックがある状態で、さらに1段上げようとしたらゲームオーバー。

        つまり、

        ┌─────────┐
        │  ブロック │ ← ここまで来た
        ├─────────┤
        │         │

        これ以上上げられないので終了。
        */
    }

    bool hasBlock = false;

    for (int i = 0; i < blockCount; i++)
    {
        if (blockExist[i] == false)
            continue;


        hasBlock = true;

        blockY[i]--;

        rising[i] = true;
    }
    /*
    ㉑ 既存ブロックを1段上げる
    void StartRising()

    ここでは、今あるブロックを全部1段上げる。

    blockY[i]--;

    なので、

    8段目 → 7段目
    9段目 → 8段目

    というように上がる。

    そして、

    rising[i] = true;

    にしてアニメーションを開始する。
    */

    if (hasBlock)
        gameState = 2;
    else
        StartNewBlock();
    /*
    ㉓ 新しい動物を出す
    void StartNewBlock()
    {
        AddNewAnimal();

        gameState = 3;
    }

    ここで新しい動物を作って、

    gameState = 3;

    にする。

    つまり、

    「新しい動物が下から上がってくる状態」

    に切り替える。
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
    ㉔ UpdateNewBlock()

    新しい動物を下から上げる。

    blockDrawY[newBlockIndex] -=
        NEW_BLOCK_SPEED;

    少しずつY座標を小さくしていく。

    目的の位置まで来たら、

    gameState = 4;
    StartGravity();

    として、

    2回目の重力

    を開始する。
    */
}

// ==================================================
// ゲーム初期化
// ==================================================
void InitGame()
{
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

    MakeInitialBlocks();
}
/*
㉕ InitGame()
void InitGame()

これはゲーム開始時の初期設定。

例えば、

turn = 0;
score = 0;
gameOver = false;

などに戻している。

そして最後に、

MakeInitialBlocks();

で最初の動物を配置する。
*/

// ==================================================
// ゲーム更新
// ==================================================
void UpdateGame()
{
    // Escキーで終了
    if (CheckHitKey(KEY_INPUT_ESCAPE))
    {
        DxLib_End();
        exit(0);
    }

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
            turn++;

            playerTurnFinished = false;

            // 好物をチェック
            CheckFavoriteFood();

            // 重力開始
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
    ㉖ 一番重要な UpdateGame()
    void UpdateGame()

    これは、

    ゲームが動いている間、毎フレーム呼ばれる中心的な関数

    だよ。

    ゲームの流れ

    gameStateによって処理を変えている。

    if (gameState == 0)
    {
        UpdatePlayer();
    }
    0

    プレイヤーが動物を操作する。

    ↓

    else if (gameState == 1)
    {
        UpdateFalling();
    }
    1

    1回目の重力。

    ↓

    else if (gameState == 2)
    {
        UpdateRising();
    }
    2

    既存ブロックを1段上げる。

    ↓

    else if (gameState == 3)
    {
        UpdateNewBlock();
    }
    3

    新しい動物を下から出す。

    ↓

    else if (gameState == 4)
    {
        UpdateFalling();
    }
    4

    2回目の重力。

    ↓

    次のプレイヤーターン

    という流れ。

    ㉗ プレイヤーが動かしたら
    if (playerTurnFinished)
    {
        turn++;

        playerTurnFinished = false;

        CheckFavoriteFood();

        StartGravity();
    }

    ここが、

    「1ターン終了した瞬間」

    の処理。

    順番は、

    プレイヤーが動かす
          ↓
    ターン終了
          ↓
    turn++
          ↓
    好物チェック
          ↓
    重力開始

    となっている。

    ㉘ 最後に好物を更新
    if (foodRising)
    {
        UpdateFavoriteFood();
    }

    好物が上昇中なら、毎フレーム少しずつ上げる。

    そして、

    if (turn >= 3 &&
        foodBlockIndex == -1)
    {
        MakeFavoriteFood();
    }

    で3ターン目以降に好物を出そうとする。
    */
}

// ==================================================
// ゲーム描画
// ==================================================
void DrawGame()
{
    DrawGameScreen(
        turn,
        score,
        gameOver
    );
    /*
    ㉙ DrawGame()

    最後は、

    void DrawGame()
    {
        DrawGameScreen(
            turn,
            score,
            gameOver
        );
    }

    となっている。

    これは、

    「画面を描いてください」とDraw.cppにお願いする関数

    だね。

    実際の描画処理はDraw.cppに任せている。
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
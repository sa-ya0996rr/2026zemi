/*
まず Draw.cpp の役割

このファイルの中心は、

void DrawGameScreen(
    int turn,
    int score,
    bool gameOver)

という関数。

この関数を使って、

背景
タイトル
ターン数
スコア
操作説明
ゲーム盤
マス目
羊・牛・小麦・とうもろこし
GAME OVER画面

を画面に描画している。

つまり、

Block.cpp = ブロックの情報や判定を管理する
Draw.cpp = その情報を画面に表示する

という分担になっている。
*/

#include "DxLib.h"
#include "Draw.h"
#include "Block.h"
#include <cstdio>
/*
① #include
#include "DxLib.h"
#include "Draw.h"
#include "Block.h"
#include <cstdio>

それぞれ役割が違う。

DxLib.h
#include "DxLib.h"

DxLibの機能を使うため。

例えば、

DrawBox()
DrawString()
DrawLine()
GetColor()

などが使えるようになる。

Draw.h
#include "Draw.h"

このファイルで使うDrawGameScreen()などの宣言を読み込む。

Block.h
#include "Block.h"

これによって、

BOARD_X
BOARD_Y
WIDTH
HEIGHT
CELL

blockCount
blockExist[]
blockX[]
blockY[]
blockLength[]
blockType[]
blockDrawY[]

など、ブロックに関係する情報を使える。

つまりDraw.cppは、

Block.cppで管理しているブロック情報を読み取って画面に描画する

ことができる。

<cstdio>
#include <cstdio>

これは、

sprintf_s()

を使うため。

TURN : 0やSCORE : 100のように、数字を文字列に変換して表示するために使っている。
*/

// ==================================================
// ゲーム画面
// ==================================================
void DrawGameScreen(
    int turn,
    int score,
    bool gameOver)
    /*
    ② DrawGameScreen() ― 関数
    void DrawGameScreen(
    int turn,
    int score,
    bool gameOver)

    これは、

    ゲーム画面を描画するための関数

    だよ。

    なぜ関数にする？

    ゲームループから、

    DrawGameScreen(turn, score, gameOver);

    と呼ぶだけで、

    「現在のゲーム状態を画面に描いて！」

    と指示できる。

    毎回、
    
    DrawBox(...)
    DrawString(...)
    DrawLine(...)

    などを全部書かなくていい。

    つまり関数を使うことで、画面描画の処理をひとまとめにできる。

    ③ int turn
    int turn

    これは現在のターン数。

    例えば、

    TURN : 0
    TURN : 1
    TURN : 2

    など。

    ターン数は整数なので、

    int

    を使っている。

    ④ int score
    int score

    これはスコア。

    例えば、

    0
    100
    200

    など。

    これも整数なのでint。

    ⑤ bool gameOver
    bool gameOver

    これはゲームオーバーかどうか。

    true  → ゲームオーバー
    false → ゲーム中

    という2種類しか必要ない。

    だからboolが適している。
    */
{
    // ==========================================
    // 背景
    // ==========================================
    DrawBox(
        0,
        0,
        1200,
        800,
    GetColor(
        180,
        220,
        180),
        TRUE);
    /*
    ⑥ 背景
    DrawBox(
       0,
        0,
        1200,
        800,
        GetColor(
            180,
            220,
            180),
        TRUE);

    これは画面全体に四角形を描いている。

    左上       右上
    (0,0) ───── (1200,0)
      │             │
      │   背景      │
      │             │
    (0,800) ─── (1200,800)

    つまりゲーム画面全体を緑色にしている。

    DrawBox()

    DrawBox()はDxLibの関数で、

    四角形を描く

    ためのもの。

    GetColor()
    GetColor(180, 220, 180)

    は、

    RGBの数値から色を作る

    関数。

    180 → 赤
    220 → 緑
    180 → 青

    という指定。

    TRUE

    最後の、

    TRUE

    は、

    四角形を塗りつぶす

    という指定。
    */

    // ==========================================
    // タイトル
    // ==========================================
    DrawString(
        500,
        20,
        "FARM PUZZLE",
        GetColor(
            0,
            0,
            0));
    /*
    ⑦ タイトル
    DrawString(
        500,
        20,
        "FARM PUZZLE",
        GetColor(0, 0, 0));

    これは画面に、

    FARM PUZZLE

    と表示している。

    DrawString()は、

    文字を画面に表示するDxLibの関数
    */

    // ==========================================
    // TURN
    // ==========================================
    char turnText[50];

    sprintf_s(
        turnText,
        "TURN : %d",
        turn);
    /*
    ⑧ ターン表示

    ここは少し重要。

    char turnText[50];

    sprintf_s(
        turnText,
        "TURN : %d",
        turn);
    char turnText[50]

    これは、

    文字列を保存するための配列

    。

    charは1文字を保存する型だったね。

    だから、

    char turnText[50];

    は、

    最大50文字程度の文字を入れられる配列

    という意味。

    なぜ配列？

    画面に表示したいのは、

    TURN : 0

    のような文字。

    ターン数は毎回変わる。

    例えば、

    TURN : 0
    TURN : 1
    TURN : 2

    となる。

    そこで、

    sprintf_s()

    を使って数字を文字列の中に入れている。

    ⑨ sprintf_s()
    sprintf_s(
        turnText,
        "TURN : %d",
        turn);

    これは、

    数字などのデータを文字列に組み立てる

    処理。

    %dの部分に、

    turn

    の値が入る。

    例えば、

    turn = 5;

    なら、

    TURN : 5

    という文字列が作られる。
    */

    DrawString(
        50,
        20,
        turnText,
        GetColor(
            0,
            0,
            0));

    // ==========================================
    // SCORE
    // ==========================================
    char scoreText[50];

    sprintf_s(
        scoreText,
        "SCORE : %d",
        score);
    /*
    ⑩ SCOREも同じ
    char scoreText[50];

    sprintf_s(
        scoreText,
        "SCORE : %d",
        score);

    これも同じ。

    例えば、

    score = 300;

    なら、

    SCORE : 300

    という文字列を作る。
    */

    DrawString(
        950,
        20,
        scoreText,
        GetColor(
            0,
            0,
            0));

    // ==========================================
    // 説明
    // ==========================================
    DrawString(
        40,
        90,
        "S = SHEEP",
        GetColor(
            0,
            0,
            0));

    DrawString(
        40,
        120,
        "C = COW",
        GetColor(
            0,
            0,
            0));

    DrawString(
        40,
        150,
        "W = WHEAT",
        GetColor(
            0,
            0,
            0));

    DrawString(
        40,
        180,
        "T = CORN",
        GetColor(
            0,
            0,
            0));

    DrawString(
        40,
        220,
        "DRAG ANIMAL",
        GetColor(
            0,
            0,
            0));

    DrawString(
        40,
        250,
        "MOVE 1 CELL = END TURN",
        GetColor(
            0,
            0,
            0));
    /*
    ⑪ 説明文
    DrawString(40, 90, "S = SHEEP", ...);
    DrawString(40, 120, "C = COW", ...);
    DrawString(40, 150, "W = WHEAT", ...);
    DrawString(40, 180, "T = CORN", ...);

    これはプレイヤーに、

    S = SHEEP
    C = COW
    W = WHEAT
    T = CORN

    と説明するため。

    その下の、

    DrawString(40, 220, "DRAG ANIMAL", ...);
    DrawString(40, 250, "MOVE 1 CELL = END TURN", ...);

    は操作方法。
    */

    // ==========================================
    // マップ
    // ==========================================
    DrawBox(
        BOARD_X,
        BOARD_Y,
        BOARD_X + WIDTH * CELL,
        BOARD_Y + HEIGHT * CELL,
        GetColor(
            220,
            210,
            170),
        TRUE);
    /*
    ⑫ マップを描く
    DrawBox(
        BOARD_X,
        BOARD_Y,
        BOARD_X + WIDTH * CELL,
        BOARD_Y + HEIGHT * CELL,
        GetColor(220, 210, 170),
        TRUE);

    ここではゲーム盤そのものを描いている。

    今の設定だと、

    WIDTH = 9
    HEIGHT = 10
    CELL = 60

    なので、

    横 = 9 × 60 = 540px
    縦 = 10 × 60 = 600px

    の盤面になる。

    そして、

    BOARD_X = 330
    BOARD_Y = 50

    なので、

    画面の(330, 50)から盤面を描く

    ということ。
    */

    // ==========================================
    // マス目
    // ==========================================
    for (int y = 0; y <= HEIGHT; y++)
    {
        DrawLine(BOARD_X, BOARD_Y + y * CELL, BOARD_X + WIDTH * CELL, BOARD_Y + y * CELL,
            GetColor(
                150,
                150,
                150));
    }
    /*
    ⑬ for文 ― 横線
    for (int y = 0; y <= HEIGHT; y++)
    {
        DrawLine(...);
    }

    これは、

    盤面の横線を1本ずつ描く

    ため。

    例えば10マスなら、

    0
    1
    2
    3
    ...
    10

    まで線を描く。

    なぜ for文？

    同じような処理を何回もするから。

    もしforを使わなければ、

    DrawLine(...);
    DrawLine(...);
    DrawLine(...);
    DrawLine(...);

    と大量に書くことになる。

    for文なら、

    for (...)
    {
        DrawLine(...);
    }

    だけで済む。

    つまり、

    同じ処理を繰り返すためにfor文を使っている。
    */

    for (int x = 0; x <= WIDTH; x++)
    {
        DrawLine(BOARD_X + x * CELL, BOARD_Y, BOARD_X + x * CELL, BOARD_Y + HEIGHT * CELL,
            GetColor(
                150,
                150,
                150));
    }
    /*
    ⑭ 縦線もfor文
    for (int x = 0; x <= WIDTH; x++)
    {
        DrawLine(...);
    }

    これは縦線。

    横線と同じ考え方。

    横線 → yを変える
    縦線 → xを変える

    ことで9×10のマス目を作っている。
    */

    // ==========================================
    // ブロック
    // ==========================================
    for (int i = 0; i < blockCount; i++)
        /*
        ⑮ ブロックを描くfor文
        for (int i = 0; i < blockCount; i++)

        これはかなり重要。

        存在しているブロックを1個ずつ描画する

        ため。

        例えば6個なら、

        i = 0
        i = 1
        i = 2
        i = 3
        i = 4
        i = 5

        と処理する。
        */

    {
        if (blockExist[i] == false)
        {
            continue;
        }
        /*
        ⑯ if文で存在するか確認
        if (blockExist[i] == false)
        {
            continue;
        }

        これは、

        存在していないブロックは描画しない

        ということ。

        blockExist[i]が、

        true  → 描画する
        false → 描画しない

        となる。

        なぜcontinue？

        存在しないブロックを見つけたら、

        「このブロックの描画処理は飛ばして、次のブロックへ行こう」

        としている。
        */

        int x = BOARD_X + blockX[i] * CELL;
        /*
        ⑰ ブロックの座標
        int x = BOARD_X + blockX[i] * CELL;

        これは、

        ブロックのゲーム内のマス位置を、画面上のピクセル位置に変換

        している。

        例えば、

        blockX = 2
        CELL = 60
        BOARD_X = 330

        なら、

        330 + 2 × 60
        = 450

        となる。
        */

        int y = (int)blockDrawY[i];
        /*
        ⑱ Y座標
        int y = (int)blockDrawY[i];

        blockDrawYはfloatだった。

        でもここでは、

        int y

        として画面描画に使っている。

        そのため、

        (int)

        で整数に変換している。

        これは型変換。
        */

        int width = blockLength[i] * CELL;
        /*
        ⑲ ブロックの幅
        int width = blockLength[i] * CELL;

        例えば、

        blockLength = 3
        CELL = 60

        なら、

        3 × 60 = 180px

        。

        つまり、

        ■■■

        という3マスのブロックを180px幅で描く。
        */

        int color;
        /*
        ⑳ int color
        int color;

        これは、

        ブロックを何色で描くか

        を保存する変数。

        後で、

        color = GetColor(...);

        として種類ごとに色を変える。
        */

        // 羊
        if (blockType[i] == 'S')
        {
            color = GetColor(
                        245,
                        245,
                        245);
        }

        // 牛
        else if (blockType[i] == 'C')
        {
            color = GetColor(
                        80,
                        80,
                        80);
        }


        // ======================================
        // 好物
        // ======================================
		// 小麦
        else if (blockType[i] == 'W')
        {
            color = GetColor(
                        240,
                        210,
                        100);
        }

		// とうもろこし
        else
        {
            color = GetColor(
                        250,
                        220,
                        80);
        }


        /*
        ！！！「羊」から


        ㉑ if / else if / else

        ここは今回のゲームで分かりやすい条件分岐。

        if (blockType[i] == 'S')
        {
            // 羊
        }
        else if (blockType[i] == 'C')
        {
            // 牛
        }
        else if (blockType[i] == 'W')
        {
            // 小麦
        }
        else
        {
            // とうもろこし
        }

        つまり、

        Sなら → 羊の色
        Cなら → 牛の色
        Wなら → 小麦の色
        それ以外なら → とうもろこし

        という処理。

        なぜswitch文ではなくif文？

        今回のコードでは、

        if
        else if
        else

        を使っている。

        blockType[i]が文字なので、switchにすることもできる。

        例えば、

        switch (blockType[i])
        {
        case 'S':
            // 羊
            break;

        case 'C':
            // 牛
            break;

        case 'W':
            // 小麦
            break;

        case 'T':
            // とうもろこし
            break;
        }

        という書き方も可能。

        今のコードはif文で作られているので、現時点では「if・else if・elseを使って種類ごとに処理を分けている」と説明できる。
        */

        // ======================================
        // ブロック本体
        // ======================================
        DrawBox(
            x + 3,
            y + 3,
            x + width - 3,
            y + CELL - 3,
            color,
            TRUE);
        /*
        ㉒ ブロック本体
        DrawBox(
            x + 3,
            y + 3,
            x + width - 3,
            y + CELL - 3,
            color,
            TRUE);

        これはブロックを塗りつぶしている。

        +3や-3を入れているのは、

        マス目の線と少し間を空ける

        ため。
        */

        // ======================================
        // 枠
        // ======================================
        DrawBox(
            x + 3,
            y + 3,
            x + width - 3,
            y + CELL - 3,
            GetColor(
                80,
                80,
                80),
            FALSE);
        /*
        ㉓ ブロックの枠
        DrawBox(
            x + 3,
            y + 3,
            x + width - 3,
            y + CELL - 3,
            GetColor(80, 80, 80),
            FALSE);

        ここでは最後が、

        FALSE

        になっている。

        さっきのTRUEとは違って、

        塗りつぶさず、枠線だけ描く

        という指定。

        だから、

        TRUE  → 中を塗る
        FALSE → 枠だけ

        と考えればOK。
        */

        // ======================================
        // S / C / W / T
        // ======================================
        char text[2];
        /*
        ㉔ char text[2]
        char text[2];

        これは、

        ブロックの文字を表示するための文字配列

        。

        例えば、

        S

        を表示したい。
        */

        text[0] = blockType[i];

        text[1] = '\0';
        /*
        ㉕ text[0]とtext[1]
        text[0] = blockType[i];
        text[1] = '\0';

        ここは少しC++らしい部分。

        例えば、

        blockType[i] = 'S';

        なら、

        text[0] = 'S'
        text[1] = '\0'

        となる。

        \0は、

        ここで文字列が終わり

        という印。

        だから、

        "S"

        という文字列としてDrawString()に渡せる。
        */

        DrawString(
            x + 20,
            y + 15,
            text,
            GetColor(
                0,
                0,
                0));
        /*
        ㉖ DrawString()
        DrawString(
            x + 20,
            y + 15,
            text,
            GetColor(0, 0, 0));

        ここで実際に、

        S
        C
        W
        T

        をブロックの中に表示している。
        */
    }

    // ==========================================
    // GAME OVER
    // ==========================================
    if (gameOver)
        /*
        ㉗ GAME OVERのif文

        最後。

        if (gameOver)
        {
            ...
        }

        これは、

        ゲームオーバーになった場合だけ、GAME OVER画面を表示する

        ということ。

        gameOverはboolなので、

        false → 何もしない
        true  → GAME OVERを描く

        になる。
        */
    {
        DrawBox(
            350,
            300,
            850,
            450,
            GetColor(
                255,
                255,
                255),
            TRUE);

        DrawString(
            500,
            340,
            "GAME OVER",
            GetColor(
                0,
                0,
                0));

        char finalScore[50];

        sprintf_s(finalScore, "SCORE : %d", score);

        DrawString(
            500,
            390,
            finalScore,
            GetColor(
                0,
                0,
                0));
        /*
        ㉘ GAME OVER画面
        DrawBox(
            350,
            300,
            850,
            450,
            GetColor(255, 255, 255),
            TRUE);

        白い四角を表示して、その上に、

        DrawString(
            500,
            340,
            "GAME OVER",
            ...);

        で文字を表示。

        さらに、

        char finalScore[50];

        sprintf_s(finalScore, "SCORE : %d", score);

        で最終スコアを作って、

        DrawString(
            500,
            390,
            finalScore,
            ...);

        で表示している。
        */
    }
}

/*
この Draw.cpp で使っているもの

先生に説明するときに整理すると、こんな感じ。

種類	        このファイルでの使用例	    目的
if文	        if (gameOver)	            ゲームオーバー表示などの条件判定
else if文	    blockType[i] == 'C'など	    ブロック種類ごとの処理
else文	        とうもろこしの処理	        それ以外の種類を処理
for文	        マス目・ブロックの描画	    同じ処理を繰り返す
continue	    存在しないブロックを飛ばす	不要な描画をしない
関数	        DrawGameScreen()	        描画処理をまとめる
配列	        turnText[50]など	        文字列を保存
変数	        x, y, width, color	        描画に必要な情報を保存
int型	        座標、スコア、ターン	    整数を扱う
bool型	        gameOver	                ON/OFF状態を扱う
char型	        text[2]	                    1文字・文字列を扱う
型変換	        (int)blockDrawY[i]	        floatをintに変換
DxLibの関数	    DrawBox()など	            実際に画面へ描画

特に覚えておくといいところ

このDraw.cppは、

ゲームの中身を計算するファイルではなく、計算された結果を「見える形」にするファイル。

例えば、

Block.cpp
「羊は横3マス、縦8段にいる」
        ↓
Draw.cpp
「じゃあ画面のこの場所に羊のブロックを描こう」

という関係。

だから、

Block.cpp → ブロックの管理・判定
Player.cpp → プレイヤーの操作
Game.cpp → ゲーム全体の進行
Draw.cpp → 画面への表示

という役割分担になっている。

そして今回のDraw.cppは、if文・else if文・else文・for文・関数・配列・変数・int・bool・char・型変換などを実際に使っているファイルだよ。
*/
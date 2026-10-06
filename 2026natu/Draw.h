/*
Draw.hの役割

今のDraw.hは、

#pragma once

void DrawGameScreen(
    int turn,
    int score,
    bool gameOver);

だけ。

これは一言でいうと、

「Draw.cppには、ゲーム画面を描くDrawGameScreen()という関数があります」と他のファイルに教えるためのファイル

だよ。

実際の描画処理はDraw.cppに書いてある。
*/

#pragma once
/*
① #pragma once
#pragma once

これは、

このDraw.hを同じプログラムの中で何回も読み込まないようにする

ためのもの。

例えばGame.cppから、

#include "Draw.h"

として読み込む。

#pragma onceがあることで、同じヘッダーファイルが重複して読み込まれることを防げる。
*/

void DrawGameScreen(
    /*
    ② void
    void DrawGameScreen(...)

    このvoidは、

    この関数は結果を返さない

    という意味。

    今回のDrawGameScreen()は、

    ゲーム画面を描く
    ↓
    処理終了

    という関数。

    「100を返す」とか「trueを返す」という必要がない。

    だから、

    void

    を使っている。

    ③ DrawGameScreen
    DrawGameScreen

    これは関数の名前。

    名前から分かるように、

    ゲーム画面を描画する

    ための関数。

    実際の処理はDraw.cppにある。

    void DrawGameScreen(...)
    {
        // 背景
        // タイトル
        // ブロック
        // スコア
        // GAME OVER
        ...
    }

    という形になっている。
    */

    int turn,
    /*
    ④ int turn
    int turn

    これは、

    現在のターン数をDrawGameScreenに渡す

    ためのもの。

    例えば、

    turn = 5;

    なら、Draw.cppで、

    TURN : 5

    と表示できる。

    turnは整数なので、

    int

    を使っている。
    */

    int score,
    /*
    ⑤ int score
    int score

    これは、

    現在のスコアをDrawGameScreenに渡す

    ためのもの。

    例えば、

    score = 300;

    なら、

    SCORE : 300

    と表示できる。

    スコアも整数なのでint。
    */

    bool gameOver);
    /*
    ⑥ bool gameOver
    bool gameOver

    これは、

    現在ゲームオーバーなのかどうかをDrawGameScreenに渡す

    ためのもの。

    boolなので、

    true
    false

    の2種類。

    例えば、

    gameOver = false;

    なら通常のゲーム画面。

    gameOver = true;

    なら、

    GAME OVER
    SCORE : ○○○

    を表示する。

    実際にDraw.cppでは、

    if (gameOver)
    {
        // GAME OVERを描画
    }

    として使っているね。
    */

/*
⑦ なぜDraw.hに処理を書かないの？

ここが.hと.cppの重要な違い。

Draw.hでは、

void DrawGameScreen(
    int turn,
    int score,
    bool gameOver);

とだけ書いている。

これは、

「こういう関数がありますよ」

という宣言。

一方、Draw.cppには、

void DrawGameScreen(
    int turn,
    int score,
    bool gameOver)
{
    // 実際の描画処理
}

と書いてある。

これは関数の中身。

Draw.hとDraw.cppの関係

簡単にすると、

Draw.h
↓
「DrawGameScreenという関数があります」
        ↓
Draw.cpp
↓
「この関数では実際にこうやって画面を描きます」

という関係。

⑧ なぜDraw.hを作るの？

例えばGame.cppからゲーム画面を描きたい場合、

#include "Draw.h"

としておけば、

DrawGameScreen(
    turn,
    score,
    gameOver);

と呼び出せる。

つまり、

他のcppファイルからDraw.cppの関数を使えるようにする

ためにDraw.hがある。

このファイルで使っているもの

先生に説明するなら、Draw.hはかなりシンプルにまとめられる。

使っているもの	目的
#pragma once	ヘッダーファイルの重複読み込みを防ぐ
関数	DrawGameScreen()として描画処理をまとめる
void	関数から値を返さない
int型	ターン数・スコアを扱う
bool型	ゲームオーバーかどうかを扱う
一番簡単に説明するなら

Draw.hは、ゲーム画面を描画するためのDrawGameScreen()関数を他のファイルから使えるようにするためのヘッダーファイル。

int turnでターン数、int scoreでスコア、bool gameOverでゲームオーバー状態を受け取り、それらをDraw.cppに渡して画面に表示できるようにしている。

これでDraw.cppとDraw.hの役割がセットで説明できるよ。
    */
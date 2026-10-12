#pragma once
    
void DrawGameScreen(

    int turn,

    int score,

    bool gameOver);

/*
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
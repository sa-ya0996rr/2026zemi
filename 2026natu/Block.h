#pragma once
/*
① #pragma once
#pragma once

これは、
このヘッダーファイルを1回だけ読み込む
ためのもの。

今回、Game.cppやPlayer.cppなどから、
#include "Block.h"
として使っている。

もし同じヘッダーファイルが何回も読み込まれてしまうと、同じ変数や定数などが重複してしまうことがある。

それを防ぐのが、
#pragma once
だよ。
*/

const int WIDTH = 9;
const int HEIGHT = 10;
/*
② ゲーム盤の大きさ
const int WIDTH = 9;
const int HEIGHT = 10;

これはゲーム盤の大きさ。

横 9マス
縦10マス
という意味。

なぜ const？
const
は、
あとから値を変更しない
という意味。

今回、ゲーム中に盤面の大きさが、
9マス → 10マス
と変わることはない。

だから、
const int
にしている。

なぜ int？
9や10という整数を保存するので、
int
を使っている。

つまり、
const int WIDTH = 9;
は、
「WIDTHという名前で、変更できない整数9を保存する」
という意味。
*/

const int CELL = 60;
/*
③ CELL
const int CELL = 60;

これは、
1マスを何ピクセルで描画するか
を決めている。

今回、
1マス = 60 × 60ピクセル
になっている。

だから盤面全体は、
横：9 × 60 = 540px
縦：10 × 60 = 600px
になる。
*/

const int BOARD_X = 330;
/*
④ BOARD_X
const int BOARD_X = 330;

これは、
盤面の左端が画面のどこにあるか
を決めている。

今はゲーム画面が、
SetGraphMode(1200, 800, 32);
なので横1200px。
盤面の横幅は540px。

そのため、
(1200 - 540) ÷ 2 = 330
となって、330にすると中央になる。
*/

const int BOARD_Y = 100;
/*
⑤ BOARD_Y
const int BOARD_Y = 50;

これは、
盤面の上端を画面の上から何ピクセル下にするか
を決めている。

つまり、

画面上から50px
↓
盤面開始

ということ。
*/

const int MAX_BLOCK = 100;
/*
⑥ MAX_BLOCK
const int MAX_BLOCK = 100;

これは、
ゲーム内で管理できるブロックの最大数
を決めている。

今回、
最大100個
まで管理できる。

これもゲーム中に変更する必要がないので、
const int
になっている。
*/

extern char blockType[MAX_BLOCK];
/*
⑦ externとは？

ここが Block.h で一番重要なところ。
例えば、
extern char blockType[MAX_BLOCK];
がある。

これは、
「blockTypeという配列は別のファイルで作られているけど、このファイルでも使います」
という宣言。

実際に作っているのは Block.cpp の、
char blockType[MAX_BLOCK];
の方。

つまり、

Block.cpp
char blockType[MAX_BLOCK];
↓
実際の配列を作る

Block.h
extern char blockType[MAX_BLOCK];
↓
他のファイルでも使えるように知らせる

という関係。

⑧ blockType
extern char blockType[MAX_BLOCK];

これは、
各ブロックが何の種類なのかを保存する配列。

例えば、

blockType[0] = 'S'
blockType[1] = 'C'
blockType[2] = 'S'

なら、

0番 → 羊
1番 → 牛
2番 → 羊

となる。

charなのは、

S
C
W
T

のように1文字で種類を表しているから。
*/

extern int blockX[MAX_BLOCK];
/*
⑨ blockX
extern int blockX[MAX_BLOCK];

これは、
各ブロックの横方向のマス位置
を保存する。

例えば、
blockX[0] = 3;
なら、
0番のブロックは横3マス目から始まっている
という意味。
*/

extern int blockY[MAX_BLOCK];
/*
⑩ blockY
extern int blockY[MAX_BLOCK];

これは、
各ブロックの縦方向の位置
を保存する。

例えば、
blockY[0] = 8;
なら、
0番のブロックは8段目
ということ。
*/

extern int blockLength[MAX_BLOCK];
/*
⑪ blockLength
extern int blockLength[MAX_BLOCK];

これは、
ブロックが横に何マス続いているか
を保存する。

例えば、
blockLength[0] = 3;
なら、
■■■
という3マスのブロック。
*/

extern bool blockExist[MAX_BLOCK];
/*
⑫ blockExist
extern bool blockExist[MAX_BLOCK];

これは、
そのブロックが現在存在しているか
を保存する。

boolなので、

true  → 存在する
false → 存在しない

の2種類。

例えばブロックを消した場合、

blockExist[3] = false;

として、
「3番のブロックはもう存在しない」
と扱える。
*/

extern float blockDrawY[MAX_BLOCK];
/*
⑬ blockDrawY
extern float blockDrawY[MAX_BLOCK];

これは、
画面にブロックを描画するときのY座標
を保存する。

ここは blockY とは少し違う。

blockY
何段目にいるか
blockDrawY
画面上の何ピクセルの位置に描画するか
という違い。

なぜ float？
ブロックを、一気に落とすのではなく、

少しずつ
↓
少しずつ
↓
少しずつ

と動かしてアニメーションさせるため。

例えば、

500.0
504.0
508.0
512.0

のように動かせる。

floatなら小数も使えるので、滑らかなアニメーションを作れる。
*/

extern bool falling[MAX_BLOCK];
/*
⑭ falling
extern bool falling[MAX_BLOCK];

これは、
そのブロックが現在落下中か
を表す。

true  → 落下中
false → 落下していない
*/

extern bool rising[MAX_BLOCK];
/*
⑮ rising
extern bool rising[MAX_BLOCK];

これは、
そのブロックが現在上昇中か
を表す。

盤面が1段上がるときなどに使う。
*/

extern bool newBlockRising;
/*
⑯ newBlockRising
extern bool newBlockRising;

これは、
新しく出てきたブロックが下から上昇中か
を表す。

さっきの rising と似ているけど、こちらは新しく出現するブロック専用の状態。
*/

extern int blockCount;
/*
⑰ blockCount
extern int blockCount;

これは、
現在登録されているブロックの数
を管理する。

例えば、
blockCount = 6
なら、現在6個のブロックを管理している。
*/

void AddBlock(char type, int x, int y, int length);
void MakeInitialBlocks();
bool IsOverlapping(int x1, int length1, int x2, int length2);
bool HasSupport(int number);
void StartRising();
/*
⑱ 関数の宣言

最後にこれらがある。

void AddBlock(char type, int x, int y, int length);
void MakeInitialBlocks();
bool IsOverlapping(int x1, int length1, int x2, int length2);
bool HasSupport(int number);
void StartRising();

これは、
「こういう関数がありますよ」と他のcppファイルに教える
ためのもの。

実際の処理は Block.cpp に書いてある。

AddBlock()
void AddBlock(char type, int x, int y, int length);

ブロックを追加する関数。

例えば、
AddBlock('S', 0, 8, 2);
とすれば、
羊・横0・縦8・長さ2
のブロックを追加できる。

MakeInitialBlocks()
void MakeInitialBlocks();

ゲーム開始時のブロックを作る。

IsOverlapping()
bool IsOverlapping(...);

2つのブロックが横方向に重なっているか調べる。

結果が、

重なる → true
重ならない → false

なので bool。

HasSupport()
bool HasSupport(int number);

そのブロックの下に支えがあるか調べる。

支えがある → true
支えがない → false

なのでこれも bool。

StartRising()
void StartRising();

ブロックを上昇させる処理を開始するための関数。

Block.hとBlock.cppの関係
ここを理解するとかなり分かりやすい。

Block.h
「何があるか」を宣言する場所

定数
↓
ブロックの配列
↓
状態を表す変数
↓
関数の名前

Block.cpp
「実際にどう動くか」を書く場所

AddBlock()
↓
MakeInitialBlocks()
↓
IsOverlapping()
↓
HasSupport()

という処理を書く。

つまり、

Block.h = ブロックに関する情報・機能の一覧表
Block.cpp = その一覧表に書かれた機能の中身

と考えると分かりやすいよ。
*/

/*
今の Block.h で使っているもの
今回先生に「どんなものを使っている？」と説明する場合、このファイルだけでも、

定数 → WIDTH, HEIGHT, CELL, BOARD_X, BOARD_Y, MAX_BLOCK
配列 → blockType[], blockX[] など
変数 → blockCount
関数 → AddBlock()など
char型 → 'S', 'C'など
int型 → 座標や長さなど
bool型 → 状態管理
float型 → 描画位置
extern → 他のcppファイルでも同じ変数を使う
const → 値を変更しないようにする

というものを使っている。
*/
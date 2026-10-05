/*
Block.cpp の役割=ゲームに登場するブロックの情報を管理するファイル

例えば、
羊なのか牛なのか
どこにあるのか
何マスの長さなのか
存在しているのか
落下中なのか
上昇中なのか
などをここで管理している。
*/

#include "Block.h" 
/*
① #include "Block.h"

#include "Block.h"
これは、
Block.h に書いてある情報を、この Block.cpp でも使えるようにする
今回の Block.h には、
const int WIDTH = 9;
const int HEIGHT = 10;
const int CELL = 60;
const int BOARD_X = 280;
const int BOARD_Y = 50;
などが書いてある。

だから Block.cpp で、
BOARD_Y
CELL
HEIGHT
MAX_BLOCK
などを使える。
*/

// ==================================================
// ブロックの変数
// ==================================================
char blockType[MAX_BLOCK];
/*
② ブロックの情報を保存する「配列」

char blockType[MAX_BLOCK];
これは、それぞれのブロックが何のブロックなのかを保存する配列

例えば、
blockType[0] = 'S'
blockType[1] = 'C'
blockType[2] = 'S'
なら、
0番 → 羊
1番 → 牛
2番 → 羊
という意味。
*/

int blockX[MAX_BLOCK];
int blockY[MAX_BLOCK];
/*
③ blockX と blockY
int blockX[MAX_BLOCK];
int blockY[MAX_BLOCK];

これはブロックの位置。

例えば、
blockX[0] = 3;
blockY[0] = 8;
なら、
0番のブロックは横3マス、縦8段目にある
ということ。

なぜ int？
int は整数を保存する型。

今回のブロックの位置は、
0
1
2
3
...
というマス単位の整数なので int が合っている。
*/

int blockLength[MAX_BLOCK];
/*
④ blockLength
int blockLength[MAX_BLOCK];

これは、
ブロックが横に何マス続いているか
を保存する。

例えば、
blockLength[0] = 2;
なら、
■■
の2マスブロック。

blockLength[1] = 4;
なら、
■■■■
の4マスブロック。

これも整数なので int。
*/

bool blockExist[MAX_BLOCK];
/*
⑤ blockExist
bool blockExist[MAX_BLOCK];

これは、
そのブロックが現在存在しているか
を保存している。

bool は、
true  → はい
false → いいえ
のように、2種類の状態を保存する型。

例えば、
blockExist[0] = true;
なら、
0番のブロックは存在している
ということ。

逆に、
blockExist[0] = false;
なら、
0番のブロックは存在しない
ということ。

これはゲームではかなり便利。
ブロックを消したとしても、配列の中身を全部詰め直す必要がなく、

blockExist[i] = false;

とするだけで「このブロックはもう存在しない」と扱える。
*/

// ==================================================
// 描画位置
// ==================================================
float blockDrawY[MAX_BLOCK];
/*
⑥ blockDrawY
float blockDrawY[MAX_BLOCK];

これは少し重要。

blockY は、
ブロックが何段目にいるか
を表している。

一方、
blockDrawY
は、
画面上で実際にどの高さに描画するか
を表している。

例えば、
8段目 → 530px
9段目 → 590px
だったとしても、落下アニメーションでは、

530
534
538
542
...
590

と少しずつ動かしたい。

そのため、
float
を使っている。

float は小数を扱える型。
*/

// ==================================================
// アニメーション
// ==================================================
bool falling[MAX_BLOCK];
bool rising[MAX_BLOCK];
/*
⑦ falling と rising
bool falling[MAX_BLOCK];
bool rising[MAX_BLOCK];

これはブロックの状態。

例えば、
falling[3] = true;
なら、
3番のブロックは現在落下中
ということ。

rising[3] = true;
なら、
3番のブロックは現在上昇中
ということ。

ここでも「はい・いいえ」で表せるので bool。
*/

bool newBlockRising = false;
/*
⑧ newBlockRising
bool newBlockRising = false;

これは、
新しく出てきたブロックが上昇中か
を管理する変数。

false でスタートしているので、
最初は新しいブロックは上昇していない
という意味。
*/

// ==================================================
// ブロック数
// ==================================================
int blockCount = 0;
/*
⑨ blockCount
int blockCount = 0;

これは、
現在登録されているブロックの数
を管理している。

例えば、
blockCount = 6
なら、
0～5番までの6個のブロックを使っている
というイメージ。
*/

// ==================================================
// ブロック追加
// ==================================================
void AddBlock(
    char type, 
    int x, 
    int y, 
    int length)
/*
⑩ AddBlock() 関数
ここからが「関数」。

void AddBlock(char type, int x, int y, int length)

これは、
ブロックを1個追加するための関数
だよ。

関数にすることで、
AddBlock('S', 0, 8, 2);
と書くだけで、
羊を横0、縦8、長さ2で追加
できる。

もし関数がなかったら、ブロックを追加するたびに同じ処理を何行も書く必要がある。
だから、
同じ処理をまとめて、何度でも使えるようにする
ために関数を使っている。
*/
{
    if (blockCount >= MAX_BLOCK)
    {
        return;
    }
    /*
    ⑪ if文
    最初に出てくるのがこれ。

    if (blockCount >= MAX_BLOCK)
    {
        return;
    }

    これは、
    ブロックが最大数に達していたら、これ以上追加しない
    という判定。

    例えば、
    MAX_BLOCK = 100
    なのに、
    blockCount = 100
    だったら、101個目を入れることはできない。

    そこで if文 を使って、
    もし100個以上なら
    追加しない
    としている。

    ⑫ return
    return;
    は、
    今実行している関数をここで終了する
    という意味。

    つまり、
    if (blockCount >= MAX_BLOCK)
    {
        return;
    }

    は、
    「もうブロックを入れられないなら、AddBlock関数をここで終了」
    ということ。
    */

    blockType[blockCount] = type;
    blockX[blockCount] = x;
    blockY[blockCount] = y;
    blockLength[blockCount] = length;
    blockExist[blockCount] = true;
    /*
    ⑬ 配列にブロックの情報を入れる
    blockType[blockCount] = type;
    blockX[blockCount] = x;
    blockY[blockCount] = y;
    blockLength[blockCount] = length;
    blockExist[blockCount] = true;

    ここで、
    新しいブロックの情報を配列に保存
    している。

    例えば、
    AddBlock('S', 0, 8, 2);
    なら、

    blockType[0]   → 'S'
    blockX[0]      → 0
    blockY[0]      → 8
    blockLength[0] → 2
    blockExist[0]  → true
    となる。

    つまり、1つのブロックについて複数の配列を使って情報を管理している。
    */

    // 描画位置
    blockDrawY[blockCount] = (float)(BOARD_Y + y * CELL);
    /*
    ⑭ (float) は何？
    blockDrawY[blockCount] = (float)(BOARD_Y + y * CELL);

    ここにある、
    (float)
    は型変換。

    計算結果を float として扱うようにしている。

    今回は blockDrawY が float なので、
    描画位置を小数まで扱えるようにする
    ために使っている。
    */
    
    falling[blockCount] = false;
    rising[blockCount] = false;
    
    blockCount++;
    /*
    ⑮ blockCount++

    最後の、
    blockCount++;
    は、
    blockCountを1増やす
    という意味。

    例えば、

    0 → 1
    1 → 2
    2 → 3

    と増えていく。

    新しいブロックを追加するたびに、
    「次のブロック番号へ進む」
    ために必要。
    */
}

// ==================================================
// 最初のブロック
// ==================================================
void MakeInitialBlocks()
/*
⑯ MakeInitialBlocks()
void MakeInitialBlocks()

これは、
ゲーム開始時のブロックを作る関数

中では、
AddBlock('S', 0, 8, 2);
AddBlock('C', 3, 8, 3);
AddBlock('S', 7, 8, 2);
などを実行している。

つまり、
AddBlock() という関数を使って、最初の盤面を作っている。

これも関数にしておくことで、ゲーム開始時に
MakeInitialBlocks();
と呼ぶだけで初期配置が完成する。
*/
{
    // ------------------------------------------
    // 8段目
    // ------------------------------------------
    AddBlock('S', 0, 8, 2);
    AddBlock('C', 3, 8, 3);
    AddBlock('S', 7, 8, 2);

    // ------------------------------------------
    // 9段目
    // ------------------------------------------
    AddBlock('C', 1, 9, 2);
    AddBlock('S', 4, 9, 3);
    AddBlock('C', 8, 9, 1);
}

// ==================================================
// 横に重なっているか
// ==================================================
bool IsOverlapping(
    int x1,
    int length1,
    int x2,
    int length2)
    /*
    ⑰ IsOverlapping() 重なり判定

    これは今回かなり重要。

    bool IsOverlapping(...)

    名前の通り、
    2つのブロックが横方向に重なっているか調べる関数

    例えば、
    ブロックA
    ■■■

    ブロックB
      ■■■

    なら重なっている。

    なぜ bool？
    結果が、

    重なっている → true
    重なっていない → false

    の2種類だから。

    だから、
    bool
    が適している。
    */
{
    int left1 = x1;
    int right1 = x1 + length1 - 1;
    /*
    ⑱ 左端と右端を計算
    int left1 = x1;
    int right1 = x1 + length1 - 1;

    例えば、
    x = 2
    length = 3
    なら、
    2 3 4
    の3マス。

    だから、

    左端 = 2
    右端 = 4

    になる。
    */

    int left2 = x2;
    int right2 = x2 + length2 - 1;

    if (right1 >= left2 && right2 >= left1)
    {
        return true;
    }
    /*
    ⑲ if で重なりを判定
    if (right1 >= left2 &&
        right2 >= left1)
    {
        return true;
    }

    これは、
    2つのブロックの範囲が重なっているか
    を判定している。

    && は、
    「かつ」
    という意味。

    つまり、

    条件1も正しい
    AND
    条件2も正しい

    なら重なっていると判断する。

    ⑳ return true
    return true;
    は、
    「重なっています」
    と関数の呼び出し元に返している。

    逆に、
    return false;
    なら、
    「重なっていません」
    となる。

    だから他のファイルでは、

    if (IsOverlapping(...))
    {
        // 重なっている
    }

    という使い方ができる。
    */

    return false;
}

// ==================================================
// 下に支えがあるか
// ==================================================
bool HasSupport(int number)
/*
㉑ HasSupport() ― 下に支えがあるか

これは、
このブロックの真下に、支えてくれるブロックがあるか
を調べる関数。

落下処理に必要。

まず、
int belowY = blockY[number] + 1;
で、
今いる段の1つ下
を計算している。

例えば、
blockY = 7
なら、
belowY = 8
*/
{
    int belowY = blockY[number] + 1;
    /*
    ㉒ 一番下かどうか
    if (belowY >= HEIGHT)
    {
        return true;
    }

    これは、
    すでに一番下なら、床に支えられている
    という判定。

    例えば高さ10なら、
    0
    1
    2
    3
    4
    5
    6
    7
    8
    9 ← 一番下

    だから、9段目にいるブロックの下は10。
    belowY = 10;
    となる。

    HEIGHT = 10 なので、
    belowY >= HEIGHT
    が成立。

    そこで、
    return true;
    として、
    「下に支えがあります」
    と扱う。
    */

    // ------------------------------------------
    // 一番下
    // ------------------------------------------
    if (belowY >= HEIGHT)
    {
        return true;
    }

    // ------------------------------------------
    // 他のブロック
    // ------------------------------------------

    for (int i = 0; i < blockCount; i++)
    /*
    ㉓ for文

    ここで for文 が登場する。

    for (int i = 0;
         i < blockCount;
         i++)

    これは、
    盤面にある全てのブロックを1個ずつ調べる
    ため。

    例えば6個なら、
    i = 0
    i = 1
    i = 2
    i = 3
    i = 4
    i = 5

    と順番に調べる。
    */
    {
        if (i == number)
        {
            continue;
        }
        if (blockExist[i] == false)
        {
            continue;
        }
        /*
        ㉔ continue
        if (i == number)
        {
            continue;
        }

        これは、
        今調べているブロック自身だったら、今回は無視して次へ進む
        という意味。
    
        自分自身を「支え」として判定してしまうとおかしくなるから。
        次の、

        if (blockExist[i] == false)
        {
            continue;
        }

        は、
        存在していないブロックなら無視する
        ということ。
        */

        // 真下の段
        if (blockY[i] != belowY)
        {
            continue;
        }
        /*
        ㉕ 真下の段だけ調べる
        if (blockY[i] != belowY)
        {
            continue;
        }

        これは、
        真下の段にいないブロックは無視する
        ということ。

        例えば上にあるブロックや、遠く離れたブロックを「支え」として扱わないため。
        */

        // 横に1マスでも重なっている
        if (IsOverlapping(
            blockX[number],
            blockLength[number],
            blockX[i],
            blockLength[i]))
        {
            return true;
        }
        /*
        ㉖ 横に重なっているか確認

        if (IsOverlapping(
        blockX[number],
        blockLength[number],
        blockX[i],
        blockLength[i]))
        {
            return true;
        }

        ここで先ほど作った
        IsOverlapping()
        を使っている。

        つまり、
        「真下の段にあるブロック」と「今のブロック」が横方向に重なっているか？
        を確認している。

        重なっていれば、
        return true;
        なので、
        支えがある → 落下しない
        と判断できる。

        ㉗ 最後の return false

        全部のブロックを調べても支えが見つからなかった場合、
        return false;
        になる。

        つまり、
        下に支えがない → 落下できる
        ということ。
        */
    }

    return false;
}

/*
このファイルで使っているものをまとめると
この Block.cpp では、今回話していたものの中からかなり多く使っている。

種類	    使っている場所	目的
if文	    AddBlock()など	条件を判定
for文	    HasSupport()	全ブロックを調べる
continue	HasSupport()	条件に合わないブロックを飛ばす
return	    各関数	        結果を返す・関数を終了
関数	    AddBlock()など	処理をまとめる
配列	    blockType[]など	複数のブロックを管理
変数	    blockCountなど	データを保存
定数	    MAX_BLOCKなど	変わらない値を管理
bool型	    blockExistなど	Yes / Noを管理
int型	    blockXなど	    整数を管理
float型	    blockDrawY	    小数を含む位置を管理
char型	    blockType	    S/Cなど1文字を管理

このBlock.cpp の一番大事な考え方

簡単に言うと、

① 配列でブロックの情報を保存する
↓
② 関数でブロックを追加・判定する
↓
③ if文で条件を判定する
↓
④ for文で全部のブロックを調べる
↓
⑤ boolで「ある・ない」「支えがある・ない」を返す

という構造になっている。

だから、このファイルは**「ブロックのデータを持っておいて、ブロックを追加したり、重なりや支えを判定したりする」**役割になっているよ。
*/
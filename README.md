# SW2 第1回課題

## やったこと

* 課題1〜3（ `mylife3.c` に全てまとめています）
* 発展課題（ `mylife4.c` と `lifegame/` 以下のコードが該当します）

## 発展課題

ライフゲームの各セルの場所に対して、栄養の量というファクターを追加してシミュレーションを行なった。
あるセルの栄養の多さに対応して、周辺の生存セルの許容量が増えるような条件づけをした。

実行コマンドは以下の通りである。

```
// build
$ make build4
もしくは
$ gcc -o mylife4 mylife4.c lifegame/stage.c lifegame/util.c

// execute
$ make
もしくは
$ ./mylife4
```

詳細を箇条書きにする。

* 各セルは栄養（ `nutrition`：非負の整数値, 初期値25 ）を持っている
* 栄養は時間が1ステップ経過するごとに `その周辺（そこを中心に3x3マス = 9マス）の生存セルの数` だけ減少する。
* あるセルが生存しているとする：
  * `その場所の栄養素の値` が `その周辺の生存セルの数` 以上であれば、そのセルは継続して生存できる。そうでなければ、そのセルは死ぬ。
  * セルが新たに死ぬ場合、周辺の栄養を15する。この加算は、1ステップごとの栄養の減少の後に行われるものとする。
* あるセルが死んでいるとする：
  * `その場所の栄養素の値` が `その周辺の生存セルの数+60` 以上であれば、セルが生まれる。そうでなければ死んだままとする。（60はセルが生まれるだけの栄養のキャパを表現する）
* 栄養の初期値、セルの死による栄養の増加量、セルが発生するためのキャパシティは `lifegame/config.h` の `define` マクロで `INITIAL_NUTRITION` と `NUTRITION_BY_DEATH` と `CELL_BORN_CAPACITY` として定義されている。
* 栄養の多さによって背景の緑色が明るくなるようにした。

シミュレーションの結果として、格子状に1つとばしでセルが生存している場合が安定しやすいことがわかった。（以下のような例である）

```
o o
   
o o
```

このような場合、栄養の競合が発生しづらく、よりサステイナブルな状況になると考える。
（ソーシャルディスタンスだなと思った）

以上です。

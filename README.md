# S2G
Siv3Dのサンプルとして公開されている「2D RPG のマップと移動の基本」(ゲーム典型 007A)を参考にしたダンジョン脱出ゲーム。

## 開発環境
PC：Windows 10 Pro、Intel Core i7-6700K  
IDE：Visual Studio 2022  
言語：C++  
フレームワーク：Siv3D 0.6.6
### テスト環境
開発環境と同じ

## 開発期間
α版  
令和5年(2023)/03/08 ~ /03/25

## 目的
ゲーム制作最初の一歩。  
C++のスキルを磨くため、できるだけ処理はC++で実装する。  
グラフィック処理はSiv3Dに頼る。  
複雑にせず、まず完成させる。

## 仕様
### ヒートマップ
敵がプレイヤーを発見し、追跡するプログラムにヒートマップを採用した。  
ステージと同じサイズのヒートマップを用意し、その温度を参照して敵の移動先を決定する。  
温度設定は下記の通りである。  
| オブジェクト | 温度 |  
----|----  
| 壁 | 0 |  
| 空間 | 1 |  
| プレイヤ | 5 |  
| スタート地点 | -5 |  
| ゴール地点 | -5 |  

プレイヤを熱源とし、移動ごとにヒートマップを更新していく。  
ヒートマップの更新は、プレイヤの位置からユークリッド距離L_pまでの縦横4マスを幅優先探索で更新していく。  
ヒートマップは更新の前に-1冷却される。  
敵は縦横4マスを走査して最も温度が高い方向へ移動する。  
周囲の温度が均一の場合はランダムで移動する。  

## ロードマップ
| アップ予定月 | バージョン |  
----|----  
| 3月 | α版 |  
| 5月 | β版 |  
| 7月 | γ版 |  
| 8月 | マスター版 |  

### α版
・操作は移動のみ  
・インタフェースはキーボードの十字キー  
・ステージ情報はcsvファイルから読み取る  
・敵の移動は、固定ルート型とヒートマップ型の2種類  
・敵と接触するとスタート地点に戻される
・ゲームオーバーは、なし  
・ゴール地点に到達したらクリア

### β版
・敵AIに視線Layを追加  
・プレイヤを見失うタイミングを綺麗にする  
・ステージ追加  
・鍵つき扉などギミック実装  
・移動手段追加
・チュートリアル実装

### γ版
・オリジナルキャラ絵で実装
・ストーリーイベント実装
・UI実装

### マスター版
・軽量化

## ライセンス
・Siv3D　[https://github.com/Siv3D/OpenSiv3D](https://github.com/Siv3D/OpenSiv3D/blob/main/LICENSE)
・音楽素材MusMus https://musmus.main.jp
・「2D RPG のマップと移動の基本」(ゲーム典型 007A)　https://github.com/Reputeless/games/blob/main/LICENSE

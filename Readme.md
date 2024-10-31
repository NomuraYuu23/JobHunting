# 機能

## クロスシミュレーション
![Screenshot of a comment on a GitHub issue showing an image, added in the Markdown, of an Octocat smiling and raising a tentacle.](picture/cloth1.png)
![Screenshot of a comment on a GitHub issue showing an image, added in the Markdown, of an Octocat smiling and raising a tentacle.](picture/cloth2.png)
### 概要
布の動きを再現する物理シミュレーションです。  
現在はGPGPUで計算する方式をとり、処理速度の向上も目指しています。  
動きを確認するデモは[developCloth](https://github.com/NomuraYuu23/JobHunting/tree/developCloth)で確認できます。

### 現在行えること
* 風力と重力を加えることができます。  
![Screenshot of a comment on a GitHub issue showing an image, added in the Markdown, of an Octocat smiling and raising a tentacle.](picture/wind_gravity.gif)

* 平面との衝突、押し出し処理ができます。  
![Screenshot of a comment on a GitHub issue showing an image, added in the Markdown, of an Octocat smiling and raising a tentacle.](picture/collision_plane.gif)

* 球との衝突、押し出し処理ができます。  
![Screenshot of a comment on a GitHub issue showing an image, added in the Markdown, of an Octocat smiling and raising a tentacle.](picture/collision_sphere.gif)

* カプセルとの衝突、押し出し処理ができます。  
![Screenshot of a comment on a GitHub issue showing an image, added in the Markdown, of an Octocat smiling and raising a tentacle.](picture/collision_capsule.gif)

* 分割数の変更ができます。
* 8 * 8
![Screenshot of a comment on a GitHub issue showing an image, added in the Markdown, of an Octocat smiling and raising a tentacle.](picture/div_8x8.gif)
* 16 * 16
![Screenshot of a comment on a GitHub issue showing an image, added in the Markdown, of an Octocat smiling and raising a tentacle.](picture/div_16x16.gif)
* 32 * 32
![Screenshot of a comment on a GitHub issue showing an image, added in the Markdown, of an Octocat smiling and raising a tentacle.](picture/div_32x32.gif)
* 64 * 64
![Screenshot of a comment on a GitHub issue showing an image, added in the Markdown, of an Octocat smiling and raising a tentacle.](picture/div_64x64.gif)

### コード
* [クロスシミュレーションコード](Project/Engine/Physics/ClothGPU/ClothGPU.cpp)  
布の計算、shaderの初期化などを行っているコードです。
* [デモコード](Project/Application/ClothDemo/ClothDemo.cpp)  
布を動きを見るためのコードです。
* [計算のためのCS](Project/Resources/shaders/ClothGPU)  
布を動きを計算するCSです。処理は複数のCSに分かれているので、リンクはまとめてあるフォルダに移動します。

# 外部ファイル
* assimp
* DirectXTex
* imgui
* nlohmarn

# 詳しい資料

[Notion](https://www.notion.so/Cloth-Simulation-119f9b3a9f56803abef5de984bb4eb9e)に詳しい処理の内容が書かれています。

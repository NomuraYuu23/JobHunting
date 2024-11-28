#include "GameScene.h"
#include "../../../Engine/base/WinApp.h"
#include "../../../Engine/base/Texture/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/base/Debug/D3DResourceLeakChecker.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/3D/Model/ModelDraw.h"
#include "../../../Engine/base/WindowSprite/WindowSprite.h"
#include "../../../Engine/Physics/ClothGPU/ClothGPU.h"

GameScene::~GameScene()
{
}

/// <summary>
/// 初期化
/// </summary>
void GameScene::Initialize() {

	IScene::Initialize();

	ModelCreate();
	TextureLoad();

	IScene::InitilaizeCheck();

}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update() {

}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw() {

}

void GameScene::ImguiDraw(){

}

void GameScene::ModelCreate()
{

}

void GameScene::TextureLoad()
{

}

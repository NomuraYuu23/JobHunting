#include "TutorialScene.h"
#include "../../../Engine/base/WinApp.h"
#include "../../../Engine/base/Texture/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/base/Debug/D3DResourceLeakChecker.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../../../Engine/3D/Model/ModelDraw.h"

#include "../../../Engine/Physics/ClothGPU/ClothGPU.h"

TutorialScene::~TutorialScene()
{
}

/// <summary>
/// 初期化
/// </summary>
void TutorialScene::Initialize() {

	IScene::Initialize();

	ModelCreate();
	TextureLoad();

	IScene::InitilaizeCheck();

}

/// <summary>
/// 更新処理
/// </summary>
void TutorialScene::Update() {

}

/// <summary>
/// 描画処理
/// </summary>
void TutorialScene::Draw() {

}

void TutorialScene::ImguiDraw() {

}

void TutorialScene::ModelCreate()
{

}

void TutorialScene::TextureLoad()
{

}

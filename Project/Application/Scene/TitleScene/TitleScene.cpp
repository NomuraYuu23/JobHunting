#include "TitleScene.h"
#include "../../../Engine/base/Texture/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/3D/Model/ModelDraw.h"

TitleScene::~TitleScene()
{

}

void TitleScene::Initialize()
{

	IScene::Initialize();

	ModelCreate();
	TextureLoad();

	IScene::InitilaizeCheck();

}

void TitleScene::Update()
{

}

void TitleScene::Draw()
{

}

void TitleScene::ImguiDraw()
{

}

void TitleScene::ModelCreate()
{

}

void TitleScene::TextureLoad()
{

}

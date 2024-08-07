#include "ObjectFactory.h"
#include "../../Engine/Object/MeshObject.h"

#include "../Object/Character/Player/Player.h"
#include "../Object/Obstacle/Ground/Ground.h"
#include "../Object/Obstacle/Block/Block.h"
#include "../Object/Character/Enemy/Ghost/Ghost.h"
#include "../Object/Character/Enemy/AxSpearMan/AxSpearMan.h"
#include "../Object/Character/Enemy/AxSpearMan/AxSpearManWeapon.h"
#include "../Object/Character/Player//PlayerWeapon/PlayerWeapon.h"
#include "../Object/Bonfire/Bonfire.h"
#include "../Object/Character/Player/TitlePlayer/TitlePlayer.h"

// 親取得用
Player* ObjectFactory::player_ = nullptr;

// オブジェクト作成でそれぞれのタイプを作成するための関数群
// 返り値 無し
// 引数1 オブジェクト
std::array<
	std::pair<std::string, std::function<IObject*(LevelData::ObjectData&)>>,
	ObjectFactory::CreateObjectIndex::kCreateObjectIndexOfCount> ObjectFactory::createObjectFunctions_ = {};

// マネージャー
BaseObjectManager* ObjectFactory::objectManager_ = nullptr;

ObjectFactory* ObjectFactory::GetInstance()
{
	static ObjectFactory instance;
	return &instance;
}

void ObjectFactory::Initialize(BaseObjectManager* objectManager)
{

	objectManager_ = objectManager;

	// 関数を入れていく
	createObjectFunctions_[kCreateObjectIndexPlayer].first = "Player";
	createObjectFunctions_[kCreateObjectIndexPlayer].second = ObjectFactory::CreateObjectPlayer;

	createObjectFunctions_[kCreateObjectIndexGround].first = "Ground";
	createObjectFunctions_[kCreateObjectIndexGround].second = ObjectFactory::CreateObjectGround;

	createObjectFunctions_[kCreateObjectIndexBlock].first = "Block";
	createObjectFunctions_[kCreateObjectIndexBlock].second = ObjectFactory::CreateObjectBlock;

	createObjectFunctions_[kCreateObjectIndexGhost].first = "Ghost";
	createObjectFunctions_[kCreateObjectIndexGhost].second = ObjectFactory::CreateObjectGhost;

	createObjectFunctions_[kCreateObjectIndexAxSpearMan].first = "AxSpearMan";
	createObjectFunctions_[kCreateObjectIndexAxSpearMan].second = ObjectFactory::CreateObjectAxSpearMan;

	createObjectFunctions_[kCreateObjectIndexAxSpearManWeapon].first = "AxSpearManWeapon";
	createObjectFunctions_[kCreateObjectIndexAxSpearManWeapon].second = ObjectFactory::CreateObjectAxSpearManWeapon;

	createObjectFunctions_[kCreateObjectIndexPlayerWeapon].first = "PlayerWeapon";
	createObjectFunctions_[kCreateObjectIndexPlayerWeapon].second = ObjectFactory::CreateObjectPlayerWeapon;

	createObjectFunctions_[kCreateObjectIndexBonfire].first = "Bonfire";
	createObjectFunctions_[kCreateObjectIndexBonfire].second = ObjectFactory::CreateObjectBonfire;

	createObjectFunctions_[kCreateObjectIndexTitlePlayer].first = "TitlePlayer";
	createObjectFunctions_[kCreateObjectIndexTitlePlayer].second = ObjectFactory::CreateObjectTitlePlayer;

}

IObject* ObjectFactory::CreateObject(LevelData::ObjectData& objectData)
{

	IObject* object = nullptr;

	// 確認のためメッシュオブジェクトのみ
	// クラスの名前など取得してオブジェクトを作る
	if (std::holds_alternative<LevelData::MeshData>(objectData)) {

		LevelData::MeshData data = std::get<LevelData::MeshData>(objectData);

		for (uint32_t i = 0; i < kCreateObjectIndexOfCount; ++i) {
			if (data.className == createObjectFunctions_[i].first) {
				object = createObjectFunctions_[i].second(objectData);
			}

		}

	}

    return object;

}

IObject* ObjectFactory::CreateObjectPlayer(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new Player();
	// 初期化
	static_cast<Player*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	player_ = static_cast<Player*>(object);

	return object;

}

IObject* ObjectFactory::CreateObjectGround(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new Ground();
	// 初期化
	static_cast<Ground*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

IObject* ObjectFactory::CreateObjectBlock(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new Block();
	// 初期化
	static_cast<Block*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

IObject* ObjectFactory::CreateObjectGhost(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new Ghost();
	// 初期化
	static_cast<Ghost*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	static_cast<Ghost*>(object)->SetPlayer(player_);
	return object;
}

IObject* ObjectFactory::CreateObjectAxSpearMan(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new AxSpearMan();
	// 初期化
	static_cast<AxSpearMan*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	static_cast<AxSpearMan*>(object)->SetPlayer(player_);
	return object;
}

IObject* ObjectFactory::CreateObjectAxSpearManWeapon(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new AxSpearManWeapon();

	// 初期化
	static_cast<AxSpearManWeapon*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	// 親セット
	static_cast<AxSpearManWeapon*>(object)->SetParent(
		static_cast<AxSpearMan*>(objectManager_->GetObjectPointer(
			static_cast<AxSpearManWeapon*>(object)->GetParentName())));
	return object;
}

IObject* ObjectFactory::CreateObjectPlayerWeapon(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new PlayerWeapon();

	// 初期化
	static_cast<PlayerWeapon*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	// 親セット
	static_cast<PlayerWeapon*>(object)->SetParent(
		static_cast<Player*>(objectManager_->GetObjectPointer(
			static_cast<PlayerWeapon*>(object)->GetParentName())));
	return object;
}

IObject* ObjectFactory::CreateObjectBonfire(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new Bonfire();
	// 初期化
	static_cast<Bonfire*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

IObject* ObjectFactory::CreateObjectTitlePlayer(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new TitlePlayer();
	// 初期化
	static_cast<TitlePlayer*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

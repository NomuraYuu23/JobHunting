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
#include "Obstacle/Pillar/PillarFoundation.h"
#include "../Object/RigidBodyObject/Piller/Pillar.h"
#include "../Object/Obstacle/BossFog/BossFog.h"
#include "../Object/StringObject/PlayerMuffler/PlayerMuffler.h"
#include "../Object/ClothObject/Flag/Flag.h"
#include "../Object/Obstacle/FlagPole/FlagPole.h"

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

BossSystem* ObjectFactory::bossSystem_ = nullptr;

ObjectFactory* ObjectFactory::GetInstance()
{
	static ObjectFactory instance;
	return &instance;
}

void ObjectFactory::Initialize(BaseObjectManager* objectManager, BossSystem* bossSystem)
{

	objectManager_ = objectManager;
	bossSystem_ = bossSystem;

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
	
	createObjectFunctions_[kCreateObjectIndexPillarFoundation].first = "PillarFoundation";
	createObjectFunctions_[kCreateObjectIndexPillarFoundation].second = ObjectFactory::CreateObjectPillarFoundation;

	createObjectFunctions_[kCreateObjectIndexPillar].first = "Pillar";
	createObjectFunctions_[kCreateObjectIndexPillar].second = ObjectFactory::CreateObjectPillar;

	createObjectFunctions_[kCreateObjectIndexBossFog].first = "BossFog";
	createObjectFunctions_[kCreateObjectIndexBossFog].second = ObjectFactory::CreateObjectBossFog;

	createObjectFunctions_[kCreateObjectIndexPlayerMuffler].first = "PlayerMuffler";
	createObjectFunctions_[kCreateObjectIndexPlayerMuffler].second = ObjectFactory::CreateObjectPlayerMuffler;

	createObjectFunctions_[kCreateObjectIndexFlag].first = "Flag";
	createObjectFunctions_[kCreateObjectIndexFlag].second = ObjectFactory::CreateObjectFlag;

	createObjectFunctions_[kCreateObjectIndexFlagPole].first = "FlagPole";
	createObjectFunctions_[kCreateObjectIndexFlagPole].second = ObjectFactory::CreateObjectFlagPole;

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

IObject* ObjectFactory::CreateObjectPillarFoundation(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new PillarFoundation();
	// 初期化
	static_cast<PillarFoundation*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

IObject* ObjectFactory::CreateObjectPillar(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new Pillar();

	// 初期化
	static_cast<Pillar*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	// 親セット
	static_cast<Pillar*>(object)->SetParent(
		static_cast<PillarFoundation*>(objectManager_->GetObjectPointer(
			static_cast<Pillar*>(object)->GetParentName())));
	return object;
}

IObject* ObjectFactory::CreateObjectBossFog(LevelData::ObjectData& objectData)
{
	
	// インスタンス生成
	IObject* object = new BossFog();
	// 初期化
	static_cast<BossFog*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	static_cast<BossFog*>(object)->SetBossSystem(bossSystem_);
	return object;

}

IObject* ObjectFactory::CreateObjectPlayerMuffler(LevelData::ObjectData& objectData)
{

	// インスタンス生成
	IObject* object = new PlayerMuffler();

	// 初期化
	static_cast<PlayerMuffler*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	// 親セット
	static_cast<PlayerMuffler*>(object)->SetParent(
		static_cast<Player*>(objectManager_->GetObjectPointer(
			static_cast<PlayerMuffler*>(object)->GetParentName())));
	return object;

}

IObject* ObjectFactory::CreateObjectFlag(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new Flag();
	// 初期化
	static_cast<Flag*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

IObject* ObjectFactory::CreateObjectFlagPole(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new FlagPole();
	// 初期化
	static_cast<FlagPole*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

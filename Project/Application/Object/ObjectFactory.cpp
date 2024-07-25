#include "ObjectFactory.h"
#include "../../Engine/Object/MeshObject.h"

#include "../Player/Player.h"
#include "../Ground/Ground.h"
#include "../Block/Block.h"
#include "../Enemy/Ghost/Ghost.h"
#include "../Enemy/AxSpearMan/AxSpearMan.h"
#include "../Enemy/AxSpearMan/AxSpearManWeapon.h"
#include "../Player/PlayerWeapon/PlayerWeapon.h"
#include "../Bonfire/Bonfire.h"
#include "../Player/TitlePlayer/TitlePlayer.h"

ObjectFactory* ObjectFactory::GetInstance()
{
	static ObjectFactory instance;
	return &instance;
}

IObject* ObjectFactory::CreateObject(LevelData::ObjectData& objectData)
{

	IObject* object = nullptr;

	// 確認のためメッシュオブジェクトのみ
	// クラスの名前など取得してオブジェクトを作る
	if (std::holds_alternative<LevelData::MeshData>(objectData)) {

		LevelData::MeshData data = std::get<LevelData::MeshData>(objectData);

		if (data.className == "Player") {
			// インスタンス生成
			object = new Player();

			// 初期化
			static_cast<Player*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));

			player_ = static_cast<Player*>(object);

		}
		else if (data.className == "Ground") {
			// インスタンス生成
			object = new Ground();

			// 初期化
			static_cast<Ground*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
		}
		else if (data.className == "Block") {
			// インスタンス生成
			object = new Block();

			// 初期化
			static_cast<Block*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
		}
		else if (data.className == "Ghost") {
			// インスタンス生成
			object = new Ghost();

			// 初期化
			static_cast<Ghost*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
			static_cast<Ghost*>(object)->SetPlayer(player_);
		}
		else if (data.className == "AxSpearMan") {
			// インスタンス生成
			object = new AxSpearMan();

			// 初期化
			static_cast<AxSpearMan*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
			static_cast<AxSpearMan*>(object)->SetPlayer(player_);
		}
		else if (data.className == "AxSpearManWeapon") {
			// インスタンス生成
			object = new AxSpearManWeapon();

			// 初期化
			static_cast<AxSpearManWeapon*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
			// 親セット
			//static_cast<AxSpearManWeapon*>(object)->SetParent(
			//	static_cast<AxSpearMan*>(objectManager_->GetObjectPointer(
			//		static_cast<AxSpearManWeapon*>(object)->GetParentName())));
		}
		else if (data.className == "PlayerWeapon") {
			// インスタンス生成
			object = new PlayerWeapon();

			// 初期化
			static_cast<PlayerWeapon*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
			// 親セット
			static_cast<PlayerWeapon*>(object)->SetParent(
				static_cast<Player*>(objectManager_->GetObjectPointer(
					static_cast<PlayerWeapon*>(object)->GetParentName())));
		}
		else if (data.className == "Bonfire") {

			// インスタンス生成
			object = new Bonfire();

			// 初期化
			static_cast<Bonfire*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
		}
		else if (data.className == "TitlePlayer") {

			// インスタンス生成
			object = new TitlePlayer();

			// 初期化
			static_cast<TitlePlayer*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
		}

	}

    return object;

}


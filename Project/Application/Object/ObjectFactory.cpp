#include "ObjectFactory.h"
#include "../../Engine/Object/MeshObject.h"

#include "../Player/Player.h"
#include "../Ground/Ground.h"


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
		}
		else if (data.className == "Ground") {
			// インスタンス生成
			object = new Ground();

			// 初期化
			static_cast<Ground*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
		}

		//// インスタンス生成
		//object = new MeshObject();

		//// 初期化
		//static_cast<MeshObject*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));

	}

    return object;

}


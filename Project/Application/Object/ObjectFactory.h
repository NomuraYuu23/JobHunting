#pragma once
#include "../../Engine/Object/AbstractObjectFactory.h"
#include "../../Engine/Level/LevelData.h"
#include "GameSceneObjectManager.h"

class ObjectFactory :
    public AbstractObjectFactory
{

public: // 静的メンバ関数

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static ObjectFactory* GetInstance();

public: //メンバ関数
	
	/// <summary>
	/// オブジェクト作成
	/// </summary>
	/// <param name="objectData">オブジェクトデータ/param>
	/// <returns>オブジェクト</returns>
	IObject* CreateObject(LevelData::ObjectData& objectData) override;

	/// <summary>
	/// オブジェクトマネージャー設定
	/// </summary>
	/// <param name="objectManager"></param>
	void SetObjectManager(BaseObjectManager* objectManager) { objectManager_ = objectManager; }

private:

	Player* player_;

	BaseObjectManager* objectManager_;

private:
	ObjectFactory() = default;
	~ObjectFactory() = default;
	ObjectFactory(const ObjectFactory&) = delete;
	const ObjectFactory& operator=(const ObjectFactory&) = delete;

};


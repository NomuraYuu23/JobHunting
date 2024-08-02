#pragma once
#include "../../Engine/Object/AbstractObjectFactory.h"
#include "../../Engine/Level/LevelData.h"
#include "GameSceneObjectManager.h"

class ObjectFactory :
    public AbstractObjectFactory
{

public: // サブクラス,定数

	enum CreateObjectIndex {
		kCreateObjectIndexPlayer, // プレイヤー
		kCreateObjectIndexGround, // 地面
		kCreateObjectIndexBlock, // 壁
		kCreateObjectIndexGhost, // ゴースト
		kCreateObjectIndexAxSpearMan, // 斧槍男
		kCreateObjectIndexAxSpearManWeapon, // 斧槍男の武器
		kCreateObjectIndexPlayerWeapon, // プレイヤーの武器
		kCreateObjectIndexBonfire, // かがり火
		kCreateObjectIndexTitlePlayer, // タイトル用プレイヤー
		kCreateObjectIndexOfCount, // 数数える用

	};

public: // 静的メンバ関数

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static ObjectFactory* GetInstance();

public: //メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="objectManager">オブジェクトマネージャー</param>
	void Initialize(BaseObjectManager* objectManager);

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

private: // メンバ関数

	/// <summary>
	/// プレイヤー
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <returns>オブジェクト</returns>
	static IObject* CreateObjectPlayer(LevelData::ObjectData& objectData);

	/// <summary>
	/// 地面
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <returns>オブジェクト</returns>
	static IObject* CreateObjectGround(LevelData::ObjectData& objectData);

	/// <summary>
	/// 壁
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <returns>オブジェクト</returns>
	static IObject* CreateObjectBlock(LevelData::ObjectData& objectData);

	/// <summary>
	/// ゴースト
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <returns>オブジェクト</returns>
	static IObject* CreateObjectGhost(LevelData::ObjectData& objectData);
	
	/// <summary>
	/// 斧槍男
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <returns>オブジェクト</returns>
	static IObject* CreateObjectAxSpearMan(LevelData::ObjectData& objectData);

	/// <summary>
	/// 斧槍男の武器
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <returns>オブジェクト</returns>
	static IObject* CreateObjectAxSpearManWeapon(LevelData::ObjectData& objectData);
	
	/// <summary>
	/// プレイヤーの武器
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <returns>オブジェクト</returns>
	static IObject* CreateObjectPlayerWeapon(LevelData::ObjectData& objectData);

	/// <summary>
	/// かがり火
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <returns>オブジェクト</returns>
	static IObject* CreateObjectBonfire(LevelData::ObjectData& objectData);

	/// <summary>
	/// タイトル用プレイヤー
	/// </summary>
	/// <param name="objectData">オブジェクトデータ</param>
	/// <returns>オブジェクト</returns>
	static IObject* CreateObjectTitlePlayer(LevelData::ObjectData& objectData);

private:

	// 親取得用
	static Player* player_;

	// オブジェクト作成でそれぞれのタイプを作成するための関数群
	// 返り値 無し
	// 引数1 オブジェクト
	static std::array<
		std::pair<std::string,std::function<IObject* (LevelData::ObjectData&)>>,
		CreateObjectIndex::kCreateObjectIndexOfCount> createObjectFunctions_;

	// マネージャー
	static BaseObjectManager* objectManager_;

private:
	ObjectFactory() = default;
	~ObjectFactory() = default;
	ObjectFactory(const ObjectFactory&) = delete;
	const ObjectFactory& operator=(const ObjectFactory&) = delete;

};


#pragma once
#include <variant>
#include "../../Engine/Collision/CollisionData.h"

class Null;
class Player;
class PlayerAttack;
class Ground;
class Block;
class BaseEnemy;
class BaseEnemyAttack;
class BaseWeapon;

using ColliderParentObject = std::variant<Null*, Player*, PlayerAttack*, Ground*, Block*, BaseEnemy*, BaseEnemyAttack*, BaseWeapon*>;

// 親がないもの用
class Null {

public:

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData) {}

};

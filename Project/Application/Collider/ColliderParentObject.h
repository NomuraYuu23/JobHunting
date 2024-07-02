#pragma once
#include <variant>
#include "../../Engine/Collision/CollisionData.h"

class Null;
class Player;
class Enemy;
class PlayerAttack;
class EnemyAttack;
class TutorialEnemy;
class Ground;
class Block;
class BaseEnemy;
class BaseEnemyAttack;

using ColliderParentObject = std::variant<Null*, Player*, Enemy*, PlayerAttack*, EnemyAttack*, TutorialEnemy*, Ground*, Block*, BaseEnemy*, BaseEnemyAttack*>;

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

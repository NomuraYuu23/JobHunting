#pragma once
#include <variant>

class Player;
class Enemy;
class PlayerAttack;
class EnemyAttack;

using ColliderParentObject = std::variant<Player*, Enemy*, PlayerAttack*, EnemyAttack*>;

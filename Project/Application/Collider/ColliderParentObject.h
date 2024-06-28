#pragma once
#include <variant>

class Null;
class Player;
class Enemy;
class PlayerAttack;
class EnemyAttack;
class TutorialEnemy;

using ColliderParentObject = std::variant<Null*, Player*, Enemy*, PlayerAttack*, EnemyAttack*, TutorialEnemy*>;

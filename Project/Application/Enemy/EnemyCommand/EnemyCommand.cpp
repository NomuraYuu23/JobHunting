#include "EnemyCommand.h"

EnemyCommand* EnemyCommand::GetInstance()
{
	static EnemyCommand instance;
	return &instance;
}

void EnemyCommand::Initialize()
{
}

uint32_t EnemyCommand::Command()
{
	return 2;
}

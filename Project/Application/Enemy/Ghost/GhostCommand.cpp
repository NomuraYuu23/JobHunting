#include "GhostCommand.h"
#include "IGhostState.h"

void GhostCommand::Initialize()
{
}

uint32_t GhostCommand::Command()
{

	uint32_t resultState = GhostState::kGhostStateChase;

	return resultState;

}

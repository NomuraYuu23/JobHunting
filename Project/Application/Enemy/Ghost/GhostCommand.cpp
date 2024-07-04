#include "GhostCommand.h"
#include "IGhostState.h"

#include "Ghost.h"
#include "../../Player/Player.h"

void GhostCommand::Initialize(Ghost* ghost)
{

	ghost_ = ghost;

}

uint32_t GhostCommand::Command()
{

	float distance = Vector3::Length(ghost_->GetPlayer()->GetWorldTransformAdress()->GetWorldPosition() - ghost_->GetWorldTransformAdress()->GetWorldPosition());

	float distanceChase = 15.0f;

	float distanceAttack = 5.0f;


	uint32_t resultState = GhostState::kGhostStateRoot;

	if (distance < distanceAttack) {
		resultState = GhostState::kGhostStateAttack;
	}
	else if (distance < distanceChase) {
		resultState = GhostState::kGhostStateChase;
	}

	return resultState;

}

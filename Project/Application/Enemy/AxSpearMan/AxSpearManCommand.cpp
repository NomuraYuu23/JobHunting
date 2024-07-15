#include "AxSpearManCommand.h"
#include "IAxSpearManState.h"

#include "AxSpearMan.h"

void AxSpearManCommand::Initialize(AxSpearMan* axSpearMan)
{

    axSpearMan_ = axSpearMan;

}

uint32_t AxSpearManCommand::Command()
{

    uint32_t resultState = AxSpearManState::kAxSpearManStateShakeOff;

    return resultState;
}

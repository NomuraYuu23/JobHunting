#include "AxSpearManCommand.h"
#include "IAxSpearManState.h"

#include "AxSpearMan.h"
#include <random>

void AxSpearManCommand::Initialize(AxSpearMan* axSpearMan)
{

    axSpearMan_ = axSpearMan;

}

uint32_t AxSpearManCommand::Command()
{

    uint32_t resultState = AxSpearManState::kAxSpearManStateRoot;

    std::random_device seedGenerator;
    std::mt19937 randomEngine(seedGenerator());

    std::uniform_real_distribution<float> dist(0.0f, 3.0f);
    float value = dist(randomEngine);

    if (value <= 1.0f) {
        resultState = AxSpearManState::kAxSpearManStateWaitSee;
    }
    else if (value <= 2.0f) {
        resultState = AxSpearManState::kAxSpearManStateDashSwingDown;
    }
    else if (value <= 2.5f) {
        resultState = AxSpearManState::kAxSpearManState3Consecutive;
    }
    else {
        resultState = AxSpearManState::kAxSpearManStateShakeOff;
    }

    return resultState;
}

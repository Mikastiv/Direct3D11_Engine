#pragma once

#include "TestObject.hpp"

class SkinnedBox : public TestObject<SkinnedBox>
{
public:
    SkinnedBox(
        Graphics& gfx,
        std::mt19937& rng,
        std::uniform_real_distribution<float>& radiusDist,
        std::uniform_real_distribution<float>& yRotOffsetDist,
        std::uniform_real_distribution<float>& rotOffsetDist,
        std::uniform_real_distribution<float>& dYRotationDist,
        std::uniform_real_distribution<float>& dPitchDist,
        std::uniform_real_distribution<float>& dYawDist,
        std::uniform_real_distribution<float>& dRollDist);
};
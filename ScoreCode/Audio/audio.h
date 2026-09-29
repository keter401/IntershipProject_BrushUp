#pragma once
#include <vector>
#include "Framework\Manager\manager.h"

class DWScene;

class DWAudio : public DWManager
{
public:
    enum ESoundType
    {
        PlayerShoot = 0,
        PlayerDamaged = 1,
        PlayerJump = 2,
		EnemyDead = 3,
    };

private:
    static constexpr int kInvalidHandle = -1;
    static constexpr int kSoundCount = 4;

    std::vector<int> Handles;
    bool bReuseableObject = true;

    int IndexOf(ESoundType type) const;

	int SoundVolume = 65; // 0 ~ 255

public:
    DWAudio(DWScene* scene);
    ~DWAudio();

    void Init() override {};
    void Uninit() override {};
    void Update() override {};

    bool IsReuseableObject() const { return bReuseableObject; }

    void PlayAudio(ESoundType type);

	void SetSoundVolume(int volume) { SoundVolume = volume; }

    void StopSound();

    int GetHandle(ESoundType type) const;
};

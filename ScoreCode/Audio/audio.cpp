#include "audio.h"
#include <utility> 
#include <string>

DWAudio::DWAudio(DWScene* scene)
{
    Handles.assign(kSoundCount, kInvalidHandle);

    const std::pair<ESoundType, const char*> soundTable[] = {
        { ESoundType::PlayerJump,    "Sound/jump.mp3"    },
        { ESoundType::PlayerDamaged, "Sound/damaged.mp3" },
        { ESoundType::PlayerShoot,   "Sound/shoot.mp3"   },
        { ESoundType::EnemyDead,   "Sound/enemyDead.ogg" },
    };

    for (const auto& def : soundTable)
    {
        const int index = IndexOf(def.first);
        if (0 <= index && index < kSoundCount)
        {
            const int h = LoadSoundMem(def.second);
            Handles[index] = h;
        }
    }

	CurrentScene = scene;
}

DWAudio::~DWAudio()
{
    for (const int h : Handles)
    {
        if (h != kInvalidHandle)
        {
            DeleteSoundMem(h);
        }
    }
}

int DWAudio::IndexOf(ESoundType type) const
{
    switch (type)
    {
    case ESoundType::PlayerShoot:   return 0;
    case ESoundType::PlayerDamaged: return 1;
    case ESoundType::PlayerJump:    return 2;
    case ESoundType::EnemyDead:     return 3;
    default:                        return -1;
    }
}

int DWAudio::GetHandle(ESoundType type) const
{
    const int idx = IndexOf(type);
    if (idx < 0 || idx >= static_cast<int>(Handles.size())) return kInvalidHandle;
    return Handles[idx];
}

void DWAudio::PlayAudio(ESoundType type)
{
    const int h = GetHandle(type);
    if (h == kInvalidHandle) return;

    ChangeVolumeSoundMem(SoundVolume, h);

    PlaySoundMem(h, DX_PLAYTYPE_BACK);
}

void DWAudio::StopSound()
{
    for (const int h : Handles)
    {
        if (h != kInvalidHandle)
        {
            StopSoundMem(h);
        }
    }
}

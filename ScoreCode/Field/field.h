#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "Framework\Manager\manager.h"
#include "Scene\Scenes\scene.h"

class DWField : public DWManager
{
private:
    static constexpr int StageWidth = 15;
    static constexpr int StageHeight = 20;
    static constexpr int TileSize = 50;
    static constexpr int OriginX = 200;
    static constexpr int OriginY = 60;
    static constexpr int RandomStagePick = 5;
    static constexpr int GoalWaitFrames = 60;

    bool bGoal = false;
    int GoalReachCounter = 0;

    using Grid = std::vector<std::vector<int>>;
    std::unordered_map<std::string, Grid> StageGrids;

    std::vector<std::string> Sequence;

    void LoadStageFiles(const std::string& dir);
    std::vector<int> ParseRow(const std::string& s);
    Grid LoadOneStageGrid(const std::string& filepath);
    Grid FitToSize(const Grid& src, int w, int h);

    void BuildSequence();

    void SpawnAllStages();
    void SpawnStageObjects(const std::string& stageName, int stageIndex);

    void ClearFieldLayer();

    bool IsPlayerPastAllStagesBottom() const;

    DWVector2 TileToWorld(int gx, int gy, int stageIndex) const;

    int TotalStages() const { return static_cast<int>(Sequence.size()); }

public:
    DWField(DWScene* scene) : DWManager(scene) {}

    void Init() override;
    void Uninit() override;
    void Update() override;
};
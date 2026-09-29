#include "field.h"
#include <fstream>
#include <sstream>
#include <random>
#include <algorithm>
#include <iomanip>

#include "Block\block.h"
#include "Enemy\FloatingEnemy\floatingEnemy.h"
#include "Enemy\CrawlingEnemy\crawlingEnemy.h"

#include "Framework\Manager\ColliderManager\colliderManager.h"
#include "Scene\Scenes\ResultScene\resultScene.h"
#include "Scene\sceneManager.h"
#include "Scene\Scenes\GameScene\gameScene.h"
#include "UI\UI.h"

void DWField::Init()
{
    LoadStageFiles("Data/");
    BuildSequence();

    ClearFieldLayer();

    SpawnAllStages();

    EnsurePlayerIfAbsent();

    bGoal = false;
    GoalReachCounter = 0;
}

void DWField::Uninit()
{
    ClearFieldLayer();
}

void DWField::Update()
{
    if (!IsPlayerPastAllStagesBottom())
    {
        bGoal = false;
        GoalReachCounter = 0;
        return;
    }

    // 到達後の再生成は1回だけ。プレイヤーがスタート地点へ戻れば上の分岐でリセットされる
    if (bGoal) return;

    GoalReachCounter++;
    if (GoalReachCounter < GoalWaitFrames) return;

    bGoal = true;

    DWUI* ui = CurrentScene->GetGameObject<DWUI>();
    if (ui != nullptr)
    {
        ui->AddStageNumber();
    }

    BuildSequence();
    ClearFieldLayer();
    SpawnAllStages();
    EnsurePlayerIfAbsent();
}

void DWField::LoadStageFiles(const std::string& dir)
{
    StageGrids.clear();

    // StartStage
    {
        const std::string path = dir + "StartStage.csv";
        Grid grid = FitToSize(LoadOneStageGrid(path), StageWidth, StageHeight);
        if (!grid.empty()) StageGrids["StartStage"] = std::move(grid);
    }

    // Stage01 ～ Stage10
    for (int i = 1; i <= 10; ++i)
    {
        std::ostringstream oss;
        oss << dir << "Stage" << std::setfill('0') << std::setw(2) << i << ".csv";
        Grid grid = FitToSize(LoadOneStageGrid(oss.str()), StageWidth, StageHeight);
        if (!grid.empty())
        {
            StageGrids.emplace("Stage" + std::to_string(i), std::move(grid));
        }
    }

    // GoalStage
    {
        const std::string path = dir + "GoalStage.csv";
        Grid grid = FitToSize(LoadOneStageGrid(path), StageWidth, StageHeight);
        if (!grid.empty()) StageGrids["GoalStage"] = std::move(grid);
    }

    if (StageGrids.empty())
    {
        StageGrids["StartStage"] = Grid(StageHeight, std::vector<int>(StageWidth, 0));
        StageGrids["GoalStage"] = Grid(StageHeight, std::vector<int>(StageWidth, 0));
    }
}

DWField::Grid DWField::LoadOneStageGrid(const std::string& filepath)
{
    Grid grid;
    std::ifstream ifs(filepath);
    if (!ifs.is_open())
    {
        return grid;
    }

    std::string line;
    while (std::getline(ifs, line))
    {
        while (!line.empty() && (line.back() == '\r' || line.back() == '\n' || line.back() == ' '))
            line.pop_back();
        if (line.empty()) continue;

        std::vector<int> row = ParseRow(line);
        grid.push_back(std::move(row));
    }
    return grid;
}

std::vector<int> DWField::ParseRow(const std::string& s)
{
    std::vector<int> row;
    std::string body = s;
    if (!body.empty() && body.front() == '{') body.erase(body.begin());
    if (!body.empty() && body.back() == '}') body.pop_back();

    std::stringstream ss(body);
    std::string tok;
    while (std::getline(ss, tok, ','))
    {
        // 空セルや数値でないトークンは 0 (空) として扱い、例外で落とさない
        int value = 0;
        try
        {
            value = std::stoi(tok);
        }
        catch (const std::exception&)
        {
            value = 0;
        }
        row.push_back(value);
    }
    return row;
}

DWField::Grid DWField::FitToSize(const DWField::Grid& src, int w, int h)
{
    Grid g;
    g.reserve(h);
    for (int y = 0; y < h; ++y)
    {
        std::vector<int> row;
        if (y < static_cast<int>(src.size())) row = src[y];
        if (static_cast<int>(row.size()) > w) row.resize(w);
        else if (static_cast<int>(row.size()) < w) row.resize(w, 0);
        g.push_back(std::move(row));
    }
    if (static_cast<int>(g.size()) > h) g.resize(h);
    return g;
}

void DWField::BuildSequence()
{
    Sequence.clear();

    if (StageGrids.count("StartStage") != 0) Sequence.push_back("StartStage");

    std::vector<std::string> pool;
    for (int i = 1; i <= 10; ++i)
    {
        std::string name = "Stage" + std::to_string(i);
        if (StageGrids.count(name) != 0) pool.push_back(name);
    }

    std::random_device rd;
    std::mt19937 rng(rd());
    std::shuffle(pool.begin(), pool.end(), rng);

    const int pick = std::min(RandomStagePick, static_cast<int>(pool.size()));
    for (int i = 0; i < pick; ++i) Sequence.push_back(pool[i]);

    if (StageGrids.count("GoalStage") != 0) Sequence.push_back("GoalStage");
}

void DWField::SpawnAllStages()
{
    for (int i = 0; i < TotalStages(); ++i)
    {
        SpawnStageObjects(Sequence[i], i);
    }
}

void DWField::SpawnStageObjects(const std::string& stageName, int stageIndex)
{
    auto it = StageGrids.find(stageName);
    if (it == StageGrids.end()) return;
    const Grid& grid = it->second;

    for (int gy = 0; gy < StageHeight; ++gy)
    {
        for (int gx = 0; gx < StageWidth; ++gx)
        {
            const int value = grid[gy][gx];
            if (value == 0) continue;

            const DWVector2 pos = TileToWorld(gx, gy, stageIndex);

            switch (value)
            {
            case 1:
            {
                DWBlock* block = CurrentScene->AddGameObject<DWBlock>(DWScene::FIELD, CurrentScene, pos);
                if (block != nullptr) block->SetBreakable(false);
                break;
            }
            case 2:
            {
                DWBlock* block = CurrentScene->AddGameObject<DWBlock>(DWScene::FIELD, CurrentScene, pos);
                if (block != nullptr) block->SetBreakable(true);
                break;
            }
            case 3:
            {
                // return するとステージ残りの生成ごと止まるので break
                CurrentScene->AddGameObject<DWFloatingEnemy>(DWScene::FIELD, CurrentScene, pos);
                break;
            }
            case 4:
            {
                CurrentScene->AddGameObject<DWCrawlingEnemy>(DWScene::FIELD, CurrentScene, pos);
                break;
            }
            case 5:
            {
                // プレイヤースポーン
                DWGameObject* player = CurrentScene->GetGameObjectByTag(DWGameObject::ETag::PLAYER);
                if (player != nullptr)
                {
                    player->SetPosition(pos);
                }
                break;
            }
            default:
                break;
            }
        }
    }
}

void DWField::EnsurePlayerIfAbsent()
{
    DWGameObject* player = CurrentScene->GetGameObjectByTag(DWGameObject::ETag::PLAYER);
    if (player != nullptr) return;

    DWVector2 pos = TileToWorld(1, 1, 0);
}

void DWField::ClearFieldLayer()
{
    std::vector<DWGameObject*> objs = CurrentScene->GetGameObjects<DWGameObject>();
    if (objs.empty()) return;

    for (DWGameObject* obj : objs)
    {
        if (obj == nullptr) continue;
        if (!obj->IsReuseableObject())
        {
            obj->SetDestory();
        }
    }

    DWColliderManager* colliderManager = CurrentScene->GetManager<DWColliderManager>();
    if (colliderManager == nullptr) return;
    colliderManager->ClearList();
}

bool DWField::IsPlayerPastAllStagesBottom() const
{
    DWGameObject* player = CurrentScene->GetGameObjectByTag(DWGameObject::ETag::PLAYER);
    if (player == nullptr) return false;

    const int totalStages = TotalStages();

    const int targetStageIndex = totalStages - 1;
    const int gy = StageHeight - 4;
    const int gx = StageWidth / 2;

    const DWVector2 targetPos = TileToWorld(gx, gy, targetStageIndex);

    return player->GetPosition().y >= targetPos.y;
}

DWVector2 DWField::TileToWorld(int gx, int gy, int stageIndex) const
{
    const int stageYOffsetPx = stageIndex * StageHeight * TileSize;

    const float x = static_cast<float>(OriginX + gx * TileSize + TileSize / 2);
    const float y = static_cast<float>(OriginY + stageYOffsetPx + gy * TileSize + TileSize / 2);
    return { x, y };
}

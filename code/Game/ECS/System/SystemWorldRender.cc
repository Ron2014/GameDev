#include <vector>
#include "SystemWorldRender.h"
#include "Component/ComponentLocation.h"
#include "Component/ComponentHealthPoint.h"

/**
 * world -> entity & component
 * ComponentLocation
 * ComponentHealthPoint
*/

void SystemWorldRender::Update()
{
#if FUTURE_WINDOWS
    system("cls");
#else
    system("clear");
#endif
    // std::cout << std::flush;

    // which world need render
    World *world = gWorldMgr.GetMember(m_iWorldID);
    if (!world){
        Log::Error("Render Error: world %d not exist!", m_iWorldID);
        return;
    }

    // render terrain of this world
    TerrainConfig *terrainCfg = world->GetTerrainConfig();
    // terrainCfg->DumpData();

    int gridRow = terrainCfg->GetGridRow();
    int gridCol = terrainCfg->GetGridCol();

    std::vector< std::vector<int> > entity_board(gridRow);
    for (int i=0; i<gridRow; i++){
        for (int j=0; j<gridCol; j++)
            entity_board[i].push_back(-1);
    }

    // get entity from world
    std::set<int> entity_ids = world->GetEntityIDs();
    std::set<int>::iterator it = entity_ids.begin();

    while( it != entity_ids.end()) {
        // entity exist ?
        Entity *e = gEntityMgr.GetMember(*it);

        if (e) {
            // get ComponentLocation
            Component *c = e->GetComponent(Component::location);

            if (c) {
                // get position & heading
                Vector3D position = ((ComponentLocation *)c)->vPosition;
                Vector3D heading = ((ComponentLocation *)c)->vHeading;

                int pRow = terrainCfg->Pos2GridRow(position);
                int pCol = terrainCfg->Pos2GridCol(position);

                entity_board[pRow][pCol] = e->GetID();
                // render

                c = e->GetComponent(Component::health_point);
                if (c) {
                    int maxHP = ((ComponentHealthPoint *)c)->iMaxHP;
                    int curHP = ((ComponentHealthPoint *)c)->iCurHP;

                    // render
                }
            }
        } else {
            Log::Error("Render Error: entity %d not exist!", *it);
        }

        for (int i=0; i<gridRow; i++) {
            for (int j=0; j<gridCol; j++) {
                if (entity_board[gridRow-i-1][j] >= 0) {
                    printf("-%d-|", entity_board[gridRow-i-1][j]);
                } else 
                    printf(" %d |", terrainCfg->GetGridType(j, gridRow-i-1));
            }
            printf("\n");
        }

        it++;
    }
}
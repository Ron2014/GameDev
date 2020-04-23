#include "ECS/Global.h"
#include "ECS/World/WorldMaintown.h"
#include "ECS/Component/ComponentLocation.h"
#include "ECS/Component/ComponentMoving.h"

#include "ComponentPainter.h"
#include "EntityPainter.h"
#include "SystemPainter.h"

#if FUTURE_WINDOWS
#include <windows.h>
#include <time.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif

/**
 * world -> system -> entity/component
*/

int main(char *argv[]) {
    WorldMaintown world0;
    world0.LoadTerrain("helloworld2.txt");

    EntityPainter entity0;

    ComponentLocation *c = entity0.AddComponent<ComponentLocation>();
    c->iWorldID = world0.GetID();
    c->vPosition.Zero();
    c->vHeading.Zero();

    ComponentMoving *c1 = entity0.AddComponent<ComponentMoving>();
    c1->vVelocity.Zero();
    c1->iMass = 100;
    c1->iMaxSpeed = 1;
    c1->iMaxForce = 1;
    c1->iMaxTurnRate = 0;

    ComponentPainter *c2 = entity0.AddComponent<ComponentPainter>();
    c2->pen = TerrainGrid::Walkable;

    world0.Enter(&entity0);

#if FUTURE_WINDOWS
    srand((unsigned int)time((time_t *)0));
#else
    srand(time(NULL));
#endif

    SystemWorldRender::Instance()->SetWorldID(world0.GetID());
    SystemPainter::Instance()->SetPlayerID(entity0.GetID());

    while(true) {
        SystemWorldRender::Instance()->Update();
        SystemPainter::Instance()->Update();
        SystemMoving::Instance()->Update();

#if FUTURE_WINDOWS
        Sleep(62);
#else
        sleep(0.0625);
#endif
        if (gExitGame) {
            // finish stuff at current frame
            break;
        }
    }

    gWorldMgr.RemoveMember(world0.GetID());
    gEntityMgr.RemoveMember(entity0.GetID());

    return 0;
}
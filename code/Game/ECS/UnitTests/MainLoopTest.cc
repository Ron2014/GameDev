#include <iostream>
#include "UnitTest++/src/UnitTest++.h"

#include "Global.h"

#include "Entity/EntityPlayer.h"
#include "World/WorldMaintown.h"

#include "Component/ComponentLocation.h"
#include "Component/ComponentMoving.h"

#include "System/SystemWorldRender.h"
#include "System/SystemMotionSteer.h"
#include "System/SystemPathfinding.h"
#include "System/SystemMoving.h"

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
TEST(MainLoop) {
    WorldMaintown world0;
    world0.LoadTerrain("helloworld2.txt");

    EntityPlayer entity0;

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

    world0.Enter(&entity0);

#if FUTURE_WINDOWS
    srand((unsigned int)time((time_t *)0));
#else
    srand(time(NULL));
#endif

    SystemWorldRender::Instance()->SetWorldID(world0.GetID());
    SystemMotionSteer::Instance()->SetPlayerID(entity0.GetID());

    while(true) {
        SystemWorldRender::Instance()->Update();
        SystemMotionSteer::Instance()->Update();
        SystemPathfinding::Instance()->Update();
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
}
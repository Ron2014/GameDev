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
#else
#include <unistd.h>
#endif

/**
 * world -> system -> entity/component
*/
TEST(MainLoop) {
    WorldMaintown world0;
    world0.LoadTerrain("helloworld2.txt");
    gWorldMgr.AddMember(world0.GetID(), &world0);

    EntityPlayer entity0;
    gEntityMgr.AddMember(entity0.GetID(), &entity0);

    ComponentLocation *c = (ComponentLocation *)entity0.AddComponent(Component::location);
    c->iWorldID = world0.GetID();
    c->vPosition.One();
    c->vHeading.Zero();

    ComponentMoving *c1 = (ComponentMoving *)entity0.AddComponent(Component::moving);
    c1->vVelocity.Zero();
    c1->iMass = 100;
    c1->iMaxSpeed = 100;
    c1->iMaxForce = 100;
    c1->iMaxTurnRate = 0;

    world0.Enter(&entity0);

    SystemWorldRender::Instance()->SetWorldID(world0.GetID());
    while(true) {
        SystemMotionSteer::Instance()->Update();
        SystemPathfinding::Instance()->Update();
        SystemMoving::Instance()->Update();
        SystemWorldRender::Instance()->Update();

#if FUTURE_WINDOWS
        Sleep(1000);
#else
        sleep(1);
#endif
    }

    gWorldMgr.RemoveMember(world0.GetID());
    gEntityMgr.RemoveMember(entity0.GetID());
}
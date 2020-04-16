#include <map>
#include "Global.h"

#include "Entity/EntityPlayer.h"
#include "World/WorldMaintown.h"

#include "System/SystemWorldRender.h"
#include "System/SystemMotionSteer.h"

/**
 * world -> system -> entity/component
*/
int main() {
    WorldMaintown world0;
    gWorldMgr.AddMember(world0.GetID(), &world0);

    EntityPlayer entity0;
    gEntityMgr.AddMember(entity0.GetID(), &entity0);


    world0.Enter(&entity0);
    while(true) {
        SystemWorldRender::Instance()->Update();
        SystemMotionSteer::Instance()->Update();
    }

    gWorldMgr.RemoveMember(world0.GetID());
    gEntityMgr.RemoveMember(entity0.GetID());

    return 0;
}
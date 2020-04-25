#include "Global.h"
#include "World/WorldTerrainEditor.h"
#include "System/SystemMoving.h"
#include "UIMgr.h"
#include "UIWndTerrainList.h"

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
#if FUTURE_WINDOWS
    srand((unsigned int)time((time_t *)0));
#else
    srand(time(NULL));
#endif

    UIMgr::Instance()->CreateWnd<UIWndTerrainList>();

    while(true) {
        SystemMoving::Instance()->Update();
        UIMgr::Instance()->Update();

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

    return 0;
}
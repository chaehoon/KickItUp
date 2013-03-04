#ifndef _KICKITUP_COMMON_H_
#define _KICKITUP_COMMON_H_

#include <SDL/SDL.h>
#include <cassert>
#include "../video/Coord.h"
#include "../video/Dim.h"

enum	ePlayMode {
    ePM_None = 0,
    ePM_Min,
    ePM_Easy = ePM_Min,
    ePM_Hard,
    ePM_Crazy,
    ePM_Double,

    ePM_Max
};

// 버튼과 배열을 연결시키기 위한 enum
enum    eButtonToArray
{
    eBA_Min = 0,
    eBA_1 = eBA_Min,
    eBA_3,
    eBA_5,
    eBA_7,
    eBA_9,
    eBA_Max,
};

// 버튼과 키를 연결시키기 위한 변수
static const int ARRAY_TO_BUTON[5] = {1, 7, 5, 9, 3 };

#endif // _KICKITUP_COMMON_H_

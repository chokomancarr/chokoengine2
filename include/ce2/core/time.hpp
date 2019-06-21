#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Time { CE_CLASS_COMMON
    static uint64_t _millis0;
    static uint64_t _millis;
    static uint64_t _lastMillis;
    static float _time;
    static float _delta;

    static uint64_t GetTicks();
    static void Reset();
    static void Update();
public:
    /* The current time in millisecond ticks
     */
    CE_GET_ST_MEMBER(millis);
    /* The current time in seconds
     */
    CE_GET_ST_MEMBER(time);
    /* Time elapsed since last frame in seconds
     */
    CE_GET_ST_MEMBER(delta);
};

CE_END_NAMESPACE
#include "chokoengine.hpp"

CE_BEGIN_NAMESPACE

class Time { CE_CLASS_COMMON
    static CE_EXPORT uint64_t _millis0;
    static CE_EXPORT uint64_t _millis;
    static CE_EXPORT uint64_t _lastMillis;
    static CE_EXPORT float _time;
    static CE_EXPORT float _delta;

    static uint64_t GetTicks();
    static void Update();
public:
	/* The current time in millisecond ticks
	 * This value might differ on the same frame
	 */
	static uint64_t actualMillis();
	/* The current frame time in millisecond ticks
     */
    CE_GET_ST_MEMBER(millis);
    /* The current frame time in seconds
     */
    CE_GET_ST_MEMBER(time);
    /* Time elapsed since last frame in seconds
     */
    CE_GET_ST_MEMBER(delta);

	static void Reset();
};

CE_END_NAMESPACE
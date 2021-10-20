#include <Arduino.h>

void log_msg(String category, String message);

#ifdef DEBUG
#define DBG_LOG(CATEGORY, MSG) log_msg(CATEGORY, MSG)
#else
#define DBG_LOG(CATEGORY, MSG)
#endif

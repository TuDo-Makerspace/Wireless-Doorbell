#include <log.h>

void log_msg(String category, String message)
{
        String tabs = "\t";
        
        if (category.length() < 7)
                tabs += "\t";

        Serial.println("[" + String(millis())+ "]\t\t" + category + ":" + tabs + message);
}
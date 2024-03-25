#ifndef __LOG_H__
#define __LOG_H__

#include <stdint.h>
typedef int32_t status_t;
typedef struct LogType LogType;
#define E_SUCCESS   0
#define E_FAILED    -1
struct LogType {
    status_t (*pLog)( const char *operation, status_t t_status);
};
status_t LogOperation(const char *operation, status_t t_status);
extern const struct LogType Log_Operation;

#endif
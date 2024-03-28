#ifndef __LOG_H__
#define __LOG_H__

#include "common.h"
typedef int32_t status_t;
typedef struct LogType LogType;

struct LogType {
    status_t (*pLog)( const char *operation, status_t t_status);
};
status_t LogOperation(const char *operation, status_t t_status);
extern const struct LogType Log_Operation;

#endif
#include "log.h"
#include <stdio.h>

const struct LogType Log_Operation = {
    .pLog = LogOperation
};


status_t LogOperation(const char *operation, status_t t_status)
{
    printf("%s %s\n",operation, (t_status == E_SUCCESS) ? "Successfully" : "Failed");
    return t_status;
}

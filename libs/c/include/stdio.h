#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

// 0x020146d8; called with sprintf's shape. Provisional until the function itself is matched.
int sprintf(char *dst, const char *format, ...);

#ifdef __cplusplus
}
#endif

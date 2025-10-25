#include <dlfcn.h>
#include <string.h>

#define PROTECT_SUBSTR "PROTECT"

static int (*real_remove)(const char *);

int remove(const char *filename) {
    real_remove = dlsym(RTLD_NEXT, "remove");
    char *substr = strstr(filename, PROTECT_SUBSTR);
    if (substr == NULL)
        return real_remove(filename);
    else
        return 0;
}

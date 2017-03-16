#include "utility.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>

void *xmalloc(size_t size)
{
	void *ret = malloc(size);
	assert(ret);
	return ret;
}

void *xcalloc(size_t nmemb, size_t size)
{
	void *ret = calloc(nmemb, size);
	assert(ret);
	return ret;
}

void* open_so(const char *path)
{
	int flag = RTLD_LAZY | RTLD_GLOBAL | RTLD_DEEPBIND;
	void *h = dlopen(path, flag);
	if (!h)
		error("can not open so: %s\n", path);
	return h;
}

void load_user_so(int start_i, int end_i, char *argv[])
{
	for (; start_i < end_i; start_i++) {
		if (!open_so(argv[start_i])) {
			debug("can't load: %s\n", argv[start_i]);
			exit(-1);
		}
	}
}

void* dlsym_safe(void *so_handle, const char *sym)
{
	dlerror(); /* clear old error conditions */
	void *ret = dlsym(so_handle, sym);
	char *err = dlerror();
	if (err) {
		debug("dlsym_safe(): can't find symbol '%s': %s\n", sym, err);
		exit(-1);
	}
	return ret;
}

char *dirname(const char *path)
{
	size_t len = strlen(path);
	assert(len);

	char *dirname = xmalloc(len + 100);

	int i = len;
	while (*(path + i - 1) == '/' && len > 1)
		i--;
	for (; i > 0; i--) {
		if (*(path + i - 1) == '/' &&
			(len == 1 || *(path + i - 2) != '/'))
			break;
	}
	if (i == 0)
		dirname = ".";
	else if (i == 1)
		dirname = "/";
	else
		strncat(dirname, path, i - 1);

	return dirname;
}

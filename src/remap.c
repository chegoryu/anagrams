#include "remap.h"
#include "config.h"

int remaps[][2] = CONFIG_REMAP;
int remap_sz = sizeof(remaps) / sizeof(remaps[0]);

UChar32 remap(UChar32 c)
{
	if (c < 0)
	{
		return c;
	}

	c = u_tolower(c);

	for (int i = 0; i < remap_sz; ++i)
	{
		if (c == remaps[i][0])
		{
			return remaps[i][1];
		}
	}

	return c;
}


#include <stdlib.h>
#include <cstdio>
#include <cstdint>
#include <cstring>
#include <chrono>
#include <thread>

using namespace std::chrono;

duration<uint64_t,std::milli> getWaitDuration(int argc, char** argv)
{
	for (int i = 1; i < argc; ++i)
	{
		char* end;
		uint64_t const value = strtol(argv[i], &end, 10);

		if (value == 0)
			continue;

		if (strcmp(end,"")==0 || strcmp(end,"ms")==0)
			return milliseconds(value);
		else if (strcmp(end,"s")==0)
			return seconds(value);
		else if (strcmp(end,"m")==0)
			return minutes(value);
		else if (strcmp(end,"h")==0)
			return hours(value);
	}

	return milliseconds(10);
}

int main(int argc, char** argv)
{
	auto const wait = getWaitDuration(argc, argv);

	int c;
	bool isInEscapeCode = false;
	while ((c=getchar())!=-1)
	{
		if (isInEscapeCode)
		{
			if (c != '[' && c >= 0x40 && c <= 0x7e)
			{
				isInEscapeCode = false;
			}
		}
		else if (c == 0x1b)
		{
			isInEscapeCode = true;
		}
		else
		{
			std::this_thread::sleep_for(wait);
		}

		putchar(c);
		fflush(stdout);
	}
}



#include <stdio.h>
#include "include/singleton.h"
#include "include/lock.h"


struct Data
{
	int a;
};

// typedef ted::CSingleton<Data, ted::CDummyLock> TED_DATA;
typedef ted::CSingleton<Data, ted::CMutex> TED_DATA;
#define DATA	TED_DATA::Instance()

#define SETA(v) do{ DATA->a = (v); }while(0)
#define PRINTA() do{ printf("a:%d\n", DATA->a); } while(0)

int main(int argc, const char *argv[])
{
	DATA->a = 10;
	printf("a:%d\n", DATA->a);
  SETA(2);
  PRINTA();
	return 0;
}

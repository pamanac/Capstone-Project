#include <stdlib.h>
#include <stdbool.h>
#include "windows.h"

#include "simulink.h"

DWORD WINAPI progressReport(LPVOID lpParameter) {
	LPINT lpiProgress = (LPINT)lpParameter;
	while (*lpiProgress >= 0 && *lpiProgress < 100) {
		Sleep(1000);
		*lpiProgress += 10;
	}
	return true;
}
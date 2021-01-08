// UsingCriticalSection.cpp : Defines the entry point for the console application.
//

#include<Windows.h>
#include<stdio.h>

/* The following example shows how a thread initializes , enters,
and releases a critical section. It uses the InitializeCriticalSectionAndSpinCount
EnterCriticalSection,LeaveCriticalSection,and DeleteCriticalSection functions.
*/


CRITICAL_SECTION CriticalSection;
int main()
{
	///

	//Initialize the critical section one time only
	if (!InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000400))
		return 1;


	// Release resources used by the critical section object.
	DeleteCriticalSection(&CriticalSection);
    return 0;
}


DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	// Request ownership of the critical section.

	EnterCriticalSection(&CriticalSection);

	//Access the shared resource.

	//Release ownership of the critical section.
	LeaveCriticalSection(&CriticalSection);
	
	return 1;
}


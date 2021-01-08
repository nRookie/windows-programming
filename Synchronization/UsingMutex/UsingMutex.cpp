// UsingMutex.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

/*
You can use a mutex object to protect a shared resource from simultaneous access
by multiple threads or processes. Each thread must wait for ownership of the mutex
before it can execute the code that access the shared resource. For example, if 
several threads share access to a database, the threads can use a mutex object 
to permit only one thread at a time to write the database.

The following example uses the CreateMutex function to create a mutex object and
the CreateThread function to create worker threads.

When a thread of this process writes to the data base, it first requests ownership of
the mutex using the WaitForSingleOjbectfunction. If the thread obtains ownership of the mutex
,it writes to the database and then releases its ownership of the mutex using the 
ReleaseMutex function.


This example uses structured exception handling to ensure that the thread properly release
the mutex object. The __finally block of code is executed no matter how the
__try block terminates (unless the __try block  includes a call to the TerminateThread
function). This prevents the mutex object from being abandoned inadvertently.

If a mutex is abandoned, the thread that owned the mutex did not properly
release it before terminating. In this case, the status of the shared resource
is indeterminate. and continuing to use the mutex can obscure a potentially
serious error. Some applications might attempt to restore the resource to
a consistent state; this example simply returns an error and stops using the mutex.
For more information, See Mutex Object.

*/

#include<Windows.h>
#include<stdio.h>

#define THREADCOUNT 2

HANDLE ghMutex;
int buffer[1000] = { 0 };
int ii = 0 ;
DWORD WINAPI WriteToDatabase(LPVOID);

int main(void)
{
	HANDLE aThread[THREADCOUNT];
	DWORD ThreadID;
	int i;

	// Create a mutex with no initial owner

	ghMutex = CreateMutex(
		NULL,   //default security attributes
		FALSE, //initially not owned
		NULL); // unnamed mutex

	if (ghMutex == NULL)
	{
		printf("CreateMutex error:%d\n", GetLastError());
		return 1;
	}

	// Create worker threads

	for (i = 0; i < THREADCOUNT; i++)
	{
		aThread[i] = CreateThread(
			NULL, // default security attributes
			0, // default stack size
			(LPTHREAD_START_ROUTINE)WriteToDatabase,
			NULL, // no thread arguments
			0, //default creation flags
			&ThreadID);

		if (aThread[i] == NULL)
		{
			printf("CreateThread error: %d\n", GetLastError());
			return 1;
		}
	}

	// Wait for all threads to terminate

	WaitForMultipleObjects(THREADCOUNT, aThread, TRUE, INFINITE);

	// Close thread and mutex handles

	for (i = 0; i < THREADCOUNT; i++)
		CloseHandle(aThread[i]);

	CloseHandle(ghMutex);

	system("pause");
	for (int i = 0; i <= ii; i++)
	{
		printf("%d\n", buffer[i]);
	}
	system("pause");
	return 0;
}

DWORD WINAPI WriteToDatabase(LPVOID lpParam)
{
	// lpParam not used in this example
	UNREFERENCED_PARAMETER(lpParam);

	DWORD dwCount = 0, dwWaitResult;

	// Request ownership of mutex.

 
		dwWaitResult = WaitForSingleObject(
			ghMutex,    // handle to mutex
			INFINITE);  // no time-out interval

		switch (dwWaitResult)
		{
			// The thread got ownership of the mutex
		case WAIT_OBJECT_0:
			__try {
				// TODO: Write to the database
				printf("Thread %d writing to database...\n",
					GetCurrentThreadId());
				buffer[ii] = GetCurrentThreadId();
				
			}

			__finally {
				// Release ownership of the mutex object
				if (!ReleaseMutex(ghMutex))
				{
					// Handle error.
				}
			}
			break;

			// The thread got ownership of an abandoned mutex
			// The database is in an indeterminate state
		case WAIT_ABANDONED:
			return FALSE;
		}
 
	return TRUE;
}



 


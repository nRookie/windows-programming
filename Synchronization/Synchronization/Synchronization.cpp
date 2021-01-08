// Synchronization.cpp : Defines the entry point for the console application.
//


/* The names of object share the same namespace 
Therefore, when creating named objects,use unique names and be sure to check function return
values for duplicate-name errors*/



/*having multiple processes use CreateMutex for the same mutex is therefore equivalent to having one
process that calls CreateMutex while the other processes call OpenMutex, except that it eliminates 
the need to ensure that the creating process is started first*/



/* A terminal Services environment has a gloabl namespace for events , semaphores, mutexes,
waitable timers,file-mapping objects, and job objects. In addition , each Terminal
Services Client session has its own separate namespace for these objects. Terminal Services
client processes can use object names with a "\Global\" or "\Local\" prefix to explicitly
create an object in the global or session namespace. For more information, see Kernel
Object Namespaces. Fast user switching is implemented object names must follow the guidelines
outlined for terminal services so that applications can support multiple users.

Synchronization objects can be created in a private namespace. For more information,see
Object Namespaces.
*/


#include<Windows.h>
#include<stdio.h>
#include<conio.h>



/*
Waiting for Multiple Objects

The following example uses the CreateEvent function to create two event objects and the create 
CreateThead function to create a thread. It then uses the WaitForMultipleObjects function
to wait for the thread to set the state of one of the objects to signaled using the
SetEvent function.
*/

HANDLE ghEvents[2];

DWORD WINAPI ThreadProc(LPVOID);

void WaitMulObjects()
{
	HANDLE hThread;
	DWORD i, dwEvent, dwThreadID;

	//Create two event objects

	for (i = 0; i < 2; i++)
	{
		ghEvents[i] = CreateEvent(
			NULL, //default security attributes
			FALSE, // auto-reset event object
			FALSE, // initial state is nonsignaled
			NULL); // unnamed object
		
		if (ghEvents[i] == NULL)
		{
			printf("CreateEvent error: %d\n", GetLastError());
			ExitProcess(0);
		}
	}

	// Create a thread

	hThread = CreateThread(
		NULL, //default security attributes
		0,   // default stack size
		(LPTHREAD_START_ROUTINE)ThreadProc,
		NULL, // no thread function arguments
		0, // default creation flags
		&dwThreadID); //receive thread identifier

	if (hThread = NULL)
	{
		printf("CreateThread error: %d \n", GetLastError());
		return;
	}

	// Wait for the thread to signal one of the event objects

	dwEvent = WaitForMultipleObjects(
		2, // number of objecs in array
		ghEvents, // array of objects
		FALSE,   // wait for any object
		5000);     // five-second wait

	// The return value indicates which event is signaled

	switch (dwEvent)
	{
		// ghEvents[0] was signaled
	case WAIT_OBJECT_0 + 0:
			//TODO: Perform tasks required by this event
		printf("First event was signaled.\n");
		break;
	case WAIT_OBJECT_0 + 1:
		printf("Second event was signaled. \n");
		break;
	case WAIT_TIMEOUT:
		printf("wait timed out .\n");
		break;
	default:
		printf("Wait error: %d\n", GetLastError());
		ExitProcess(0);
	}

	//Close event handles
	for (i = 0; i < 2; i++)
		CloseHandle(ghEvents[i]);

	system("pause");
	return;
}

DWORD WINAPI ThreadProc(LPVOID lpParam)
{

	// lpParam not used in this example
	UNREFERENCED_PARAMETER(lpParam);

	// Set one event to the signaled state

	if (!SetEvent(ghEvents[0]))
	{
		printf("SetEvent failed (%d)\n", GetLastError());
		return 1;
	}
	return 0;
}



int main()
{
	WaitMulObjects();
	return 0;
}
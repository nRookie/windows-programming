// firstProcess.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include<Windows.h>
#include <conio.h>
#include <stdio.h>
int main()
{
	/* Frist Process
	The first process uses the CreateMutex function to create the mutex object. Note that
	this function succeeds even if there is an existing object with the same name.

	*/

 
		HANDLE hMutex;

		hMutex = CreateMutex(
			NULL,    // default security descriptor
			FALSE,   // mutex not owned
			TEXT("NameOfMutexObject"));  // object name

		if (hMutex == NULL)
			printf("CreateMutex error: %d\n", GetLastError());
		else
			if (GetLastError() == ERROR_ALREADY_EXISTS)
				printf("CreateMutex opened an existing mutex \n");
			else printf("CreateMutex created a new mutex .\n");

			// keep this process around until the second process is run
			_getch();

			CloseHandle(hMutex);
			return 0;

}


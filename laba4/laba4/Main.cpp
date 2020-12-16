#include "Header.h"

#include <iostream>
#include <conio.h>


using namespace std;
int wmain(int argc, wchar_t *argv[])
{
	if (argc == 3)
	{
		if (!wcscmp(argv[1], TEXT("-check")))
			Check(argv[2]);
		else if (!wcscmp(argv[2], TEXT("-check")))
			Check(argv[1]);
		else
		{
			cout << "-check <*.bmp>\n";
		}
	}
	else if (argc == 4)
	{
		if (!wcscmp(argv[1], TEXT("-embed")))
			Embed(argv[2], argv[3]);
		else if (!wcscmp(argv[1], TEXT("-retrieve")))
			Retrieve(argv[2], argv[3]);
		else
		{
			cout << "-embed <*.bmp> <*.txt>\n";
			cout << "-retrieve <*.bmp> <*.txt>\n";
		}
	}
	else
	{
		cout << "-check <*.bmp>\n";
		cout << "-embed <*.bmp> <*.txt>\n";
		cout << "-retrieve <*.bmp> <*.txt>\n";
	}
	return 0;
}

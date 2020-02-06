
/* Driver 6*/

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "disk.h"
#include "diskmanager.h"
#include "partitionmanager.h"
#include "filesystem.h"
#include "client.h"

using namespace std;

/*
  This driver will test the getAttribute() and setAttribute()
  functions. You need to complete this driver according to the
  attributes you have implemented in your file system, before
  testing your program.


  Required tests:
  get and set on the fs1 on a file
	and on a file that doesn't exist
	and on a file in a directory in fs1
	and on a file that doesn't exist in a directory in fs1

 fs2, fs3
  on a file both get and set on both fs2 and fs3

  samples are provided below.  Use them and/or make up your own.


*/

int main()
{

	Disk* d = new Disk(300, 64, const_cast<char*>("DISK1"));
	DiskPartition* dp = new DiskPartition[3];

	dp[0].partitionName = 'A';
	dp[0].partitionSize = 100;
	dp[1].partitionName = 'B';
	dp[1].partitionSize = 75;
	dp[2].partitionName = 'C';
	dp[2].partitionSize = 105;

	DiskManager* dm = new DiskManager(d, 3, dp);
	FileSystem* fs1 = new FileSystem(dm, 'A');
	FileSystem* fs2 = new FileSystem(dm, 'B');
	FileSystem* fs3 = new FileSystem(dm, 'C');
	Client* c1 = new Client(fs1);
	Client* c2 = new Client(fs2);
	Client* c3 = new Client(fs3);
	Client* c4 = new Client(fs1);
	Client* c5 = new Client(fs2);



	int r;


	//What every need to show your set and get Attributes functions work
	char attribute[64];
	r = c1->myFS->setAttribute(const_cast<char*>("/e/f"), 4, 0, const_cast<char*>("Gage"), 4);
	cout << "rv from setAttribute 0 for /e/f is " << r << (r == 4 ? " Correct" : " fail") << endl;
	r = c4->myFS->setAttribute(const_cast<char*>("/e/f"), 4, 1, const_cast<char*>("Purple"), 6);
	cout << "rv from setAttribute 1 for /e/f is " << r << (r == 6 ? " Correct" : " fail") << endl;
	r = c1->myFS->getAttribute(const_cast<char*>("/e/f"), 4, 0, attribute);
	cout << "rv from getAttribute 0 for /e/f is " << r << (r == 0 ? " Correct" : " fail") << endl;
	if (r == 0)
	{
		cout << "Data read was: " << endl;
		for (int i = 0; i < 4; i++) cout << attribute[i];
		cout << endl;
	}
	r = c1->myFS->getAttribute(const_cast<char*>("/e/f"), 4, 1, attribute);
	cout << "rv from getAttribute 1 for /e/f is " << r << (r == 0 ? " Correct" : " fail") << endl;
	if (r == 0)
	{
		cout << "Data read was: " << endl;
		for (int i = 0; i < 6; i++) cout << attribute[i];
		cout << endl;
	}
	
	
	r = c4->myFS->setAttribute(const_cast<char*>("/e/b"), 4, 0, const_cast<char*>("Archduke Ferdinand"), 18);
	cout << "rv from setAttribute 0 for /e/b is " << r << (r == 18 ? " Correct" : " fail") << endl;
	r = c4->myFS->getAttribute(const_cast<char*>("/e/b"), 4, 0, attribute);
	cout << "rv from getAttribute 0 for /e/b is " << r << (r == 0 ? " Correct" : " fail") << endl;
	if (r == 0)
	{
		cout << "Data read was: " << endl;
		for (int i = 0; i < 18; i++) cout << attribute[i];
		cout << endl;
	}
	r = c1->myFS->getAttribute(const_cast<char*>("/p"), 2, 0, attribute);  //should fail!
	cout << "rv from getAttribute 0 for /p is " << r << (r == -1 ? " Correct" : " fail") << endl;
	r = c4->myFS->setAttribute(const_cast<char*>("/p"), 2, 0, const_cast<char*>("Archduke Ferdinand"), 18);  //should fail!
	cout << "rv from setAttribute 0 for /e/b is " << r << (r == -1 ? " Correct" : " fail") << endl;

	for(int i = 0; i < 64; i++) attribute[i] = ' ';
	//////////////////////////////////////FileSystem 2 Test/////////////////////////////////////////
	r = c5->myFS->setAttribute(const_cast<char*>("/z"), 2, 1, const_cast<char*>("White"), 5);
	cout << "rv from setAttribute 1 for /z in FS2 is " << r << (r == 5 ? " Correct" : " fail") << endl;
	r = c5->myFS->getAttribute(const_cast<char*>("/z"), 2, 1, attribute);
	cout << "rv from getAttribute 1 for /z in FS2 is " << r << (r == 0 ? " Correct" : " fail") << endl;

		if (r == 0)
	{
		cout << "Data read was: " << endl;
		for (int i = 0; i < 18; i++) cout << attribute[i];
		cout << endl;
	}
	r = c5->myFS->setAttribute(const_cast<char*>("/z"),  2, 0, const_cast<char*>("Gandalf"), 7);
	cout << "rv from setAttribute 0 for /z in FS2 is " << r << (r == 7 ? " Correct" : " fail") << endl;
	r = c5->myFS->getAttribute(const_cast<char*>("/z"), 2, 0, attribute);
	cout << "rv from getAttribute 0 for /z in FS2 is " << r << (r == 0 ? " Correct" : " fail") << endl;
	if (r == 0)
	{
		cout << "Data read was: " << endl;
		for (int i = 0; i < 18; i++) cout << attribute[i];
		cout << endl;
	}

	for(int i = 0; i < 64; i++) attribute[i] = ' ';
	//////////////////////////////////////FileSystem 3 Test/////////////////////////////////////////
	r = c3->myFS->setAttribute(const_cast<char*>("/a"), 2, 0, const_cast<char*>("Luxanna"), 7);
	cout << "rv from setAttribute 0 for /a in FS3 is " << r << (r == 7 ? " Correct" : " fail") << endl;
	r = c3->myFS->getAttribute(const_cast<char*>("/a"), 2, 0, attribute);
	cout << "rv from getAttribute 0 for /a in FS3 is " << r << (r == 0 ? " Correct" : " fail") << endl;
	if (r == 0)
	{
		cout << "Data read was: " << endl;
		for (int i = 0; i < 18; i++) cout << attribute[i];
		cout << endl;
	}
	
	r = c3->myFS->setAttribute(const_cast<char*>("/a"), 2, 1, const_cast<char*>("Rainbow"), 7);
	cout << "rv from setAttribute 1 for /a in FS3 is " << r << (r == 7 ? " Correct" : " fail") << endl;
	r = c3->myFS->getAttribute(const_cast<char*>("/a"), 2, 1, attribute);
	cout << "rv from getAttribute 1 for /a in FS3 is " << r << (r == 0 ? " Correct" : " fail") << endl;
	if (r == 0)
	{
		cout << "Data read was: " << endl;
		for (int i = 0; i < 18; i++) cout << attribute[i];
		cout << endl;
	}
	

	return 0;
}

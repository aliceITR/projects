
/* This is an example of a driver to test the filesystem */

#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "partitionmanager.h"
#include "filesystem.h"
#include "client.h"
using namespace std;

#define bLENGTH 24

int main()
{
        cout << "starting" << endl;
        Disk* d = new Disk(300, 64, const_cast<char*>("DISK1"));
        cout << "disk was made" << endl;
        DiskPartition* dp = new DiskPartition[3];

        dp[0].partitionName = 'A';
        dp[0].partitionSize = 100;
        dp[1].partitionName = 'B';
        dp[1].partitionSize = 75;
        dp[2].partitionName = 'C';
        dp[2].partitionSize = 105;
        //cout << "disk partitions were made" << endl;
        DiskManager* dm = new DiskManager(d, 3, dp);
        //cout << "disk manager was made" << endl;
        FileSystem* fs1 = new FileSystem(dm, 'A');
        Client* c2 = new Client(fs1);
        
		int i, r, l1, l2, f1, f2, f3, f4, f5;
		char buf1[37], buf2[64], buf3[100], buf4[600], buf5[1200];
		char rbuf1[37], rbuf2[64], rbuf3[100], rbuf4[600], rbuf5[1200];

		for (i = 0; i < 37; i++) buf1[i] = 's';
		for (i = 0; i < 64; i++) buf2[i] = 'b';
		for (i = 0; i < 100; i++) buf3[i] = 'K';
		for (i = 0; i < 600; i++) buf4[i] = 'M';

		        r = c2->myFS->createDirectory(const_cast<char*>("/o"), 2);
        r = c2->myFS->createDirectory(const_cast<char*>("/o/o"), 4);
        r = c2->myFS->createDirectory(const_cast<char*>("/o/o/o"), 6);
        r = c2->myFS->createDirectory(const_cast<char*>("/o/o/o/a"), 8);
        r = c2->myFS->createFile(const_cast<char*>("/o/o/o/a/k"), 10);
		r = c2->myFS->createFile(const_cast<char*>("/z"), 2);
		cout << "rv from createFile /z is " << r << (r == 0 ? " correct " : " fail") << endl;
		f2 = c2->myFS->openFile(const_cast<char*>("/o/o/o/a/k"), 10, 'm', -1);
		cout << "rv from openFile /o/o/o/a/k r is " << f2 << (f2 > 0 ? " correct " : " fail") << endl;
		r = c2->myFS->writeFile(f2, buf4, 600);
		cout << "rv from writeFile /o/o/o/a/k is " << r << (r == 600 ? " correct " : " fail") << endl;
		r = c2->myFS->writeFile(f2, buf4, 600);
		cout << "rv from writeFile /o/o/o/a/k is " << r << (r == 600 ? " correct " : " fail") << endl;

// 		r = c2->myFS->seekFile(f2, 32, 1);
// 		cout << "rv from seekFile is " << r << (r == 0 ? " correct " : " fail") << endl;
// 		r = c2->myFS->writeFile(f2, buf2, 64);
// 		cout << "rv from writeFile /z is " << r << (r == 64 ? " correct " : " fail") << endl;
// 
// 		for (i = 62; i < 1152; i += 64) {
// 			r = c2->myFS->seekFile(f2, i, 1);
// 			cout << "rv from seekFile is " << r << endl;
// 			r = c2->myFS->writeFile(f2, buf1, 4);
// 			cout << "rv from writeFile /z is " << r << (r == 4 ? " correct " : " fail") << endl;
// 		}
// 		r = c2->myFS->appendFile(f2, buf2, 16);
// 		cout << "rv from appendFile is " << r << (r == 16 ? " correct " : " fail") << endl;
// 
// 		r = c2->myFS->appendFile(f2, buf1, 4);
// 		cout << "rv from appendFile is " << r << (r == -3 ? " correct exceeds max file size" : " fail") << endl;
// 
// 		r = c2->myFS->seekFile(f2, 0, -1);
// 		cout << "rv from seekFile is " << r << endl;
// 
// 		for (int j = 0; j < 19; j++) {
// 			r = c2->myFS->readFile(f2, rbuf2, 64);
// 			cout << "rv from readFile /z is " << r << (r == 64 ? " correct " : " fail") << endl;
// 			cout << "Data read is " << endl;
// 			for (i = 0; i < r; i++) cout << rbuf2[i];
// 			cout << endl;
// 		}
// 		r = c2->myFS->closeFile(f2);
// 		cout << "rv from closeFile /z is " << r << (r == 0 ? " correct" : " fail") << endl;


//         
        return 0;
}
#include <iostream>
#include "UHideFileClass.h"

using namespace std;

int main()
{
	string srcFile = "C:\\Users\\s12\\Documents\\example.txt";
	string dstFile = "C:\\Users\\s12\\Documents\\example2.txt";
    UHideFile hideFile = UHideFile();
	hideFile.hideFragmentedEncryptedFile(srcFile, dstFile, "pis", 3);
	getchar();
	hideFile.restoreFragmentedEncryptedFile(srcFile, dstFile, "pis", 3);
	return 0;
}
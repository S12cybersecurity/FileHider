#include <iostream>
#include "UHideFileClass.h"

using namespace std;

int main()
{
	string srcFile = "C:\\Users\\s12\\Documents\\example.txt";
	string dstFile = "C:\\Users\\s12\\Documents\\example2.txt";
    UHideFile hideFile = UHideFile();
	char dynamicKey = hideFile.hideFragmentedEncryptedFile(srcFile, dstFile, "test", 3);
	getchar();
	hideFile.restoreFragmentedEncryptedFile(dstFile, srcFile, "test", 3, dynamicKey);
	return 0;
}
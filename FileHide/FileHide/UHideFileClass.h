#include <Windows.h>
#include <iostream>
#include <string>

using namespace std;

class UHideFile {
private:
    string targetFile;
    string adsName;
    string adsContent;
    string adsPath;
public:
    UHideFile(string targetFile, string adsName, string adsContent) {
        this->targetFile = targetFile;
        this->adsName = adsName;
        this->adsContent = adsContent;
        adsPath = targetFile + ":" + adsName;
    }
    UHideFile() {}
    // Setters
    void setTargetFile(string targetFile) {
        this->targetFile = targetFile;
    }
    void setADSName(string adsName) {
        this->adsName = adsName;
    }
    void setADSContent(string adsContent) {
        this->adsContent = adsContent;
    }
    void setADSPath(string adsPath) {
        this->adsPath = adsPath;
    }
    // Getters
    string getTargetFile() {
        return this->targetFile;
    }
    string getADSName() {
        return this->adsName;
    }
    string getADSContent() {
        return this->adsContent;
    }
    string getADSPath() {
        return this->adsPath;
    }
    // Hide file converting a file content to an ADS.
    bool hideFile(string sourceFile, string destinationFile) {
        // Open the source file for reading.
        HANDLE hSourceFile = CreateFileA(sourceFile.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hSourceFile == INVALID_HANDLE_VALUE) {
            cout << "Error: " << GetLastError() << endl;
            return false;
        }
        // Read the source file content.
        const int bufferSize = 1024;
        char buffer[bufferSize];
        DWORD bytesRead;
        string adsContent = "";
        while (ReadFile(hSourceFile, buffer, bufferSize, &bytesRead, NULL) && bytesRead > 0) {
            adsContent += buffer;
        }
        // Create ADS
        string adsPath = destinationFile + ":" + adsName;
        HANDLE hFile = CreateFileA(adsPath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
            cout << "Error: " << GetLastError() << endl;
            return false;
        }
        // Write data to the Alternate Data Stream.
        DWORD dwBytesWritten = 0;
        bool bErrorFlag = WriteFile(hFile, adsContent.c_str(), strlen(adsContent.c_str()), &dwBytesWritten, NULL);
        if (FALSE == bErrorFlag) {
            cout << "Error: in writing file content to ADS" << GetLastError() << endl;
            return false;
        }
        // Remove the original file.
        CloseHandle(hFile);
        CloseHandle(hSourceFile);
        if (remove(targetFile.c_str()) != 0) {
            cout << "Error: in removing original file" << GetLastError() << endl;
            return false;
        }
        std::cout << "Alternate Data Stream created successfully." << std::endl;
        return true;
    }
    // Unhide file converting the ADS to a file content.
    bool unHideFile(string sourceFile, string destinationFile) {
        HANDLE hFile = CreateFileA(destinationFile.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hFile == INVALID_HANDLE_VALUE) {
            cout << "Error in creating target file: " << GetLastError() << endl;
            return false;
        }
        // Open the ADS for reading.
        string adsPath = sourceFile + ":" + adsName;
        cout << adsPath << endl;
        HANDLE hADS = CreateFileA(adsPath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hADS == INVALID_HANDLE_VALUE) {
            cout << "Error in opening ADS: " << GetLastError() << endl;
            CloseHandle(hFile);
            return false;
        }
        // Read data from the ADS and write it to the target file.
        const int bufferSize = 1024;
        char buffer[bufferSize];
        DWORD bytesRead, bytesWritten;
        while (ReadFile(hADS, buffer, bufferSize, &bytesRead, NULL) && bytesRead > 0) {
            WriteFile(hFile, buffer, bytesRead, &bytesWritten, NULL);
        }
        // Close the handles.
        CloseHandle(hADS);
        CloseHandle(hFile);
        // Remove the ADS.
        if (remove(adsPath.c_str()) != 0) {
            cout << "Error in removing ADS: " << GetLastError() << endl;
            CloseHandle(hADS);
            CloseHandle(hFile);
            return false;
        }
        std::cout << "File content restored successfully." << std::endl;
        return true;
    }


    void encryptFileWithXOR(const char* fileName, char xorKey) {
        FILE* file = fopen(fileName, "rb+");
        if (file == NULL) {
            perror("Error opening the file for encryption");
            exit(EXIT_FAILURE);
        }

        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        fseek(file, 0, SEEK_SET);

        char* buffer = (char*)malloc(fileSize);
        if (buffer == NULL) {
            fclose(file);
            exit(EXIT_FAILURE);
        }

        fread(buffer, 1, fileSize, file);

        //char xorKey = 'S';
        for (long i = 0; i < fileSize; ++i) {
            buffer[i] ^= xorKey;
        }

        fseek(file, 0, SEEK_SET);
        fwrite(buffer, 1, fileSize, file);

        free(buffer);
        fclose(file);
    }

    void splitFile(const char* fileName, int numChunks) {
        FILE* file = fopen(fileName, "rb");
        if (file == NULL) {
            perror("Error opening the file");
            exit(EXIT_FAILURE);
        }

        fseek(file, 0, SEEK_END);
        long fileSize = ftell(file);
        rewind(file);

        int chunkSize = fileSize / numChunks;
        char buffer[1024];

        for (int i = 0; i < numChunks; ++i) {
            char chunkName[256];
            sprintf(chunkName, "%s.part%d", fileName, i + 1);
            FILE* chunk = fopen(chunkName, "wb");
            if (chunk == NULL) {
                perror("Error creating the chunk");
                fclose(file);
                exit(EXIT_FAILURE);
            }

            int bytesToRead = (i == numChunks - 1) ? fileSize - (chunkSize * i) : chunkSize;
            fread(buffer, 1, bytesToRead, file);
            fwrite(buffer, 1, bytesToRead, chunk);

            fclose(chunk);
        }

        fclose(file);
    }

    char generateDynamicKey() {
        std::srand(std::time(0));
        return static_cast<char>(std::rand() % 256); // Generate a random number between 0 and 255
    }

    char hideFragmentedEncryptedFile(const std::string& sourceFile, const std::string& destinationFile, const std::string& adsName, int numChunks) {
        // Step 1: Encrypt the File
        char key = generateDynamicKey();
        encryptFileWithXOR(sourceFile.c_str(), key);

        // Step 2: Split the File
        splitFile(sourceFile.c_str(), numChunks);

        // Step 3: Hide each chunk in a different ADS
        for (int i = 0; i < numChunks; ++i) {
            char chunkName[256];
            sprintf(chunkName, "%s.part%d", sourceFile.c_str(), i + 1);

            std::string adsPath = destinationFile + ":" + adsName + std::to_string(i + 1);
            FILE* ads = fopen(adsPath.c_str(), "wb");
            if (ads == NULL) {
                perror("Error creating the ADS");
                return false;
            }

            FILE* chunk = fopen(chunkName, "rb");
            if (chunk == NULL) {
                perror("Error opening the chunk");
                fclose(ads);
                return false;
            }

            char buffer[1024];
            size_t bytesRead;
            while ((bytesRead = fread(buffer, 1, sizeof(buffer), chunk)) > 0) {
                fwrite(buffer, 1, bytesRead, ads);
            }

            fclose(chunk);
            fclose(ads);
            remove(chunkName);
        }

        // Remove the original file
        if (remove(sourceFile.c_str()) != 0) {
            perror("Error deleting the original file");
            return false;
        }

        return key;
    }


    bool restoreFragmentedEncryptedFile(const std::string& sourceFile, const std::string& destinationFile, const std::string& adsBaseName, int numChunks, char xorKey) {
        // Open the destination file for writing
        FILE* destFile = fopen(destinationFile.c_str(), "wb");
        if (destFile == NULL) {
            perror("Error opening the destination file");
            return false;
        }

        char buffer[1024];

        // Iterate through each ADS fragment
        for (int i = 0; i < numChunks; ++i) {
            // Construct the ADS name dynamically
            std::string adsPath = sourceFile + ":" + adsBaseName + std::to_string(i + 1);

            // Open the ADS for reading
            FILE* ads = fopen(adsPath.c_str(), "rb");
            if (ads == NULL) {
                perror(("Error opening ADS fragment: " + adsPath).c_str());
                fclose(destFile);
                return false;
            }

            // Read from the ADS and write to the destination file
            size_t bytesRead;
            while ((bytesRead = fread(buffer, 1, sizeof(buffer), ads)) > 0) {
                fwrite(buffer, 1, bytesRead, destFile);
            }

            fclose(ads);
        }

        fclose(destFile);

        // Decrypt the restored file
        encryptFileWithXOR(destinationFile.c_str(), xorKey);

        // Remove the ADS fragments
		for (int i = 0; i < numChunks; ++i) {
			std::string adsPath = sourceFile + ":" + adsBaseName + std::to_string(i + 1);
			remove(adsPath.c_str());
		}

        return true;
    }




};
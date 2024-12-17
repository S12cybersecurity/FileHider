# FileHider
C++ Implementation to Hide File in ADS with the option of Encrpytion + Fragmentation

The hideFragmentedEncryptedFile() function can be used to hide some file using the following steps:
- Encrypt the File
- Split the File
- Hide each chunk in a different ADS
- Remove the original file

![hideFike](https://github.com/user-attachments/assets/7240b65a-348e-47fe-9902-47b9ef2dd350)

### encryptFileWithXOR
The encryptFileWithXOR function applies a simple XOR encryption to a file to obfuscate its content. It reads the file into memory and iterates through each byte, applying an XOR operation with a predefined key (in this case, 'S'). The result is written back to the file, effectively encrypting or decrypting it, depending on whether the operation is repeated. This lightweight encryption method is ideal for quick obfuscation while ensuring reversibility.

### splitFile
The splitFile function splits a given file into multiple smaller fragments (chunks) to facilitate fragmentation. It calculates the size of each chunk based on the total file size and the specified number of chunks. The function then creates separate files for each chunk and writes the respective portions of the original file into them. This is crucial for distributing encrypted file fragments into multiple Alternate Data Streams (ADS) for added stealth.

### hideFragmentedEncryptedFile
The hideFragmentedEncryptedFile function combines encryption, fragmentation, and ADS storage. It first encrypts the source file using the encryptFileWithXOR function and then splits it into smaller chunks via splitFile. Each chunk is hidden within a separate ADS of the specified destination file using dynamically generated ADS names. After successfully storing all fragments, the original file is deleted to ensure no traces remain. This function is the core of the concealment process.

### restoreFragmentedEncryptedFile
The restoreFragmentedEncryptedFile function restores an encrypted and fragmented file hidden in multiple ADS. It sequentially reads each ADS fragment from the specified source file and writes its content into a destination file, reconstructing the original data. Once all fragments are restored, it decrypts the file using encryptFileWithXOR to recover the original content. This function completes the full lifecycle of the technique, allowing the hidden file to be restored seamlessly.

Additionally, you can use the hideFile() function to store the file content in an ADS without encryption or fragmentation!

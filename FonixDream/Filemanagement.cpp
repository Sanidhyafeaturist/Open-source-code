#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sys/stat.h> // Include for mkdir on Unix-like systems
#ifdef _WIN32
#include <direct.h> // Include for mkdir on Windows
#endif

using namespace std;

// Map to associate file extensions with images
map<string, string> fileExtensionsToImages;

// Function to initialize file extension to image mappings
void initializeFileExtensionsToImages() {
    fileExtensionsToImages[".wid"] = "OIP (1).jpeg";
    fileExtensionsToImages[".cpp"] = "download.jpeg";
    fileExtensionsToImages[".texture"] = "OIP (2).jpeg";
    fileExtensionsToImages[".server"] = "OIP (2).jpeg";
    // Add more mappings as needed for different file types
}

// Function to create a folder (directory)
bool createFolder(const string& folderName) {
#ifdef _WIN32
    if (_mkdir(folderName.c_str()) == 0) {
        return true;
    }
#else
    if (mkdir(folderName.c_str(), 0777) == 0) {
        return true;
    }
#endif
    return false;
}

// Function to create a file with a custom extension in a specified folder
void createFile(const string& folderName, const string& fileName, const string& extension) {
    // Create the folder if it doesn't exist
    if (!createFolder(folderName)) {
        cerr << "Failed to create folder: " << folderName << endl;
        return;
    }

    ofstream file(folderName + "/" + fileName + extension);

    if (!file) {
        cerr << "Failed to create file: " << fileName << extension << endl;
        return;
    }

    cout << "File created: " << fileName << extension << " in folder: " << folderName << endl;
    file.close();
}

// Function to write content to a file in a specified folder
void writeFile(const string& folderName, const string& fileName, const string& content, const string& extension) {
    // Create the folder if it doesn't exist
    if (!createFolder(folderName)) {
        cerr << "Failed to create folder: " << folderName << endl;
        return;
    }

    ofstream file(folderName + "/" + fileName + extension, ios::app);

    if (!file) {
        cerr << "Failed to open file for writing: " << fileName << extension << endl;
        return;
    }

    file << content << endl;
    cout << "Content written to " << fileName << extension << " in folder: " << folderName << endl;
    file.close();
}

// Function to read and display the contents of a file in a specified folder
void readFile(const string& folderName, const string& fileName, const string& extension) {
    ifstream file(folderName + "/" + fileName + extension);

    if (!file) {
        cerr << "Failed to open file for reading: " << fileName << extension << " in folder: " << folderName << endl;
        return;
    }

    cout << "Contents of " << fileName << extension << " in folder: " << folderName << ":\n";

    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }

    file.close();
}

// Function to delete a file in a specified folder
void deleteFile(const string& folderName, const string& fileName, const string& extension) {
    if (remove((folderName + "/" + fileName + extension).c_str()) != 0) {
        cerr << "Failed to delete file: " << fileName << extension << " in folder: " << folderName << endl;
    }
    else {
        cout << "File deleted: " << fileName << extension << " in folder: " << folderName << endl;
    }
}

int main() {
    string folderName = "CustomFiles"; // Folder name
    string fileName = "example";      // File name without extension
    string extension = ".txt";        // Default extension

    // Initialize file extension to image mappings
    initializeFileExtensionsToImages();

    // Create a file with a custom extension in the specified folder
    createFile(folderName, fileName, extension);

    // Write content to the file
    string content = "Hello, World!";
    writeFile(folderName, fileName, content, extension);

    // Read and display the file's contents
    readFile(folderName, fileName, extension);

    // Delete the file
    deleteFile(folderName, fileName, extension);

    // Get the associated image for the extension
    string imageFileName = fileExtensionsToImages[extension];

    // Display the associated image (placeholder) for the file type
    if (!imageFileName.empty()) {
        cout << "Associated image: " << imageFileName << endl;
    }
    else {
        cout << "No associated image found for extension: " << extension << endl;
    }

    return 0;
}

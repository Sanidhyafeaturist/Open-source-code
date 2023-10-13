#include <QCoreApplication>
#include <QFileDialog>
#include <iostream>
#include <windows.h>
#include <vector>
#include <thread>
#include <atomic>
#include <windows.h>

// Function to watch a folder using ReadDirectoryChangesW
void watchFolder(const std::wstring& folderPath, std::atomic<bool>& stopFlag) {
    HANDLE hDir = CreateFileW(
        folderPath.c_str(),
        FILE_LIST_DIRECTORY,
        FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
        NULL,
        OPEN_EXISTING,
        FILE_FLAG_BACKUP_SEMANTICS,
        NULL
    );

    if (hDir == INVALID_HANDLE_VALUE) {
        std::wcerr << L"Failed to open directory: " << folderPath << std::endl;
        return;
    }

    CHAR buffer[4096];
    DWORD bytesReturned;
    BOOL result = FALSE;

    std::wcout << L"Watching folder: " << folderPath << std::endl;
    while (!stopFlag.load()) {
        result = ReadDirectoryChangesW(
            hDir,
            buffer,
            sizeof(buffer),
            FALSE,
            FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_SIZE,
            &bytesReturned,
            NULL,
            NULL
        );

        if (!result) {
            std::wcerr << L"ReadDirectoryChangesW failed." << std::endl;
            break;
        }

        FILE_NOTIFY_INFORMATION* fileInfo = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(buffer);

        while (fileInfo) {
            std::wcout << L"Change detected: " << fileInfo->FileName << std::endl;

            if (fileInfo->NextEntryOffset == 0) {
                break;
            }

            fileInfo = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(
                reinterpret_cast<BYTE*>(fileInfo) + fileInfo->NextEntryOffset
                );
        }
    }

    CloseHandle(hDir);
}

int main(int argc, char* argv[]) {
    QCoreApplication a(argc, argv);

    std::wcout << L"Folder Monitor - Select a folder:" << std::endl;

    // Use QFileDialog to open a folder dialog
    QString folderPath = QFileDialog::getExistingDirectory(nullptr, "Select Folder", "", QFileDialog::ShowDirsOnly);

    if (!folderPath.isEmpty()) {
        std::wcout << L"Selected folder: " << folderPath.toStdWString() << std::endl;

        // Create a flag to stop the monitoring thread
        std::atomic<bool> stopFlag(false);

        // Start monitoring the selected folder for changes in a separate thread
        std::thread monitoringThread([&folderPath, &stopFlag]() {
            watchFolder(folderPath.toStdWString(), stopFlag);
            });

        // Wait for user input to stop monitoring
        std::wcout << L"Press Enter to stop monitoring." << std::endl;
        std::cin.get();

        // Set the stop flag to stop the monitoring thread
        stopFlag.store(true);

        // Join the monitoring thread to wait for it to finish
        if (monitoringThread.joinable()) {
            monitoringThread.join();
        }
    }
    else {
        std::wcout << L"No folder selected." << std::endl;
    }

    return a.exec();
}

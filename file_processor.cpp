#include <iostream>
#include <cstring>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <cctype>
#include <string>
#include <vector>
#include <semaphore.h>

using namespace std;

// Global Variables
int badFilesCount = 0;
int directoriesCount = 0;
int regularFilesCount = 0;
int specialFilesCount = 0;
long long regularFileBytes = 0;
int textFilesCount = 0;
long long textFileBytes = 0;

sem_t mutex; // Semaphore for mutual exclusion

// Function to determine if a file is a text file
bool isTextFile(const string& fileName) {
    int fd = open(fileName.c_str(), O_RDONLY);
    if (fd < 0) {
        return false; // Unable to open the file
    } 

    char buffer[8192];
    ssize_t bytesRead;
    bool isText = true;

    while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
        for (ssize_t i = 0; i < bytesRead; ++i) {
            if (!isprint(buffer[i]) && !isspace(buffer[i])) {
                isText = false;
                break;
            }
        }
        if (!isText) break;
    }

    close(fd);
    return isText;
}

// Thread function to process a file
void* processFile(void* arg) {
    string fileName = *(string*)arg; // Cast argument to string
    delete (string*)arg;  // Free the allocated memory for the file name
    struct stat buf;

    // Use semaphore for mutual exclusion
    sem_wait(&mutex);

    if (stat(fileName.c_str(), &buf) == -1) {
        badFilesCount++;
    } else {
        if (S_ISREG(buf.st_mode)) {
            regularFilesCount++;
            regularFileBytes += buf.st_size;
            if (isTextFile(fileName)) {
                textFilesCount++;
                textFileBytes += buf.st_size;
            }
        } else if (S_ISDIR(buf.st_mode)) {
            directoriesCount++;
        } else {
            specialFilesCount++;
        }       
    }

    // Release the semaphore
    sem_post(&mutex);
    return nullptr;
}

int main(int argc, char* argv[]) {
    int maxThreads = 10; // Set a maximum cap of 10 threads
    vector<pthread_t> threads;
    sem_init(&mutex, 0, 1); // Initialize the semaphore

    if (argc > 1 && strcmp(argv[1], "thread") == 0) {
        if (argc > 2) {
            int threadsRequested = atoi(argv[2]);
            if (threadsRequested >= 1 && threadsRequested <= 10) {
                maxThreads = threadsRequested; // Limit to a max of 10
            } else {
                cout << "Invalid number of threads. Defaulting to 10." << endl;
                maxThreads = 10;
            }
        }
    }

    string fileName;
    while (getline(cin, fileName)) {
        
        while (threads.size() >= maxThreads) {
            pthread_t finishedThread;
            pthread_join(threads[0], nullptr); // Wait for the first thread to finish
            threads.erase(threads.begin()); // Remove it from the list
        }

        // Create a new copy of the file name for each thread
        string* fileNameCopy = new string(fileName);

        pthread_t thread;
        threads.push_back(thread);
        pthread_create(&threads.back(), nullptr, processFile, fileNameCopy); // Create a new thread with a copy of the file name
    }

    // Wait for all remaining threads to finish
    for (pthread_t thread : threads) {
        pthread_join(thread, nullptr);
    }

    // Print results
    cout << "Bad Files: " << badFilesCount << endl;
    cout << "Directories: " << directoriesCount << endl;
    cout << "Regular Files: " << regularFilesCount << endl;
    cout << "Special Files: " << specialFilesCount << endl;
    cout << "Regular File Bytes: " << regularFileBytes << endl;
    cout << "Text Files: " << textFilesCount << endl;
    cout << "Text File Bytes: " << textFileBytes << endl;

    sem_destroy(&mutex); // Clean up semaphore
    return 0;
}

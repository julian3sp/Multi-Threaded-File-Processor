# Multi-Threaded File Processor

## Overview

This C++ program processes file statistics using POSIX threads (pthreads) while ensuring mutual exclusion with semaphores. It classifies files into different categories, including regular files, text files, directories, and special files. The program dynamically manages a limited number of threads to efficiently handle multiple files concurrently.

## Features

- Multi-threaded file processing using `pthread`
- Semaphore-based synchronization to avoid race conditions
- Categorizes files as:
  - Bad files (cannot be opened or accessed)
  - Regular files (tracks total count and size in bytes)
  - Text files (subset of regular files, identified by printable characters)
  - Directories
  - Special files (non-regular, non-directory files)
- Supports a configurable number of threads (default: 10, max: 10)

## Compilation & Execution

### Compile the Program

```bash
g++ -pthread -o file_processor file_processor.cpp
```

### Run the Program

```bash
./file_processor [thread] [num_threads]
```

- `thread` (optional): Enables multi-threading mode.
- `num_threads` (optional): Specifies the number of threads (1-10). If invalid, defaults to 10.

### Example Usage

```bash
./file_processor thread 5 < file_list.txt
```

This processes files listed in `file_list.txt` using up to 5 threads.

## How It Works

1. Reads file names from standard input.
2. Creates a new thread (up to `maxThreads`) to process each file.
3. Uses `stat()` to determine file type and size.
4. If the file is regular, checks if it is a text file.
5. Uses a semaphore (`sem_t mutex`) to protect shared global counters.
6. Waits for all threads to complete before displaying results.

## Output Example

```
Bad Files: 3
Directories: 5
Regular Files: 20
Special Files: 2
Regular File Bytes: 150432
Text Files: 15
Text File Bytes: 104857
```

## Code Breakdown

### Key Components

- **`isTextFile()`**: Checks if a file contains only printable characters.
- **`processFile()`**: Processes a file, updates global counters, and uses `sem_wait` and `sem_post` for synchronization.
- **`main()`**:
  - Initializes semaphore.
  - Reads file names from input.
  - Spawns threads while managing a maximum limit.
  - Waits for all threads to finish.
  - Cleans up and prints results.

## Dependencies

- POSIX threads (`pthread`)
- POSIX semaphores (`semaphore.h`)
- Standard C++ libraries (`iostream`, `vector`, `string`, `fcntl.h`, `unistd.h`, etc.)

## Notes

- The program dynamically manages the number of active threads, ensuring efficiency without overwhelming system resources.
- Uses `pthread_join()` to synchronize thread execution and avoid resource leaks.
- Properly handles memory allocation and deallocation for file name storage.

## License

This project is open-source and available under the MIT License.

## Author

- **Julian Espinal**
- **[julianrespinal24@gmail.com](mailto\:julianrespinal24@gmail.com)**
- **[[https://github.com/julian3sp/](https://github.com/julian3sp/)]**

# CreateMutexes Script

The `CreateMutexes` script is a Windows application that creates multiple mutexes based on the provided list of mutex names. It allows you to substitute placeholders in the mutex names, such as `[ComputerName]`, with the actual name of the machine where the script is executed.

## Purpose

Mutexes (mutual exclusion objects) are synchronization primitives that can be used to control access to a shared resource among multiple processes or threads. This script is designed to automate the creation of mutexes with customizable names, providing a simple way to set up synchronization mechanisms in a Windows environment.

## Features

- Substitutes `[ComputerName]` placeholders with the name of the executing machine.
- Reads mutex names from a file for flexible configuration.
- Creates mutexes based on the provided names.
- Displays script information and execution details.
- Provides error handling and logging for a user-friendly experience.

## Getting Started

1. Download the `CreateMutexes.exe` binary from the [releases](https://github.com/wikijm/CreateMutexes/releases/) section.
2. Create a `mutex.list` file in the same directory as the script.
3. Add the desired mutex names to the `mutex.list` file, with placeholders like `[ComputerName]` if needed.
4. Run the `CreateMutexes.exe` script to create the specified mutexes.
5. Press Enter to release the mutexes when done.

## Usage

1. Clone or download the repository.
2. Compile the source code to generate the `CreateMutexes.exe` binary.
3. Create a `mutex.list` file in the same directory as the binary.
4. Add the desired mutex names to the `mutex.list` file, with placeholders like `[ComputerName]` if needed.
5. Run the `CreateMutexes.exe` script to create the specified mutexes.
6. Press Enter to release the mutexes when done.

## Example `mutex.list`

```plaintext
Global\MyMutex_[ComputerName]
Local\MyMutex2
Global\SyncMutex_[ComputerName]_2
Local\Mutex_[ComputerName]_3
```

If you're looking for Mutexes used by various malware families, you can refer to https://github.com/albertzsigovits/malware-mutex

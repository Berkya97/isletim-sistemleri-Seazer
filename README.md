# Caesar Cipher with Fork, Pipe, Thread and Shared Memory in C

This C program demonstrates an implementation of **Caesar cipher encryption** and decryption using **multi-processing**, **multi-threading**, and **shared memory** in a Linux environment. The code is structured to encrypt and decrypt a given message by:

- Sending data from **child process to parent process** using `pipe()`
- Splitting the received message into 4 parts
- Encrypting each part in separate **threads**
- Combining the encrypted results in **shared memory (`mmap`)**
- Decrypting the final encrypted string using Caesar cipher again

---

## 🔧 Technologies Used

- C (GCC)
- POSIX Threads (`pthread`)
- UNIX Pipes (`pipe`)
- Forked Processes (`fork`)
- Shared Memory (`mmap`)
- Caesar Cipher Algorithm

---

## 📜 Description

The program starts by forking a child process. The child writes a hardcoded Atatürk quote into a pipe. The parent process reads this string, then divides it into four equal parts and creates one thread for each part to encrypt it using Caesar cipher with a fixed key (`key = 4`).

Each encrypted part is stored into a **shared memory region**. After all parts are encrypted, the parent process concatenates them and prints:

- The total number of characters
- The encrypted message
- The decrypted version (original message)

---



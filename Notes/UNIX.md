# UNIX Lecture Notes

## What is UNIX?
UNIX is a **multi-user, multi-tasking** and **machine-indepenent** operating system.  There are many flavours of UNIX in use today including Mac OS X.

## Why use UNIX?
* Multiuser
* Multitasking
* Remote processing
* Stable
* Highly modular
* Some versions are free
* Large user community

## The UNIX Model
Highest level: User <br>
Lowest level: UNIX Kernel and Device Drivers <br>
The **shell** acts as an interpreter, or bridge, between the user and the kernel and has responsibilities including:
* Reading and interpreting user commands at the console
* Implementing job control

The **kernel** is the core of the operating system and is itself a large and complex program.  There is a distinct boundary between a user and the kernel.  Specifically,
* To access a computer's hardware, the user's commands must go through teh kernel
* The user must request the kernel to perform work
Its main resposibilities are;
* Memory allocation
* The file system
* Loading and executing programs
* Communicating with devices, etc.



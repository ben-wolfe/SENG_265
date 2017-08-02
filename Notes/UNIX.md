# UNIX Lecture Notes

[This is the link text](#name)

## What is UNIX?
UNIX is a **multi-user, multi-tasking** and **machine-independent** operating system.  There are many flavours of UNIX in use today including Mac OS X.

## Why use UNIX?
* Multiuser
* Multitasking
* Remote processing
* Stable
* Highly modular
* Some versions are free
* Large user community

## The UNIX Model
![Image of the UNIX Model](./Images/UNIX_model.png)
The **shell** acts as an interpreter, or bridge, between the user and the kernel.  Its responsibilities include 
* Reading and interpreting user commands at the console
* Implementing job control

The **kernel** is the core of the operating system and is itself a large and complex program.  There is a clear demarcation between a user and the kernel.  Specifically,
* To access a computer's hardware, the user's commands must go through the kernel
* The user must request the kernel to perform work

The kernel's main resposibilities are:
* Memory allocation
* The file system
* Loading and executing programs
* Communicating with devices, etc.

## The UNIX Filesystem
*File* and *filesystem* are **key abstractions** of the UNIX computing model.  Practically anything can be abstracted as a file including devices, programs, data, memory, etc.  In summary, the filesystem creates the relationship between *abstracting blocks* of information on the **physical** storage device and **logical** blocks that the user can manipulate.


### Directories
The file system is arranged as a hierachy of a collection of **directories**.  A directory can be thought of as a folder, however is just a normal file.  The only difference is that they have a special format.  Every directory contains two special directory entries. **.** and **..** refer to the current directory and parent directory respectively.

Note that
```shell
~
``` 
is used to access the **home directory** (the home directory is usually made to be the working directory upon logging into the system).  In other words,
```shell
cd /home/user
```
is equivalent to
```shell
~user
```

## HEADING<a name="name"></a>

# UNIX Lecture Notes

[What is UNIX?](#what-is-unix)<br>
[Why use UNIX?](#why-use-unix)<br>
[The UNIX Model](#the-unix-model)<br>
[The UNIX Filesystem](#the-unix-filesystem)<br>
[Directories](#directories)<br>
[Commands](#commands)<br>

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
*File* and *filesystem* are **key abstractions** of the UNIX computing model.  Practically anything can be abstracted as a file including devices, programs, data, memory, etc.  In summary, the filesystem establishes the relationship between *abstracting blocks* of information on the **physical** storage device and **logical** blocks that the user can manipulate.

Every plain file and directory has a set of attributes including
* User name (owner of file)
* Group name (for sharing)
* File size (bytes)
* Creation time, modification time
* File type (file, directory, device link)
* Permissions

**File permissions** are specified for the **user (u)**, **group (g)**, **other (o)**, and **all (a)**.<br>
For **files**:
* Read (r): Allows the file to be read
* Write (w): Allows the file to be modified (edit, delete)
* Execute (x): Tells UNIX the file is executable
* Dash (-): Owner/group/other have no permissions

For **directories**:
* Read (r): Allws directory contents to be read (listed)
* Write (w): Allows directory contents to be modified (create, delete)
* Execute (x): Allows users to navigate into that directory
* Dash (-): Owner/group/other have no permissions

### Directories
The file system is arranged as a hierachy of a collection of **directories**.  A directory can be thought of as a folder, however is just a normal file.  The only difference is that they have a special format.  Every directory contains two special directory entries, '.' and '..', which refer to the current directory and parent directory respectively.

## Commands
Accessing the **home** directory (the home directory is usually made to be the *working directory* upon logging into the system).  The working directory is the directory you are in at any given time.

```shell
~
``` 

** OR **

```shell
cd /home/user
```

** OR **
```shell
~user


```

Opening files:
```shell
open <filename>
```

Creating a **symbolic reference**
```shell
ln -s <target> <name of alias>
```

Changing file permissions using **chmod**
```shell
chmod o+rx ~
```

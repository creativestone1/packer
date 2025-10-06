# packer

Simple archiver program for packing and unpacking files or directories.

## Overview

This project provides a command-line tool `archiver` that can create archives from files or directories and extract archives back to files or directories.

## Usage
```shell
archiver [OPTION] [FILE]
```


### Options:

- `-p <fileIn> [<pathOut>]`  
  Pack (archive) the specified file or directory.  
  - `<fileIn>` is the path to the file or directory to pack.  
  - `[<pathOut>]` is optional output path for the archive.

- `-u <fileIn> [<pathOut>]`  
  Unpack (extract) the specified archive.  
  - `<fileIn>` is the path to the archive file.  
  - `[<pathOut>]` is optional output directory for the extracted contents.

- `-h`  
  Display help message and exit.

## Examples

```shell
archiver -p myfolder
```
Pack the folder `myfolder` in the current directory.

```shell
archiver -p path/myfolder pathOut
```
Pack the folder `myfolder` in the current directory and output archive to `pathOut`.

```shell
archiver -u archive.arc
```
Unpack `archive.arc` archive in the current directory.

```shell
archiver -h
```
Show the help message.

## Files in the project

- `main.c` — Main entry point and argument parsing.
- `packer.h` — Header defining pack and unpack functions.
- `additional.h` — Additional helper headers (assumed).
- `LICENSE` — License file.
- `README.md` — This file.

## License

This project is licensed under the MIT License. See the LICENSE file for details.


# packer

Simple packer program for packing and unpacking files or directories.

## Overview

This project provides a command-line tool `packer` that can create archives from files or directories and extract archives back to files or directories.

## Build

```shell
gcc main.c -opacker
```

## Usage
```shell
packer [OPTION] [FILE]
```


### Options:

- `-p <fileIn> [<pathOut>]`  
  Pack (pack) the specified file or directory.  
  - `<fileIn>` is the path to the file or directory to pack.  
  - `[<pathOut>]` is optional output path for the pack.

- `-u <fileIn> [<pathOut>]`  
  Unpack (extract) the specified pack.  
  - `<fileIn>` is the path to the pack file.  
  - `[<pathOut>]` is optional output directory for the extracted contents.

- `-h`  
  Display help message and exit.

## Examples

```shell
packer -p myfolder
```
Pack the folder `myfolder` in the current directory.

```shell
packer -p path/myfolder pathOut
```
Pack the folder `myfolder` in the current directory and output pack to `pathOut`.

```shell
packer -u pack.arc
```
Unpack `pack.arc` pack in the current directory.

```shell
packer -h
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


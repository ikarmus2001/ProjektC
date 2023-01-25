#!/bin/sh

gcc -g main.c windowPrepare.h windowPrepare.c databaseOperations.h databaseOperations.c FileOperations/fileOperations.h FileOperations/fileOperations.c Structures/structures.h Structures/structures.c ViewController.h ViewController.c -o main  `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` -D DEBUG
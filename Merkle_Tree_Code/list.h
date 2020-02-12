/*
list.h
Sam Blair, Oyedola Ajao, Sam Goering, and Easton Tuttle
Computer Science II
Homework 5 - Merkle Votes
November 27, 2017

The interface of the list class.
*/

#ifndef LIST_H
#define LIST_H
#define NULL 0

#include "stdafx.h"
#include "treeNode.h"
#include <iostream>

using std::cout;

class List
{
private:
	treeNode *head;
	treeNode *current;
	treeNode *tail;

public:
	// Allocates a default list.
	List();
	// Deletes the list from memory.
	~List();
	// Prints the contents of the list.
	void printList();
	// Empties the list.
	void makeEmpty();
	// If the list is empty, return true, else return false.
	bool isEmpty();
	// Inserts an object into a defined position.
	void insert(string newData);
	// Removes an object from the list.
	void remove(string comparison);
	// Removes the first element added to the list.
	treeNode removeFirst();
	// Removes the last element added to the list.
	treeNode removeLast();
};

#endif

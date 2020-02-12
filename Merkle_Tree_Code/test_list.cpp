/*
test_list.cpp
Sam Blair, Oyedola Ajao, and Easton Tuttle
Computer Science II
Homework 5 - Merkle Votes
November 8, 2017

The testing program for the list data structure.
*/

#include "stdafx.h"
#include "list.h"

//int main()
//{
//	List newList = List();
//	int index = 0;
//
//	// Print the empty list.
//	newList.printList();
//
//	// Check if the list is empty.
//	if (newList.isEmpty())
//	{
//		cout << "The list is currently empty.\n";
//	}
//	else
//	{
//		cout << "The list is not empty.\n";
//	}
//
//	// Insert 5 elements into the list and print the list.
//	newList.insert("hello");
//	newList.insert("goodbye");
//	newList.insert("foo");
//	newList.insert("burger");
//	newList.insert("Tokyo");
//	newList.printList();
//
//	// Remove the first element of the list and print the list.
//	newList.remove("hello");
//	newList.printList();
//
//	// Remove an element from the middle of the list.
//	newList.remove("goodbye");
//	newList.printList();
//
//	// Remove an element from the end of the list.
//	newList.insert("Donkey");
//	newList.printList();
//	newList.remove("Donkey");
//	newList.printList();
//
//	// Check if the list is empty.
//	if (newList.isEmpty())
//	{
//		cout << "The list is currently empty.\n";
//	}
//	else
//	{
//		cout << "The list is not empty.\n";
//	}
//
//	// Attempt to remove an element that is not in the list.
//	newList.remove("Japan");
//	newList.printList();
//
//	// Remove the last element of the list.
//	newList.removeLast();
//	newList.printList();
//
//	// Remove the first element of the list.
//	newList.removeFirst();
//	newList.printList();
//
//	// Insert 3 elements into the list and print the list.
//	newList.insert("a");
//	newList.insert("b");
//	newList.insert("c");
//	newList.printList();
//	
//	// Make the list empty and then print the empty list.
//	newList.makeEmpty();
//	newList.printList();
//}
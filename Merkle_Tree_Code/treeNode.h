/*
treeNode.h
Sam Blair, Oyedola Ajao, and Easton Tuttle
Computer Science II
Homework 5 - Merkle Votes
November 8, 2017

The treeNode structure.
*/

#pragma once
#ifndef TREENODE_H
#define TREENODE_H
#define NULL 0

#include <string>

using std::string;

// Nodes of the tree which hold a pseudo-time stamp (integer, non-unique,)
// a 128-bit data component (string of 32 characters,)
// have 2 pointers to type node, and be differentiable
// if a leaf or not.
struct treeNode
{
	int time;
	string data;
	treeNode *left;   //changed from nextNode -> left
	treeNode *right;  //changed from prevNode -> right
	bool isLeaf = false;

	// Default constructor which sets all values to a "zero or null" state.
	treeNode()
	{
		time = 0;
		data = "";
		left = NULL;
		right = NULL;
		isLeaf = false;
	}

	// Constructor which sets the data to the paramater passed in.
	treeNode(string newData)
	{
		time = 0;
		data = newData;
		left = NULL;
		right = NULL;
		isLeaf = false;
	}
};

#endif
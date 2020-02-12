/*
bTREE.cpp
Sam Blair, Oyedola Ajao, Sam Goering, and Easton Tuttle
Computer Science II
Homework 5 - Merkle Votes
November 8, 2017

Implementations of the bTREE class.
*/

#include <iostream>

#include "bTREE.h"

using namespace std;

//look at descriptions in pMT.h for guidance on what you might need for these function to actually do

bTREE::bTREE()

{
	tree = List();

	count = 0;
}



bTREE::~bTREE()

{

}



int bTREE::dataInserted()

{
	return 0;
}



int bTREE::numberOfNodes()

{
	return count;
}



bool bTREE::insert(string data, int time)

{
	return insert(tree, data, time);
}



bool bTREE::find(string input)

{
	return find(tree, input);
}



string bTREE::locate(string input)

{
	if (find(input))
	{
		return locate(tree, input, "\n");
	}
	else
	{
		return "-";
	}

	return "\n";
}

bool bTREE::insert(treeNode *tmp, string data, int time)
{
	if (tmp == NULL)

	{

		tmp->data = data;

		tmp->time = time;

		count++;

		return true;

	}

	else if (tmp->left == NULL)

	{

		insert(tmp->left, data, time);

	}

	else if (tmp->left->left == NULL)

	{

		insert(tmp->left->left, data, time);

	}

	else if (tmp->left->right == NULL)

	{

		insert(tmp->left->right, data, time);

	}

	else if (tmp->right == NULL)

	{

		insert(tmp->right, data, time);

	}

	else if (tmp->right->left == NULL)

	{

		insert(tmp->right->left, data, time);

	}

	else if (tmp->right->right == NULL)

	{

		insert(tmp->right->right, data, time);

	}

	return false;
}

bool bTREE::find(treeNode *tmp, string input)
{
	if (tmp->data == input)

	{

		return true;
	}

	if (tmp->left != NULL)

	{

		find(tmp->left, input);

	}

	if (tmp->right != NULL)

	{

		find(tmp->right, input);

	}

	return false;
}

string bTREE::locate(treeNode *tmp, string input, string dest)
{
	if (tmp->right != NULL)

	{

		locate(tmp->right, input, dest + "R");

	}

	if (tmp->left != NULL)

	{

		locate(tmp->left, input, dest + "L");

	}

	if (tmp->data == input)

	{

		return dest;

	}

	return dest;
}


bool operator==(const bTREE & lhs, const bTREE & rhs)
{
	if (lhs.tree->data == rhs.tree->data && lhs.tree->time == rhs.tree->time)
	{
		return true;
	}

	return false;
}

bool operator!=(const bTREE & lhs, const bTREE & rhs)
{
	if (lhs.tree->data != rhs.tree->data && lhs.tree->time != rhs.tree->time)
	{
		return true;
	}

	return false;
}

std::ostream & operator<<(std::ostream & out, const bTREE & p)
{
	cout << "Time : " << p.tree->time << "  Data : " << p.tree->data;

	// TODO: insert return statement here
	return out;
}

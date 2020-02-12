/*
list.cpp
Sam Blair, Oyedola Ajao, Sam Goering, and Easton Tuttle
Computer Science II
Homework 5 - Merkle Votes
November 27, 2017

The implementations of the list class.
*/

#include "list.h"

List::List()
{
	head = new treeNode();
	current = head;
	tail = head;
}


List::~List()
{

}


void List::printList()
{
	current = head->left;
	if (current == NULL)
	{
		cout << "The list currently contains: \n";
	}
	else
	{
		cout << "The list currently contains: ";
		while (current->left != NULL)
		{
			cout << current->data << ", ";
			current = current->left;
		}
		cout << current->data << "\n";
	}
}

void List::makeEmpty()
{
	head->left = NULL;
}

bool List::isEmpty()
{
	if (head->left == NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}


void List::insert(string newData)
{
	treeNode *newCell = new treeNode(newData);
	tail->left = newCell;
	newCell->right = tail;
	tail = newCell;
}

void List::remove(string comparison)
{
	if (head != NULL)
	{
		current = head;
		while (current->left != NULL)
		{
			if (current->data == comparison)
			{
				if (current->right == NULL)
				{
					head = current->left;
				}
				else
				{
					treeNode *temp;
					temp = current->left;
					current = current->right;
					current->left = temp;
					temp->right = current;
				}
			}
			current = current->left;
		}
		if (tail->data == comparison)
		{
			tail = tail->right;
			tail->left = NULL;
		}
	}
	else
	{
		return;
	}
}

treeNode List::removeFirst()
{
	treeNode *tempCell;
	tempCell = head;
	head = head->left;
	head->right = NULL;
	return *tempCell;
}

treeNode List::removeLast()
{
	treeNode *tempCell;
	tempCell = tail;
	tail = tail->right;
	tail->left = NULL;
	return *tempCell;
}
/*
pMT.h
Sam Blair, Oyedola Ajao, Sam Goering, and Easton Tuttle
Computer Science II
Homework 5 - Merkle Votes
November 8, 2017

Interfaces of the pMT class.
*/

#pragma once

#include "stdafx.h"
#include "bTREE.h"
#include <string>
using namespace std;
class pMT
{
private:
    int selectedHash;
    bTREE myMerkle;
    
    string hash_SamB(string);
    string hash_SamG(string);
    string hash_Dola(string);
	string hash_Easton(string);
    
public:
    pMT(int);
    ~pMT();
    
    
    int insert(string, int);
    
    int find(string);
	int find(string vote, int time, int hashSelect);
    int findHash(string);
    
    string locate(string);
	string locateData(string vote);
	string locateHash(string mhash);

    friend bool operator==(const pMT& lhs, const pMT& rhs);
    
    friend bool operator!=(const pMT& lhs, const pMT& rhs);
    
    friend pMT operator^(const pMT& lhs, const pMT& rhs);
    friend std::ostream& operator<<(std::ostream& out, const pMT& p);

	friend bool operator ==(const pMT& lhs, const pMT& rhs)
		/**
		* @brief Comparison between two merkle trees
		* @param lhs, the left hand side of the equality statment
		* @param rhs, the right hand side of the equality statement
		* @return true if equal, false otherwise
		*/
	{
	}

	friend bool operator !=(const pMT& lhs, const pMT& rhs)
		/**
		* @brief Comparison between two merkle trees
		* @param lhs, the left hand side of the equality statment
		* @param rhs, the right hand side of the equality statement
		* @return true if not equal, false otherwise
		*/
	{

	}

	friend pMT operator ^=(const pMT& lhs, const pMT& rhs)
		/**
		* @brief XOR between two merkle trees
		* @param lhs, the left hand side of the equality statment
		* @param rhs, the right hand side of the equality statement
		* @return true if not equal, false otherwise
		*/
	{

	}


	friend std::ostream& operator <<(std::ostream& out, const pMT& p)
		/**
		* @brief Print out a tree
		* @param out
		* @param p
		* @return a tree to the screen
		*/
	{
	}


	friend pMT operator ^(const pMT& lhs, const pMT& rhs)
		/**
		* @brief Where do two trees differ
		* @param lhs
		* @param rhs
		* @return a tree comprised of the right hand side tree nodes that are different from the left
		*/
	{
	}
    
};
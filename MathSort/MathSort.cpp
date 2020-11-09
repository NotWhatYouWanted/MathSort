// MathSort.cpp : First 
//

#include "pch.h"
#include <iostream>

#include <string>
#include <vector>
#include <fstream>


std::vector<int> GetFromFile();

std::vector<int> MathSort(std::vector<int> Indices);

std::vector<std::pair<int, int>> Tokenise(std::vector<int> Indices);

template <typename T>
std::vector<int> EncodeAsIndices(std::vector<T> Data, std::vector<std::pair<T,int>> Tokens);
template <typename T>
std::vector<int> CalcOffsets(std::vector<std::pair<T, int>> Tokens);

int main()
{
	std::vector<int> Indices = { 50, 20, 20, 60, 50, 20, 10};
	Indices = GetFromFile();

	//for (unsigned int i = 0; i < Indices.size(); i++)
	//	std::cout << Indices[i] << std::endl;

	std::vector<int> Sorted = MathSort(Indices);
	for (unsigned int i = 0; i < Sorted.size(); i++)
		std::cout << Sorted[i] << std::endl;
}

std::vector<int> GetFromFile()
{
	std::string line;
	std::ifstream file("manufacturer20.idx");

	std::vector<int> Output;

	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			Output.push_back(stoi(line));
			//cout << line << '\n';
		}
		file.close();
	}

	return Output;
}

std::vector<int> MathSort(std::vector<int> Indices)
{
	//Array of value / frequency pairs. Sorted by val
	std::vector<std::pair<int, int>> Tokens = Tokenise(Indices);

	//Encode original data as indices of the token map
	std::vector<int> CodedIndices = EncodeAsIndices(Indices, Tokens);

	//Calc culminative freqs
	std::vector<int> Offsets = CalcOffsets(Tokens);

	std::vector<int> Sorted(Indices.size(), 0);
	for (unsigned int i = 0; i < Indices.size(); ++i)
	{
		Sorted[ Offsets[CodedIndices[i]]++ ] = i;
	}

	return Sorted;
}

std::vector<std::pair<int, int>> Tokenise(std::vector<int> Indices)
{	
	//Use an indexed vector instead of a map as we need indices in math sort
	std::vector<std::pair<int, int>> Out;

	//TODO Dont like this default INT16 max system..
	int Low = INT16_MAX;
	int PrevLow = Low;

	//For every element in the input
	for (unsigned int i = 0; i < Indices.size(); i++)
	{
		Low = INT16_MAX;
		for (unsigned int j = 0; j < Indices.size(); j++)
		{
			//Find lowest element not yet inserted
			if (
				Indices[j] < Low && 
				(Out.size() < 1 || Indices[j] > (--Out.end())->first) 
			)
				Low = Indices[j];
		}
		if (Low == PrevLow || Low == INT16_MAX)
			break;
		else
		{
			Out.push_back(std::pair<int, int>(Low, 0));
			PrevLow = Low;
		}
	}

	//TODO put this inside the above loop..
		//Loop size + 1 times. 
		//Compare digits checked for lowest with Out.end() for counting too.
	//Loop output again and take freq counts
	for (unsigned int i = 0; i < Out.size(); i++)
	{
		//TODO Think we can calculate the offsets here? 
			//Offset array size of out - 1.
			//Increment Offsets[i+1] when incrementing token[i]

		for (unsigned int j = 0; j < Indices.size(); j++)
		{
			if (Out[i].first == Indices[j])
				++Out[i].second;
		}
	}

	return Out;
}

//Encodes data vector as indices of its Token vector
template <typename T>
std::vector<int> EncodeAsIndices(std::vector<T> Data, std::vector<std::pair<T,int>> Tokens)
{
	std::vector<int> CodedIndices(Data.size(), 0);

	for (unsigned int i = 0; i < Data.size(); i++)
	{
		for (unsigned int j = 0; j < Tokens.size(); j++)
		{
			if (Tokens[j].first == Data[i])
			{
				CodedIndices[i] = j;
				break;
			}
		}
	}

	return CodedIndices;
}

//Calculates culmulative frequencies of tokenised data
template <typename T>
std::vector<int> CalcOffsets(std::vector<std::pair<T, int>> Tokens)
{
	std::vector<int> Offsets;
	Offsets.push_back(0);
	int ind = 0;

	for (auto it = Tokens.begin(); it != --Tokens.end(); ++it)
	{
		Offsets.push_back(Offsets[ind] + it->second);
		++ind;
	}

	return Offsets;
}

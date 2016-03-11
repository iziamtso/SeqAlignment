#pragma once
#include <string>
#include <vector>
#include "ScoringMatrix.h"
#include "Alignment.h"


using namespace std;

struct record {
	std::string read;
	double score;
	std::string methodUsed;

	//Describes deletion
	int del_index;

	//Describes substitution
	int sub_index;
	char sub_letter;

	//Describes insertion
	int ins_index;
	char ins_letter;

	record::record() {
		read = "";
		score = 0.0;
		del_index = -1;
		sub_index = -1;
		sub_letter = '*';
		ins_index = -1;
		ins_letter = '*';
	}
};

class Worker {
	ScoringMatrix *scoreMat;
	vector<string> reads;
	//Alignment align;
	int filePos;

	void readFasta(vector<string>& reads, string path);
	string readFastaSpecial(vector<string>& reads, string path);
	vector<string> split(const string& str, char delim);
	vector<string>& split(const string& str, char delim, vector<string>& e);

public:
	Worker(string scoreMatPath);
	void runOneToAll(std::string candidate, record& rec);
	void run(string dataPath);
	void run(int start, int stop, string dataPath);
	~Worker();
};


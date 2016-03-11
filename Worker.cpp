#include "Worker.h"

Worker::Worker(string scoreMatPath){
	//Parse scoring matrix
	scoreMat = new ScoringMatrix(5, 5);
	scoreMat->loadMatrix(scoreMatPath);
	filePos = 0;
}

void Worker::run(string dataPath) {
	//Parse Fasta file (DNA)
	readFasta(reads, dataPath);

	//Find the candidate
	std::string candidate = reads[0];
	record rec;
	std::string prev_candidate = "";

	
	int count = 0; //Delete this later
	while(candidate.compare(prev_candidate)) {
		std::cout << "Iteration: " << count <<"-----------------------------------------" << std::endl;

		prev_candidate = candidate;
		runOneToAll(candidate, rec);
		candidate = rec.read;

		count++;
		std::cout << "-----------------------------------------" << std::endl;
	}

	//Record the rec
}

void Worker::run(int start, int stop, string dataPath) {
	while (start < stop) {
		//Parse Fasta file (DNA)
		//Find the candidate
		std::string candidate = readFastaSpecial(reads, dataPath);

		record rec;
		std::string prev_candidate = "";

		while (candidate.compare(prev_candidate)) {
			prev_candidate = candidate;
			runOneToAll(candidate, rec);
			candidate = rec.read;
		}

		//Record the rec
		//Ad hoc just writting it down
		//std::cout <<"Bubble: " <<start <<" final str: " << rec.read <<" score: "<< rec.score << std::endl;
		start++;
	}
}

void Worker::runOneToAll(std::string candidate, record& rec) {
	double score = 0;
	Alignment align;
	//Global
	for each (string read in reads) {
		score += align.globalAlignment(candidate, read, scoreMat);
	}
	rec.methodUsed = "global";
	rec.score = score;
	rec.read = candidate;

	//Test
	//std::cout << "Global: " << candidate << " score: " << score << std::endl;

	//Deletion
	for (int del_index = 0; del_index < candidate.size(); del_index++) {
		score = 0;
		for (int i = 0; i < reads.size(); i++) {
			score += align.addDeletion(i, del_index + 1);
		}

		//Test-------------------------------------------------
		//std::string strI = candidate;
		//std::cout << "Deletion: " << strI.erase(del_index, 1) << " score: " << score << std::endl;
		//Test-------------------------------------------------

		//Record if less
		if (score > rec.score) {
			std::string str = candidate;
			rec.methodUsed = "deletion";
			rec.score = score;
			rec.read = str.erase(del_index, 1);
			rec.del_index = del_index;
		}
	}

	//Substitution
	char alphabet[4] = { 'A', 'C', 'G', 'T' };
	for (int sub_index = 0; sub_index < candidate.size(); sub_index++) {
		for each (char letter in alphabet) {

			if (letter == toupper(candidate[sub_index])) {
				continue;
			}

			score = 0;
			for (int i = 0; i < reads.size(); i++) {
				score += align.addSubstitution(i, sub_index + 1, letter, reads[i], scoreMat);
			}

			//Test-------------------------------------------------
			//std::string strI = candidate;
			//strI.erase(sub_index, 1);
			//std::cout << "Substitution: " << strI.insert(sub_index, 1, letter) << " score: " << score << std::endl;
			//Test-------------------------------------------------

			//Record if less
			if (score > rec.score) {
				std::string str = candidate;
				rec.methodUsed = "substitution";
				rec.score = score;
				str.erase(sub_index, 1);
				rec.read = str.insert(sub_index, 1, letter);
				rec.sub_index = sub_index;
				rec.sub_letter = letter;
			}
		}
	}

	//Insertion
	for (int ins_index = 0; ins_index < candidate.size()+1; ins_index++) {
		for each (char letter in alphabet) {
			score = 0;
			for (int i = 0; i < reads.size(); i++) {
				score += align.addInsertion(i, ins_index + 1, letter, reads[i], scoreMat);		
			}

			//Test-------------------------------------------------
			//std::string strI = candidate;
			//std::cout << "Insertion: " << strI.insert(ins_index, 1, letter) << " score: " << score << std::endl;
			//Test-------------------------------------------------


			//Record if less
			if (score > rec.score) {
				std::string str = candidate;
				rec.methodUsed = "insertion";
				rec.score = score;
				rec.read = str.insert(ins_index, 1, letter);
				rec.ins_index = ins_index;
				rec.ins_letter = letter;
			}
		}
	}	
}

void Worker::readFasta(vector<string>& reads, string path) {
	std::string line;
	std::ifstream file(path);

	if (file.is_open()) {
		while (getline(file, line)) {
			if (line[0] == '>' || line == "") {
				continue;
			}
			reads.push_back(line);
		}
	}
}

string Worker::readFastaSpecial(vector<string>& reads, string path) {
	std::string line;
	std::ifstream file(path, std::ios::binary);
	string candidate; 

	if (file.is_open()) {
		//Get to the current bubble
		file.seekg(filePos, file.beg);

		getline(file, line);
		vector<string> elems = split(line, ' ');
		
		string marker = elems[0];
		int index = stoi(elems[1]);
		int numOfReads = stoi(elems[2]);
		int sizeOfCand = stoi(elems[3]);

		if (marker.compare(">current") == 0) {
			getline(file, candidate);
		}

		//Get all appropriate string for the current bubble
		int count = 0;
		reads.clear();
		while (getline(file, line) && count < numOfReads) {
			if (line[0] == '>' || line == "") {
				continue;
			}
			reads.push_back(line);
			count++;
			filePos = file.tellg();
		}
	}
	file.close();
	return(candidate);
}

std::vector<std::string>& Worker::split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> Worker::split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

Worker::~Worker() {
	delete scoreMat;
}

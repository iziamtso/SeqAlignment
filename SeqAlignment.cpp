// SeqAlignment.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Worker.h"
#include <ctime>
#include <iostream>


int main(int argc, char* argv[]) {
	std::clock_t start;
	start = std::clock();

	Worker worker("../input/alignment_probs_newchem_20k_noins.txt");
	//worker.run("../input/nhood.2.fasta");
	//worker.run("../input/pat/Zscan20_ME_top10.txt");
	worker.run(0, 1000, "../input/reads.20k.fasta.15.8.24.v0.fasta.all.bubble1.fasta");
	

	std::cout << "Time: " << (std::clock() - start) / (double)(CLOCKS_PER_SEC / 1000) << " ms" << std::endl;
	cin.get();
	return 0;
}


#include <fstream>
#include <cstring>
#include <string>
#include <iostream>
#include <vector>



using namespace std;


//~ static unsigned int seed;

inline uint32_t xs ( uint32_t x ) {
	x = ( ( x >> 16 ) ^ x ) * 0x2c1b3c6d;
	x = ( ( x >> 16 ) ^ x ) * 0x297a2d39;
	x = ( ( x >> 16 ) ^ x );
	return x;
}



char randNucle(char c='N'){
	return 'N';
	switch (rand()%4){
	//~ seed=xs(seed)%4
	//~ switch (seed){
		case 0:
			if(c!='A'){
				return 'A';
			}
			return randNucle(c);
		case 1:
			if(c!='C'){
				return 'C';
			}
			return randNucle(c);
		case 2:
			if(c!='G'){
				return 'G';
			}
			return randNucle(c);
		case 3:
			if(c!='T'){
				return 'T';
			}
			return randNucle(c);
	}
	return randNucle(c);
}



char randNucle_lower(char c='N'){
	switch (rand()%4){
	//~ seed=xs(seed)%4
	//~ switch (seed){
		case 0:
			if(c!='a'){
				return 'a';
			}
			return randNucle_lower(c);
		case 1:
			if(c!='c'){
				return 'c';
			}
			return randNucle_lower(c);
		case 2:
			if(c!='g'){
				return 'g';
			}
			return randNucle_lower(c);
		case 3:
			if(c!='t'){
				return 't';
			}
			return randNucle_lower(c);
	}
	return randNucle(c);
}



char randNucle_respect_cases(char c='N'){
	if(c=='a' or c=='c' or c=='g' or  c=='a'){
		return randNucle_lower(c);
	}else{
		return randNucle(c);
	}
}




void insertion(double rate, string& result){
	uint dice(rand() % 100);
	if(dice < rate){
		char newNucleotide(randNucle());
		result.push_back(newNucleotide);
		insertion(rate, result);
	}
}


//~ string mutateSequence(const string& referenceSequence,uint mutRate, vector <double> ratioMutation={0.06,0.73,0.21}){
string mutateSequence(const string& referenceSequence,uint mutRate, vector <double> ratioMutation={0.37,0.09,0.54}){//NANOPORE
	string result;
	result.reserve(5 * referenceSequence.size());
	for(uint i(0); i < referenceSequence.size(); ++i){
		double substitutionRate(mutRate * ratioMutation[0]);
		double insertionRate(mutRate * ratioMutation[1]);
		double deletionRate(mutRate * ratioMutation[2]);
		uint dice(rand() % 100);

		if (dice <substitutionRate ){
			//SUBSTITUTION
			char newNucleotide(randNucle());
			while(newNucleotide == referenceSequence[i]){
				newNucleotide = randNucle();
			}
			result.push_back(newNucleotide);
			continue;
		} else if(dice < deletionRate+substitutionRate){
			//DELETION
			uint dice2(rand() % 100);
			while (dice2 < deletionRate+substitutionRate){ // deletions larger than 1
				++i;
				dice2 = rand() % 100;
			}
			continue;
		} else if (dice < deletionRate + substitutionRate + insertionRate){
			//INSERTION
			char newNucleotide(randNucle());
			result.push_back(referenceSequence[i]);
			result.push_back(newNucleotide);
			insertion(deletionRate + substitutionRate + insertionRate, result); // larger than 1 insertions

			continue;
		} else {
			result.push_back(referenceSequence[i]);
		}

	}
	return result;
}




int main(int argc, char ** argv){
	if(argc<4){
		cout<<"[read file] [error rate] [prefix]"<<endl;
		exit(0);
	}
	string input(argv[1]);
	srand (time(NULL));
	ifstream in(input);
	uint errorRate((stof(argv[2]))*10000);
	string prefix(argv[3]);
	string useless, ref,read,pread;
	uint indice_header(0);
	ofstream out(prefix+".fa");
	while(not in.eof()){
		getline(in,useless);
		getline(in,read);
		if(not useless.empty()){
			if(not read.empty()){
				for(uint i(0);i<read.size();++i){
					if((uint32_t)rand()%10000<=errorRate){
						read[i]=randNucle_respect_cases(read[i]);
					}
				}
				out<<useless<<"\n";
				indice_header++;
				out<<read<<"\n";
			}else{
				out<<useless<<"\n";
				indice_header++;
				out<<read<<"\n";
			}
		}
	}
}


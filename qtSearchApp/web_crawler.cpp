#include <sstream>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <ctype.h>
#include <utility>
#include <deque>
#include "Set.h"
#include <fstream>

using namespace std;

void webCrawler( string seedFile, string output );
void dfs(string &currFile, Set<string> &crawledPages, deque<string> &DFSqueue, string & outputString);

int main(int argc, char* argv[]){
	string seedFile = argv[1];
	string outputFile = argv[2];

	webCrawler(seedFile, outputFile);	

	return 0;
}

void webCrawler( string seedFile, string output ){

	ofstream ofile(output);

	deque<string> DFSqueue;

	Set<string> crawledPages;

	string outputString = "";

	ifstream ifile(seedFile);

	if (ifile.fail()){
		cout << "Error reading in " << seedFile << endl;
		ifile.clear();
		return;
	}	

	string line; 

	while (getline(ifile, line)){
		DFSqueue.push_back(line);
	}

	while(!DFSqueue.empty()){
		string currFile = DFSqueue.front();
		//cout << "entering dfs with currFile = to " << currFile << endl;
		// If current file has not already been found, implement DFS on it
		if( crawledPages.find(currFile) == crawledPages.end() ){
			dfs(currFile, crawledPages, DFSqueue, outputString);
			DFSqueue.pop_front();
		}
		else{
			DFSqueue.pop_front();	
		}
	}

	ofile << outputString;
	ofile.close();

}

void dfs(string &currFile, Set<string> &crawledPages, deque<string> &DFSqueue, string & outputString){
		//cout << "	entering dfs function with currFile = to " << currFile << endl;
		ifstream ifile(currFile);

		if (ifile.fail()){
			cout << "Error reading in " << currFile << endl;
			ifile.clear();
			return;
		}

		// Checks to see if current page has not been found
		if( crawledPages.find(currFile) == crawledPages.end() ){
			crawledPages.insert(currFile);
			outputString += currFile + '\n';

			string fileText;
			string line; 

			while (getline(ifile, line)){
				fileText += line;
			}

			// Iterate through the text to find any outgoing links, indicated by parentheses
			int linkStart = 0;
			for(unsigned int i = 0; i < fileText.length(); i++){
				// Indicates found outgoing link
				if(fileText.at(i) == '(' ){
					linkStart = i+1;
				}
				if(fileText.at(i) == ')' ){
					string outgoingLink = fileText.substr(linkStart, i - linkStart);
					DFSqueue.push_back(outgoingLink);
					dfs(outgoingLink, crawledPages, DFSqueue, outputString);
					DFSqueue.pop_back();
				}

			}
		}
		ifile.close();
		return;
}

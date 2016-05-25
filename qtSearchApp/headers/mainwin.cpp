#include "mainwin.h"
#include "WebPage.h"
#include "msort.h"
#include "AdInfo.h"
#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
#include <exception>
#include <QCoreApplication>

using namespace std;

struct bidComp{

	bool operator() (const Ad & lhs, const Ad & rhs)
	{
		return lhs.bidAmount > rhs.bidAmount;
	}
};

struct nameComp{

	bool operator() (const Ad & lhs, const Ad & rhs)
	{
		return lhs.companyName < rhs.companyName;
	}
};

struct WebRankComp
{
	bool operator() (WebPage* lhs, WebPage* rhs)
	{
		return lhs->getRank() < rhs->getRank();
	}
};

struct AlphaStrComp
{
	bool operator() (const string & lhs, const string & rhs)
	{
		return lhs < rhs;
	}
};

MainWin::MainWin(QWidget *parent) : QMainWindow(parent)
{
	// initialize webMap and webPtrs
	initializeMaps();

	//initialize Ad dababase and information
	initializeAds();

	// allocate memory for all widgets
	mainWindowLayout = new QVBoxLayout;
	resultTypeLayout = new QHBoxLayout;
	searchButtonLayout = new QHBoxLayout;
	mainResults = new QListWidget;
	adResults = new QListWidget; // hw10
	txtWordsInput = new QPlainTextEdit;
	searchListLabel = new QLabel("Webpage Results:", this);
	adListLabel = new QLabel("Ad Results:", this);
	singleButton = new QPushButton("&Single Word");
	orButton = new QPushButton("&OR");
	andButton = new QPushButton("&AND");
	aboutButton = new QPushButton("&About");
	alphaButton = new QRadioButton("Sort results alphabetically", this);
	rankButton = new QRadioButton("Sort results by PageRank", this);

	// setup the grid layout
	centerWidget  = new QWidget(this);
	centerWidget->setLayout(mainWindowLayout);
	this->setCentralWidget(centerWidget);

	// set up the layout by adding widgets
	mainWindowLayout->addWidget(txtWordsInput, 0);
	mainWindowLayout->addLayout(resultTypeLayout, 1);
	mainWindowLayout->addLayout(searchButtonLayout, 2);
	mainWindowLayout->addWidget(searchListLabel, 3);
	mainWindowLayout->addWidget(mainResults, 4);
	mainWindowLayout->addWidget(adListLabel, 5);
	mainWindowLayout->addWidget(adResults, 6);
	mainWindowLayout->addWidget(aboutButton, 7);

	// set up resultTypeLayout
	resultTypeLayout->addWidget(alphaButton, 0);
	resultTypeLayout->addWidget(rankButton,1);

	// set up searchButtonLayout
	searchButtonLayout->addWidget(singleButton, 0);
	searchButtonLayout->addWidget(orButton, 1);
	searchButtonLayout->addWidget(andButton, 2);

	// connect clicked signal of each button to the right slot
	connect(aboutButton, SIGNAL(clicked()), this, SLOT(showAbout()));
	connect(singleButton, SIGNAL(clicked()), this, SLOT(singleSearch()));
	connect(orButton, SIGNAL(clicked()), this, SLOT(orSearch()));
	connect(andButton, SIGNAL(clicked()), this, SLOT(andSearch()));
	connect(mainResults, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(openResult(QListWidgetItem*)));
	connect(adResults, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(openAd (QListWidgetItem*)));
}


MainWin::~MainWin()
{
	int size = listResultWindows.size();
	for (int i = 0; i < size; i++) {
		delete listResultWindows[i];
	}

	typename std::map<std::string, WebPage*>::iterator it = webPtrs.begin();
	while (it != webPtrs.end()){
		WebPage* webpagePtr = it->second;
		delete webpagePtr;
		++it;
	}

	//write out billing information to companies
	QString argument3_q = QCoreApplication::arguments().at(3);

	string argument3 = argument3_q.toStdString();

	// if (QCoreApplication::arguments().argc<4) {
	// 	cout << "Please enter a output file" << endl;
	// 	return;
	// }
	ofstream ofile (argument3);

	

	std::map<string,Ad>::iterator itt = adMap.begin();
	deque<Ad> ads;
	while(itt!=adMap.end()){
		ads.push_back(itt->second); // put into deque for sorting
		++itt;
	}

	std::sort(ads.begin(), ads.end(), nameComp()); // sort company name alphabetically
	for(unsigned int k = 0; k<ads.size();k++){
		 ofile<<"Company Name: "<<ads[k].companyName<<endl;
		 ofile<<"Company Bill: "<<ads[k].currentBill;
		 ofile<<endl<<endl;

	}

	cout<<"billing information outputed to current directory"<<endl;




	delete mainResults;
	delete searchListLabel;
	delete adListLabel;
	delete adResults; //*
	delete txtWordsInput;
	delete aboutButton;
	delete andButton;
	delete orButton;
	delete singleButton;
	delete alphaButton;
	delete rankButton;
	delete resultTypeLayout;
	delete searchButtonLayout;
	delete mainWindowLayout;
	delete centerWidget;
}


void MainWin::openResult(QListWidgetItem *item)
{
	QString Qquery = item->text();
	string query = Qquery.toStdString();
	auto w = new ResultWin(query, &webPtrs, this);
	listResultWindows.push_back(w);
	w->show();
}

void MainWin::openAd(QListWidgetItem *item)
{
	QString Qquery = item->text();
	QString message = "You are visiting site of the advertiser:  "+Qquery;

	string compName = Qquery.toStdString();
	updateBill(compName); // update bill for company


	QMessageBox::information(this, Qquery, message);

	
}

void MainWin::updateBill(string s)
{
	std::map<string,Ad>::iterator it = adMap.begin();
	while(it!=adMap.end()){ // update bill for company
		if(it->second.companyName == s) {
			it->second.currentBill = it->second.currentBill+ it->second.bidAmount;
			//cout<<"test: "<< it->second.currentBill;
		}
		++it;
	}
	//cout<<endl;
}


void MainWin::showAbout()
{
	QMessageBox::information(this, tr("About"), tr("Assignment 10 -- Ad-Supported Real Search Engine"));
}


int MainWin::initializeMaps()
{	

	QString argument1_q = QCoreApplication::arguments().at(1);
	string argument1 = argument1_q.toStdString();

	// open file
	ifstream ifile (argument1);

	// check if file opened
	if (ifile.fail()) {
		cout << "The data base file could not be opened." << endl;
		return -1;
	}

	// read webpages and words into webMap
	while (!ifile.eof()) {
		string webName;
		ifile >> webName;
		if (ifile.fail()) break;
		// make WebPage object with webName read from file
		WebPage *page;
		page = new WebPage(webName);
		webPtrs.insert(std::pair<string,WebPage*>(webName,page));
		// make Set of all words in that WebPage
		Set<string> webWords = page->allWords();
		// make Set of WebPages to put in Map
		Set<WebPage> webSet;
		webSet.insert(*page);
		typename std::set<string>::iterator it = webWords.begin();
		while (it != webWords.end()){
			// add word as key and Set of WebPages as value
			std::pair<std::map< string,Set<WebPage> >::iterator,bool> check;
			check = webMap.insert( std::pair< string,Set<WebPage> >(*it,webSet) );
			// if that key is already in the Map, add WebPage to the Set
			if (check.second == false) {
				(webMap.at(*it)).insert(*page);
			}
			++it;
		}
	}

	return 0;
}


int MainWin::initializeAds()
{	
	
	QString argument2_q = QCoreApplication::arguments().at(2);
	string argument2 = argument2_q.toStdString();

	// open file
	ifstream ifile (argument2);

	// check if file opened
	if (ifile.fail()) {
		cout << "The ad info file could not be opened." << endl;
		return -1;
	}

	int totalCompany;
	ifile >> totalCompany;

	string keyword;
	double bid;
	string companyName;

	// insert into Ad object
	while(ifile >>keyword && ifile >> bid && getline(ifile, companyName)){
		//erase garbage in company name
		while(companyName[0]==' ') companyName.erase (companyName.begin()+0);
		//correctCase(keyword);
		Ad newAd(bid, companyName); // initialize new add
		adMap.insert(std::pair<string, Ad>(keyword, newAd)); // keyword is key, Ad itself is value
	}
	//used for test
	// std::map<string,Ad>::iterator it = adMap.begin();
	// for (it=adMap.begin(); it!=adMap.end(); ++it)
 //    std::cout << it->first << " => " << it->second.companyName << '\n';

	return 0;
}
void MainWin::listAds(Set<std::string> & strSet){ 

	adResults->clear();

	Set<string> keywords = strSet;
	deque<Ad> adDeque;
	

	std::map<string,Ad>::iterator it = adMap.begin();
	int insertion=0;

	while(it!=adMap.end()){
		string word = (*it).first;
		correctCase(word);

		Set<std::string>::iterator itt = keywords.begin();
		while(itt !=keywords.end()){
			if(*itt == word) {
				adDeque.push_back(it->second); // push found Ads into deque for sortin later
				//(*it).second.currentBill += (*it).second.bidAmount; renew bill information for companies
			}
			++itt;
		}
		++it;
	}

	//sort ads by bid
	if(adDeque.empty()){
		QString noAd = "No Ad exists for the word(s) you searched";
		adResults->insertItem (0, noAd);
	}
	std::sort(adDeque.begin(),adDeque.end(), bidComp());
	for(unsigned int i =0; i<adDeque.size(); i++) {
		adResults->insertItem (insertion, QString::fromStdString(  adDeque[i].companyName   ) );
		insertion++;
	}

}
void MainWin::singleSearch()
{
	// clear the results from the last query
	mainResults->clear();

	Set<string> singleWord;

	// take input from text edit and store in std::string
	QString Qinput = txtWordsInput->toPlainText();
	string input1 = Qinput.toStdString();

	checkAlphaNum(input1,singleWord);

	listAds(singleWord);

	try {
		if (singleWord.size() != 1) {
			QString tooManyWords = "You did not enter a single word.";
			mainResults->insertItem(0,tooManyWords);
		}
		else {
			// get Set of WebPages that are associated with that word
			Set<WebPage> results1 = webMap.at(input1);

			// check if alphaButton was checked
			if (alphaButton->isChecked()) {
				// get expanded results using setGraph
				map< WebPage*, Set<WebPage*> > singleAdjacencyList = setGraph(results1);

				// make deque to give to mergeSort
				deque<string> singleResults;
				typename std::map< WebPage*, Set<WebPage*> >::iterator singleIt = singleAdjacencyList.begin();
				while (singleIt != singleAdjacencyList.end()) {
					singleResults.push_back((singleIt->first)->filename());
					++singleIt;
				}

				// declare comparator and call mergeSort
				AlphaStrComp alComp;
				mergeSort (singleResults, alComp);

				// iterate over singleResults (deque) and insert into mainResults
				for (unsigned int j = 0; j < singleResults.size(); j++) {
					mainResults->insertItem (j, QString::fromStdString(singleResults[j]));
				}
			}

			// check if rankButton is checked
			else if (rankButton->isChecked()) {
				// get expanded results using setGraph
				map< WebPage*, Set<WebPage*> > singleAdjacencyList = setGraph(results1);

				// make deque to give to mergeSort
				deque<WebPage*> singleResults;
				typename std::map< WebPage*, Set<WebPage*> >::iterator singleIt = singleAdjacencyList.begin();
				while (singleIt != singleAdjacencyList.end()) {
					singleResults.push_back(singleIt->first);
					++singleIt;
				}

				// declare comparator, calculate page rank, and call mergeSort
				WebRankComp rankComp;
				doPageRank (singleAdjacencyList);
				mergeSort (singleResults, rankComp);

				// iterate over singleResults (deque) and insert into mainResults
				for (unsigned int j = 0; j < singleResults.size(); j++) {
					mainResults->insertItem (j, QString::fromStdString((singleResults[j])->filename()));
				}
			}

			else {
				QString noResultType = "Please choose a way to sort results and search again.";
				mainResults->insertItem (0, noResultType);
			}

/*
			typename std::set<WebPage>::iterator it = results1.begin();
			while (it != results1.end()){
				string webName = (*it).filename();
				mainResults->insertItem(0,QString::fromStdString(webName));
				++it;
			}
*/

		}
	}
	catch (const out_of_range & oor) {
		QString noMatch = "There were no matches for your query.";
		mainResults->insertItem(0,noMatch);
	}
}


void MainWin::orSearch()
{
	// clear the results from the last query
	mainResults->clear();

	// take input from text edit and store in std::string
	QString Qinput = txtWordsInput->toPlainText();
	string userInput = Qinput.toStdString();

	Set<WebPage> orWeb;
	Set<string> orWords;

	checkAlphaNum(userInput,orWords);
	listAds(orWords);

	// iterates over orWords to get values from webMap
	// add that value to orWeb
	typename std::set<string>::iterator strPtr = orWords.begin();
	while (strPtr != orWords.end()){
		try {
			orWeb = orWeb.setUnion(webMap.at(*strPtr));
		}
		catch (const out_of_range & oor){}
		// error means that the word was not in webMap
		++strPtr;
	}

	// results
	if (orWeb.size() == 0) {
		QString noMatches = "There were no matches to your query.";
		mainResults->insertItem(0,noMatches);
	}
	else {
		// check if alphaButton was checked
		if (alphaButton->isChecked()) {
			// get expanded results using setGraph
			map< WebPage*, Set<WebPage*> > orAdjacencyList = setGraph(orWeb);

			// make deque to give to mergeSort
			deque<string> orResults;
			typename std::map< WebPage*, Set<WebPage*> >::iterator orIt = orAdjacencyList.begin();
			while (orIt != orAdjacencyList.end()) {
				orResults.push_back((orIt->first)->filename());
				++orIt;
			}

			// declare comparator and call mergeSort
			AlphaStrComp alComp;
			mergeSort (orResults, alComp);

			// iterate over orResults (deque) and insert into mainResults
			for (unsigned int j = 0; j < orResults.size(); j++) {
				mainResults->insertItem (j, QString::fromStdString(orResults[j]));
			}
		}

		// check if rankButton is checked
		else if (rankButton->isChecked()) {
			// get expanded results using setGraph
			map< WebPage*, Set<WebPage*> > orAdjacencyList = setGraph(orWeb);

			// make deque to give to mergeSort
			deque<WebPage*> orResults;
			typename std::map< WebPage*, Set<WebPage*> >::iterator orIt = orAdjacencyList.begin();
			while (orIt != orAdjacencyList.end()) {
				orResults.push_back(orIt->first);
				++orIt;
			}

			// declare comparator, calculate page rank, and call mergeSort
			WebRankComp rankComp;
			doPageRank (orAdjacencyList);
			mergeSort (orResults, rankComp);

			// iterate over orResults (deque) and insert into mainResults
			for (unsigned int j = 0; j < orResults.size(); j++) {
				mainResults->insertItem (j, QString::fromStdString((orResults[j])->filename()));
			}
		}

		// print message if sort option not chosen
		else {
			QString noResultType = "Please choose a way to sort results and search again.";
			mainResults->insertItem (0, noResultType);
		}
	}
}


void MainWin::andSearch()
{
	// clear the results from the last query
	mainResults->clear();

	// take input from text edit and store in std::string
	QString Qinput = txtWordsInput->toPlainText();
	string userInput = Qinput.toStdString();

	Set<WebPage> andWeb;
	Set<string> andWords;

	// parse userInput and add valid words to andWords
	checkAlphaNum(userInput,andWords);
	listAds(andWords);
	// iterates over andWords to get values from webMap
	// add that value to andWeb
	typename std::set<string>::iterator strPtr1 = andWords.begin();
	// to use setIntersection andWeb should not be empty
	try {
		andWeb = webMap.at(*strPtr1);
		while (strPtr1 != andWords.end()){
			andWeb = andWeb.setIntersection(webMap.at(*strPtr1));
			// error means that the word was not in webMap
			++strPtr1;
		}

		// results
		if (andWeb.size() == 0) {
			QString noMatches = "There were no matches to your query.";
			mainResults->insertItem(0,noMatches);
		}
		else {
			// check if alphaButton was checked
			if (alphaButton->isChecked()) {
				// get expanded results using setGraph
				map< WebPage*, Set<WebPage*> > andAdjacencyList = setGraph(andWeb);
	
				// make deque to give to mergeSort
				deque<string> andResults;
				typename std::map< WebPage*, Set<WebPage*> >::iterator andIt = andAdjacencyList.begin();
				while (andIt != andAdjacencyList.end()) {
					andResults.push_back((andIt->first)->filename());
					++andIt;
				}
	
				// declare comparator and call mergeSort
				AlphaStrComp alComp;
				mergeSort (andResults, alComp);
	
				// iterate over andResults (deque) and insert into mainResults
				for (unsigned int j = 0; j < andResults.size(); j++) {
					mainResults->insertItem (j, QString::fromStdString(andResults[j]));
				}
			}
	
			// check if rankButton is checked
			else if (rankButton->isChecked()) {
				// get expanded results using setGraph
				map< WebPage*, Set<WebPage*> > andAdjacencyList = setGraph(andWeb);
	
				// make deque to give to mergeSort
				deque<WebPage*> andResults;
				typename std::map< WebPage*, Set<WebPage*> >::iterator andIt = andAdjacencyList.begin();
				while (andIt != andAdjacencyList.end()) {
					andResults.push_back(andIt->first);
					++andIt;
				}

				// declare comparator, calculate page rank, and call mergeSort
				WebRankComp rankComp;
				doPageRank (andAdjacencyList);
				mergeSort (andResults, rankComp);

				// iterate over andResults (deque) and insert into mainResults
				for (unsigned int j = 0; j < andResults.size(); j++) {
					mainResults->insertItem (j, QString::fromStdString((andResults[j])->filename()));
				}
			}

			// print message if sort option not chosen
			else {
				QString noResultType = "Please choose a way to sort results and search again.";
				mainResults->insertItem (0, noResultType);
			}
		}
	}
	catch (const out_of_range & oor){
		QString wordNotFound = "One of the words was not found in any of the web pages.";
		mainResults->insertItem(0,wordNotFound);
	}
}


void MainWin::correctCase(string & s)
{
  int size = s.size();
  for (int z = 0; z < size; z++) 
  	s[z] = towlower(s[z]);
}


void MainWin::checkAlphaNum (string s, Set<string> & strSet)
{
	while (!s.empty()) {
		unsigned int numChar = 0;
		// check if word is alphanumeric
		while (isalnum(s[numChar])){
			// makes characters lowercase
			s[numChar] = towlower(s[numChar]);
			numChar++;
		}

		string subWord;

		// if word is interuppted by symbol,
		// creates substring of just alphanumeric and adds it to strSet
		// word becomes whatever comes after the substring
		if (numChar < s.size()) {
			subWord = s.substr(0,numChar);
			s = s.substr(numChar+1);
			if (!subWord.empty())
				strSet.insert(subWord);
		}

		// if word is all alphanumeric, add it to wordList
		else {
			strSet.insert(s);
			break;
		}
	}
}

std::map< WebPage*,Set<WebPage*> > MainWin::setGraph (Set<WebPage> s)
{
	Set<WebPage*> t;
	map< WebPage*, Set<WebPage*> > mapGraph;

	// Add all pages from Set<WebPage> S into Set<WebPage*> T
	for(std::set<WebPage>::iterator it = s.begin(); it != s.end(); ++it)
	{
		WebPage* temp = webPtrs.at((*it).filename());
		t.insert(temp);
	}

	// For every WebPage in S, add its incoming/outgoing links to T
	for(std::set<WebPage*>::iterator it = t.begin(); it != t.end(); ++it)
	{
		Set<WebPage*> outgoingLinks = (*it)->allOutgoingLinks(&webPtrs);
		Set<WebPage*> incomingLinks = (*it)->allIncomingLinks(&webPtrs);

		for(Set<WebPage*>::iterator itt = outgoingLinks.begin(); itt != outgoingLinks.end(); ++itt){
			if(s.contains(**itt)){
				t.insert(*itt);
			}
		}

		for(Set<WebPage*>::iterator itt = incomingLinks.begin(); itt != incomingLinks.end(); ++itt){
			if(s.contains(**itt)){
				t.insert(*itt);
			}
		}
	}

	// Create map with key == WebPage* in Set T and value == Set<WebPage*> outgoingLinks
	for(std::set<WebPage*>::iterator it = t.begin(); it != t.end(); ++it)
	{
		Set<WebPage*> outgoingLinks = (*it)->allOutgoingLinks(&webPtrs);
		Set<WebPage*> mapKeyValues;

		for(Set<WebPage*>::iterator itt = outgoingLinks.begin(); itt != outgoingLinks.end(); ++itt)
		{
			if(t.contains(*itt))
			{
				mapKeyValues.insert(*itt);
			}
		}

		mapGraph.insert( std::pair<WebPage*,Set<WebPage*>>(*it,mapKeyValues) );
	}

	return mapGraph;
}

void MainWin::doPageRank (std::map< WebPage*, Set<WebPage*> > adjacencyList)
{
	// map which will hold the new score
	std::map<WebPage*, double> newScore;

	double adjSize = (double) adjacencyList.size();
	double initialScore = 1/adjSize;

	typename std::map< WebPage*, Set<WebPage*> >::iterator initialIt = adjacencyList.begin();
	while (initialIt != adjacencyList.end())
	{
		// fill newScore with Set T and set initial 
		newScore.insert (std::pair<WebPage*, double> (initialIt->first,0.0));
		// set initial rank values (1/number of hits)
		(initialIt->first)->setRank(initialScore);
		++initialIt;
	}

/*
	// debug -- print initial scores //
	cout << "Scores before iterations:" << endl;
	typename std::map< WebPage*, Set<WebPage*> >::iterator inDebugIt = adjacencyList.begin();
	while (inDebugIt != adjacencyList.end())
	{
		cout << (inDebugIt->first)->filename() << ": " << (inDebugIt->first)->getRank() << endl;
		++inDebugIt;
	}
	cout << endl;
*/

	// iterate 30 times
	for (int a = 0; a < 30; a++)
	{
		typename std::map< WebPage*, Set<WebPage*> >::iterator it = adjacencyList.begin();
		while (it != adjacencyList.end())
		{
			if (!(it->second).empty())
			{
				// find 85% of page's score to distribute between itself and successors
				double addTo = (0.85 * (it->first)->getRank())/((it->second).size() /*+ 1*/);

/*
				// debug -- print out rank and addTo //
				cout << "## Rank of " << (it->first)->filename() << ": " << (it->first)->getRank() << endl;
				cout << "## addTo is: " << addTo << endl;
*/

				// add fraction to itself
				/*newScore.at(it->first) += addTo;*/
				// add fraction to successors
				typename std::set<WebPage*>::iterator successorIt = (it->second).begin();
				while (successorIt != (it->second).end())
				{
					newScore.at(*successorIt) += addTo;
					++successorIt;
				}
				// split the remaining 15% between all pages
				double remaining15 = (0.15 * (it->first)->getRank())/(adjacencyList.size());

/*
				// debug -- print out remaining15 //
				cout << "## remaining15 is: " << remaining15 << endl << endl;
*/

				typename std::map<WebPage*, double>::iterator allIt = newScore.begin();
				while (allIt != newScore.end())
				{
					(allIt->second) += remaining15;
					++allIt;
				}
			}

			else
			{
				// divide a sink node's current score by the number of total pages
				double sinkAddTo = ((it->first)->getRank())/(adjacencyList.size());

/*
				// debug -- print out rank and sinkAddTo //
				cout << "## Rank of " << (it->first)->filename() << ": " << (it->first)->getRank() << endl;
				cout << "## addTo is: " << sinkAddTo << endl << endl;
*/

				// distribute that number to all the pages
				typename std::map<WebPage*, double>::iterator sinkAllIt = newScore.begin();
				while (sinkAllIt != newScore.end())
				{
					(sinkAllIt->second) += sinkAddTo;
					++sinkAllIt;
				}
			}

			++it;
		}
		
		// set currentRank to newScore and then set newScore to 0
		typename std::map<WebPage*, double>::iterator newScoreIt = newScore.begin();
		while (newScoreIt != newScore.end())
		{
			(newScoreIt->first)->setRank(newScoreIt->second);
			(newScoreIt->second) = 0.0;
			++newScoreIt;
		}

/*
		// debug -- print scores after each iteration//
		cout << "Scores after " << a+1 << " iterations:" << endl;
		typename std::map< WebPage*, Set<WebPage*> >::iterator debugIt = adjacencyList.begin();
		while (debugIt != adjacencyList.end())
		{
			cout << (debugIt->first)->filename() << ": " << (debugIt->first)->getRank() << endl;
			++debugIt;
		}
		cout << endl;
*/
	}
}

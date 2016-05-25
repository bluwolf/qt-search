#include "resultwin.h"
#include <QTextStream>
#include <sstream>

ResultWin::ResultWin(std::string webName, std::map<std::string, WebPage*> *webPtrs, QWidget *parent) : QMainWindow(parent)
{
	// set window title
	setWindowTitle(QString::fromStdString(webName));
	
	// allocate memory for layouts and widgets
	mainLayout = new QHBoxLayout;
	pageText = new QPlainTextEdit;
	linkLayout = new QVBoxLayout;
	incomingLabel = new QLabel("Incoming Links", this);
	outgoingLabel = new QLabel("Outgoing Links", this);
	incomingList = new QListWidget;
	outgoingList = new QListWidget;

	// set up grid
	centerWidget  = new QWidget(this);
	centerWidget->setLayout(mainLayout);
	this->setCentralWidget(centerWidget);

	// set up mainLayout
	mainLayout->addWidget(pageText, 0);
	mainLayout->addLayout(linkLayout, 1);

	// set up linkLayout
	linkLayout->addWidget(incomingLabel, 0);
	linkLayout->addWidget(incomingList, 1);
	linkLayout->addWidget(outgoingLabel, 2);
	linkLayout->addWidget(outgoingList, 3);

	// store arguments
	name = webName;
	pagePtrs = webPtrs;

	// clears old result
	pageText->clear();
	incomingList->clear();
	outgoingList->clear();

	// store the WebPage of the result in resultPage
	WebPage* resultPage = pagePtrs->at(name);

	// access incoming and outgoing links
	Set<WebPage*> resultIncoming = resultPage->allIncomingLinks(pagePtrs);
	Set<WebPage*> resultOutgoing = resultPage->allOutgoingLinks(pagePtrs);

	// add incoming and outgoing links to their QListWidget
	typename std::set<WebPage*>::iterator it = resultIncoming.begin();
	while (it != resultIncoming.end()){
		string incomingName = (*it)->filename();
		incomingList->insertItem(0,QString::fromStdString(incomingName));
		++it;
	}
	it = resultOutgoing.begin();
	while (it != resultOutgoing.end()){
		string outgoingName = (*it)->filename();
		outgoingList->insertItem(0,QString::fromStdString(outgoingName));
		++it;
	}

	// set pageText to read only
	pageText->setReadOnly(true);

	stringstream ss;
	ss << *resultPage;
	while (!ss.eof()) {
		string line;
		getline(ss,line);
		pageText->appendPlainText(QString::fromStdString(line));
	}

	// connection for when a user selects a link
	connect(incomingList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(updateWin(QListWidgetItem*)));
	connect(outgoingList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(updateWin(QListWidgetItem*)));
}

ResultWin::~ResultWin()
{
	delete pageText;
	delete incomingLabel;
	delete outgoingLabel;
	delete incomingList;
	delete outgoingList;
	delete linkLayout;
	delete mainLayout;
	delete centerWidget;
}

void ResultWin::updateWin(QListWidgetItem* item)
{
	// changing QListWidgetItem to string
	QString QupdateName = item->text();
	string updateName = QupdateName.toStdString();

	// clears old result
	pageText->clear();
	incomingList->clear();
	outgoingList->clear();

	// set window title
	setWindowTitle(QString::fromStdString(updateName));

	// store the WebPage of the result in resultPage
	WebPage* resultPage = pagePtrs->at(updateName);

	// access incoming and outgoing links
	Set<WebPage*> resultIncoming = resultPage->allIncomingLinks(pagePtrs);
	Set<WebPage*> resultOutgoing = resultPage->allOutgoingLinks(pagePtrs);

	// add incoming and outgoing links to their QListWidget
	typename std::set<WebPage*>::iterator it = resultIncoming.begin();
	while (it != resultIncoming.end()){
		string incomingName = (*it)->filename();
		incomingList->insertItem(0,QString::fromStdString(incomingName));
		++it;
	}
	it = resultOutgoing.begin();
	while (it != resultOutgoing.end()){
		string outgoingName = (*it)->filename();
		outgoingList->insertItem(0,QString::fromStdString(outgoingName));
		++it;
	}

	// set pageText to read only
	pageText->setReadOnly(true);

	stringstream ss;
	ss << *resultPage;
	while (!ss.eof()) {
		string line;
		getline(ss,line);
		pageText->appendPlainText(QString::fromStdString(line));
	}
}

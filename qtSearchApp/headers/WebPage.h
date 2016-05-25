#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <ctype.h>
#include <cstring>
#include <string>
#include <sstream>
#include <map>
#include "Set.h"

using namespace std;

class WebPage {
  public:
    WebPage ();
    WebPage (string fname);
    ~WebPage ();
    // returns a set with all distinct words on the web page
    Set<string> allWords () const;
    // returns a set containing the names of outgoing links as strings
    Set<string> stringOutgoingSet () const;
    // returns a set of pointers to the WebPage objects that this WebPage links to
    Set<WebPage*> allOutgoingLinks (map<string, WebPage*> *pagePtrs) const;
    // returns a set of pointers to the WebPage objects that link to this WebPage
    Set<WebPage*> allIncomingLinks (map<string, WebPage*> *pagePtrs) const;
    // returns filename_ as string
    string filename () const;
    // returns true if same webPage (filename_)
    bool operator== (const WebPage & page);
    // returns true if filename_ < page.filename_
    bool operator< (const WebPage & page) const;
    // declares operator that lets one print the web page
    friend ostream & operator<< (ostream & os, const WebPage & page);

    void setRank (double cr);
    double getRank ();
  private:
    string filename_;
    Set<string> wordList;
    Set<string> outlinksList;

    double currentRank;
};

#endif

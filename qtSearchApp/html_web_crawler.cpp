#define CURL_STATICLIB
#include <iostream>
#include <string>
#include <cstring>
#include <curl/curl.h>
#include <fstream>
#include <set>
#include <deque>
#include <stdio.h>
#include <curl/easy.h>

using namespace std;

string getHTMLCode(string url);
void webCrawler(string htmlCode, char* outputFile);
void dfs( string &currURL, set<string> &crawledPages, deque<string> &DFSqueue, string &outputString, int depth );
void convertToTextFile(char * pageDirectory);

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int main(int argc, char** argv)
{
  // second argument in command line will be output file
  // third argument in terminal will be the URL of the seed page
  string seedPage = argv[2];

  webCrawler(seedPage, argv[1]);

  // Convert web files to txt files so that they are compatible with our search engine
  //convertToTextFile(argv[1]);

  return 0;
}

string getHTMLCode(string url){
  CURL *curl;
  CURLcode res;
  std::string readBuffer;

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    }
    //std::cout << readBuffer << std::endl;

    return readBuffer;
}


void webCrawler(string pageURL, char* outputFile){

  ofstream ofile(outputFile);

  deque<string> DFSqueue;
  set<string> crawledPages;
  string outputString = "";
  int depth = 0;

  //DFSqueue.push_back(pageURL);

  string htmlCode = getHTMLCode(pageURL);

  unsigned int startIndex = htmlCode.find("href=\"http://");
  htmlCode = htmlCode.substr(startIndex+6);
  unsigned int endIndex = htmlCode.find("\">");
  endIndex = htmlCode.find("\"");
  string outgoingLink = htmlCode.substr(0, endIndex);
  htmlCode = htmlCode.substr(endIndex);
  DFSqueue.push_back(outgoingLink);

  // Parses html code and adds outgoing links to the queue
  while( startIndex != -1){
    startIndex = htmlCode.find("href=\"http://");
    htmlCode = htmlCode.substr(startIndex+6);
    endIndex = htmlCode.find("\">");
    endIndex = htmlCode.find("\"");
    outgoingLink = htmlCode.substr(0, endIndex);
    //cout << outgoingLink << endl;
    htmlCode = htmlCode.substr(endIndex);
    //cout << "code that is left:" << endl << htmlCode << endl << endl;
    DFSqueue.push_back(outgoingLink);
  }

  // Remove excess html code 
  DFSqueue.pop_back();

 /* cout << "Pages in DFSqueue:" << endl;
  for(int i = 0; i < DFSqueue.size(); i++){
    cout << DFSqueue[i] << endl << endl;
  }*/

  while(!DFSqueue.empty()){
    string currURL = DFSqueue.front();

    if(crawledPages.find(currURL) == crawledPages.end()){
      dfs(currURL, crawledPages, DFSqueue, outputString, depth );
      DFSqueue.pop_front();
    }
    else{
      DFSqueue.pop_front();
    }
  }

  ofile << outputString;
  ofile.close();

}

void dfs( string &currURL, set<string> &crawledPages, deque<string> &DFSqueue, string &outputString, int depth ){
  // Limit the depth of the web crawler's search per page
  if(depth == 3){
    return;
  }

  //cout << "entering dfs with page " << currURL << endl;

  depth++;

  // If current URL has not already been found
  if( crawledPages.find(currURL) == crawledPages.end()){
    crawledPages.insert(currURL);
    outputString += currURL + '\n';

    string htmlCode = getHTMLCode(currURL);
    //cout << htmlCode << endl << endl;
    unsigned int startIndex = htmlCode.find("href=\"http://");

    // check to see if there is an outgoing link in the current page
    if(startIndex == -1){
     // cout << "current page " << currURL << " does not have an outgoing link" << endl;
      return;
    }

    htmlCode = htmlCode.substr(startIndex+6);
    unsigned int endIndex = htmlCode.find("\">");
    endIndex = htmlCode.find("\"");
    string outgoingLink = htmlCode.substr(0, endIndex);
   // cout << "       outgoing link is: " << outgoingLink << endl;
    htmlCode = htmlCode.substr(endIndex);
    DFSqueue.push_back(outgoingLink);

    // Parses html code and adds outgoing links to the queue
    while( startIndex != -1){
      startIndex = htmlCode.find("href=\"http://");

      // check to see if there is an outgoing link in the current page
      if(startIndex == -1){
        //cout << "current page " << currURL << " does not have an outgoing link" << endl;
        return;
      }

      htmlCode = htmlCode.substr(startIndex+6);
      endIndex = htmlCode.find("\">");
      endIndex = htmlCode.find("\"");
      outgoingLink = htmlCode.substr(0, endIndex);
      //cout << outgoingLink << endl;
      htmlCode = htmlCode.substr(endIndex);
      //cout << "code that is left:" << endl << htmlCode << endl << endl;
      DFSqueue.push_back(outgoingLink);
      dfs(outgoingLink, crawledPages, DFSqueue, outputString, depth);
      DFSqueue.pop_back();
    }

    // Remove excess html code 
    DFSqueue.pop_back();
  }
  
} // end of DFS function

void convertToTextFile(char * pageDirectory){
  ifstream ifile(pageDirectory);
  // No need to check if file opens correctly because we just made it ourselves

  string textFileNames = "";

  string line;

  while(getline(ifile,line)){
   // cout << "   convert to text file function with file " << line << endl;
    // Remove trailing forward slashes in url
    if(line[line.length()-1] == '/'){
      line = line.substr(0,line.length()-1);
    }

    string newFileName = line + ".txt";

    CURL *curl;
    FILE *fp;
    //char *url = line.c_str();
    newFileName = "./realpages/"+newFileName;
    
    // Create file to write to
    fstream newFile(newFileName.c_str(), fstream::out);
    newFile.close();
    newFile.open(newFileName.c_str() );
    newFile << flush;
    newFile.close();

    CURLcode res = CURLE_OK;
    const char* url       = line.c_str();
    const char* file_name = newFileName.c_str();
    std::cout << "download : " << url       << std::endl;
    std::cout << "to file  : " << file_name << std::endl;
    curl = curl_easy_init();
    if (curl) {
      cout << "curl is good" << endl;
        fp = fopen(file_name,"wb");
        //if(fp) {
         // cout << "fp is good" << endl;
            curl_easy_setopt(curl, CURLOPT_URL, url);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
            res = curl_easy_perform(curl);
            if(CURLE_OK == res) {
                std::cout << "curl_easy_perform succeeded" << std::endl;
            } else {
                std::cout << "curl_easy_perform failed" << std::endl;
            }
            fclose(fp);
       // }
        curl_easy_cleanup(curl);
    }
    //cout << "Text file directory: " << endl << textFileNames << endl;
  }
  ifile.close();

}
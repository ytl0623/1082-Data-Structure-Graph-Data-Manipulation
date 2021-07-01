// 10727130 ¶À·N³Ô 10727124 ¼B¦t§Ê
#include <iostream>
#include <fstream>
#include <cstdio>
#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>
#include <cstring>
#include <string.h>
#include <cmath>
#include <math.h>
#include <time.h>
#include <vector>
#include <stack>
#include <queue>
#include <iomanip>
#include <map>
using namespace std ;

class DirectedGraph {

struct StudentPair {
  char sid1[10] ;
  char sid2[10] ;
  float weight ;
};

struct AdjListNode {
  string sid2 ;
  float weight ;
  AdjListNode *next ;
};

struct AdjList {
  string sid1 ;
  AdjListNode *head = NULL ;
  int influenceValue ;
  map<string,int> process ;     // string:key,int:value, key will be sorted.
};

public:
string fileNum, fileName ;
fstream inFile, outFile ;
vector<AdjList> adjlist ;     // don't forget to clear.
vector<StudentPair> list ;     // don't forget to clear.
float weightLB ;

bool readFile() {
  list.clear() ;
  
  do {
    cout << "Input a file number ([0]Quit): " ;
    cin >> fileNum ;

    if( fileNum == "0" ) {
      cout << endl ;
      return false ;
    }
    else {
        fileName = "pairs" + fileNum + ".bin" ;
        inFile.open(  fileName.c_str(), fstream::in ) ;

        if( inFile.is_open() ) {
          cout << endl ;
          inFile.close() ;
          break ;
        }
        else {
          cout << "\n### " << fileName << " does not exist! ###\n\n" ;
        }
    }
  } while(true) ;

  StudentPair oneSt ;
  int stNo = 0 ;

  inFile.open( fileName.c_str(), fstream::in | fstream::binary ) ;
  if( inFile.is_open() ) {
    inFile.seekg( 0, inFile.end ) ;
    stNo = inFile.tellg() / sizeof(oneSt) ;
    inFile.seekg( 0, inFile.beg ) ;

    for( int i = 0 ; i < stNo ; i++ ) {
      inFile.read( (char*)&oneSt, sizeof(oneSt) ) ;
      list.push_back(oneSt) ;
    }
  }
  inFile.close() ;
  return true ;
}

void sortListNode() {
  for( int k = 0 ; k < adjlist.size() ; k++ ) {
    AdjListNode *tail = adjlist.at(k).head ;
    AdjListNode *temp = adjlist.at(k).head ;
    AdjListNode *cur = adjlist.at(k).head ;
    AdjListNode *pre = adjlist.at(k).head ;

    int size = 0 ;
    while( tail != NULL ) {
      tail = tail->next ;
      size++ ;
    }

    for( int i = size ; i > 0 ; i-- ) {
      cur = adjlist.at(k).head ;
      pre = adjlist.at(k).head ;

      for( int j = 0 ; j < i-1 && cur->next != NULL ; j++ ) {
        if( cur->sid2.compare( cur->next->sid2 ) > 0 ) {
          temp = cur->next ;
          cur->next = temp->next ;
          temp->next = cur ;

          if( cur == adjlist.at(k).head ) {
            adjlist.at(k).head = temp ;
            pre =temp ;
          }
          else {
            pre->next = temp ;
            pre = pre->next ;
          }
        }
        else {
          cur = cur->next ;
          if( j != 0 ) {
            pre = pre->next ;
          }
        }
      }
    }
  }
}

void sortListbyID() {
  int min ;

  for( int i = 0 ; i < adjlist.size() ; i++ ) {
    min = i ;

    for( int j = i+1 ; j < adjlist.size() ; j++ ) {
      if( adjlist.at(j).sid1.compare( adjlist.at(min).sid1 ) < 0 ) {
        min = j ;
      }
    }

    AdjList temp ;
    temp.sid1 = adjlist.at(i).sid1 ;
    temp.head = adjlist.at(i).head ;
    temp.process = adjlist.at(i).process ;
    temp.influenceValue = adjlist.at(i).influenceValue ;

    adjlist.at(i).sid1 = adjlist.at(min).sid1 ;
    adjlist.at(i).head = adjlist.at(min).head ;
    adjlist.at(i).process = adjlist.at(min).process ;
    adjlist.at(i).influenceValue = adjlist.at(min).influenceValue ;
    
    adjlist.at(min).sid1 = temp.sid1 ;
    adjlist.at(min).head = temp.head ;
    adjlist.at(min).process = temp.process ;
    adjlist.at(min).influenceValue = temp.influenceValue ;
  }
}

void check() {
  bool existed = false ;
  AdjListNode *walk ;

  for( int i = 0 ; i < adjlist.size() ; i++ ) {
    walk = adjlist.at(i).head ;

    while( walk != NULL ) {
      existed = false ;
      
      for( int j = 0 ; j < adjlist.size() ; j++ ) {
        if( adjlist.at(j).sid1 == walk->sid2 ) {
          existed = true ;
          break ;
        }
      }
      
      if( !existed ) {
        AdjList temp ;
        temp.sid1 = walk->sid2 ;
        temp.head = NULL ;
        adjlist.push_back( temp ) ;
      }
      walk = walk->next ;
    }
  }
}

void m1() {
  adjlist.clear() ;
  
  bool found = false ;
  string tempStr ;
  int i, j ;

  for( i = 0 ; i < list.size() ; i++ ) {     // list to adjlist.
    found = false ;
    
    tempStr.assign( list.at(i).sid1 ) ;

    for( j = 0 ; j < adjlist.size() && !found ; j++ ) {
      if( adjlist.at(j).sid1 == tempStr ) {
        found = true ;
        break ;
      }
    }

    if( !found ) {
      AdjList tempAdjList ;
      tempAdjList.sid1 = tempStr ;
      tempAdjList.head = new AdjListNode ;
      tempStr.assign( list.at(i).sid2 ) ;
      tempAdjList.head->sid2 = tempStr ;
      tempAdjList.head->weight = list.at(i).weight ;
      tempAdjList.head->next = NULL ;
      adjlist.push_back( tempAdjList ) ;
    }
    else {
      AdjListNode *walk = adjlist.at(j).head ;
      while( walk->next != NULL ){     // point to the last node.
        walk = walk->next ;
      }

      AdjListNode *tempNode = new AdjListNode ;
      tempStr.assign( list.at(i).sid2 ) ;
      tempNode->sid2 = tempStr ;
      tempNode->weight = list.at(i).weight ;
      tempNode->next = NULL ;
      walk->next = tempNode ;
    }
  }

  check() ;
  sortListbyID() ;
  sortListNode() ;

  fileName = "pairszzzzzzzzzzzzzz" + fileNum + ".adj" ;
  outFile.open( fileName.c_str(), fstream::out ) ;

  if( outFile.is_open() ) {
    outFile << "<<< There are " << adjlist.size() << " IDs in total. >>>\n" ;
    cout << "<<< There are " << adjlist.size() << " IDs in total. >>>\n\n" ;

    for( i = 0 ; i < adjlist.size() ; i++ ) {
      outFile << "[" << setw(3) << i+1 << "] " << adjlist.at(i).sid1 << ": \n" ;

      AdjListNode *walk = adjlist.at(i).head ;
      for( j = 1 ; walk != NULL ; j++ ) {
        outFile << "\t(" << setw(2) << j << ") " << walk->sid2 << "," << setw(6) << walk->weight ;
        if( j % 10 == 0 ) {
          outFile << endl ;
        }
        walk = walk->next ;
      }
      outFile << endl ;
    }
    outFile << "<<< There are " << list.size() << " nodes in total. >>>\n" ;
    cout << "<<< There are " << list.size() << " nodes in total. >>>\n\n" ;
  }
  outFile.close() ;
}

int returnPos( string id ) {
  for( int i = 0 ; i < adjlist.size() ; i++ ) {
    if( adjlist.at(i).sid1 == id ) {
      return i ;
    }
  }
}

void count() {
  queue<string> q ;
  bool visited[adjlist.size()] ;

  for( int i = 0 ; i < adjlist.size() ; i++ ) {
    cout << "i: " << i << endl ;
    for( int j = 0 ; j < adjlist.size() ; j++ ) {     // reset
      visited[j] = false ;
    }

    q.push( adjlist.at(i).sid1 ) ;
    visited[i] = true ;

    while( q.size() != 0 ) {
      AdjListNode *walk = adjlist.at( returnPos(q.front()) ).head ;
      q.pop() ;

      while( walk != NULL && adjlist.at(i).process.size() != adjlist.size()-1 ) {
        if( !visited[returnPos(walk->sid2)] ) {
          q.push( walk->sid2 ) ;
          visited[returnPos(walk->sid2)] = true ;
          adjlist.at(i).process.insert( pair<string,int>( walk->sid2, adjlist.at(i).process.size() ) ) ;
        }
        walk = walk->next ;
      }
    }
    adjlist.at(i).influenceValue = adjlist.at(i).process.size() ;
  }
  cout << endl ;
}

void sortListByValue() {
  int priority ;
  
  for( int i = 0 ; i < adjlist.size() ; i++ ) {
    priority = i ;

    for( int j = i+1 ; j < adjlist.size() ; j++ ) {
      if( adjlist.at(j).influenceValue > adjlist.at(priority).influenceValue ) {
        priority = j ;
      }
      else if( adjlist.at(j).influenceValue == adjlist.at(priority).influenceValue ) {
        if(  adjlist.at(j).sid1 <  adjlist.at(priority).sid1 ) {
          priority = j ;
        }
      }
    }

    AdjList temp ;
    temp.sid1 = adjlist.at(i).sid1 ;
    temp.head = adjlist.at(i).head ;
    temp.process = adjlist.at(i).process ;
    temp.influenceValue = adjlist.at(i).influenceValue ;


    adjlist.at(i).sid1 = adjlist.at(priority).sid1 ;
    adjlist.at(i).head = adjlist.at(priority).head ;
    adjlist.at(i).process = adjlist.at(priority).process ;
    adjlist.at(i).influenceValue = adjlist.at(priority).influenceValue ;

    adjlist.at(priority).sid1 = temp.sid1 ;
    adjlist.at(priority).head = temp.head ;
    adjlist.at(priority).process = temp.process ;
    adjlist.at(priority).influenceValue = temp.influenceValue ;
  }
}

void m2() {
  count() ;
  sortListByValue() ;

  fileName = "pairsszzzzzzzzz" + fileNum + ".cnt" ;
  outFile.open( fileName.c_str(), fstream::out ) ;

  if( outFile.is_open() ) {
    outFile << "<<< There are " << adjlist.size() << " IDs in total. >>>\n" ;
    cout << "<<< There are " << adjlist.size() << " IDs in total. >>>\n\n" ;

    for( int i = 0 ; i < adjlist.size() ; i++ ) {
      outFile << "[" << setw(3) << i+1 << "] " << adjlist.at(i).sid1
              << "(" << adjlist.at(i).process.size() << "): \n" ;
              
      int j = 0 ;
      for( map<string,int>::iterator it = adjlist.at(i).process.begin() ; it != adjlist.at(i).process.end() ; it++ ) {
        outFile << "\t(" << setw(2) << j+1 << ") " << it->first ;

        if( (j+1) % 10 == 0 ) {
          outFile << endl ;
        }
        j++ ;
      }
      outFile << endl ;
    }
  }
  outFile.close() ;
}

};

int main() {
  string choice ;
  DirectedGraph work ;

  cout << "**** Graph data manipulation ****\n"
       << "* 0. QUIT                       *\n"
       << "* 1. Build adjacency lists      *\n"
       << "* 2. Compute connection counts  *\n"
       << "*********************************\n"
       << "Input a choice(0, 1, 2): " ;
  cin >> choice ;

  while( choice != "0" ) {
    if( choice == "1" ) {
      if( work.readFile() ) {
        work.m1() ;
      }
    }
    else if( choice == "2" ) {
      if( work.list.empty() ) {
        cout << "\n### There is no graph and choose 1 first. ###\n\n" ;
      }
      else {
        work.m2() ;
      }
    }
    else {
      cout << "\nCommand does not exist!\n\n" ;
    }

    cout << "**** Graph data manipulation ****\n"
         << "* 0. QUIT                       *\n"
         << "* 1. Build adjacency lists      *\n"
         << "* 2. Compute connection counts  *\n"
         << "*********************************\n"
         << "Input a choice(0, 1, 2): " ;
    cin >> choice ;
  }
}

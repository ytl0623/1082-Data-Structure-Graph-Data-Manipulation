// 10727130 独種吃 10727124 糂

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
  int pos ;
  AdjListNode *next ;
};

struct AdjList {
  string sid1 ;
  AdjListNode *head = NULL ;
  int influenceValue ;
  vector<string> process ;
  vector<string> process2 ;
  //multimap<greater<string>,string> process2 ;
};

struct ForQ {
  string sid ;	
  int pos ;
};

public:
string fileNum, fileName ;
fstream inFile, outFile ;
vector<AdjList> adjlist ;     // don't forget to clear.
vector<StudentPair> list ;     // don't forget to clear.
float weightLB ;
float realnum ;

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

  fileName = "pairs" + fileNum + ".adj" ;
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

void sortProcessbyID( vector<string> &process ) {
  int min ;
  
  for( int i = 0 ; i < process.size() ; i++ ) {
    min = i ;

    for( int j = i+1 ; j < process.size() ; j++ ) {
      if( process.at(j).compare( process.at(min) ) < 0 ) {
        min = j ;
      }
    }

    string temp ;
    temp = process.at(i) ;
    process.at(i) = process.at(min) ;
    process.at(min) = temp ;
  }
}

void setPos() {
	for ( int i = 0 ; i < adjlist.size() ; i++ ) {
		AdjListNode *walk = adjlist.at(i).head ;
		while ( walk != NULL ) {
			for ( int i = 0 ; i < adjlist.size() ; i++ ) {
			    if ( walk->sid2 == adjlist.at(i).sid1 )
			      walk->pos = i ;
		    }
		    walk = walk->next ;
		}		
	}
}

void count() {
  queue<ForQ> q ;
  
  ForQ temp ;
  bool visited[adjlist.size()] ;
  //int ccount = 0 ;
  //int a = 0 ; 代Ω计ノ 
  
  for( int i = 0 ; i < adjlist.size() ; i++ ) {
    //cout << "i: " << i << endl ;
    for( int j = 0 ; j < adjlist.size() ; j++ ) {     // reset
      visited[j] = false ;
    }
    
    temp.sid = adjlist.at(i).sid1 ;
    temp.pos = i ;

    q.push( temp ) ;
    visited[i] = true ;
    //ccount++ ;


    while( q.size() != 0 ) {
      int pos = q.front().pos ;
      AdjListNode *walk = adjlist.at( pos ).head ;
      q.pop() ;

      while( walk != NULL ) {
      	int pos = walk->pos ;
      	 //a++ ; 代Ω计 
        if( !visited[pos] ) {
          temp.sid = walk->sid2 ;
          temp.pos = pos ;
          q.push( temp ) ;
          visited[pos] = true ;
          //ccount++ ;
          adjlist.at(i).process.push_back( walk->sid2 ) ;
        }
        walk = walk->next ;
        //if ( ccount == adjlist.size() )
          //break ;
      }
    }
    
    sortProcessbyID( adjlist.at(i).process ) ;
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
    temp.process2 = adjlist.at(i).process2 ;
    temp.influenceValue = adjlist.at(i).influenceValue ;


    adjlist.at(i).sid1 = adjlist.at(priority).sid1 ;
    adjlist.at(i).head = adjlist.at(priority).head ;
    adjlist.at(i).process = adjlist.at(priority).process ;
    adjlist.at(i).process2 = adjlist.at(priority).process2 ;
    adjlist.at(i).influenceValue = adjlist.at(priority).influenceValue ;

    adjlist.at(priority).sid1 = temp.sid1 ;
    adjlist.at(priority).head = temp.head ;
    adjlist.at(priority).process = temp.process ;
    adjlist.at(priority).process2 = temp.process2 ;
    adjlist.at(priority).influenceValue = temp.influenceValue ;
  }
}

void m2() {
  setPos() ;
  count() ;
  sortListByValue() ;

  fileName = "pairs" + fileNum + ".cnt" ;
  outFile.open( fileName.c_str(), fstream::out ) ;

  if( outFile.is_open() ) {
    outFile << "<<< There are " << adjlist.size() << " IDs in total. >>>\n" ;
    cout << "<<< There are " << adjlist.size() << " IDs in total. >>>\n\n" ;

    for( int i = 0 ; i < adjlist.size() ; i++ ) {
      outFile << "[" << setw(3) << i+1 << "] " << adjlist.at(i).sid1
              << "(" << adjlist.at(i).process.size() << "): \n" ;

      for( int j = 0 ; j < adjlist.at(i).process.size() ; j++ ) {
        outFile << "\t(" << setw(2) << j+1 << ") " << adjlist.at(i).process.at(j) ;

        if( (j+1) % 10 == 0 ) {
          outFile << endl ;
        }
      }
      outFile << endl ;
    }
  }
  outFile.close() ;
}

void count2() {
  stack<ForQ> s ;
  
  ForQ temp ;
  bool visited[adjlist.size()] ;
  
  for( int i = 0 ; i < adjlist.size() ; i++ ) {
    adjlist.at(i).process2.clear() ;
    //cout << "i: " << i << endl ;
    for( int j = 0 ; j < adjlist.size() ; j++ ) {     // reset
      visited[j] = false ;
    }
    
    temp.sid = adjlist.at(i).sid1 ;
    temp.pos = i ;

    s.push( temp ) ;
    visited[i] = true ;

    while( s.size() != 0 ) {
      int pos = s.top().pos ;
      AdjListNode *walk = adjlist.at( pos ).head ;
      s.pop() ;

      while( walk != NULL ) {
      	int pos = walk->pos ;

        if( !visited[pos] && walk->weight >= realnum && walk->weight != 0 ) {
          temp.sid = walk->sid2 ;
          temp.pos = pos ;
          s.push( temp ) ;
          visited[pos] = true ;

          adjlist.at(i).process2.push_back( walk->sid2 ) ;
        }
        walk = walk->next ;

      }
    }
    
    sortProcessbyID( adjlist.at(i).process2 ) ;
    adjlist.at(i).influenceValue = adjlist.at(i).process2.size() ;
  }
  cout << endl ;
}

void m3() {
  do {
    cout << "Input a real number in [0,1]: " ;
    cin >> realnum ;

    if( realnum > 1 || realnum < 0 ) {
      cout << "\n### It is NOT in [0,1] ###\n\n" ;
    }
    else {
      break ;
    }
  } while(true) ;
  
  setPos() ;
  count2() ;
  sortListByValue() ;
   
  fileName = "pairs" + fileNum + ".inf" ;
  outFile.open( fileName.c_str(), fstream::out ) ;
  
  int IDs = 0 ;
  for( int i = 0 ; i < adjlist.size() ; i++ ) {
    if( adjlist.at(i).process2.size() != 0 ) {
      IDs++ ;
    }
  }
  
  int c = 0;
  if( outFile.is_open() ) {
    outFile << "<<< There are " << IDs << " IDs in total. >>>\n" ;
    cout << "<<< There are " << IDs << " IDs in total. >>>\n\n" ;

    for( int i = 0 ; i < adjlist.size() ; i++ ) {
      if( adjlist.at(i).process2.size() != 0 ) {
      outFile << "[" << setw(3) << c+1 << "] " << adjlist.at(i).sid1
              << "(" << adjlist.at(i).process2.size() << "): \n" ;   
              
      for( int j = 0 ; j < adjlist.at(i).process2.size() ; j++ ) {
        outFile << "\t(" << setw(2) << j+1 << ") " << adjlist.at(i).process2.at(j) ;

        if( (j+1) % 10 == 0 ) {
          outFile << endl ;
        }
      }
      c++ ;
      outFile << endl ;
      }
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
       << "* 3. Estimate influence values  *\n" 
       << "*********************************\n"
       << "Input a choice(0, 1, 2, 3): " ;
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
    else if( choice == "3" ) {
      if( work.list.empty() ) {
        cout << "\n### There is no graph and choose 1 first. ###\n\n" ;
      }
      else {
        work.m3() ;
      }
	}
    else {
      cout << "\nCommand does not exit!\n\n" ;
    }

  cout << "**** Graph data manipulation ****\n"
       << "* 0. QUIT                       *\n"
       << "* 1. Build adjacency lists      *\n"
       << "* 2. Compute connection counts  *\n"
       << "* 3. Estimate influence values  *\n" 
       << "*********************************\n"
       << "Input a choice(0, 1, 2, 3): " ;
    cin >> choice ;
  }
}

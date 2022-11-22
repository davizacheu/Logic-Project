#pragma once
#include <iostream>
#include <set>
#include <string>
#include <sstream>
using namespace std;



class Node {

 private:

  set<int> adjacentNodeIDs;
  bool visited = false;
  int postorder;
  

 public:

  set<int> GetAdjNodeIds(){
    return adjacentNodeIDs;
  }

  bool Getvisited(){
    return visited;
  }

  void Setvisited(bool flag){
    this->visited = flag;
  }

  int GetPostOrder(){
    return postorder;
  }

  void SetPostOrder(int order){
    postorder = order;
  }

  void addEdge(int adjacentNodeID) {
    adjacentNodeIDs.insert(adjacentNodeID);
  }

  string toString() const {
    stringstream out;
    for (set<int>::iterator it = adjacentNodeIDs.begin(); it != adjacentNodeIDs.end(); ++it){
      if(it != adjacentNodeIDs.begin()){
        out << ",";
      }
      out << "R" << *it;
    }
    return out.str();
  }

};
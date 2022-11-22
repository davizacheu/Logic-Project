#pragma once
#include <iostream>
#include <map>
#include <set>
#include "Node.h"
#include <vector>
using namespace std;


class Graph {

 private:

  map<int,Node> nodes;
  int size;

 public:

  Graph(int size) {
    this->size = size;
    for (int nodeID = 0; nodeID < size; nodeID++)
      nodes[nodeID] = Node();
  }

  map<int,Node> Getnodes(){
    return nodes;
  }

  int Getsize(){
    return size;
  }

  void addEdge(int fromNodeID, int toNodeID) {
    nodes[fromNodeID].addEdge(toNodeID);
  }

  string toString() const {
    stringstream out;
    out << "Dependency Graph" << endl;
    for (auto& pair: nodes) {
      int nodeID = pair.first;
      const Node& node = pair.second;
      out << "R" << nodeID << ":" << node.toString() << endl;
    }
    out << endl;
    return out.str();
  }

  Graph reverseGraph(){
    Graph reverse(size);
    for(int fromId = 0; fromId < size; fromId++){
      for(int toId = 0; toId < size; toId++){
        if(nodes[toId].GetAdjNodeIds().count(fromId)){ 
          reverse.addEdge(fromId,toId);
        }
      }  
    }
    return reverse;
  }

  void OrderFinder( Node& currNode, int& order){
    currNode.Setvisited(true);
    // cout << currNode.toString() << endl;
    for (int i = 0; i < size; i++){
      if(currNode.GetAdjNodeIds().count(i) && !nodes[i].Getvisited()){
        OrderFinder(nodes[i], order);
      }
    }
    
    currNode.SetPostOrder(order);
    order++;
    return;
  }

  void DFS(){
    int order = 1;
    for (int currId = 0; currId < this->size; currId++){
      if(!nodes[currId].Getvisited()){
        OrderFinder(nodes[currId], order);
      }
    }
  }

  vector<set<int>> SCC(){
    vector<set<int>> SCCs;
    for(int currOrder = size; currOrder > 0; currOrder--){
      for (int currId = 0; currId < this->size; currId++){
        if(nodes[currId].GetPostOrder() == currOrder && !nodes[currId].Getvisited()){
          set<int> chain;
          SCCFinder(currId, nodes[currId], chain);
          SCCs.push_back(chain);
          break;
        }
      }
    }
    return SCCs;
  }

  void SCCFinder(int nodeid, Node& currNode, set<int>& chain){
    currNode.Setvisited(true);
    chain.insert(nodeid);
    // cout << currNode.toString() << endl;
    for (int i = 0; i < size; i++){
      if(currNode.GetAdjNodeIds().count(i) && !nodes[i].Getvisited()){
        SCCFinder(i, nodes[i], chain);
      }
    }
    
    return;
  }

  void PrintPostOrder(){
    for(int i =0; i < this->size; i++){
      cout << "R" << i << ": " << nodes[i].GetPostOrder() << endl;
    }
    cout << endl;
  }

  void SetPostOrders(Graph sender){
    for(int id = 0; id < size; id++){
      nodes[id].SetPostOrder(sender.Getnodes()[id].GetPostOrder());
    }
  }

  bool DependencyChecker(int fromId, int toId){
    return nodes[fromId].GetAdjNodeIds().count(toId);
  }

};

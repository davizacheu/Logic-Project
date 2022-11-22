#pragma once 
#include <iostream>
#include <sstream>
#include <set>
#include <vector>
#include "Scheme.h"
#include "Tuple.h"
using namespace std;

class Relation {

 private:

  string name;
  Scheme scheme;
  set<Tuple> tuples;

 
  

public:
  
  // Relation(){
  //   name ="";
    
  // }

  Relation(const string& name, const Scheme& scheme)
    : name(name), scheme(scheme) { }

  

  string GetName() const {
    return name;
  }

  void SetName(string newname){
    name = newname;
  }

  Scheme GetScheme() const {
    return scheme;
  }

  set<Tuple> GetTuples() const{
    return tuples;
  }

  void addTuple(const Tuple& tuple) {
    tuples.insert(tuple);
  }

  string toString() const {
    stringstream out;
    for (auto& tuple : tuples)
      out << tuple.toString(scheme) << endl;
    return out.str();
  }

  Relation select(int index, const string& value) const {
    Relation result(name, scheme);
    for (auto& tuple : tuples)
      if (tuple.at(index) == value)
	result.addTuple(tuple);
    return result;
  }

  Relation select2(int index1, int index2){
    Relation result(name,scheme);
    for(auto& tuple : tuples){
      if(tuple.at(index1) == tuple.at(index2)){
        result.addTuple(tuple);
      }
    }
    return result;
  }

  Relation project(vector<int> position) const{
    vector<string> projattributes;
    for (unsigned int i = 0; i < position.size(); i++){
      projattributes.push_back(scheme.at(position[i]));
    }
    Scheme newscheme(projattributes);
    Relation result(name, newscheme);
    vector<string> projvalues(position.size());
    for (auto& tuple : tuples){
        for(unsigned int i = 0; i < position.size(); i++){
          projvalues.at(i) = tuple.at(position[i]);
        }
    
        Tuple newtuple(projvalues);
        result.addTuple(newtuple);
      }
    return result;
}

  Relation rename(vector<string> rename){
    Scheme newscheme(scheme);
    for(unsigned int i = 0; i < rename.size(); i++){
      newscheme.at(i) = rename.at(i);
    }
    Relation result(name, newscheme);
    for(auto& tuple : tuples){
      result.addTuple(tuple);
    }
    return result;
  }

  static bool joinable(const Scheme& leftScheme, const Scheme& rightScheme,
		       const Tuple& leftTuple, const Tuple& rightTuple) {
    for (unsigned leftIndex = 0; leftIndex < leftScheme.size(); leftIndex++) {
      const string& leftName = leftScheme.at(leftIndex);
      const string& leftValue = leftTuple.at(leftIndex);
      // cout << "left name: " << leftName << " value: " << leftValue << endl;
      for (unsigned rightIndex = 0; rightIndex < rightScheme.size(); rightIndex++) {
        const string& rightName = rightScheme.at(rightIndex);
        const string& rightValue = rightTuple.at(rightIndex);
        // cout << "right name: " << rightName << " value: " << rightValue << endl;
        if(leftName == rightName && leftValue != rightValue){
          return false;
        }
      }
    }
    return true;
  }

  Relation join(const Relation& r) {
    string resultname = "result";
    vector<string> resultScheme;
    for (unsigned int i = 0; i < scheme.size(); i++){
      resultScheme.push_back(scheme.at(i));
    }
    bool found;
    for(unsigned int j =0; j < r.GetScheme().size(); j++){
       found = false;
      for(unsigned int k =0; k < resultScheme.size(); k ++){
        if(r.GetScheme().at(j) == resultScheme.at(k)){
          found = true;
        }
      }
      if(!found){
        resultScheme.push_back(r.GetScheme().at(j));
      }
    }
    Scheme joinScheme(resultScheme);

    Relation joinRelation(resultname, joinScheme);

    const Scheme& leftScheme = scheme;
    
    for (const Tuple& leftTuple : tuples) {
      // cout << "left tuple: " << leftTuple.toString(leftScheme) << endl;
      const Scheme& rightScheme = r.scheme;
      for (const Tuple& rightTuple : r.tuples) {
        // cout << "right tuple: " << rightTuple.toString(rightScheme) << endl;
        if(joinable(leftScheme, rightScheme, leftTuple, rightTuple)){
          vector<string> jointuple;
          for(unsigned int i = 0; i < leftTuple.size(); i++){
            jointuple.push_back(leftTuple.at(i));
          }
          for(unsigned int j =0; j < rightTuple.size(); j++){
            found = false;
            for(unsigned int k =0; k < leftScheme.size(); k ++){
              if(rightScheme.at(j) == leftScheme.at(k)){
              found = true;
              }
            }
            if(!found){
              jointuple.push_back(rightTuple.at(j));
            }
          }
          Tuple newTuple(jointuple);
          joinRelation.addTuple(newTuple);
        }
      }      
    }
    return joinRelation;
  }

  



};
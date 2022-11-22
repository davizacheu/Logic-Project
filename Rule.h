#pragma once 
#include <sstream> 
#include <iostream>
#include <string>
#include <vector>
#include "Predicate.h"

using namespace std;

class Rule{
  private:
    Predicate headPredicate;
    vector<Predicate> bodyPredicates;
    int ruleId;

  public:

    void setHeadPredicate(Predicate head){
      headPredicate = head;
    }

    void SetId(int id){
      ruleId = id;
    }

    int GetId(){
      return ruleId;
    }

    void addBodyPredicate(Predicate body){
      bodyPredicates.push_back(body);
    }

    Predicate GetHead() {
      return headPredicate;
    }

    vector<Predicate> GetBody(){
      return bodyPredicates;
    }

    string toString(){
      stringstream out;
      out << headPredicate.toString() << " :- ";
      for (unsigned int b = 0; b < bodyPredicates.size(); b++){
        out << bodyPredicates[b].toString();
        if (b < bodyPredicates.size()-1){
          out << ",";
        }
      }
      out << ".";
      return out.str();
    }

};
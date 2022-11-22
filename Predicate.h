#pragma once 
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "Parameter.h"
using namespace std;


class Predicate {
  private:
    string name;
    vector<Parameter> parameters;
  public:

  string Getname() const{
    return name;
  }

  vector<Parameter> Getparameters() const{
    return parameters;
  }
  void setName(string id){
    name = id;
  }

  void addParameter(Parameter newparam){
    parameters.push_back(newparam);
  }

  string toString(){
    stringstream out;
    out << name << "(";
    for (unsigned int p = 0; p < parameters.size(); p++){
      out << parameters.at(p).toString();
      if (p < parameters.size()-1){
        out << ",";
      }
    }
    out << ")";
    return out.str();
  }
};
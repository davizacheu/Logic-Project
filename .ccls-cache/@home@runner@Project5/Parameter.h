#pragma once
#include<iostream>
#include<string>
using namespace std;


class Parameter {
  private:
    string parameter;

  public:
    Parameter(string id) : parameter(id) {}
    
    string toString(){
      return parameter;
    }
};
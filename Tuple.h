#pragma once 
#include <iostream>
#include <sstream>
#include <vector>
#include "Scheme.h"
using namespace std;


class Tuple : public vector<string> {

 public:

  Tuple(vector<string> values) : vector<string>(values) { }

  string toString(const Scheme& scheme) const {
    stringstream out;
    out << "  ";
    for (unsigned i = 0; i < size(); i++) {
      if (i > 0)
	    out << ", ";
      const string& name = scheme.at(i);
      const string& value = at(i);
      out << name << "=" << value;
    }
    return out.str();
  }
};
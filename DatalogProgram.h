#pragma once
#include "Predicate.h"
#include "Rule.h"
#include <iostream>
#include <set>
using namespace std;

class DatalogProgram {
private:
  vector<Predicate> schemes;
  vector<Predicate> facts;
  vector<Rule> rules;
  vector<Predicate> queries;
  set<string> domain;
  int ruleEnum = 0;

public:
  vector<Predicate> Getschemes() const { return schemes; }
  vector<Predicate> Getfacts() const { return facts; }
  vector<Predicate> Getqueries() const { return queries; }
  vector<Rule> Getrules() const { return rules; }
  set<string> GetDomain() const { return domain; }

  void addScheme(Predicate newScheme) { schemes.push_back(newScheme); }

  void addFact(Predicate newFact) { facts.push_back(newFact); }

  void addRule(Rule newRule) {
    newRule.SetId(this->ruleEnum);
    rules.push_back(newRule);
    ruleEnum++;
  }

  void addDomain(string newString) { domain.insert(newString); }

  void addQuery(Predicate newQuery) { queries.push_back(newQuery); }

  void toString() {
    cout << "Success!" << endl;
    cout << "Schemes(" << schemes.size() << "):" << endl;
    for (unsigned int s = 0; s < schemes.size(); s++) {
      cout << "  " << schemes[s].toString() << endl;
    }
    cout << "Facts(" << facts.size() << "):" << endl;
    for (unsigned int f = 0; f < facts.size(); f++) {
      cout << "  " << facts[f].toString() << "." << endl;
    }
    cout << "Rules(" << rules.size() << "):" << endl;
    for (unsigned int r = 0; r < rules.size(); r++) {
      cout << "  " << rules[r].toString() << endl;
    }
    cout << "Queries(" << queries.size() << "):" << endl;
    for (unsigned int q = 0; q < queries.size(); q++) {
      cout << "  " << queries[q].toString() << "?" << endl;
    }
    cout << "Domain(" << domain.size() << "):" << endl;
    for (set<string>::iterator it = domain.begin(); it != domain.end(); ++it) {
      cout << "  " << *it << endl;
    }
  }
};

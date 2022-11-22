#pragma once
#include <iostream>
#include "Database.h"
#include "Graph.h"
#include "Rule.h"

using namespace std;

class Interpreter{
public:

static Graph makeGraph(const vector<Rule>& rules) {

    Graph graph(rules.size());

    for (unsigned fromID = 0; fromID < rules.size(); fromID++) {
      Rule fromRule = rules.at(fromID);
      // cout << "from rule R" << fromID << ": " << fromRule.toString() << endl;
      for (unsigned pred = 0; pred < fromRule.GetBody().size(); pred++) {
	Predicate bodyPred = fromRule.GetBody().at(pred);
	// cout << "from body predicate: " << bodyPred.toString() << endl;
        for (unsigned toID = 0; toID < rules.size(); toID++) {
	  Rule toRule = rules.at(toID);
	  // cout << "to rule R" << toID << ": " << toRule.toString() << endl;
	      if(toRule.GetHead().Getname() == bodyPred.Getname()){
          // cout << "dependency found: (R" << fromID << ",R" << toID << ")" << endl;
          graph.addEdge(fromID, toID);
        }
        }
      }
    }
    return graph;
  }

  static vector<vector<Rule>> SCCtoRules(vector<Rule> rules, vector<set<int>> scc){
    vector<vector<Rule>> ruleChains;
      for(unsigned int currSet = 0; currSet < scc.size(); currSet++){
        vector<Rule> chainedRules;
        for(set<int>::iterator it = scc.at(currSet).begin(); it != scc.at(currSet).end(); ++it){
          unsigned int nodeId = *it; 
          for(unsigned int i = 0; i < rules.size(); i++){
            if(i == nodeId){
              chainedRules.push_back(rules.at(i));
            }
          }
        }
        ruleChains.push_back(chainedRules);
      }
    return ruleChains;
  }

  static void SCCEval(vector<vector<Rule>> SCCPartition, Database& SCCdb, vector<set<int>> graphSCC, Graph SCCgraph){
    //cout << "Rule Evaluation" << endl;
    for(unsigned int i =0; i < SCCPartition.size(); i++){
    //   for(unsigned int k = 0; k < SCCPartition.at(i).size(); k++){
    //cout << SCCPartition.at(i).at(k).GetId() << " ";
    //   }
    //   cout << endl;
      vector<Rule> currRulechain = SCCPartition.at(i);
      if(currRulechain.size() < 2 && !SCCgraph.DependencyChecker(currRulechain.at(0).GetId(), currRulechain.at(0).GetId())){
        // cout << "IN SOLO SCCEVAL" << endl;
        bool uselessbool;
          //currRulechain.at(0).GetId() << endl;
          //cout << currRulechain.at(0).toString() << endl;
          SCCdb.EvalRule(currRulechain.at(0), uselessbool);
          //cout << "1 passes: R" << currRulechain.at(0).GetId() << endl;
      }
      else{
        //cout << "SCC: ";
        for (set<int>::iterator it = graphSCC.at(i).begin(); it != graphSCC.at(i).end(); ++it){
            //if(it != graphSCC.at(i).begin()){
            //cout << ",";
            //}
            //cout << "R" << *it;
        }
        //cout << endl;
        SCCdb.EvalRules(currRulechain);
        for (set<int>::iterator it = graphSCC.at(i).begin(); it != graphSCC.at(i).end(); ++it){
      //if(it != graphSCC.at(i).begin()){
      //cout << ",";
      //}
      //cout << "R" << *it;
      }
      //cout << endl;
      }
      }
      //cout << endl;
    }
  
   
};
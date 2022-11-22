#pragma once
#include "Parser.h"
#include "Relation.h"
#include <iostream>
using namespace std;

class Database {
private:
  Parser dbparser;
  vector<Relation> relations;

public:
  Database(Parser parser) : dbparser(parser) {}

  void EvalSchemes() {
    
    vector<Predicate> dbschemes = dbparser.Getprogram().Getschemes();
    for (unsigned int i = 0; i < dbschemes.size(); i++) {
      string relname = dbschemes.at(i).Getname();
      vector<string> relattributes;
      vector<Parameter> Paramnames = dbschemes.at(i).Getparameters();
      for (unsigned int j = 0; j < Paramnames.size(); j++) {
        relattributes.push_back(Paramnames.at(j).toString());
      }
      Scheme relscheme(relattributes);
      Relation newrelation(relname, relscheme);
      relations.push_back(newrelation);
    }
  }

  void EvalFacts() {
    vector<Predicate> dbfacts = dbparser.Getprogram().Getfacts();
    for (unsigned int i = 0; i < dbfacts.size(); i++) {
      string relname = dbfacts.at(i).Getname();
      vector<string> relvalues;
      vector<Parameter> Paramvalues = dbfacts.at(i).Getparameters();
      for (unsigned int j = 0; j < Paramvalues.size(); j++) {
        relvalues.push_back(Paramvalues.at(j).toString());
      }
      Tuple reltuple(relvalues);
      for (unsigned int k = 0; k < relations.size(); k++) {
        if (relname == relations.at(k).GetName()) {
          relations.at(k).addTuple(reltuple);
        }
      }
    }
  }

  void EvalAllQueries(){
    cout << "Query Evaluation" << endl;
    vector<Predicate> dbqueries = dbparser.Getprogram().Getqueries();
    bool isRulePred = false;
    for (unsigned int i = 0; i < dbqueries.size(); i++){
      EvalQuery(dbqueries.at(i), isRulePred);
    }
  }

  Relation EvalQuery(Predicate query, bool isRulePred) {
    
    string querel = query.Getname();
    vector<Parameter> queparam = query.Getparameters();
    int relindex = 0;
    for(unsigned int i = 0; i < relations.size(); i++){
      if( querel == relations.at(i).GetName()){
        relindex = i;
      }
    }
    
    Relation queryrelation = relations.at(relindex);
    
    for(unsigned int i = 0; i < queparam.size();i++){
     
       if(dbparser.Getprogram().GetDomain().count(queparam.at(i).toString()) || (queparam.at(i).toString()).at(0) == '\''){
        queryrelation = queryrelation.select(i,queparam.at(i).toString());
      }
      else{
        for(int j = i - 1; j >= 0; j--){
          if(queparam.at(j).toString() == queparam.at(i).toString()){
            queryrelation = queryrelation.select2(i,j);
          }
        }
      }
    }

    vector<int> projpositions;
    vector<string> querybutes;
    for(unsigned int i = 0; i < queparam.size(); i++){
      bool found = false;
      for(int j = i - 1; j >= 0; j--){
        if(queparam.at(j).toString() == queparam.at(i).toString()){
          found = true;
        }
      }
      if(!dbparser.Getprogram().GetDomain().count(queparam.at(i).toString()) && queparam.at(i).toString().at(0) != '\'' && !found){
        projpositions.push_back(i);
        querybutes.push_back(queparam.at(i).toString());
      }
    }
    if(!projpositions.empty()){
    queryrelation = queryrelation.project(projpositions);
    queryrelation = queryrelation.rename(querybutes);
    }

    if(!isRulePred){
    PrintQuery(queryrelation, query, projpositions);
    }
    return queryrelation;
  }

  void PrintQuery(Relation queryrelation, Predicate query, vector<int> projpositions){
    if(!queryrelation.GetTuples().empty()){
      cout << query.toString() << "? Yes(" << queryrelation.GetTuples().size() << ")" << endl;
        
      if (!projpositions.empty()){
      cout << queryrelation.toString();
      }
    }
    else{
      cout << query.toString() << "? No" << endl;
    }
    }

  void EvalRules(vector<Rule> dbrules){
    // cout << "Rule Evaluation" << endl;
     
    int passes = 0;
    bool added;
    do{
      
      added = false;
      for(unsigned int i = 0; i < dbrules.size(); i++){
        //cout << dbrules.at(i).toString() << endl;
        EvalRule(dbrules.at(i), added);
      }
      passes++;
    }while(added);

    //cout << passes << " passes: ";
  }

    void EvalRule(Rule rule, bool& added){
    vector<Relation> interRelations;
    bool isRulePred = true;
    for(unsigned int i = 0; i < rule.GetBody().size(); i++){
      interRelations.push_back(EvalQuery(rule.GetBody().at(i), isRulePred));
    }
    Relation resultRelation = interRelations.at(0);
    for(unsigned int i = 1; i < interRelations.size(); i++){
      resultRelation = resultRelation.join(interRelations.at(i));
    }

    
    vector<Parameter> headParams = rule.GetHead().Getparameters();
    vector<int> joinproj;
    for(unsigned int i = 0; i < headParams.size(); i++){
      for(unsigned int j = 0; j < resultRelation.GetScheme().size(); j++){
        if(headParams.at(i).toString() == resultRelation.GetScheme().at(j)){
          joinproj.push_back(j);
        }
      }
    }

    
    resultRelation = resultRelation.project(joinproj);
    
    resultRelation.SetName(rule.GetHead().Getname());

    
    for(unsigned int i = 0; i < relations.size(); i++){
      if(relations.at(i).GetName() == resultRelation.GetName()){
        resultRelation = resultRelation.rename(relations.at(i).GetScheme());
        for(auto& tuple : resultRelation.GetTuples()){
          if(!relations.at(i).GetTuples().count(tuple)){
            relations.at(i).addTuple(tuple);
            added = true;
            //cout << tuple.toString(resultRelation.GetScheme()) << endl;
            
          }
        }
        break;
      }
    }
    
    
  }
    

  void printDataBase() {
    for (unsigned int i = 0; i < relations.size(); i++) {
      cout << "In print function" << endl;
      cout << relations.at(i).toString();
    }
  }
};
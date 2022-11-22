#pragma once
#include <iostream>
#include <vector>
#include "Token.h"
#include "Scan.h"
#include "Predicate.h"
#include "DatalogProgram.h"
using namespace std;

class Parser{
private:
  vector<Token> tokens;
  DatalogProgram Program;

public:
  Parser(const vector<Token>& tokens) : tokens(tokens) { }

  DatalogProgram Getprogram() const{
    return Program;
  }
  TokenType tokenType() const {
    return tokens.at(0).GetTokenType();
  }
  void advanceToken() {
    tokens.erase(tokens.begin());
  }
  void throwError() {
    cout << "Failure!" << endl;
    cout << "  " << tokens[0].toString() << endl;
    throw tokens[0];
  }

  void match(TokenType t) {
    // cout << "match: " << t << endl;
    if (tokenType() == t){
      advanceToken();
    }
    else
      throwError();
  }

  void idList(Predicate& currPredicate) {
    if (tokenType() == COMMA) {
      match(COMMA);
      Parameter newParameter = Parameter(tokens[0].GetValue());
      currPredicate.addParameter(newParameter);
      match(ID);
      idList(currPredicate);
    } else {
      // lambda
    }
  }

  void scheme() {
    Predicate newScheme;
    if (tokenType() == ID) {
      newScheme.setName(tokens[0].GetValue());
      match(ID);
      match(LEFT_PAREN);
      Parameter newParameter = Parameter(tokens[0].GetValue());
      newScheme.addParameter(newParameter);
      match(ID);
      idList(newScheme);
      match(RIGHT_PAREN);
      Program.addScheme(newScheme);
    } else {
      throwError();
    }
  }

  void schemeList(){
    if(tokenType()==ID){
      scheme();
      schemeList();
    }
    else{
      //lambda
    }
  }

  void factList(){
    if(tokenType()==ID){
      fact();
      factList();
    }
    else{
      //lambda
    }
  }

  void fact(){
    Predicate newFact;
    if(tokenType()==ID){
      newFact.setName(tokens[0].GetValue());
      match(ID);
      match(LEFT_PAREN);
      Parameter newParameter = Parameter(tokens[0].GetValue());
      newFact.addParameter(newParameter);
      Program.addDomain(tokens[0].GetValue());
      match(STRING);
      stringList(newFact);
      match(RIGHT_PAREN);
      match(PERIOD);
      Program.addFact(newFact);
    }
    else{
      throwError();
    }
  }

  void stringList(Predicate& currFact) {
    if (tokenType() == COMMA) {
      match(COMMA);
      Parameter newParameter = Parameter(tokens[0].GetValue());
      currFact.addParameter(newParameter);
      Program.addDomain(tokens[0].GetValue());
      match(STRING);
      stringList(currFact);
    } else {
      // lambda
    }
  }

  void ruleList(){
    if (tokenType()== ID){
      rule();
      ruleList();
    }
    else{
      //lambda
    }
  }

  void rule(){
    Rule newRule;
    if(tokenType()==ID){
    headPredicate(newRule);
    match(COLON_DASH);
    predicate(newRule);
    predicateList(newRule);
    match(PERIOD);
    Program.addRule(newRule);
    }
    else{
      throwError();
    }
  }

  void headPredicate(Rule& currRule){
    Predicate newHead;
    if(tokenType()==ID){
      newHead.setName(tokens[0].GetValue());
      match(ID);
      match(LEFT_PAREN);
      Parameter newParameter = Parameter(tokens[0].GetValue());
      newHead.addParameter(newParameter);
      match(ID);
      idList(newHead);
      match(RIGHT_PAREN);
      currRule.setHeadPredicate(newHead);
    }
    else{
      throwError();
    }
  }

  void predicate(Rule& currRule){
    Predicate newPred;
    if(tokenType()==ID){
      newPred.setName(tokens[0].GetValue());
      match(ID);
      match(LEFT_PAREN);
      parameter(newPred);
      parameterList(newPred);
      match(RIGHT_PAREN);
      currRule.addBodyPredicate(newPred);
    }
    else{
      throwError();
    }
  }

  void predicateList(Rule& currRule){
    if(tokenType()==COMMA){
      match(COMMA);
      predicate(currRule);
      predicateList(currRule);
    }
    else{
      //lambda
    }
  }

  void parameter(Predicate& currPredicate){
    if(tokenType()==ID){
      Parameter newParameter = Parameter(tokens[0].GetValue());
      currPredicate.addParameter(newParameter);
      match(ID);
    }
    else if(tokenType()==STRING){
      Parameter newParameter = Parameter(tokens[0].GetValue());
      currPredicate.addParameter(newParameter);
      match(STRING);
    }
    else{
      throwError();
    }
  }

  void parameterList(Predicate& currPredicate){
    if(tokenType()==COMMA){
      match(COMMA);
      parameter(currPredicate);
      parameterList(currPredicate);
    }
    else{
      //lambda
    }
  }

  void query(){
    Predicate newQuery;
    if(tokenType()==ID){
      newQuery.setName(tokens[0].GetValue());
      match(ID);
      match(LEFT_PAREN);
      parameter(newQuery);
      parameterList(newQuery);
      match(RIGHT_PAREN);
      match(Q_MARK);
      Program.addQuery(newQuery);
    }
    else{
      throwError();
    }
  }

  void queryList(){
    if(tokenType()==ID){
      query();
      queryList();
    }
    else{
      //lambda
    }
  }

  void Parse(){
    match(SCHEMES);
    match(COLON);
    scheme();
    schemeList();
    match(FACTS);
    match(COLON);
    factList();
    match(RULES);
    match(COLON);
    ruleList();
    match(QUERIES);
    match(COLON);
    query();
    queryList();
    match(eof);
    
  }

  void PrintProgram(){
    Program.toString();
  }
  

};
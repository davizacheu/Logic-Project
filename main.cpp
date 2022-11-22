#include "Database.h"
#include "Graph.h"
#include "Interpreter.h"
#include "Node.h"
#include "Parser.h"
#include "Relation.h"
#include "Scan.h"
#include "Scheme.h"
#include "Token.h"
#include "Tuple.h"
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << " input_file" << endl;
    return 1;
  }

  // open file
  string fileName = argv[1];
  ifstream infile;
  infile.open(fileName);

  stringstream buffer;
  buffer << infile.rdbuf();
  string input = buffer.str();

  // Creating a scanner object with a list of tokens
  Scanner s = Scanner(input);
  s.CreateList();

  Parser p = Parser(s.getTokenList());
  // // try {
  p.Parse();
  //   // p.PrintProgram();
  // // }
  // // catch(Token token) {

  // // }

  Database dbprogram(p);
  dbprogram.EvalSchemes();
  dbprogram.EvalFacts();
  // dbprogram.EvalRules(b.Getprogram().Getrules());

  vector<Rule> rules = p.Getprogram().Getrules();

  Graph graph = Interpreter::makeGraph(rules);
  // cout << graph.toString();

  Graph reverse = graph.reverseGraph();
  // cout << reverse.toString();

  reverse.DFS();
  // reverse.PrintPostOrder();

  graph.SetPostOrders(reverse);

  // graph.PrintPostOrder();

  vector<set<int>> graphSCC = graph.SCC();

  // for(unsigned int i = 0; i < graphSCC.size(); i++){
  //   for(set<int>::iterator it = graphSCC.at(i).begin(); it !=
  //   graphSCC.at(i).end(); ++it){
  //     cout << "R" << *it << " ";
  //   }
  //   cout << endl;
  // }

  vector<vector<Rule>> RulesPartition =
      Interpreter::SCCtoRules(rules, graphSCC);

  // for(unsigned int i = 0; i < RulesPartition.size(); i++){
  //   for(unsigned int k = 0; k < RulesPartition.at(i).size(); k++){
  //     cout << RulesPartition.at(i).at(k).toString() << " ";
  //   }
  //   cout << endl;
  // }

  Interpreter::SCCEval(RulesPartition, dbprogram, graphSCC, graph);
  dbprogram.EvalAllQueries();

  return 0;
}

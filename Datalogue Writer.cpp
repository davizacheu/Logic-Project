#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

int NUM_ATRRIBUTES = 4;
int NUM_ATTRIBUTE_VALS = 4;

int main() {

  vector<char> a;
  // Creating a set of char to help check for existence during user inputing
  set<char> as;
  ifstream infile;
  infile.open("Doc.txt");

  char c;
  for (int i = 0; i < NUM_ATRRIBUTES; i++) {
    infile >> c;
    a.push_back(c);
    as.insert(c);
  }

  vector<vector<string>> Aelements(NUM_ATRRIBUTES);
  // Creating sets of string to help check for existence during user inputing
  vector<set<string>> Aelemset(NUM_ATRRIBUTES);

  // Code below is to extract the file info and determine attributes and values
  string element;
  for (int i = 0; i < NUM_ATRRIBUTES; i++) {
    for (int j = 0; j < NUM_ATTRIBUTE_VALS; j++) {
      infile >> element;
      Aelements.at(i).push_back(element);
      Aelemset.at(i).insert(element);
    }
    cout << "Size of set of elements " << a[i] << ":" << Aelemset[i].size()
         << endl;
    for (set<string>::iterator itr = Aelemset[i].begin();
         itr != Aelemset[i].end(); itr++) {
      cout << *itr << " ";
    }
    cout << endl;
  }

  infile.close();

  // Creating Datalog file
  vector<char> variables{'a', 'b', 'c', 'd'};
  vector<char> Variables{'A', 'B', 'C', 'D'};
  ofstream outfile;
  outfile.open("Datalog_gods.txt");
  
  
  // Creating Schemes
  outfile << "Schemes:" << endl << endl;
  for (int i = 0; i < NUM_ATRRIBUTES - 1; i++) {
    for (int j = i + 1; j < NUM_ATRRIBUTES; j++) {
      outfile << a[i] << a[j] << "(A,B)" << endl;
      outfile << "n" << a[i] << a[j] << "(A,B)" << endl;
    }
    outfile << "All" << a[i] << "(a,b,c,d)" << endl << endl;
  }

  outfile << "All" << a[NUM_ATRRIBUTES - 1] << "(a,b,c,d)" << endl << endl;

  // Creating Facts
  outfile << "Facts:" << endl;

  for (int i = 0; i < NUM_ATRRIBUTES; i++) {
    outfile << "All" << a[i] << "(";
    for (int j = 0; j < NUM_ATTRIBUTE_VALS; j++) {
      outfile << Aelements[i][j];
      if (j < NUM_ATTRIBUTE_VALS - 1) {
        outfile << ",";
      }
    }
    outfile << ")." << endl;
  }

  string clues;
  do {
    cout << "Any factual clues you like to add in?" << endl;
    cin >> clues;
  } while (clues != "no" && clues != "n" && clues != "yes" && clues != "y");

  char firstatt;
  int faind;
  string firstval;
  char secatt;
  int saind;
  string secval;
  char facttype;

  while (clues != "no" && clues != "n") {
    cout << "Please identify the attributes and their values" << endl;
    do {
      cout << "First attribute:" << endl;
      cin >> firstatt;
    } while (!as.count(firstatt));
    for (int i = 0; i < NUM_ATRRIBUTES; i++) {
      if (a[i] == firstatt) {
        faind = i;
        break;
      }
    }
    cout << "Index of first attribute:" << faind << endl;

    do {
      cout << "First value:" << endl;
      cin >> firstval;
    } while (!Aelemset[faind].count(firstval));

    do {
      cout << "Second attribute:" << endl;
      cin >> secatt;
    } while (!as.count(secatt));
    for (int i = 0; i < NUM_ATRRIBUTES; i++) {
      if (a[i] == secatt) {
        saind = i;
        break;
      }
    }
    cout << "Index of second attribute:" << saind << endl;

    do {
      cout << "Second value:" << endl;
      cin >> secval;
    } while (!Aelemset[saind].count(secval));

    do {
      cout << "Is this a statement(type s) or negation(type n)" << endl;
      cin >> facttype;
    } while (facttype != 's' && facttype != 'n');

    if (facttype == 's') {
      outfile << firstatt << secatt << "(" << firstval << "," << secval << ")."
              << endl;
      cout << "Any factual clues you like to add in?" << endl;
      cin >> clues;
      cout << clues << endl;
    } else {
      outfile << "n" << firstatt << secatt << "(" << firstval << "," << secval
              << ")." << endl;
      cout << "Any factual clues you like to add in?" << endl;
      cin >> clues;
      cout << clues << endl;
    }
  }

  outfile << endl;

  // Creating Rules
  outfile << "Rules:" << endl;

  //This first section is to print all the bridging rules
  for (int i = 0; i < NUM_ATRRIBUTES - 1; i++) {
    for (int j = i + 1; j < NUM_ATRRIBUTES; j++) {
      for (int k = 0; k < NUM_ATRRIBUTES; k++) {
        if (a[k] != a[i] && a[k] != a[j]) {
          outfile << a[i] << a[j] << "(A,C) :- ";
          if (i < k) {
            outfile << a[i] << a[k] << "(A,B), ";
          } else {
            outfile << a[k] << a[i] << "(B,A), ";
          }
          if (j < k) {
            outfile << a[j] << a[k] << "(C,B).";
          } else {
            outfile << a[k] << a[j] << "(B,C).";
          }
          outfile << endl;

          outfile << "n" << a[i] << a[j] << "(A,C) :- ";
          if (i < k) {
            outfile << "n" << a[i] << a[k] << "(A,B), ";
          } else {
            outfile << "n" << a[k] << a[i] << "(B,A), ";
          }
          if (j < k) {
            outfile << a[j] << a[k] << "(C,B).";
          } else {
            outfile << a[k] << a[j] << "(B,C).";
          }
          outfile << endl;

          outfile << "n" << a[i] << a[j] << "(A,C) :- ";
          if (i < k) {
            outfile << a[i] << a[k] << "(A,B), ";
          } else {
            outfile << a[k] << a[i] << "(B,A), ";
          }
          if (j < k) {
            outfile << "n" << a[j] << a[k] << "(C,B).";
          } else {
            outfile << "n" << a[k] << a[j] << "(B,C).";
          }
          outfile << endl;
        }
      }
      outfile << endl;
    }
  }

  //This second section is to create rules from knowing one fact or that derive a fact
  for (int i = 0; i < NUM_ATRRIBUTES - 1; i++) {
    for (int j = i + 1; j < NUM_ATRRIBUTES; j++) {
      for (int k = 0; k < NUM_ATTRIBUTE_VALS; k++) {
        for (int l = 0; l < NUM_ATTRIBUTE_VALS; l++) {
          if (variables[k] != variables[l]) {
            outfile << "n" << a[i] << a[j] << "(f," << variables[l] << ") :- ";
            outfile << a[i] << a[j] << "(f," << variables[k] << "), "
                    << "All" << a[j] << "(a,b,c,d)." << endl;
            outfile << "n" << a[i] << a[j] << "(" << variables[l] << ",f) :- ";
            outfile << a[i] << a[j] << "(" << variables[k] << ",f), "
                    << "All" << a[i] << "(a,b,c,d)." << endl;
          }
        }
        outfile << a[i] << a[j] << "(f," << variables[k] << ") :- ";
        for (int l = 0; l < NUM_ATTRIBUTE_VALS; l++) {
          if (variables[k] != variables[l]) {
            outfile << "n" << a[i] << a[j] << "(f," << variables[l] << "), ";
          }
        }
        outfile << "All" << a[j] << "(a,b,c,d)." << endl;

        outfile << a[i] << a[j] << "(" << variables[k] << ",f) :- ";
        for (int l = 0; l < NUM_ATTRIBUTE_VALS; l++) {
          if (variables[k] != variables[l]) {
            outfile << "n" << a[i] << a[j] << "(" << variables[l] << ",f), ";
          }
        }
        outfile << "All" << a[i] << "(a,b,c,d)." << endl;

        outfile << endl;
      }
    }
  }

  do {
    cout << "Any inferential clues?" << endl;
    cin >> clues;
  } while (clues != "no" && clues != "n" && clues != "yes" && clues != "y");

  char type;
  char righta;
  int raind;
  string rightval;
  int rvalindex;
  char lefta;
  int laind;
  string leftval;
  int lvalindex;

  while (clues != "no" && clues != "n") {
    do {
      cout << "Specify what kind:" << endl;
      cout << "Ordering (type o) or proximity (type p)" << endl;
      cin >> type;
    } while (type != 'o' && type != 'p');

    do {
      cout << "Type attribute and value to the right" << endl;
      cout << "Or just one of the values for proximity clue" << endl;
      cout << "Attribute to the right (or first attribute for proximity):"
           << endl;
      cin >> righta;
    } while (!as.count(righta));
    for (int i = 0; i < NUM_ATRRIBUTES; i++) {
      if (a[i] == righta) {
        raind = i;
        break;
      }
    }

    do {
      cout << "Value to the right (or first value for proximity):" << endl;
      cin >> rightval;
    } while (!Aelemset[raind].count(rightval));
    for (int i = 0; i < NUM_ATTRIBUTE_VALS; i++) {
      if (rightval == Aelements[raind][i]) {
        rvalindex = i;
        break;
      }
    }

    do {
      cout << "Type attribute and value to the left" << endl;
      cout << "Or second attribuate and value for proximty clue" << endl;
      cout << "Attribute to the left (or second attribute for proximity):"
           << endl;
      cin >> lefta;
    } while (!as.count(lefta));
    for (int i = 0; i < NUM_ATRRIBUTES; i++) {
      if (a[i] == lefta) {
        laind = i;
        break;
      }
    }

    do {
      cout << "Value to the left (or second value for proximity):" << endl;
      cin >> leftval;
    } while (!Aelemset[laind].count(leftval));
    for (int i = 0; i < NUM_ATTRIBUTE_VALS; i++) {
      if (leftval == Aelements[laind][i]) {
        lvalindex = i;
        break;
      }
    }

    if (type == 'o') {
      do {
        cout << "Immediate(type i) or relative(type r) ordering?" << endl;
        cin >> type;
      } while (type != 'i' && type != 'r');

      if (type == 'r') {
        cout << "Producing Relative Ordering clue." << endl;

        for (int i = 1; i < NUM_ATTRIBUTE_VALS - 1; i++) {
          for (int j = i + 1; j < NUM_ATTRIBUTE_VALS - 1; j++) {
            outfile << "n" << a[0] << lefta << "(" << variables[j] << ","
                    << Variables[lvalindex] << ") :- " << a[0] << righta << "("
                    << variables[i] << "," << rightval << ")"
                    << ", All" << a[0] << "(a,b,c,d,e)"
                    << ", All" << lefta << "(A,B,C,D,E)." << endl;
          
          }
          //TODO: finish relativity rules 
          outfile << "n" << a[0] << lefta << "(" << variables[i] << "," 
                  << Variables[lvalindex] << ") :- ";
          for(int k = i + 1; k < NUM_ATTRIBUTE_VALS; k++){
            outfile << "n" << a[0] << righta << "(" << variables[k] << "," << rightval << "), ";
          }
          outfile << "All" << a[0] << "(a,b,c,d,e)"
                    << ", All" << lefta << "(A,B,C,D,E)." << endl;

        }
        for (int i = 1; i < NUM_ATTRIBUTE_VALS - 1; i++) {
          for (int j = 1; j < i; j++) {
            outfile << "n" << a[0] << righta << "(" << variables[j] << ","
                    << Variables[rvalindex] << ") :- " << a[0] << lefta << "("
                    << variables[i] << "," << leftval << ")"
                    << ", All" << a[0] << "(a,b,c,d,e)"
                    << ", All" << righta << "(A,B,C,D,E)." << endl;
          }
          outfile << "n" << a[0] << righta << "(" << variables[i] << "," 
                  << Variables[rvalindex] << ") :- ";
          for(int k = 0; k < i; k++){
            outfile << "n" << a[0] << lefta << "(" << variables[k] << "," << leftval << "), ";
          }
          outfile << "All" << a[0] << "(a,b,c,d,e)"
                    << ", All" << righta << "(A,B,C,D,E)." << endl;
        }
      } else {
        cout << "Producing Immediate Ordering rule." << endl;
        for (int i = 1; i < NUM_ATTRIBUTE_VALS; i++) {
          outfile << a[0] << lefta << "(" << variables[i - 1] << ","
                  << Variables[lvalindex] << ") :- " << a[0] << righta << "("
                  << variables[i] << "," << rightval << ")"
                  << ", All" << a[0] << "(a,b,c,d,e)"
                  << ", All" << lefta << "(A,B,C,D,E)." << endl;
          outfile << "n" << a[0] << lefta << "(" << variables[i - 1] << ","
                  << Variables[lvalindex] << ") :- n" << a[0] << righta << "("
                  << variables[i] << "," << rightval << ")"
                  << ", All" << a[0] << "(a,b,c,d,e)"
                  << ", All" << lefta << "(A,B,C,D,E)." << endl;

          outfile << a[0] << righta << "(" << variables[i] << ","
                  << Variables[rvalindex] << ") :- " << a[0] << lefta << "("
                  << variables[i - 1] << "," << leftval << ")"
                  << ", All" << a[0] << "(a,b,c,d,e)"
                  << ", All" << righta << "(A,B,C,D,E)." << endl;
          outfile << "n" << a[0] << righta << "(" << variables[i] << ","
                  << Variables[rvalindex] << ") :- n" << a[0] << lefta << "("
                  << variables[i - 1] << "," << leftval << ")"
                  << ", All" << a[0] << "(a,b,c,d,e)"
                  << ", All" << righta << "(A,B,C,D,E)." << endl;
        }
      }
    } else {
      cout << "Producing Proximty rules" << endl;
      outfile << a[0] << lefta << "(" << variables[1] << ","
              << Variables[lvalindex] << ") :- " << a[0] << righta << "("
              << variables[0] << "," << rightval << ")"
              << ", All" << a[0] << "(a,b,c,d,e)"
              << ", All" << lefta << "(A,B,C,D,E)." << endl;
      outfile << a[0] << righta << "(" << variables[1] << ","
              << Variables[rvalindex] << ") :- " << a[0] << lefta << "("
              << variables[0] << "," << leftval << ")"
              << ", All" << a[0] << "(a,b,c,d,e)"
              << ", All" << righta << "(A,B,C,D,E)." << endl;
      outfile << a[0] << lefta << "(" << variables[3] << ","
              << Variables[lvalindex] << ") :- " << a[0] << righta << "("
              << variables[4] << "," << rightval << ")"
              << ", All" << a[0] << "(a,b,c,d,e)"
              << ", All" << lefta << "(A,B,C,D,E)." << endl;
      outfile << a[0] << righta << "(" << variables[3] << ","
              << Variables[rvalindex] << ") :- " << a[0] << lefta << "("
              << variables[4] << "," << leftval << ")"
              << ", All" << a[0] << "(a,b,c,d,e)"
              << ", All" << righta << "(A,B,C,D,E)." << endl;

      outfile << "n" << a[0] << lefta << "(" << variables[0] << ","
              << Variables[lvalindex] << ") :- n" << a[0] << righta << "("
              << variables[1] << "," << rightval << ")"
              << ", All" << a[0] << "(a,b,c,d,e)"
              << ", All" << lefta << "(A,B,C,D,E)." << endl;
      outfile << "n" << a[0] << righta << "(" << variables[0] << ","
              << Variables[rvalindex] << ") :- n" << a[0] << lefta << "("
              << variables[1] << "," << leftval << ")"
              << ", All" << a[0] << "(a,b,c,d,e)"
              << ", All" << righta << "(A,B,C,D,E)." << endl;
      outfile << "n" << a[0] << lefta << "(" << variables[4] << ","
              << Variables[lvalindex] << ") :- n" << a[0] << righta << "("
              << variables[3] << "," << rightval << ")"
              << ", All" << a[0] << "(a,b,c,d,e)"
              << ", All" << lefta << "(A,B,C,D,E)." << endl;
      outfile << "n" << a[0] << righta << "(" << variables[4] << ","
              << Variables[rvalindex] << ") :- n" << a[0] << lefta << "("
              << variables[3] << "," << leftval << ")"
              << ", All" << a[0] << "(a,b,c,d,e)"
              << ", All" << righta << "(A,B,C,D,E)." << endl;

      for (int i = 1; i < variables.size() - 1; i++) {
        for (int j = 0; j < variables.size(); j++) {
          if (abs(i - j) > 1) {
            outfile << "n" << a[0] << lefta << "(" << variables[j] << ","
                    << Variables[lvalindex] << ") :- " << a[0] << righta << "("
                    << variables[i] << "," << rightval << ")"
                    << ", All" << a[0] << "(a,b,c,d,e)"
                    << ", All" << lefta << "(A,B,C,D,E)." << endl;
            outfile << "n" << a[0] << righta << "(" << variables[j] << ","
                    << Variables[rvalindex] << ") :- " << a[0] << lefta << "("
                    << variables[i] << "," << leftval << ")"
                    << ", All" << a[0] << "(a,b,c,d,e)"
                    << ", All" << righta << "(A,B,C,D,E)." << endl;
          } else {
            for (int k = j; k < NUM_ATRRIBUTES; k++) {
              if (abs(i - k) == 1 && k != j && j != i) {
                outfile << "n" << a[0] << lefta << "(" << variables[i] << ","
                        << Variables[lvalindex] << ") :- n" << a[0] << righta
                        << "(" << variables[j] << "," << rightval << ") "
                        << ", n" << a[0] << righta << "(" << variables[k] << ","
                        << rightval << "), All" << a[0] << "(a,b,c,d,e)"
                        << ", All" << lefta << "(A,B,C,D,E)." << endl;

                outfile << "n" << a[0] << righta << "(" << variables[i] << ","
                        << Variables[rvalindex] << ") :- n" << a[0] << lefta
                        << "(" << variables[j] << "," << leftval << "), "
                        << "n" << a[0] << lefta << "(" << variables[k] << ","
                        << leftval << "), All" << a[0] << "(a,b,c,d,e)"
                        << ", All" << righta << "(A,B,C,D,E)." << endl;
              }
            }
          }
        }
      }
    }
    do {
      cout << "Any other inferential clues?" << endl;
      cin >> clues;
    } while (clues != "n" && clues != "no" && clues != "y" && clues != "yes");
  }

  outfile << endl << "Queries:" << endl;
  for (int i = 0; i < NUM_ATRRIBUTES - 1; i++) {
    for (int j = i + 1; j < NUM_ATRRIBUTES; j++) {
      outfile << a[i] << a[j] << "(A,B)?" << endl;
      outfile << "n" << a[i] << a[j] << "(A,B)?" << endl;
    }
  }

  outfile.close();

  return 0;
}
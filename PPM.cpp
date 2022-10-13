#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <functional>
#include <stdlib.h>
#include "PPM.h"

using namespace std;

void PPM::read(string fname) { 
  //open file
  ifstream fin;
  fin.open(fname);
  if (!fin.is_open()) {
    cout << "Error: could not open file " << fname << endl;
    exit(1);
  }
  //read in header info
  char newLine;
  fin >> ID;
  fin >> numCols;
  fin >> numRows;
  fin >> maxVal;
  fin.read(&newLine, 1);
  //error check ID and max val
  if (ID != "P6") {
    cerr << "Error: invalid ID" << endl;
    exit(1);
  }
  if (maxVal != 255) {
    cerr << "Error: invalid maxVal" << endl;
    exit(1);
  }
  // make RGB based off info
  img = new RGB[numRows * numCols];
  //set values of r,g,b 
  fin.read((char *)img, numRows * numCols * 3);
  if (fin.gcount() < numRows * numCols * 3) {
      cerr << "Error: too few bytes in file" << endl;
      exit(1);
  }
  if(fin.read((char *)img, 1)) {
    cerr << "Error: too many bytes in file" << endl;
    exit(1);
  }
  fin.close();
}
void PPM::write(string fname) { 
  //writing to the new file. uses fout this time. 
  string newFname = fname.substr(0, fname.length() - 4) + "_wmsg.ppm";
  ofstream fout;
  fout.open(newFname, ios::trunc);
  if(!fout.is_open()){
    cerr << "Error: could not open file " << newFname << endl;
    exit(1);
  }
  fout << ID << endl;
  fout << numCols << " " << numRows << endl;
  fout << maxVal << endl;
  fout.write((char *)img, numRows * numCols * 3);
  fout.close();
}

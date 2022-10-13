#ifndef __PPM_H__
#define __PPM_H__

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <functional>
#include <stdlib.h>

using namespace std;


struct RGB { 
  //default values
  RGB() {
    R = 0;
    G = 0;
    B = 0;
  }
  unsigned char R, G, B;
};

class PPM { 
  public:
    PPM() {
      ID = "P6";
      numRows = 0;
      numCols = 0;
      maxVal = 255;
      img = NULL;
    }
    //ppm deconstrutor that deletes the img array after use
    ~PPM() {
      if (img != NULL) {
        delete [] img;
      }
    }

	void read(string fname);
	void write(string fname);

	RGB &operator[](int i) {
    return img[i];
  }

	int get_Npixel() {
    return numRows * numCols;
  }

  private:
	//data members
    string ID;
    int numRows;
    int numCols;
    int maxVal;
	  RGB *img;
};

#endif

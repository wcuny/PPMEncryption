#ifndef __RNUMGEN_H__
#define __RNUMGEN_H__

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


class rnumgen {
  public:
    rnumgen(int seedvalue, vector<int> &v);
    int rand();

  private:
    vector<float> F;
    
};

#endif

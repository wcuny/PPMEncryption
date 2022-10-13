//Will Cuny
//wcuny
//10/11/22
//this lab encodes the users message into a ppm image. includes funtionalitly for multiple command line args
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
#include "Rnumgen.h"

using namespace std;

const char EOT = 0x4;	// ASCII: end-of-transmission

void set_pixel_list(int size, vector <int> &v, int startIndex) {
  //fibonacci sequence that adds the number to the index.
  int fib[6] = {1, 1, 2, 3, 5, 8};
  int in = startIndex;
  int fibIndex = 0;
  while (in < size) {
    v.push_back(in);
    in += fib[fibIndex];
    fibIndex++;
    if (fibIndex == 6) {
        fibIndex = 0;
    }
  }
}

void perturb_pixel_list(PPM &img, vector <int> &v, int seed) {
  //make a histgram
  vector <int> histogram(32768, 0);
  int runs = img.get_Npixel();
  //get bits 6-2 of each number
  for (int i = 0; i < runs; i++){
    int r = (img[i].R & 0x7c) >> 2;
    int g = (img[i].G & 0x7c) >> 2;
    int b = (img[i].B & 0x7c) >> 2;
    int rgb = (r << 10) | (g << 5) | b;
    histogram[rgb]++;
  }
  //instance of class
  rnumgen r(seed, histogram);
  //30 bit number making and swapping
  for (int i = 0; i < (int)v.size(); i++) {
    int r1 = r.rand();
    int r2 = r.rand();
    int r_30 = (r1 << 15) | r2;
    int index = r_30 % v.size();
    int temp = v[i];
    v[i] = v[index];
    v[index] = temp;
  }
}

void encode(PPM &img, vector <int> &v) {
  char letter;
  int next = 0;
  while(cin.get(letter)){
    //error checking
    if (next >= (int)v.size()){
      cerr << "Error: not enough space to encode message" << endl;
      exit(1);
    }
    //starts from bit 6 and goes down, alternating between r,g, and b
    for (int j = 6; j >= 0; j--) {
      int bit = (letter >> j) & 1;
      int pixel = v[next];
      if ((next) % 3 == 0) {
        img[pixel].R &= 0xFE;
        img[pixel].R |= (bit);
      }
      else if ((next) % 3 == 1) {
        img[pixel].G &= 0xFE;
        img[pixel].G |= (bit);
      }
      else {
        img[pixel].B &= 0xFE;
        img[pixel].B |= (bit);
      }
      next++;
    }
  }
  //encoding the EOT using the same methods
  for (int j = 6; j >= 0; j--) {
    int bit = (EOT >> j) & 1;
    int pixel = v[next];
    if ((next) % 3 == 0) {
      img[pixel].R &= 0xFE;
      img[pixel].R |= (bit);
    }
    else if ((next) % 3 == 1) {
      img[pixel].G &= 0xFE;
      img[pixel].G |= (bit);
    }
    else {
      img[pixel].B &= 0xFE;
      img[pixel].B |= (bit);
    }
    next++;
  }
}

void decode(PPM &img, vector <int> &v) {
  string msg;
  int msgSize = v.size() / 3;
  int count = 0;
  //going through each char and decoding 
  for (int i = 0; i < msgSize; i++) {
    char c = 0;
    for (int j = 6; j >= 0; j--) {
      int pixel = v[count];
      if (count % 3 == 0) {
        c |= (img[pixel].R & 0x1) << j;
      }
      else if (count % 3 == 1) {
        c |= (img[pixel].G & 0x1) << j;
      }
      else {
        c |= (img[pixel].B & 0x1) << j;
      }
      count++;
    }
    if (c == EOT) {
      break;
    }
    msg += c;
  }
  cout << msg << endl;
}

int main(int argc, char *argv[]) {
  //parse command line arguments
  if (argc < 3 || argc > 5) {
    cerr << "Usage: " << argv[0] << " -encode|decode [-option=value] image.ppm" << endl;
    return 1;
  }
  PPM img;
  string fname = argv[argc - 1];
  string mode = argv[1];

  img.read(fname);

  //declare pixel_list
  vector <int> pixel_list;

  //default first pixel index
  int StartP = 0;
  int seed = 0;
  bool perturb = false;
  //looping through command line and parsing to correct varibles
  for (int i = 2; i < argc - 1; i++) {
    string option = argv[i];
    if (option.find("-seed=") != string::npos) {
      seed = atoi(option.substr(6).c_str());
      perturb = true;
    }
    else if (option.find("-m=") != string::npos) {
      StartP = atoi(option.substr(3).c_str());
    }
    else {
      cerr << "Usage: " << argv[0] << " -encode|decode [-option=value] image.ppm" << endl;
      return 1;
    }
  }
  int size = img.get_Npixel();
  set_pixel_list(size, pixel_list, StartP);

  if(perturb == true){
    perturb_pixel_list(img, pixel_list, seed);
  }
  if (mode == "-encode") {
    encode(img, pixel_list);
	  img.write(fname);
  }
  else if (mode == "-decode"){
    decode(img, pixel_list);
  }
  return 0;
  
}


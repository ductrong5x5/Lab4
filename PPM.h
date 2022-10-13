/*  Author: Duc Trong Nguyen
    Net ID: dnguye38
    Description: This file define class PPM and struct RGB
*/
#ifndef __PPM_H__
#define __PPM_H__

#include <fstream>
#include <iostream>
//struct of a pixel
struct RGB { 
  RGB() { R=0;G=0;B=0;} //default constructor
  unsigned char R, G, B;
};

class PPM { 
  public:
    PPM() { ncols=0;nrows=0;maxvalue=0;fmat=""; (img);} //default constructor
    ~PPM() { delete [] img; }   //destructor
	void read(string);            //read file
	void write(string);           //write file
	RGB &operator[](int i) { return img[i]; } //get pixel it i
	int get_Npixel() { return ncols*nrows; }  //get total pixel

  private:
	string fmat;              //format of file
  int ncols,nrows;          //size of file
  int maxvalue;             //max value of file
	RGB *img;                 //pixel
};

#endif

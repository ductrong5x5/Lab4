/*  Author: Duc Trong Nguyen
    Net ID: dnguye38
    Description: This file declare read and write function.
*/
using namespace std;
#include "PPM.h"
//read file
void PPM::read(string fname) { 
  string fmat;
  int ncols,nrows;
  int maxvalue;
  ifstream infile;
  int sizebyte;
  //open file
  infile.open(fname.c_str(),ifstream::binary);
  if(infile.is_open()){   //if file is opened
    infile>>fmat>>ncols>>nrows>>maxvalue; //take data
    if(!infile.eof()){    //check eof
      if(fmat!="P6"){     //check format
        cout<<"Error: Not P6 format type\n";
        exit(1);
      }
      if(maxvalue!=255){  //check maxvalue
        cout<<"Error: Max value not correct\n";
        exit(1);
      }
      this->fmat=fmat;
      this->ncols=ncols;
      this->nrows=nrows;
      this->maxvalue=maxvalue;
      sizebyte=ncols*nrows*3;  
      img=new RGB[sizebyte/3];
      infile.ignore();
      infile.read((char*)img,sizebyte); //read pixel
      if(infile.gcount()!=sizebyte){     //check total pixel in file 
        cout<<"Error: Not correct number of bytes read\n";
        exit(1);
      }
    }
    infile.close();     
  }
  else{     //if file can not be opened
    cout<<"Error: Unable to open "<< fname<<". Please try again\n";
    exit(1);
  }
}

//write data to a file
void PPM::write(string fname) { 
  ofstream outfile;
  string wmsg="_wmsg";
  int size=fname.length();
  fname.insert(size-4,wmsg); //insert _wmsg to file name
  outfile.open(fname.c_str(),ofstream::binary); //create file with corrected name
  if(outfile.is_open()){     //check if file can be opened, then write data to file
    outfile<<fmat<<"\n";
    outfile<<ncols<<" "<<nrows<<"\n";
    outfile<<maxvalue<<endl;
    int sizebyte=ncols*nrows*3;
    outfile.write((char*)img,sizebyte);
    outfile.close();
  }
  else{
    cout<<"Error: Unable to open "<< fname<<". Please try again\n";
    exit(1);
  }
}

/*  Author: Duc Trong Nguyen
    Net ID: dnguye38
    Description: This program will read data of a picture and then encoding/decoding message inside. If user want to make the pixel list more complicated, Fibonacci sequence,
    or random number will be generated.
*/
#include <cstring>
using namespace std;

#include "PPM.h"
#include "Rnumgen.h"
// EOT : 0000 0100
const char EOT = 0x4;	// ASCII: end-of-transmission

void set_pixel_list(PPM &p,vector<int> &A,int &m) { //set pixel list for encoding/decoding
  int size=p.get_Npixel();          //Size of image (total pixel)
  A.resize(size);                   //Make size of the pixel list equal to size of image
  A[0]=m;                           // -m=number in argument comandline is the first element in pixel list
  int x=0;                           
  int newsize=1;                    //The first element is put in, so size =1
  int Fibonacci [6]={1,1,2,3,5,8};  //declare the Fibonacci sequence
  for(int i=1;i<size;i++){              
    A[i]=A[i-1]+Fibonacci[x];       //change value of element inside pixel list, the next = before + fibonacci element
    x++;                            //increment index of fibonacci
    newsize++;                      //increment size
    if(x==6)                      
      x=0;                          //go back to first elemnt of Fibonacci sequence
    if(A[i]>size){                  //if the value of pixel list larger than biggest pixel value, stop the loop
      newsize=i-1;                  //new size will decrease 1
      break;
    }
  }
  A.resize(newsize);                //change size of pixel list
}

//Create pixel list in random order
void perturb_pixel_list(PPM &p,vector<int> &B,int &seed) {
  int size= B.size();               //
  vector<int> A;                    //create list of all 15bit number
  A.resize(32768);                  //size of this list
  for(int i=0;i<size;i++){          //create a 15bit number based on RGB 
    unsigned int t=0x0;           
    unsigned int r,g,b;
    r = p[i].R >>2;                 
    g = p[i].G >>2;
    b = p[i].B >>2;
    r &= 0x1F;                      //0x1F: 0001 1111
    g &= 0x1F;
    b &= 0x1F;
    t |= r;
    t = t << 5;
    t |= g;
    t = t << 5;
    t |= b;
    A[t]++;                         //increase number a 15bit number occur
  }
  rnumgen RNG(seed, A);             //create probability vector of 15bit number list with seed 
  RNG.randperm(B);                  //Do permutation in pixel with random.
}
//encoding message
void encode(PPM &p,vector<int> &A,string text) {
  string message;       
  getline(cin,message);               //type in message to encode
  int vi=0;                           //number to swap RGB
  int length=message.length();        
  int mit;                            //number to swap character in -key, will use to xor EOT
  char f,tempEOT;                     //f is each character of -key, tempEOT is EOT after xor or not.
  int q=text.length();                
  if(q!=0){                           //if -key is not empty, do xor
    for(int i = 0; i < length; i++){  
      char x=message.at(i);            
      char y=text.at(i%text.length());
      x= x^y;                         //xor each charactor of message to each charactor of -key
      mit=i;                          // increase value of mit
      message.at(i)=x;
    }
    mit++;                            // go to next char of -key
    mit=mit%text.length();            //find index of char in -key
    f=text.at(mit);                   
    tempEOT=EOT^f;                    //xor EOT with that character
  }
  for(int i = 0; i < length; i++){    
    char x=message.at(i);
    for(unsigned int bitPos = 0; bitPos < 7; bitPos++){
      unsigned char y=x>>bitPos;
      y=y&0x1;              //0x1: 0000 0001, keep LSB of each character
      if(vi%3==0){          //swap to R
        p[A[vi]].R &= 0xFE; //0xFE=1, make LSB to 0
        p[A[vi]].R |= y;    //change LSB
      }
      else if(vi%3==1){     //swap to G
        p[A[vi]].G &= 0xFE; 
        p[A[vi]].G |= y;
      }
      else if(vi%3==2){     //swap to B
        p[A[vi]].B &= 0xFE;
        p[A[vi]].B |= y;
      }
      vi++;                 //increase number to swap RGB
    }
  }
  
  for(unsigned int bitPos = 0; bitPos < 7; bitPos++){
    if(q==0){        //if -key is empty -> not encoding EOT
    tempEOT=0x4;     //tempEOT = EOT = 0x4 : 0000 0100
    }
    //encoding each bit in EOT, same as each character in message
    unsigned char y=tempEOT>>bitPos;    
    y=y&0x1;                
    if(vi %3==0){
        p[A[vi]].R &= 0xFE;
        p[A[vi]].R |= y;
      }
      else if(vi %3==1){
        p[A[vi]].G &= 0xFE;
        p[A[vi]].G |= y;
      }
      else if(vi %3==2){
        p[A[vi]].B &= 0xFE;
        p[A[vi]].B |= y;
      }
    vi++;
  }
  // if the message is too long, print error.
  // Still encoding available message, but not the rest
  if(vi>p.get_Npixel())     
    cout<<"Error message is too long to be encoded"<<endl;
}
//decoding message
void decode(PPM &p,vector<int> &A,string text) {
  char c=0x0;                //decoded character
  int bitpos=0;              
  int rev;                   //positon of decoded bit in decoded charactor
  int size=A.size();        
  int length = text.length();
  int m=0;                  
  for(int i=0;i<size;i++){   //traverse through pixel list
      rev=bitpos%7;          //find position of decoded bit in decode char
      if(i %3 == 0){         //swap RGB
          p[A[i]].R &= 0x1;  //make all bit to 0, except LSB
          p[A[i]].R = p[A[i]].R<< rev;  //shift to its proper position in character
          c=p[A[i]].R|c;     // put it in character
      }
      else if(i %3 == 1){    //swap RGB
        p[A[i]].G &= 0x1;    
        p[A[i]].G = p[A[i]].G<< rev;
        c=p[A[i]].G|c;
      }
      else if(i %3 == 2){    //swap RGB
        p[A[i]].B &= 0x1;
        p[A[i]].B = p[A[i]].B<< rev;
        c=p[A[i]].B|c;
      }
      bitpos++;               //increase bit position
      if(rev==6){             //when all bit of character is decoded
        if(length!=0){        //if -key is not empty, do do xor to get real character
          int y=m%length;
          c=c^text.at(y);
          m++;                
        }
        cout<<c;              //print decoded charactor
        if(c==EOT )           //if decoded character is EOT, stop.
          break;
        c=0x0;                //after decoding character, set is back to 0000 0000 to decode another character
      }
  }
  cout<<endl;
}

int main(int argc, char *argv[]) {
  //Usage for command line argument
  if(argc != 5 ){
    cerr << "usage: " << argv[0]
    << " -encode|-decode  -m=N|-seed=N  -key=\"text\" file.ppm\n";
    return 0;
  }
  vector<int>A;   
  PPM img;
  
  int morseed = strtol(argv[2]+3, NULL, 10); //get number from argv, which is number for -m
  string xortext;
  //find length of -key xor
  int z=strlen(argv[3])+2;
  //if length is 7, which is -key="", key empty
  if(z==7)
    xortext="";
  //
  else{   //get token, which is text for key
  xortext=strtok (argv[3],"=");
  xortext=strtok (NULL,"\"");  
  }
  img.read(argv[argc-1]);   //open and read file
  set_pixel_list(img,A,morseed);  //create pixel list
  if(argv[2][1]=='s'){      //if user use -seed
    morseed = strtol(argv[2]+6, NULL, 10); //get number for seed from -seed=
    perturb_pixel_list(img,A,morseed); //create perturb pixel list
  }
  if(strcmp(argv[1],"-encode" )==0){  //if mode is encode
    encode(img,A,xortext);          //encode
    img.write(argv[argc-1]);        //write to file _wmsg
  }
  if(strcmp(argv[1],"-decode" )==0)    //if mode is decode 
    decode(img,A,xortext);    
   return 0;
}


#include<iostream>
#include<cstdio>
#include<cstdlib>

#include"EasyBMP.h"
#include"Hide.h"
#include"Unhide.h"

using namespace std;


char ext[4];              // extention of encrypted file i.e txt, png etc
unsigned int size;        // size of output file
char sizeInChar[4];       // size of output file in characters
unsigned int height;      // height of image
unsigned int width;       // width  of image
char verification[] = {'s','t','e','g','o','\0'};
char verify[6];
BMP Image;                // image pointing to cover image
char stegoFile[50];
char *message;
char *outputFile;
const char *pwd;
int pwdlen;

/** It returns a character embedded at i,j to i+1,j+1 pos in image pixels
*/
char extractChar(int i, int j)
{
     
     RGBApixel pixel1 = *Image(i,j);
     i++; 
     if( i == Image.TellWidth() ){ i=0; j++; }
       
     RGBApixel pixel2 = *Image(i,j);
    
     // convert the two pixels to a character
       
     unsigned int T = 0;
          
     T = T | pixel1.Red % 2;
     T = T << 1;
     T = T | pixel1.Green % 2;
     T = T << 1;
     T = T | pixel1.Blue  % 2;
     T = T << 1;
     T = T | pixel1.Alpha  % 2;
     T = T << 1;
     
     T = T | pixel2.Red  % 2;
     T = T << 1;
     T = T | pixel2.Green % 2;
     T = T << 1;
     T = T | pixel2.Blue % 2;
     T = T << 1;
     T = T | pixel2.Alpha % 2;
     
     char c = (char) T;
     
     return c;
}

/** It embeds character c after encrypting in image at i, j 
*/
void embedChar(char c,int i, int j){
    RGBApixel pixel1, pixel2; // pixels having one character
 
    
     // Get Two Pixels to embed
     int ti=i, tj=j;
     
     pixel1 = *Image(i,j);
     i++; 
     if( i == width ){ i=0; j++; }
               
     pixel2 = *Image(i,j);    
     
     
     unsigned short int mask = 0x80;   // mask to get bit
     
     // PIXEL 1
     if( c & mask )
         pixel1.Red = pixel1.Red | 1;          // make 0th bit 1    
     else
         pixel1.Red = pixel1.Red & 0xfe;       // make 0th bit 0
     
     mask = mask >> 1;    
     
     if( c & mask )
         pixel1.Green = pixel1.Green | 1;      // make 0th bit 1
     else
         pixel1.Green = pixel1.Green & 0xfe;   // make 0th bit 0       
      
     mask = mask >> 1;    
     
     if( c & mask )
         pixel1.Blue = pixel1.Blue | 1;        // make 0th bit 1
     else
         pixel1.Blue = pixel1.Blue & 0xfe;     // make 0th bit 0     
 
     mask = mask >> 1;    
 
     if( c & mask )
         pixel1.Alpha = pixel1.Alpha | 1;      // make 0th bit 1
     else
         pixel1.Alpha = pixel1.Alpha & 0xfe;   // make 0th bit 0       
     
     mask = mask >> 1;    
     //   pixel 2
     if( c & mask )
         pixel2.Red = pixel2.Red | 1;          // make 0th bit 1
     else
         pixel2.Red = pixel2.Red & 0xfe;       // make 0th bit 0
      
     mask = mask >> 1;    

     if( c & mask )
         pixel2.Green = pixel2.Green | 1;      // make 0th bit 1
     else
         pixel2.Green = pixel2.Green & 0xfe;   // make 0th bit 0       
         
     mask = mask >> 1;    

     if( c & mask )
         pixel2.Blue = pixel2.Blue | 1;        // make 0th bit 1
     else
         pixel2.Blue = pixel2.Blue & 0xfe;     // make 0th bit 0     

     mask = mask >> 1;    

     if( c & mask )
         pixel2.Alpha = pixel2.Alpha | 1;      // make 0th bit 1
     else
         pixel2.Alpha = pixel2.Alpha & 0xfe;   // make 0th bit 0       
     
     *Image(ti,tj) = pixel1;                   // updating Image Pixels
     *Image(i,j)   = pixel2;     
         
}         
         
// Checking weather hidden file has got extention
int hasExtention(const char *f){
	for(int i=0; i<strlen(f); i++){
		if(f[i]=='.')
			return 1;
	}
	return 0;
}


         // Hiding    
JNIEXPORT jint JNICALL Java_Hide_hideInBMP
  (JNIEnv *env, jclass jc, jstring secretFileName, jstring coverFileName, jstring dstPath, jstring password){
          
      const char* hiddenFile;
      const char* coverFile;
      const char* stegoFile;
      const char* dstPathName;
      const char* msg;
      char c;                   // character to be embedded
      int i=0, j=0;             // pixel handling variables
      FILE* fp;                 // file pointer to hidden file   
      
      jboolean tr = (char)1;
      
      hiddenFile = env->GetStringUTFChars( secretFileName, &tr );
               
      coverFile  = env->GetStringUTFChars( coverFileName, &tr );
      dstPathName = env->GetStringUTFChars( dstPath, &tr);
      pwd = env->GetStringUTFChars(password,&tr);        
      pwdlen = strlen(pwd);        
      cout<<"Entered";
      
     				// to hide was a file
		  if(hasExtention(hiddenFile)){
				char *q = strchr(hiddenFile, '.');
				
				ext[0] = ((*(q+1) != '\0') ? *(q+1) : 0) ;
				ext[1] = ((*(q+2) != '\0') ? *(q+2) : 0) ;
				ext[2] = ((*(q+3) != '\0') ? *(q+3) : 0) ;
				ext[3] = ((*(q+4) != '\0') ? *(q+4) : 0) ;
		  }		
		  else{
			   ext[0] = 'y';
			   ext[1] = 'y';
			   ext[2] = 'y';
			   ext[3] = 'y';
		  }

		  // open hidden file             
      
          fp = fopen( hiddenFile , "rb" );
          if( !fp ){
              cout << "Error: unable to read file " << hiddenFile << " for text input!" << endl;
              return 1; 
          }
           
     
       
      
      // determine size of hidden file
      
           size = 0;
           while( !feof( fp ) ){
                  char c;
                  fread( &c, 1, 1, fp );
                  size++;
           }
           size--;
           fseek(fp,0,SEEK_SET);   // restore to starting position
      
      
      // open cover image 
      if(!(Image.ReadFromFile( coverFile )))
                               return 2;	// failed to open cover file
        
      width  = Image.TellWidth();
      height = Image.TellHeight();
      int MaxNumberOfPixels = width * height - 2;

      // set image to 32 bpp
      Image.SetBitDepth( 32 );
      
      if( 2 * size+16+10 > MaxNumberOfPixels ){
           cout<<"Error : hidden file exceeds encoding capacity of the image\n";
           return 3;
      }
      
      // converting size into character to be embedded
      unsigned int tempSize = size;
      sizeInChar[3] = (unsigned char) (tempSize & 0x000000ff);

      tempSize = tempSize>>8;
      sizeInChar[2] =  (unsigned char)(tempSize & 0x000000ff) ;
      
      tempSize = tempSize>>8;
      sizeInChar[1] = (unsigned char)(tempSize & 0x000000ff);
      
      tempSize = tempSize>>8;
      sizeInChar[0] =  (unsigned char)(tempSize & 0x000000ff);
      
      /* EMBEDDING THE CONTENT TO COVER MESSAGE */
      srand(15243);
      i = 0;
      j = 0;
	  int p;
      // insert verification characters
      for( p=0; p<5; p++){
              c = verification[p];
			  c = c ^ pwd[rand()%pwdlen];
              embedChar(c,i,j);
              
              i++;                              // character embed into two pixels
              if(i == width) { i = 0; j++; }
              i++;
              if(i == width) { i = 0; j++; }
      }
      
     // insert extention of the hidden file
      for( p=0; p<4; p++){
              c = ext[p];
              embedChar(c,i,j);
              
              i++;                              // character embed into two pixels
              if(i == width) { i = 0; j++; }
              i++;
              if(i == width) { i = 0; j++; }
      }            
           
      // insert size of the hidden file
      for(p=0; p<4; p++){
              c = sizeInChar[p];
              embedChar(c,i,j);         
              
              i++;                              // character embeed into two pixels
              if(i == width) { i = 0; j++; }
              i++;
              if(i == width) { i = 0; j++; }
      }
      
      for( p=0; p<size; p++){
              
              fread( &c, 1, 1, fp );
              embedChar(c,i,j);
              
              i++;                              // character embeed into two pixels
              if(i == width) { i = 0; j++; }
              i++;
              if(i == width) { i = 0; j++; }
      }
      
      char outputFile[50];
      strcpy(outputFile, dstPathName);
      strcat(outputFile, "/steg.bmp" );
      if( ! (Image.WriteToFile(outputFile)))
          return 5;
      cout<<"Hidden File is successfully Encoded into cover Image and outputed to steg.bmp";           
       
      return 4;
}                
      
      
 //UNHIDING
 
JNIEXPORT jint JNICALL Java_Unhide_unhideInBMP
  (JNIEnv *env, jclass jc, jstring coverFileName , jstring dstPath,jstring password){
      
      const char* coverFile;
      const char* dstPathName;
      int i,j;
      char c;
      FILE *fp;
      
      jboolean tr = (char)1;	// convert to char[] in C from String in Java
      
      coverFile = env->GetStringUTFChars( coverFileName, &tr );
      dstPathName = env->GetStringUTFChars( dstPath, &tr);
      pwd = env->GetStringUTFChars( password, &tr);
      pwdlen = strlen(pwd);
      srand(15243);
      
         
      if( !(Image.ReadFromFile(coverFile)))
          return 1;				// failed to open image

      width = Image.TellWidth();
      height = Image.TellHeight();
      
      i = 0;
      j = 0;
      // verifying weather image contain hidden data or valid password
      int m;
      for(m=0; m<5; m++){
              c = extractChar(i,j);
			  verify[m] = c ^ pwd[rand()%pwdlen];
              
              i++;                              // character extracted from two pixels
              if(i == width) { i = 0; j++; }
              i++;
              if(i == width) { i = 0; j++; }
      }
      verify[m] = '\0';
      
      if(strcmp(verification,verify) != 0){
		      cout<<verify<<endl;
              cout<<"Error: Please Verify the password or Image has been encrypted by StegBMP\n";
              return 0;
      }      
      
      // extract extention for output file    
      bool flag = true;
      int p;
      for(p=0; p<4; p++){
              c = extractChar(i,j);
              if(!c)
                    flag = false;
              if(flag) ext[p] = c; 
              
              i++;                              // character extracted from two pixels
              if(i == width) { i = 0; j++; }
              i++;
              if(i == width) { i = 0; j++; }
      }    
      
	  // checking weather hidden file or message
	  int isFile = 0;
	  for(p=0; p<4; p++){
		  if(ext[p]!='x'){
			  isFile = 1;
			  break;
		  }
	  }
	  int hasExt = 0;	// hidden file has no extention
	  for(p=0; p<4; p++){
		  if(ext[p]!='y'){
			  hasExt = 1;	// hidden file has extention
			  break;
		  }
	  }

            
	  cout<<"\n\nHidden was ";
	  if(isFile)
			cout<<"file";
	  else
			cout<<"Message";

      unsigned int i1,i2,i3,i4;
      // extract size of hidden file
      for( p=0; p<4; p++){
              sizeInChar[p] = extractChar(i,j);
              
              i++;                              // character extracted from two pixels
              if(i == width) { i = 0; j++; }
              i++;
              if(i == width) { i = 0; j++; }
      }
              
      
      
      i1 = sizeInChar[0];
      i2 = sizeInChar[1];
      i3 = sizeInChar[2];
      i4 = sizeInChar[3];
      
      if(i1 >= 0xff00)
                       i1 = i1& 0x000000ff;
      if(i2 >= 0xff00)
                       i2 = i2& 0x000000ff;
      if(i3 >= 0xff00)
                       i3 = i3& 0x000000ff;
      if(i4 >= 0xff00)
                       i4 = i4& 0x000000ff;
      
      size = i1;
      size = size << 8;
      size = size | i2;
      size = size << 8;
      size = size | i3;
      size = size << 8;
      size = size | i4;

	  cout<<"\nSize : "<<size;
      
	  //creating output filename
      if(isFile){
		  int t;
		  if(!hasExt){						// hidden file has not extention
			  strcpy(stegoFile,"hidden");
			  t = strlen(stegoFile);
          }
		  else{
			strcpy(stegoFile , "hidden.");
			t=strlen(stegoFile);
			for(int q=0; q < 4; t++,q++){
				if(ext[q]==0)
                           break;
				stegoFile[t] = ext[q];                      
			}
		  }
		  stegoFile[t] = '\0';
		  outputFile = new char[strlen(dstPathName)+15];
		  strcpy(outputFile, dstPathName);
		  strcat(outputFile, "/");
		  strcat(outputFile, stegoFile);
		  if((fp = fopen(outputFile,"wb")) == NULL)
			  return 3;							// report invalid destination
	  }
	  else{
		  cout<<"\n creating space for message\n";
		  message = new char[size+1];
		  cout<<"\n space created\n";
	  }


      for(p=0; p<size; p++){
              c = extractChar(i,j);
              if(isFile)
				  fwrite(&c,1,1,fp);
			  else{
				  cout<<c;
				  message[p] = c;
              }
              
              i++;                              // character extracted from two pixels
              if(i == width) { i = 0; j++; }
              i++;
              if(i == width) { i = 0; j++; }
      }
      
      
	  if(isFile){
		  fclose(fp);
		  cout<<"Hidden File is retreived Successfully to stego.";
		  for( p=0; p<4; p++)
              if(ext[p])
                        cout<<ext[p];    
      
          return 2;
	  }
	  else{
		  cout<<"showing message\n";
		  message[size]='\0';
		  cout<<"Hidden Message retreived Successfully\n"
			  <<"-----------------------------------------------------\n\n"
			  <<message<<endl;
		  return 4;
	  }
}


JNIEXPORT jstring JNICALL Java_Unhide_getHiddenFileNameInBMP
	(JNIEnv *env, jclass jc){
		
		return env->NewStringUTF(outputFile);
}



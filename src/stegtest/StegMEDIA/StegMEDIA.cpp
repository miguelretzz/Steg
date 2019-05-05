
#include<iostream>
#include<stdio.h>
#include<string.h>
#include<conio.h>

#include"Hide.h"
#include"Unhide.h"

char ext[4];
unsigned int size;
char sizeInChar[4];
char *message;
char verification[] = "stego";
char verify[6];
char stegoFile[20];
char *outputFileName;
const char *pwd;
int pwdlen;

// Checking weather hidden file has got extention
int hasExtention(const char *f){
	for(int i=0; i<strlen(f); i++){
		if(f[i]=='.')
			return 1;
	}
	return 0;
}

void getExtension(const char *fileName){
     
     if(hasExtention(fileName)){
         char *q = strchr(fileName, '.');
			
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
}

// get size of the file into BCD Representation
void readSizeInChar(){
     // converting size into character to be embedded
     unsigned int tempSize;
     
     tempSize = size;
     sizeInChar[3] = (unsigned char) (tempSize & 0x000000ff);

     tempSize = tempSize>>8;
     sizeInChar[2] =  (unsigned char)(tempSize & 0x000000ff) ;
      
     tempSize = tempSize>>8;
     sizeInChar[1] = (unsigned char)(tempSize & 0x000000ff);
      
     tempSize = tempSize>>8;
     sizeInChar[0] =  (unsigned char)(tempSize & 0x000000ff);
}
     
void getSize(const char *secretFileName){
     FILE *fp = fopen(secretFileName,"rb");
     char c;
     
     
     size = 0;
     while( !feof( fp ) ){
                  c = fgetc(fp);
                  size++;
     }
     size--;
     
     fclose(fp);
}
  
char* embedChar(char *buffer, char c){
     int k;
     int mask = 0x80;
     char *buf = new char[8];
     unsigned short int data=(unsigned short int) c;
     
  //   printf("data: %d\n",data);
     for(k=0; k<8; k++){
          //    printf("buffer: %c, bit: %d\n",buffer[k],data & mask);
               if( data & mask )
                    buf[k] =  buffer[k] | 1 ;
               else
                    buf[k] = buffer[k] & 0xfe;
           //    printf("buf: %c\n",buf[k]);
               mask = mask>>1;
     }
  //   printf("%s\n",buf);
     
     return buf;
}

char extractChar(char *buffer){
     int data=0;
     int mask=1;
     int k=0;
     char c;
     
     for(k=0; k<8; k++){
           //   printf("buf: %c bit: %d data: %d\n",buf[k], buf[k] & mask,data);
              data = data << 1;
              
              if(buffer[k] & mask)
                        data = data | 1;
              
              
     }
     c=(char) data;
     //printf("data: %d %c %d\n ",data, s,s);
     return c;     
}
      
//int hide(char *secretFileName, char *mediaFileName, char *destinationPath, int isFile){
JNIEXPORT jint JNICALL Java_Hide_hideInMEDIA
  (JNIEnv *env, jclass jc, jstring SecretFileName, jstring coverFileName, jstring dstPath, jstring password){
    FILE *secretFile, *mediaFile, *outFile;
    char outputFileName[100];
    char readBuffer[8], *writeBuffer, c;
    char header[100],buf[1024];
    int bytesRead = 0, bytesWritten = 0; 
    int i;

	const char* secretFileName;
    const char* mediaFileName;
    const char* destinationPath;
	const char* msg;
    
	// transform jstring to char*
	jboolean tr = (char)1;
      
   
    secretFileName = env->GetStringUTFChars( SecretFileName, &tr );
             
    mediaFileName  = env->GetStringUTFChars( coverFileName, &tr );
    destinationPath = env->GetStringUTFChars( dstPath, &tr);

    pwd = env->GetStringUTFChars( password, &tr);
	pwdlen = strlen(pwd);

    // creat destination file name
    strcpy(outputFileName, destinationPath);
    strcat(outputFileName, "/steg" );    
	strcat(outputFileName, strchr(mediaFileName,'.'));
    srand(15243);
    // open files
    // todo - add error opening handlers
    
       secretFile = fopen(secretFileName, "rb");
       if(!secretFile){
      //   cout<<"Error:File "<<secretFileName<<" cannot be opened";
         return 1;
       }
   
    
        
    mediaFile = fopen(mediaFileName, "rb");
    if(!mediaFile){
      //   cout<<"Error:File "<<mediaFileName<<" cannot be opened";
         return 2;
    }
    
    outFile = fopen(outputFileName, "wb");
    if(!outFile)
    {
	   printf("Error:File not opened %s\n",outputFileName);
       return 5;
    }
    // copy 44 byte header + 54 bytes of extension space
    bytesRead = fread(header, 100, 1, mediaFile);
    bytesWritten += fwrite(header, 100, 1, outFile);
    
   
          getExtension(secretFileName);
          getSize(secretFileName);
          readSizeInChar();
     
   
    // embed verification
    for(i=0; i<5; i++){
             bytesRead = fread(readBuffer, 8, 1, mediaFile);
             writeBuffer = embedChar(readBuffer, verification[i] ^ pwd[rand()%pwdlen]);
             bytesWritten += fwrite(writeBuffer, 8, 1, outFile);
    }
    
    // embed extension
    for(i=0; i<4; i++){
             bytesRead = fread(readBuffer, 8, 1, mediaFile);
             writeBuffer = embedChar(readBuffer, ext[i]);
             bytesWritten += fwrite(writeBuffer, 8, 1, outFile);
    }
    
    // embed size
    for(i=0; i<4; i++){
             bytesRead = fread(readBuffer, 8, 1, mediaFile);
             writeBuffer = embedChar(readBuffer, sizeInChar[i]);
             bytesWritten += fwrite(writeBuffer, 8, 1, outFile);
    }
    
    // embed secret file
    for( i=0; i<size; i++){
              c = fgetc(secretFile);
              bytesRead = fread(readBuffer,8, 1, mediaFile);
              writeBuffer = embedChar(readBuffer, c);
              bytesWritten += fwrite(writeBuffer, 8, 1, outFile);
    }
    
    // embed actual media data   
    while(!feof(mediaFile)){
          bytesRead = fread(buf,1024, 1, mediaFile);
		  bytesWritten += fwrite(buf, 1024, 1, outFile);
    }
    
    
    // close opened files
    fclose(secretFile);
    fclose(mediaFile);
    fclose(outFile);
    
    return 4; // success
} 

JNIEXPORT jint JNICALL Java_Unhide_unhideInMEDIA
  (JNIEnv *env, jclass jc, jstring coverFileName, jstring DestinationPath,jstring password){
    FILE *mediaFile, *outFile;
    
    char readBuffer[8], c;
    int bytesRead=0;
    int i,k;
    unsigned int i1,i2,i3,i4;
    int isFile = 0, hasExt = 0;

    const char *mediaFileName, *destinationPath;
	jboolean tr = (char)1;

	mediaFileName  = env->GetStringUTFChars( coverFileName, &tr );
    destinationPath = env->GetStringUTFChars( DestinationPath, &tr);
	pwd = env->GetStringUTFChars( password, &tr);
	pwdlen = strlen(pwd);
    srand(15243);
    // open stego file
    // todo - error handling
    mediaFile = fopen(mediaFileName,"rb");
    if(!mediaFile)
    {
    //   cout<<"Error:mediae file "<<mediaFile<<"not opened";
       return 1;
    }
    
    // skip 100 bytes (header + extention space)
    fseek(mediaFile, 100, SEEK_SET);
    
    // extract verification
    for(i=0; i<5; i++){
             bytesRead += fread(readBuffer, 8, 1, mediaFile);
             c = extractChar(readBuffer);
        //     printf("%c",c);
             verify[i] = c ^ pwd[rand()%pwdlen];
    }
    verify[i] = '\0';
    
    if(strcmp(verification, verify)!=0)
                            return 0;
    
    // extract extension
    for(i=0; i<4; i++){
             bytesRead += fread(readBuffer, 8, 1, mediaFile);
             c = extractChar(readBuffer);
             printf("%c",c);
             ext[i] = c;
    }
    
    // parse extension
    
	// check if message or file
    for(i=0; i<4; i++){
		  if(ext[i]!='x'){
			  isFile = 1;
			  break;
		  }
	}
	// check if file has extention or not
	for(i=0; i<4; i++){
	   if(ext[i]!='y'){
		  hasExt = 1;	// hidden file has extention
		  break;
	  }
	}
	
			
	// extract size in BCD Representation
	for(i=0; i<4; i++){
             bytesRead += fread(readBuffer, 8, 1, mediaFile);
             sizeInChar[i] = extractChar(readBuffer);
    }
    // transform BCD to Decimal size
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

	printf("Size: %d\n",size);
	
	//creating output filename
    if(isFile){
	      
		  if(!hasExt){						// hidden file has not extention
			  strcpy(stegoFile,"hidden");
			  i = strlen(stegoFile);
          }
   		  else{
			strcpy(stegoFile , "hidden.");
			i=strlen(stegoFile);
			for(k=0; k < 4; i++,k++){
				if(ext[k]==0)
                           break;
				stegoFile[i] = ext[k];                      
			}
		  }
		  outputFileName = new char[strlen(destinationPath)+15];
		  strcpy(outputFileName, destinationPath);
		  strcat(outputFileName, "/");
		  strcat(outputFileName, stegoFile);
		  printf("OF : %s",outputFileName);
		  if((outFile = fopen(outputFileName,"wb")) == NULL)
			  return 3;							// report invalid destination
	}
	else{
	//	  cout<<"\n creating space for message\n";
		  message = new char[size+1];
	//	  cout<<"\n space created\n";
	}
	
	// extract secret data
	for(i=0; i<size; i++){
             bytesRead += fread(readBuffer, 8, 1, mediaFile);
             c = extractChar(readBuffer);
             if(isFile)
                       fputc(c, outFile);
             else
                       message[i] = c;
    }
    if(!isFile)
               message[i] = '\0';	
    // close opened files
    fclose(mediaFile);
    if(isFile){
              fclose(outFile);
              return 2; // success file
    }
    else
        return 4; // success message
    
}


JNIEXPORT jstring JNICALL Java_Unhide_getHiddenFileNameInMEDIA
	(JNIEnv *env, jclass jc){
		
		return env->NewStringUTF(outputFileName);
}
    
    

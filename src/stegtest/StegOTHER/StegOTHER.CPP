
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
     
unsigned int getFileSize(const char *fileName){
     FILE *fp = fopen(fileName,"rb");
     char c;
	 unsigned int sz = 0;
     
     
     sz = 0;
     while( !feof( fp ) ){
                  c = fgetc(fp);
                  sz++;
     }
     sz--;
     
     fclose(fp);
	 return sz;
}


/***************************************/
JNIEXPORT jint JNICALL  Java_Hide_hideInOTHER
  (JNIEnv *env, jclass jc, jstring SecretFileName, jstring CoverFileName, jstring dstPath, jstring password){
    FILE *secretFile, *coverFile, *outFile;
    char *outputFileName;
    char c;
	char ch;
    int i;

	const char* secretFileName;
    const char* coverFileName;
    const char* destinationPath;
	const char* msg;
    
	// transform jstring to char*
	jboolean tr = (char)1;
      
    
    secretFileName = env->GetStringUTFChars( SecretFileName, &tr );
          
    coverFileName  = env->GetStringUTFChars( CoverFileName, &tr );
    destinationPath = env->GetStringUTFChars( dstPath, &tr);
    pwd = env->GetStringUTFChars( password, &tr);
	pwdlen = strlen(pwd);

	// creat destination file name
	outputFileName = new char[strlen(destinationPath)+15];
    strcpy(outputFileName, destinationPath);
	strcat(outputFileName, "/steg" );
	strcat(outputFileName, strchr(coverFileName,'.'));
	srand(15243);

	// open files

       secretFile = fopen(secretFileName, "rb");
       if(!secretFile){
      //   cout<<"Error:File "<<secretFileName<<" cannot be opened";
         return 1;
       }
  
        
    coverFile = fopen(coverFileName, "rb");
    if(!coverFile){
      //   cout<<"Error:File "<<wavFileName<<" cannot be opened";
         return 2;
    }
    
    outFile = fopen(outputFileName, "wb");
    if(!outFile)
    {
	   printf("Error:File not opened %s\n",outputFileName);
       return 5;
    }

    /******************/

	
    //copy cover file
	size = 0;
    while(!feof(coverFile)){
        c = fgetc(coverFile);
        fputc(c, outFile);
        size++;
    }


    
     getExtension(secretFileName);
      
   

	

	// copy extention
	for(i=0; i<4; i++)
		fputc(ext[i], outFile);
    
   
		//copy secret file
		while(!feof(secretFile)){
			c = fgetc(secretFile);
			fputc(c, outFile);
		}

	


	readSizeInChar();
    //copy size
    for(i=0; i<4; i++)
             fputc(sizeInChar[i], outFile);

	// copy verification
	for(i=0; i<5; i++){
		ch = verification[i] ^ pwd[rand()%pwdlen];
		fputc(ch, outFile);
	}

    
	// close opened files
  
    fclose(secretFile);
    fclose(coverFile);
    fclose(outFile);
    
    return 4; // success
}

/**********************************/

JNIEXPORT jint JNICALL Java_Unhide_unhideInOTHER
  (JNIEnv *env, jclass jc, jstring CoverFileName, jstring DestinationPath, jstring password){
    FILE *coverFile, *outFile;
    
	int i,k;
    char c;
    unsigned int i1,i2,i3,i4;
    int endPosition;
    int isFile = 0, hasExt = 0;

    const char *coverFileName, *destinationPath;
	jboolean tr = (char)1;

	coverFileName  = env->GetStringUTFChars( CoverFileName, &tr );
    destinationPath = env->GetStringUTFChars( DestinationPath, &tr);
	pwd = env->GetStringUTFChars( password, &tr);
	pwdlen = strlen(pwd);
    srand(15243);
    // open stego file
    // todo - error handling
    coverFile = fopen(coverFileName,"rb");
    if(!coverFile)
    {
    //   cout<<"Error:Wave file "<<wavFile<<"not opened";
       return 1;
    }

	// verify
	fseek( coverFile, -5, SEEK_END);
	for(i=0; i<5; i++)
		verify[i] = fgetc(coverFile) ^ pwd[rand()%pwdlen];
	verify[i]='\0';

	printf("verify: %s\n",verify);
	
	if(strcmp(verification,verify)!=0)
		return 0;

	fseek( coverFile, -9, SEEK_END);
    endPosition = ftell( coverFile ) - 1;

	// get size of original file
	for(i=0; i<4; i++)
             sizeInChar[i] = fgetc(coverFile);

	// transform BCD to Decimal
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

	rewind( coverFile );
    fseek( coverFile, size, SEEK_SET );

	// extract size of hidden data
//	size = getFileSize(coverFileName) - (size + 4 + 5);

	// extract extension
    for(i=0; i<4; i++)
            ext[i] = fgetc(coverFile);
    

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
		  
		  outputFileName = new char[strlen(destinationPath)+20];
		  strcpy(outputFileName, destinationPath);
		  strcat(outputFileName, "/");
		  strcat(outputFileName, stegoFile);
		  ;
		  if((outFile = fopen(outputFileName,"wb")) == NULL)
			  return 3;							// report invalid destination
	}
	else{
	//	  cout<<"\n creating space for message\n";
		  message = new char[size+1];
	//	  cout<<"\n space created\n";
	}

	// extract hidden data
	while(ftell(coverFile) < endPosition ){	
             c = fgetc(coverFile);
             if(isFile)
                       fputc(c, outFile);
             else
                       message[i] = c;
    }
    if(!isFile)
               message[i] = '\0';

	// close opened files
    fclose(coverFile);
    if(isFile){
              fclose(outFile);
              return 2; // success file
    }
    else
        return 4; // success message
}



JNIEXPORT jstring JNICALL Java_Unhide_getHiddenFileNameInOTHER
	(JNIEnv *env, jclass jc){
		
		return env->NewStringUTF(outputFileName);
}
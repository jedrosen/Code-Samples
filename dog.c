#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int change(const char* mapName, char targ);//prototype
int changeBack(const char* mapName, char targ);//prototype

int main(const int argc, const char *argv[]){//start main method


const char* fname;
FILE* map = NULL;


if(argc == 1){//the user did not put in any switches so we just read form stdin to stdout
	int c;//declaration of what we are tryingt o find
	while((c = getchar())!= EOF){//checks for the end of satndard in
		fprintf( stdout,"%c",c);
	}
	return 0;//success
}


//keeps track of switches
bool num = false;
bool bigM = false;
bool smallM = false;
bool help = false;
bool noFiles = false;
bool start = true;
int mapfileIndex = 0;




for(int i = 1;i<argc;i++){
	const char* arg = argv[i];
	//printf("%s\n",arg);
	if(!(strncmp(arg,"-n\0",3))){
		num = true;
	}
	if(!(strncmp(arg,"-M\0",3))){
		bigM = true;
		if(i == argc-1){
			printf("There was no file specified after the switch\n");
			return 2;//2 is the code for a bad mapfile
		}
		mapfileIndex = (i + 1);//the mapfile will be immediately after this swtich
	}
	if(!(strncmp(arg,"-m\0",3))){
		smallM = true;
		if(i == argc-1){
			printf("There was no file specified after the switch\n");
			return 2;//2 is the code for a bad mapfile
		}
		mapfileIndex = (i + 1);//the mapfile will be immediately after this swtich
	}
	if(!(strncmp(arg,"--help\0",7))){
		//printf("went into if4\n");
		help = true;
	}
}//end for


if(help == true){
	printf("This is a program like cat with a few alterations. With the -n you can get the lines ordered. With -M you can specify a map file and with -M you can uncode using that same file. Thisprogram will read from standard in unless you specify filenames\n");
	return 0;
}


//checks to see if they have a -m and identify the coder file
if(bigM == true && smallM == true){
	printf("you cannot have the -M swtich and the -m switch on the same line for this program\n");
	return 1;
}//now we know that there is only one or zero of the -M or -m switches


int nonFiles = 0;//will count how many things arent files so we allocate an appropriately sized array
	if(bigM || smallM){
		nonFiles = 2;
	}//there will be two things on the cmmand line that are not files
	if(num){
		nonFiles++;
	}
if(nonFiles == argc -1){//everyhting on the commandline is not a file
	//printf("nothing was a file\n");
	noFiles = true;
	nonFiles--;//we are considering stdin to be a file
}//end if

//now we know how many switches there are on the command line
//checks to see if they have a -m and open the coder file
if(bigM == true || smallM == true){//check to see if there is a mapfile
	fname = argv[mapfileIndex];
	if((map = fopen(fname,"r"))){//opens the file to be read out of
	} else {
		printf("the mapfile %s specified was either unreadable or not a mapfile\n",fname);
		return 2;//two is code for a bad mapfile
	}
	fclose(map);//close file if opened
}

/*
 *at this point the switches are taken care oof and the mapfile if open if there is one
 *now we cna start reading form the files in the input stream
 */

int lineCount = 1;//start a line counter for if the user put -n

for(int i = nonFiles +1;i < argc;i++){//we are going through the files specified after the switches
	start = true;//used for the -n option
	FILE* read;//declares a filepointer to be put into getchar 
	const char* fileName = argv[i];//gets the name of the file be using the argument
	int c;//for reading from files

	if((!(strncmp(fileName,"-\0",2))) || noFiles){//this is either the special case - or the user didn't put any files ont the line
			clearerr(stdin);//for if stdin has been closed
			while((c = getchar())!= EOF){//checks for the end of satndard in
			if(num && start){
			fprintf( stdout,"   %d ",lineCount);
			lineCount++;
			start = false;
		    }
			if(c == '\n'){//new line dont check for a mapfile because we jsut have to look at characters not special characters
					fprintf( stdout,"%c",c);
					start = true;
				} else{
					fprintf( stdout,"%c",c);
				}
			}
			noFiles = false;//we can safely assume that nofiles can be flase after excecuting this for 
		}
		/*
		 *this is the end of the if that checks to read from standard in. Now we read from the actual files
		 *we need an else here so that it either reads form standard in or a file
		 */
		 else{
		 	if((read = fopen(fileName,"r"))){//we have to read by character to accomidate for mapfiles
				while ((c = fgetc(read)) != EOF){//checks for the end of satndard in
					if(num && start){
						fprintf( stdout,"   %d ",lineCount);
						lineCount++;
						start = false;
		    			}
		    			//now we get characters
		    		if(c == '\n'){
		    			start = true;
		    			fprintf( stdout,"%c",c);
		    		} else if(c == ' '){
		    			fprintf( stdout,"%c",c);//the users mapfile does not have to deal with spaces
		    		} else{
		    			if(smallM){
		    				if(change(fname,c)){
		    					printf("No other character was found for %c, check to see if your mapfiles is properly formatted\n",c);
		    					return 2;//code for bad mapfile
		    				}
		    			} else if(bigM){
		    				//change back
		    				if(changeBack(fname,c)){
		    					printf("No other character was found for %c, check to see if your mapfiles is properly formatted\n",c);
		    					return 2;//code for bad mapfile
		    				}
		    			} else {
							fprintf( stdout,"%c",c);//be normal
		    				}
		    			}
		    		}//End while which means we have reached the end of the file
		    		fclose(read);//close file
		    	}//end if which means that the file did not open
		    	else{//file did not open
		    		printf("The file %s could not be open to be read. If this is a switch this occurred because you put switches after file names\n",fileName);
		    		return 3;//code for bad argument file
		    	}//end else that tells the user what happerned
		 }











}//end of for that iterates through the part of the comman line that is not switches or mapfiles

if(bigM == true || smallM == true){//check to see if there is a mapfile
	fclose(map);//close file if opened
}






return 0;//success
}//end main method


int change(const char* mapName, char targ){
    FILE* fp = fopen(mapName,"r");//we know it is openable because we checked in main
    char out;
    if ( fp != NULL )
   	{
      
      char line [ 4 ]; 
      while ( fgets ( line, sizeof line, fp ) != NULL ) 
      {
         //if(line[1] != ' ' ){
         //printf("This is not the correct syntax for a mapfile. All mapfiles must be two characters sepparated by a space then a newlien on every line");
         //return('\0');
     	//	}
     	if(line[0] == targ){//we have a match
     		out = line[2];
     		fprintf( stdout,"%c",out);
     		fclose(fp);
     		return 0;//means that that was a success
          	}
      }
      //will get here if no replacement is found at which point it returns a null
   }
   return(1);//a return of one means that there was no change put to standard out
}

int changeBack(const char* mapName, char targ){
    FILE* fp = fopen(mapName,"r");//we know it is openable because we checked in main
    char out;
    if ( fp != NULL )
   	{
      
      char line [ 4 ]; 
      while ( fgets ( line, sizeof line, fp ) != NULL ) 
      {
         //if(line[1] != ' ' ){
         //printf("This is not the correct syntax for a mapfile. All mapfiles must be two characters sepparated by a space then a newlien on every line");
         //return('\0');
     	//	}
     	if(line[0] == targ){//we have a match
     		out = line[2];
     		fprintf( stdout,"%c",out);
     		fclose(fp);
     		return 0;//means that that was a success
          	}
      }
      //will get here if no replacement is found at which point it returns a null
   }
   return(1);//a return of one means that there was no change put to standard out
}

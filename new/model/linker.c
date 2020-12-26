/**
 * Project 2
 * LC-2K Linker
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXSIZE 300
#define MAXLINELENGTH 1000
#define MAXFILES 6
#define BIGNUM 65535
#define MINUM -65536

typedef struct FileData FileData;
typedef struct SymbolTableEntry SymbolTableEntry;
typedef struct RelocationTableEntry RelocationTableEntry;
typedef struct CombinedFiles CombinedFiles;

struct SymbolTableEntry {
    char label[7];
    char location;
    int offset;
};

struct RelocationTableEntry {
    int offset;
    char inst[7];
    char label[7];
    int file;

};

struct FileData {
    int textSize;
    int dataSize;
    int symbolTableSize;
    int relocationTableSize;
    int textStartingLine; // in final executible
    int dataStartingLine; // in final executible
    int text[MAXSIZE];
    int data[MAXSIZE];
    SymbolTableEntry symbolTable[MAXSIZE];
    RelocationTableEntry relocTable[MAXSIZE];
};
//
//struct CombinedFiles {
//    int text[MAXSIZE];
//    int data[MAXSIZE];
//    SymbolTableEntry     symTable[MAXSIZE];
//    RelocationTableEntry relocTable[MAXSIZE];
//    int textSize;
//    int dataSize;
//    int symTableSize;
//    int relocTableSize;
//};

int isUpper(char *firstletter){
    //EFFECTS: return whether the letter is an upper letter or not
    return (*firstletter)>='A'&&(*firstletter)<='Z';
}

void checkDuplicate(FileData *files, int argc){
    int i=0;
    int index=0;
    while(i<argc-2){
        for(int j=i+1;j<argc-2;++j){
            for (int k=0;k<files[j].symbolTableSize;++k){
                if(!strcmp(files[i].symbolTable[index].label,files[j].symbolTable[k].label)
                   &&files[i].symbolTable[index].location!='U'&&files[j].symbolTable[k].location!='U'){
                    printf("duplicate defined labels");
                    exit(1);
                }
            }
        }
        index++;
        if(index>=files[i].symbolTableSize){
            i++;
            index=0;
        }
    }
}

int calNewtext(int relo_text, int new_off){
    int oldoffset=relo_text&BIGNUM;
    int res=relo_text-oldoffset;
    res+=new_off;
    return res;
}
int findSizes(FileData files[MAXFILES], int exception, int argc);
void initFiles(FileData *files ,int argc){
    //only init dataStargting line
    int line=0;
    for(int i=0; i< argc -2; ++i){
        line+=files[i].textSize;
    }
    for(int i=0;i<argc-2;++i){
        files[i].dataStartingLine=line;
        line+=files[i].dataSize;
    }

}

void fixLocals(FileData *files, int argc);
int searchGlobal(FileData *files, int argc, char *inputstr);
int main(int argc, char *argv[])
{
    char *inFileString, *outFileString;
    FILE *inFilePtr, *outFilePtr;
    int i, j;

    if (argc <= 2) {
        printf("error: usage: %s <obj file> ... <output-exe-file>\n",
               argv[0]);
        exit(1);
    }

    outFileString = argv[argc - 1];
    outFilePtr = fopen(outFileString, "w");
    if (outFilePtr == NULL) {
        printf("error in opening %s\n", outFileString);
        exit(1);
    }

    FileData files[MAXFILES];

    //Reads in all files and combines into master
    for (i = 0; i < argc - 2; i++) {
        inFileString = argv[i+1];

        inFilePtr = fopen(inFileString, "r");
        printf("opening %s\n", inFileString);

        if (inFilePtr == NULL) {
            printf("error in opening %s\n", inFileString);
            exit(1);
        }

        char line[MAXLINELENGTH];
        int sizeText, sizeData, sizeSymbol, sizeReloc;


        // parse first line
        fgets(line, MAXSIZE, inFilePtr);
        sscanf(line, "%d %d %d %d",
               &sizeText, &sizeData, &sizeSymbol, &sizeReloc);

        files[i].textSize = sizeText;
        files[i].dataSize = sizeData;
        files[i].symbolTableSize = sizeSymbol;
        files[i].relocationTableSize = sizeReloc;
        files[i].dataStartingLine=0;
        files[i].textStartingLine=0;
        for(int k=0;k<i;k++){
            files[i].textStartingLine+=files[k].textSize;
        }
        // read in text
        int instr;
        for (j = 0; j < sizeText; j++) {
            fgets(line, MAXLINELENGTH, inFilePtr);
            instr = atoi(line);
            files[i].text[j] = instr;
        }

        // read in data
        int data;
        for (j = 0; j < sizeData; j++) {
            fgets(line, MAXLINELENGTH, inFilePtr);
            data = atoi(line);
            files[i].data[j] = data;
        }

        // read in the symbol table
        char label[7];
        char type;
        int addr;
        for (j = 0; j < sizeSymbol; j++) {
            fgets(line, MAXLINELENGTH, inFilePtr);
            sscanf(line, "%s %c %d",
                   label, &type, &addr);
            files[i].symbolTable[j].offset = addr;
            strcpy(files[i].symbolTable[j].label, label);
            files[i].symbolTable[j].location = type;

            //check defined stack
            if(!strcmp("Stack",label)&&type!='U'){
                exit(1);
            }
        }

        // read in relocation table
        char opcode[7];
        for (j = 0; j < sizeReloc; j++) {
            fgets(line, MAXLINELENGTH, inFilePtr);
            sscanf(line, "%d %s %s",
                   &addr, opcode, label);
            files[i].relocTable[j].offset = addr;
            strcpy(files[i].relocTable[j].inst, opcode);
            strcpy(files[i].relocTable[j].label, label);
            files[i].relocTable[j].file	= i;
        }
        fclose(inFilePtr);
    } // end reading files

    int textcount=0;
    for(int i=0;i<argc-2;i++){
        textcount+=files[i].textSize;
    }
//    for(int i=0;i<argc-2;i++){
//        for(int k=0;k<i;k++){
//            files[i].dataStartingLine+=files[k].dataSize;
//        }
//        files[i].dataStartingLine+=textcount;
//    }

    // *** INSERT YOUR CODE BELOW ***
    //    Begin the linking process
    //    Happy coding!!!
//    for(int i=0;i<argc-2;i++){
//        printf("%d is files %d size\n",files[i].textSize,i);
//    }
    checkDuplicate(files,argc);
    initFiles(files,argc);
    fixLocals(files,argc);
    for (int i = 0; i < argc - 2; ++i){
        for (int j = 0; j < files[i].textSize; ++j){
//            printf("%d\n",files[i].text[j]);
            fprintf(outFilePtr, "%d\n", files[i].text[j]);
        }
    }
    for (int i = 0; i < argc - 2; ++i){
        for (int j = 0; j < files[i].dataSize; ++j){
//            printf("%d\n",files[i].data[j]);
            fprintf(outFilePtr, "%d\n", files[i].data[j]);
        }
    }
} // end main
void fixLocals(FileData *files, int argc){
    int totalline=0;
    for(int i=0;i< argc -2; ++i){
        totalline+=files[i].dataSize+files[i].textSize;
    }
//    printf("The total line is %d\n",totalline);
    for(int i = 0; i< argc - 2; i++){
        for(int j = 0; j < files[i].relocationTableSize;++j){
            int offset=files[i].relocTable[j].offset;
           // int relo_text=files[i].text[offset]; // find the text of the relocation (to be added)
            if(!strcmp(files[i].relocTable[j].inst, "lw")
               ||!strcmp(files[i].relocTable[j].inst, "sw")){
                if(isUpper(files[i].relocTable[j].label)){
                    if(!strcmp(files[i].relocTable[j].label, "Stack")){
                        files[i].text[offset]+=totalline; //original it is zero
                    }
                    else{
                        int res = searchGlobal(files,argc,files[i].relocTable[j].label);
                        int preoff=files[i].text[offset]&BIGNUM;
                        files[i].text[offset]+=(res-preoff);

                    }
                }
                else{
                    int res = files[i].text[offset];
                    res=res&BIGNUM;
                    if(res>=files[i].textSize){
                        files[i].text[offset]+=files[i].dataStartingLine-files[i].textSize;
                    }
                    else{
                        files[i].text[offset]+=files[i].textStartingLine;
                    }
//                    if(res>=files[i].textSize){
//                        int temp=0;
//                        for(int k=0;k<i;k++){
//                            temp+=files[k].dataSize;
//                        }
//                        for(int k=0;k<argc-2;k++){
//                            if(k!=i){
//                                temp+=files[k].textSize;
//                            }
//                        }
//                        files[i].text[offset]+=temp;
//                    }
//                    else{
//                        int temp=0;
//                        for(int k=0;k<i;k++){
//                            temp+=files[k].textSize;
//                        }
//                        files[i].text[offset]+=temp;
//                    }
                }
            }
            else if(!strcmp(files[i].relocTable[j].inst,".fill")){
                printf("%s is the label,and %d is the offset\n",files[i].relocTable[j].label,files[i].data[offset]);
                if(isUpper(files[i].relocTable[j].label)){
                    if(!strcmp(files[i].relocTable[j].label, "Stack")){
                        files[i].data[offset]=totalline; //original it is zero
                    }
                    else{
                        int res = searchGlobal(files,argc,files[i].relocTable[j].label);
                        files[i].data[offset]=res;
                    }
                }
                else{
                    int res = files[i].data[offset];
                    if(res>=files[i].textSize){
 
                        files[i].data[offset]+=files[i].dataStartingLine-files[i].textSize;
                    }
                    else{

                        files[i].data[offset]+=files[i].textStartingLine;
                    }
                }
                printf("%s is the label,and %d is the offset\n",files[i].relocTable[j].label,files[i].data[offset]);
            }
        }
    }
}


int searchGlobal(FileData *files, int argc, char *inputstr){
    int res=-1;
    for(int i=0;i<argc-2;++i){
        for(int j=0;j<files[i].symbolTableSize;++j){
            if((!strcmp(files[i].symbolTable[j].label,inputstr))){
                if(files[i].symbolTable[j].location=='T'){
                    res=files[i].symbolTable[j].offset+files[i].textStartingLine; //belong to text
                }
                else if(files[i].symbolTable[j].location=='D'){
                    res=files[i].symbolTable[j].offset+files[i].dataStartingLine;
                }
                else if(files[i].symbolTable[j].location=='U'&&!strcmp("Stack",inputstr)){
                    res=0;
                }
            }
        }
    }
    if(res==-1&&strcmp("Stack",inputstr)) {
        printf("%s undefined\n",inputstr);
        printf("undefined global labels.");
        exit(1);
    }
    if(res==-1){
        exit(1);
    }
    return res;
}

int findSizes(FileData *files, int itself, int argc){
    int size = 0;
    for (int i = 0; i <argc-2; i++){
        if (i != itself){

                if (i > itself) {
                    size += files[i].textSize;
                } else {
                    size += files[i].dataSize + files[i].textSize;
                }


        }
    }
    return size;
}
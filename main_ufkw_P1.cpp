

#include <fstream>
#include <string>


using namespace std;

string kwt[6] = {"if", "else", "int", "return", "void", "while"};
string ss[19] = {"+", "-", "*", "/", "<", "<=", ">", ">=", "==", "!=", "=", ";",
                ",", "(", ")", "[", "]", "{", "}"}; 
string errs[14] = {"!", "@", "#", "$", "%", "^", "&", "_", "`", "~", "|", "\\", "?", "."};
string nums [10] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};


int checkNumsArr(string s)
{
    int found = -1;
    for(int i = 0; i < 10; i++)
    {
        found = s.compare(nums[i]);

        if(found == 0)
            return 4;

    }

    return 0;
}


int checkErrsArr(string s)
{
    int found = -1;

    //check illegal chars dictionary
    for(int i = 0; i < 14; i++)
    {
        found = s.compare(errs[i]);

        if(found == 0)
            return i;
        
    }


    return -1;

}


void writeFunction(string tokeypokey, ofstream& outputFile)
{

    if(tokeypokey == "if" || tokeypokey == "else" || tokeypokey ==  "int" || tokeypokey ==  "return" || tokeypokey ==  "void" || tokeypokey == "while")
    {
        outputFile << "KW: " << tokeypokey << endl;
    }
    else
    {
         //should have no errors if in here

    //id or num and print
    int isNum;
    string tokenN;
    string tokenA;
    int n = 0;
    int a = 0;
 

    for(int k = 0; k < tokeypokey.length(); k++)
    {
        char curChar = tokeypokey.at(k);
        string s(1, curChar);
        isNum = checkNumsArr(s);


        //checking new char
        if(isNum == 4) //new char is a num
        {

            //two scenarios, switched or not switched


            //keep appending ints
            tokenN.push_back(curChar);
            
            if(!tokenA.empty()) //has switched
            {
                outputFile << "ID: " << tokenA << endl;
                tokenA.erase();
            }

        }
        else // new char is alph
        {

            //two scenarios, switched or not switched

            //keep appending alph
            tokenA.push_back(curChar);

            if(!tokenN.empty())
            {
                outputFile << "NUM: " << tokenN << endl;
                tokenN.erase();
            }
            

            
        }
        


    }
    

    //should print last string of token, should also print solo ids or nums
    if(tokenN.length() > 0) //is a num
    {
        outputFile << "NUM: " << tokenN << endl;
    }
    else if(tokenA.length() > 0)
    {
        outputFile << "ID: " << tokenA << endl;
    }

    }  

}


int checkDict(string s)
{
    int found = -1;



    //check kwt dictionary

    for(int i = 0; i < 6; i++)
    {
        found = s.compare(kwt[i]);

        if(found == 0)
            return 1;

    }

    //check ss dictionary


    for(int i = 0; i < 19; i++)
    {
        found = s.compare(ss[i]);

        if(found == 0)
            return 2;
        
    }




    
}

string deleteSpace(string s)
{
    string wSpace = " \t";
    size_t it = s.find_first_not_of(wSpace);

    if(it == 0)
    {
        return s;
    }
    else
    {
        return s.substr(it);
    }
    

}

char& k_back(string token)
{
    int size = token.size();
    char& back = token.at(size-1);
    return back;

}

void k_pop_back(string& token)
{
    int size = token.size();
    string newToken = token.substr(0, size-1);
    token = newToken;
}


int main( int argc, char *argv[], char *envp[] )
{
    string fileName = string(argv[1]);

    ifstream inputFile;
    inputFile.open(fileName.c_str());

    //create output file
    ofstream outputFile;
    string outputName = "outputFileForP1.txt";
    outputFile.open(outputName.c_str());
    
    string line;
    if(inputFile.is_open())
    {

        //read file line by line
        bool d = false;
       
        while(getline(inputFile, line))
        {
            

            if(!line.empty() && line != " " && line != "\t" && line != "\r" && line != "\n")
                outputFile << "INPUT: " << deleteSpace(line) << endl;

            //read token by token deliminated by space

            string token;
            bool error = false;
            for(int p = 0; p < line.length(); p++)
            {


                //check for end of comment
                if(d == true)
                {
                     if(line.at(p) == '*' && p != line.length() - 1)
                     {
                        if(line.at(p+1) == '/')
                        {
                            d = false; //comment is ended
                            p++;
                        }
                     }

                }
                else if(line.at(p) == '/' && p != line.length() - 1)//check for comments starting
                {
                     
                    if(line.at(p+1) == '/')
                    {
                        
                        //go end of line
                        p = line.length(); //this should break us out
                    }
                    else if(line.at(p+1) == '*')
                    {
                        d = true;
                        p++;
                        //go till we find */


                    }
                    else //it's just a slash
                    {
                        goto label;
                    }
                    

                }
                else if(d == false) //not inside comment
                {
                    label:

                    token.push_back(line.at(p));
                    int f = checkDict(token);
                    string lastSym1;
                    lastSym1.push_back(line.at(p));
                    int f2 = checkDict(lastSym1); //is the newest added a special symbol

                    if(line.at(p) == ' ' || line.at(p) == '\n' || line.at(p) == '\t' || line.at(p) == '\r')
                    {

                        k_pop_back(token);
                        if(!token.empty())
                        {
                            //write token func
                            writeFunction(token, outputFile);

                            token.erase();
                        }

                    }
                    else if(f2 == 2) //ss dict
                    {

                        if(token.length() > 1)
                        {
                            //write token func
                            k_pop_back(token); //get rid of special symbol
                            writeFunction(token, outputFile);

                            token.erase();
                            token.push_back(line.at(p)); //token is now only one special symbol
                        }

                        if(p != line.length() - 1)
                        {
                            token.push_back(line.at(p+1)); //check if its a double special symbol
                            int f3 = checkDict(token);

                            if(f3 != 2)
                            {
                                k_pop_back(token);
                            }
                            else
                            {
                                p++;
                            }
                            
                       
                        }
                      
                            //write  special symbol
                            outputFile << token << endl;
                            token.erase();
                        
                    }
                    else if(f == 1)
                    {
                        
                        if(p != line.length() - 1)
                        {
                            if(line.at(p+1) == ' ' || line.at(p+1) == '\n' || line.at(p+1) == '\t' || line.at(p+1) == '\r')
                            {
                                outputFile << "KW: " << token << endl;
                                token.erase();
                            }
                            else
                            {
                              
                            }
                            
                        }
                        else
                        {
                            outputFile << "KW: " << token << endl;
                            token.erase();
                        }
                        





                    }
                    else //not a deliminiter 
                    {

                        //check if last symbol is an error
                        string curChar;
                        curChar.push_back(line.at(p));
                        int hErr = checkErrsArr(curChar); //error is pushed onto string

                        if(hErr != -1) //last symbol is an error
                        {

                            error = true;

                            
                            if(token.length() > 1) //writing everything before the error
                            {
                                //write token func
                                k_pop_back(token); //get rid of error symbol
                                writeFunction(token, outputFile);

                                token.erase();
                                token.push_back(line.at(p)); //token is now only one error
                            }

                            if(hErr == 0 && p != line.length() - 1)//error is !
                            {
                                if(line.at(p+1) == '=')
                                {
                                    token.push_back(line.at(p+1)); // token is !=
                                    outputFile << token << endl;
                                    token.erase();
                                    error = false;
                                    p++;
                                }
                                else
                                {
                                    goto label2;
                                }
                                
                                
                            }
                            else //it's an error including !
                            {
                                label2:
                                //go until we hit a /n /t /r ' ' special char */ // 
                                //current token is one char and an error

                                if(p == line.length() - 1)
                                {
                                    outputFile << "Error: " << token << endl;
                                    token.erase();
                                    error = false;
                                }
                                else
                                {


                                    bool cont = true;
                                
                                    p++;

                                    while(cont && p != line.length())
                                    {
                                    
                                        string lastTwoSym;
                                        string::iterator it = token.end()-1;
                                        lastTwoSym.push_back(*it);
                                        it++;
                                        lastTwoSym.push_back(*it);
                                        int boop = lastTwoSym.compare("!=");
                                        int beep = lastTwoSym.compare("//");
                                        int gorp = lastTwoSym.compare("/*");

                                        token.push_back(line.at(p));
                                        string lastSym;
                                        
                                        
                                        lastSym.push_back(k_back(token));
                                        

                                        int fSSKW = checkDict(lastSym);

                                        if(line.at(p) == '\n' || line.at(p) == '\t' || line.at(p) == '\r' || line.at(p) == ' ')
                                        {
                                            cont = false;
                                            k_pop_back(token);
                                            outputFile << "Error: " << token << endl;
                                            token.erase();
                                            error = false;
                                        }
                                        else if(beep == 0) //found // comment
                                        {
                                            cont = false;
                                            k_pop_back(token);
                                            k_pop_back(token);
                                            outputFile << "Error: " << token << endl;
                                            token.erase();
                                            error = false;
                                            p--;
                                            p--;

                                        }
                                        else if(gorp == 0) //found /* comment
                                        {
                                            cont = false;
                                            k_pop_back(token);
                                            k_pop_back(token);
                                            outputFile << "Error: " << token << endl;
                                            token.erase();
                                            error = false;
                                            p--;
                                            p--;
                                        }
                                        else if(fSSKW == 2) //found 1 char ss
                                        {
                                            cont = false;
                                            k_pop_back(token);
                                            outputFile << "Error: " << token << endl;
                                            token.erase();
                                            error = false;
                                            p--;
                                        }
                                        else if(boop == 0) //found !=
                                        {
                                            cont = false;
                                            k_pop_back(token);
                                            k_pop_back(token);
                                            outputFile << "Error: " << token << endl;
                                            outputFile << lastTwoSym << endl; //!= is printed
                                            token.erase();
                                            error = false;

                                        }
                                        else
                                        {
                                            p++;
                                        }
                                        



                                    }
                                }    
                                
 
                            }
                            

                        }


                    }
  

                }
               

            
            }

        
            //check if there was a token still from before new line
            if(!token.empty() && !error)
            {
                //write token func
                writeFunction(token, outputFile);
                token.erase();
            }
            else if(!token.empty() && error)
            {
                outputFile << "Error: " << token << endl;
                token.erase();
                error = false;

            }

        }
        inputFile.close();



    }


    outputFile.close();



}

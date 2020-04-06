#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "fstream"
#include <string>
using namespace std;  //in between 9-23 lines there are helper functions which lots of essential aseembly funcitons can use at the same time.
unsigned short *return_pointer(string s);      //this function returns 16 bit pointer of assembly 8086 register
unsigned char *get8Bit(string s);       //this function returns 8 bit pointer of assembly 8086 register
int calculate_index(int arr_num);      //this function returns the index of memory to reach that address
int convert_from_hexadecimal_to_decimal(string hex_val);  //this function convert hexadecimal number to decimal number
int return_value();     //rcl-rcr-shl-shr funcitons use this to take the value which know how many times are these functions processed
int last_8bit(int o);   //this function returns the least significant 8 bit of any number
int last_16bit(int p);  //this function returns the least significant 16 bit of any number
string convert_from_decimal_to_binary(int i);      //this function returns binary form of any number in the type of string
int return_value_of_right_hand_side(int index);     //this functions returns the value of right side of comma in the all line by eliminating and calculating
void change_flags(int value_1, int value_2);    //this function compares value_1 with value_2 and changes flags according to it
int control_overflow(int a, int b, int byte);   //this function is only used by the div function to check whether the result of process is arithmetic overflow or not
void change_flags_only_one_paramether(int value);   //this function controls purely one unsigned value and change flags according to it
bool change_flags_with_byte(int byte,int sayi); //this function is used by add-sub-mul functions to change flags
int xor_or_and(string s);   // as there are many common points among xor-and-or functions so all process of them are done by it
bool is_it_hex(string s);   //checks it is hex value or not

//24-52 lines contains assembly essantial functions described in project description.
int mov(int i); // it moves data from one location to another
int dec(int i); //it subtracts 1 from the destination operand, while preserving the state of the CF flag
int inc_dec(int i);
int add(int i); // it adds data from source operand to destination operand
int sub(int i); // it subtructs data from destination operand as much as source operand
int JZ_JE(int i); // Jump if zero (Zero flag set)
int JNE_JNZ(int i); //Jump if not zero (Zero flag clear)
int JA_JNBE(int i); //Jump if not below or equal (same as JA)
int JB_JC_JNAE(int i); //Jump if not above or equal (same as JB)
int JBE(int i); // Jump if below or equal (if leftOp  rightOp)
int JNB_JAE(int i); //Jump if not below (same as JAE)
int JNC(int i); //Jump if not carry
int div(int i); //the div (unsigned divide) instruction performs 8-bit, 16-bit unsigned integer division
int mul(int i);  //the mul ((unsigned multiply) instruction performs 8-bit, 16-bit unsigned integer multiplication
int JMP(int i);  //Jump to a new location without no condition
int no_t(int i); //The not instruction toggles (inverts) all bits in an operand
int x_or(int i); //The xor instruction performs a boolean exclusive-or operation between each pair of matching bits in two operands and stores the result in the destination operand
int o_r(int i);  //The or instruction performs a boolean or operation between each pair of matching bits in two operands and stores the result in the destination operand
int an_d(int i); //The or instruction performs a boolean and operation between each pair of matching bits in two operands and stores the result in the destination operand
int rcr(int i);  //The rcr (rotate carry right) instruction shifts each bit to the right, copies the Carry flag into the MSB, and copies the LSB into the Carry flag
int shl(int i);  //The shl (shift left ) instruction shifts a destination operand a given number of bits to the left
int rcl(int i); //The rcl (rotate carry left) instruction shifts each bit to the left, copies the Carry flag to the LSB, and copies the MSB into the Carry flag
int push(int i); //this function copies a value into the location in the stack pointed to by the stack pointer
int nop(int i); //?
int pop(int i); //The pop function first copies the contents of the stack element pointed to by sp into a 16 bit destination operand and then increments sp
char int_21h(int i, ofstream &outfile,ifstream& infile); //this functions is used as I|O
int shr(int i); //The shr (shift right) instruction performs a logical right shift on the destination operand, replacing the highest bit with a 0
int compare(int i); //this funciton compares destination and source operand and change flasg with according to it
//these are 16 bit registers
unsigned short ax = 0;
unsigned short bx = 0;
unsigned short cx = 0;
unsigned short dx = 0;
unsigned short di = 0;
unsigned short si = 0;
unsigned short bp = 0;
unsigned short sp = (2 << 15) - 2;
//these are the pointer of the 16 bit and 8 bit registers
unsigned short *pax = &ax;
unsigned short *pbx = &bx;
unsigned short *pcx = &cx;
unsigned short *pdx = &dx;
unsigned short *pdi = &di;
unsigned short *psi = &si;
unsigned short *pbp = &bp;
unsigned short *psp = &sp;
unsigned char *pah = (unsigned char *) (((unsigned char *) &ax) + 1);
unsigned char *pal = (unsigned char *) &ax;
unsigned char *pbh = (unsigned char *) (((unsigned char *) &bx) + 1);
unsigned char *pbl = (unsigned char *) &bx;
unsigned char *pch = (unsigned char *) (((unsigned char *) &cx) + 1);
unsigned char *pcl = (unsigned char *) &cx;
unsigned char *pdh = (unsigned char *) (((unsigned char *) &dx) + 1);
unsigned char *pdl = (unsigned char *) &dx;
bool CF, OF, AF, ZF, SF; //these are flags which processer processes according to them
vector<string> line_array; //this array keeps assembly line code line by line
vector<string> code_array; //this array keeps all assembly code entered by coder
bool is_bit16=false;    //this bool shows the left hand side of comma includes 16 bit element
bool is_bit8=false;     //this bool shows the left hand side of comma includes 8 bit element
bool is_reg=false;      //this bool shows the left hand side of comma includes any regesters
bool is_memory=false;   //this bool shows the left hand side of comma includes any memory addres
bool is_var_global=false;   ////this bool shows the left hand side of comma includes any variables that defined after int 20h command
vector<string> reg_array = {"ax", "ah", "al", "bx", "bh", "bl", "cx", "ch", "cl", "dx", "dh", "dl", "si", "di", "bp"}; //this array covers all register 8,16 bit
vector<string> bit16_array = {"ax", "bx", "cx", "dx", "si", "di", "bp", "sp"}; //this array keeps only 16 bit registers
vector<string> bit8_array = {"al", "ah", "bl", "bh", "cl", "ch", "dl", "dh"}; //this array keeps onlt 8 bit registers
vector<string> bit16_array_with_breaked = {"[bx]", "[si]", "[di]", "[bp]"}; //this array keeps the addres of which these 16 bit registers point
vector<string> bit16_array_with_b_and_breaked = {"b[bx]", "b[di]", "b[si]", "b[bp]"}; //this array keeps the addres of which these 16 bit registers point only 8 bit side
vector<string> bit16_array_with_w_and_breaked = {"w[bx]", "w[si]", "w[di]", "w[bp]"};  //this array keeps the addres of which these 16 bit registers point as identifying type of the addres
vector<pair<string, string>> variable_array; //this array keeps the name and type of variables defined in commands
vector<int> index_keeper; //this array contains the addres of variables
unsigned char memory[2 << 15];  // this array represets memory in allowed size given by instructor
vector<string>::iterator p;
void trim(string& s){
    string temp="";
    for (int i = 0; i < s.size(); ++i) {
        if(s[i]!=' ')
            temp+=s[i];
    }
    s=temp;
}
void tokenizer(string s)//this function reads the input given by developer as a line by line and send to process
{
    string left_side=s.substr(0,s.find_first_of(',')); //this line split the given line as getting with reference to the commma. left hand side
    string right_side=s.substr(s.find_first_of(',')+1,s.length()); //right hand side
    int middle_of_line=s.find_first_of(','); // this keeps the comma index in the line
    string token="";
    vector<string> temp_array;
    string left_temp;
    string right_temp;
    //there are some special input such as b  [  bx  ]  and these if catch such items
    if(left_side.find('[')!=string::npos && left_side.find(']')!=string::npos){
        if(left_side[left_side.find_first_of('[')-1]=='b' || left_side[left_side.find_first_of('[')-1]=='w'){
            left_temp=left_side.substr(left_side.find_first_of('[')-1,left_side.length());
            left_temp=left_temp.substr(0,left_temp.find_first_of(']')+1);
        }else{
            left_temp=left_side.substr(left_side.find_first_of('['),left_side.length());
            left_temp=left_temp.substr(0,left_temp.find_first_of(']')+1);
        }
    }
    if(right_side.find_first_of('[')!=string::npos && right_side.find_last_of(']')!=string::npos) {
        if (right_side[right_side.find_first_of('[') - 1] == 'b' || right_side[right_side.find_first_of('[') - 1] == 'w'){
            right_temp = right_side.substr(right_side.find_first_of('[') - 1, right_side.length());
            right_temp = right_temp.substr(0, right_temp.find_last_of(']')+1);
        }else{
            right_temp=right_side.substr(right_side.find_first_of('['),right_side.length());
            right_temp=right_temp.substr(0,right_temp.find_last_of(']')+1);
        }
    }
    trim(left_temp); //items that above if statements catched are tirmed by helper trim functions
    trim(right_temp);
    for (int k = 0; k < s.size() && s != "code segment" && s != "code ends"; k++) //this for loop splits the line according to given if statements and combines them into temp_array
    {
        if (s[k] != ' ' && s[k] != ',' && s[k] != '\t' && s[k]!='\'' && s[k]!='\"' && s[k+1]!='\'' && s[k+1]!='\"' && s[k]!='[' && s[k]!=']') {
            token += s[k];
            if(k+1==s.size()){
                temp_array.push_back(token);
                token.clear();
            }
        }else if(s[k]=='\'' || s[k]=='\"'){
            token+='\'';token+=s[k+1];token+='\'';
            temp_array.push_back(token);
            token.clear();
            break;
        }else if( s[k]=='['){
            if(token=="b" ||token=="w"){
                if(token[0]== (k<middle_of_line ? left_temp[0] : right_temp[0]))
                    token.clear();
                token+= k<middle_of_line ? left_temp : right_temp;
                temp_array.push_back(token);
                token.clear();
                k= k<middle_of_line ? middle_of_line : k=s.size();
                continue;
            }else if(temp_array[temp_array.size()-1]=="b" || temp_array[temp_array.size()-1]=="w"){
                token+=temp_array[temp_array.size()-1]; token+= k<middle_of_line ? left_temp : right_temp;
                vector<string>::iterator str; str=temp_array.end();
                temp_array.erase(str);
                temp_array.push_back(token);
                token.clear();
                k=  k<middle_of_line ? middle_of_line : k=s.size();
            }else{
                token+= k<middle_of_line ? left_temp : right_temp;
                temp_array.push_back(token);
                k= k<middle_of_line ? middle_of_line : k=s.size();
            }
        }
        else if (token!="") {
            temp_array.push_back(token);
            token.clear();
        }
    }
    for (int  i = 0; i <temp_array.size(); i++)
        line_array.push_back(temp_array[i]);
}
int main(int argc, char *argv[])//main function get the instructions and control all assembly functions with according to given input
{
    //in there input is red line by line with argv[1] and put into code array
    ofstream outFile(argv[2]);
    ifstream infile(argv[1]);
    string line_code;
    while (!infile.eof()) {
        getline(infile, line_code);
        while(line_code.length()==0 && !infile.eof())
            getline(infile,line_code);
        istringstream linestream(line_code);
        code_array.push_back(line_code);
    }
    string instruction;
    int i = 0;
    for (vector<string>::iterator p = code_array.begin(); p != code_array.end(); p++) //this for travels all code int the input
    {
        string line = *p;
        tokenizer(line);
        if (line_array.size() > 1) {
            if (line_array[1] == "db" || line_array[1] == "dw") //this if statements controls whether the line include a variable or not
            {
                if(line_array[0]=="label:"){cout<<"Unsupported Variable Name in line : " <<i; return 0;}
                int temp_ascii;
                if(line_array[2][0]=='\'' || line_array[2][0]=='\"')
                    temp_ascii= line_array[2][1];
                else temp_ascii=line_array[2][0];
                if (line_array[2][0]!='\'' && line_array[2][0]!='\"' && temp_ascii >= 48 && temp_ascii <= 57 ||(line_array[2].at(line_array[2].size() - 1) == 'h')||(line_array[2].at(line_array[2].size() - 1) == 'd'))
                    //this statements decides the input any number or not
                {
                    int sayi = 0;
                    if (line_array[2].at(line_array[2].size() - 1) == 'h' || line_array[2].at(0) == 0) //this fi statements check if this hexadecimal number or not
                    {

                        sayi = convert_from_hexadecimal_to_decimal(line_array[2]);
                    } else {
                        if (line_array[2].at(line_array[2].size() - 1) == 'd') //this fi statements check if this decimal number or not
                        {
                            line_array[2] = line_array[2].substr(0, line_array[2].size() - 1);
                            sayi = stoi(line_array[2]);
                        } else {
                            sayi = stoi(line_array[2]);
                        }
                    }
                    if ((line_array[1] == "db" && sayi < 256) || line_array[1] == "dw") //this statements controls if it is number or char
                    {
                        if ((line_array[1] == "db")) {
                            memory[i] = sayi;
                            variable_array.push_back({line_array[0],line_array[1]});
                            line_array.clear();
                            index_keeper.push_back(i);
                            i++;
                            continue;
                        } else {
                            memory[i + 1] = sayi >> 8;
                            int tail = last_8bit(sayi);
                            memory[i] = tail;
                            variable_array.push_back({line_array[0],line_array[1]});
                            index_keeper.push_back(i);
                            line_array.clear();
                            i += 2;
                            continue;
                        }
                    }
                } else //it is a char
                {
                    memory[i] = temp_ascii;
                    variable_array.push_back({line_array[0],line_array[1]});
                    index_keeper.push_back(i);
                    line_array.clear();
                    i++;
                    continue;
                }
            }
        }
        line_array.clear();
        i += 6;
        continue;
    }
    i = 0;
    for (p = code_array.begin(); p != code_array.end(); p++) //this loop travels all given code ,which sometimes forward or back
    {
        string line = *p;
        tokenizer(line);
        i++;
        if(line_array.size()>0){
            //to determine which function is processed
            if (line_array.size() > 2 && line_array[0] == "mov") //this instructions oversees given instruction is mov
            {
                if (mov(i) == -1) //this if statemnts controls the value which funciton returns and finish
                {
                    cout << "Error in line :" << i;
                    return 0;
                }
            } else if (line_array[0] == "inc") //this instructions oversees given instruction is inc
            {
                int a=inc_dec(1);//this if statemnts controls the value which funciton returns and finish
                if (a == -1) {
                    cout << "Error in line :" << i;
                    return 0;
                }
                else if(a==1){
                    cout << "Overflow in line :" << i;
                    return 0;
                }
            } else if (line_array[0] == "dec") //this instructions oversees given instruction is dec
            {
                int a=inc_dec(-1);//this if statemnts controls the value which funciton returns and finish
                if (a == -1) {
                    cout << "Error in line :" << i;
                    return 0;
                }
                else if(a==1){
                    cout << "Overflow in line :" << i;
                    return 0;
                }
            } else if (line_array.size() > 2 && line_array[0] == "add") //this instructions oversees given instruction is add
            {
                int a=add(i) == -1;//this if statemnts controls the value which funciton returns and finish
                if (a==-1) {
                    cout << "Error in line :" << i;
                    return 0;
                }
            } else if (line_array.size() > 2 && line_array[0] == "sub") //this instructions oversees given instruction is sub
            {
                int a=sub(i);//this if statemnts controls the value which funciton returns and finish
                if (a== -1)
                {
                    cout << "Error in line :" << i;
                    return 0;
                }
            } else if (line_array.size() > 0 && line_array[0] == "div") //this instructions oversees given instruction is div
            {
                int a = div(i);//this if statemnts controls the value which funciton returns and finish
                if (a == -1) {
                    cout << "Error in line :" << i;
                    return 0;
                } else if (a == 1) {
                    cout << "Overflow in line : " << i;
                    return 0;
                }
            } else if (line_array.size() > 0 && line_array[0] == "mul") //this instructions oversees given instruction is mul
            {
                int a = mul(i);//this if statemnts controls the value which funciton returns and finish or continue the process
                if (a == -1) {
                    cout << "Error in line :" << i;
                    return 0;
                }
            } else if (line_array[0] == "xor") //this instructions oversees given instruction is xor
            {
                if (x_or(i) == -1) //this if statemnts controls the value which funciton returns and finish or continue the process
                {
                    cout << "Error in line :" << i;
                    return 0;
                }
            } else if (line_array[0] == "or") //this instructions oversees given instruction is or
            {
                if (o_r(i) == -1) //this if statemnts controls the value which funciton returns and finish or continue the process
                {
                    cout << "Error in line :" << i;
                    return 0;
                }
            } else if (line_array[0] == "and") //this instructions oversees given instruction is and
            {
                if (an_d(i) == -1) //this if statemnts controls the value which funciton returns and finish or continue the process
                {
                    cout << "Error in line :" << i;
                    return 0;
                }
            } else if (line_array[0] == "not") //this instructions oversees given instruction is not
            {
                if (no_t(i) == -1)//this if statemnts controls the value which funciton returns and finish or continue the process
                {
                    cout << "Error in line :" << i;
                    return 0;
                }
            } else if (line_array[0] == "rcl") //this instructions oversees given instruction is rcl
            {
                if (rcl(i) == -1) //this if statemnts controls the value which funciton returns and finish or continue the process
                {
                    cout << "Error in line :" << i;
                    return 0;
                }
            } else if (line_array[0] == "rcr") //this instructions oversees given instruction is rcr
            {
                if (rcr(i) == -1) //this if statemnts controls the value which funciton returns and finish or continue the process
                {
                    cout << "Error in line :" << i;
                    return 0;
                }
            } else if (line_array[0] == "shl") //this instructions oversees given instruction is shl
            {
                if (shl(i) == -1) //this if statemnts controls the value which funciton returns and finish or continue the process
                {
                    cout << "Error in line :" << i;
                    return 0;
                }
            } else if (line_array[0] == "shr") //this instructions oversees given instruction is shr
            {
                if (shr(i) == -1) //this if statemnts controls the value which funciton returns and finish or continue the process
                {
                    cout << "Error in line :" << i;
                    return 0;
                }
            } else if (line_array[0] == "cmp") //this instructions oversees given instruction is cmp
            {
                if (compare(i) == -1) //this if statemnts controls the value which funciton returns and finish or continue the process
                {
                    cout << "Error in line :" << i;
                    return 0;
                }
            } else if (line_array[0] == "push") //this instructions oversees given instruction is push
            {
                if (push(i) == -1) //this if statemnts controls the value which funciton returns and finish or continue the process
                {
                    cout << "Error in line :" << i;
                    return 0;
                }
            } else if (line_array[0] == "pop") //this instructions oversees given instruction is pop
            {
                if (pop(i) == -1) //this if statemnts controls the value which funciton returns and finish or continue the process
                {
                    cout << "Error in line :" << i;
                    return 0;
                }
            } else if (line_array[0] == "nop") //this instructions oversees given instruction is nop
            {
                if (nop(i) == -1) //this if statemnts controls the value which funciton returns and finish or continue the process
                {
                    cout << "Error in line :" << i;
                    return 0;
                }
            } else if (line_array[0] == "jz" || line_array[0] == "je") //this instructions oversees given instruction is jz or je
            {
                if (JZ_JE(i) == -1)//this if statemnts controls the value which funciton returns and finish or continue the process
                {
                    cout << "Error in line :" << i;
                    return 0;
                }
            } else if (line_array[0] == "jne" || line_array[0] == "jnz") //this instructions oversees given instruction is jne or jnz
            {
                if (JNE_JNZ(i) == -1) //this if statemnts controls the value which funciton returns and finish or continue the process
                {
                    cout << "Error in line :" << i;
                    return 0;
                }
            } else if (line_array[0] == "ja" || line_array[0] == "jnbe") //this instructions oversees given instruction is ja or jnbe
            {
                if (JA_JNBE(i) == -1) //this if statemnts controls the value which funciton returns and finish or continue the process
                {
                    cout << "Error in line :" << i;
                    return 0;
                }
            } else if (line_array[0] == "jb" || line_array[0] == "jc" || line_array[0] == "jnae") //this instructions oversees given instruction is jb,jc or jnae
            {
                if (JB_JC_JNAE(i) == -1)//this if statemnts controls the value which funciton returns and finish or continue the process
                {
                    cout << "Error in line :" << i;
                    return 0;
                }
            } else if (line_array[0] == "jbe") //this instructions oversees given instruction is jbe
            {
                if (JBE(i) == -1) //this if statemnts controls the value which funciton returns and finish or continue the process
                {
                    cout << "Error in line :" << i;
                    return 0;
                }
            } else if (line_array[0] == "jnb" || line_array[0] == "jae") //this instructions oversees given instruction is jnb or jae
            {
                if (JNB_JAE(i) == -1) //this if statemnts controls the value which funciton returns and finish or continue the process
                {
                    cout << "Error in line :" << i;
                    return 0;
                }
            } else if (line_array[0] == "jnc") //this instructions oversees given instruction is jnc
            {
                if (JNC(i) == -1) //this if statemnts controls the value which funciton returns and finish or continue the process
                {
                    cout << "Error in line :" << i;
                    return 0;
                }
            } else if (line_array[0] == "jmp") //this instructions oversees given instruction is jmp
            {
                if (JMP(i) == -1) //this if statemnts controls the value which funciton returns and finish or continue the process
                {
                    cout << "Error in line :" << i;
                    return 0;
                }
            } else if (line_array[0] == "int" && line_array[1] == "21h") //this instructions oversees given instruction is int 21h
            { int_21h(i, outFile,infile); }
            else if (line_array[0] == "int" && line_array[1] == "20h")//this instructions oversees given instruction is inc
            { return 0; }
            else if (line_array[0] == "code" || (line_array[0].at(line_array[0].size() - 1) == ':'))//this instructions oversees given instruction is inc
            {}
            else {cout<<"Error in line :"<<i; return 0;}
            //these commands reset all global variables
            line_array.clear();
            is_bit8= false;
            is_bit16= false;
            is_reg=false;
            is_memory=false;
            is_var_global=false;
            continue;
        }
    }
}
int mov(int i) {
    //reg,memory,(var,w|b war)
    vector<string>::iterator bit16_reg = find(bit16_array.begin(), bit16_array.end(), line_array[1]); //it returns 16 and 8 bit register if it is in the process
    vector<string>::iterator bit8_reg = find(bit8_array.begin(), bit8_array.end(), line_array[1]);
    vector<string>::iterator bit16_reg_b_breaked = find(bit16_array_with_b_and_breaked.begin(),
                                                        bit16_array_with_b_and_breaked.end(), line_array[1]);
    vector<string>::iterator bit16_reg_w_breaked = find(bit16_array_with_w_and_breaked.begin(),
                                                        bit16_array_with_w_and_breaked.end(), line_array[1]);

    string variable = "";
    unsigned char variable_value;
    string variable_type = "";
    int keep_index = 0;
    bool is_var=false; //this for travels the specific array to find variable name and type if necessary
    for (auto const variable_temp:variable_array) {
        if (variable_temp.first == line_array[(line_array[1] == "b" || line_array[1] == "w") ? 2 : 1]) {
            variable = variable_temp.first;
            variable_type = variable_temp.second;
            is_var=true;
            break;
        }
        keep_index++;
    }
    if(!index_keeper.empty())
        keep_index=index_keeper[keep_index];
    if (bit16_reg != bit16_array.end() || bit8_reg != bit8_array.end()) //if 16 or 8 bit register is in process
    {
        is_reg=true;
        unsigned short *ptr = nullptr;
        unsigned char *ptr2 = nullptr;
        if (bit16_reg != bit16_array.end()) {
            ptr = return_pointer(line_array[1]); //returns 16 bit register
            is_bit16=true;
            int value = return_value_of_right_hand_side(2); //this line returns the value of the right hand side ,which is the right of comma in line
            if(value==-1)return -1;
            *ptr=value;
        } else {
            ptr2 = get8Bit(line_array[1]); //return 8 bit resgister
            is_bit8=true;
            int value = return_value_of_right_hand_side(2); //this line returns the value of the right hand side ,which is the right of comma in line
            if(value==-1)
                return -1;
            *ptr2=value;
        }
    } else if (bit16_reg_b_breaked != bit16_array_with_b_and_breaked.end() || bit16_reg_w_breaked != bit16_array_with_w_and_breaked.end()) //if such b[bx] or w[si] registers are in the process
    {
        is_reg=true;
        if(bit16_reg_b_breaked != bit16_array_with_b_and_breaked.end())
            is_bit8=true;
        else is_bit16=true;
        unsigned short *ptr;
        string s = ""; s.push_back(line_array[1].at(2)); s.push_back(line_array[1].at(3));
        ptr = return_pointer(s);   //returns 16 bit register
        int value = return_value_of_right_hand_side(2); //this line returns the value of the right hand side ,which is the right of comma in line
        if (value == -1)
            return -1;
        if (bit16_reg_b_breaked != bit16_array_with_b_and_breaked.end())
            memory[*ptr] = value;
        else {
            if ((*ptr + 1) < (2 << 16))
                memory[*ptr + 1] = (value >> 8); //to put the most signifiant 8 bit of the value into higher index in the memory, line scrolles 8 bit to the right
            int tail = last_8bit(value); //this returns the least significant 8 bit of the value
            memory[*ptr] = tail;
        }
    } else if (line_array[1].at(0) == '[' || ((line_array[1].at(line_array[1].size() - 1) == ']') && line_array[1].at(0) == 'w'))//if memory addres is in the process
    {
        is_bit16=true; is_memory=true;
        int location = calculate_index(1);
        int value = return_value_of_right_hand_side(2); //this line returns the value of the right hand side ,which is the right of comma in line
        if(location==-1 || value == -1)
            return -1;
        if (value < (pow(2, 16))) {
            int head_of_8 = value >> 8;  //to put the most signifiant 8 bit of the value into higher index in the memory, line scrolles 8 bit to the right
            int  tail_8 = last_8bit(value);   //this returns the least significant 8 bit of the value
            memory[location] = tail_8;
            if (location + 1 < (pow(2, 16)))
                memory[location + 1] = head_of_8;
        } else return -1;
    } else if ((line_array[1].at(line_array[1].size() - 1) == ']') && line_array[1].at(0) == 'b') //if such b[01h] memory addresing is in the process
    {
        is_bit8=true; is_memory==true;
        int location = calculate_index(1);  //calculate memory index
        int value = return_value_of_right_hand_side(2); //this line returns the value of the right hand side ,which is the right of comma in line
        if (value == -1 || value > pow(2, 8))
            return -1;
        memory[location] = value;
    } else if (line_array[1] == "b" || line_array[1] == "w" || is_var) {
        if ((line_array[1] == "b" )||(variable_type == "db" && line_array[1]==variable)) {
            is_bit16=false; is_bit8=true;
            int value = return_value_of_right_hand_side(line_array[1] == "b" ? 3 : 2);  //this line returns the value of the right hand side ,which is the right of comma in line
            if (value == -1)
                return -1;
            if(value<256)
                memory[keep_index] = value;
            else return -1;
            is_bit16 = false;
        }
        else if ((line_array[1] == "w" )||(variable_type == "dw" && line_array[1]==variable)) {
            is_bit8 = false; is_bit16=true;
            int value = return_value_of_right_hand_side(line_array[1] == "w" ? 3 : 2);  //this line returns the value of the right hand side ,which is the right of comma in line
            if (value == -1)
                return -1;
            int head_of_8 = (value >> 8); // to extract the most significant 8 bit
            int tail_8 = last_8bit(value);  //to obtain the lest significant 8 bit
            memory[keep_index] = tail_8;
            if (keep_index + 1 < (pow(2, 16)))
                memory[keep_index + 1] = head_of_8;
        }
    } else
        return -1;
    return 0;
}
int add(int i) {
    //reg,memory,(var,w|b war)
    vector<string>::iterator bit16_reg = find(bit16_array.begin(), bit16_array.end(), line_array[1]);
    vector<string>::iterator bit8_reg = find(bit8_array.begin(), bit8_array.end(), line_array[1]);
    vector<string>::iterator bit16_reg_b_breaked = find(bit16_array_with_b_and_breaked.begin(),
                                                        bit16_array_with_b_and_breaked.end(), line_array[1]);
    vector<string>::iterator bit16_reg_w_breaked = find(bit16_array_with_w_and_breaked.begin(),
                                                        bit16_array_with_w_and_breaked.end(), line_array[1]);
    string variable = "";
    unsigned char variable_value;
    string variable_type = "";
    int keep_index = 0;
    bool is_var=false;
    for (auto const variable_temp:variable_array) //this for traverse the specific array to find variable name and type if necessary
    {
        if (variable_temp.first == line_array[(line_array[1] == "b" || line_array[1] == "w") ? 2 : 1]) {
            variable = variable_temp.first;
            variable_type = variable_temp.second;
            is_var=true;
            break;
        }
        keep_index++;
    }
    if(!index_keeper.empty())
        keep_index=index_keeper[keep_index];
    if (bit16_reg != bit16_array.end() || bit8_reg != bit8_array.end()) //if 16 or 8 bit register is in process
    {
        if (bit16_reg != bit16_array.end()) {
            is_bit16= true;
            unsigned short *ptr;
            ptr = return_pointer(line_array[1]); //returns 16 bit register
            int a=return_value_of_right_hand_side(2); //this line returns the value of the right hand side ,which is the right of comma in line
            if(a==-1)
                return -1;
            int value =*ptr + a;
            if(change_flags_with_byte(16,value))
                *ptr=value%(2<<15);
            else *ptr=value;
            change_flags_only_one_paramether(*ptr); //changes the flags according to the result of operation
        } else if (bit8_reg != bit8_array.end()) {
            is_bit8= true;
            unsigned char *ptr;
            ptr = get8Bit(line_array[1]);
            int tt=return_value_of_right_hand_side(2); //this line returns the value of the right hand side ,which is the right of comma in line
            if(tt==-1)
                return -1;
            int value =*ptr + tt;
            if(change_flags_with_byte(8,value)) //if the result of process is biggerden storage, it is taken mod of storage size
                *ptr=value%(2<<15);
            else *ptr=value;
            change_flags_only_one_paramether(*ptr);
        }
    } else if ((line_array[1].at(0) == '[' && line_array[1].at(line_array[1].size() - 1) == ']') ||
               (bit16_reg_b_breaked!=bit16_array_with_b_and_breaked.end() || bit16_reg_w_breaked!=bit16_array_with_w_and_breaked.end()))//this if statemts for memory direct and also indirect addresing
    {
        is_bit16=true; int value=0; is_bit8=true;
        int location = calculate_index(1);
        bool temp=false;
        if( (line_array[1].at(0) == '[' && line_array[1].at(line_array[1].size() - 1) == ']') || (bit16_reg_w_breaked!=bit16_array_with_w_and_breaked.end())) //is it 16 bit process
            temp=true;
        if(temp)
            value= memory[location] + ((location + 1 )< (pow(2, 16)) ? (memory[location + 1] << 8) : 0);
        else
            value= memory[location];
        int a=return_value_of_right_hand_side(2);
        if(a==-1)
            return -1;
        int deger=value+a;
        if(temp && change_flags_with_byte(16,deger)) //change flags
            deger=deger%(2<<15);
        else if(!temp && change_flags_with_byte(8,deger)) //change flags
            deger=deger%(2<<7);
        change_flags_only_one_paramether(deger);
        if(temp){
            int headof_8 = (deger >> 8);  //the most significant 8 bit
            int tail_8 = last_8bit(deger);
            memory[location] = headof_8;  //push the specific location in the memory
            if (location + 1 < (pow(2, 16)))
                memory[location + 1] = tail_8;
        }else memory[location]=deger;
    } else if (line_array[1] == "b" || line_array[1] == "w" || is_var) //is it var ?
    {is_var_global = true;is_bit8 = true;is_bit16 = true;
        if ((line_array[1] == "b" )||(variable_type == "db" && line_array[1]==variable))//is it byte type?
        {
            is_bit16=false;
            int value  = return_value_of_right_hand_side(line_array[1] == "b" ? 3 : 2);  //this line returns the value of the right hand side ,which is the right of comma in line
            int result = value +  memory[keep_index] ;
            if ((value == -1 ) || (value>256))
                return -1;
            if(change_flags_with_byte(8,result))
                value%=(2<<7);
            change_flags_only_one_paramether(result);
            memory[keep_index] = result ;
        }
        else if ((line_array[1] == "w" )||(variable_type == "dw" && line_array[1]==variable)) //is it word type?
        {
            is_bit8 = false;
            int value = return_value_of_right_hand_side(line_array[1] == "w" ? 3 : 2);  //this line returns the value of the right hand side ,which is the right of comma in line
            int deger=memory[keep_index]+(memory[keep_index+1]<<8);
            int result=value+deger;
            if (value == -1 || (deger>(2<<15)))
                return -1 ;
            int head_of_8 = (result >> 8); // to extract the most significant 8 bit
            int tail_8 = last_8bit(result);  //to obtain the lest significant 8 bit
            memory[keep_index] = tail_8;
            if (keep_index + 1 < (pow(2, 16)))
                memory[keep_index + 1] = head_of_8;
        }
    }
    return 0;
}
int sub(int i) {
    //reg,memory,(var,w|b war)
    vector<string>::iterator bit16_reg = find(bit16_array.begin(), bit16_array.end(), line_array[1]);
    vector<string>::iterator bit8_reg = find(bit8_array.begin(), bit8_array.end(), line_array[1]);
    vector<string>::iterator bit16_reg_b_breaked = find(bit16_array_with_b_and_breaked.begin(),
                                                        bit16_array_with_b_and_breaked.end(), line_array[1]);
    vector<string>::iterator bit16_reg_w_breaked = find(bit16_array_with_w_and_breaked.begin(),
                                                        bit16_array_with_w_and_breaked.end(), line_array[1]);
    string variable = "";
    unsigned char variable_value;
    string variable_type = "";
    int keep_index = 0;
    bool var_is=false;
    for (auto const variable_temp:variable_array) //this for traverse the specific array to find variable name and type if necessary
    {
        if (variable_temp.first == line_array[(line_array[1] == "b" || line_array[1] == "w") ? 2 : 1]) {
            variable = variable_temp.first;
            variable_type = variable_temp.second;
            var_is=true;
            break;
        }
        keep_index++;
    }
    if(!index_keeper.empty())
        keep_index=index_keeper[keep_index];
    if (bit16_reg != bit16_array.end() || bit8_reg != bit8_array.end()) //if 16 or 8 bit register is in process
    {

        if (bit16_reg != bit16_array.end()) {
            is_bit16=true;
            unsigned short *ptr;
            ptr = return_pointer(line_array[1]); //returns 16 bit register
            int a=return_value_of_right_hand_side(2); //this line returns the value of the right hand side ,which is the right of comma in line
            if(a==-1)
                return -1;
            int temp=*ptr - a;
            change_flags_only_one_paramether(temp);  //changes the flags according to the result of operation
            *ptr=temp;
        } else if (bit8_reg != bit8_array.end()) {
            is_bit8=true;
            unsigned char *ptr;
            ptr = get8Bit(line_array[1]);
            int a=return_value_of_right_hand_side(2);//changes the flags according to the result of operation
            if(a==-1)
                return -1;
            int temp=*ptr - a;
            change_flags_only_one_paramether(temp); //changes the flags according to result
            *ptr=temp;
        }
    } else if ((line_array[1].at(0) == '[' && line_array[1].at(line_array[1].size() - 1) == ']')||
               (bit16_reg_b_breaked!=bit16_array_with_b_and_breaked.end() || bit16_reg_w_breaked!=bit16_array_with_w_and_breaked.end()) ||
               ((line_array[1][0]=='w' || line_array[1][0]=='b') && line_array[1][line_array[1].size()-1]==']')) {
        is_bit16=true; is_bit8=true;
        int location = calculate_index(1);
        int value = return_value_of_right_hand_side(2);
        if(value==-1)
            return -1;
        bool temp=false; int deger=0;
        if( (line_array[1].at(0) == '[' && line_array[1].at(line_array[1].size() - 1) == ']') ||
            (bit16_reg_w_breaked!=bit16_array_with_w_and_breaked.end()) || (line_array[1][0]=='w')) //is it 16 bit process
            temp=true;
        if(temp)
            deger= memory[location] + ((location + 1 )< (pow(2, 16)) ? (memory[location + 1] << 8) : 0);
        else
            deger= memory[location];
        int result = deger-value;
        change_flags_only_one_paramether(result); //change flags according to result
        int head_of_8 = (result >> 8);
        int  tail_8 = last_8bit(result);
        memory[location] = tail_8;
        if (location + 1 < (pow(2, 16)))
            memory[location + 1] = head_of_8;
    } else if (line_array[1] == "b" || line_array[1] == "w" || var_is)//is it var ?
    {       is_var_global=true;
        if ((line_array[1] == "b" )||(variable_type == "db" && line_array[1]==variable)) {
            is_bit8=true;
            int value = memory[keep_index];
            int deger_of_right = return_value_of_right_hand_side(line_array[1] == "b" ? 3 : 2);
            if( deger_of_right==-1 || value<0 )
                return -1;
            memory[keep_index] = value - deger_of_right;
            change_flags_only_one_paramether(memory[keep_index]);
        } else if ((line_array[1] == "w" )||(variable_type == "dw" && line_array[1]==variable)) {
            is_bit16=true;
            int deger = memory[keep_index] + (((keep_index + 1) < (pow(2, 16))) ? (memory[keep_index + 1] << 8) : 0);
            int value = return_value_of_right_hand_side(variable == line_array[1] ? 2 : 3); //returns value of right side
            if(value==-1)
                return -1;
            int result=deger-value;
            change_flags_only_one_paramether(result); //change flags
            int head_of_8 = (result >> 8);  //the most significant 8 bit
            int tail_8 = last_8bit(result);  //the lest significant 8 bit
            memory[keep_index] = tail_8;
            if (keep_index + 1 < (pow(2, 16)))
                memory[keep_index + 1] = head_of_8;
        }
    }else return -1;
    return 0;
}
void change_flag_acc_to(int byte){
    if(byte==16){
        if (*pah != 0)
            CF = OF = 1;
        else CF = 0;
    }else if(byte==32){
        if (*pdx != 0)
            CF = OF = 1;
        else CF = 0;
    }
}
int mul(int i) {
    int multiplier = return_value_of_right_hand_side(line_array[1] != "b" || line_array[1] != "w" ? 1 : 2);
    vector<string>::iterator x, y, x1, y1, bit16_with_breaked, bit16_with_b_breaked, bit16_with_w_breaked, reg;
    x = find(bit16_array.begin(), bit16_array.end(), line_array[1]);
    y = find(bit8_array.begin(), bit8_array.end(), line_array[1]);
    bit16_with_w_breaked = find(bit16_array_with_w_and_breaked.begin(), bit16_array_with_w_and_breaked.end(),
                                line_array[1]);
    bit16_with_b_breaked = find(bit16_array_with_b_and_breaked.begin(), bit16_array_with_b_and_breaked.end(),
                                line_array[1]);
    string variable = "";
    unsigned char variable_value;
    string variable_type = "";
    int keep_index = 0; bool var_is=false;
    for (auto const variable_temp:variable_array) //traverse whole array to find var type and name
    {
        if (variable_temp.first == line_array[(line_array[1]=="b" || line_array[1]=="w") ? 2 : 1]) {
            variable = variable_temp.first;
            variable_type = variable_temp.second;
            var_is=true;
            break;
        }
        keep_index++;
    }
    if(!index_keeper.empty())
        keep_index=index_keeper[keep_index];
    if (y != bit8_array.end()) //checks 8 bit reg
    {
        unsigned char *ptr;
        ptr = get8Bit(line_array[1]);
        long long  int value = *pal * *ptr;
        if (value > 0xFF)
            value%=0xFF;
        *pax=value;
        change_flag_acc_to(16); //change flags
    } else if (bit16_with_b_breaked != bit16_array_with_b_and_breaked.end()) // checks reg with breaked b[  ]
    {
        unsigned char *ptr;
        string s = "";s.push_back(line_array[1].at(2));s.push_back(line_array[1].at(3));
        ptr = (unsigned char *) return_pointer(s); //returns pointer
        long long int result = *pal * memory[*ptr];
        if (result > pow(2, 16))
            result%=(2<<15);
        *pax = result;
        change_flag_acc_to(16);  //changes according to result;
    } else if (x != bit16_array.end())  //checks 16 bit reg
    {
        unsigned short *ptr;
        ptr = return_pointer(line_array[1]); //returns pointer
        long long int value = *pax * *ptr;
        if (value > (pow(2, 32)))
            value%=((long long)pow(2,32));
        *pdx = (value >> 16);
        *pax =last_16bit( value);
        change_flag_acc_to(32);  //change flags
    } else if (bit16_with_w_breaked != bit16_array_with_w_and_breaked.end()) //check 16 bit reg with breaked w[  ]
    {
        unsigned short *ptr;
        string s = "";s.push_back(line_array[1].at(2));s.push_back(line_array[1].at(3));
        ptr = return_pointer(s);  //returns 16 bit register
        int deger = memory[*ptr] + (((*ptr + 1) < (pow(2, 16))) ? (memory[*ptr + 1] << 8) : 0);
        long long int result = deger * *ptr;
        if (result > pow(2, 32))
            result%=(long long)(pow(2,32));
        *pdx = (result >> 16);
        int  tail = last_16bit(result);
        *pax = tail;
        change_flag_acc_to(32);
    } else if ((line_array[1].at(0) == 'w' || line_array[1].at(0) == 'b')&& line_array[1][line_array[1].size()-1]==']')//is it memory
    {
        int location; string temp_location=line_array[1].substr(line_array[1].find_first_of('[')+1,line_array[1].length());
        temp_location=temp_location.substr(0,temp_location.find_last_of(']'));

        if (temp_location[temp_location.size()-1] == 'h' ||temp_location[temp_location.size()-1] == 'H' || temp_location[0] == '0')//it it hex number
            location = calculate_index(1);
        else if (temp_location[temp_location.size()-1] == 'd' || (temp_location[temp_location.size()-1] == 'D'))  //is it decimal number
        {
            string s = "";
            if(temp_location[temp_location.size()-1] == 'd')
                s=temp_location.substr(0,temp_location.find_last_of('d'));
            else if(temp_location[temp_location.size()-1] == 'D')
                s=temp_location.substr(0,temp_location.find_last_of('D'));
            location = stoi(s);
        } else location = stoi(line_array[1]);
        if (line_array[1].at(0) == 'w') {
            long long int result =*pax * (memory[location] + (location + 1 < (pow(2, 16)) ? (memory[location + 1] << 8) : 0));
            if(result>(pow(2,32)))
                result%=(long long)(pow(2,32));
            *pdx = (result >> 16);
            int  tail = last_16bit(result);
            *pax = tail;
            change_flag_acc_to(32);
        } else {
            long long int result = *pal * memory[location];
            if(result>(pow(2,16)))
                result%=(long long)(pow(2,16));
            *pax = result;
            change_flag_acc_to(32);
        }
    } else if (line_array[1] == "w" || line_array[1] == "b"  || var_is)// sayı veya variable
    {
        if(var_is){     //is it var ?
            if (line_array[1] == "w" || line_array[1]==variable) {  //is it word size ?
                long long int result =  *pax * ((memory[keep_index]+(memory[keep_index+1]<<8)));
                if(result>(pow(2,32)))
                    result%=(long long)(pow(2,32));
                *pdx=(result>>16);
                *pax = last_16bit(result);
                change_flag_acc_to(32);
            } else if(line_array[1]=="b"){   //is it byte size ?
                long long int result = *pal * (memory[keep_index]);
                *pax = result;
                change_flag_acc_to(16);     //change flags
            }else return -1;
        }
        else{
            int sayi = 0;
            if(line_array[2][0]!='\''){
                if (line_array[2].at(line_array[2].size() - 1) == 'h' && line_array[2].at(0) == '0')
                    sayi = calculate_index(2);
                else if (line_array[2].at(line_array[2].size() - 1) == 'd')
                    sayi = stoi(line_array[2].substr(0, line_array[2].size() - 1));
                else
                    sayi = stoi(line_array[2]);
            }else sayi=line_array[2][1];

            if(line_array[1]=="w"){
                long long  int result = *pax * sayi;
                if(result>(pow(2,32)))
                    result%=(long long)(pow(2,32));
                *pdx = (result >> 16);
                int  tail =last_16bit( result);
                *pax = tail;
                change_flag_acc_to(32);
            }else if(line_array[1]=="b"){
                long long  int result = *pal * sayi;
                if(result>(pow(2,16)))
                    result%=(long long)(pow(2,16));
                *pax = result;
                change_flag_acc_to(16);
            }else return -1;
        }
    } else
        return -1;
    return 0;
}
int control_overflow(int a, int b, int byte){//this function controls overflow anly for div instruction
    if ((((a / b) > pow(2, byte))) || ((a % b) > pow(2, byte)))
        return 1;
    return 0;
}
int div(int i) {
    vector<string>::iterator x, y, x1, y1, bit16_with_breaked, bit16_with_b_breaked, bit16_with_w_breaked, reg;
    x = find(bit16_array.begin(), bit16_array.end(), line_array[1]);
    y = find(bit8_array.begin(), bit8_array.end(), line_array[1]);
    bit16_with_w_breaked = find(bit16_array_with_w_and_breaked.begin(), bit16_array_with_w_and_breaked.end(),line_array[1]);
    bit16_with_b_breaked = find(bit16_array_with_b_and_breaked.begin(), bit16_array_with_b_and_breaked.end(),line_array[1]);
    string variable = "";
    unsigned char variable_value;
    string variable_type = "";
    int keep_index = 0; bool var_is=false;
    for (auto const variable_temp:variable_array)
    {
        if (variable_temp.first == line_array[2 ]) {
            variable = variable_temp.first;
            variable_type = variable_temp.second;
            var_is=true;
            break;
        }
        keep_index++;
    }
    if(!index_keeper.empty())
        keep_index=index_keeper[keep_index];
    if (x != bit16_array.end()) { //is it 16 bit reg
        unsigned short *ptr;
        ptr = return_pointer(line_array[1]);
        if(ptr== nullptr || *ptr==0)
            return -1;
        int value = (*pdx << 16) + *pax;
        if (control_overflow(value, *ptr, 16) == 1) //check that there is overflow or not
            return 1;
        int kalan = value % *ptr;
        int bolum = value / *ptr;
        *pdx=kalan;*pax=bolum;
    } else if (y != bit8_array.end()) {
        unsigned char *ptr;
        ptr = get8Bit(line_array[1]);
        if(ptr== nullptr || *ptr==0)
            return -1;
        if (control_overflow(*pax, *ptr, 8) == 1) //check that there is overflow or not
            return 1;
        int kalan = *pax % *ptr;
        int bolum= *pax / *ptr;
        *pah=kalan; *pal=bolum;
    } else if (bit16_with_b_breaked != bit16_array_with_b_and_breaked.end()) { //is b breaked 16 bit register such as b[di]
        unsigned short *ptr;
        string s = "";s.push_back(line_array[1].at(2));s.push_back(line_array[1].at(3));
        ptr =  return_pointer(s); //return 16 bit register
        if(ptr== nullptr)
            return -1;
        if (control_overflow(*pax, memory[*ptr], 8) == 1)
            return 1;
        int kalan = *pax % memory[*ptr];
        int bolum = *pax / memory[*ptr];
        *pdx=kalan; *pax=bolum;
    } else if (bit16_with_w_breaked != bit16_array_with_w_and_breaked.end()) //is it w breaked reg such as w[si]
    {
        unsigned short *ptr;
        string s = "";s.push_back(line_array[1].at(2));s.push_back(line_array[1].at(3));
        ptr = return_pointer(s);
        int value = (*pdx << 16) + *pax;
        int tt=(memory[*ptr] + (*ptr + 1 < (2 << 16)) ? (memory[*ptr + 1] << 8) : 0);
        if (control_overflow(value,tt , 16) == 1)
            return 1;
        int kalan = value % tt;
        int bolum = value / tt;
        *pdx=kalan;*pax=bolum;
    } else if ((line_array[1].at(0) == 'w' || line_array[1].at(0) == 'b') && line_array[1].at(1) == '['&&line_array[1][line_array[1].size()-1]==']')//memory
    {
        int location; string temp=line_array[1].substr(line_array[1].find_first_of('[')+1,line_array[1].size()-1);
        temp=temp.substr(0,temp.find_last_of(']'));
        if (is_it_hex(temp)) //is it hexadecimal number
            location = calculate_index(1);
        else if ((temp[temp.size()-1]=='d' || temp[temp.size()-1]=='D')) {
            location = stoi(line_array[1].substr(2, line_array[1].size() - 2));
        } else
            location = stoi(line_array[1]);
        if (line_array[1][0] == 'w') {
            int value = (*pdx << 16) + *pax;
            int sayi=(memory[location] + (((location + 1) < (2 << 16)) ? (memory[location + 1] << 8) : 0));
            if (control_overflow(value,  sayi, 16) == 1)
                return 1;
            if(sayi==0 || (value==0 && sayi==0))
                return -1;
            int kalan = value % sayi;
            int bolum= value / sayi;
            *pdx=kalan;*pax=bolum;
        } else {
            if (control_overflow(*pax, memory[location], 8) == 1)
                return 1;
            if(memory[location]==0 || (*pax==0 && memory[location]==0))
                return -1;
            int kalan = *pax % memory[location];
            int bolum = *pax / memory[location];
            *pah=kalan; *pal=bolum;
        }
    } else if (line_array[1] == "w" || line_array[1] == "b" || var_is)// sayı veya variable
    {
        if(var_is){
            int value=0; int deger = 0;
            if (line_array[1] == "w"){
                value= (*pdx << 16) + *pax;
                deger = memory[keep_index] + (((keep_index + 1) < (pow(2, 16))) ? (memory[keep_index + 1] << 8) : 0);
            }
            else if(line_array[1]=="b"){
                value=*pax;
                deger = memory[keep_index];
            }
            else return -1;
            if (line_array[1] == "w" && control_overflow(value, deger, 16) == 1)
                return 1;
            else if(line_array[1] == "b" && control_overflow(value, deger, 8) == 1)
                return 1;
            if(deger==0 || ( value==0 && deger==0))
                return -1;
            int kalan = value % deger;
            int bolum = value / deger;
            if(line_array[1]=="w")
            {*pdx=kalan;*pax=bolum;}
            else if(line_array[1]=="b")
            {*pah=kalan; *pal=bolum;}
        }else{
            int sayi = 0;
            if (is_it_hex(line_array[2]))  //check is hex number
                sayi = calculate_index(2);
            else if (line_array[2].at(line_array[2].size() - 1) == 'd' || line_array[2][line_array[2].size()-1]=='D') // is decimal number
                sayi = stoi(line_array[2].substr(0, line_array[2].size() - 1));
            else
                sayi = stoi(line_array[2]);  //decimal
            if(line_array[1] == "w") {
                int value = (*pdx << 16) + *pax;
                if (control_overflow(value, sayi, 16) == 1)
                    return 1;
                if(sayi==0 || (value==0 && sayi==0)) //is 0/0 division or any number/division
                    return -1;
                int kalan = value % sayi;
                int bolum = value / sayi;
                *pdx=kalan;*pax=bolum;
            } else {
                if (control_overflow(*pax, sayi, 8) == 1)
                    return 1;
                if((sayi==0) || (*pax==0 && sayi==0)) //is 0/0 division or any number/division
                    return -1;
                int kalan = *pax % sayi;
                int bolum = *pax / sayi;
                *pah=kalan; *pal=bolum;
            }
        }
    }else return -1;
    return 0;
}
int no_t(int i) {
    vector<string>::iterator x, y, x1, y1, bit16_with_breaked, bit16_with_b_breaked, bit16_with_w_breaked, reg;

    x = find(bit16_array.begin(), bit16_array.end(), line_array[1]);
    x1 = find(bit8_array.begin(), bit8_array.end(), line_array[1]);

    bit16_with_w_breaked = find(bit16_array_with_w_and_breaked.begin(), bit16_array_with_w_and_breaked.end(),
                                line_array[1]);
    bit16_with_b_breaked = find(bit16_array_with_b_and_breaked.begin(), bit16_array_with_b_and_breaked.end(),
                                line_array[1]);
    string variable = "";
    unsigned char variable_value;
    string variable_type = "";
    int keep_index = 0; bool var_is=false;
    for (auto const variable_temp:variable_array)//travers variable array to find var name and type
    {
        if (variable_temp.first == line_array[2]) {
            variable = variable_temp.first;
            variable_type = variable_temp.second;
            var_is=true;
            break;
        }
        keep_index++;
    }
    if(!index_keeper.empty())
        keep_index=index_keeper[keep_index];
    if (x != bit16_array.end() || x1 != bit8_array.end()) { //check is it 16 or 8 bit register
        if (x != bit16_array.end()) {
            unsigned short *ptr;
            ptr = return_pointer(line_array[1]); //return pointer
            *ptr = ~*ptr;
        } else {
            unsigned char *ptr;
            ptr = get8Bit(line_array[1]); //return pointer
            *ptr = ~*ptr;
        }
    } else if (bit16_with_b_breaked != bit16_array_with_b_and_breaked.end() || bit16_with_w_breaked != bit16_array_with_w_and_breaked.end()) { //is it breaked register
        string ss = line_array[1].substr(2, line_array[1].size() - 1);
        if (bit16_with_w_breaked != bit16_array_with_w_and_breaked.end()) { //is it w breaked register such as w[bp]
            unsigned short *ptr;
            ptr = return_pointer(ss); //return register
            int deger = memory[*ptr] + (*ptr + 1 < (pow(2, 16))) ? (memory[*ptr + 1] << 8) : 0;
            deger = ~deger;
            int  tail =last_8bit( deger); //the least significant 8 bit
            memory[*ptr] = tail;
            if (*ptr + 1 < (pow(2, 16)))
                memory[*ptr + 1] = (deger >> 8); //the most 8 significant bit
        } else { //is it b breaked register such as b[bx]
            unsigned char *ptr;
            ptr = get8Bit(ss);
            memory[*ptr] = ~memory[*ptr];
        }
    } else if ((line_array[1][0] == 'w' || line_array[1][0]=='b') && line_array[1].at(line_array[1].size() - 1)==']') { //is it memory direct addresing
        int location = 0; string ss=line_array[1].substr(line_array[1].find_first_of('[')+1,line_array[1].length());
        ss=ss.substr(0,ss.find_last_of(']'));
        if (is_it_hex(ss))  //is it hexadecimal number
            location = convert_from_hexadecimal_to_decimal(ss);
        else if ((ss[ss.size()-1]=='d' || ss[ss.size()-1]=='D') && (line_array[1][0]!='0')) //is it decimal number
            location=stoi(ss.substr(0,ss.size()-1));
        else location=stoi(ss);
        int deger = memory[location] + (location + 1 < (pow(2, 16))) ? (memory[location + 1] << 8) : 0;
        deger = ~deger;
        int tail = last_8bit(deger);
        memory[location] = tail;
        if (location + 1 < (pow(2, 16)))
            memory[location + 1] = (deger >> 8);
    }else if(var_is){  //is it variable
        if(line_array[1]=="w"){   //is it word size
            int deger=memory[keep_index]+(memory[keep_index]<<8);  //take the value of memory in specific index
            deger=~deger;
            int tail=last_8bit(deger);
            int head=(deger>>8);
            memory[keep_index]=tail;
            memory[keep_index+1]=head;
        }else if(line_array[1]=="b"){ //is it byte size
            int deger=memory[keep_index];
            deger=~deger;
            memory[keep_index]=deger;
        }else return -1;
    }else return -1;
    return 0;
}
int x_or(int i) {
    return xor_or_and("xor");
}
int o_r(int i) {
    return xor_or_and("or");
}
int an_d(int i) {
    return xor_or_and("and");
}
int rcr(int i) {
    /*
   * reg-sth
   * memory-sth
   * var-sth
   */
    string variable = "";
    unsigned char variable_value;
    string variable_type = "";
    int keep_index = 0;
    bool is_var=false;
    for (auto const variable_temp:variable_array) {//the loop traverse the my array to find the name and type of variable
        if (variable_temp.first == line_array[(line_array[1]=="b" || line_array[1]=="w") ? 2 : 1]) {
            variable = variable_temp.first;
            variable_type = variable_temp.second;
            is_var=true;
            break;
        }
        keep_index++;
    }
    if(!index_keeper.empty())
        keep_index=index_keeper[keep_index];
    vector<string>::iterator x, y, x1, y1, bit16_with_breaked, bit16_with_b_breaked, bit16_with_w_breaked, reg;

    x = find(bit16_array.begin(), bit16_array.end(), line_array[1]);
    x1 = find(bit8_array.begin(), bit8_array.end(), line_array[1]);

    bit16_with_breaked = find(bit16_array_with_breaked.begin(), bit16_array_with_breaked.end(), line_array[1]);
    bit16_with_w_breaked = find(bit16_array_with_w_and_breaked.begin(), bit16_array_with_w_and_breaked.end(),
                                line_array[1]);
    bit16_with_b_breaked = find(bit16_array_with_b_and_breaked.begin(), bit16_array_with_b_and_breaked.end(),
                                line_array[1]);

    int value = return_value();
    if(value==-1)
        return -1;
    if (x != bit16_array.end() || x1 != bit8_array.end()) {//is it 16 or 8 bit register
        if (x != bit16_array.end()) {
            unsigned short *ptr;
            ptr = return_pointer(line_array[1]); //return reg pointer
            for (int i = 0; i < value; i++) {
                unsigned short tt = CF << sizeof(*ptr) * 8 - 1; //to put cf flag the right hand side of the value, it shifts cf flag as much as the size of value
                CF = *ptr & 1; //CF equal to the least one significant bit of pointer
                *ptr >>= 1;
                *ptr |= tt;
            }
        } else {
            unsigned char *ptr;
            ptr = get8Bit(line_array[1]);  //it it 8 bit register
            for (int i = 0; i < value; i++) {
                unsigned short tt = CF << sizeof(*ptr) * 8 - 1;   //to put cf flag the right hand side of the value, it shifts cf flag as much as the size of value
                CF = *ptr & 1;  //cf is made equals to the least significant bit of value
                *ptr >>= 1;
                *ptr |= tt;
            }
        }
    } else if (bit16_with_breaked != bit16_array_with_breaked.end() ||
               bit16_with_b_breaked != bit16_array_with_b_and_breaked.end() ||
               bit16_with_w_breaked != bit16_array_with_w_and_breaked.end()) {//is it breaked register such as [bx],b[bx],w[bx]
        if (bit16_with_b_breaked != bit16_array_with_b_and_breaked.end()) {//is it b breaked register like b[bx]
            unsigned char *ptr;
            string s = "";s.push_back(line_array[1].at(2));s.push_back(line_array[1].at(3));
            ptr = (unsigned char *) return_pointer(s);   //return register
            for (int i = 0; i < value; i++) {
                int value = memory[*ptr] + (*ptr + 1 < (pow(2, 16))) ? memory[*ptr + 1] << 8 : 0;
                unsigned short tt = CF << sizeof(value) * 8 - 1;   //to put cf flag the right hand side of the value, it shifts cf flag as much as the size of value
                CF = value & 1;   //cf is made equals to the least significant bit of value
                value >>= 1;
                value |= tt;
            }
            memory[*ptr] = value;
        } else {
            unsigned short *ptr;
            string s=line_array[1].substr(line_array[1].find_first_of('[')+1,line_array[1].length());
            s=s.substr(0,s.find_last_of(']'));
            ptr = return_pointer(s);  //return pointer to access the memory specific index
            for (int i = 0; i < value; i++) {  //this process continues as given value
                int value = memory[*ptr] + (*ptr + 1 < (pow(2, 16))) ? (memory[*ptr + 1] << 8) : 0;
                unsigned short tt = CF << sizeof(value) * 8 - 1;  //CF is shifted 15 bit left
                CF = value & 1; //cf is made equal to the lest dignificant bit ot the value
                value >>= 1;
                value |= tt;
            }
            int  tail = last_8bit(value); //returns the lest 8 significant bit of the result
            memory[keep_index] = tail;
            if (keep_index + 1 < (pow(2, 16)))
                memory[keep_index + 1] = (value >> 8);  //to get the most 8 significant bit

        }
    } else if ((line_array[1].at(0) == '[' || line_array[1][0]=='w' || line_array[1][0]=='b')&&( line_array[1].at(line_array[1].size() - 1) == ']')) {
        int location = calculate_index(1); int value=0; if(location==-1)return -1;
        if((line_array[1].at(0) == '[' || line_array[1][0]=='w')&&(line_array[1].at(line_array[1].size() - 1) == ']')) //is it 16 bit process
            value    = memory[location] + (location + 1 < (pow(2, 16))) ? (memory[location + 1] << 8) : 0;
        else value=memory[location]; //else 8 bit process
        for (int i = 0; i < value; i++) {
            unsigned short tt = CF << sizeof(value) * 8 - 1;  //to give cf to the value, it must shift to left the size of its
            CF = value & 1;
            value >>= 1;
            value |= tt;
        }
        if((line_array[1].at(0) == '[' || line_array[1][0]=='w')&&(line_array[1].at(line_array[1].size() - 1) == ']')){ //th 16 bit operation
            int tail =last_8bit( value);
            memory[keep_index] = tail;
            if (keep_index + 1 < (pow(2, 16)))
                memory[keep_index + 1] = (value >> 8);
        }else
            memory[location]=value;  //8 bit operation
    } else if ((line_array[1] == "w" || line_array[1] == "b") && is_var && line_array[3] != "w" && line_array[3] != "b")//shl w var,1  //is it variable ?
    {
        if (line_array[1] == "b") { //is it byte size ?
            int value = memory[keep_index];
            for (int i = 0; i < value; i++) {
                unsigned short tt = CF << sizeof(value) * 8 - 1;   //to put cf flag the right hand side of the value, it shifts cf flag as much as the size of value
                CF = value & 1;
                value >>= 1;
                value |= tt;
            }
            memory[keep_index] = value;
        } else { //is it word size
            int value = memory[keep_index] + (keep_index + 1 < (pow(2, 16))) ? (memory[keep_index + 1] << 8) : 0;
            for (int i = 0; i < value; i++) {
                unsigned short tt = CF << sizeof(value) * 8 - 1;  //to put cf flag the right hand side of the value, it shifts cf flag as much as the size of value
                CF = value & 1;
                value >>= 1;
                value |= tt;
            }
            int tail = last_8bit(value); //putting operation
            memory[keep_index] = tail;
            if (keep_index + 1 < (pow(2, 16)))
                memory[keep_index + 1] = (value >> 8);
        }

    }else return -1;
    return 0;
}
int shr(int i) {

    string variable = "";
    unsigned char variable_value;
    string variable_type = "";
    int keep_index = 0;
    bool is_var=false;
    for (auto const variable_temp:variable_array) {  //is it var int the process
        if (variable_temp.first == line_array[(line_array[1]=="b" || line_array[1]=="w") ? 2 : 1]) {
            variable = variable_temp.first;
            variable_type = variable_temp.second;
            is_var=true;
            break;
        }
        keep_index++;
    }
    if(!index_keeper.empty())
        keep_index=index_keeper[keep_index];
    vector<string>::iterator x, y, x1, y1, bit16_with_breaked, bit16_with_b_breaked, bit16_with_w_breaked, reg;

    x = find(bit16_array.begin(), bit16_array.end(), line_array[1]); //16 bit reg
    x1 = find(bit8_array.begin(), bit8_array.end(), line_array[1]); //8 bit reg

    bit16_with_breaked = find(bit16_array_with_breaked.begin(), bit16_array_with_breaked.end(), line_array[1]); //16 bit breaked reg like [si]
    bit16_with_w_breaked = find(bit16_array_with_w_and_breaked.begin(), bit16_array_with_w_and_breaked.end(),
                                line_array[1]);//16 bit breaked reg like w[bp]
    bit16_with_b_breaked = find(bit16_array_with_b_and_breaked.begin(), bit16_array_with_b_and_breaked.end(),
                                line_array[1]);  //16 bit reg with b breaked like b[bx]

    int value = return_value();
    if(value==-1)
        return -1;
    if (x != bit16_array.end() || x1 != bit8_array.end()) { //is 16 or 8 bit reg?
        if (x != bit16_array.end()) {
            unsigned short *ptr;
            ptr = return_pointer(line_array[1]);  //return reg
            int a = *ptr;
            for (; a != 0; a >>= 1)     //shr process
                CF = a & 1;
            *ptr >>= value;
        } else {
            unsigned char *ptr;
            ptr = get8Bit(line_array[1]);     //returns 8 bit operation
            int a = *ptr;
            for (; a != 0; a >>= 1)//shr process
                CF = a & 1;
            *ptr >>= value;
        }
    } else if (bit16_with_breaked != bit16_array_with_breaked.end() ||
               bit16_with_b_breaked != bit16_array_with_b_and_breaked.end() ||
               bit16_with_w_breaked != bit16_array_with_w_and_breaked.end()) {
        if (bit16_with_b_breaked != bit16_array_with_b_and_breaked.end()) {     //are they 16 bit breaked reg
            string s = "";s.push_back(line_array[1].at(2));s.push_back(line_array[1].at(3));
            int adres = *return_pointer(s);
            int deger = memory[adres];
            for (; deger != 0; deger >>= 1)  //shr operation
                CF = deger & 1;
            memory[adres] = deger;
        } else {
            string s=""; s=line_array[1].substr(line_array[1].find_first_of('[')+1,line_array[1].length());
            s=s.substr(0,s.find_last_of(']'));
            int adres = *return_pointer(s);     //returns pointer value
            int deger = memory[adres] + (adres + 1 < (pow(2, 16))) ? (memory[adres + 1] << 8) : 0;
            for (; deger != 0; deger >>= 1)     //shr process
                CF = deger & 1;
            int tail =last_8bit( deger);  //the least significant 8 bit push operation
            memory[adres] = tail;
            if (adres + 1 < (pow(2, 16)))  //the most significant 8 bit push operation
                memory[adres + 1] = (deger >> 8);
        }
    } else if ((line_array[1].at(0) == '['||line_array[1][0]=='b' || line_array[1][0]=='w') && (line_array[1].at(line_array[1].size() - 1) == ']') ){
        int location = calculate_index(1); int a=0;
        if((line_array[1].at(0) == '['||line_array[1][0]=='w')&&(line_array[1].at(line_array[1].size() - 1) == ']')) //is it 16 bit operation
            a = memory[location] + location + 1 < (2 << 15) ? (memory[location + 1] << 8) : 0;
        else a=memory[location];
        for (; a != 0; a >>= 1)//shr process
            CF = a & 1;
        int tail =last_8bit( a);  //the least significant 8 bit push operation
        memory[location] = tail;
        if (location + 1 < (pow(2, 16)) && (line_array[1].at(0) == '['||line_array[1][0]=='w') )  //is it 16 bit push operation into index
            memory[location + 1] = (a >> 8);
    } else if ((line_array[1] == "w" || line_array[1] == "b") && is_var && line_array[3] != "w" && line_array[3] != "b")//shl w var,1 //is it var
    {
        if (line_array[1] == "b") {
            int deger = memory[keep_index];
            for (; deger != 0; deger >>= 1) //shr operation
                CF = deger & 1;
            memory[keep_index] = deger;  //the least significant 8 bit push operation
        } else {
            int deger = memory[keep_index] + (keep_index + 1 < (pow(2, 16))) ? (memory[keep_index] << 8) : 0;
            for (; deger != 0; deger >>= 1)  //shr operation
                CF = deger & 1;
            int tail = last_8bit(deger); //the least significant 8 bit push operation
            memory[keep_index] = tail;
            if (keep_index + 1 < (pow(2, 16))) //the most significant 8 bit push operation
                memory[keep_index + 1] = (deger >> 8);

        }
    } else
        return -1;
    return 0;
}
int shl(int i) {
    /*
     * reg-sth
     * memory-sth
     * var-sth
     */
    string variable = "";
    unsigned char variable_value;
    string variable_type = "";
    int keep_index = 0; bool is_var;
    for (auto const variable_temp:variable_array) {  //is it var int the process
        if (variable_temp.first == line_array[(line_array[1]=="b" || line_array[1]=="w") ? 2 : 1]) {
            variable = variable_temp.first;
            variable_type = variable_temp.second;
            is_var=true;
            break;
        }
        keep_index++;
    }
    if(!index_keeper.empty())
        keep_index=index_keeper[keep_index];
    vector<string>::iterator x, y, x1, y1, bit16_with_breaked, bit16_with_b_breaked, bit16_with_w_breaked, reg;

    x = find(bit16_array.begin(), bit16_array.end(), line_array[1]); //16 bit reg
    x1 = find(bit8_array.begin(), bit8_array.end(), line_array[1]);//8 bit reg

    bit16_with_breaked = find(bit16_array_with_breaked.begin(), bit16_array_with_breaked.end(), line_array[1]);  //16 bit breaked reg like [si]
    bit16_with_w_breaked = find(bit16_array_with_w_and_breaked.begin(), bit16_array_with_w_and_breaked.end(),
                                line_array[1]); //16 bit breaked reg like w[bp]
    bit16_with_b_breaked = find(bit16_array_with_b_and_breaked.begin(), bit16_array_with_b_and_breaked.end(),
                                line_array[1]);  //16 bit reg with b breaked like b[bx]

    int value = return_value();
    if(value==-1)
        return -1;
    if (x != bit16_array.end() || x1 != bit8_array.end()) {  //is 16 or 8 bit reg?
        if (x != bit16_array.end()) {
            unsigned short *ptr;
            ptr = return_pointer(line_array[1]);  //return reg
            CF = *ptr & 1;
            *ptr <<= value;
        } else {  //8 bit shl process
            unsigned char *ptr;
            ptr = get8Bit(line_array[1]); //return 8 bi reg
            CF = *ptr & 1;
            *ptr <<= value;
        }
    } else if (bit16_with_breaked != bit16_array_with_breaked.end() ||
               bit16_with_b_breaked != bit16_array_with_b_and_breaked.end() ||
               bit16_with_w_breaked != bit16_array_with_w_and_breaked.end()) {//are they 16 bit breaked reg
        if (bit16_with_b_breaked == bit16_array_with_b_and_breaked.end()) {
            unsigned short *ptr;
            string s = "";s.push_back(line_array[1].at(2));s.push_back(line_array[1].at(3));
            ptr = return_pointer(s);
            int deger = memory[*ptr] + *ptr + 1 < (pow(2, 16)) ? (memory[*ptr + 1] << 8) : 0;
            CF = deger & 1; //shl operation
            deger <<= value;
            int  tail = last_8bit(deger);  //the least significant 8 bit push operation
            memory[*ptr] = tail;
            if (*ptr + 1 < (pow(2, 16)))
                memory[*ptr + 1] = (deger >> 8);  //the most significant 8 bit push operation
        } else {
            unsigned short *ptr;
            string s = "";s.push_back(line_array[1].at(2));s.push_back(line_array[1].at(3));
            ptr = return_pointer(s);
            int deger = memory[*ptr];
            CF = deger & 1; //shl operation
            deger <<= value;
            memory[*ptr] = deger;  //push
        }
    } else if ((line_array[1].at(0) == '['||line_array[1][0]=='b' || line_array[1][0]=='w') && (line_array[1].at(line_array[1].size() - 1) == ']') ){  //is it memory direct or indirect addressing shl mode
        int location = calculate_index(1); int deger=0;
        if((line_array[1].at(0) == '['||line_array[1][0]=='w')&&(line_array[1].at(line_array[1].size() - 1) == ']')) //is it 16 bit operation
            deger = memory[location] + location + 1 < (2 << 15) ? (memory[location + 1] << 8) : 0;
        else deger=memory[location];
        CF = deger & 1;  //shl process
        deger <<= value;
        int tail = last_8bit(deger);    //the least significant 8 bit push operation
        memory[location] = tail;
        if (location + 1 < (pow(2, 16)) && (line_array[1].at(0) == '['||line_array[1][0]=='w'))
            memory[location + 1] = (deger >> 8);  //the most significant 8 bit push operation
    } else if ((line_array[1] == "w" || line_array[1] == "b") && is_var && line_array[3] != "w" && line_array[3] != "b")//shl w var,1  //is it var?
    {       int deger=0;
        if(line_array[1] == "b")  //it it byte
            deger = memory[keep_index];
        else if((line_array[1] == "w"))  //is t it word
            deger= memory[keep_index] + (keep_index + 1 < (pow(2, 16))) ? (memory[keep_index + 1] << 8) : 0;
        CF = deger & 1;
        deger <<= value;
        int  tail = last_8bit(deger);  //the least 8 significant bit operation
        memory[keep_index] = tail;
        if (keep_index + 1 < (pow(2, 16))  && (line_array[1].at(0) == '['||line_array[1][0]=='w'))
            memory[keep_index + 1] = (deger >> 8);  //the most 8 significant push operation
    } else return -1;
    return 0;
}
int inc_dec(int i){  //is it incremant or decremant operation
    /**
     * reg ->ax,al,b[bx],w[bx]
     * memory w[01h] ,b[01h]
     * var w var,v var
     * */
    vector<string>::iterator bit16_reg = find(bit16_array.begin(), bit16_array.end(), line_array[1]);  //is it 16 bit reg
    vector<string>::iterator bit8_reg = find(bit8_array.begin(), bit8_array.end(), line_array[1]);  //is it 8 bit operation
    vector<string>::iterator bit16_reg_b_breaked = find(bit16_array_with_b_and_breaked.begin(),
                                                        bit16_array_with_b_and_breaked.end(), line_array[1]);  //is it b breaked reg operaiton
    vector<string>::iterator bit16_reg_w_breaked = find(bit16_array_with_w_and_breaked.begin(),
                                                        bit16_array_with_w_and_breaked.end(), line_array[1]);  //is it w breaked reg operation
    vector<string>::iterator bit16_reg_with_breaked = find(bit16_array_with_breaked.begin(),bit16_array_with_breaked.end(), line_array[1]);  //it it breaked reg operation
    int index=0; bool is_var=false; string variable="";
    for (auto const st:variable_array) {
        if(st.first==line_array[(line_array[1]=="b" || line_array[1]=="w") ? 2 : 1]){
            variable=st.first;
            is_var=true;
            break;
        }
        index++;
    }
    if(bit8_reg!=bit8_array.end())  //8 bit reg
    {
        unsigned char *ptr;
        ptr=get8Bit(line_array[1]);
        *ptr+=i;
        change_flags_only_one_paramether(*ptr);
    }
    else if(bit16_reg!=bit16_array.end())  //16 bit reg
    {
        unsigned short *ptr;
        ptr=return_pointer(line_array[1]);
        *ptr+=i;
        change_flags_only_one_paramether(*ptr);
    }
    else if(bit16_reg_with_breaked!=bit16_array_with_breaked.end()||bit16_reg_w_breaked!=bit16_array_with_w_and_breaked.end()||line_array[1].at(0)=='w'){  //is it 16 bit reg ?
        int location;
        if(bit16_reg_with_breaked!=bit16_array_with_breaked.end())
            location=*return_pointer(line_array[1].substr(1,line_array[1].size()-1));
        else if(bit16_reg_w_breaked!=bit16_array_with_w_and_breaked.end())
            location=*return_pointer(line_array[1].substr(2,line_array[1].size()-1));
        else
            location=calculate_index(1);
        int value=memory[location]+((location+1<(2,15)) ? (memory[location+1]<<8) : 0);
        value+=1;
        change_flags_only_one_paramether(value);
        memory[location]=last_8bit(value);
        if((location+1)<(2<<15))
            memory[location]=(value>>8);
    }
    else if(bit16_reg_b_breaked!=bit16_array_with_b_and_breaked.end() || line_array[1].at(0)=='b' || line_array[1].at(0)=='w' || line_array[1].at(0)=='[')  //it 8 bit reg operaiton
    {
        if(bit16_reg_b_breaked!=bit16_array_with_b_and_breaked.end()||line_array[1].at(0)=='b'){
            int location;
            if(bit16_reg_b_breaked!=bit16_array_with_b_and_breaked.end())
                location=*get8Bit(line_array[1].substr(line_array[1].find_last_of('[')+1,line_array[1].find_last_of(']')));
            else
                location=calculate_index(1);
            if(location==-1) return -1;
            memory[location]+=i;
            change_flags_only_one_paramether(memory[location]);
        }
        else{
            int location=calculate_index(1); if(location==-1)return -1;
            int value=memory[location]+((location+1)<(2<<15))?(memory[location+1]<<8) : 0;
            value+=i;
            change_flags_only_one_paramether(value);
            memory[location]=last_8bit(value);
            if((location+1)<(2<<15))
                memory[location+1]=(value>>8);
        }
    }
    else if(is_var)  // is var ?
    {
        if(!index_keeper.empty())
            index=index_keeper[index];
        if(line_array[1]=="b"){  // is it 8 bit reg?
            memory[index]+=i;
            change_flags_only_one_paramether(memory[index]);
        }
        else if(line_array[1]=="w" || line_array[1]==variable)  //is it 16 bit var ?
        {
            int value=memory[index]+((index+1)<(2<<15))?(memory[index+1]<<8) : 0;
            value+=i;
            change_flags_only_one_paramether(value);
            memory[index]=last_8bit(value);
            if((index+1)<(2<<15))
                memory[index+1]=(value>>8);
        }else return -1;
    }else return -1;
    return 0;
}
int rcl(int i) {
    /*
    * reg-sth
    * memory-sth
    * var-sth
    */
    string variable = "";
    unsigned char variable_value;
    string variable_type = "";
    int keep_index = 0; bool var_is_=false;
    for (auto const variable_temp:variable_array) {  //the loop traverse the my array to find the name and type of variable
        if (variable_temp.first == line_array[(line_array[1]=="b" || line_array[1]=="w") ? 2 : 1]) {
            variable = variable_temp.first;
            variable_type = variable_temp.second;
            var_is_=true;
            break;
        }
        keep_index++;
    }
    if(!index_keeper.empty())
        keep_index=index_keeper[keep_index];
    vector<string>::iterator x, y, x1, y1, bit16_with_breaked, bit16_with_b_breaked, bit16_with_w_breaked, reg;

    x = find(bit16_array.begin(), bit16_array.end(), line_array[1]);
    x1 = find(bit8_array.begin(), bit8_array.end(), line_array[1]);

    bit16_with_breaked = find(bit16_array_with_breaked.begin(), bit16_array_with_breaked.end(), line_array[1]);
    bit16_with_w_breaked = find(bit16_array_with_w_and_breaked.begin(), bit16_array_with_w_and_breaked.end(),
                                line_array[1]);
    bit16_with_b_breaked = find(bit16_array_with_b_and_breaked.begin(), bit16_array_with_b_and_breaked.end(),
                                line_array[1]);

    int value = return_value();
    if(value==-1)
        return -1;
    if (x != bit16_array.end() || x1 != bit8_array.end()) { //is it 16 or 8 bit register
        if (x != bit16_array.end()) {
            unsigned short *ptr;
            ptr = return_pointer(line_array[1]);  //return reg pointer
            for (int i = 0; i < value; i++) {  //the rcl processes until reach the value
                bool k = CF;
                string s=convert_from_decimal_to_binary(*ptr);  //to take the most significant bit, this line convert decimal number into binary form
                if(s.size()==16)
                    CF = convert_from_decimal_to_binary(*ptr).at(0) - '0';
                else CF=0;
                *ptr <<= 1;
                *ptr |= k;
            }
        } else {
            unsigned char *ptr;
            ptr = get8Bit(line_array[1]); //return reg pointer
            unsigned char temp=*ptr;
            for (int i = 0; i < value; i++) {  //the rcl processes until reach the value
                bool k = CF;
                string s=convert_from_decimal_to_binary(temp);   //to take the most significant bit, this line convert decimal number into binary form
                if(s.size()==8)
                    CF = convert_from_decimal_to_binary(temp).at(0) - '0';
                else CF=0;
                temp=(temp << 1);
                temp=(temp | k);
            }
            *ptr=temp;
        }
    } else if (bit16_with_breaked != bit16_array_with_breaked.end() ||
               bit16_with_b_breaked != bit16_array_with_b_and_breaked.end() ||
               bit16_with_w_breaked != bit16_array_with_w_and_breaked.end()) {  //is it breaked register such as [bx],b[bx],w[bx]
        if (bit16_with_b_breaked == bit16_array_with_b_and_breaked.end()) {  //is it b breaked register like b[bx]
            unsigned short *ptr;
            string pointer=line_array[1].substr(line_array[1].find_first_of('[')+1,line_array[1].find_last_of(']'));
            ptr=return_pointer(pointer);  //return register
            int deger = memory[*ptr] + (*ptr + 1 < (pow(2, 16))) ? (memory[*ptr + 1] << 8) : 0;
            for (int i = 0; i < value; i++) {
                bool k = CF;
                string s=convert_from_decimal_to_binary(deger); //to take the most significant bit, this line convert decimal number into binary form
                if(s.size()==16)
                    CF = convert_from_decimal_to_binary(deger).at(0) - '0';
                else CF=0;
                deger <<= 1;
                deger |= k;
            }
            int tail = last_8bit(deger);
            memory[*ptr] = tail;
            if (*ptr + 1 < (pow(2, 16)))
                memory[*ptr + 1] = (deger >> 8);

        } else {
            unsigned char *ptr;
            string s = "";s.push_back(line_array[1].at(2));s.push_back(line_array[1].at(3));
            ptr = get8Bit(s);  //return pointer to access the memory specific index
            int deger = memory[*ptr];
            for (int i = 0; i < value; i++) {  //this process continues as given value
                bool k = CF;
                string s=convert_from_decimal_to_binary(deger);   //to take the most significant bit, this line convert decimal number into binary form
                if(s.size()==8)
                    CF = convert_from_decimal_to_binary(deger).at(0) - '0';
                else CF=0;
                deger <<= 1;
                deger |= k;
            }
            memory[*ptr] = deger;  //push process
        }
    } else if ((line_array[1].at(0) == '[' || line_array[1][0]=='w' || line_array[1][0]=='b')&&( line_array[1].at(line_array[1].size() - 1) == ']')){
        int location = calculate_index(1); int deger=0; if(location==-1)return -1;
        if((line_array[1].at(0) == '[' || line_array[1][0]=='w')&&(line_array[1].at(line_array[1].size() - 1) == ']')) //is it 16 bit process
            deger    = memory[location] + (location + 1 < (pow(2, 16))) ? (memory[location + 1] << 8) : 0;
        else deger=memory[location]; //else 8 bit process

        for (int i = 0; i < value; i++) {
            bool k = CF;
            string s=convert_from_decimal_to_binary(deger);  //to take the most significant bit, this line convert decimal number into binary form
            if(s.size()==8)
                CF = convert_from_decimal_to_binary(deger).at(0) - '0';
            else CF=0;
            deger <<= 1;
            deger |= k;
        }
        int tail = last_8bit(deger);  //the least significant bit
        memory[keep_index] = tail;
        if (keep_index + 1 < (pow(2, 16))&&(line_array[1].at(0) == '[' || line_array[1][0]=='w'))  //the most 8 significant bit
            memory[keep_index + 1] = (deger >> 8);
    } else if (var_is_ )//rcl w var,1  //is var ?
    {
        int deger;
        if(line_array[1]=="w" || line_array[1]==variable)  //16 bit operation
            deger = memory[keep_index] + (keep_index + 1 < (pow(2, 16))) ? (memory[keep_index + 1] << 8) : 0;
        else  if(line_array[1]=="b")//8 bit operation
            deger = memory[keep_index];
        for (int i = 0; i < value; i++) {  //rcl operation until process reach to that value
            bool k = CF;
            string s=convert_from_decimal_to_binary(deger);   //to take the most significant bit, this line convert decimal number into binary form
            if(s.size()==8)
                CF = convert_from_decimal_to_binary(deger).at(0) - '0';
            else CF=0;
            deger <<= 1;
            deger |= k;
        }
        int tail = last_8bit(deger);
        if (keep_index + 1 < (pow(2, 16)) && line_array[1]=="w") //16 bit push operation
            memory[keep_index + 1] = (deger >> 8);


    } else
        return -1;
    return 0;
}
int push(int i) {
    is_bit16=true; //push the value into top of stack
    if (return_value_of_right_hand_side(1) < (2 << 15)) {
        int head_8bit = (return_value_of_right_hand_side(1) >> 8);
        int value = return_value_of_right_hand_side(1);
        if(value==-1)
            return -1;
        int tail_8 =last_8bit( value);
        memory[sp] = head_8bit;
        memory[sp - 1] = tail_8;
        sp -= 2;
    } else
        return -1;
    return 0;
}
int nop(int i) {//does nothing
    return 0;
}
int pop(int i) { // returns value in the top of the stack
    vector<string>::iterator regbul = find(bit16_array.begin(), bit16_array.end(), line_array[1]);
    unsigned short *ptr;
    ptr = return_pointer(line_array[1]);
    for (int p = 1; p < line_array.size(); p++) {
        if (regbul != bit16_array.end()) {
            int head_8bit = (memory[sp + 2]<<8);
            int tail_8bit = memory[sp + 1];
            *ptr = head_8bit + tail_8bit;
            memory[sp + 2] = memory[sp + 1] = 0;
            sp += 2;
        } else return -1;
    }
    return 0;
}
char int_21h(int i, ofstream &outFile,ifstream& infile ) {
    if (*pah == 1) { //reads given input from file
        char ch=cin.get();
        if(ch=='\n')
        {
            *pal=13;
            *pdl = ch;
        }
        else
        {
            *pal = (unsigned char) ch;
            *pdl = ch;
        }

    } else if (*pah == 2) {  //display from the value of dl register  to console
        *pal = *pdl;
        char ch = *pal;
        cout << ch;
        return ch;
    }
    return ' ';
}
int compare(int i) {
    vector<string>::iterator x, y, x1, y1, reg, bit16_with_breaked, bit16_with_w_breaked, bit16_with_b_breaked;
    x = find(bit16_array.begin(), bit16_array.end(), line_array[1]);
    y = find(bit16_array.begin(), bit16_array.end(), line_array[2]);

    x1 = find(bit8_array.begin(), bit8_array.end(), line_array[1]);
    y1 = find(bit8_array.begin(), bit8_array.end(), line_array[2]);

    reg = find(reg_array.begin(), reg_array.end(), line_array[1]);

    bit16_with_breaked = find(bit16_array_with_breaked.begin(), bit16_array_with_breaked.end(), line_array[1]);
    bit16_with_w_breaked = find(bit16_array_with_w_and_breaked.begin(), bit16_array_with_w_and_breaked.end(),
                                line_array[1]);
    bit16_with_b_breaked = find(bit16_array_with_b_and_breaked.begin(), bit16_array_with_b_and_breaked.end(),
                                line_array[1]);

    int asci=0;
    if(line_array[1][0]=='\'')
        asci=line_array[1][1];
    else asci=line_array[1][0];
    string var_name = "";
    int variable_index = 0;
    string var_type;
    bool is_variable=false;
    for (auto const l:variable_array) {
        if (l.first == line_array[(line_array[1] == "b" || line_array[1] == "w") ? 2 : 1]) {
            var_name = l.first;
            var_type = l.second;
            is_variable=true;
            break;
        }
        variable_index++;
    }
    if(!index_keeper.empty())
        variable_index=index_keeper[variable_index];
    if (bit16_with_b_breaked != bit16_array_with_b_and_breaked.end() ||
        bit16_with_w_breaked != bit16_array_with_w_and_breaked.end() ||
        bit16_with_breaked != bit16_array_with_breaked.end()) {
        if ((bit16_with_breaked != bit16_array_with_breaked.end() || ((bit16_with_w_breaked != bit16_array_with_w_and_breaked.end()) && line_array[2] != "b"))) {
            is_bit16=true;
            int oo = return_value_of_right_hand_side(2);
            if (oo == -1)
                return -1;
            string ss = line_array[1].substr(line_array[1].find_first_of('[')+1,line_array[1].length());ss=ss.substr(0,ss.find_last_of(']'));
            unsigned short *ptr; ptr=return_pointer(ss);
            int value = memory[*ptr] + (((*ptr + 1) < (pow(2, 16))) ? ( (memory[*ptr + 1] << 8) ): 0);
            change_flags(value, oo);
        } else if ((bit16_with_b_breaked != bit16_array_with_b_and_breaked.end() && line_array[2] != "w")) {
            is_bit8=true;
            int oo = return_value_of_right_hand_side(2);
            if (oo == -1)
                return -1;
            string s = "";s.push_back(line_array[1].at(2));s.push_back(line_array[1].at(3));
            unsigned short *ptr; ptr=return_pointer(s);
            change_flags(memory[*ptr], oo);
        }
    } else if (reg != reg_array.end()) {
        if (line_array[1].at(1) != 'l' && line_array[1].at(1) != 'h') {
            is_bit16=true;
            int deger = return_value_of_right_hand_side(2);
            if (deger == -1)
                return -1;
            unsigned short *ptr;
            ptr = return_pointer(line_array[1]);
            change_flags(*ptr, deger);
        } else {
            is_bit8=true;
            int deger = return_value_of_right_hand_side(2);
            if (deger == -1)
                return -1;
            unsigned char *ptr;
            ptr = get8Bit(line_array[1]);
            change_flags(*ptr, deger);
        }
    } else if ((line_array[1].at(0) == '[' || line_array[1][0]=='w' || line_array[1][0]=='b')&& line_array[1].at(line_array[1].size() - 1) == ']') {
        string s=""; s=line_array[1].substr(line_array[1].find_first_of('[')+1,line_array[1].length());s=s.substr(0,s.find_last_of(']'));
        if((line_array[1].at(0) == '[' || line_array[1][0]=='w' ))
            is_bit16=true;
        else is_bit8=true;
        string location = "";
        if (is_it_hex(s))
            location = calculate_index(1);
        else if (line_array[1].at(line_array[1].size() - 2) == 'd'||line_array[1][line_array[1].size()-2]=='D')
            location =s.substr(0,s.size()-1);
        else location=s;
        int sayi = stoi(location);
        is_bit16=true;
        int value = return_value_of_right_hand_side(2);
        if (sayi == -1 || value == -1)
            return -1;
        if((line_array[1].at(0) == '[' || line_array[1][0]=='w' ))
            change_flags((memory[sayi] + (memory[sayi + 1] << 8)), value);
        else
            change_flags(memory[sayi] , value);
    }
    else if(is_variable)//cmp w,b var, ax
    {
        int value=0;
        if(line_array[1]!="b" && line_array[1]!="w"){
            if(var_type=="dw")
                is_bit16=true;
            else is_bit8=true;
            int right_value=return_value_of_right_hand_side(2);
            if(right_value==-1)
                return -1;
            value= var_type=="dw" ? (memory[variable_index] + (memory[variable_index+1]<<8)) : memory[variable_index];
            change_flags(value,right_value);
        }else{
            if(line_array[1]=="b")
                is_bit8=true;
            else if(line_array[1]=="w")
                is_bit16=true;
            value = line_array[1]=="b" ? memory[variable_index] : (memory[variable_index]+(memory[variable_index+1]<<8));
            int right_value=return_value_of_right_hand_side(3);
            if(right_value==-1)
                return -1;
            change_flags(value,right_value);
        }
    }
    else if (line_array[1] == "b" || line_array[1] == "w")//cmp w 'a',ax
    {
        int value=0;
        if(line_array[1] == "b")
            is_bit8=true;
        else if(line_array[1]=="w")
            is_bit16=true;
        else return -1;
        int value_right_hand_side = return_value_of_right_hand_side(3);
        if(value_right_hand_side==-1)
            return -1;
        int asci=line_array[2][1];
        change_flags(asci,value_right_hand_side);
    } else if (asci >= 48 && asci <= 57)//sayi
    {   is_bit16=is_bit8=true;
        int value_rigth_hand_side = return_value_of_right_hand_side(2);
        if (value_rigth_hand_side == -1)
            return -1;
        change_flags(asci, value_rigth_hand_side);
    } else if(line_array[1]=="sp"){
        int value=return_value_of_right_hand_side(2);
        if(value==-1)
            return -1;
        change_flags(sp,value);
    }else
        return -1;
    return 0;
}
int JZ_JE(int i) { // Jump if zero (Zero flag set)
    if (line_array[0] == "jz" || line_array[0] == "je") {
        line_array[1].push_back(':');
        vector<string>::iterator o = find(code_array.begin(), code_array.end(), line_array[1]);
        if (o == code_array.end())
            return -1;
        if (ZF)
            p = --o;
    }
    return 0;
}
int JNE_JNZ(int i) {  //Jump if not zero (Zero flag clear)
    if (line_array[0] == "jne" || line_array[0] == "jnz") {
        line_array[1].push_back(':');
        vector<string>::iterator o = find(code_array.begin(), code_array.end(), line_array[1]);
        if (o == code_array.end())
            return -1;
        if (!ZF)
            p = --o;
    }
    return 0;
}
int JA_JNBE(int i) { //Jump if not below or equal (same as JA)
    if (line_array[0] == "ja" || line_array[0] == "jnbe") {
        line_array[1].push_back(':');
        vector<string>::iterator o = find(code_array.begin(), code_array.end(), line_array[1]);
        if (o == code_array.end())
            return -1;
        if (!CF && !ZF)
            p = --o;
    }
    return 0;
}
int JB_JC_JNAE(int i) { //Jump if not above or equal (same as JB)
    if (line_array[0] == "jb" || line_array[0] == "jc" || line_array[0] == "jnae") {
        line_array[1].push_back(':');
        vector<string>::iterator o = find(code_array.begin(), code_array.end(), line_array[1]);
        if (o == code_array.end())
            return -1;
        if (CF)
            p = --o;
    }
    return 0;
}
int JBE(int i) { // Jump if below or equal (if leftOp  rightOp)
    if (line_array[0] == "jbe") {
        line_array[1].push_back(':');
        vector<string>::iterator o = find(code_array.begin(), code_array.end(), line_array[1]);
        if (o == code_array.end())
            return -1;
        if ((CF || ZF)) {
            p = --o;
        }
    }
    return 0;
}
int JNB_JAE(int i) { //Jump if not below (same as JAE)
    if (line_array[0] == "jnb" || line_array[0] == "jae") {
        line_array[1].push_back(':');
        vector<string>::iterator o = find(code_array.begin(), code_array.end(), line_array[1]);
        if (o == code_array.end())
            return -1;
        if (!CF)
            p = --o;
    }
    return 0;
}
int JNC(int i) {
    if (line_array[0] == "jnc") { //Jump if not carry
        line_array[1].push_back(':');
        vector<string>::iterator o = find(code_array.begin(), code_array.end(), line_array[1]);
        if (o == code_array.end())  //is there any label
            return -1;
        if (!CF)
            p = --o;
    }
    return 0;
}
int JMP(int i) { //jump no need any condition
    if (line_array[0] == "jmp") {
        line_array[1].push_back(':');
        vector<string>::iterator o = find(code_array.begin(), code_array.end(), line_array[1]);
        if (o == code_array.end()) //is there any label
            return -1;

        p = --o;
    }
    return 0;
}
unsigned short *return_pointer(string s) {  //return 16 bit register
    if (s == "ax")
        return pax;
    else if (s == "bx")
        return pbx;
    else if (s == "cx")
        return pcx;
    else if (s == "dx")
        return pdx;
    else if (s == "di")
        return pdi;
    else if (s == "si")
        return psi;
    else if (s == "bp")
        return pbp;
    else if (s == "sp")
        return psp;
    else
        return nullptr;
}
unsigned char *get8Bit(string s) {  //returns 8 bit register
    if (s.at(0) == 'a') {
        if (s.at(1) == 'l')
            return (pal);
        else if (s.at(1) == 'h')
            return (pah);
    }
    else if (s.at(0) == 'b')
    {
        if (s.at(1) == 'l')
            return (pbl);
        else if (s.at(1) == 'h')
            return (pbh);
    }
    else if (s.at(0) == 'c')
    {
        if (s.at(1) == 'l')
            return (pcl);
        else if (s.at(1) == 'h')
            return (pch);
    }
    else if (s.at(0) = 'd')
    {
        if (s.at(1) == 'l')
            return (pdl);
        else if (s.at(1) == 'h')
            return (pdh);
    }

    return nullptr;

}
int calculate_index(int arr_num) { // calculate memory location
    string location_of_memory_s = "";
    int location_of_memory=0;
    location_of_memory_s=line_array[arr_num].substr(line_array[arr_num].find_first_of('[')+1,line_array[arr_num].length());
    location_of_memory_s=location_of_memory_s.substr(0,location_of_memory_s.find_last_of(']'));

    unsigned short *ptr; ptr=return_pointer(location_of_memory_s);
    if(ptr!= nullptr)
        return *ptr;
    if (location_of_memory_s[location_of_memory_s.size() - 1] == 'h')
        return convert_from_hexadecimal_to_decimal(location_of_memory_s);// converting from Hexadec to Dec
    else if(location_of_memory_s[location_of_memory_s.size() - 1] == 'd')
        return stoi(location_of_memory_s.substr(0,location_of_memory_s.size()-1));
    else{
        bool hex=false;
        for (int i = 0; i <location_of_memory_s.size(); ++i) {
            if((location_of_memory_s[i]>=65 && location_of_memory_s[i]<=70)||(location_of_memory_s[i]>=97 && location_of_memory_s[i]<=102))
            {hex=true; break;}
        }
        if(hex)
            return convert_from_hexadecimal_to_decimal(location_of_memory_s);
        else
            return stoi(location_of_memory_s);
    }
    return 0;
}
int convert_from_hexadecimal_to_decimal(string hex_val) {  //convert from hex decimal value to decimal value
    if(hex_val[hex_val.size()-1]=='h' || hex_val[hex_val.size()-1]=='H')
        hex_val=hex_val.substr(0,hex_val.size()-1);
    int len = hex_val.size();
    int base = 1;
    int dec_val = 0;
    for (int i = len - 1; i >= 0; i--) {
        if (hex_val[i] >= '0' && hex_val[i] <= '9') {
            dec_val += (hex_val[i] - 48) * base;
            base *= 16;
        } else if ((hex_val[i] >= 'a' && hex_val[i] <= 'f')) {
            dec_val += (hex_val[i] -87) * base;
            base*= 16;
        }else if((hex_val[i] >= 'A' && hex_val[i] <= 'F')){
            dec_val +=(hex_val[i]- 55)*base;
            base*=16;
        }
    }
    return dec_val;
}
int return_value_of_right_hand_side(int index) { //this funciton returns the value of the rest of comma int the line by eliminating staffs
    vector<string>::iterator x, y, x1, y1, bit16_with_breaked, bit16_with_b_breaked, bit16_with_w_breaked, reg;

    x = find(bit16_array.begin(), bit16_array.end(), line_array[index]);
    x1 = find(bit8_array.begin(), bit8_array.end(), line_array[index]);

    bit16_with_breaked = find(bit16_array_with_breaked.begin(), bit16_array_with_breaked.end(), line_array[index]);
    bit16_with_w_breaked = find(bit16_array_with_w_and_breaked.begin(), bit16_array_with_w_and_breaked.end(),
                                line_array[index]);
    bit16_with_b_breaked = find(bit16_array_with_b_and_breaked.begin(), bit16_array_with_b_and_breaked.end(),
                                line_array[index]);
    int asci_value;
    if(line_array[index].at(0)=='\'') //checks is it char or not
        asci_value=line_array[index][1];
    else asci_value=line_array[index][0];
    int keep_index=0;
    string variable = "";
    bool is_it_variable=false; string var_type="";
    for (auto const variable_temp:variable_array) { //traverse of var array to obtain name and type
        if (variable_temp.first == line_array[(line_array[index]=="w" || line_array[index]=="b") ? (index+1) : index]){
            variable = variable_temp.first;
            is_it_variable=true;
            var_type=variable_temp.second;
            break;
        }
        keep_index++;
    }
    int ooo=keep_index;
    if(!index_keeper.empty())
        keep_index=index_keeper[keep_index];
    if (line_array[index] == "offset")//check this addres of the anything
    {
        if (line_array[3].at(0) == 'w' || line_array[3].at(0) == 'b' || (line_array[3].at(line_array[3].size() - 1) == ']' || line_array[3].at(0) == '[')){
            string s=line_array[3].substr(line_array[3].find_first_of('[')+1,line_array[3].length());
            s=s.substr(0,s.find_last_of(']'));
            if (line_array[3].at(line_array[3].size() - 2) == 'h' || line_array[3].at(line_array[3].size() - 2) == 'H' || s[0]=='0'){
                return convert_from_hexadecimal_to_decimal(s);
            }
            else if((s[s.size()-1]=='d'||s[s.size()-1]=='D')&&s[0]!='0'){
                s=s.substr(0,s.size()-2);
                return stoi(s);
            }else return stoi(s);

        }
        string n = "";
        int variable_index = 0; bool var_is=false;
        for (auto const l:variable_array) { //traverse to find addres of var
            if (l.first ==line_array[(line_array[index + 1] != "w" || line_array[index + 1] != "b") ? index + 1 : index + 2]) {
                n = l.first;
                var_is=true;
                break;
            }
            variable_index++;
        }
        if(!index_keeper.empty())
            variable_index=index_keeper[variable_index];
        if (var_is)
            return variable_index;

        return -1;
    }
    if (x != bit16_array.end() || x1 != bit8_array.end()) //reg-reg --- mov ax,cx || mov al,cl
    {
        if (x != bit16_array.end() && is_bit16) //is it 16 bit reg
            return *return_pointer(line_array[index]);
        else if(x1 != bit8_array.end() && is_bit8) //is it 8 bit reg
            return *get8Bit(line_array[index]);
        else return -1;
    }
    if (bit16_with_breaked !=
        bit16_array_with_breaked.end()) //reg-b[reg] ---mov ah, b[bx] ----- //reg-[reg] --mov cx,[bx] -- mov di,[si] -- mov ax,[di] ---//reg-w[reg]-- mov ax, w[bx]
    {
        unsigned short *ptr;string s = "";s.push_back(line_array[index].at(1));s.push_back(line_array[index].at(2));
        ptr = return_pointer(s);
        return memory[*ptr] + ((*ptr + 1) < pow(2, 16) ? (memory[*ptr + 1] << 8) : 0);
    } else if (bit16_with_b_breaked != bit16_array_with_b_and_breaked.end() ||
               (bit16_with_w_breaked != bit16_array_with_w_and_breaked.end())) {
        unsigned short *ptr;string s = "";s.push_back(line_array[index].at(2));s.push_back(line_array[index].at(3));
        ptr = return_pointer(s);
        if (bit16_with_w_breaked != bit16_array_with_w_and_breaked.end())
            return memory[*ptr] + ((*ptr + 1) < (pow(2, 16)) ? (memory[*ptr + 1] << 8) : 0);
        else
            return memory[*ptr];
    } else if ((line_array[index].at(0) == '['  || ((line_array[index].at(0) == 'w' || line_array[index].at(0) == 'b')))
               &&(line_array[index].at(line_array[index].size() - 1) == ']'))     //reg-memory|w[memory]|b[memory]
    {
        if (line_array[index].at(0) == 'b') {  //is it byte
            int location=0;  string gg=line_array[index].substr(line_array[index].find_first_of('[')+1,line_array[index].length());
            gg=gg.substr(0,gg.find_last_of(']'));
            if (is_it_hex(gg))
                location=convert_from_hexadecimal_to_decimal(gg);
            else if((gg[gg.size()-1] == 'd' || gg[gg.size()-1] == 'D'))
                location=stoi(gg);
            if(location<0 || location>(2<<15))
                return -1;
            return memory[location];
        } else if (line_array[index].at(0) == 'w' || line_array[index].at(0) == '['){  //is it word size ?
            int location;
            int value; string gg=line_array[index].substr(line_array[index].find_first_of('[')+1,line_array[index].length());
            gg=gg.substr(0,gg.find_last_of(']'));
            if (is_it_hex(gg)){
                location=convert_from_hexadecimal_to_decimal(gg);
            }
            else if (gg[gg.size()-1] == 'd' || gg[gg.size()-1] == 'D'){
                location=stoi(gg.substr(0,gg.size()-1));
            }else location=stoi(gg);
            if(location<0 || location>(2<<15))
                return -1;
            value=memory[location]+(memory[location+1]<<8);
            return value;
        }
    }
    else if(is_it_variable){ //is it variable
        if((is_bit16))
            return memory[keep_index] + ((((keep_index + 1) < (pow(2, 16)))) ? (memory[keep_index + 1] << 8): 0);
        else if((is_bit8))
            return memory[keep_index];
        else return -1;
    }
    else if ((line_array[index] == "w" || line_array[index] == "b" ))// sayı
    {
        int asci_2=0;
        if(line_array[index+1][0]=='\'' || line_array[index+1][0]!='\"') //is it char --  mov ax,'a'
            return line_array[index + 1][1];
        else asci_2=line_array[index+1][0];
        if (asci_2 >= 48 && asci_2 <= 57)//bu bir sayi --mov [01h],02h //not b
        {
            if (is_it_hex(line_array[index+1]))
                return convert_from_hexadecimal_to_decimal(line_array[index + 1]);
            else if (line_array[index + 1].at(line_array[index + 1].size() - 1) == 'd' || line_array[index + 1].at(line_array[index + 1].size() - 1) == 'D') {
                string ss = line_array[index + 1].substr(0, line_array[index + 1].size() - 1);
                return stoi(ss);
            } else return stoi(line_array[index + 1]);
        }
    }
    else if (line_array[index][0]!='\'' && asci_value>=48 && asci_value<=57 )//bu bir sayi --mov [01h],02h //not b
    {
        if (is_it_hex(line_array[index]))
            return convert_from_hexadecimal_to_decimal(line_array[index]);
        else if ((line_array[index].at(line_array[index].size() - 1) == 'd' || line_array[index].at(line_array[index].size() - 1) == 'D')) {
            string ss = line_array[index].substr(0, line_array[index].size() - 1);
            return stoi(ss);
        }else
            return stoi(line_array[index]);
    }
    else if (line_array[index] [0]=='\'' || line_array[index][0]=='\"') {
        //is it char --  mov ax,'a'
        return asci_value;
    }
    else return -1;
    return 0;
}
void change_flags(int value_1, int value_2) { //change flags with according to in the case of each other
    if (value_1 == value_2) {
        ZF = 1;
        CF = 0;
    } else if (value_1 > value_2) {
        ZF = CF = 0;
    } else {
        ZF = 0;
        CF = 1;
        SF=  1;
        AF=  1;
    }
}
void change_flags_only_one_paramether(int value){ //change flaggs according to 0
    if (value == 0) {
        ZF = 1;
        SF = 0;
    } else if (value < 0) {
        SF = 1;
        ZF = 0;
        if((value)<(-16))
            CF = 1;
        AF = 1;
    } else ZF = SF = 0;
}
bool change_flags_with_byte(int byte,int sayi) { //change flags
    if (pow(2, byte) < sayi){
        CF=AF=1;
        return true;
    }else{CF=AF=0;}
    return false;
}
int return_value() {  //retur value for rcl-rcr-shl-shr
    int value;
    if (line_array[1] == "w" || line_array[1] == "b") {
        if (line_array[3] == "cl")
            value = *get8Bit(line_array[3]);
        else {
            if (line_array[3].at(0) == '0' && line_array[3].at(line_array[3].size() - 1) == 'h')
                value = convert_from_hexadecimal_to_decimal(line_array[3]);
            else if (line_array[3].at(line_array[3].size() - 1) == 'd')
                value = stoi(line_array[3].substr(0, line_array[3].size() - 1));
            else {
                try {
                    value=stoi(line_array[3]);
                }catch (exception e){
                    throw "Unsupported Number Type";
                }
            }
        }
        if (value > 31) {
            return -1;
        }
    }
    else {
        if (line_array[2] == "cl")
            value = *get8Bit(line_array[2]);
        else {
            if (line_array[2].at(0) == '0' || line_array[2].at(line_array[2].size() - 1) == 'h')
                value = convert_from_hexadecimal_to_decimal(line_array[2]);
            else if (line_array[2].at(line_array[2].size() - 1) == 'd')
                value = stoi(line_array[2].substr(0, line_array[2].size() - 1));
            else {
                try {
                    value=stoi(line_array[2]);
                }catch (exception e){
                    throw "Unsupported Number Type";
                }
            }
            if (value > 31) {
                return -1;
            }
        }
    }
    return value;
}
template <class type>
void helper_or_and_xor(type * left_value,int right_value,string process){  //helper funciton for or_xor_and funcitons
    if (process == "or")
        *left_value |= right_value;
    else if (process == "xor")
        *left_value ^= right_value;
    else if (process == "and")
        *left_value &= right_value;
}
int xor_or_and(string s) {
    OF = CF = 0;
    string variable = "";
    unsigned char variable_value;
    string variable_type = "";
    int keep_index = 0;
    bool is_variable=false;
    for (auto const variable_temp:variable_array) {  //traverse to find name and type of var
        if (variable_temp.first == line_array[(line_array[1]=="b" || line_array[1]=="w") ? 2 : 1]) {
            variable = variable_temp.first;
            variable_type = variable_temp.second;
            is_variable=true;
            break;
        }
        keep_index++;
    }
    if(!index_keeper.empty())
        keep_index=index_keeper[keep_index];
    vector<string>::iterator x, y, x1, y1, bit16_with_breaked, bit16_with_b_breaked, bit16_with_w_breaked, reg;

    x = find(bit16_array.begin(), bit16_array.end(), line_array[1]);
    x1 = find(bit8_array.begin(), bit8_array.end(), line_array[1]);

    bit16_with_breaked = find(bit16_array_with_breaked.begin(), bit16_array_with_breaked.end(), line_array[1]);
    bit16_with_w_breaked = find(bit16_array_with_w_and_breaked.begin(), bit16_array_with_w_and_breaked.end(),
                                line_array[1]);
    bit16_with_b_breaked = find(bit16_array_with_b_and_breaked.begin(), bit16_array_with_b_and_breaked.end(),
                                line_array[1]);



    if (x != bit16_array.end() || x1 != bit8_array.end()) {  //reg is 8 or 16 bit
        if (x != bit16_array.end()) {
            is_bit16=true;
            unsigned short *ptr;
            ptr = return_pointer(line_array[1]);
            int value=return_value_of_right_hand_side(2);
            if(value==-1)return -1;
            helper_or_and_xor(ptr,value,s);
            change_flags_only_one_paramether(*ptr);
        } else {
            is_bit8=true;
            unsigned char *ptr;
            ptr = get8Bit(line_array[1]);
            int value = return_value_of_right_hand_side(2);
            if(value==-1)return -1;
            helper_or_and_xor(ptr,value,s);
            change_flags_only_one_paramether(*ptr);
        }
    } else if (bit16_with_b_breaked != bit16_array_with_b_and_breaked.end() ||
               bit16_with_breaked != bit16_array_with_breaked.end() ||
               bit16_with_w_breaked != bit16_array_with_w_and_breaked.end()) { //is it breaked reg

        string ss ="";  s=line_array[1].substr(line_array[1].find_first_of('[')+1,line_array[1].length());
        s=s.substr(0,s.find_last_of(']'));
        unsigned short *ptr;
        ptr = return_pointer(ss);
        if(bit16_with_b_breaked != bit16_array_with_b_and_breaked.end())
            is_bit8=true;
        else is_bit16=true;
        int value = return_value_of_right_hand_side(2);
        int toplam = memory[*ptr] + (*ptr + 1 < (pow(2, 16))) ? (memory[*ptr + 1] << 8) : 0;
        if (s == "or")
            toplam |= value;
        else if (s == "xor")
            toplam ^= value;
        else if (s == "and")
            toplam &= value;
        change_flags_only_one_paramether(toplam);
        int headof_8 = toplam >> 8;
        int tail_8 = last_8bit(toplam);
        memory[*ptr] = tail_8;
        if ((*ptr + 1 < (pow(2, 16)))&&(bit16_with_b_breaked == bit16_array_with_b_and_breaked.end()))
            memory[*ptr + 1] = headof_8;
    } else if ((line_array[1].at(0) == '[' || line_array[1][0]=='w' || line_array[1][0]=='b')&& line_array[1].at(line_array[1].size() - 1) == ']') {  //is it memory
        string s=line_array[1].substr(line_array[1].find_first_of('[')+1,line_array[1].length());s=s.substr(0,s.find_last_of(']'));
        int sayi;
        if (is_it_hex(s))//is hexadecimal value
            sayi = convert_from_hexadecimal_to_decimal(s);
        else if (s[s.size()-1] == 'd' || s[s.size()-1]=='D') //is decimal index
            sayi = stoi(s);
        else
            sayi = stoi(s);
        if((line_array[1].at(0) == '[' || line_array[1][0]=='w')) //is word operation ?
            is_bit16=true;
        else is_bit8=true;                                                  //is byte operation ?
        int value = return_value_of_right_hand_side(2);
        if(value==-1)
            return -1;
        int k=0;
        if((line_array[1].at(0) == '[' || line_array[1][0]=='w'))
            k = memory[sayi] + ((sayi + 1 < (pow(2, 16))) ? (memory[sayi + 1] << 8) : 0); //word value
        else k = memory[sayi];      //byte value
        if (s == "or")
            k |= value;
        else if (s == "xor")
            k ^= value;
        else if (s == "and")
            k &= value;
        change_flags_only_one_paramether(k);        //modify flags
        int headof_8 = (k >> 8);
        int tail_8 = last_8bit(k);
        memory[sayi] = tail_8;
        if ((sayi + 1 < (pow(2, 16)))&&(line_array[1].at(0) == '[' || line_array[1][0]=='w'))  //word push operation
            memory[sayi + 1] = headof_8;
    }
    else if (is_variable)//or b var , bl//is it variable
    {
        vector<string>::iterator reg_bit16 = find(bit16_array.begin(), bit16_array.end(),line_array[(line_array[1] == "w") ? 3 : 2]);
        vector<string>::iterator reg_bit8 = find(bit8_array.begin(), bit8_array.end(),line_array[(line_array[1] == "b") ? 3 : 2]);
        if (reg_bit16 != bit16_array.end() && (variable_type=="dw" || line_array[1]=="w") && line_array[1]!="b") {  //it it word variable
            int value_of_memory = memory[keep_index] + ((keep_index + 1) < (pow(2, 16)) ? (memory[keep_index + 1] << 8) : 0);
            unsigned short *ptr;
            ptr = return_pointer(*reg_bit16.base());  //return pointer
            if (s == "or") {
                value_of_memory |= *ptr;
            } else if (s == "xor") {
                value_of_memory ^= *ptr;
            } else if (s == "and") {
                value_of_memory &= *ptr;
            }
            int headof_8 = (value_of_memory >> 8);
            int  tail_8 = last_8bit(value_of_memory);
            memory[keep_index] = tail_8;
            if (keep_index + 1 < (pow(2, 16)))
                memory[keep_index + 1] = headof_8;
            change_flags_only_one_paramether(value_of_memory);
        } else if (reg_bit8 != bit8_array.end()&&(variable_type=="db" || line_array[1]=="b") && line_array[1]!="w" ) {  //is it byte variable
            unsigned char *ptr;
            ptr = get8Bit(*reg_bit8.base());
            int value_of_memory = memory[keep_index];
            if (s == "or")
                value_of_memory |= *ptr;
            else if (s == "xor")
                value_of_memory ^= *ptr;
            else if (s == "and")
                value_of_memory &= *ptr;
            memory[keep_index] = value_of_memory;
            change_flags_only_one_paramether(value_of_memory);
        }else return -1;
    }else return -1;
    return 0;
}
int last_8bit(int o){ //the least significant 8 bit
    int i=o>>8;
    int k=i<<8;
    int l=~k;
    int m=l&o;
    return m;
}
int last_16bit(int p){  //the most significant 16 bit
    int i=p>>16;
    int k=i<<16;
    int l=~k;
    int m=l&p;
    return m;
}
string convert_from_decimal_to_binary(int value) {  //convert value into decimal form
    int binaryNum[32];
    string s = "";
    int i = 0;
    while (value > 0) {
        binaryNum[i] = value % 2;
        value = value / 2;
        i++;
    }
    for (int j = i - 1; j > -1; j--)
        s.push_back(binaryNum[j]);
    return s;
}
bool is_it_hex(string s){
    if(s[s.size()-1]=='h' || s[s.size()-1]=='H')
        return true;
    for (int i = 0; i < s.size(); ++i) {
        if(i==s.size()-1){
            if(s[i]=='d'||s[i]=='D')
                return false;
        }
        if((s[i]>=65 && s[i]<=70)||(s[i]>=97&&s[i]<=102))
            return true;
    }
    return false;
}
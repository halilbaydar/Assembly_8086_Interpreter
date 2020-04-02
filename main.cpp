#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "fstream"
#include <string>
using namespace std;
unsigned short *return_pointer(string s);
unsigned char *get8Bit(string s);
int calculate_index(int arr_num);
int hec_to_dec(string tmpstr);
int convert_from_hexadecimal_to_decimal(string hex_val);
int return_value();
int mov(int i);
int dec(int i);
int inc_dec(int i);
int add(int i);
int sub(int i);
int last_8bit(int o);
int last_16bit(int p);
string convert_from_decimal_to_binary(int i);
int return_value_of_right_hand_side(int index);
void change_flags(int value_1, int value_2);
int control_overflow(int a, int b, int byte);
void change_flags_only_one_paramether(int value);
bool change_flags_wiht_byte(int byte,int sayi);
int xor_or_and(string s);
int JZ_JE(int i);
int JNE_JNZ(int i);
int JA_JNBE(int i);
int JB_JC_JNAE(int i);
int JBE(int i);
int JNB_JAE(int i);
int JNC(int i);
int div(int i);
int mul(int i);
int JMP(int i);
int no_t(int i);
int x_or(int i);
int o_r(int i);
int an_d(int i);
int rcr(int i);
int shl(int i);
int rcl(int i);
int push(int i);
int nop(int i);
int pop(int i);
char int_21h(int i, ofstream &outfile,ifstream& infile);
int shr(int i);
int compare(int i);
void nop();
unsigned short ax = 0;
unsigned short bx = 0;
unsigned short cx = 0;
unsigned short dx = 0;
unsigned short di = 0;
unsigned short si = 0;
unsigned short bp = 0;
unsigned short sp = (2 << 15) - 2;
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
unsigned char *psh = (unsigned char *) (((unsigned char *) &si) + 1);
unsigned char *psl = (unsigned char *) &si;
unsigned char *pdih = (unsigned char *) (((unsigned char *) &di) + 1);
unsigned char *pdil = (unsigned char *) &di;
unsigned char *pbph = (unsigned char *) (((unsigned char *) &bp) + 1);
unsigned char *pbpl = (unsigned char *) &bp;
vector<string> line_array;
bool CF, OF, AF, ZF, SF;
vector<string> code_array;
bool is_bit16=false;
bool is_bit8=false;
bool is_reg=false;
bool is_memory=false;
bool is_var=false;
vector<string> reg_array = {"ax", "ah", "al", "bx", "bh", "bl", "cx", "ch", "cl", "dx", "dh", "dl", "si", "di", "bp"};
vector<string> bit16_array = {"ax", "bx", "cx", "dx", "si", "di", "bp", "sp"};
vector<string> bit8_array = {"al", "ah", "bl", "bh", "cl", "ch", "dl", "dh"};
vector<string> bit16_array_with_breaked = {"[bx]", "[si]", "[di]", "[bp]"};
vector<string> bit16_array_with_b_and_breaked = {"b[bx]", "b[di]", "b[si]", "b[bp]"};
vector<string> bit16_array_with_w_and_breaked = {"w[bx]", "w[si]", "w[di]", "w[bp]"};
vector<string> whole_array={"ax", "ah", "al", "bx", "bh", "bl", "cx", "ch", "cl", "dx", "dh", "dl", "si", "di", "bp","[bx]", "[si]",
                            "[di]", "[bp]","b[bx]", "b[di]", "b[si]", "b[bp]","w[bx]", "w[si]", "w[di]", "w[bp]","mov","add","sub","div",
                            "mul","xor","or","and","not","rcl","rcr","shl","shr","nop","cmp","jc","jnz","je","jne","ja","jae","jb","jbe","jnae",
                            "jnb","jnbe","jnc","jc","push","pop","jmp","dec","inc","int","code segments","code ends"};
vector<pair<string, string>> variable_array;
vector<int> index_keeper;
unsigned char memory[2 << 15];
vector<string>::iterator p;
void trim(string& s){
    string temp="";
    for (int i = 0; i < s.size(); ++i) {
            if(s[i]!=' ')
                temp+=s[i];
    }
    s=temp;
}
void tokenizer(string s){
    string left_side=s.substr(0,s.find_first_of(','));
    string right_side=s.substr(s.find_first_of(',')+1,s.length());
    int middle_of_line=s.find_first_of(',');
    string token="";
    vector<string> temp_array;
    string left_temp;
    string right_temp;
    if(left_side.find('[')!=string::npos && left_side.find(']')!=string::npos){
        if(left_side[left_side.find_first_of('[')-1]=='b' || left_side[left_side.find_first_of('[')-1]=='w'){
            left_temp=left_side.substr(left_side.find_first_of('[')-1,s.length());
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
            right_temp=s.substr(right_side.find_first_of('['),right_side.length());
            right_temp=right_temp.substr(0,right_temp.find_last_of(']')+1);
        }
    }
    trim(left_temp);
    trim(right_temp);
    for (int k = 0; k < s.size() && s != "code segment" && s != "code ends"; k++) {
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
                k= k<middle_of_line ? middle_of_line+1 : k=s.size();
                continue;
            }else if(temp_array[temp_array.size()-1]=="b" || temp_array[temp_array.size()-1]=="w"){
                token+=temp_array[temp_array.size()-1]; token+= k<middle_of_line ? left_temp : right_temp;
                vector<string>::iterator str; str=temp_array.end();
                temp_array.erase(str);
                temp_array.push_back(token);
                token.clear();
                k=  k<middle_of_line ? middle_of_line+1 : k=s.size();
            }else{
                token+= k<middle_of_line ? left_temp : right_temp;
                temp_array.push_back(token);
                k= k<middle_of_line ? middle_of_line+1 : k=s.size();
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
int main(int argc, char *argv[]) {
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
    for (vector<string>::iterator p = code_array.begin(); p != code_array.end(); p++) {
        string line = *p;
        tokenizer(line);
        if (line_array.size() > 1) {
            if (line_array[1] == "db" || line_array[1] == "dw") {
                if(line_array[0]=="label"){cout<<"Unsupported Variable Name in line : " <<i; return 0;}
                int temp_ascii;
                if(line_array[2][0]=='\'')
                    int temp_ascii= line_array[2][1];
                else temp_ascii=line_array[2][0];
                if (line_array[2][0]!='\'' && line_array[2][0]!='\"' && temp_ascii >= 48 && temp_ascii <= 57 )//bu bir sayı
                {
                    int sayi = 0;
                    if (line_array[2].at(line_array[2].size() - 1) == 'h' || line_array[2].at(0) == 0) {
                        line_array[2].substr(0, line_array[2].size() - 1);
                        sayi = hec_to_dec(line_array[2]);
                    } else {
                        if (line_array[2].at(line_array[2].size() - 1) == 'd') {
                            line_array[2] = line_array[2].substr(0, line_array[2].size() - 1);
                            sayi = stoi(line_array[2]);
                        } else {
                            sayi = stoi(line_array[2]);
                        }
                    }
                    if ((line_array[1] == "db" && sayi < 256) || line_array[1] == "dw") {
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
                } else //bu bir char
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
    for (p = code_array.begin(); p != code_array.end(); p++) {
        string line = *p;
        tokenizer(line);
        i++;
        if(line_array.size()>0){
        if (line_array.size() > 2 && line_array[0] == "mov") {
            if (mov(i) == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array[0] == "inc") {
            int a=inc_dec(1);
            if (a == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
            else if(a==1){
                cout << "Overflow in line :" << i;
                return 0;
            }
        } else if (line_array[0] == "dec") {
            int a=inc_dec(-1);
            if (a == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
            else if(a==1){
                cout << "Overflow in line :" << i;
                return 0;
            }
        } else if (line_array.size() > 2 && line_array[0] == "add") {
            int a=add(i) == -1;
            if (a==-1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array.size() > 2 && line_array[0] == "sub") {
            int a=sub(i);
            if (a== -1)
            {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array.size() > 0 && line_array[0] == "div") {
            int a = div(i);
            if (a == -1) {
                cout << "Error in line :" << i;
                return 0;
            } else if (a == 1) {
                cout << "Overflow in line : " << i;
                return 0;
            }
        } else if (line_array.size() > 0 && line_array[0] == "mul") {
            int a = mul(i);
            if (a == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array[0] == "xor") {
            if (x_or(i) == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array[0] == "or") {
            if (o_r(i) == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array[0] == "and") {
            if (an_d(i) == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array[0] == "not") {
            if (no_t(i) == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array[0] == "rcl") {
            if (rcl(i) == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array[0] == "rcr") {
            if (rcr(i) == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array[0] == "shl") {
            if (shl(i) == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array[0] == "shr") {
            if (shr(i) == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array[0] == "cmp") {
            if (compare(i) == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array[0] == "push") {
            if (push(i) == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array[0] == "pop") {
            if (pop(i) == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array[0] == "nop") {
            if (nop(i) == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array[0] == "jz" || line_array[0] == "je") {
            if (JZ_JE(i) == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array[0] == "jne" || line_array[0] == "jnz") {
            if (JNE_JNZ(i) == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array[0] == "ja" || line_array[0] == "jnbe") {
            if (JA_JNBE(i) == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array[0] == "jb" || line_array[0] == "jc" || line_array[0] == "jnae") {
            if (JB_JC_JNAE(i) == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array[0] == "jbe") {
            if (JBE(i) == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array[0] == "jnb" || line_array[0] == "jae") {
            if (JNB_JAE(i) == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array[0] == "jnc") {
            if (JNC(i) == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array[0] == "jmp") {
            if (JMP(i) == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array[0] == "int" && line_array[1] == "21h") { int_21h(i, outFile,infile); }
        else if (line_array[0] == "int" && line_array[1] == "20h") { return 0; }
        else if (line_array[0] == "code" || (line_array[0].at(line_array[0].size() - 1) == ':')){}
        line_array.clear();
        is_bit8= false;
        is_bit16= false;
        is_reg=false;
        is_memory=false;
        is_var=false;
        continue;
        }
    }
}
int mov(int i) {
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
    for (auto const variable_temp:variable_array) {
        if (variable_temp.first == line_array[(line_array[1] == "b" || line_array[1] == "w") ? 2 : 1]) {
            variable = variable_temp.first;
            variable_type = variable_temp.second;
            break;
        }
        keep_index++;
    }
    if(!index_keeper.empty())
        keep_index=index_keeper[keep_index];
    if (bit16_reg != bit16_array.end() || bit8_reg != bit8_array.end()) {
        is_reg=true;
        unsigned short *ptr = nullptr;
        unsigned char *ptr2 = nullptr;
        if (bit16_reg != bit16_array.end()) {
            ptr = return_pointer(line_array[1]);
            is_bit16=true;
            *ptr = return_value_of_right_hand_side(2);
        } else {
            ptr2 = get8Bit(line_array[1]);
            is_bit8=true;
            *ptr2 = return_value_of_right_hand_side(2);
        }
    } else if (bit16_reg_b_breaked != bit16_array_with_b_and_breaked.end() || bit16_reg_w_breaked != bit16_array_with_w_and_breaked.end()) {
        is_reg=true;
        if(bit16_reg_b_breaked != bit16_array_with_b_and_breaked.end())
            is_bit8=true;
        else is_bit16=true;
        unsigned short *ptr;
        string s = ""; s.push_back(line_array[1].at(2)); s.push_back(line_array[1].at(3));
        ptr = return_pointer(s);
        int value = return_value_of_right_hand_side(2);
        if (value == -1)
            return -1;
        if (bit16_reg_b_breaked != bit16_array_with_b_and_breaked.end())
            memory[*ptr] = value;
        else {
            if ((*ptr + 1) < (2 << 16))
                memory[*ptr + 1] = (value >> 8);
            int tail = last_8bit(value);
            memory[*ptr] = tail;
        }
    } else if (line_array[1].at(0) == '[' || ((line_array[1].at(line_array[1].size() - 1) == ']') && line_array[1].at(0) == 'w')) {
        is_bit16=true; is_memory=true;
        int location = calculate_index(1);
        int value = return_value_of_right_hand_side(2);
        if(location==-1 || value == -1)
            return -1;
        if (value < (pow(2, 16))) {
            int head_of_8 = value >> 8;
            int  tail_8 = last_8bit(value);
            memory[location] = tail_8;
            if (location + 1 < (pow(2, 16)))
                memory[location + 1] = head_of_8;
        } else return -1;
    } else if ((line_array[1].at(line_array[1].size() - 1) == ']') && line_array[1].at(0) == 'b') {
        is_bit8=true; is_memory==true;
        int location = calculate_index(1);
        int value = return_value_of_right_hand_side(2);
        if (value == -1 || value > pow(2, 8))
            return -1;
        memory[location] = value;
    } else if (line_array[1] == "b" || line_array[1] == "w" || variable == line_array[1]) {
        is_var=true;is_bit8=true;is_bit16=true;
        if (line_array[1] == "b") {
            is_bit16=false;
            int value = return_value_of_right_hand_side(3);
            if (value == -1)
                return -1;
            memory[keep_index] = value;
        } else if (variable == line_array[1] || line_array[1] == "w") {
            if(line_array[1]!="w" && variable_type=="db")
                is_bit16=false;
            if(line_array[1]!="w" && variable_type=="dw")
                is_bit8=false;
            int value = return_value_of_right_hand_side(line_array[1] == "w" ? 3 : 2);
            if (value == -1)
                return -1;
            int head_of_8 = (value >> 8);
            int tail_8 = last_8bit(value);
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
    string variable = "";
    unsigned char variable_value;
    string variable_type = "";
    int keep_index = 0;
    for (auto const variable_temp:variable_array) {
        if (variable_temp.first == line_array[(line_array[1] == "b" || line_array[1] == "w") ? 2 : 1]) {
            variable = variable_temp.first;
            variable_type = variable_temp.second;
            break;
        }
        keep_index++;
    }
    if(!index_keeper.empty())
        keep_index=index_keeper[keep_index];
    if (bit16_reg != bit16_array.end() || bit8_reg != bit8_array.end()) {
        if (bit16_reg != bit16_array.end()) {
            is_bit16= true;
            unsigned short *ptr;
            ptr = return_pointer(line_array[1]);
            if(return_value_of_right_hand_side(2)==-1)
                return -1;
            int value =*ptr + return_value_of_right_hand_side(2);
            if(change_flags_wiht_byte(16,value))
                *ptr=value%(2<<15);
            else *ptr=value;
            change_flags_only_one_paramether(*ptr);
        } else if (bit8_reg != bit8_array.end()) {
            is_bit8= true;
            unsigned char *ptr;
            ptr = get8Bit(line_array[1]);
            if(return_value_of_right_hand_side(2)==-1)
                return -1;
            int value =*ptr + return_value_of_right_hand_side(2);
            if(change_flags_wiht_byte(16,value))
                *ptr=value%(2<<15);
            else *ptr=value;
            change_flags_only_one_paramether(*ptr);
        }
    } else if (line_array[1].at(0) == '[' && line_array[1].at(line_array[1].size() - 1) == ']') {
        is_bit16=true;
        int location = calculate_index(1);
        int value = memory[location] + ((location + 1 )< (pow(2, 16)) ? (memory[location + 1] << 8) : 0);
        if(return_value_of_right_hand_side(2)==-1)
            return -1;
        int deger=value+return_value_of_right_hand_side(2);
        if(change_flags_wiht_byte(16,deger))
            deger=deger%(2<<15);
        change_flags_only_one_paramether(deger);
        int headof_8 = (deger >> 8);
        int tail_8 = last_8bit(deger);
        memory[location] = headof_8;
        if (location + 1 < (pow(2, 16)))
            memory[location + 1] = tail_8;
    } else if (line_array[1] == "b" || line_array[1] == "w" || variable == line_array[1])
        if (line_array[1] == "b") {
            is_bit8=true;
            int deger = memory[keep_index];
            if(return_value_of_right_hand_side(3)==-1)
                return -1;
            int value=deger+return_value_of_right_hand_side(3);
            if(change_flags_wiht_byte(8,value))
                value%=256;
            memory[keep_index] = value;
        } else if (line_array[1] == "w" || variable == line_array[1]) {
            is_bit16=true;
            int value = memory[keep_index] + (((keep_index + 1) < (pow(2, 16))) ? (memory[keep_index + 1] << 8) : 0);
            if(change_flags_wiht_byte(16,value))
                value%=(2<<15);
            change_flags_only_one_paramether(value);
            int head_of_8 = (value >> 8);
            int tail_8 =last_8bit( value);
            memory[keep_index] = tail_8;
            memory[keep_index + 1] = head_of_8;
        } else
            return -1;
    return 0;
}
int sub(int i) {
    //reg,memory,(var,w|b war)
    vector<string>::iterator bit16_reg = find(bit16_array.begin(), bit16_array.end(), line_array[1]);
    vector<string>::iterator bit8_reg = find(bit8_array.begin(), bit8_array.end(), line_array[1]);

    string variable = "";
    unsigned char variable_value;
    string variable_type = "";
    int keep_index = 0;
    for (auto const variable_temp:variable_array) {
        if (variable_temp.first == line_array[(line_array[1] == "b" || line_array[1] == "w") ? 2 : 1]) {
            variable = variable_temp.first;
            variable_type = variable_temp.second;
            break;
        }
        keep_index++;
    }
    if(!index_keeper.empty())
        keep_index=index_keeper[keep_index];
    if (bit16_reg != bit16_array.end() || bit8_reg != bit8_array.end()) {

        if (bit16_reg != bit16_array.end()) {
            is_bit16=true;
            unsigned short *ptr;
            ptr = return_pointer(line_array[1]);
            if(return_value_of_right_hand_side(2)==-1)
                return -1;
            *ptr -= return_value_of_right_hand_side(2);
            change_flags_only_one_paramether(*ptr);
        } else if (bit8_reg != bit8_array.end()) {
            is_bit8=true;
            unsigned char *ptr;
            ptr = get8Bit(line_array[1]);
            if(return_value_of_right_hand_side(2)==-1)
                return -1;
            *ptr -= return_value_of_right_hand_side(2);
            change_flags_only_one_paramether(*ptr);
        }
    } else if (line_array[1].at(0) == '[' && line_array[1].at(line_array[1].size() - 1) == ']') {
        is_bit16=true;
        int location = calculate_index(1);
        int value = return_value_of_right_hand_side(2);
        if(value==-1)
            return -1;
        int deger = memory[location] + ((location + 1) < (pow(2, 16)) ? (memory[location + 1] << 8) : 0);
        int result = deger - value;
        change_flags_only_one_paramether(result);
        int head_of_8 = (result >> 8);
        int  tail_8 = last_8bit(result);
        memory[location] = tail_8;
        if (location + 1 < (pow(2, 16)))
            memory[location + 1] = head_of_8;
    } else if (line_array[1] == "b" || line_array[1] == "w" || variable == line_array[1])
        if (line_array[1] == "b") {
            is_bit8=true;
            int value = memory[keep_index];
            int deger_of_right = return_value_of_right_hand_side(3);
            if(deger_of_right==-1)
                return -1;
            memory[keep_index] = value - deger_of_right;
            change_flags_only_one_paramether(memory[keep_index]);
        } else if (line_array[1] == "w" || variable == line_array[1]) {
            is_bit16=true;
            int deger = memory[keep_index] + (((keep_index + 1) < (pow(2, 16))) ? (memory[keep_index + 1] << 8) : 0);
            int value = return_value_of_right_hand_side(variable == line_array[1] ? 2 : 3);
            if(value==-1)
                return -1;
            int result=deger-value;
            change_flags_only_one_paramether(result);
            int head_of_8 = (result >> 8);
            int tail_8 = last_8bit(result);
            memory[keep_index] = tail_8;
            if (keep_index + 1 < (pow(2, 16)))
                memory[keep_index + 1] = head_of_8;
        }
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
    for (auto const variable_temp:variable_array) {
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
    if (y != bit8_array.end()) {
        unsigned char *ptr;
        ptr = get8Bit(line_array[1]);
        long long  int value = *pal * *ptr;
        if (value > 0xFF)
            value%=0xFF;
        *pax=value;
        if (*pah != 0)
            CF = OF = 1;
        else CF = 0;
    } else if (bit16_with_b_breaked != bit16_array_with_b_and_breaked.end()) {
        unsigned char *ptr;
        string s = "";s.push_back(line_array[1].at(2));s.push_back(line_array[1].at(3));
        ptr = (unsigned char *) return_pointer(s);
        long long int result = *pal * memory[*ptr];
        if (result > pow(2, 16))
            result%=(2<<15);
        *pax = result;
        change_flag_acc_to(16);
    } else if (x != bit16_array.end()) {
        long long int value = (*pdx << 16) + *pax;
        unsigned short *ptr;
        ptr = return_pointer(line_array[1]);
        value *= *ptr;
        if (value > (pow(2, 32)))
            value%=((long long)pow(2,32));
        *pdx = (value >> 16);
        *pax =last_16bit( value);
        change_flag_acc_to(32);
    } else if (bit16_with_w_breaked != bit16_array_with_w_and_breaked.end()) {
        unsigned short *ptr;
        string s = "";s.push_back(line_array[1].at(2));s.push_back(line_array[1].at(3));
        ptr = return_pointer(s);
        int deger = memory[*ptr] + (((*ptr + 1) < (pow(2, 16))) ? (memory[*ptr + 1] << 8) : 0);
        long long int result = deger * *ptr;
        if (result > pow(2, 32))
            result%=(long long)(pow(2,32));
        *pdx = (result >> 16);
        int  tail = last_16bit(result);
        *pax = tail;
        change_flag_acc_to(32);
    } else if (line_array[1].at(0) == 'w' || line_array[1].at(0) == 'b')//memory
    {
        int location;
        if (line_array[1].at(line_array[1].size() - 2) == 'h' && line_array[1].at(0) == '0')
            location = calculate_index(1);
        else if (line_array[1].at(line_array[1].size() - 2) == 'd') {
            string s = "";s.push_back(line_array[1].at(2));s.push_back(line_array[1].at(3));
            location = stoi(s);
        } else location = stoi(line_array[1]);
        if (line_array[1].at(0) == 'w') {
            long long int result =*pax * (memory[location] + (location + 1 < (pow(2, 16)) ? (memory[location + 1] << 8) : 0));
            if(result>(pow(2,32)))
                result%=(long long)(pow(2,32));
            *pdx = result >> 16;
            int  tail = last_16bit(result);
            *pax = tail;
            change_flag_acc_to(32);
        } else {
            int result = *pal * memory[location];
            if(result>(pow(2,16)))
                result%=(long long)(pow(2,16));
            *pax = result;
            change_flag_acc_to(32);
        }
    } else if (line_array[1] == "w" || line_array[1] == "b" || var_is)// sayı veya variable
    {
        if(var_is && variable==line_array[1]){
            if (line_array[1] == "w") {
                int result = *pax * ((memory[keep_index]+(memory[keep_index+1]<<8)));
                if(result>(pow(2,32)))
                    result%=(long long)(pow(2,32));
                int tail = last_16bit(result);
                *pax = tail;
                change_flag_acc_to(32);
            } else {
                int result = *pal * (memory[keep_index]);
                *pax = result;
                change_flag_acc_to(16);
            }
        }
        else{
            int asci = line_array[2].at(0);
            if (asci >= 48 && asci <= 57)//sayı
            {
                int sayi = 0;
                if (line_array[2].at(line_array[2].size() - 1) == 'h' && line_array[2].at(0) == '0')
                    sayi = calculate_index(2);
                else if (line_array[2].at(line_array[2].size() - 1) == 'd')
                    sayi = stoi(line_array[2].substr(0, line_array[2].size() - 1));
                else
                    sayi = stoi(line_array[2]);
                if (line_array[1] == "w") {
                    int result = *pax * sayi;
                    if(result>(pow(2,32)))
                        result%=(long long)(pow(2,32));
                    *pdx = (result >> 16);
                    int  tail =last_16bit( result);
                    *pax = tail;
                    change_flag_acc_to(32);
                } else {
                    int result = *pal * sayi;
                    if(result>(pow(2,16)))
                        result%=(long long)(pow(2,16));
                    *pax = result;
                    change_flag_acc_to(32);
                }
            }
        }
    } else
        return -1;
    return 0;
}
int control_overflow(int a, int b, int byte) {
    if ((((a / b) > pow(2, byte))) || ((a % b) > pow(2, byte)))
        return 1;
    return 0;
}
int div(int i) {
    int multiplier = return_value_of_right_hand_side(line_array[1] != "b" || line_array[1] != "w" ? 1 : 2);
    vector<string>::iterator x, y, x1, y1, bit16_with_breaked, bit16_with_b_breaked, bit16_with_w_breaked, reg;
    x = find(bit16_array.begin(), bit16_array.end(), line_array[1]);
    y = find(bit8_array.begin(), bit8_array.end(), line_array[1]);
    bit16_with_w_breaked = find(bit16_array_with_w_and_breaked.begin(), bit16_array_with_w_and_breaked.end(),line_array[1]);
    bit16_with_b_breaked = find(bit16_array_with_b_and_breaked.begin(), bit16_array_with_b_and_breaked.end(),line_array[1]);
    string variable = "";
    unsigned char variable_value;
    string variable_type = "";
    int keep_index = 0; bool var_is=false;
    for (auto const variable_temp:variable_array) {
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
    if (x != bit16_array.end()) {
        unsigned short *ptr;
        ptr = return_pointer(line_array[1]);
        if(ptr== nullptr || *ptr==0)
            return -1;
        int value = (*pdx << 16) + *pax;
        if (control_overflow(value, *ptr, 16) == 1)
            return 1;
        int kalan = value % *ptr;
        int bolum = value / *ptr;
        *pdx=kalan;*pax=bolum;
    } else if (y != bit8_array.end()) {
        unsigned char *ptr;
        ptr = get8Bit(line_array[1]);
        if(ptr== nullptr || *ptr==0)
            return -1;
        if (control_overflow(*pax, *ptr, 8) == 1)
            return 1;
        int kalan = *pax % *ptr;
        int bolum= *pax / *ptr;
        *pah=kalan; *pal=bolum;
    } else if (bit16_with_b_breaked != bit16_array_with_b_and_breaked.end()) {
        unsigned short *ptr;
        string s = "";s.push_back(line_array[1].at(2));s.push_back(line_array[1].at(3));
        ptr =  return_pointer(s);
        if(ptr== nullptr)
            return -1;
        if (control_overflow(*pax, memory[*ptr], 8) == 1)
            return 1;
        int kalan = *pax % memory[*ptr];
        int bolum = *pax / memory[*ptr];
        *pdx=kalan; *pax=bolum;
    } else if (bit16_with_w_breaked != bit16_array_with_w_and_breaked.end()) {
        unsigned short *ptr;
        string s = "";s.push_back(line_array[1].at(2));s.push_back(line_array[1].at(3));
        ptr = return_pointer(s);
        if(ptr== nullptr)
            return -1;
        int value = (*pdx << 16) + *pax;
        if (control_overflow(value, (memory[*ptr] + (*ptr + 1 < (2 << 16)) ? (memory[*ptr + 1] << 8) : 0), 16) == 1)
            return 1;
        int kalan = value % (memory[*ptr] + (((*ptr + 1) < (2 << 15)) ? (memory[*ptr + 1] << 8) : 0));
        int bolum = value / (memory[*ptr] + (((*ptr + 1) < (2 << 15)) ? (memory[*ptr + 1] << 8) : 0));
        *pdx=kalan;*pax=bolum;
    } else if ((line_array[1].at(0) == 'w' || line_array[1].at(0) == 'b') && line_array[1].at(1) == '[')//memory
    {
        int location;
        if (line_array[1].at(line_array[1].size() - 2) == 'h' && line_array[1].at(0) == '0')
            location = calculate_index(1);
        else if (line_array[1].at(line_array[1].size() - 2) == 'd') {
            location = stoi(line_array[1].substr(2, line_array[1].size() - 2));
        } else
            location = stoi(line_array[1]);
        if (line_array[1].at(0) == 'w') {
            int value = (*pdx << 16) + *pax;
            if (control_overflow(value,  (memory[location] +((location + 1 < (2 << 16)) ? (memory[location + 1] << 8) : 0)), 16) == 1)
                return 1;
            int sayi=(memory[location] + (((location + 1) < (2 << 16)) ? (memory[location + 1] << 8) : 0));
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
            int value = (*pdx << 16) + *pax;
            int deger = 0;
            if (line_array[1] == "w")
                deger = memory[keep_index] + (((keep_index + 1 )< (pow(2, 16))) ? (memory[keep_index + 1] << 8) : 0);
            else
                deger = memory[keep_index];
            if (control_overflow(value, deger, 16) == 1)
                return 1;
            if(deger==0 || ( value==0 && deger==0))
                return -1;
            int kalan = value % deger;
            int bolum = value / deger;
            *pdx=kalan;*pax=bolum;
        }else{
            int asci = line_array[2].at(0);
            int sayi = 0;
            if (line_array[2].at(line_array[2].size() - 1) == 'h' && line_array[2].at(0) == '0')
                sayi = calculate_index(2);
            else if (line_array[2].at(line_array[2].size() - 1) == 'd')
                sayi = stoi(line_array[2].substr(0, line_array[2].size() - 1));
            else
                sayi = stoi(line_array[2]);
            if (line_array[1] == "w") {
                int value = (*pdx << 16) + *pax;
                if (control_overflow(value, sayi, 16) == 1)
                    return 1;
                if(sayi==0 || (value==0 && sayi==0))
                    return -1;
                int kalan = value % sayi;
                int bolum = value / sayi;
                *pdx=kalan;*pax=bolum;
            } else {
                if (control_overflow(*pax, sayi, 8) == 1)
                    return 1;
                if((sayi==0) || (*pax==0 && sayi==0))
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

    if (x != bit16_array.end() || x1 != bit8_array.end()) {
        if (x != bit16_array.end()) {
            unsigned short *ptr;
            ptr = return_pointer(line_array[1]);
            *ptr = ~*ptr;
        } else {
            unsigned char *ptr;
            ptr = get8Bit(line_array[1]);
            *ptr = ~*ptr;
        }
    } else if (bit16_with_b_breaked != bit16_array_with_b_and_breaked.end() ||
               bit16_with_w_breaked != bit16_array_with_w_and_breaked.end()) {
        if (bit16_with_b_breaked != bit16_array_with_b_and_breaked.end() ||
            bit16_with_w_breaked != bit16_array_with_w_and_breaked.end()) {
            string ss = line_array[1].substr(2, line_array[1].size() - 1);
            if (bit16_with_w_breaked != bit16_array_with_w_and_breaked.end()) {
                unsigned short *ptr;
                ptr = return_pointer(ss);
                int deger = memory[*ptr] + (*ptr + 1 < (pow(2, 16))) ? (memory[*ptr + 1] << 8) : 0;
                deger = ~deger;
                int  tail =last_8bit( deger);
                memory[*ptr] = tail;
                if (*ptr + 1 < (pow(2, 16)))
                    memory[*ptr + 1] = (deger >> 8);
            } else {
                unsigned char *ptr;
                ptr = get8Bit(ss);
                memory[*ptr] = ~memory[*ptr];
            }
        }
    } else if (line_array[1].at(0) == '[' && line_array[1].at(line_array[1].size() - 1)) {
        int location = 0;
        if (line_array[1].at(line_array[1].size() - 2) == 'h') {
            location = hec_to_dec(line_array[1].substr(1, line_array[1].size() - 1));
            int deger = memory[location] + (location + 1 < (pow(2, 16))) ? (memory[location + 1] << 8) : 0;
            deger = ~deger;
            int tail = last_8bit(deger);
            memory[location] = tail;
            if (location + 1 < (pow(2, 16)))
                memory[location + 1] = (deger >> 8);
        }
    }
    return 0;
}
int x_or(int i) {
    return xor_or_and("xor") == -1 ? -1 : 0;
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
    for (auto const variable_temp:variable_array) {
        if (variable_temp.first == line_array[(line_array[1] == "b" || line_array[1] == "w") ? 2 : 1]) {
            variable = variable_temp.first;
            variable_type = variable_temp.second;
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
    if (x != bit16_array.end() || x1 != bit8_array.end()) {
        if (x != bit16_array.end()) {
            unsigned short *ptr;
            ptr = return_pointer(line_array[1]);
            for (int i = 0; i < value; i++) {
                unsigned short tt = CF << sizeof(*ptr) * 8 - 1;
                CF = *ptr & 1;
                *ptr >>= 1;
                *ptr |= tt;
            }
        } else {
            unsigned char *ptr;
            ptr = get8Bit(line_array[1]);
            for (int i = 0; i < value; i++) {
                unsigned short tt = CF << sizeof(*ptr) * 8 - 1;
                CF = *ptr & 1;
                *ptr >>= 1;
                *ptr |= tt;
            }
        }
    } else if (bit16_with_breaked != bit16_array_with_breaked.end() ||
               bit16_with_b_breaked != bit16_array_with_b_and_breaked.end() ||
               bit16_with_w_breaked != bit16_array_with_w_and_breaked.end()) {
        if (bit16_with_b_breaked != bit16_array_with_b_and_breaked.end()) {
            unsigned char *ptr;
            string s = "";
            s.push_back(line_array[1].at(2));
            s.push_back(line_array[1].at(3));
            ptr = (unsigned char *) return_pointer(s);
            for (int i = 0; i < value; i++) {
                int value = memory[*ptr] + (*ptr + 1 < (pow(2, 16))) ? memory[*ptr + 1] << 8 : 0;
                unsigned short tt = CF << sizeof(value) * 8 - 1;
                CF = value & 1;
                value >>= 1;
                value |= tt;
            }
            memory[*ptr] = value;
        } else {
            unsigned short *ptr;
            ptr = return_pointer(
                    line_array[1].substr(bit16_with_w_breaked != bit16_array_with_w_and_breaked.end() ? 2 : 1,
                                         line_array[1].size() - 1));
            for (int i = 0; i < value; i++) {
                int value = memory[*ptr] + (*ptr + 1 < (pow(2, 16))) ? (memory[*ptr + 1] << 8) : 0;
                unsigned short tt = CF << sizeof(value) * 8 - 1;
                CF = value & 1;
                value >>= 1;
                value |= tt;
            }
            int  tail = last_8bit(value);
            memory[keep_index] = tail;
            if (keep_index + 1 < (pow(2, 16)))
                memory[keep_index + 1] = (value >> 8);

        }
    } else if (line_array[1].at(0) == '[' && line_array[1].at(line_array[1].size() - 1) == ']') {
        int location = calculate_index(1);
        for (int i = 0; i < value; i++) {
            int value = memory[location] + (location + 1 < (pow(2, 16))) ? (memory[location + 1] << 8) : 0;
            unsigned short tt = CF << sizeof(value) * 8 - 1;
            CF = value & 1;
            value >>= 1;
            value |= tt;
        }
        int tail =last_8bit( value);
        memory[keep_index] = tail;
        if (keep_index + 1 < (pow(2, 16)))
            memory[keep_index + 1] = (value >> 8);
    } else if ((line_array[1] == "w" || line_array[1] == "b") && line_array[2] == variable && line_array[3] != "w" &&
               line_array[3] != "b")//shl w var,1
    {
        if (line_array[1] == "b") {
            int value = memory[keep_index];
            for (int i = 0; i < value; i++) {
                unsigned short tt = CF << sizeof(value) * 8 - 1;
                CF = value & 1;
                value >>= 1;
                value |= tt;
            }
            memory[keep_index] = value;
        } else {
            int value = memory[keep_index] + (keep_index + 1 < (pow(2, 16))) ? (memory[keep_index + 1] << 8) : 0;
            for (int i = 0; i < value; i++) {
                unsigned short tt = CF << sizeof(value) * 8 - 1;
                CF = value & 1;
                value >>= 1;
                value |= tt;
            }
            int tail = last_8bit(value);
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
    for (auto const variable_temp:variable_array) {
        if (variable_temp.first == line_array[(line_array[1] == "b" || line_array[1] == "w") ? 2 : 1]) {
            variable = variable_temp.first;
            variable_type = variable_temp.second;
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
    if (x != bit16_array.end() || x1 != bit8_array.end()) {
        if (x != bit16_array.end()) {
            unsigned short *ptr;
            ptr = return_pointer(line_array[1]);
            int a = *ptr;
            for (; a != 0; a >>= 1)
                CF = a & 1;
            *ptr >>= value;
        } else {
            unsigned char *ptr;
            ptr = get8Bit(line_array[1]);
            int a = *ptr;
            for (; a != 0; a >>= 1)
                CF = a & 1;
            *ptr >>= value;
        }
    } else if (bit16_with_breaked != bit16_array_with_breaked.end() ||
               bit16_with_b_breaked != bit16_array_with_b_and_breaked.end() ||
               bit16_with_w_breaked != bit16_array_with_w_and_breaked.end()) {
        if (bit16_with_b_breaked != bit16_array_with_b_and_breaked.end()) {
            string s = "";
            s.push_back(line_array[1].at(2));
            s.push_back(line_array[1].at(3));
            int adres = (unsigned char) *return_pointer(s);
            int deger = memory[adres];
            for (; deger != 0; deger >>= 1)
                CF = deger & 1;
            memory[adres] = deger;
        } else {
            int adres = *return_pointer(
                    line_array[1].substr(bit16_with_w_breaked != bit16_array_with_w_and_breaked.end() ? 2 : 1,
                                         line_array[1].size() - 1));
            int deger = memory[adres] + (adres + 1 < (pow(2, 16))) ? (memory[adres + 1] << 8) : 0;
            for (; deger != 0; deger >>= 1)
                CF = deger & 1;

            int tail =last_8bit( deger);
            memory[adres] = tail;
            if (adres + 1 < (pow(2, 16)))
                memory[adres + 1] = (deger >> 8);
        }
    } else if (line_array[1].at(0) == '[' && line_array[1].at(line_array[1].size() - 1) == ']') {
        int location = calculate_index(1);
        int a = memory[location] + location + 1 < (2 << 15) ? (memory[location + 1] << 8) : 0;
        for (; a != 0; a >>= 1)
            CF = a & 1;
        int tail =last_8bit( a);
        memory[location] = tail;
        if (location + 1 < (pow(2, 16)))
            memory[location + 1] = (a >> 8);
    } else if ((line_array[1] == "w" || line_array[1] == "b") && line_array[2] == variable && line_array[3] != "w" &&
               line_array[3] != "b")//shl w var,1
    {
        if (line_array[1] == "b") {
            int deger = memory[keep_index];
            for (; deger != 0; deger >>= 1)
                CF = deger & 1;

            memory[keep_index] = deger;
        } else {
            int deger = memory[keep_index] + (keep_index + 1 < (pow(2, 16))) ? (memory[keep_index] << 8) : 0;
            for (; deger != 0; deger >>= 1)
                CF = deger & 1;

            int tail = last_8bit(deger);
            memory[keep_index] = tail;
            if (keep_index + 1 < (pow(2, 16)))
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
    int keep_index = 0;
    for (auto const variable_temp:variable_array) {
        if (variable_temp.first == line_array[(line_array[1] == "b" || line_array[1] == "w") ? 2 : 1]) {
            variable = variable_temp.first;
            variable_type = variable_temp.second;
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
    if (x != bit16_array.end() || x1 != bit8_array.end()) {
        unsigned short value = return_value_of_right_hand_side(2);
        if (value == -1)
            return -1;
        if (x != bit16_array.end()) {
            unsigned short *ptr;
            ptr = return_pointer(line_array[1]);
            CF = *ptr & 1;
            *ptr <<= value;
        } else {
            unsigned char *ptr;
            ptr = get8Bit(line_array[1]);
            CF = *ptr & 1;
            *ptr <<= value;
        }
    } else if (bit16_with_breaked != bit16_array_with_breaked.end() ||
               bit16_with_b_breaked != bit16_array_with_b_and_breaked.end() ||
               bit16_with_w_breaked != bit16_array_with_w_and_breaked.end()) {
        unsigned short value = return_value_of_right_hand_side(2);
        if (value == -1)
            return -1;
        if (bit16_with_b_breaked == bit16_array_with_b_and_breaked.end()) {
            unsigned short *ptr;
            string s = "";
            s.push_back(line_array[1].at(2));
            s.push_back(line_array[1].at(3));
            ptr = return_pointer(s);
            int deger = memory[*ptr] + *ptr + 1 < (pow(2, 16)) ? (memory[*ptr + 1] << 8) : 0;
            CF = deger & 1;
            deger <<= value;
            int  tail = last_8bit(deger);
            memory[*ptr] = tail;
            if (*ptr + 1 < (pow(2, 16)))
                memory[*ptr + 1] = (deger >> 8);
        } else {
            unsigned char *ptr;
            string s = "";
            s.push_back(line_array[1].at(2));
            s.push_back(line_array[1].at(3));
            ptr = (unsigned char *) return_pointer(s);
            int deger = memory[*ptr];
            CF = deger & 1;
            deger <<= value;
            memory[*ptr] = deger;
        }
    } else if (line_array[1].at(0) == '[' && line_array[1].at(line_array[1].size() - 1) == ']') {
        int location = calculate_index(1);
        unsigned short value = return_value_of_right_hand_side(2);
        if (value == -1)
            return -1;
        int deger = memory[location] + location + 1 < (pow(2, 16)) ? (memory[location + 1] << 8) : 0;
        CF = deger & 1;
        deger <<= value;

        int tail = last_8bit(deger);
        memory[location] = tail;
        if (location + 1 < (pow(2, 16)))
            memory[location + 1] = (deger >> 8);
    } else if ((line_array[1] == "w" || line_array[1] == "b") && line_array[2] == variable && line_array[3] != "w" &&
               line_array[3] != "b")//shl w var,1
    {
        int value = return_value_of_right_hand_side(3);
        if (value == -1)
            return -1;
        int deger = memory[keep_index] + (keep_index + 1 < (pow(2, 16))) ? (memory[keep_index + 1] << 8) : 0;
        CF = deger & 1;
        deger <<= value;

        int  tail = last_8bit(deger);
        memory[keep_index] = tail;
        if (keep_index + 1 < (pow(2, 16)))
            memory[keep_index + 1] = (deger >> 8);
    } else return -1;
    return 0;
}
int inc_dec(int i){
    /**
     * reg ->ax,al,b[bx],w[bx]
     * memory w[01h] ,b[01h]
     * var w var,v var
     * */
    vector<string>::iterator bit16_reg = find(bit16_array.begin(), bit16_array.end(), line_array[1]);
    vector<string>::iterator bit8_reg = find(bit8_array.begin(), bit8_array.end(), line_array[1]);
    vector<string>::iterator bit16_reg_b_breaked = find(bit16_array_with_b_and_breaked.begin(),
                                                        bit16_array_with_b_and_breaked.end(), line_array[1]);
    vector<string>::iterator bit16_reg_w_breaked = find(bit16_array_with_w_and_breaked.begin(),
                                                        bit16_array_with_w_and_breaked.end(), line_array[1]);
    vector<string>::iterator bit16_reg_with_breaked = find(bit16_array_with_breaked.begin(),
                                                           bit16_array_with_breaked.end(), line_array[1]);
    if(bit8_reg!=bit8_array.end())
    {
        unsigned char *ptr;
        ptr=get8Bit(line_array[1]);
        *ptr+=i;
        change_flags_only_one_paramether(*ptr);
    }
    else if(bit16_reg!=bit16_array.end())
    {
        unsigned short *ptr;
        ptr=return_pointer(line_array[1]);
        *ptr+=i;
        change_flags_only_one_paramether(*ptr);
    }
    else if(bit16_reg_with_breaked!=bit16_array_with_breaked.end()||bit16_reg_w_breaked!=bit16_array_with_w_and_breaked.end()||line_array[1].at(0)=='w'){
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
    else if(bit16_reg_b_breaked!=bit16_array_with_b_and_breaked.end()||line_array[1].at(0)=='b'||line_array[1].at(0)=='[')
    {
        if(bit16_reg_b_breaked!=bit16_array_with_b_and_breaked.end()||line_array[1].at(0)=='b'){
            int location;
            if(bit16_reg_b_breaked!=bit16_array_with_b_and_breaked.end())
                location=*get8Bit(line_array[1].substr(2,line_array[1].size()-1));
            else
                location=calculate_index(1);
            memory[location]+=i;
            change_flags_only_one_paramether(memory[location]);
        }
        else{
            int location=calculate_index(1);
            int value=memory[location]+((location+1)<(2<<15))?(memory[location+1]<<8) : 0;
            value+=i;
            change_flags_only_one_paramether(value);
            memory[location]=last_8bit(value);
            if((location+1)<(2<<15))
                memory[location+1]=(value>>8);
        }
    }
    else if(line_array[1]=="w" || line_array[1]=="b")
    {
        int index;
        for (auto const st:variable_array) {
            if(st.first==line_array[2])
                if(st.second.at(1)!=line_array[1].at(0))
                    return -1;
            index++;
        }
        if(!index_keeper.empty())
            index=index_keeper[index];
        if(line_array[1]=="b"){
            memory[index]+=i;
            change_flags_only_one_paramether(memory[index]);
        }
        else
        {
            int value=memory[index]+((index+1)<(2<<15))?(memory[index+1]<<8) : 0;
            value+=i;
            change_flags_only_one_paramether(value);
            memory[index]=last_8bit(value);
            if((index+1)<(2<<15))
                memory[index+1]=(value>>8);
        }
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
    int keep_index = 0;
    for (auto const variable_temp:variable_array) {
        if (variable_temp.first == line_array[(line_array[1] == "b" || line_array[1] == "w") ? 2 : 1]) {
            variable = variable_temp.first;
            variable_type = variable_temp.second;
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
    if (x != bit16_array.end() || x1 != bit8_array.end()) {
        if (x != bit16_array.end()) {
            unsigned short *ptr;
            ptr = return_pointer(line_array[1]);
            for (int i = 0; i < value; i++) {
                bool k = CF;
                //CF = convert_from_decimal_to_binary(*ptr).at(0) - '0';
                CF=(*ptr>>15);
                *ptr <<= 1;
                *ptr |= k;
            }
        } else {
            unsigned char *ptr;
            ptr = get8Bit(line_array[1]);
            for (int i = 0; i < value; i++) {
                bool k = CF;
                //CF = convert_from_decimal_to_binary(*ptr).at(0) - '0';
                CF=(*ptr>>15);
                *ptr <<= 1;
                *ptr |= k;
            }
        }
    } else if (bit16_with_breaked != bit16_array_with_breaked.end() ||
               bit16_with_b_breaked != bit16_array_with_b_and_breaked.end() ||
               bit16_with_w_breaked != bit16_array_with_w_and_breaked.end()) {
        if (bit16_with_b_breaked == bit16_array_with_b_and_breaked.end()) {
            unsigned short *ptr;
            ptr = return_pointer(line_array[1].substr(bit16_with_breaked != bit16_array_with_breaked.end() ? 1 : 2,
                                                      line_array[1].size() - 1));
            int deger = memory[*ptr] + *ptr + 1 < (pow(2, 16)) ? (memory[*ptr + 1] << 8) : 0;
            for (int i = 0; i < value; i++) {
                bool k = CF;
                //CF = convert_from_decimal_to_binary(deger).at(0) - '0';
                CF=(deger>>15);
                deger <<= 1;
                deger |= k;
            }
            int tail = last_8bit(deger);
            memory[*ptr] = tail;
            if (*ptr + 1 < (pow(2, 16)))
                memory[*ptr + 1] = (deger >> 8);

        } else {
            unsigned char *ptr;
            string s = "";
            s.push_back(line_array[1].at(2));
            s.push_back(line_array[1].at(3));
            ptr = get8Bit(s);
            int deger = memory[*ptr];
            for (int i = 0; i < value; i++) {
                bool k = CF;
                CF = convert_from_decimal_to_binary(deger).at(0) - '0';
                deger <<= 1;
                deger |= k;
            }
            memory[*ptr] = deger;
        }
    } else if (line_array[1].at(0) == '[' && line_array[1].at(line_array[1].size() - 1) == ']') {
        int location = calculate_index(1);
        if (location == -1)
            return -1;
        for (int i = 0; i < value; i++) {
            bool k = CF;
            int deger = memory[location] + (location + 1 < (pow(2, 16))) ? (memory[location + 1] << 8) : 0;
            CF = convert_from_decimal_to_binary(
                    memory[location] + (location + 1 < (pow(2, 16))) ? (memory[location + 1] << 8) : 0).at(0) - '0';
            deger <<= 1;
            deger |= k;
            string ss = convert_from_decimal_to_binary(deger);
            int tail = last_8bit(deger);
            memory[keep_index] = tail;
            if (keep_index + 1 < (pow(2, 16)))
                memory[keep_index + 1] = (deger >> 8);
        }
    } else if ((line_array[1] == "w" || line_array[1] == "b") && line_array[2] == variable && line_array[3] != "w" &&
               line_array[3] != "b")//shl w var,1
    {
        for (int i = 0; i < value; i++) {
            bool k = CF;
            int deger = memory[keep_index] + (keep_index + 1 < (pow(2, 16))) ? (memory[keep_index + 1] << 8) : 0;
            CF = convert_from_decimal_to_binary(
                    memory[keep_index] + (keep_index + 1 < (pow(2, 16))) ? (memory[keep_index + 1] << 8) : 0).at(0) -
                 '0';
            deger <<= 1;
            deger |= k;
            string ss = convert_from_decimal_to_binary(deger);
            int tail = last_8bit(deger);
            if (keep_index + 1 < (pow(2, 16)))
                memory[keep_index + 1] = (deger >> 8);

        }
    } else
        return -1;
    return 0;
}
int push(int i) {
    is_bit16=true;
    if (return_value_of_right_hand_side(1) < (2 << 15)) {
        int head_8bit = (return_value_of_right_hand_side(1) >> 8);
        int value = return_value_of_right_hand_side(1);
        int tail_8 =last_8bit( value);
        memory[sp] = head_8bit;
        memory[sp - 1] = tail_8;
        sp -= 2;
    } else
        return -1;
    return 0;
}
int nop(int i) {
    return 0;
}
int pop(int i) {
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
    if (*pah == 1) {
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

    } else if (*pah == 2) {
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

    int asci = line_array[1].at(0);

    //memory-reg_addres yok
    /*                ZF CF
    Destination < source 0 1
         Destination > source 0 0
    Destination = source 1 0
*/
    string n = "";
    int variable_index = 1;
    unsigned short variable_value;
    string type;
    for (auto const l:variable_array) {
        if (l.first == line_array[(line_array[1] == "b" || line_array[1] == "w") ? 2 : 1]) {
            n = l.first;
            type = l.second;
            break;
        }
        variable_index++;
    }
    if(!index_keeper.empty())
        variable_index=index_keeper[variable_index];
    if (bit16_with_b_breaked != bit16_array_with_b_and_breaked.end() ||
        bit16_with_w_breaked != bit16_array_with_w_and_breaked.end() ||
        bit16_with_breaked != bit16_array_with_breaked.end()) {
        int oo = return_value_of_right_hand_side(2);
        if (oo == -1)
            return -1;
        if ((bit16_with_breaked != bit16_array_with_breaked.end() || ((bit16_with_w_breaked != bit16_array_with_w_and_breaked.end()) && line_array[2] != "b"))) {
            is_bit16=true;
            string ss = line_array[1].substr(bit16_with_breaked != bit16_array_with_breaked.end() ? 1 : 2,line_array[1].size() - 1);
            int value = memory[*return_pointer(ss)] + (((*return_pointer(ss) + 1) < (pow(2, 16))) ? ( (memory[*return_pointer(ss) + 1] << 8) ): 0);
            change_flags(value, oo);
        } else if ((bit16_with_b_breaked != bit16_array_with_b_and_breaked.end() && line_array[2] != "w")) {
            is_bit8=true;
            string s = "";s.push_back(line_array[1].at(2));s.push_back(line_array[1].at(3));
            unsigned short *ptr; ptr=return_pointer(s);
            if(ptr == nullptr)
                return -1;
            change_flags(memory[*ptr], oo);
        }
    } else if (reg != reg_array.end()) {
        int deger = return_value_of_right_hand_side(2);
        if (deger == -1)
            return -1;
        if (line_array[1].at(1) != 'l' && line_array[1].at(1) != 'h') {
            is_bit8=true;
            unsigned short *ptr;
            ptr = return_pointer(line_array[1]);
            change_flags(*ptr, deger);
        } else {
            is_bit16=true;
            unsigned char *ptr;
            ptr = get8Bit(line_array[1]);
            change_flags(*ptr, deger);
        }
    } else if (line_array[1].at(0) == '[' && line_array[1].at(line_array[1].size() - 1) == ']') {
        is_bit16=true;
        string location = "";
        if (line_array[1].at(line_array[1].size() - 1) == 'h')
            location = calculate_index(1);
        else if (line_array[1].at(line_array[1].size() - 1) == 'd')
            location = line_array[1].substr(1, line_array[1].size() - 2);
        int sayi = stoi(location);
        int value = return_value_of_right_hand_side(2);
        if (sayi == -1 || value == -1)
            return -1;
        change_flags((memory[sayi] + (memory[sayi + 1] << 8)), value);
    }
    else if (line_array[1] == "b" || line_array[1] == "w" || n == line_array[1] || n == line_array[2])//cmp w 'a',ax
    {
        int value_right_hand_side = return_value_of_right_hand_side((line_array[1] == "b" || line_array[1] == "w") ? 3 : 2);
        if (value_right_hand_side == -1)
            return -1;
        change_flags(variable_value, value_right_hand_side);
        if (n == line_array[1] || n == line_array[2]) {
            int right = return_value_of_right_hand_side(n == line_array[1] ? 2 : 3);
            int value =memory[variable_index] + ((((variable_index + 1) < (pow(2, 16)))) ? (memory[variable_index + 1] << 8): 0);
            change_flags(value, right);
        } else {
            int asci_code = line_array[2].at(0);
            int right_value = return_value_of_right_hand_side(3);
            change_flags(asci_code, right_value);
        }
    } else if (asci >= 48 && asci <= 57)//sayi
    {
        int value_rigth_hand_side = return_value_of_right_hand_side(2);
        if (value_rigth_hand_side == -1)
            return -1;
        change_flags(asci, value_rigth_hand_side);
    } else if(line_array[1]=="sp"){
        int value=sp;
        change_flags(sp,value);
    }else
        return -1;
    return 0;
}
int JZ_JE(int i) {
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
int JNE_JNZ(int i) {
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
int JA_JNBE(int i) {
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
int JB_JC_JNAE(int i) {
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
int JBE(int i) {
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
int JNB_JAE(int i) {
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
    if (line_array[0] == "jnc") {
        line_array[1].push_back(':');
        vector<string>::iterator o = find(code_array.begin(), code_array.end(), line_array[1]);
        if (o == code_array.end())
            return -1;
        if (!CF)
            p = --o;
    }
    return 0;
}
int JMP(int i) {
    if (line_array[0] == "jmp") {
        line_array[1].push_back(':');
        vector<string>::iterator o = find(code_array.begin(), code_array.end(), line_array[1]);
        if (o == code_array.end())
            return -1;

        p = --o;
    }
    return 0;
}
unsigned short *return_pointer(string s) {
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
unsigned char *get8Bit(string s) {
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
int calculate_index(int arr_num) { // CALCALUTES MEMORY LOCATİON
    string location_of_memory_s = "";
    int location_of_memory=0;
    for (int i = 1; i < line_array[arr_num].size(); i++) { // memory location al
        if (line_array[arr_num][i] != ']' && line_array[arr_num][i] != '[' ) // size-1
            location_of_memory_s += line_array[arr_num][i];
    }
    if (location_of_memory_s[location_of_memory_s.size() - 1] == 'h')
        location_of_memory=hec_to_dec(location_of_memory_s);// converting from Hexadec to Dec
    else if(location_of_memory_s[location_of_memory_s.size() - 1] == 'd')
        location_of_memory=stoi(location_of_memory_s.substr(0,location_of_memory_s.size()-1));
    else
        location_of_memory = stoi(location_of_memory_s);

    if (location_of_memory > (2 << 15)) { // mem 64k
        return -1;
    }
    return location_of_memory;
}
int hec_to_dec(string tmpstr) {
    tmpstr.at(tmpstr.size() - 1) = ' '; // sondaki h yi sil
    return convert_from_hexadecimal_to_decimal(tmpstr);
}
int convert_from_hexadecimal_to_decimal(string hex_val) {
    int len = hex_val.size();
    int base = 1;
    int dec_val = 0;

    for (int i = len - 1; i >= 0; i--) {
        if (hex_val[i] >= '0' && hex_val[i] <= '9') {
            dec_val += (hex_val[i] - 48) * base;
            base = base * 16;
        } else if ((hex_val[i] >= 'a' && hex_val[i] <= 'f')) {
            dec_val += (hex_val[i] + 10 - 97) * base;
            base = base * 16;
        }
    }
    return dec_val;
}
int return_value_of_right_hand_side(int index) {
    vector<string>::iterator x, y, x1, y1, bit16_with_breaked, bit16_with_b_breaked, bit16_with_w_breaked, reg;

    x = find(bit16_array.begin(), bit16_array.end(), line_array[index]);
    x1 = find(bit8_array.begin(), bit8_array.end(), line_array[index]);

    bit16_with_breaked = find(bit16_array_with_breaked.begin(), bit16_array_with_breaked.end(), line_array[index]);
    bit16_with_w_breaked = find(bit16_array_with_w_and_breaked.begin(), bit16_array_with_w_and_breaked.end(),
                                line_array[index]);
    bit16_with_b_breaked = find(bit16_array_with_b_and_breaked.begin(), bit16_array_with_b_and_breaked.end(),
                                line_array[index]);
    int asci_value;
    if(line_array[index].at(0)=='\'')
        asci_value=line_array[index][1];
    else asci_value=line_array[index][0];
    int keep_index=0;
    string variable = "";
    bool is_it_variable=false;
    for (auto const variable_temp:variable_array) {
        if (variable_temp.first == line_array[(line_array[index]=="w" || line_array[index]=="b") ? (index+1) : index]){
            variable = variable_temp.first;
            is_it_variable=true;
            break;
        }
        keep_index++;
    }
    int ooo=keep_index;
    if(!index_keeper.empty())
        keep_index=index_keeper[keep_index];
    if (line_array[index] == "offset")//reg-adress || mov [01h], offset (w || b) v1 //returns value of v1
    {
        if (line_array[3].at(0) == 'w' || line_array[3].at(0) == 'b' ||
            (line_array[3].at(line_array[3].size() - 1) == ']' || line_array[3].at(0) == '['))
            if (line_array[3].at(line_array[3].size() - 2) == 'h')
                return hec_to_dec(line_array[3]);
            else
                return stoi(line_array[3].substr((line_array[3].at(0) == 'b' || line_array[3].at(0) == 'w') ? 2 : 1,
                                                 line_array[3].size() - 1));
        string n = "";
        int variable_index = 0;
        for (auto const l:variable_array) {
            if (l.first ==
                line_array[(line_array[index + 1] != "w" || line_array[index + 1] != "b") ? index + 1 : index + 2]) {
                n = l.first;
                break;
            }
            variable_index++;
        }
        if(!index_keeper.empty())
            variable_index=index_keeper[variable_index];
        if (n == line_array[line_array[index + 1] != "w" || "b" ? index + 1 : index + 2])
            return variable_index;
        return -1;

    }
    if (x != bit16_array.end() || x1 != bit8_array.end()) //reg-reg --- mov ax,cx || mov al,cl
    {
        if (x != bit16_array.end() && is_bit16)
            return *return_pointer(line_array[index]);
        else if(x1 != bit8_array.end() && is_bit8)
            return *get8Bit(line_array[index]);
        else return -1;
    }
    if (bit16_with_breaked !=
        bit16_array_with_breaked.end()) //reg-b[reg] ---mov ah, b[bx] ----- //reg-[reg] --mov cx,[bx] -- mov di,[si] -- mov ax,[di] ---//reg-w[reg]-- mov ax, w[bx]
    {
        if(is_var && is_bit8)
            return -1;
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
    } else if (line_array[index].at(0) == '[' || line_array[index].at(line_array[index].size() - 1) == ']' ||
               ((line_array[index].at(0) == 'w' || line_array[index].at(0) == 'b') &&
                line_array[index].size() > 1))//reg-memory|w[memory]|b[memory]
    {
        if (line_array[index].at(0) == '[')
            return memory[calculate_index(index)] + ((calculate_index(index) + 1 < (pow(2, 16))) ?
                                                     (memory[calculate_index(index) + 1] << 8) : 0);
        if (line_array[index].at(0) == 'b') {
            if (line_array[index].at(line_array[index].size() - 1) == 'h')
                return memory[hec_to_dec(line_array[index].substr(1, line_array[index].size() - 1))];
            else
                return memory[stoi(line_array[index].substr(2, line_array[index].size() - 2))];
        } else if (line_array[index].at(0) == 'w'){
            int value;
            if (line_array[index].at(line_array[index].size() - 2) == 'h'){
                string temp=line_array[index].substr(2, line_array[index].size() - 1);
                int location=hec_to_dec(temp);
                 value=memory[location]+(memory[location+1]<<8);
            }
            else if (line_array[index].at(line_array[index].size() - 2) == 'd'){
                string temp=line_array[index].substr(line_array[index].find_first_of('[')+1, line_array[index].size() - 2);
                int location=stoi(temp);
                 value=memory[location]+(memory[location+1]<<8);
            }
            return value;
        }
    }
    else if(is_it_variable){
        if(variable_array[ooo].second=="dw")
            return memory[keep_index] + ((((keep_index + 1) < (pow(2, 16)))) ? (memory[keep_index + 1] << 8): 0);
        else if(variable_array[ooo].second=="db")
            return memory[keep_index];
    }
    else if (line_array[index] == "w" || line_array[index] == "b" )// sayı
    {
        int asci_2 = line_array[index + 1][1];
        if (asci_2 < 48 || asci_2 > 57)//bu bir harf --  mov ax,'a'
            return asci_2;
        if (asci_2 >= 48 && asci_2 <= 57)//bu bir sayi --mov [01h],02h //not b
        {
            if (line_array[index + 1].at(line_array[index + 1].size() - 1) == 'h')
                return hec_to_dec(line_array[index + 1]);
            else if (line_array[index + 1].at(line_array[index + 1].size() - 1) == 'd') {
                string ss = line_array[index + 1].substr(0, line_array[index + 1].size() - 1);
                return stoi(ss);
            } else return stoi(line_array[index + 1]);
        }
    }
    else if (line_array[index].at(0)!='\'' && asci_value>=48 && asci_value<=57)//bu bir sayi --mov [01h],02h //not b
    {
        if (line_array[index].at(line_array[index].size() - 1) == 'h')
            return hec_to_dec(line_array[index]);
        else if (line_array[index].at(line_array[index].size() - 1) == 'd') {
            string ss = line_array[index].substr(0, line_array[index].size() - 1);
            return stoi(ss);
        }else
            return stoi(line_array[index]);
    }
    else if (line_array[index].at(0)=='\'') {
        //bu bir harf --  mov ax,'a'
        return asci_value;
    }
    return 0;
}
void change_flags(int value_1, int value_2) {
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
void change_flags_only_one_paramether(int value){
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
bool change_flags_wiht_byte(int byte,int sayi) {
    if (pow(2, byte) < sayi){
        CF=AF=1;
        return true;
    }
    return false;
}
int return_value() {
    int value;
    if (line_array[1] == "w" || line_array[1] == "b") {
        if (line_array[3] == "cl")
            value = *return_pointer(line_array[3]);
        else {
            if (line_array[3].at(0) == '0' && line_array[3].at(line_array[3].size() - 1) == 'h')
                value = hec_to_dec(line_array[3]);
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
            value = *return_pointer(line_array[2]);
        else {
            if (line_array[2].at(0) == '0' || line_array[2].at(line_array[2].size() - 1) == 'h')
                value = hec_to_dec(line_array[2]);
            else if (line_array[2].at(line_array[2].size() - 1) == 'd')
                value = stoi(line_array[2].substr(0, line_array[2].size() - 1));
            else {
                try {
                    value=stoi(line_array[3]);
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
int xor_or_and(string s) {
    OF = CF = 0;
    string variable = "";
    unsigned char variable_value;
    string variable_type = "";
    int keep_index = 0;
    bool is_variable=false;
    for (auto const variable_temp:variable_array) {
        if (variable_temp.first == line_array[(line_array[1] == "b" || line_array[1] == "w") ? 2 : 1]) {
            variable = variable_temp.first;
            variable_type = variable_temp.second;
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

    if (x != bit16_array.end() || x1 != bit8_array.end()) {
        if (x != bit16_array.end()) {
            unsigned short *ptr;
            ptr = return_pointer(line_array[1]);
            int value = return_value_of_right_hand_side(2);
            if (s == "or")
                *ptr |= value;
            else if (s == "xor")
                *ptr ^= value;
            else if (s == "and")
                *ptr &= value;
            change_flags_only_one_paramether(*ptr);
        } else {
            unsigned char *ptr;
            ptr = get8Bit(line_array[1]);
            int value = return_value_of_right_hand_side(2);
            if (s == "or")
                *ptr |= value;
            else if (s == "xor")
                *ptr ^= value;
            else if (s == "and")
                *ptr &= value;
            change_flags_only_one_paramether(*ptr);
        }
    } else if (bit16_with_b_breaked != bit16_array_with_b_and_breaked.end() ||
               bit16_with_breaked != bit16_array_with_breaked.end() ||
               bit16_with_w_breaked != bit16_array_with_w_and_breaked.end()) {
        string ss = line_array[1].substr((bit16_with_b_breaked != bit16_array_with_b_and_breaked.end() ||
                                          bit16_with_w_breaked != bit16_array_with_w_and_breaked.end()) ? 2 : 1,
                                         line_array[1].size() - 1);
        unsigned short *ptr;
        ptr = return_pointer(ss);
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
        if (*ptr + 1 < (pow(2, 16)))
            memory[*ptr + 1] = headof_8;
    } else if (line_array[1].at(0) == '[' && line_array[1].at(line_array[1].size() - 1) == '[') {
        int sayi;
        if (line_array[1].at(line_array[1].size() - 2) == 'h' && line_array[1].at(1) == '0')
            sayi = hec_to_dec(line_array[1].substr(1, line_array[1].size() - 1));
        else if (line_array[3].at(line_array[1].size() - 2) == 'd')
            sayi = stoi(line_array[1].substr(1, line_array[1].size() - 3));
        else
            sayi = stoi(line_array[1].substr(1, line_array[1].size() - 2));
        int value = return_value_of_right_hand_side(2);
        int k = memory[sayi] + ((sayi + 1 < (pow(2, 16))) ? (memory[sayi + 1] << 8) : 0);
        if (s == "or")
            k |= value;
        else if (s == "xor")
            k ^= value;
        else if (s == "and")
            k &= value;
        change_flags_only_one_paramether(k);
        int headof_8 = k >> 8;
        int tail_8 = last_8bit(k);
        memory[sayi] = tail_8;
        if (sayi + 1 < (pow(2, 16)))
            memory[sayi + 1] = headof_8;
    } else if (variable == line_array[(line_array[1] == "w" || line_array[1] == "b") ? 2 : 1])//or b var , bl
    {
        vector<string>::iterator reg_bit16 = find(bit16_array.begin(), bit16_array.end(),line_array[(line_array[1] == "w") ? 3 : 2]);
        vector<string>::iterator reg_bit8 = find(bit8_array.begin(), bit8_array.end(),line_array[(line_array[1] == "b") ? 3 : 2]);
        int value_of_memory = memory[keep_index] + ((keep_index + 1) < (pow(2, 16)) ? (memory[keep_index + 1] << 8) : 0);
        if (reg_bit16 != bit16_array.end()) {
            unsigned short *ptr;
            ptr = return_pointer(*reg_bit16.base());
            if (s == "or") {
                value_of_memory |= *ptr;
            } else if (s == "xor") {
                value_of_memory ^= *ptr;
            } else if (s == "and") {
                value_of_memory &= *ptr;
            }
            int headof_8 = value_of_memory >> 8;
            int  tail_8 = last_8bit(value_of_memory);
            memory[keep_index] = tail_8;
            if (keep_index + 1 < (pow(2, 16)))
                memory[keep_index + 1] = headof_8;
            change_flags_only_one_paramether(value_of_memory);
        } else if (reg_bit8 != bit8_array.end()) {
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
        }
    }
    return 0;
}
int last_8bit(int o){
    int i=o>>8;
    int k=i<<8;
    int l=~k;
    int m=l&o;
    return m;
}
int last_16bit(int p){
    int i=p>>16;
    int k=i<<16;
    int l=~k;
    int m=l&p;
    return m;
}
string convert_from_decimal_to_binary(int value) {
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
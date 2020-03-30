#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <stack>
#include <cmath>
#include "fstream"




using namespace std;
bool CF, OF, AF, ZF, SF;
//gsad
unsigned short *return_pointer(string s);

unsigned char *get8Bit(string s);

 vector<string> line_array;

int calc_memory_loc(int arr_num);

int hec_to_dec(string tmpstr);

int convert_from_hexadecimal_to_decimal(string hex_val);

bool control_of_sum(unsigned short sayi1, unsigned short sayi2, int bit);

unsigned int countSetBitsInGivenRange(unsigned int n, unsigned int l, unsigned int r);

string convert_from_decimal_to_binary(int value);

int return_value();

int mov(int i);

int add(int i);

int sub(int i);

vector<string> code_array;
static vector<string> instructor_array = {"mov", "add", "sub", "div", "mul", "xor", "or", "and", "not", "rcl", "rcr",
                                          "shl", "shr", "nop", "cmp"};
vector<string> jmp_stack_array = {"jc", "jnz", "je", "jne", "ja", "jae", "jb", "jbe", "jnae", "jnb", "jnbe", "jnc",
                                  "jc", "push", "pop"};
vector<string> reg_array = {"ax", "ah", "al", "bx", "bh", "bl", "cx", "ch", "cl", "dx", "dh", "dl", "si", "di", "bp"};
vector<string> bit16_array = {"ax", "bx", "cx", "dx", "si", "di", "bp", "sp"};
vector<string> bit8_array = {"al", "ah", "bl", "bh", "cl", "ch", "dl", "dh"};
vector<string> bit16_array_with_breaked = {"[bx]", "[si]", "[di]", "[bp]"};
vector<string> bit16_array_with_b_and_breaked = {"b[bx]", "b[di]", "b[si]", "b[bp]"};
vector<string> bit16_array_with_w_and_breaked = {"w[bx]", "w[si]", "w[di]", "w[bp]"};
vector<string> reg_array_with_breaked = {"[ax]", "[ah]", "[al]", "[bx]", "[bh]", "[bl]", "[cx]", "[ch]", "[cl]", "[dx]",
                                         "[dh]", "[dl]", "[si]", "[di]", "[bp]"};

vector<pair<string, string>> variable_array(2 << 15);
unsigned char memory[2 << 15];// 64K memory
stack<int> sp_stack;

string insert(string s, int binary);

unsigned short *return_8bit_pointer_h(string s);

unsigned short *return_8bit_pointer_l(string s);

int return_value_of_right_hand_side(int index);

void change_flags(int value_1, int value_2);

int convert_from_binary_to_decimal(string s);

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
char int_21h(int i);
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

vector<unsigned short> pointer_of_reg = {ax, bx, cx, dx, di, si, bp};

unsigned char *pah = (unsigned char *) (((unsigned char *) &ax) + 1);
unsigned char *pal = (unsigned char *) &ax;
unsigned char *pbh = (unsigned char *) (((unsigned char *) &bx) + 1);
unsigned char *pbl = (unsigned char *) &bx;
unsigned char *pch = (unsigned char *) (((unsigned char *) &cx) + 1);
unsigned char *pcl = (unsigned char *) &cx;
unsigned char *pdh = (unsigned char *) (((unsigned char *) &dx) + 1);
unsigned char *pdl = (unsigned char *) &dx;
unsigned char *psl = (unsigned char *) (((unsigned char *) &si) + 1);
unsigned char *psh = (unsigned char *) &si;
unsigned char *pdil = (unsigned char *) (((unsigned char *) &di) + 1);
unsigned char *pdih = (unsigned char *) &di;
unsigned char *pbpl = (unsigned char *) (((unsigned char *) &bp) + 1);
unsigned char *pbph = (unsigned char *) &bp;

bool sum_is = true;
bool mov_is = true;
bool sub_is = true;
bool shr_is = true;
bool shl_is = true;
bool rcl_is = true;
bool rcr_is = true;
vector<string>::iterator p;

int main(int argc, char *argv[]) {
    ofstream outFile(argv[2]);
    ifstream infile(argv[1]);
    string line_code;
    while (!infile.eof()) {
        getline(infile, line_code);
        istringstream linestream(line_code);
        code_array.push_back(line_code);
    }
    string instruction;
    int i = 0;
    for (vector<string>::iterator p = code_array.begin(); p != code_array.end(); p++) {
        string line = *p;
        for (int k = 0; k < line.size(); k++) {
            if (line[k] != ' ' && line[k] != ',' && line[k] != '\t' && line != "code segment" && line != "code ends") {
                instruction += line[k];
                if (k + 1 == line.size()) {
                    line_array.push_back(instruction);
                    instruction.clear();
                }
            } else if (instruction != "") {
                line_array.push_back(instruction);
                instruction.clear();
            }
        }
        if (line_array.size() > 1) {
            if (line_array[1] == "db" || line_array[1] == "dw") {
                int temp_ascii = line_array[2].at(0);
                if (temp_ascii >= 48 && temp_ascii <= 57)//bu bir sayı
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
                        if ((line_array[1] == "db")){
                            memory[i]=sayi;
                        }
                        else{
                            memory[i+1]=sayi>>8;
                            string ss=convert_from_decimal_to_binary(sayi);
                            int tail=0;
                            if(ss.size()>8){
                                string s=ss.substr(8,ss.size()-1);
                                tail=convert_from_binary_to_decimal(s);
                            }else tail=sayi;
                            memory[i]=tail;
                        }
                    }
                } else //bu bir char
                {
                    int k = line_array[2].at(1);
                    memory[i]=k;
                }
                variable_array[i].first=line_array[0];
                variable_array[i].second=line_array[1];
            }
        }
        line_array.clear();
        i += 6;
        continue;
    }
    i = 0;
    for (p = code_array.begin(); p != code_array.end(); p++) {
        string line = *p;
        for (int k = 0; k < line.size(); k++) {
            if (line[k] != ' ' && line[k] != ',' && line[k] != '\t') {
                instruction += line[k];
                if (k + 1 == line.size()) {
                    line_array.push_back(instruction);
                    instruction.clear();
                }
            } else if (instruction != "") {
                line_array.push_back(instruction);
                instruction.clear();
            }
        }
        if (line_array.size() > 2 && line_array[0] == "mov") {
            if (mov(i) == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        }else if(line_array[0]=="inc"){
            line_array.push_back("1");
            if (add(i) == -1)
            {
                cout << "Error in line :" << i;
                return 0;
            }
        }
        else if(line_array[0]=="dec"){
            line_array.push_back("1");
            if (sub(i) == -1)
            {
                cout << "Error in line :" << i;
                return 0;
            }
        }
        else if (line_array.size() > 2 && line_array[0] == "add") {
            if (add(i) == -1)
            {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array.size() > 2 && line_array[0] == "sub") {
            if (sub(i) == -1);
            {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array.size() > 0 && line_array[0] == "div") {
            if (div(i) == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array.size() > 0 && line_array[0] == "mul") {
            if (mul(i) == -1) {
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
        } else if (line_array[0] == "jb" ||line_array[0] == "jc" ||line_array[0] == "jnae") {
            if (JB_JC_JNAE(i) == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array[0] == "jbe") {
            if (JBE(i) == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array[0] == "jnb" || line_array[0] =="jae") {
            if (JNB_JAE(i) == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array[0] == "jnc") {
            if (JNC(i) == -1) {
                cout << "Error in line :" << i;
                return 0;
            }
        } else if (line_array[0] == "int" && line_array[1] == "21h")
        {int_21h(i);}
        else if (line_array[0] == "int" && line_array[1] == "20h")
        { return 0;}
        else if (line_array[0] == "code" || (line_array[0].at(line_array[0].size() - 1) == ':'))
        {  }
        else{
            return 0;
        }
        i++;
        line_array.clear();

    }


}


int mov(int i) {
    bool is8Bit = false;
    //reg,memory,(var,w|b war)
    vector<string>::iterator bit16_reg = find(bit16_array.begin(), bit16_array.end(), line_array[1]);
    vector<string>::iterator bit8_reg = find(bit8_array.begin(), bit8_array.end(), line_array[1]);
    vector<string>::iterator bit16_reg_b_breaked = find(bit16_array_with_b_and_breaked.begin(), bit16_array_with_b_and_breaked.end(), line_array[1]);
    vector<string>::iterator bit16_reg_w_breaked = find(bit16_array_with_w_and_breaked.begin(), bit16_array_with_w_and_breaked.end(), line_array[1]);


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
    if (bit16_reg != bit16_array.end() || bit8_reg != bit8_array.end()) {
        unsigned short *ptr = nullptr;
        unsigned char *ptr2 = nullptr;
        if (bit16_reg != bit16_array.end()) {
            ptr = return_pointer(line_array[1]);
            *ptr = return_value_of_right_hand_side(2);
        } else {
            ptr2 = get8Bit(line_array[1]);
            *ptr2 = return_value_of_right_hand_side(2);
        }
    }else if(bit16_reg_b_breaked!=bit16_array_with_b_and_breaked.end() ||bit16_reg_w_breaked!=bit16_array_with_w_and_breaked.end()){
            unsigned short *ptr;
            ptr=return_pointer(line_array[1].substr(2,line_array[1].size()-1));
            int value=return_value_of_right_hand_side(2);
            if(value==-1)
                throw "error";
            if(bit16_reg_b_breaked!=bit16_array_with_b_and_breaked.end())
                memory[*ptr]=value;
            else
            {
                if((*ptr+1)<(2<<16))
                    memory[*ptr+1]=value>>16;
                string aa=convert_from_decimal_to_binary(value);
                int tail=0;
                if(aa.size()>8)
                {
                    aa=aa.substr(8,aa.size()-1);
                    tail=convert_from_binary_to_decimal(aa);
                } else tail=value;
                memory[*ptr]=tail;
            }
    }
    else if (line_array[1].at(0) == '[' && line_array[1].at(line_array[1].size() - 1) == ']') {
        int location = calc_memory_loc(1);
        int value = return_value_of_right_hand_side(2);
        if (value == -1)
            return -1;
        if (value < (pow(2, 15))) {
            int head_of_8 = return_value_of_right_hand_side(2) >> 7;
            string binary_type = convert_from_decimal_to_binary(value);
            int tail_8 = 0;
            if (binary_type.size() > 8) {
                binary_type = binary_type.substr(8, binary_type.size() - 1);
                tail_8 = convert_from_binary_to_decimal(binary_type);
            } else tail_8 = value;
            memory[location] = tail_8;
            if (location + 1 < (pow(2, 15)))
                memory[location + 1] = head_of_8;
        } else return -1;
    } else if (line_array[1] == "b" || line_array[1] == "w" || variable == line_array[1]) {
        if (line_array[1] == "b") {
            int value = return_value_of_right_hand_side(3);
            if (value == -1)
                return -1;
            int k = control_of_sum(memory[keep_index], value, 8) ? (memory[keep_index] + value) : 0;
            if (k != 0)
                memory[keep_index] = k;
            else return -1;
        } else if (variable == line_array[1] || line_array[1] == "w") {
            int value = return_value_of_right_hand_side(line_array[1] == "w" ? 3 : 2);
            if (value == -1)
                return -1;
            int head_of_8 = value >> 7;
            string binary_type = convert_from_decimal_to_binary(value);
            int tail_8 = 0;
            if (binary_type.size() > 8) {
                binary_type = binary_type.substr(8, binary_type.size() - 1);
                tail_8 = convert_from_binary_to_decimal(binary_type);
            } else tail_8 = value;
            memory[keep_index] = tail_8;
            if (keep_index + 1 < (pow(2, 15)))
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
        }
        keep_index++;
    }
    if (bit16_reg != bit16_array.end() || bit8_reg != bit8_array.end()) {


        if (bit16_reg != bit16_array.end()) {
            unsigned short *ptr;
            ptr = return_pointer(line_array[1]);
            if (!control_of_sum(*ptr, return_value_of_right_hand_side(2), 16))
                return -1;
            *ptr += return_value_of_right_hand_side(2);
        } else if (bit8_reg != bit8_array.end()) {
            unsigned char *ptr;
            ptr=get8Bit(line_array[1]);
            if (!control_of_sum(*ptr, return_value_of_right_hand_side(2), 8)) {
                return -1;
            }
            *ptr += return_value_of_right_hand_side(2);
        }
    } else if (line_array[1].at(0) == '[' && line_array[1].at(line_array[1].size() - 1) == ']') {
        int location = calc_memory_loc(1);
        int value = memory[location] + location + 1 < (pow(2, 15)) ? memory[location + 1] << 7 : 0;
        if (!control_of_sum(value, return_value_of_right_hand_side(2), 16))
            return -1;
        value += return_value_of_right_hand_side(2);
        int headof_8 = value >> 8;
        string tail = convert_from_decimal_to_binary(value);
        int tail_8 = 0;
        if (tail.size() > 7) {
            tail = tail.substr(8, tail.size() - 1);
            tail_8 = convert_from_binary_to_decimal(tail);
        } else
            tail_8 = value;
        memory[location] = headof_8;
        if (location + 1 < (pow(2, 15)))
            memory[location + 1] = tail_8;
    }
    else if (line_array[1] == "b" || line_array[1] == "w" || variable == line_array[1])
        if (line_array[1] == "b") {
            int value = memory[keep_index];
            if (!control_of_sum(value, (unsigned char) return_value_of_right_hand_side(3), 8))
                return -1;
            int k = value + (unsigned char) return_value_of_right_hand_side(3);
            if (!CF) {
                memory[keep_index] = k;
            }
        } else if (line_array[1] == "w" || variable == line_array[1]) {
            int value = memory[keep_index] + keep_index + 1 < (pow(2, 15)) ? memory[keep_index + 1] << 7 : 0;
            if (!control_of_sum(value,
                                (unsigned char) return_value_of_right_hand_side(variable == line_array[1] ? 2 : 3), 16))
                return -1;
            int k = value + (unsigned char) return_value_of_right_hand_side(variable == line_array[1] ? 2 : 3);
            if (k != 0) {
                int head_of_8 = k >> 7;
                string binary_type = convert_from_decimal_to_binary(k);
                int tail_8 = 0;
                if (binary_type.size() > 8) {
                    binary_type = binary_type.substr(8, binary_type.size() - 1);
                    tail_8 = convert_from_binary_to_decimal(binary_type);
                } else
                    tail_8 = k;
                memory[keep_index] = tail_8;
                memory[keep_index + 1] = head_of_8;
            } else return -1;
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
        }
        keep_index++;
    }
    if (bit16_reg != bit16_array.end() || bit8_reg != bit8_array.end()) {

        if (bit16_reg != bit16_array.end()) {
            unsigned short *ptr;
            ptr = return_pointer(line_array[1]);
            *ptr -= return_value_of_right_hand_side(2);
            if (*ptr == 0)
                ZF = 1;
        } else if (bit8_reg != bit8_array.end()) {
            unsigned char *ptr;
            ptr = get8Bit(line_array[1]);
            *ptr -= return_value_of_right_hand_side(2);
            if (*ptr == 0 && sub_is)
                ZF = 1;
        }
    } else if (line_array[1].at(0) == '[' && line_array[1].at(line_array[1].size() - 1) == ']') {
        int location = calc_memory_loc(1);
        int value = return_value_of_right_hand_side(2);
        int deger = memory[location] + location + 1 < (pow(2, 15)) ? memory[location + 1] << 7 : 0;
        int result = deger - value;
        int head_of_8 = result >> 7;
        string binary_byte = convert_from_decimal_to_binary(result);
        int tail_8 = 0;
        if (binary_byte.size() > 8) {
            binary_byte = binary_byte.substr(8, binary_byte.size() - 1);
            tail_8 = convert_from_binary_to_decimal(binary_byte);
        } else tail_8 = result;
        memory[location] = tail_8;
        if (location + 1 < (pow(2, 15)))
            memory[location + 1] = head_of_8;
        if (result == 0)
            ZF = 1;
    }
    else if (line_array[1] == "b" || line_array[1] == "w" || variable == line_array[1])
        if (line_array[1] == "b") {
            int value = memory[keep_index];
            int deger_of_right = return_value_of_right_hand_side(3);
            memory[keep_index] = value - deger_of_right;
            if (memory[keep_index] == 0)
                ZF = 1;
        } else if (line_array[1] == "w" || variable == line_array[1]) {
            int deger = memory[keep_index] + keep_index + 1 < (pow(2, 15)) ? memory[keep_index + 1] << 7 : 0;
            int result = deger - return_value_of_right_hand_side(variable == line_array[1] ? 2 : 3);
            int head_of_8 = result >> 7;
            string binary_byte = convert_from_decimal_to_binary(result);
            int tail_8 = 0;
            if (binary_byte.size() > 8) {
                binary_byte = binary_byte.substr(8, binary_byte.size() - 1);
                tail_8 = convert_from_binary_to_decimal(binary_byte);
            } else tail_8 = result;
            memory[keep_index] = tail_8;
            if (keep_index + 1 < (pow(2, 15)))
                memory[keep_index + 1] = head_of_8;
            if (result == 0)
                ZF = 1;
        }
    return 0;
}

int mul(int i) {
    int multiplier = return_value_of_right_hand_side(line_array[1] != "b" || line_array[1] != "w" ? 1 : 2);
    //byte-word main issue
    vector<string>::iterator x, y, x1, y1, bit16_with_breaked, bit16_with_b_breaked, bit16_with_w_breaked, reg;

    x = find(bit16_array.begin(), bit16_array.end(), line_array[1]);
    y = find(bit8_array.begin(), bit8_array.end(), line_array[1]);
    bit16_with_w_breaked = find(bit16_array_with_w_and_breaked.begin(), bit16_array_with_w_and_breaked.end(),
                                line_array[1]);
    bit16_with_b_breaked = find(bit16_array_with_b_and_breaked.begin(), bit16_array_with_b_and_breaked.end(),
                                line_array[1]);

    if (y != bit8_array.end()) {
        unsigned char *ptr;
        ptr = get8Bit(line_array[1]);
        int value = *pal * *ptr;
        if (value > 0xFF)
            return -1;
        if (*pah != 0)
            CF = OF = 1;
    } else if (bit16_with_b_breaked != bit16_array_with_b_and_breaked.end()) {
        unsigned char *ptr;
        ptr = (unsigned char *) return_pointer(line_array[1].substr(2, line_array[1].size() - 2));
        int result = *pal * *ptr;
        if (result > pow(2, 16))

            *pax = result;
        if (*pah != 0)
            CF = OF = 1;
    }
   else  if (x != bit16_array.end()) {
        int value = (*pdx<<15)+*pax;
        unsigned short *ptr;
        ptr = return_pointer(line_array[1]);
        value *= *ptr;
        if (value > (pow(2, 32)))
            return -1;
        *pdx = value >> 16;
        string ssl = convert_from_decimal_to_binary(value);
        if (ssl.size() > 8)
            *pax = convert_from_binary_to_decimal(ssl.substr(8, ssl.size() - 1));
        else
            *pax = value;
        if (*pdx != 0)
            CF = OF = 1;
    } else if (bit16_with_w_breaked != bit16_array_with_w_and_breaked.end()) {
        unsigned short *ptr;
        ptr = return_pointer(line_array[1].substr(2, line_array[1].size() - 2));
        int deger = memory[*ptr] + (*ptr + 1 < (pow(2, 15))) ? memory[*ptr + 1] << 7 : 0;
        int result = deger * *ptr;
        if (result > pow(2,32))
            return -1;
        *pdx = result >> 15;
        string ss = convert_from_decimal_to_binary(result);
        int tail = 0;
        if (ss.size() > 16) {
            ss = ss.substr(16, ss.size() - 1);
            tail = convert_from_binary_to_decimal(ss);
        } else tail = result;
        *pax = tail;
        if (*pdx != 0)
            CF = OF = 1;
    } else if (line_array[1].at(0) == 'w' || line_array[1].at(0) == 'b')//memory
    {
        int location;
        if (line_array[1].at(line_array[1].size() - 2) == 'h' && line_array[1].at(0) == '0')
            location = calc_memory_loc(1);
        else if (line_array[1].at(line_array[1].size() - 2) == 'd')
            location = stoi(line_array[1].substr(2, line_array[1].size() - 2));
        else
            location = stoi(line_array[1]);
        if (line_array[1].at(0) == 'w') {
            long long int result =
                    *pax * (memory[location] + location + 1 < (pow(2, 15) ? memory[location + 1] << 7 : 0));
            *pdx = result >> 15;
            string binary_type = convert_from_decimal_to_binary(result);
            int tail = 0;
            if (binary_type.size() > 16) {
                binary_type = binary_type.substr(16, binary_type.size() - 1);
                tail = convert_from_binary_to_decimal(binary_type);
            } else tail = result;
            *pax = tail;
            if (*pdx != 0)
                CF = OF = 1;
        } else {
            int result = *pal * memory[location];
            *pax = result;
            if (*pah != 0)
                CF = OF = 1;
        }
    } else if (line_array[1] == "w" || line_array[1] == "b")// sayı veya variable
    {
        int asci = line_array[2].at(0);
        if (asci >= 48 && asci <= 57)//sayı
        {
            int sayi = 0;
            if (line_array[2].at(line_array[2].size() - 1) == 'h' && line_array[2].at(0) == '0')
                sayi = calc_memory_loc(2);
            else if (line_array[2].at(line_array[2].size() - 1) == 'd')
                sayi = stoi(line_array[2].substr(0, line_array[2].size() - 1));
            else
                sayi = stoi(line_array[2]);
            if (line_array[1] == "w") {
                int result = *pax * sayi;
                *pdx = result >> 15;
                string binary_type = convert_from_decimal_to_binary(result);
                int tail = 0;
                if (binary_type.size() > 16) {
                    binary_type = binary_type.substr(16, binary_type.size() - 1);
                    tail = convert_from_binary_to_decimal(binary_type);
                } else tail = result;
                *pax = tail;
                if (*pdx != 0)
                    CF = OF = 1;
            } else {
                int result = *pal * sayi;
                *pax = result;
                if (*pah != 0)
                    CF = OF = 1;
            }
        } else {
            string var = "";
            string var_type = "";
            unsigned short var_value = 0;
            for (auto const variable:variable_array) {
                if (variable.first == line_array[2]) {
                    var = variable.first;
                    var_type = variable.second;
                }
            }
            if (line_array[1] == "w") {
                int result = *pax * var_value;
                string binary_type = convert_from_decimal_to_binary(result);
                int tail = 0;
                if (binary_type.size() > 16) {
                    binary_type = binary_type.substr(16, binary_type.size() - 1);
                    tail = convert_from_binary_to_decimal(binary_type);
                } else tail = result;
                *pax = tail;
                if (*pdx != 0)
                    CF = OF = 1;
            } else {
                int result = *pal * var_value;
                *pax = result;
                if (*pah != 0)
                    CF = OF = 1;
            }
        }
    } else
        return -1;
    return 0;
}

int div(int i) {
    int multiplier = return_value_of_right_hand_side(line_array[1] != "b" || line_array[1] != "w" ? 1 : 2);
    //byte-word main issue
    vector<string>::iterator x, y, x1, y1, bit16_with_breaked, bit16_with_b_breaked, bit16_with_w_breaked, reg;

    x = find(bit16_array.begin(), bit16_array.end(), line_array[1]);
    y = find(bit8_array.begin(), bit8_array.end(), line_array[1]);
    bit16_with_w_breaked = find(bit16_array_with_w_and_breaked.begin(), bit16_array_with_w_and_breaked.end(),
                                line_array[1]);
    bit16_with_b_breaked = find(bit16_array_with_b_and_breaked.begin(), bit16_array_with_b_and_breaked.end(),
                                line_array[1]);

    if (x != bit16_array.end()) {
        unsigned short *ptr;
        ptr=return_pointer(line_array[1]);
        int value=(*pdx<<15) + *pax;
        *pdx=value % *ptr;
        *pax=value / *ptr;
    }else if(y!=bit8_array.end()){
        unsigned char *ptr;
        ptr = get8Bit(line_array[1]);
        *pah = *pax % *ptr;
        *pal = *pax / *ptr;
    }
    else if (bit16_with_b_breaked != bit16_array_with_b_and_breaked.end()) {
        unsigned char *ptr;
        ptr = (unsigned char *) return_pointer(line_array[1].substr(2, line_array[1].size() - 2));
        *pah = *pax % memory[*ptr];
        *pal = *pax / memory[*ptr];
    } else if (bit16_with_w_breaked != bit16_array_with_w_and_breaked.end()) {
        unsigned short *ptr;
        ptr = return_pointer(line_array[1].substr(2, line_array[1].size() - 2));
        string s;
        s += *pdx;
        s += *pax;
        int value = stoi(s);
        *pdx = value % (memory[*ptr] + *ptr + 1 < (2 << 15) ? memory[*ptr + 1] << 7 : 0);
        *pax = value / (memory[*ptr] + *ptr + 1 < (2 << 15) ? memory[*ptr + 1] << 7 : 0);
    } else if ((line_array[1].at(0) == 'w' || line_array[1].at(0) == 'b') && line_array[1].at(1) == '[')//memory
    {
        int location;
        if (line_array[1].at(line_array[1].size() - 2) == 'h' && line_array[1].at(0) == '0')
            location = calc_memory_loc(1);
        else if (line_array[1].at(line_array[1].size() - 2) == 'd')
            location = stoi(line_array[1].substr(2, line_array[1].size() - 2));
        else
            location = stoi(line_array[1]);
        if (line_array[1].at(0) == 'w') {
            string s;
            s += *pdx;
            s += *pax;
            int value = stoi(s);
            *pdx = value % (memory[location] + location + 1 < (2 << 15) ? memory[location + 1] << 7 : 0);
            *pax = value / (memory[location] + location + 1 < (2 << 15) ? memory[location + 1] << 7 : 0);
        } else {
            *pah = *pax % memory[location];
            *pal = *pax / memory[location];
        }
    } else if (line_array[1] == "w" || line_array[1] == "b")// sayı veya variable
    {
        int asci = line_array[2].at(0);
        if (asci >= 48 && asci <= 57)//sayı
        {
            int sayi = 0;
            if (line_array[2].at(line_array[2].size() - 1) == 'h' && line_array[2].at(0) == '0')
                sayi = calc_memory_loc(2);
            else if (line_array[2].at(line_array[2].size() - 1) == 'd')
                sayi = stoi(line_array[2].substr(0, line_array[2].size() - 1));
            else
                sayi = stoi(line_array[2]);
            if (line_array[1] == "w") {
                string s;
                s += *pdx;
                s += *pax;
                int value = stoi(s);
                *pdx = value % sayi;
                *pax = value / sayi;
            } else {
                *pah = *pax % sayi;
                *pal = *pax / sayi;
            }
        } else {
            string var = "";
            string var_type = "";
            unsigned short var_value = 0;
            int kk = 0;
            for (auto const variable:variable_array) {
                if (variable.first == line_array[2]) {
                    var = variable.first;
                    var_type = variable.second;
                }
                kk++;
            }
            if (line_array[1] == "w") {
                string s;
                s += *pdx;
                s += *pax;
                int value = stoi(s);
                int deger = memory[kk] + (kk + 1 < (pow(2, 15))) ? memory[kk + 1] << 7 : 0;
                *pdx = value % deger;
                *pax = value / deger;
            } else {
                string s;
                s += *pdx;
                s += *pax;
                int value = stoi(s);
                int deger = memory[kk];
                *pdx = value % deger;
                *pax = value / deger;
            }
        }
    }
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
                int deger = memory[*ptr] + (*ptr + 1) < (pow(2, 15)) ? memory[*ptr + 1] << 7 : 0;
                deger = ~deger;
                string ss = convert_from_decimal_to_binary(deger);
                int tail = 0;
                if (ss.size() > 8) {
                    ss = ss.substr(8, ss.size() - 1);
                    tail = convert_from_binary_to_decimal(ss);
                } else tail = deger;
                memory[*ptr] = tail;
                if (*ptr + 1 < (pow(2, 15)))
                    memory[*ptr + 1] = (deger >> 7);
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
            int deger = memory[location] + (location + 1 < (pow(2, 15))) ? memory[location + 1] << 7 : 0;
            deger = ~deger;
            string ss = convert_from_decimal_to_binary(deger);
            int tail = 0;
            if (ss.size() > 8) {
                ss = ss.substr(8, ss.size() - 1);
                tail = convert_from_binary_to_decimal(ss);
            } else tail = deger;
            memory[location] = tail;
            if (location + 1 < (pow(2, 15)))
                memory[location + 1] = (deger >> 7);
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
        }
        keep_index++;
    }
    vector<string>::iterator x, y, x1, y1, bit16_with_breaked, bit16_with_b_breaked, bit16_with_w_breaked, reg;

    x = find(bit16_array.begin(), bit16_array.end(), line_array[1]);
    x1 = find(bit8_array.begin(), bit8_array.end(), line_array[1]);

    bit16_with_breaked = find(bit16_array_with_breaked.begin(), bit16_array_with_breaked.end(), line_array[1]);
    bit16_with_w_breaked = find(bit16_array_with_w_and_breaked.begin(), bit16_array_with_w_and_breaked.end(),
                                line_array[1]);
    bit16_with_b_breaked = find(bit16_array_with_b_and_breaked.begin(), bit16_array_with_b_and_breaked.end(),
                                line_array[1]);

    int value = return_value();


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
            ptr = (unsigned char *) return_pointer(line_array[1].substr(3, line_array[1].size() - 1));
            for (int i = 0; i < value; i++) {
                int value = memory[*ptr] + *ptr + 1 < (pow(2, 15)) ? memory[*ptr + 1] : 0;
                unsigned short tt = CF << sizeof(value) * 8 - 1;
                CF = value & 1;
                value >>= 1;
                value |= tt;
            }
            memory[*ptr] = value;
        } else {
            unsigned short *ptr;
            ptr = return_pointer(
                    line_array[1].substr(bit16_with_w_breaked != bit16_array_with_w_and_breaked.end() ? 3 : 2,
                                         line_array[1].size() - 1));
            for (int i = 0; i < value; i++) {
                int value = memory[*ptr] + *ptr + 1 < (pow(2, 15)) ? memory[*ptr + 1] : 0;
                unsigned short tt = CF << sizeof(value) * 8 - 1;
                CF = value & 1;
                value >>= 1;
                value |= tt;
            }
            string ss = convert_from_decimal_to_binary(value);
            int tail = 0;
            if (ss.size() > 8) {
                ss = ss.substr(8, ss.size() - 1);
                tail = convert_from_binary_to_decimal(ss);
            } else tail = value;
            memory[keep_index] = tail;
            if (keep_index + 1 < (pow(2, 15)))
                memory[keep_index + 1] = value >> 7;

        }
    }
    else if (line_array[1].at(0) == '[' && line_array[1].at(line_array[1].size() - 1) == ']') {
        int location = calc_memory_loc(1);
        for (int i = 0; i < value; i++) {
            int value = memory[location] + location + 1 < (2 << 15) ? memory[location + 1] : 0;
            unsigned short tt = CF << sizeof(value) * 8 - 1;
            CF = value & 1;
            value >>= 1;
            value |= tt;
        }
        string ss = convert_from_decimal_to_binary(value);
        int tail = 0;
        if (ss.size() > 8) {
            ss = ss.substr(8, ss.size() - 1);
            tail = convert_from_binary_to_decimal(ss);
        } else tail = value;
        memory[keep_index] = tail;
        if (keep_index + 1 < (pow(2, 15)))
            memory[keep_index + 1] = value >> 7;
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
            int value = memory[keep_index] + (keep_index + 1 < (2 << 15)) ? memory[keep_index + 1] << 7 : 0;
            for (int i = 0; i < value; i++) {
                unsigned short tt = CF << sizeof(value) * 8 - 1;
                CF = value & 1;
                value >>= 1;
                value |= tt;
            }
            string ss = convert_from_decimal_to_binary(value);
            int tail = 0;
            if (ss.size() > 8) {
                ss = ss.substr(8, ss.size() - 1);
                tail = convert_from_binary_to_decimal(ss);
            } else tail = value;
            memory[keep_index] = tail;
            if (keep_index + 1 < (pow(2, 15)))
                memory[keep_index + 1] = value >> 7;
        }

    }
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
        }
        keep_index++;
    }
    vector<string>::iterator x, y, x1, y1, bit16_with_breaked, bit16_with_b_breaked, bit16_with_w_breaked, reg;

    x = find(bit16_array.begin(), bit16_array.end(), line_array[1]);
    x1 = find(bit8_array.begin(), bit8_array.end(), line_array[1]);

    bit16_with_breaked = find(bit16_array_with_breaked.begin(), bit16_array_with_breaked.end(), line_array[1]);
    bit16_with_w_breaked = find(bit16_array_with_w_and_breaked.begin(), bit16_array_with_w_and_breaked.end(),
                                line_array[1]);
    bit16_with_b_breaked = find(bit16_array_with_b_and_breaked.begin(), bit16_array_with_b_and_breaked.end(),
                                line_array[1]);

    int value = return_value();
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
            int adres = (unsigned char) *return_pointer(line_array[1].substr(2, line_array[1].size() - 1));
            int deger = memory[adres];
            for (; deger != 0; deger >>= 1)
                CF = deger & 1;
            memory[adres] = deger;
        } else {
            int adres = *return_pointer(
                    line_array[1].substr(bit16_with_w_breaked != bit16_array_with_w_and_breaked.end() ? 2 : 1,
                                         line_array[1].size() - 1));
            int deger = memory[adres] + (adres + 1 < (pow(2, 15))) ? memory[adres + 1] << 7 : 0;
            for (; deger != 0; deger >>= 1)
                CF = deger & 1;

            int tail = 0;
            string binary_type = convert_from_decimal_to_binary(deger);
            if (binary_type.size() > 8) {
                binary_type = binary_type.substr(8, binary_type.size() - 1);
                tail = convert_from_binary_to_decimal(binary_type);
            }
            tail = deger;
            memory[adres] = tail;
            if (adres + 1 < (pow(2, 15)))
                memory[adres + 1] = deger >> 7;
        }
    }
    else if (line_array[1].at(0) == '[' && line_array[1].at(line_array[1].size() - 1) == ']') {
        int location = calc_memory_loc(1);
        int a = memory[location] + location + 1 < (2 << 15) ? memory[location + 1] << 7 : 0;
        for (; a != 0; a >>= 1)
            CF = a & 1;
        string ss = convert_from_decimal_to_binary(a);
        int tail = 0;
        if (ss.size() > 8) {
            ss = ss.substr(8, ss.size() - 1);
            tail = convert_from_binary_to_decimal(ss);
        } else tail = a;
        memory[location] = tail;
        if (location + 1 < (pow(2, 15)))
            memory[location + 1] = (a >> 7);
    } else if ((line_array[1] == "w" || line_array[1] == "b") && line_array[2] == variable && line_array[3] != "w" &&
               line_array[3] != "b")//shl w var,1
    {
        if (line_array[1] == "b") {
            int deger = memory[keep_index];
            for (; deger != 0; deger >>= 1)
                CF = deger & 1;

            memory[keep_index] = deger;
        } else {
            int deger = memory[keep_index] + (keep_index + 1 < (pow(2, 15))) ? memory[keep_index] << 7 : 0;
            for (; deger != 0; deger >>= 1)
                CF = deger & 1;

            string ss = convert_from_decimal_to_binary(deger);
            int tail = 0;
            if (ss.size() > 8) {
                ss = ss.substr(8, ss.size() - 1);
                tail = convert_from_binary_to_decimal(ss);
            } else tail = deger;
            memory[keep_index] = tail;
            if (keep_index + 1 < (pow(2, 15)))
                memory[keep_index + 1] = (deger >> 7);

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
    vector<string>::iterator x, y, x1, y1, bit16_with_breaked, bit16_with_b_breaked, bit16_with_w_breaked, reg;

    x = find(bit16_array.begin(), bit16_array.end(), line_array[1]);
    x1 = find(bit8_array.begin(), bit8_array.end(), line_array[1]);

    bit16_with_breaked = find(bit16_array_with_breaked.begin(), bit16_array_with_breaked.end(), line_array[1]);
    bit16_with_w_breaked = find(bit16_array_with_w_and_breaked.begin(), bit16_array_with_w_and_breaked.end(),
                                line_array[1]);
    bit16_with_b_breaked = find(bit16_array_with_b_and_breaked.begin(), bit16_array_with_b_and_breaked.end(),
                                line_array[1]);

    int value = return_value();

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
            ptr = return_pointer(line_array[1].substr(2, line_array[1].size() - 1));
            int deger = memory[*ptr] + *ptr + 1 < (pow(2, 15)) ? memory[*ptr + 1] << 7 : 0;
            CF = deger & 1;
            deger <<= value;
            string ss = convert_from_decimal_to_binary(deger);
            int tail = 0;
            if (ss.size() > 8) {
                ss = ss.substr(8, ss.size() - 1);
                tail = convert_from_binary_to_decimal(ss);
            } else tail = deger;
            memory[*ptr] = tail;
            if (*ptr + 1 < (pow(2, 15)))
                memory[*ptr + 1] = (deger >> 7);
        } else {
            unsigned char *ptr;
            ptr = (unsigned char *) return_pointer(line_array[1].substr(2, line_array[1].size() - 1));
            int deger = memory[*ptr];
            CF = deger & 1;
            deger <<= value;
            memory[*ptr] = deger;
        }
    }
   else if (line_array[1].at(0) == '[' && line_array[1].at(line_array[1].size() - 1) == ']') {
        int location = calc_memory_loc(1);
        unsigned short value = return_value_of_right_hand_side(2);
        if (value == -1)
            return -1;
        int deger = memory[location] + location + 1 < (pow(2, 15)) ? memory[location + 1] << 7 : 0;
        CF = deger & 1;
        deger <<= value;

        string ss = convert_from_decimal_to_binary(deger);
        int tail = 0;
        if (ss.size() > 8) {
            ss = ss.substr(8, ss.size() - 1);
            tail = convert_from_binary_to_decimal(ss);
        } else tail = deger;
        memory[location] = tail;
        if (location + 1 < (pow(2, 15)))
            memory[location + 1] = (deger >> 7);
    } else if ((line_array[1] == "w" || line_array[1] == "b") && line_array[2] == variable && line_array[3] != "w" &&
               line_array[3] != "b")//shl w var,1
    {
        int value = return_value_of_right_hand_side(3);
        if (value == -1)
            return -1;
        int deger = memory[keep_index] + keep_index + 1 < (pow(2, 15)) ? memory[keep_index + 1] << 7 : 0;
        CF = deger & 1;
        deger <<= value;

        string ss = convert_from_decimal_to_binary(deger);
        int tail = 0;
        if (ss.size() > 8) {
            ss = ss.substr(8, ss.size() - 1);
            tail = convert_from_binary_to_decimal(ss);
        } else tail = deger;
        memory[keep_index] = tail;
        if (keep_index + 1 < (pow(2, 15)))
            memory[keep_index + 1] = (deger >> 7);
    } else return -1;
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
        }
        keep_index++;
    }
    vector<string>::iterator x, y, x1, y1, bit16_with_breaked, bit16_with_b_breaked, bit16_with_w_breaked, reg;

    x = find(bit16_array.begin(), bit16_array.end(), line_array[1]);
    x1 = find(bit8_array.begin(), bit8_array.end(), line_array[1]);

    bit16_with_breaked = find(bit16_array_with_breaked.begin(), bit16_array_with_breaked.end(), line_array[1]);
    bit16_with_w_breaked = find(bit16_array_with_w_and_breaked.begin(), bit16_array_with_w_and_breaked.end(),
                                line_array[1]);
    bit16_with_b_breaked = find(bit16_array_with_b_and_breaked.begin(), bit16_array_with_b_and_breaked.end(),
                                line_array[1]);

    int value = return_value();
    if (x != bit16_array.end() || x1 != bit8_array.end()) {
        if (x != bit16_array.end()) {
            unsigned short *ptr;
            ptr = return_pointer(line_array[1]);
            for (int i = 0; i < value; i++) {
                bool k = CF;
                CF = convert_from_decimal_to_binary(*ptr).at(0) - '0';
                *ptr <<= 1;
                *ptr |= k;
            }
        } else {
            unsigned char *ptr;
            ptr = get8Bit(line_array[1]);
            for (int i = 0; i < value; i++) {
                bool k = CF;
                CF = convert_from_decimal_to_binary(*ptr).at(0) - '0';
                *ptr <<= 1;
                *ptr |= k;
            }
        }
    } else if (bit16_with_breaked != bit16_array_with_breaked.end() ||
               bit16_with_b_breaked != bit16_array_with_b_and_breaked.end() ||
               bit16_with_w_breaked != bit16_array_with_w_and_breaked.end()) {
        if (bit16_with_b_breaked == bit16_array_with_b_and_breaked.end()) {
            unsigned short *ptr;
            ptr = return_pointer(line_array[1].substr(bit16_with_breaked != bit16_array_with_breaked.end() ? 2 : 3,
                                                      line_array[1].size() - 1));
            int deger = memory[*ptr] + *ptr + 1 < (pow(2, 15)) ? memory[*ptr + 1] << 7 : 0;
            for (int i = 0; i < value; i++) {
                bool k = CF;
                CF = convert_from_decimal_to_binary(deger).at(0) - '0';
                deger <<= 1;
                deger |= k;
            }
            string ss = convert_from_decimal_to_binary(deger);
            int tail = 0;
            if (ss.size() > 8) {
                ss = ss.substr(8, ss.size() - 1);
                tail = convert_from_binary_to_decimal(ss);
            } else tail = deger;
            memory[*ptr] = tail;
            if (*ptr + 1 < (pow(2, 15)))
                memory[*ptr + 1] = (deger >> 7);

        } else {
            unsigned char *ptr;
            ptr = get8Bit(line_array[1].substr(2, line_array[1].size() - 1));
            int deger = memory[*ptr];
            for (int i = 0; i < value; i++) {
                bool k = CF;
                CF = convert_from_decimal_to_binary(deger).at(0) - '0';
                deger <<= 1;
                deger |= k;
            }
            memory[*ptr] = deger;
        }
    }
    else if (line_array[1].at(0) == '[' && line_array[1].at(line_array[1].size() - 1) == ']') {
        int location = calc_memory_loc(1);
        if (location == -1)
            return -1;
        for (int i = 0; i < value; i++) {
            bool k = CF;
            int deger = memory[location] + location + 1 < (2 << 15) ? memory[location + 1] : 0;
            CF = convert_from_decimal_to_binary(
                    memory[location] + location + 1 < (2 << 15) ? memory[location + 1] << 7 : 0).at(0) - '0';
            deger <<= 1;
            deger |= k;
            string ss = convert_from_decimal_to_binary(deger);
            int tail = 0;
            if (ss.size() > 8) {
                ss = ss.substr(8, ss.size() - 1);
                tail = convert_from_binary_to_decimal(ss);
            } else tail = deger;
            memory[keep_index] = tail;
            if (keep_index + 1 < (pow(2, 15)))
                memory[keep_index + 1] = (deger >> 7);
        }
    } else if ((line_array[1] == "w" || line_array[1] == "b") && line_array[2] == variable && line_array[3] != "w" &&
               line_array[3] != "b")//shl w var,1
    {
        for (int i = 0; i < value; i++) {
            bool k = CF;
            int deger = memory[keep_index] + keep_index + 1 < (2 << 15) ? memory[keep_index + 1] : 0;
            CF = convert_from_decimal_to_binary(
                    memory[keep_index] + keep_index + 1 < (2 << 15) ? memory[keep_index + 1] << 7 : 0).at(0) - '0';
            deger <<= 1;
            deger |= k;
            string ss = convert_from_decimal_to_binary(deger);
            int tail = 0;
            if (ss.size() > 8) {
                ss = ss.substr(8, ss.size() - 1);
                tail = convert_from_binary_to_decimal(ss);
            } else tail = deger;
            if (keep_index + 1 < (pow(2, 15)))
                memory[keep_index + 1] = deger >> 7;

        }
    } else
        return -1;
    return 0;
}

int push(int i) {
    if (return_value_of_right_hand_side(1) < (2 << 15)) {
        int head_8bit = return_value_of_right_hand_side(1) >> 7;
        int value = return_value_of_right_hand_side(1);
        string tail_8bit = convert_from_decimal_to_binary(value);
        int tail_8 = 0;
        if (tail_8bit.size() > 8) {
            tail_8bit = tail_8bit.substr(8, tail_8bit.size() - 1);
            tail_8 = convert_from_binary_to_decimal(tail_8bit);
        } else
            tail_8 = value;
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
            int head_8bit = memory[sp + 2];
            head_8bit <<= 7;
            int tail_8bit = memory[sp + 1];
            *ptr = head_8bit + tail_8bit;
            sp += 2;
        } else return -1;
    }
    return 0;
}

char int_21h(int i) {
    if (*pah == 1) {
        char ch;
        cin >> ch;
        *pah = (unsigned short) ch;
        *pdl = ch;
        cout << ch;
    } else if (*pah == 2) {
        *pal = *pdl;
        char ch = *pal;
        cout<<ch;
        return ch;
    } else if (*pah == 8) {
        char ch;
        cin >> ch;
        *pah = (unsigned short) ch;
        *pdl = ch;
    } else if (*pah == 9) {


    } else if (*pah == 10) {

    } else if (*pah == 76) {
        return ' ';
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
    if (bit16_with_b_breaked != bit16_array_with_b_and_breaked.end() ||
        bit16_with_w_breaked != bit16_array_with_w_and_breaked.end() ||
        bit16_with_breaked != bit16_array_with_breaked.end()) {
        int oo = return_value_of_right_hand_side(2);
        if (oo == -1)
            return -1;
        if ((bit16_with_breaked != bit16_array_with_breaked.end() ||
             (bit16_with_w_breaked != bit16_array_with_w_and_breaked.end()) && line_array[2] != "b")) {
            string ss = line_array[1].substr(bit16_with_breaked != bit16_array_with_breaked.end() ? 1 : 2,
                                             line_array[1].size() - 1);
            int value = memory[*return_pointer(ss)] + (*return_pointer(ss) + 1 < (pow(2, 15))) ?
                        memory[*return_pointer(ss) + 1] << 7 : 0;
            change_flags(value, oo);
        } else if ((bit16_with_b_breaked != bit16_array_with_b_and_breaked.end() && line_array[2] != "w")) {
            string ss = line_array[1].substr(2, line_array[1].size() - 1);
            change_flags(memory[(unsigned char) *return_pointer(ss)], oo);
        }
    } else if (reg != reg_array.end()) {
        int deger = return_value_of_right_hand_side(2);
        if (deger == -1)
            return -1;
        if (line_array[1].at(1) != 'l' || line_array[1].at(1) != 'h') {
            unsigned short *ptr;
            ptr = return_pointer(line_array[1]);
            change_flags(*ptr, deger);
        } else {
            unsigned char *ptr;
            ptr = get8Bit(line_array[1]);
            change_flags(*ptr, deger);
        }
    } else if (line_array[1].at(0) == '[' && line_array[1].at(line_array[1].size() - 1) == ']') {
        string location = "";
        if (line_array[1].at(line_array[1].size() - 1) == 'h')
            location = calc_memory_loc(1);
        else if (line_array[1].at(line_array[1].size() - 1) == 'd')
            location = line_array[1].substr(1, line_array[1].size() - 2);
        int sayi = stoi(location);
        int value = return_value_of_right_hand_side(2);
        if (sayi == -1 || value == -1)
            return -1;
        change_flags((memory[sayi] + memory[sayi + 1] << 7), value);
    } else if (line_array[1] == "b" || line_array[1] == "w" || n == line_array[1] || n == line_array[2])//cmp w 'a',ax
    {
        int value_right_hand_side = return_value_of_right_hand_side(
                (line_array[1] == "b" || line_array[1] == "w") ? 3 : 2);
        if (value_right_hand_side == -1)
            return -1;
        change_flags(variable_value, value_right_hand_side);
        if (n == line_array[1] || n == line_array[2]) {
            int right = return_value_of_right_hand_side(n == line_array[1] ? 2 : 3);
            int value =
                    memory[variable_index] + (variable_index + 1 < (pow(2, 15))) ? memory[variable_index + 1] << 7 : 0;
            change_flags(value, right);
        } else {
            int asci_code = line_array[2].at(1);
            int right_value = return_value_of_right_hand_side(3);
            change_flags(asci_code, right_value);
        }
    } else if (asci >= 48 && asci <= 57)//sayi
    {
        int value_rigth_hand_side = return_value_of_right_hand_side(2);
        if (value_rigth_hand_side == -1)
            return -1;
        change_flags(asci, value_rigth_hand_side);
    } else
        return -1;
    return 0;
}

int JZ_JE(int i) {
    if (line_array[0] == "JZ" || line_array[0] == "JE") {
        vector<string>::iterator o = find(code_array.begin(), code_array.end(), line_array[1]);
        if (o != code_array.end() && ZF) {
            p = --o;
        } else return -1;
    }
    return 0;
}

int JNE_JNZ(int i) {
    if (line_array[1] == "jne" || line_array[1] == "jnz") {
        vector<string>::iterator o = find(code_array.begin(), code_array.end(), line_array[1]);
        if (o != code_array.end() && !ZF) {
            p = --o;
        } else return -1;
    }
    return 0;
}

int JA_JNBE(int i) {
    if (line_array[1] == "ja" || line_array[1] == "jnbe") {
        vector<string>::iterator o = find(code_array.begin(), code_array.end(), line_array[1]);
        if (o != code_array.end() && !CF && !ZF) {
            p = --o;
        } else return -1;
    }
    return 0;
}

int JB_JC_JNAE(int i) {
    if (line_array[0] == "jb" || line_array[0] == "jc" || line_array[0] == "jnae") {
        string ss = line_array[1] + ':';
        vector<string>::iterator o = find(code_array.begin(), code_array.end(), ss);
        if (o != code_array.end() && CF) {
            p = --o;
        } else return -1;
    }
    return 0;
}

int JBE(int i) {
    if (line_array[1] == "jbe") {
        vector<string>::iterator o = find(code_array.begin(), code_array.end(), line_array[1]);
        if (o != code_array.end() && (CF || ZF)) {
            p = --o;
        } else return -1;
    }
    return 0;
}

int JNB_JAE(int i) {
    if (line_array[1] == "jnb" || line_array[1] == "jae") {
        vector<string>::iterator o = find(code_array.begin(), code_array.end(), line_array[1]);
        if (o != code_array.end() && !CF) {
            p = --o;
        } else return -1;
    }
    return 0;
}

int JNC(int i) {
    if (line_array[1] == "JNC") {
        vector<string>::iterator o = find(code_array.begin(), code_array.end(), line_array[1]);
        if (o != code_array.end() && !CF) {
            p = --o;
        } else return -1;
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
    if (s.at(0) == 'a')
        if (s.at(1) == 'l')
            return (pal);
        else
            return (pah);
    else if (s.at(0) == 'b')
        if (s.at(1) == 'l')
            return (pbl);
        else
            return (pbh);
    else if (s.at(0) == 'c')
        if (s.at(1) == 'l')
            return (pcl);
        else
            return (pch);
    else if (s.at(0) = 'd')
        if (s.at(1) == 'l')
            return (pdl);
        else
            return (pdh);
    else
        return nullptr;
}

int calc_memory_loc(int arr_num) { // CALCALUTES MEMORY LOCATİON
    string location_of_memory_s = "";
    for (int i = 1; i < line_array[arr_num].size(); i++) { // memory location al
        if (line_array[arr_num].at(i) != ']' && line_array[arr_num].at(i) != '[' && line_array[arr_num].at(i) != 'w' &&
            line_array[arr_num].at(i) != 'b') // size-1
            location_of_memory_s += line_array[arr_num].at(i);
    }
    if (location_of_memory_s[location_of_memory_s.size() - 1] == 'h')
        hec_to_dec(location_of_memory_s);// converting from Hexadec to Dec

    int location_of_memory = stoi(location_of_memory_s);
    if (location_of_memory >= (2 << 16)) { // mem 64k
        cout << "error" << endl;
        return 0; // TERMİNATE PROGRAM // BURASI DÜZELTİLECEK
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
        } else if (hex_val[i] >= 'a' && hex_val[i] <= 'f') {
            dec_val += (hex_val[i] + 10 - 97) * base;
            base = base * 16;
        }
    }
    return dec_val;
}

bool control_of_sum(unsigned short sayi1, unsigned short sayi2, int bit) {
    if (bit == 8) {
        if ((sayi1 + sayi2) > 256)
            return false;
    } else if (bit == 16){
        if ((sayi1 + sayi2) > 65535)
            return false;
    }else if(bit==32)
        if ((sayi1 + sayi2) > (pow(2,32)))
            return false;
    return true;
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
    int asci_value=0;
    if(line_array[index].size()>1)
        asci_value = line_array[index].at(1);
    if (line_array[index] == "offset")//reg-adress || mov [01h], offset (w || b) v1 //returns value of v1
    {
        x = find(bit16_array.begin(), bit16_array.end(),
                 line_array[line_array[index + 1] == "w" ? index + 2 : index + 1]);
        x1 = find(bit8_array.begin(), bit8_array.end(),
                  line_array[line_array[index + 1] == "b" ? index + 2 : index + 1]);

        if (x != bit16_array.end() && line_array[index + 1] != "b") {
            unsigned short *ptr;
            ptr = return_pointer(line_array[index].substr(1, 3));
            return memory[*ptr];
        }
        if (x1 != bit8_array.end() && line_array[index + 1] != "w") {
            unsigned short *ptr;
            ptr = return_pointer(line_array[index].substr(1, 3));
            return memory[*ptr] + (*ptr + 1 < (pow(2, 16))) ? memory[*ptr + 1] << 7 : 0;
        }
        /*if(bit16_with_b_breaked!=bit16_array_with_b_and_breaked.end())
            return reg_adresi(3);
        if(bit16_with_b_breaked!=bit16_array_with_b_and_breaked.end())
            return reg_adresi(3);*/
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
            if (l.first == line_array[line_array[index + 1] != "w" || "b" ? index + 1 : index + 2]) {
                n = l.first;
                break;
            }
            variable_index++;
        }
        if (n == line_array[line_array[index + 1] != "w" || "b" ? index + 1 : index + 2])
            return variable_index;

    }
    if (x != bit16_array.end() || x1 != bit8_array.end()) //reg-reg --- mov ax,cx || mov al,cl
    {
        if (x != bit16_array.end())
            return (unsigned short) *return_pointer(line_array[index]);
        else
            return *get8Bit(line_array[index]);
    }
    if (bit16_with_breaked !=
        bit16_array_with_breaked.end()) //reg-b[reg] ---mov ah, b[bx] ----- //reg-[reg] --mov cx,[bx] -- mov di,[si] -- mov ax,[di] ---//reg-w[reg]-- mov ax, w[bx]
    {
        unsigned short *ptr;
        ptr = return_pointer(line_array[index].substr(1, line_array[index].size() - 1));
        return memory[*ptr] + ((*ptr + 1) < pow(2, 16) ? memory[*ptr + 1] >> 7 : 0);
    }
    else if (bit16_with_b_breaked != bit16_array_with_b_and_breaked.end() ||
        (bit16_with_w_breaked != bit16_array_with_w_and_breaked.end())) {
        unsigned short *ptr;
        ptr = return_pointer(line_array[index].substr(2, line_array[3].size() - 1));
        if (bit16_with_w_breaked != bit16_array_with_w_and_breaked.end())
            return memory[*ptr] + ((*ptr + 1) < (pow(2, 15)) ? memory[*ptr + 1] << 7 : 0);
        else
            return memory[*ptr];
    }
    else if (line_array[index].at(0) == '[' || line_array[index].at(line_array[index].size() - 1) == ']' ||
        ((line_array[index].at(0) == 'w' || line_array[index].at(0) == 'b') &&
         line_array[index].size() > 1))//reg-memory|w[memory]|b[memory]
    {
        if (line_array[index].at(0) == '[')
            return memory[calc_memory_loc(index)] + (calc_memory_loc(index) + 1 < (pow(2, 15))) ?
                   memory[calc_memory_loc(index)] << 7 : 0;
        if (line_array[index].at(0) == 'b') {
            if (line_array[index].at(line_array[index].size() - 1) == 'h')
                return memory[hec_to_dec(line_array[index].substr(1, line_array[index].size() - 1))];
            else
                return memory[stoi(line_array[index].substr(1, line_array[index].size() - 1))];
        } else if (line_array[index].at(0) == 'w')
            if (line_array[index].at(line_array[index].size() - 1) == 'h')
                return memory[hec_to_dec(line_array[index].substr(1, line_array[index].size() - 1))] +
                       (hec_to_dec(line_array[index].substr(1, line_array[index].size() - 1)) + 1) < (pow(2, 15)) ?
                       memory[hec_to_dec(line_array[index].substr(1, line_array[index].size() - 1)) + 1] << 7 : 0;
            else if (line_array[index].at(line_array[index].size() - 1) == 'd')
                return memory[stoi(line_array[index].substr(1, line_array[index].size() - 1))] +
                       (stoi(line_array[index].substr(1, line_array[index].size() - 1)) + 1) < (pow(2, 15)) ?
                       memory[stoi(line_array[index].substr(1, line_array[index].size() - 1))] << 7 : 0;
    }
   else if (line_array[index].at(0) >= 48 && line_array[index].at(0) <= 57)//bu bir sayi --mov [01h],02h //not b
    {
        if (line_array[index].at(line_array[index].size() - 1) == 'h')
            return hec_to_dec(line_array[index]);
        else if (line_array[index].at(line_array[index].size() - 1) == 'd') {
            string ss = line_array[index].substr(0, line_array[index].size() - 1);
            return stoi(ss);
        } else return stoi(line_array[index]);
    }
    else if (line_array[index].at(0 < 48 || line_array[index].at(0) > 57)) {
        //bu bir harf --  mov ax,'a'
        return asci_value;
    }
   else  if (line_array[index] == "w" || line_array[index] == "b")//w var,b var,w sayı
    {
        string n = "";
        int variable_index = 0;
        unsigned short variable_value;
        string type;
        for (auto const l:variable_array) {
            if (l.first == line_array[line_array[index + 1] != "w" || "b" ? index + 1 : index]) {
                n = l.first;
                type = l.second;
                break;
            }
            variable_index++;
        }
        if (n != line_array[3] && line_array[3].size() > 2) {
            int asci_2 = line_array[index + 1].at(0);
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
        } else {
            if (line_array[index].at(0) == type.at(1))
                return memory[variable_index] + variable_index + 1 < (pow(2, 15)) ? memory[variable_index + 1] << 7 : 0;
            else
                return 0;
        }
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
    }
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
                shr_is = false;
                return -1;
            }

            if (value > 31) {
                shr_is = false;
                return -1;
            }
        }
    } else {
        if (line_array[2] == "cl")
            value = *return_pointer(line_array[2]);
        else {
            if (line_array[2].at(0) == '0' || line_array[2].at(line_array[2].size() - 1) == 'h')
                value = hec_to_dec(line_array[2]);
            else if (line_array[2].at(line_array[2].size() - 1) == 'd')
                value = stoi(line_array[2].substr(0, line_array[2].size() - 1));
            else { shr_is = false; }
            if (value > 31) {
                shr_is = false;
                return -1;
            }
        }
    }
    return value;
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
        s += binaryNum[j];
    if (s.at(s.size() - 1) != binaryNum[0] || s.size()!=(i-1))
        s += binaryNum[0];
    return s;
}

int convert_from_binary_to_decimal(string n) {
    string num = n;
    int dec_value = 0;

    int base = 0;

    int len = num.size();
    for (int i = len - 1; i >= 0; i--) {
        dec_value += (pow(2, base) * num[i]);
        base++;
    }

    return dec_value;
}

int xor_or_and(string s) {
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
    vector<string>::iterator x, y, x1, y1, bit16_with_breaked, bit16_with_b_breaked, bit16_with_w_breaked, reg;

    x = find(bit16_array.begin(), bit16_array.end(), line_array[1]);
    x1 = find(bit8_array.begin(), bit8_array.end(), line_array[1]);

    bit16_with_breaked = find(bit16_array_with_breaked.begin(), bit16_array_with_breaked.end(), line_array[1]);
    bit16_with_w_breaked = find(bit16_array_with_w_and_breaked.begin(), bit16_array_with_w_and_breaked.end(),
                                line_array[1]);
    bit16_with_b_breaked = find(bit16_array_with_b_and_breaked.begin(), bit16_array_with_b_and_breaked.end(),
                                line_array[1]);

    if (x != bit16_array.end() || x1 != bit8_array.end()) {
        if(x != bit16_array.end()){
            unsigned short *ptr;
            ptr = return_pointer(line_array[1]);
            int value = return_value_of_right_hand_side(2);
            if (s == "or")
                *ptr |= value;
            else if (s == "xor")
                *ptr ^= value;
            else if (s == "and")
                *ptr &= value;
            if (*ptr == 0) {
                ZF = 1;
                SF = 0;
            }
            else if (*ptr < 0) {
                SF = 1;
                ZF = 0;
            }
            else ZF = SF = 0;
        }else{
            unsigned char *ptr;
            ptr = get8Bit(line_array[1]);
            int value = return_value_of_right_hand_side(2);
            if (s == "or")
                *ptr |= value;
            else if (s == "xor")
                *ptr ^= value;
            else if (s == "and")
                *ptr &= value;
            if (*ptr == 0) {
                ZF = 1;
                SF = 0;
            }
            else if (*ptr < 0) {
                SF = 1;
                ZF = 0;
            }
            else ZF = SF = 0;
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
        int toplam = memory[*ptr] + *ptr + 1 < (pow(2, 15)) ? memory[*ptr + 1] << 7 : 0;
        if (s == "or")
            toplam |= value;
        else if (s == "xor")
            toplam ^= value;
        else if (s == "and")
            toplam &= value;
        if (toplam == 0) {
            ZF = 1;
            SF = 0;
        }
        if (toplam < 0) {
            SF = 1;
            ZF = 0;
        }
        else SF = ZF = 0;
        int headof_8 = toplam >> 7;
        int tail_8 = 0;
        string s_s = convert_from_decimal_to_binary(toplam);
        if (s_s.size() > 8) {
            s_s = s_s.substr(8, s_s.size() - 1);
            tail_8 = convert_from_binary_to_decimal(s_s);
        } else tail_8 = toplam;
        memory[*ptr] = tail_8;
        if (*ptr + 1 < (pow(2, 15)))
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
        int k = memory[sayi] + sayi + 1 < (2 << 15) ? memory[sayi + 1] << 7 : 0;
        if (s == "or")
            k |= value;
        else if (s == "xor")
            k ^= value;
        else if (s == "and")
            k &= value;
        if (k == 0) {
            ZF = 1;
            SF = 0;
        }
        if (k < 0) {
            SF = 1;
            ZF = 0;
        }
        else ZF = SF = 0;
        int headof_8 = k >> 7;
        int tail_8 = 0;
        string s_s = convert_from_decimal_to_binary(k);
        if (s_s.size() > 8) {
            s_s = s_s.substr(8, s_s.size() - 1);
            tail_8 = convert_from_binary_to_decimal(s_s);
        } else tail_8 = k;
        memory[sayi] = tail_8;
        if (sayi + 1 < (pow(2, 15)))
            memory[sayi + 1] = headof_8;
    } else if (variable == line_array[(line_array[1] == "w" || line_array[1] == "b") ? 2 : 1])//or b var , bl
    {
        vector<string>::iterator reg_bit16 = find(bit16_array.begin(), bit16_array.end(),
                                                  line_array[(line_array[1] == "w") ? 3 : 2]);
        vector<string>::iterator reg_bit8 = find(bit16_array.begin(), bit8_array.end(),
                                                 line_array[(line_array[1] == "b") ? 3 : 2]);
        int value_of_memory = memory[keep_index] + keep_index + 1 < (pow(2, 15)) ? memory[keep_index + 1] << 7 : 0;
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
            int headof_8 = value_of_memory >> 7;
            int tail_8 = 0;
            string s_s = convert_from_decimal_to_binary(value_of_memory);
            if (s_s.size() > 8) {
                s_s = s_s.substr(8, s_s.size() - 1);
                tail_8 = convert_from_binary_to_decimal(s_s);
            } else tail_8 = value_of_memory;
            memory[keep_index] = tail_8;
            if (keep_index + 1 < (pow(2, 15)))
                memory[keep_index + 1] = headof_8;
            if (value_of_memory == 0) {
                ZF = 1;
                SF = 0;
            }
            if (value_of_memory < 0) {
                SF = 1;
                ZF = 0;
            }
            else ZF = SF = 0;
        } else if (reg_bit8 != bit8_array.end() && line_array[line_array[1] == "b" ? 3 : 2].at(1) == 'l') {
            unsigned short *ptr;
            ptr = return_pointer(*reg_bit8.base());
            int value_of_memory = memory[keep_index];
            if (s == "or")
                value_of_memory |= *ptr;
            else if (s == "xor")
                value_of_memory ^= *ptr;
            else if (s == "and")
                value_of_memory &= *ptr;
            memory[keep_index] = value_of_memory;
            if (value_of_memory == 0) {
                ZF = 1;
                SF = 0;
            }
            if (value_of_memory < 0) {
                SF = 1;
                ZF = 0;
            }
            else ZF = SF = 0;
        }
    }
    return 0;
}


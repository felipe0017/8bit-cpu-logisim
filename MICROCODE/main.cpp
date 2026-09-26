#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include <fstream>
#include <conio.h>
#include <utility> // std::pair

std::string lowercase(std::string s){
	std::string r;
    for(std::size_t i=0; i<s.size(); i++){
        r += std::tolower(s[i]);
    }
    return r;
}

template <typename t>
std::string hex(t number){
	std::stringstream ss;
	ss << std::hex << number;
	return ss.str();
}

template <typename t>
bool is_std_vector_duplicate_elements(std::vector<t> v){
	bool duplicated = false;
	for(std::size_t i=0; i<v.size(); i++){
		for(std::size_t j=0; j<v.size(); j++){
			if(j != i and v[i] == v[j]){
				duplicated = true;
				return duplicated;
			}
		}
	}
	return duplicated;
}

std::string flip(std::string str){
	std::string flipped;
	for(std::size_t i=0; i<str.size(); i++){
		std::size_t ii = str.size() - i - 1;
		flipped += str[ii];
	}
	return flipped;
}

std::string decimal_format(std::string str){
	std::string formatted;
	
	// flip string
	std::string flipped;
	flipped = flip(str);
	
	// put space each 3 digits
	// less the last digit (if decimal length is multiple of 3)
	std::string formatted_flipped;
	int count = 0;
	for(std::size_t i=0; i<flipped.size(); i++){
		formatted_flipped += flipped[i];
		count++;
		if(count == 3 and i<flipped.size()-1){
			formatted_flipped += ' ';
			count = 0;
		}
	}
	
	// flip formatted-flipped string
	formatted = flip(formatted_flipped);
	
	return formatted;
}

template <typename t>
std::string to_string(t x){
	std::stringstream ss;
    ss << x;
    return ss.str();
}

template <typename t>
t min(t x , t y){
	return (x<y)? x : y;
}

template <typename t>
t max(t x , t y){
	return (x>y)? x : y;
}

// control word parser
// convert list of high control bits to hex number to put in rom in logiisim
#include "cw_parser.h"

const std::vector< std::string > jmp_conditions = {
	"0",
	"1",
	"Z",
	"!Z",
	"N",
	"!N",
	"C",
	"!C"
};

template <typename t>
bool contain(std::vector<t> v, t data){
	bool exist = false;
	
	for(t element : v){
		if(element == data){
			exist = true;
			break;
		}
	}
	
	return exist;
}

template <typename t>
std::size_t seach(std::vector<t> v, t data){
	std::size_t index = 0;
	
	std::size_t i=0;
	for(t element : v){
		if(element == data){
			index = i;
			break;
		}
		else i++;
	}
	return index;
}

int main(){
	std::ifstream input_file;
	
	std::ofstream output_file_mc_high;
	std::ofstream output_file_mc_low; // low 32 bits
	
	std::ofstream output_file_jmp;
	std::ofstream output_file_length;
	std::ofstream output_file_mp_jmp;
	
	std::vector<std::string> ROMjmp_content;
	std::vector<std::string> ROMmc_content;
	std::vector<std::string> ROMmpJMP_content;
	
	struct instruction_t{
		std::string name;
		int         length;
	};
	
	std::vector<instruction_t> instruction;
	
	input_file.open("input-mc.txt", std::ios::in);
	if(input_file.fail()){
		std::cout << "error: file not found <input-mc.txt>" << std::endl;
		getch();
		return 0;
	}
	
	output_file_mc_high.open("output-mc-high"   , std::ios::out);
	output_file_mc_low .open("output-mc-low"    , std::ios::out);
	
	output_file_jmp    .open("output-jmp"       , std::ios::out);
	output_file_length .open("output-length.txt", std::ios::out);
	output_file_mp_jmp .open("output-mp-jmp"    , std::ios::out);
	
	// vars for error checks
	int accum_MPp_calls = 0;
	int objetive_MPp_calls = 0;
	
	const int cpu_hz = 1e6;
	
	while(!input_file.eof()){
		std::string file_line;
        std::getline(input_file, file_line);
        
        // if line is empty or is a commentary (start with #), ignore
        if(file_line.empty() or file_line[0] == '#') continue;
        
        // extract first word
        std::istringstream iss(file_line);
    	std::string first_word;
		iss >> first_word;
        
        if(first_word == std::string("NEW")){
        	// get the instruction name
        	std::string instruction_name;
        	iss >> instruction_name;
        	
        	// get the instruction assembler-format length in bytes
        	// RET instruction is special, not have MPp calls
        	if(instruction_name != std::string("RET")){
        		std::string instruction_bytes_length;
	        	iss >> instruction_bytes_length;
	        	objetive_MPp_calls += atoi(instruction_bytes_length.c_str());
        	}
        	
        	if(instruction_name != std::string("FETCH")){
        		// check if is a jmp-type instruction
	        	if(instruction_name[0] == 'J'){
	        		// get the jump condition
	        		std::string jump_condition;
	        		iss >> jump_condition;
	        		
	        		if(!contain(jmp_conditions, jump_condition)){
	        			std::cout << "error: invalid jump condition" << std::endl;
						getch();
						return 0;
	        		}
	        		
	        		ROMmpJMP_content.push_back( hex( seach(jmp_conditions, jump_condition) ) );
	        	}
	        	else{
	        		ROMmpJMP_content.push_back( hex(0) );
	        	}
        	}
        	
        	// save instruction name
        	instruction_t ins;
        	ins.name   = instruction_name;
        	ins.length = 0;
        	instruction.push_back( ins );
        	
        	// register this start pos in mc-rom in the jmp-rom
        	// if is`nt fetch cycle
        	if(instruction_name != std::string("FETCH")){
        		ROMjmp_content.push_back( hex( ROMmc_content.size() ) );
        	}
        	
        }
        else if(first_word == std::string("END")){
        	// check if is fetch cycle
        	// read the name for current instruction for this
        	if(instruction.back().name != std::string("FETCH")){
        		// if is'nt fetch cycle, put 0x0 at the end of
        		// instruction micro-code
        		ROMmc_content.push_back( "0" );
        	}
        }
        else{
        	// convert high control bits to hex string and save it in
        	// micro-code rom
        	bool has_MPp = false;
        	ROMmc_content.push_back( cw_parser(file_line, has_MPp) );
        	
        	if(has_MPp) accum_MPp_calls++;
        	
        	// update the current instruction length
        	instruction[instruction.size() - 1].length++;
        }
	}
	
	// error checks
	// MPp valid calls
	objetive_MPp_calls += 2; // including the two MPp calls in RET instruction
	if(accum_MPp_calls != objetive_MPp_calls){
		std::cout << "error: invalid number of MPp calls" << std::endl;
		getch();
		return 0;
	}
	
	// instruction names repetition
	std::vector<std::string> mnemonics;
	for(instruction_t ins : instruction){
		mnemonics.push_back(ins.name);
	}
	if(is_std_vector_duplicate_elements(mnemonics)){
		std::cout << "error: instruction names repetition" << std::endl;
		getch();
		return 0;
	}
	
	// aqui rellenar los 3 blocks de notas .txt con los datos
	
	// divide ROMmc_content in 2 files
	output_file_mc_high << "v2.0 raw" << std::endl;
	output_file_mc_low  << "v2.0 raw" << std::endl;
	
	for(std::string str : ROMmc_content){
		std::string high, low;
		
		// get the lower 32 bits(8 hex digits)
		low = str.substr( max(int(str.size()-1-(8-1)), int(0)), str.size() );
		
		// get the higher bits
		if(str.size() > 8){
			high = str.substr(0, str.size()-1-(8-1));
		}
		else{
			high = std::string("0");
		}
		
		output_file_mc_high << high << " ";
		output_file_mc_low  << low  << " ";
	}
	
	output_file_jmp << "v2.0 raw" << std::endl;
	for(std::string str : ROMjmp_content) output_file_jmp << str << " ";
	
	output_file_mp_jmp << "v2.0 raw" << std::endl;
	for(std::string str : ROMmpJMP_content) output_file_mp_jmp << str << " ";
	
	// compute average instruction length in clock cycles
	// for this, divide the valid number of microinstructions by number of instruction
	float average;
	average = float(ROMmc_content.size() - 1) / (instruction.size() - 1);
	
	output_file_length << "average clock-cycles per instruction: " << average << std::endl;
	output_file_length << "average instructions per second: " << decimal_format(to_string(int(cpu_hz / average))) << std::endl;
	output_file_length << "control bits existency count: " << cbn.size() << std::endl;
	output_file_length << "mc-rom bytes used: " << ROMmc_content.size() << std::endl;
	for(instruction_t ins : instruction){
		if(ins.name == std::string("FETCH")) continue;
		output_file_length << ins.name << " " << (ins.length + 1) << std::endl;
	}
	
    input_file.close();
    
    output_file_mc_high.close();
    output_file_mc_low .close();
    output_file_jmp    .close();
    output_file_length .close();
    output_file_mp_jmp .close();
    
    return 0;
}

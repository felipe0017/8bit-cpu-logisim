// miscellaneous

const std::vector<std::string> instruction_name = {
	"NOP",
	"LVA",
	"LVB",
	"LVC",
	"LAA",
	"LAB",
	"LAC",
	"SVA",
	"SVB",
	"SVC",
	"TBA",
	"TCA",
	"TAB",
	"TCB",
	"TAC",
	"TBC",
	"ADD",
	"ADV",
	"ADA",
	"ADC",
	"ACV",
	"ACA",
	"INA",
	"INB",
	"INC",
	"NOT",
	"TWO",
	"TWA",
	"AND",
	"ANV",
	"ANA",
	"ORR",
	"ORV",
	"ORA",
	"XOR",
	"XOV",
	"XOA",
	"SLR",
	"SLA",
	"SXR",
	"SXA",
	"SLL",
	"SYA",
	"SZL",
	"SZA",
	"SRR",
	"SRA",
	"SRL",
	"SWA",
	"CCF",
	"JMP",
	"JZZ",
	"JNZ",
	"JNE",
	"JNN",
	"JCC",
	"JNC",
	"CZF",
	"CNF",
	"SFZ",
	"SFN",
	"SFC",
	"SBR",
	"RET",
	"SSP",
	"UFA",
	"UFB",
	"UFC",
	"LPA",
	"SPA"
};

const std::vector<int> instruction_lenght = {
	1,
	2,
	2,
	2,
	3,
	3,
	3,
	3,
	3,
	3,
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	2,
	3,
	1,
	2,
	3,
	1,
	1,
	1,
	1,
	1,
	3,
	1,
	2,
	3,
	1,
	2,
	3,
	1,
	2,
	3,
	1,
	3,
	1,
	3,
	1,
	3,
	1,
	3,
	1,
	3,
	1,
	3,
	1,
	3,
	3,
	3,
	3,
	3,
	3,
	3,
	1,
	1,
	1,
	1,
	1,
	3,
	1,
	2,
	1,
	1,
	1,
	1,
	1
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

bool contain(std::string str, char c){
	bool exist = false;
	
	for(std::size_t i=0; i<str.size(); i++){
		if(str[i] == c){
			exist = true;
			break;
		}
	}
	
	return exist;
}

std::size_t seach(std::string str, char c){
	std::size_t index = 0;
	
	for(std::size_t i=0; i<str.size(); i++){
		if(str[i] == c){
			index = i;
			break;
		}
	}
	
	return index;
}

template <typename t>
std::string hex(t number){
	std::stringstream ss;
	ss << std::hex << number;
	return ss.str();
}

unsigned char hexDigitToNumber(char hex){
	unsigned char number;
	
	if(hex >= '0' and hex <= '9'){
		number = hex - '0';
	}
	else{
		number = hex - 'A' + 10;
	}
	
	return number;
}

// support horizontal tab delete
std::string delete_initial_spaces(std::string input){
	std::string output;
	
	std::size_t index = 0;
	bool flag = false;
	for(auto simbol : input){
		if(simbol != ' ' and simbol != 9){
			flag = true;
		}
		if(flag){
			output += simbol;
		}
		index++;
	}
	
	return output;
}

// support horizontal tab delete
std::string delete_final_spaces(std::string input){
	std::string output;
	
	// get the number of insignificant spaces
	std::size_t spaces_amount = 0;
	for(std::size_t i=0; i<input.size(); i++){
		std::size_t ii = input.size() - i - 1;
		char simbol = input[ii];
		
		if(simbol == ' ' or simbol == 9){
			spaces_amount++;
		}
		else{
			break;
		}
	}
	
	// delete last simbol 'spaces_amount' times in output
	output = input;
	for(std::size_t i=0; i<spaces_amount; i++){
		output.pop_back();
	}
	
	return output;
}

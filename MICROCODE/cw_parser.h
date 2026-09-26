// control bits names
const std::vector<std::string> cbn = {
	"FETCH",
    "MPo",
    "MPp",
    "MAo",
    "MAihblb",
    "MAilblb",
    "RAMany",
    "RAMo",
    "MDoram",
    "RAMi",
    "MDo",
    "MDi",
    "Ai",
    "Ao",
    "Bi",
    "Bo",
    "Ci",
    "Co",
    "A'i",
	"B'i",
	"ALU0",
	"ALU1",
	"ALU2",
	"ALU3",
	"CFc",
	"JMP",
	"ZFc",
	"NFc",
	"ZFs",
	"NFs",
	"CFs",
	"MPi",
	"Fo",
	"Fi",
	"SPi",
	"SPo",
	"SPp",
	"SPm",
	"MAihbhb",
	"MAolbhb"
};

const unsigned char alu_cw_offset_in_cw = 20;

const unsigned char alu_number_of_instructions = 15;

std::string cw_parser(std::string hcb, bool &has_MPp){ // hcb <- high control bits list as text

    std::istringstream iss(hcb);
    std::string cw_bit;
    
    unsigned long long number = 0ULL;
	
    while(iss >> cw_bit){
        // find bit control name
        bool valid = false;
        bool is_alu_bit = false;
        std::size_t index;
        for(std::size_t i=0; i<cbn.size(); i++){
        	
        	// check alu control bits existency
        	// for this, check if user-input current control bit star with "ALU"
        	std::string start;
        	start = cw_bit.substr(0, 3);
        	
        	if(cw_bit == std::string("MPp")) has_MPp = true;
        	
        	if(start == std::string("ALU")){
        		is_alu_bit = true;
        		valid = true;
        		break;
        	}
            else if(lowercase(cbn[i]) == lowercase(cw_bit)){
                valid = true;
                index = i;
                break;
            }
        }
        
        if(!valid){
        	std::cout << "error";
        	getch();
            exit(-1);
        }
        
        if(!is_alu_bit){
        	number = number | (1ULL << index);
        }
        else{
        	// alu-control-bits format is
        	// ALUcw{ddd...}
        	// ddd... is a length-variable decimal-coded string
        	// for get ddd... we need eliminate the first 6 chars ("ALUcw{") and the last ("}")
        	std::string alu_codeop;
        	alu_codeop = cw_bit.substr(6);
        	alu_codeop.pop_back();
        	
        	// convert decimal-coded string to int
        	int alu_codeop_int;
        	alu_codeop_int = atoi(alu_codeop.c_str());
        	
        	// check if "alu_codeop_int" is valid
        	// this need be in the range ]0, alu_number_of_instructions-1[
        	if(alu_codeop_int < 0 or alu_codeop_int >= alu_number_of_instructions){
				std::cout << "error";
				getch();
				exit(-1);
			}
			
        	// add this 4-low-bits to cw
        	number = number | (unsigned long long)(alu_codeop_int << (alu_cw_offset_in_cw - 0));
        }
    }
    
    std::string cw;
    cw = hex(number);
    return cw;
}

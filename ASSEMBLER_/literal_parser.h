// literal parser

unsigned char literal_parser(std::string str){
	std::string error_msg;
	std::stringstream ss;
	ss << "invalid literal <" << str << ">" << std::endl;
	error_msg = ss.str();
	
	unsigned char number;
	
	// examples of constant-format (all in uppercase!)
	// 0X0A (need be 2 digits)
	// 0B-0011-1001 (need be 8 digits and two '-' simbols)
	// 16
	
	if(str.size() >= 3){
		std::string prefix;
		prefix = str.substr(0, 2);
		
		if(prefix == std::string("0X")){
			
			bool valid = true;
			std::string unprocessed_str;
			unprocessed_str = str.substr(2);
			
			for(auto simbol : unprocessed_str){
				if( !( (simbol >= 'A' and simbol <= 'Z') or (simbol >= '0' and simbol <= '9')) ){
					valid = false;
					break;
				}
			}
			
			if(str.size() != 4 or !valid) error(error_msg);
			
			number = 0;
			number = number | (hexDigitToNumber(str[2]) << 4);
			number = number | (hexDigitToNumber(str[3]) << 0);
		}
		else if(prefix == std::string("0B")){
			// erase two '-' simbols in 'str'
			str.erase(7, 1);
			str.erase(2, 1);
			
			bool valid = true;
			std::string unprocessed_str;
			unprocessed_str = str.substr(2);
			
			for(auto simbol : unprocessed_str){
				if( !(simbol == '0' or simbol == '1') ){
					valid = false;
					break;
				}
			}
			
			if(unprocessed_str.size() != 8 or !valid) error(error_msg);
			
			for(unsigned char i=0; i<8; i++){
				number = number | ((str[i+2] - '0') << (8-i-1));
			}
		}
		else{
			goto x;
		}
	}
	else{
		x: 0;
		
		bool negative = false;
		
		if(str[0] == '-'){
			negative = true;
			str.erase( str.begin() );
		}
		
		bool valid = true;
		for(auto simbol : str){
			if( !(simbol >= '0' and simbol <= '9') ){
				valid = false;
				break;
			}
		}
		
		if(str.size() > 3 or !valid) error(error_msg);
		
		int unprocessed_number;
		unprocessed_number = atoi(str.c_str());
		
		if(unprocessed_number > 255) error(error_msg);
		
		number = unprocessed_number;
		
		if(negative){
			number = ~number + 1;
		}
	}
	
	return number;
}

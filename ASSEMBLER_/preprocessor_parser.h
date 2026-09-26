
void preprocessor_parser(std::size_t &sub_file_count){
	
	for(std::size_t i=0; i<file_content.size(); i++){
		std::string line;
		line = file_content[i];
		
		// if line is empty or is a commentary (start with #), ignore
        if(line.empty() or line[0] == '#'){
        	continue;
        }
        
        // check if part of line is a commentary
        if(contain(line, '#')){
        	line = line.substr(0, seach(line, '#'));
        }
		
		// extract first word
        std::istringstream iss(line);
    	std::string first_word;
		iss >> first_word;
		
		// parse the file-command
		// syntax: file {filename}.txt
		if(first_word == std::string("FILE")){
			
			std::vector< std::string > parameter;
			
			std::string current_parameter;
			while(iss >> current_parameter){
				parameter.push_back( current_parameter );
			}
			
			if(parameter.size() != 1){
				std::stringstream ss;
	        	ss << "invalid file-command declaration <" << line << "> (param_count != 1)" << std::endl;
				error(ss.str());
			}
			
			// try to open filename
			std::string filename;
			filename = parameter.front();
			
			std::ifstream file;
			file.open(filename, std::ios::in);
			if(file.fail()){
				std::stringstream ss;
			    ss << "file not found <" << filename << ">" << std::endl;
				error(ss.str());
			}
			
			sub_file_count++;
			
			// replace file-command by file-content
			// erase file-command line
			file_content.erase( file_content.begin() + i );
			
			// add sub-file-content to main-file-content
			std::size_t ii=0;
			while(!file.eof()){
				std::string sub_file_line;
				std::getline(file, sub_file_line);
				file_content.insert( file_content.begin() + i + ii, sub_file_line );
				ii++;
			}
			
			file.close();
		}
	}
}

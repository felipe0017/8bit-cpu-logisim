#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <stdlib.h>     /* exit, EXIT_FAILURE */
#include <fstream>
#include <conio.h>

void error(std::string str){
	std::cout << "error: " << str << std::endl;
	getch();
	exit(0);
}

std::vector< std::string > file_content;

#include "misc.h"
#include "literal_parser.h"
#include "dir_label.h"
#include "data_label.h"
#include "preprocessor_parser.h"


void open_main_file(std::string filename){
	std::ifstream input_file;
	input_file.open(filename, std::ios::in);
	if(input_file.fail()){
		std::stringstream ss;
	    ss << "main-file not found <" << filename << ">" << std::endl;
		error(ss.str());
	}
	while(!input_file.eof()){
		std::string file_line;
		std::getline(input_file, file_line);
		file_content.push_back( file_line );
	}
	input_file.close();
}

int main(int argc, char** argv){
	std::ofstream output_file_ram;
	std::ofstream output_file_statistics;
	
	std::vector<std::string> ram_content;
	
	output_file_ram.open("output-ram", std::ios::out);
	output_file_statistics.open("output-stats.txt", std::ios::out);
	
	// stats
	std::size_t stats_instruction_amount = 0;
	std::size_t stats_lines_amount = 0;
	std::size_t stats_lines_commentORempty_amount = 0;
	std::size_t stats_dir_label_amount = 0;
	std::size_t stats_data_label_amount = 0;
	std::size_t stats_file_amount = 1;
	
	std::size_t file_line_number = 0;
	
	open_main_file("input-code.txt");
	
	preprocessor_parser(stats_file_amount);
	
	// create an infinite loop when the program ends so as not to interpret
	// free bytes as instructions
	file_content.push_back( std::string("@END:") );
	file_content.push_back( std::string("	JMP @END") );
	
	for(auto file_line : file_content){
		stats_lines_amount++;
		
		// add more assembler code flexibility erasing start spaces
		file_line = delete_initial_spaces(file_line);
		
		// if line is empty or is a commentary (start with #), ignore
        if(file_line.empty() or file_line[0] == '#'){
        	stats_lines_commentORempty_amount++;
        	file_line_number++;
        	continue;
        }
        
        // check if part of line is a commentary
        if(contain(file_line, '#')){
        	file_line = file_line.substr(0, seach(file_line, '#'));
        }
        
        // add more assembler code flexibility erasing useless spaces
        file_line = delete_final_spaces(file_line);
        
        // extract first word
        std::istringstream iss(file_line);
    	std::string first_word; 
		iss >> first_word;
		
		// check if is a dir_label declaration
		if(first_word[0] == '@'){
			// check some declaration errors
			
			// invalid size
			if(first_word.size() < 3){
				std::stringstream ss;
	        	ss << "invalid dir_label declaration <" << first_word << "> (size<3)" << std::endl;
				error(ss.str());
			}
			
			// invalid number of words
			// if file_line contain spaces, launch error
			for(auto simbol : file_line){
				if(simbol == ' '){
					std::stringstream ss;
		        	ss << "invalid dir_label declaration <" << first_word << "> (wordsCount)" << std::endl;
					error(ss.str());
				}
			}
			
			// not end in ':'
			if(first_word.back() != ':'){
				std::stringstream ss;
	        	ss << "invalid dir_label declaration <" << first_word << "> (:)" << std::endl;
				error(ss.str());
			}
			
			// remove format-simbols ('@' and ':')
			std::string dir_label_name;
			dir_label_name = first_word;
			dir_label_name.erase( dir_label_name.begin() );
			dir_label_name.pop_back();
			
			// check if label name exist
			if(dir::exist(dir_label_name)){
				
				// if was declared, is a duplication error
				if( dir::was_declared(dir_label_name) ){
					std::stringstream ss;
		        	ss << "dir_label name duplication <" << dir_label_name << ">" << std::endl;
					error(ss.str());
				}
				
				// else, dir_label was mentioned but not declared
				// then, we declare it
				dir::update_declaration_address(dir_label_name, 257 + (ram_content.size() - 1) + 1);
			}
			else{
				// register new label
				dir::register_label(dir_label_name, 257 + (ram_content.size() - 1) + 1);
			}
			
			file_line_number++;
			stats_dir_label_amount++;
			continue;
		}
		// check if is a data_label declaration
		else if(first_word[0] == '%'){
			// check some declaration errors
			
			// invalid size
			if(first_word.size() < 3){
				std::stringstream ss;
	        	ss << "invalid data_label declaration <" << first_word << "> (size<3)" << std::endl;
				error(ss.str());
			}
			
			// not end in ':'
			if(first_word.back() != ':'){
				std::stringstream ss;
	        	ss << "invalid data_label declaration <" << first_word << "> (:)" << std::endl;
				error(ss.str());
			}
			
			// parse all byte-string
			std::vector<unsigned char> new_data;
			
			std::string byte;
			std::size_t data_string_length = 0;
			while(iss >> byte){
				new_data.push_back( literal_parser(byte) );
				data_string_length++;
			}
			
			// if label not have data, send error (ex: %LIST: )
			if( !data_string_length ){
				std::stringstream ss;
	        	ss << "invalid data_label declaration <" << first_word << "> (expected data)" << std::endl;
				error(ss.str());
			}
			
			// remove format-simbols ('%' and ':')
			std::string data_label_name;
			data_label_name = first_word;
			data_label_name.erase( data_label_name.begin() );
			data_label_name.pop_back();
			
			// check if label name exist
			if(!data::exist(data_label_name)){
				data::register_label(data_label_name);
			}
			
			// add new_data to label
			data::add_data(data_label_name, new_data);
			
			file_line_number++;
			stats_data_label_amount++;
			continue;
		}
		
		// check if is a mnemonic
        if(contain(instruction_name, first_word)){
        	ram_content.push_back( hex(seach(instruction_name, first_word)) );
        	stats_instruction_amount++;
        }
        else{
        	std::stringstream ss;
        	ss << "what is <" << first_word << "> ?" << std::endl;
			error(ss.str());
        }
        
        // parse all parameters
        std::size_t parameter_amount = 0;
        std::string parameter;
        while(iss >> parameter){
        	
        	// check if is a dir_label
        	if(parameter[0] == '@'){
        		// remove first simbol '@'
        		parameter.erase( parameter.begin() );
        		
        		// check if the current instruction accept a dir_label
        		// for this, check if the instruction length is 3 bytes
        		int lenght = instruction_lenght[seach(instruction_name, first_word)];
        		if(lenght != 3){
        			std::stringstream ss;
		        	ss << "instruction <" << first_word << "> not expected a dir_label" << std::endl;
					error(ss.str());
				}
				
				// check if dir_label name exist
				if(!dir::exist(parameter)){
				
					// register new dir_label as not-declared
					dir::register_label(parameter, 0);
				}
				
				// register new mention and reserve 2 bytes in ram memory to modify later
				dir::new_mention(parameter, 257 + (ram_content.size() - 1) + 2);
				ram_content.push_back( std::string("0") );
				ram_content.push_back( std::string("0") );
				
				// register this parameter as double parameter
				parameter_amount += 2;
        	}
        	// check if is a data_label
        	else if(parameter[0] == '%'){
        		// remove first simbol '%'
        		parameter.erase( parameter.begin() );
        		
        		// check if the current instruction accept a data_label
        		// for this, check if the instruction length is 3 bytes
        		int lenght = instruction_lenght[seach(instruction_name, first_word)];
        		if(lenght != 3){
        			std::stringstream ss;
		        	ss << "instruction <" << first_word << "> not expected a data_label" << std::endl;
					error(ss.str());
				}
				
				// check if data_label name exist
				if(!data::exist(parameter)){
				
					// register new data_label as not-declared
					data::register_label(parameter);
				}
				
				// register new mention and reserve 2 bytes in ram memory to modify later
				data::new_mention(parameter, 257 + (ram_content.size() - 1) + 2);
				ram_content.push_back( std::string("0") );
				ram_content.push_back( std::string("0") );
				
				// register this parameter as double parameter
				parameter_amount += 2;
        	}
        	// check if is a high/low mention of data_label
        	else if(parameter[0] == '>' or parameter[0] == '<'){
        		
        		// save mention type
        		bool mention_type; // 0->low 1->high
        		mention_type = parameter[0] == '>';
        		
        		// remove first simbol ('>' or '<')
        		parameter.erase( parameter.begin() );
				
				// check if data_label name exist
				if(!data::exist(parameter)){
				
					// register new data_label as not-declared
					data::register_label(parameter);
				}
				
				// register new high/low mention and reserve 1 byte in ram memory to modify later
				switch(mention_type){
					case true:
						data::new_high_mention(parameter, 257 + (ram_content.size() - 1) + 1);
						break;
					
					case false:
						data::new_low_mention(parameter, 257 + (ram_content.size() - 1) + 1);
						break;
				}
				ram_content.push_back( std::string("0") );
				
				// register this parameter as single parameter
				parameter_amount += 1;
        	}
        	else{
        		ram_content.push_back( hex(int(literal_parser(parameter))) );
        		parameter_amount++;
        	}
        }
        
        // check if amount of parameters is valid for current instruction
        int expected_parameter_amount = instruction_lenght[seach(instruction_name, first_word)];
        if( expected_parameter_amount != parameter_amount + 1){
        	std::stringstream ss;
        	ss << "invalid parameter amount in instruction <" << first_word << ">. the expected is <" << expected_parameter_amount << ">" << std::endl;
			error(ss.str());
        }
        
        file_line_number++;
	}
	
	// write in ram file
	output_file_ram << "v2.0 raw" << std::endl;
	output_file_ram << hex(seach(instruction_name, std::string("JMP"))) << " " << hex(0) << " " << hex(255) << " ";
	output_file_ram << "252*0 ";
	output_file_ram << hex(seach(instruction_name, std::string("SSP"))) << " " << hex(2) << " ";
	
	// before write ram_content in file, apply final address to all dir_label
	// for each label registered
	for(auto label : dir_label){
		
		// if dir_label was mentioned but not declared, send an error
		if(!label.declaration_address){
			std::stringstream ss;
		    ss << "dir_label <" << label.name << "> was mentioned, but never was declared" << std::endl;
			error(ss.str());
		}
		
		// for each mention
		for(auto mention : label.mention_address){
			
			mention = mention - 257;
			
			// update low byte
			ram_content[mention] = hex( label.declaration_address & 0xFF );
			
			// update high byte
			ram_content[mention-1] = hex( (label.declaration_address >> 8) & 0xFF );
		}
	}
	
	// before write ram_content in file, add all free data from data_label
	// vector to ram and after apply final address to all data_label
	// for each label registered
	for(auto label : data_label){
		
		// if data_label was mentioned but not declared, send an error
		if(!label.data.size()){
			std::stringstream ss;
		    ss << "data_label <" << label.name << "> was mentioned, but never was declared" << std::endl;
			error(ss.str());
		}
		
		// get the address of the first byte
		// of byte-string for current label
		unsigned int start_address;
		start_address = ram_content.size();
		start_address = start_address + 257;
		
		// add label data to ram_content
		for(auto byte : label.data){
			ram_content.push_back( hex(int(byte)) );
		}
		
		// for each mention
		for(auto mention : label.mention_address){
			
			// apply final address to data_label
			// the ram address of first data of
			// label.data is the address
			
			mention = mention - 257;
			
			// update low byte
			ram_content[mention] = hex( start_address & 0xFF );
			
			// update high byte
			ram_content[mention-1] = hex( (start_address >> 8) & 0xFF );
		}
		
		// for each high mention
		for(auto mention : label.high_mention_address){
			
			// apply final part-of-address to data_label high mention
			// the high byte of mention is for high-mentions
			
			mention = mention - 257;
			
			// update byte
			ram_content[mention] = hex( (start_address >> 8) & 0xFF );
		}
		
		// for each low mention
		for(auto mention : label.low_mention_address){
			
			// apply final part-of-address to data_label low mention
			// the low byte of mention is for low-mentions
			
			mention = mention - 257;
			
			// update byte
			ram_content[mention] = hex( start_address & 0xFF );
		}
	}
	
	for(std::string str : ram_content) output_file_ram << str << " ";
	
	// write in statistics file
	// not consider infinite cycle at the end as valid stats
	stats_instruction_amount--;
	stats_lines_amount -=2 ;
	stats_dir_label_amount--;
	output_file_statistics << "stats:" << std::endl;
	output_file_statistics << "ram bytes used: " << ram_content.size() + 257 << std::endl;
	output_file_statistics << "number of lines: " << stats_lines_amount << std::endl;
	output_file_statistics << "number of full comment/empty lines: " << stats_lines_commentORempty_amount << std::endl;
	output_file_statistics << "number of instructions: " << stats_instruction_amount << std::endl;
	output_file_statistics << "number of dir_label: " << stats_dir_label_amount << std::endl;
	output_file_statistics << "number of data_label: " << stats_data_label_amount << std::endl;
	output_file_statistics << "file amount: " << stats_file_amount << std::endl;
	
	output_file_ram.close();
	output_file_statistics.close();
	
	std::cout << "all okay" ;
	return 0;
}

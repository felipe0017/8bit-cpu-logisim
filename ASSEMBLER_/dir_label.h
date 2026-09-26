
struct dir_label_t{
	std::string name;
	unsigned int declaration_address; // two-byte RAM address of start of declaration, 0 -> no declared (error)
	std::vector<int> mention_address; // RAM address of low byte of mentions
};

std::vector<dir_label_t> dir_label;

namespace dir{
	
	bool exist(std::string dir_label_name){
	
		for(auto label : dir_label){
			if(label.name == dir_label_name){
				return true;
			}
		}
	
		return false;
	}
	
	void register_label(std::string dir_label_name, int declaration_address){
		dir_label_t ddir_label;
		ddir_label.name = dir_label_name;
		ddir_label.declaration_address = declaration_address;
		dir_label.push_back(ddir_label);
	}
	
	void new_mention(std::string dir_label_name, int mmention_address){
		
		std::size_t index = 0;
		for(auto label : dir_label){
			if(label.name == dir_label_name){
				break;
			}
			index++;
		}
		
		dir_label[index].mention_address.push_back(mmention_address);
	}
	
	void update_declaration_address(std::string dir_label_name, int ddeclaration_address){
		
		std::size_t index = 0;
		for(auto label : dir_label){
			if(label.name == dir_label_name){
				break;
			}
			index++;
		}
		
		dir_label[index].declaration_address = ddeclaration_address;
	}
	
	bool was_declared(std::string dir_label_name){
		
		std::size_t index = 0;
		for(auto label : dir_label){
			if(label.name == dir_label_name){
				break;
			}
			index++;
		}
		
		return (dir_label[index].declaration_address);
	}
};

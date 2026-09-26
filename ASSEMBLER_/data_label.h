
struct data_label_t{
	std::string name;
	std::vector<unsigned char> data; // string of contiguous bytes. if length is zero, is a no declaration error
	std::vector<int> mention_address; // RAM address of low byte of mentions
	std::vector<int> low_mention_address; // special mention of low byte
	std::vector<int> high_mention_address; // special mention of high byte
};

std::vector<data_label_t> data_label;

namespace data{
	
	bool exist(std::string data_label_name){
		
		for(auto label : data_label){
			if(label.name == data_label_name){
				return true;
			}
		}
	
		return false;
	}
	
	void register_label(std::string data_label_name){
		data_label_t ddata_label;
		ddata_label.name = data_label_name;
		data_label.push_back(ddata_label);
	}
	
	void new_mention(std::string data_label_name, int mmention_address){
		
		std::size_t index = 0;
		for(auto label : data_label){
			if(label.name == data_label_name){
				break;
			}
			index++;
		}
		
		data_label[index].mention_address.push_back(mmention_address);
	}
	
	
	void new_low_mention(std::string data_label_name, int llow_mention_address){
		
		std::size_t index = 0;
		for(auto label : data_label){
			if(label.name == data_label_name){
				break;
			}
			index++;
		}
		
		data_label[index].low_mention_address.push_back(llow_mention_address);
	}
	
	void new_high_mention(std::string data_label_name, int hhigh_mention_address){
		
		std::size_t index = 0;
		for(auto label : data_label){
			if(label.name == data_label_name){
				break;
			}
			index++;
		}
		
		data_label[index].high_mention_address.push_back(hhigh_mention_address);
	}
	
	void add_data(std::string data_label_name, std::vector<unsigned char> new_data){
		
		std::size_t index = 0;
		for(auto label : data_label){
			if(label.name == data_label_name){
				break;
			}
			index++;
		}
		
		for(auto byte : new_data){
			data_label[index].data.push_back(byte);
		}
	}
};

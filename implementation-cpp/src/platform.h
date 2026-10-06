#pragma once
#include <string>
#include <vector>

namespace platform {

std::string read_file(const std::string& path);
bool file_exists(const std::string& path);
bool write_file(const std::string& path, const std::string& data);
std::string get_data_dir();
std::string get_writable_dir();
bool zip_ready();
void try_open_zip();
std::vector<std::string> verify_assets(const std::string& os, const std::vector<std::string>& bundle_names);

#ifdef __ANDROID__
void init_android();
#endif

}

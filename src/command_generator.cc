// MIT License
// 
// Copyright (c) [2020] [Jerry ZJ]
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "argparse.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv) {
  argparse::ArgumentParser program("GCP Firewall Command Generator");
  program.add_argument("--projectid", "-id").required().help("GCP project id");
  program.add_argument("--input", "-i").required().help("IP list file in CIDR format");
  program.add_argument("--output", "-o").default_value(std::string("block_china.sh")).required().help("Specify the output file.");

  try {
    program.parse_args(argc, argv);
  } catch(const std::runtime_error& err) {
    std::cout << err.what() << std::endl;
    std::cout << program;
    exit(0);
  }
  std::ifstream in_file(program.get<std::string>("-i"), std::ios::in);
  std::ofstream out_file(program.get<std::string>("-o"), std::ios::out);
  std::string buffer;

  // Command headers
  std::string command_header("gcloud compute ");
  std::string project("--project=");
  std::string project_id(program.get<std::string>("-id") + " ");
  std::string firewall_command("firewall-rules create ");
  std::string firewall_rule_name("block-china-");
  std::string firewall_rule_direction("--direction=INGRESS ");
  std::string firewall_rule_priority("--priority=");
  std::string firewall_rule_network("--network=default ");
  std::string firewall_rule_action("--action=DENY ");
  std::string firewall_rule_target("--rules=all ");
  std::string firewall_rupe_ip("--source-ranges=");
  std::vector<std::string> ip_ranges;

  // Counters
  int command_counter(0);   // How many commands generated
  int ip_dumped_counter(0); // How many IP ranges is dumped
  int priority_counter(0);  // Firewall priority setting

  // Read IP block list
  buffer.clear();
  while (std::getline(in_file, buffer)) {
    ip_ranges.push_back(buffer);
    buffer.clear();
  }
  in_file.close();
  // Dump IP list and generate gcloud shell commands
  // Note that each firewall rule has maximum limit of IP ranges
  for (auto ip : ip_ranges) {
    if ((ip_dumped_counter % 256) == 0) {
      ++command_counter;
      ++priority_counter;
      out_file << command_header;
      out_file << project;
      out_file << project_id;
      out_file << firewall_command;
      out_file << firewall_rule_name + std::to_string(command_counter) + " ";
      out_file << firewall_rule_direction;
      out_file << firewall_rule_priority + std::to_string(priority_counter) +
                      " ";
      out_file << firewall_rule_network;
      out_file << firewall_rule_action;
      out_file << firewall_rule_target;
      out_file << firewall_rupe_ip;
    }
    ++ip_dumped_counter;
    if ((ip_dumped_counter % 256 == 0 ||
         ip_dumped_counter == ip_ranges.size())) {
      out_file << ip << std::endl;
    } else {
      out_file << ip << ",";
    }
  }
  out_file.close();

  return 0;
}
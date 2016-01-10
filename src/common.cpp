
/*
 * ansvif
 * A Not So Very Intelligent Fuzzer
 * Marshall Whittaker / oxagast
 */


#include <thread>
#include <sys/wait.h>
#include <sys/stat.h>
#include <vector>
#include <unistd.h>
#include <cstdlib>
#include <regex>
#include <iostream>
#include <stdio.h>
#include <fstream>

#define READ 0
#define WRITE 1

bool match_seg(int buf_size, std::vector<std::string> opts,
               std::vector<std::string> spec_env, std::string path_str,
               std::string strip_shell, bool rand_all, bool write_to_file,
               std::string write_file_n, bool rand_buf,
               std::vector<std::string> opt_other, bool is_other,
               std::string other_sep, int t_timeout, std::string low_lvl_user,
               std::string junk_file_of_args, std::string always_arg,
               bool never_rand, std::string run_command, std::regex sf_reg, bool verbose,
               bool debug);
void help_me(std::string mr_me);
std::vector<std::string> get_flags_man(char *cmd, std::string man_loc,
                                       bool verbose, bool debug,
                                       bool dump_opts);



std::string remove_chars(const std::string &source, const std::string &chars) {
  std::string result = "";
  for (unsigned int i = 0; i < source.length(); i++) {
    bool foundany = false;
    for (unsigned int j = 0; j < chars.length() && !foundany; j++) {
      foundany = (source[i] == chars[j]);
    }
    if (!foundany) {
      result += source[i];
    }
  }
  return (result);
}

int toint(std::string ints) {
  return atoi(ints.c_str());  // for compatability with cygwin and mingw
}

bool file_exists(const std::string &filen) {
  struct stat buf;
  return (stat(filen.c_str(), &buf) == 0);
}



int rand_me_plz(int rand_from, int rand_to) {
  std::random_device rd;
  std::default_random_engine generator(rd()); // seed random
  std::uniform_int_distribution<int> distribution(rand_from,
                                                  rand_to); // get a random
  auto roll = std::bind(distribution,
                        generator); // bind it so we can do it multiple times
  return (roll());
}

char fortune_cookie() {
  char chr;
  const char *hex_digits = "0123456789ABCDEF";
  int i;
  for (i = 0; i < 1; i++) {
    chr = hex_digits[(rand_me_plz(0, 255))];
  }
  return (chr);
}

int reaper(int grim, int t_timeout) {
  //  std::this_thread::sleep_for(std::chrono::milliseconds(t_timeout));
  sleep(t_timeout);
  kill(grim, 9);
  return (0);
}

std::vector<std::string> get_flags_template(std::string filename, bool verbose,
                                            bool debug) {
  std::vector<std::string> opt_vec;
  std::string line;
  std::ifstream template_file(filename);
  if (template_file.is_open()) {
    while (std::getline(template_file, line)) {
      opt_vec.push_back(line);
    }
    template_file.close();
  } else {
    std::cerr << "Could not open template file..." << std::endl;
    exit(1);
  }
  return (opt_vec);
}

std::vector<std::string> get_other(std::string filename, bool verbose,
                                   bool debug) {
  std::vector<std::string> other_vec;
  std::string line;
  std::ifstream other_file(filename);
  if (other_file.is_open()) {
    while (std::getline(other_file, line)) {
      other_vec.push_back(line);
    }
    other_file.close();
  } else {
    std::cerr << "Could not open template file..." << std::endl;
    exit(1);
  }
  return (other_vec);
}

std::string trash_generator(int trash, int buf, std::string user_junk,
                            std::string opt_other_str, bool never_rand) {
  std::string junk = "";
  std::string hex_stuff;
  int trash_num;
  if (trash == 0) { // kosher
    for (trash_num = 0; trash_num < buf; trash_num++) {
      junk = "A" + junk; // put lots of As
    }
  }
  if (trash == 1) {
    for (trash_num = 0; trash_num < buf; trash_num++) {
      junk = "9" + junk; // yadda yadda
    }
  }
  if (trash == 2) {
    for (trash_num = 0; trash_num < buf/2; trash_num++) {
      junk = "\%s" + junk; // yadda yadda
    }
  }
  if (never_rand == false) {
    if (trash == 3) {
      for (trash_num = 0; trash_num < buf; trash_num++) {
        junk = junk += fortune_cookie();
      }
    }
  }
  if (trash == 4) { // front
    for (trash_num = 0; trash_num < buf; trash_num++) {
      junk = "A" + junk; // put lots of As
    }
    junk = user_junk + junk;
    if (buf - user_junk.length() < junk.size())
      junk = junk.substr(0, buf);
    else
      return ("OOR");
  }
  if (trash == 5) {
    for (trash_num = 0; trash_num < buf; trash_num++) {
      junk = "9" + junk; // yadda yadda
    }
    junk = user_junk + junk;
    if (buf - user_junk.length() < junk.size())
      junk = junk.substr(0, buf);
    else
      return ("OOR");
  }
    if (trash == 6) {
    for (trash_num = 0; trash_num < buf/2; trash_num++) {
      junk = "\%s" + junk; // yadda yadda
    }
    junk = user_junk + junk;
    if (buf - user_junk.length() < junk.size())
      junk = junk.substr(0, buf);
    else
      return ("OOR");
  }
  if (never_rand == false) {
    if (trash == 7) {
      for (trash_num = 0; trash_num < buf; trash_num++) {
        junk = junk += fortune_cookie();
      }
      junk = user_junk + junk;
      if (buf - user_junk.length() < junk.size())
        junk = junk.substr(0, buf);
      else
        return ("OOR");
    }
  }
  if (trash == 8) {
    for (trash_num = 0; trash_num < buf; trash_num++) { // back
      junk = "A" + junk;                                // put lots of As
    }
    junk = junk + user_junk;
    if (buf - user_junk.length() < junk.size())
      junk = junk.substr(junk.length() - buf);
    else
      return ("OOR");
  }
  if (trash == 9) {
    for (trash_num = 0; trash_num < buf; trash_num++) {
      junk = "9" + junk; // yadda yadda
    }
    junk = junk + user_junk;
    if (buf - user_junk.length() < junk.size())
      junk = junk.substr(junk.length() - buf);
    else
      return ("OOR");
  }
  if (trash == 10) {
    for (trash_num = 0; trash_num < buf/2; trash_num++) {
      junk = "\%s" + junk; // yadda yadda
    }
    junk = junk + user_junk;
    if (buf - user_junk.length() < junk.size())
      junk = junk.substr(junk.length() - buf);
    else
      return ("OOR");
  }
  if (never_rand == false) {
    if (trash == 11) {
      for (trash_num = 0; trash_num < buf; trash_num++) {
        junk = junk += fortune_cookie();
      }
      junk = junk + user_junk;
      if (buf - user_junk.length() < junk.size())
        junk = junk.substr(junk.length() - buf);
      else
        return ("OOR");
    }
  }
  if (trash == 12) {
    junk = opt_other_str;
  }
  return (junk);
}

std::string make_garbage(int trash, int buf, std::string opt_other_str,
                         bool is_other, bool never_rand) {
  buf = buf - 1;
  std::string all_junk;
  if (is_other == true) {
    if (isatty(STDIN_FILENO)) {
      std::string user_stuff = "";
      all_junk =
          trash_generator(trash, buf, user_stuff, opt_other_str, never_rand);
    } else {
      std::string user_stuff;
      getline(std::cin, user_stuff);
      all_junk =
          trash_generator(trash, buf, user_stuff, opt_other_str, never_rand);
    }
  } else if (is_other == false) {
    if (isatty(STDIN_FILENO)) {
      std::string user_stuff = "";
      all_junk = trash_generator(trash, buf, user_stuff, "", never_rand);
    } else {
      std::string user_stuff;
      getline(std::cin, user_stuff);
      all_junk = trash_generator(trash, buf, user_stuff, "", never_rand);
    }
  }
  return (all_junk);
}

void write_seg(std::string filename, std::string seg_line) {
  std::ofstream w_f;
  w_f.open(filename, std::ios::out | std::ios::app);
  w_f << seg_line << std::endl;
  w_f.close();
}

void write_junk_file(std::string filename, std::vector<std::string> opt_other,
                     int buf_size, int rand_spec_one, int rand_spec_two,
                     bool never_rand, std::string other_sep, bool verbose) {
  remove(filename.c_str());
  std::string oscar;
  std::ofstream w_f;
  w_f.open(filename, std::ios::out | std::ios::app);
  for (int start_buf = 0; start_buf <= buf_size; start_buf++) {
    std::string oscar = opt_other.at(rand_me_plz(0, opt_other.size() - 1));
    std::string trash =
        make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                     rand_me_plz(1, buf_size), "", false, never_rand);
    w_f << oscar;
    if (trash != "OOR") {
      w_f << trash;
    }
    int is_sep = rand_me_plz(0, 1);
    if (is_sep == 1) {
      w_f << other_sep << std::endl;
    } else {
      w_f << std::endl;
    }
    if (verbose == true) {
      std::cerr << oscar;
      {
        if (trash != "OOR")
          std::cerr << trash;
      }
      if (is_sep == 1) {
        std::cerr << other_sep << std::endl;
      } else {
        std::cerr << std::endl;
      }
    }
  }
  w_f.close();
}
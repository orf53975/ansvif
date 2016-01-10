#include <string>
#include <vector>
#include <unistd.h>
#include <iomanip>
#include <thread>

bool match_seg(int buf_size, std::vector<std::string> opts,
               std::vector<std::string> spec_env, std::string path_str,
               std::string strip_shell, bool rand_all, bool write_to_file,
               std::string write_file_n, bool rand_buf,
               std::vector<std::string> opt_other, bool is_other,
               std::string other_sep, int t_timeout, std::string low_lvl_user,
               std::string junk_file_of_args, std::string always_arg,
               bool never_rand, std::string run_command, std::regex sf_reg, bool verbose,
               bool debug) {
  bool segged = false;
  if (file_exists(path_str) == true) {
    while (segged == false) {
      int rand_spec_one, rand_spec_two;
      if (rand_all == true) {
        rand_spec_one = 2;
        rand_spec_two = 2;
      } else {
        rand_spec_one = 0;
        rand_spec_two = 12;
      }
      std::vector<std::string> junk_opts_env;
      std::vector<std::string> junk_opts;
      std::string env_str;
      std::string sys_str;
      if (junk_file_of_args != "") {
        write_junk_file(junk_file_of_args, opt_other, buf_size, rand_spec_one,
                        rand_spec_two, never_rand, other_sep, verbose);
      }
      int sep_type;
      for (int cmd_flag_l = 0; cmd_flag_l < opts.size();
           cmd_flag_l++) {            // loop around the options
        if (rand_me_plz(0, 1) == 1) { // roll tha die
          junk_opts.push_back(
              opts.at(cmd_flag_l)); // put the random arg in the vector
        }
      }
      for (int cmd_flag_a = 0; cmd_flag_a < spec_env.size();
           cmd_flag_a++) {            // loop around the options
        if (rand_me_plz(0, 1) == 1) { // roll tha die
          junk_opts_env.push_back(
              spec_env.at(cmd_flag_a)); // put the random arg in the vector
        }
      }
      if (is_other == true) {
        if (rand_buf == true) {
          for (std::vector<std::string>::const_iterator junk_opt_env =
                   junk_opts_env.begin();
               junk_opt_env != junk_opts_env.end();
               ++junk_opt_env) { // loop through the vector of junk envs
            std::string oscar_env = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             rand_me_plz(1, buf_size),
                             opt_other.at(rand_me_plz(0, opt_other.size() - 1)),
                             is_other, never_rand),
                " ");
            if (oscar_env != "OOR") {
              env_str = env_str + *junk_opt_env + oscar_env + " ";
            }
          }
          for (std::vector<std::string>::const_iterator junk_opt =
                   junk_opts.begin();
               junk_opt != junk_opts.end();
               ++junk_opt) { // loop through the vector of junk opts
            std::string oscar = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             rand_me_plz(1, buf_size),
                             opt_other.at(rand_me_plz(0, opt_other.size() - 1)),
                             is_other, never_rand),
                strip_shell);
            if (oscar != "OOR") {
              sep_type = rand_me_plz(0, 1);
              if (sep_type == 0) {
                sys_str = " " + sys_str + *junk_opt + " " + oscar + " ";
              }
              if (sep_type == 1) {
                sys_str =
                    sys_str + " " + *junk_opt + other_sep + oscar + other_sep;
              }
            }
          }
        } else if (rand_buf == false) {
          for (std::vector<std::string>::const_iterator junk_opt_env =
                   junk_opts_env.begin();
               junk_opt_env != junk_opts_env.end();
               ++junk_opt_env) { // loop through the vector of junk envs
            std::string oscar_env = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             buf_size,
                             opt_other.at(rand_me_plz(0, opt_other.size() - 1)),
                             is_other, never_rand),
                " ");
            if (oscar_env != "OOR") {
              env_str = env_str + *junk_opt_env + oscar_env + " ";
            }
          }
          for (std::vector<std::string>::const_iterator junk_opt =
                   junk_opts.begin();
               junk_opt != junk_opts.end();
               ++junk_opt) { // loop through the vector of junk opts
            std::string oscar = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             buf_size,
                             opt_other.at(rand_me_plz(0, opt_other.size() - 1)),
                             is_other, never_rand),
                strip_shell);
            if (oscar != "OOR") {
              sep_type = rand_me_plz(0, 1);
              if (sep_type == 0) {
                sys_str = " " + sys_str + *junk_opt + " " + oscar + " ";
              }
              if (sep_type == 1) {
                sys_str =
                    sys_str + " " + *junk_opt + other_sep + oscar + other_sep;
              }
            }
          }
        }
      }
      if (is_other == false) {
        if (rand_buf == true) {
          for (std::vector<std::string>::const_iterator junk_opt_env =
                   junk_opts_env.begin();
               junk_opt_env != junk_opts_env.end();
               ++junk_opt_env) { // loop through the vector of junk envs
            std::string oscar_env = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             rand_me_plz(1, buf_size), "", is_other,
                             never_rand),
                " ");
            if (oscar_env != "OOR") {
              env_str = env_str + *junk_opt_env + oscar_env + " ";
            }
          }
          for (std::vector<std::string>::const_iterator junk_opt =
                   junk_opts.begin();
               junk_opt != junk_opts.end();
               ++junk_opt) { // loop through the vector of junk opts
            std::string oscar = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             rand_me_plz(1, buf_size), "", is_other,
                             never_rand),
                strip_shell);
            if (oscar != "OOR") {
              sep_type = rand_me_plz(0, 1);
              if (sep_type == 0) {
                sys_str = " " + sys_str + *junk_opt + " " + oscar + " ";
              }
              if (sep_type == 1) {
                sys_str =
                    sys_str + " " + *junk_opt + other_sep + oscar + other_sep;
              }
            }
          }
        } else if (rand_buf == false) {
          for (std::vector<std::string>::const_iterator junk_opt_env =
                   junk_opts_env.begin();
               junk_opt_env != junk_opts_env.end();
               ++junk_opt_env) { // loop through the vector of junk envs
            std::string oscar_env = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             buf_size, "", is_other, never_rand),
                " ");
            if (oscar_env != "OOR") {
              env_str = env_str + *junk_opt_env + oscar_env + " ";
            }
          }
          for (std::vector<std::string>::const_iterator junk_opt =
                   junk_opts.begin();
               junk_opt != junk_opts.end();
               ++junk_opt) { // loop through the vector of junk opts
            std::string oscar = remove_chars(
                make_garbage(rand_me_plz(rand_spec_one, rand_spec_two),
                             buf_size, "", is_other, never_rand),
                strip_shell);
            if (oscar != "OOR") {
              sep_type = rand_me_plz(0, 1);
              if (sep_type == 0) {
                sys_str = " " + sys_str + *junk_opt + " " + oscar + " ";
              }
              if (sep_type == 1) {
                sys_str =
                    sys_str + " " + *junk_opt + other_sep + oscar + other_sep;
              }
            }
          }
        }
      }
      std::string out_str;
      std::string out_str_p;
      if (sys_str != "") {
        if (env_str != "") {
          out_str_p = "$(printf \"\\\\x" + binstr_to_hex(env_str) + "\") " +
                    path_str + " $(printf \"\\\\x" + binstr_to_hex(sys_str) +
                    "\") " + always_arg;
        }
        if (env_str == "") {
          out_str_p = path_str + " $(printf \"\\\\x" + binstr_to_hex(sys_str) +
                    "\") " + always_arg;
        }
        out_str = env_str + " " + path_str + " " + sys_str + " " + always_arg;
      }
      out_str = out_str + "; echo $?"; // get the signal
      if (out_str != "; echo $?") {
        junk_opts.clear();
        junk_opts.shrink_to_fit();
        junk_opts_env.clear();
        junk_opts_env.shrink_to_fit();
        if (debug == true) {
          std::ofstream w_f;
          w_f.open(write_file_n, std::ios::out | std::ios::app);
          w_f << out_str << std::endl << out_str_p << std::endl << std::endl;
          w_f.close();
          std::cout << out_str << std::endl << out_str_p << std::endl << std::endl;
        }
        int pid; // initializes child
        FILE *fp =
            popen2(out_str, "r", pid, low_lvl_user); // opens child process fork
        char command_out[4096] = {0};
        std::stringstream output;
        while (read(fileno(fp), command_out, sizeof(command_out) - 1) != 0) {
          output << std::string(command_out);
          memset(&command_out, 0, sizeof(command_out));
        }
        pclose2(fp, pid);
        if (run_command != "") {
          int run_com_pid; // initializes child
          FILE *fp = popen2(run_command, "r", run_com_pid,
                            low_lvl_user); // opens child process fork
          pclose2(fp, run_com_pid);
        }
        std::thread reaper_thread(
            reaper, pid,
            t_timeout); // takes care of killing it off if it takes too long
        reaper_thread.detach();
        std::string token;
        while (std::getline(output, token)) {
//          std::regex sf_reg("(132|136|139|135|134|159)"); // regex for the crash
          //std::regex sf_reg(catch_sig);
          std::smatch sf;
          if (regex_match(token, sf, sf_reg)) { // match crash
            std::cout << "Crashed with command: " << std::endl
                      << out_str_p << std::endl;
            if (junk_file_of_args != "") {
              std::cout << "File data left in: " << junk_file_of_args
                        << std::endl;
            }
            if (write_to_file == true) {
              write_seg(write_file_n, out_str_p);
              std::cout << "Crash logged." << std::endl;
              exit(0);
            } else {
              exit(0);
            }
          }
        }
      }
    }
  }
  else {
    std::cerr << "Command not found at path..." << std::endl;
    exit(1);
  }
}
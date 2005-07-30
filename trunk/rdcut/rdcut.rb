#!/usr/bin/ruby
# RDCUT
# Ruby Driven C++ Unit Testing
# The following code is hereby released into the public domain

def get_command(line)
  if line =~ /^\/\/R\s*(.*)(\s*)$/
    $1
  end
end


class Generator
  attr_reader :code
  def initialize
    @tests = []
    @test_count = 0
    @code = ""
    @preamble = ""
  end

  def process_command(name, body, args, line)
    case name
    when "test"
      @tests << args
      generate_method("test#{@test_count}", body, line)
      @test_count += 1
    when "teardown"
      generate_method("tearDown", body, line)
    when "setup"
      generate_method("setUp", body, line)
    when "preamble"
      prepend body + "\n"
    when "include"
      $files << args
    else
      return
    end    
  end
  def generate_method(name, body, line)
    append "void #{name}() {\n"
    append "#line #{line+1} \"#{$filename}\"\n"
    body.each_line do |line|
      append "  #{line}"
    end
    append "}\n\n"
  end

  def generate_all
    text =  <<-EOF
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;
int current_test;
int ok;
int failed;
vector<string> test_names;
void rassert(bool, const char* = 0, bool = false);

template <typename T1, typename T2>
void rassert_equal(const T1&, const T2&);

template <typename T1, typename T2>
void rassert_equal_float(const T1&, const T2&, double epsilon = 1e-10);

#{generate_rassert}
EOF
    append  <<-EOF
int main() {

  ok = 0;
  failed = 0;
  test_names.resize(#{@test_count});
#{fill_test_vector}


  // fixture initialization
  setUp();

#{generate_test_calls("  ")}
  // fixture destruction
  tearDown();

  cout << " --- DONE ---" << endl;
  cout << ok << " assertions passed" << endl;
  cout << failed << " assertions failed" << endl;

  return 0;
}
EOF
    @code = text + @preamble + @code
  end

  private
  def append(text)
    @code += text
  end

  def prepend(text)
    @preamble += text
  end

  def generate_test_calls(ind)
    res = ""
    (0...@test_count).each do |i|
      res += <<-EOF
#{ind}// #{@tests[i]}
#{ind}current_test = #{i};
#{ind}test#{i}();
EOF
    end
    res
  end

  def generate_rassert()
    <<-EOF
void rassert(bool condition, const char* message, bool msg_on_pass) {
  if (!condition) {
    cout << "ASSERTION FAILED on test" << endl;
    cout << "  " << test_names[current_test] << endl;
    if (message)
      cout << "  " << message << endl;
    failed++;
  }
  else {
    if (message && msg_on_pass) {
      cout << "assertion passed (";
      cout << message << ")" << endl;
    }
    ok++;
  }
}

template <typename T1, typename T2>
void rassert_equal(const T1& x1, const T2& x2) {
  ostringstream msg;
  msg << "expected " << x2 << ", got " << x1;
  rassert(x1 == x2, msg.str().c_str(), false);
}

template <typename T1, typename T2>
void rassert_equal_float(const T1& x1, const T2& x2, double epsilon) {
  ostringstream msg;
  msg << "expected " << x2 << ", got " << x1;
  rassert(abs(x1-x2) < epsilon, msg.str().c_str(), false);
}

EOF
  end

  def fill_test_vector()
    res = ""
    (0...@test_count).each do |i|
      res += "test_names[#{i}] = \"#{@tests[i]}\";\n"
    end
    res
  end
end

file = ARGV[0]
unless file
  STDERR.puts "usage: rdcut FILENAME"
  exit 1
end

output = file + ".cpp"

content = ""
method_name = nil
args = nil
gen = Generator.new
lineno = 1
method_line = 1

$files = [file]
file_index = 0
while $files[file_index]
  $filename = $files[file_index]
  File.new($filename).each_line do |line|
    if cmd = get_command(line)
      gen.process_command(method_name, content, args, method_line)
      method_line = lineno
      content = ""
      cmd.chomp =~ /^(\w*)\s*(.*)$/
      method_name = $1
      args = $2
    else
      # check assertion
      content += line
    end
    lineno += 1
  end
  gen.process_command(method_name, content, args, method_line)
  content = ""
  method_name = nil
  args = nil
  lineno = 1
  method_line = 1

  file_index += 1
end

gen.generate_all
puts gen.code

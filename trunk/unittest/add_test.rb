#!/usr/bin/ruby
require 'erb'

def usage
  warn "Usage: #$0 TEST_NAME"
  exit 1
end

@name = ARGV.shift or usage
%w(cpp h).each do |ext|
  File.open("#{@name}.#{ext}",'w') do |file|
    file.write(ERB.new(File.open("template.#{ext}.erb").read).result(binding))
  end
end

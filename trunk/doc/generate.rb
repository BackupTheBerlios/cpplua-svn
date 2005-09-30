#!/usr/bin/ruby
require 'rubygems'
require_gem 'rake'
require 'rake'
require 'hlight/hlight'

$doc = Dir['xml/*.xml']

def transform(file, prefix, extension)
  File.join(prefix,File.basename(file, File.extname(file))+'.'+extension)
end

rule( /^temp\/.*\.xml$/ => [
  proc {|doc| transform(doc, 'xml', 'xml')}
]) do |t|
  warn "generating #{t.name}"
  File.open(t.name, 'w') do |output|
    HLight.main(File.new(t.source), output, 'hlight/config.yaml')
  end
end

rule( /^html\/.*\.html/ => [
  proc {|doc| transform(doc, 'temp', 'xml')}
]) do |t|
  warn "generating #{t.name}"
  `xsltproc documentation.xsl #{t.source} > #{t.name}`
end

task :default => $doc.map{|doc| transform(doc, 'html', 'html')}

if $0 == __FILE__
  while arg = ARGV.shift
    case arg
    when "-a","--all"
      Dir['xml/*.xml'].each do |xml_file|
        `touch #{xml_file}`
      end
    end
  end
    
  Task["default"].invoke
end
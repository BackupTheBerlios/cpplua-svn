#!/usr/bin/ruby
require 'highlighter'

Dir['*.xml'].each do |doc_file|
  base = File.basename(doc_file, File.extname(doc_file))
  temp_file = File.join("temp",doc_file)
  output_file = File.join("html", base + ".html")
  
  File.open(temp_file, 'w') do |output|
    HLight.main(File.new(doc_file), output)
  end

  `xsltproc documentation.xsl #{temp_file} > #{output_file}`
end

#!/usr/bin/ruby
require 'hlight/hlight'

Dir['xml/*.xml'].each do |doc_file|
  base = File.basename(doc_file, File.extname(doc_file))
  temp_file = File.join("temp",base + ".xml")
  output_file = File.join("html", base + ".html")
  
  warn "generating #{temp_file}..."
  File.open(temp_file, 'w') do |output|
    HLight.main(File.new(doc_file), output, 'hlight/config.yaml')
  end

  warn "generating #{output_file}..."
  `xsltproc documentation.xsl #{temp_file} > #{output_file}`
end

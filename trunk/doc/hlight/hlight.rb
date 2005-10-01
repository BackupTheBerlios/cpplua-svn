require 'rexml/document'
require 'strscan'
require 'yaml'

module HLight
  extend self
  
  class Keyword
    attr_reader :name, :type
    def initialize(name, type)
      @name = name
      @type = type
    end
  end
  
  class HighlightRules
    class Context
      attr_reader :name, :attribute, :line_end_context
      def initialize(context_element)
        @name = context_element.attribute("name").value
        @attribute = context_element.attribute("attribute").value
        @line_end_context = context_element.attribute("lineEndContext").value
        @rules = []
      end
      
      def add_rule(rule)
        @rules << rule
      end
      
      def each_rule(&blk)
        @rules.each(&blk)
      end
      
      alias each each_rule
    end
    
    class ItemData
      attr_reader :style, :color, :bold, :italic
      def initialize(style, color = nil, bold = nil, italic = nil)
        @style = style
        @color = color
        @bold = (bold == "1")
        @italic = (italic == "1")
        warn "itemdata #{@style} initialized with color=#{@color}" if @color and $DEBUG
      end
    end
  
    attr_reader :contexts, :itemdata
    def initialize(file)
      @rules = REXML::Document.new(file).elements["language"].elements["highlighting"]
      
      # read keywords
      @keywords = {}
      @rules.each_element("list") do |list|
        type = list.attribute("name").value
        @keywords[type] = []
        list.each_element("item") do |item|
          @keywords[type] << Keyword.new(item.text.strip, type)
        end
  #      warn "keywords -> #{@keywords['keywords'].inspect}" if $DEBUG
      end
      
      # read contexts
      @contexts = {}
      @rules.elements["contexts"].each_element("context") do |context|
        name = context.attribute("name").value
        c = Context.new(context)
        @contexts[name] = c
        
        context.each_element do |rule|
          c.add_rule(Rule.new(rule, @keywords))
        end
      end
      
      # read itemdata
      @itemdata = {}
      @rules.elements["itemDatas"].each_element("itemData") do |item|
        id = ItemData.new(
          *%w(defStyleNum color bold italic).map do |attr_name|
            if attr = item.attribute(attr_name)
              attr.value
            end
          end
        )
        warn "adding itemdata #{item.attribute("name").value} -> #{id.inspect}" if $DEBUG
        @itemdata[item.attribute("name").value] = id 
      end
    end
  end
  
  module Rule
    def self.new(rule, *args)
      module_eval("Rule_"+rule.name).new(rule, *args)
    end
  
    class Rule
      attr_reader :attribute, :context, :size, :match
      def initialize(rule)
        @attribute = rule.attribute("attribute").value
        @context = rule.attribute("context").value
        warn "creating rule #{@attribute}, #{@context}" if $DEBUG
      end
      
      def scan(scanner, re)
        @match = scanner.scan(re)
        @size = scanner.matched_size        
        @match
      end
    end
    
    module RegExprScanner
      def setup(re)
        @re = re
      end
      
      def apply(scanner)
        scan(scanner, @re)
      end
    end
    
    class Rule_keyword < Rule
      include RegExprScanner
      def initialize(rule, keywords)
        super rule
        list = rule.attribute("String").value
        setup(/(#{keywords[list].map { |k| Regexp.escape(k.name) + '\b' }.join("|")})/)
      end
    end
    
    class Rule_RegExpr < Rule
      include RegExprScanner
      def initialize(rule, _)
        super rule
        setup(/#{rule.attribute("String")}/)
      end
    end
    
    class Rule_DetectChar < Rule
      include RegExprScanner
      def initialize(rule, _)
        super rule
        setup(/#{Regexp.escape(rule.attribute("char").value)}/)
      end
    end
    
    class Rule_AnyChar < Rule
      include RegExprScanner
      def initialize(rule, _)
        super rule
        chars = Regexp.escape(rule.attribute("String").value)
        setup(/[#{chars}]/)
      end
    end
    
    class Rule_StringDetect < Rule
      include RegExprScanner
      def initialize(rule, _)
        super rule
        setup(/#{Regexp.escape(rule.attribute("String").value)}/)
      end
    end
      
    class Rule_Detect2Chars < Rule
      include RegExprScanner
      def initialize(rule, _)
        super rule
        setup(/#{Regexp.escape(rule.attribute("char").value)}#{Regexp.escape(rule.attribute("char1").value)}/)
      end
    end
    
    class Rule_HlCChar < Rule
      include RegExprScanner
      def initialize(rule, _)
        super rule
        setup(/'.'/)
      end
    end
    
    class Rule_HlCStringChar < Rule
      include RegExprScanner
      def initialize(rule, _)
        super rule
        setup(/\\[abefnrtv"'?\\]/)
      end
    end
    
    class Rule_RangeDetect < Rule
      include RegExprScanner
      def initialize(rule, _)
        super rule
        chars = %w(char char1).each do |attribute|
          Regexp.escape(rule.attribute(attribute).value)
        end
        setup(/#{chars[0]}.*#{chars[1]}/)
      end
    end
    
    class Rule_LineContinue < Rule
      include RegExprScanner
      def initialize(rule, _)
        super rule
        setup(/\\$/)
      end
    end
  
    class Rule_Float < Rule
      include RegExprScanner
      def initialize(rule, _)
        super rule
        setup(/[0-9]*\.[0-9]+([eE][-+]?[0-9]+)?/)
      end
    end
    
    class Rule_HlCOct < Rule
      include RegExprScanner
      def initialize(rule, _)
        super rule
        setup(/0[0-7]+(L|U)?/)
      end
    end
    
    class Rule_HlCHex < Rule
      include RegExprScanner
      def initialize(rule, _)
        super rule
        setup(/0x[0-9a-fA-F]+(L|U)?/)
      end
    end
    
    class Rule_Int < Rule
      include RegExprScanner
      def initialize(rule, _)
        super rule
        setup(/[0-9]+/)
      end
    end
    
    class Rule_IncludeRules < Rule
      def initialize(*args)
      end
      
      def apply(scanner)
        #TODO
      end
    end
    
  end
  
  class HLMatchList
    class HLMatch
      attr_reader :first, :length, :attribute
      def initialize(first, length, attribute)
        @first = first
        @length = length
        @attribute = attribute
      end
      
      def last
        @first+@length
      end
      
      def redim(additional_length)
        @length += additional_length
      end
    end
    
    def initialize
      @matches = []
    end
    
    def add_match(start, length, attribute)
      last = @matches.last
      if last and last.last == start and last.attribute == attribute
        # these matches can be combined
        last.redim(length)
      else
        @matches << HLMatch.new(start, length, attribute)
      end
    end
    
    alias << add_match
    
    def each(&blk)
      @matches.each(&blk)
    end
  end
  
  class Highlighter
    def initialize(rules)
      @rules = rules
    end
    
    def highlight(str)
      @contexts = ["Normal"]
      matches = []
      str.each_line do |line|
        matches << match(line.chomp)
        update_context(@rules.contexts[current_context].line_end_context)
      end
      
      matches
    end
    
    def current_context
      @contexts.last
    end
    
    def match(line)
      scanner = StringScanner.new(line)
      matches = HLMatchList.new
      until scanner.eos?
        pointer = scanner.pointer
        matched = false
        @rules.contexts[current_context].each do |rule|
          if match = rule.apply(scanner)
            warn "rule matched #{match}: #{rule.attribute} #{rule.class.name}" if $DEBUG
            update_context(rule.context)
            matches.add_match(pointer, rule.size, rule.attribute)
            matched = true
            break
          end
        end
        
        unless matched
          # if no rule matches
          c = scanner.scan(/\w+(\b|\s)/)
          c ||= scanner.scan(/./)
          n = c.size
          warn "consuming '#{c}' (size = #{n})" if $DEBUG
          matches.add_match(pointer, n, @rules.contexts[current_context].attribute)
        end
      end
      
      matches
    end
    
    def update_context(context)
      if context[0,1] == "#"
        case context[1..-1]
        when "stay"
          warn "keeping current context #{current_context}" if $DEBUG
        when "pop"
          @contexts.pop
          warn "switching back to context #{current_context}" if $DEBUG
        end
      else
        @contexts.push(context)
        warn "switching to context #{context}" if $DEBUG
      end
    end
  end
  
  def usage
    warn "Usage: #$0 file"
    exit
  end
  
  class XMLCodeHighlighter
    def initialize(config)
      @default_language = config["default language"]
      @syntax_dir = config["syntax dir"]
      @custom_files = config["custom"]
    end
    
    def syntax_file(language)
      if base = @custom_files[language]
        File.join(File.dirname(__FILE__), base)
      else
        File.join(@syntax_dir, language + '.xml')
      end
    end
    
    def highlight!(document)
      rules = {}
      document.each_element("//code[@block=1]") do |code_block|
        lang_attr = code_block.attribute("lang")
        language = if lang_attr
          lang_attr.value
        else
          @default_language
        end
        
        rules[language] ||= HighlightRules.new(File.new(syntax_file(language)))
        hl_block = REXML::Element.new("code")
        hl_block.add_attributes({"lang" => language, "block" => "1"})
        code = code_block.text
        
        # perform highlighting
        hl = Highlighter.new(rules[language])
        data = hl.highlight(code)
        lines = code.split("\n")
        data.zip(lines).each do |match_list, line|
          match_list.each do |match|
            matched_text = line[match.first...match.last]
            itemdata = rules[language].itemdata[match.attribute]

            # preparing style clause
            style = []
            style << "color: #{itemdata.color}" if itemdata.color
            style << "font-weigth: bold" if itemdata.bold
            style << "font-style: italic" if itemdata.italic

            # nested function
            add_element = lambda do |name, attributes|
              hl_element = REXML::Element.new(name, hl_block)
              hl_element.add_attributes(attributes)
              hl_element.add_text(matched_text)
            end
            
            # setup element
            if style.empty?
              if itemdata.style == "dsNormal"
                hl_block.add_text(matched_text)
              else
                add_element["hl", {"class" => itemdata.style}]
              end
            else
              add_element["hl-custom", {"style" => style.join("; ")}]
            end

          end
          hl_block.add_text("\n")
        end
        
        code_block.parent.replace_child(code_block, hl_block)
      end
    end
  end
  
  def main(input, output, config_file = 'config.yaml')
    config = YAML.load(File.new(config_file))
    hl = XMLCodeHighlighter.new(config)
    hl.highlight!(doc = REXML::Document.new(input))
    output.puts doc
  end
end
  
if $0 == __FILE__
  file = ARGV.shift or usage
  HLight.main(File.new(file), $stdout)
end
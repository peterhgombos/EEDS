# A sample Guardfile
# More info at https://github.com/guard/guard#readme
#

require 'guard/plugin'

module ::Guard
    class Guardtex < ::Guard::Plugin
        def start
            puts "Starting Guardtex"
        end

        def run_all
            puts "Rebuilding project (Run all)"
            `make`
        end

        def run_on_changes(paths)
            puts "Rebuilding project (Changes)"
            `make`
        end
    end
end

guard 'guardtex' do
    watch(%r{.+\.tex})
end

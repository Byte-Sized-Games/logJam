# Compiler Variables
compiler := "clang++"
std := "c++23"
sources := "src/"
libraries := "inc"
linkFolder := "-L./lib"
links := "-l raylib"
flags := "-Wall -g"
dir := "bin"
target := "logger"
os := "linux"


# Compile & Link
@build: compile
    mkdir -p ./{{dir}}/out/
    echo -e "\nLinking Binaries..."
    {{compiler}} -std={{std}} {{dir}}/tmp/*.o {{flags}} {{linkFolder}}/{{os}} {{links}} -o {{dir}}/out/{{target}}
    echo "Binaries Linked!"
    echo -e "\n---------------------------------------------"
    just clean
    echo "Project: {{target}} successfully built to ./{{dir}}/out/{{target}}"

# Run code & clean up output - useful for testing
@run: build
    echo -e "\n- Running binary -\n"
    ./{{dir}}/target/{{target}}
    just clean

# Compile Code, do not link
@compile:
    mkdir -p ./{{dir}}/tmp
    echo -e "\n"
    echo "Building source files"
    echo "---------------------------------------------"
    for file in `ls -I "{{libraries}}" -I "*.hpp" {{sources}}`; do \
        echo "> building source file: $file"; \
        {{compiler}} -std={{std}} {{flags}} -c {{sources}}$file -o {{dir}}/tmp/"$file".o; \
    done

# Clean output
clean:
    @rm -rf {{dir}}/tmp/*

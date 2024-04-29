# Download, Compile & link source code files. The whole shebang
build:
    cmake . -B bin -G Ninja
    cmake --build bin

# Runs the code after calling the build command. Good for testing releases or quickly playing
run: build
    ./bin/steel-crown

# Runs the code after calling the build command, but also enables debug mode to determine errors, bugs and the like
debug: build
    ./bin/steel-crown debug

# Removes files from bin/ dir
clean:
    rm -rf bin/*

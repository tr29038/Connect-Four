# Connect-Four
# Requirements
CMake version 3.22+  
GNU g++ version 11.4.0+  

# Getting Started
Run the following commands:  
```
git clone https://github.com/tr29038/Connect-Four.git
cd ./Connect-Four
```

# Building
Run the following commands:  
```
cd ./build
./build.sh
make
```

# Running
Run the following command:  
```
./connect-four
```

For a list of options, run the command:  
```
./connect-four gibberish
```
Where [] indicates optional.  
If the difficulty is omitted, medium is selected by default.  
If pruning toggle is omitted, pruning is enabled by default.  
If which player goes first is omitted, the player goes first by default.  

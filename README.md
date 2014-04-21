# Raymini


Ray tracer project for SI350 class @ Télécom ParisTech


## Compiling


First of all, we will use [homebrew](http://brew.sh) as a package & dependency manager. If you don't have it, you can simply install it with the line 
    
    ruby -e "$(curl -fsSL https://raw.github.com/Homebrew/homebrew/go/install)"

The project has dependency on `qt` and `libqglviewer` : install both of those by typing `brew install qt` and `brew install libqglviewer`. You will need to remove previous versions of those libraries if you installed them any other way. Quick sanity check : `brew info qt` should tell you you have version `4.8.5`.

Finally, as the `libqglviewer` guys really aren't that clever, we need to execute the following commands:

    ln -s /usr/local/Cellar/libqglviewer/2.5.1/QGLViewer.framework/ /usr/local/lib/QGLViewer.framework
    install_name_tool -id /usr/local/lib/QGLViewer.framework/Versions/2/QGLViewer /usr/local/lib/QGLViewer.framework/QGLViewer 


The project has been correctly configured to work under those conditions. Loading the Xcode project and pressing `cmd+B` should successfully build the program : a `RayTracer` binary should appear in the `bin` folder at the root of the project.

## Running

The program hard-codes a bunch of path to model files and expects to be launched with the project directory as the working directory. 
Two solutions: you can navigate to the project directory (the directory containing the Xcode file) using your terminal and launch the program like that: `./bin/RayTracer`, which is easy but not really convenient.

Better yet; you can configure Xcode to set the custom working directory: in the upper-left corner, click on `RayTracer`, "Edit Scheme". 
![1](https://github.com/jcaille/Raymini/raw/master/doc/1.png)

Go to "Run" -> "Options" -> Check "Use custom working directory" and choose the project directory as custom working directory
![2](https://github.com/jcaille/Raymini/raw/master/doc/2.png)

Running the project (`cmd+R`) should bring up this Qt window :
![3](https://github.com/jcaille/Raymini/raw/master/doc/3.png)




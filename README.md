# Wheel and Pedals OpenCV
 The final project for the C++ Course at AGH UST

## :gear: Requirements and building

```git
git clone https://github.com/Goader/wheel_opencv.git
```

* C++ 14
* CMake 3.20.1
* OpenCV 4.5.2
* OpenCV-contrib

_To run the application use CMake._

**You must have 2 active cameras. I have used DroidCam and OBS Virtual Camera.**

### :eye: Installing OpenCV
As this application is designed for Windows operating system, there may appear problems with installing OpenCV package.

* download [OpenCV](https://github.com/opencv/opencv) and [OpenCV-contrib](https://github.com/opencv/opencv_contrib) source code
* download and install CMake with GUI
* start CMake, select folder with OpenCV source code and folder for the build
* click `Configure`
* after lots of variables appeared find `OPENCV_EXTRA_MODULES_PATH` and set it to the folder with OpenCV-contrib source code
* click `Configure` again
* find `BUILD_opencv_world` variable and check it (this will build one common library for all subpackages)
* click `Generate` and select your current version of Visual Studio Compiler
* open Visual Studio and compile `ALL_BUILD` with `Release` flag (this will boost performance)
* add `<your build folder>/install/x64/v16/bin` to `PATH` variable (notice that some numbers may differ depending on your version of OpenCV and compiler)
* create/modify an environment variable called `CMAKE_PREFIX_PATH` and add `<your build folder>/install`

After those steps, adding `find_package(OpenCV REQUIRED)` to your CMake file should work properly and OpenCV package should be seen in the project files.

## :control_knobs: Calibrating
All the parameters in the application must be set to interpret tracking results correctly. For example, without knowing where is the center of the wheel it will be really difficult to calculate the angle of your turn.

`ControllerTracker`'s `init` method is written to help you calibrate those parameters. There will pop out windows with the titles telling you what to do. Below is the example of correct calibrating.

### :film_strip: Example of calibrating parameters
![Calibrating example](readme/example-calibrating.gif)

There are 2 main phases:
* calibrating the wheel
    * clicking on the center of the wheel
    * clicking on the greatest possible angle as shown in the GIF above
    * setting the wheel in its initial position and selecting ROI (a rectangular around a point the algorithm should track), in other words selecting a top point of the wheel as in the GIF
* calibrating the pedals (for each do the steps below)
    * clicking on the highest possible position (which means the position in which it is not pressed on)
    * clicking on the lowest possible position (fully pressed on)
    * setting the pedal in its initial position and selecting ROI (as above, a rectangular around the point the algorithm should track) as in the example in the GIF above

As we see, after calibrating - 3 windows should pop out showing the current tracking state, FPS and name of the tracking algorithm.

After these 3 windows pop out and you can see that there are rectangulars around the tracked points you have marked before, then you can start playing. The program has been successfully calibrated and is already reacting to the changes on the wheel and the pedals.

Getting back to tracking algorithm, in our case, we use **CSRT**, but it's possible to set **MedianFlow** too. Others algorithms are not used, because after lots of testing they could not perform well for this particular problem.

Criteria for choosing an algorithm:
* **CSRT** - slow, but accurate. Works with low FPS, cannot handle tracking failure, needs more computing power
* **MedianFlow** - fast, great for predictable simple moves, but gets lost for very fast movement. Handles tracking failures and after it can find the tracking object again, easier to compute

Originally, this program needs 8 threads, so if you have less, you should consider choosing **MedianFlow** over **CSRT**.

The name of the algorithm, and the camera indices (a special index used by Windows to indicate a specific camera attached to the device) must be written in the [```config.json```](https://github.com/Goader/wheel_opencv/blob/main/config.json) file.

## :video_game: Gaming
Let's see how this program allows us to play games like **Grand Theft Auto V** without having real gaming wheel or pedals (actually, real gaming wheel and pedals are used to show this, but they are not plugged in, and I'm not even sure if they still work :smile:).

![Gaming example](readme/example-gaming.gif)
![Gaming example 2](readme/example-gaming2.gif)

You should be aware of the fact, that very fast movement makes it much easier for an algorithm to fail. After failure you must run this application again and calibrate everything again.
    
## :keyboard: Created by [@Goader](https://github.com/Goader)
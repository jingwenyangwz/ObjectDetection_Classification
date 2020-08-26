# ObjectDetection_Classification
To implement blob extraction and classification using the output of the MOG2 background subtraction module provided by OpenCV. • To detect stationary foreground blobs in video sequences.


- task1:
  • Implement blob extraction in C++  based on the sequential Grass-Fire algorithm
  • Implement blob removal to blobs that have less than a certain size.

- task2:
  • Implement in C/C++ language a blob classification routine based on the aspect ratio feature and the simple statistical classifier explained in class (Gaussian, with feature models based on mean and variance). −int classifyBlobs(std::vector &blobllist>); • The considered classes are defined in Blob.hpp using a typedef command named as CLASS (PERSON, CAR …). 




Implement in C/C++ language a routine for extracting stationary foreground pixels. 
−int extractStationatyFG(Mat fgmask, Mat &fgmask_history, Mat &sfgmask); 

• This routine should be based on the paper “Stationary foreground detection for video-surveillance based on foreground and motion history images” and implement the algorithm described in section 3.1.

 • In order to detect stationary foreground, also apply equations (9) and (12) without considering MHI or SHI variables 

• Test sequences are available in Moodle. Sample results must be reported for the majority of the sequences. 

• Static detection: Foreground analysis −Accumulation over time of background subtraction results

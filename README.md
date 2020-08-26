# ObjectDetection_Classification
To implement blob extraction and classification using the output of the MOG2 background subtraction module provided by OpenCV. • To detect stationary foreground blobs in video sequences.


- task1: BLOB EXTRACTION
  - Implement blob extraction in C++  based on the sequential Grass-Fire algorithm
  - Implement blob removal to blobs that have less than a certain size.

- task2: BLOB CLASSIFICATION
  - Implement in C++ language a blob classification algorithm based on the aspect ratio feature and the simple statistical classifier(Gaussian, with feature models based on mean and variance)
  - The considered classes are defined in Blob.hpp using a typedef command named as CLASS (PERSON, CAR …). 



- task3: STATIONARY ForeGround EXTRACTION
  - Implement in C++ language stationary foreground pixel extraction
    - based on the paper “Stationary foreground detection for video-surveillance based on foreground and motion history images” 
    - In order to detect stationary foreground, also apply equations (9) and (12) without considering MHI or SHI variables 
    - Static detection: Foreground analysis −Accumulation over time of background subtraction results
